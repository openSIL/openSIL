/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BrhAcpiLocality.c
 * @brief BRH DRAM address map and proximity domain lookups for the ACPI tables
 *
 * @details The register layout and quadrant maps are Breithorn specific. They
 *          reach the revision-independent SRAT, CRAT, and PCIe proximity-domain
 *          assembly through the Cmn2Rev transfer table.
 */

#include <SilCommon.h>
#include <string.h>
#include <DF/Df.h>
#include <DF/DfIp2Ip.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <DF/DfX/BRH/SilFabricRegistersBrh.h>
#include <DF/DfX/BRH/SilFabricInfoBrh.h>
#include "DfInitBrh.h"

/// Each CS carries four DRAM map register groups
#define CS_DRAM_MAP_GROUPS  4

/// The VGA hole, in units of 64KB
#define VGA_HOLE_BASE   0xA
#define VGA_HOLE_LIMIT  0xC

/// Top of the 32-bit address space, in units of 64KB
#define BELOW_4GB_LIMIT  0x10000

/// Base of the 1TB remap window, in units of 64KB
#define ONE_TB_BASE  0x1000000

/**
 * Quadrant each physical CCD belongs to
 */
static const uint32_t BrhCcdToQuadrant[] = {0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2};

/**
 * Proximity domain of each IOS, indexed by NPS then by system wide IOS number.
 * Row order is NPS0, NPS1, NPS2, NPS4.
 */
static const uint32_t BrhCsDomainTable_Bx[MAX_NPS][SIL_RESERVED_0216 * PROJ_MAX_SOCKETS_SUPPORTED] = {
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 0, 1, 0, 1, 0, 1,  2, 3, 2, 3, 2, 3, 2, 3},
  {0, 3, 0, 3, 1, 2, 1, 2,  4, 7, 4, 7, 5, 6, 5, 6}
};

/**
 * CCD bitmap reachable from each IOS, one table per achievable domain count
 */
static const uint32_t BrhCcxMaskDomainTable4Domain[SIL_RESERVED_0216] = {
  0x1111, 0x2222, 0x1111, 0x2222, 0x4444, 0x8888, 0x4444, 0x8888
};

static const uint32_t BrhCcxMaskDomainTable2Domain[SIL_RESERVED_0216] = {
  0x5555, 0x5555, 0x5555, 0x5555, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA
};

static const uint32_t BrhCcxMaskDomainTable1Domain[SIL_RESERVED_0216] = {
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

/**
 * BrhGetNumberOfDramRegions
 *
 * @brief Number of DRAM address map ranges the fabric implements.
 *
 * @retval uint32_t  Range count
 */
uint32_t
BrhGetNumberOfDramRegions (void)
{
  return SIL_RESERVED_0207;
}

/**
 * BrhCollectDramMap
 *
 * @brief Read the fabric DRAM address map and the windows it hoists over.
 *
 * @details Runs entirely off register state, so it reflects whatever ABL
 *          programmed at the moment it is called rather than a cached copy.
 *
 * @param[in]  OneTbRemapEnabled  True when the platform remaps around 1TB, in
 *                                which case the gap below it is not hoisted
 * @param[in]  MaxRegions         Capacity of Regions
 * @param[out] Regions            Receives one entry per DRAM range
 * @param[out] HoistCount         Hoisted windows written
 * @param[out] HoistRegions       Receives up to DF_MAX_HOIST_REGIONS windows
 *
 * @retval SilPass                Map collected
 * @retval SilInvalidParameter    NULL pointer passed
 * @retval SilOutOfBounds         MaxRegions smaller than the implemented count
 */
SIL_STATUS
BrhCollectDramMap (
  bool             OneTbRemapEnabled,
  uint32_t         MaxRegions,
  DF_DRAM_REGION   *Regions,
  uint32_t         *HoistCount,
  DF_HOIST_REGION  *HoistRegions
  )
{
  uint32_t                     DramMapIndex;
  uint32_t                     SocketIndex;
  uint32_t                     CsIndex;
  uint32_t                     Group;
  uint32_t                     NumberOfHoisted;
  uint32_t                     PreviousLimit;
  bool                         OneTbHoisted;
  VGAEN_REGISTER               VgaEn;
  DRAM_HOLE_CONTROL_REGISTER   DramHoleCtrl;
  DRAM_ADDRESS_CTL_REGISTER    DramAddressCtl;
  DRAM_ADDRESS_CTL_REGISTER    CsDramAddressCtl;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr;
  DRAM_LIMIT_ADDRESS_REGISTER  DramLimitAddr;
  DF_IP2IP_API                 *DfApi;
  SIL_STATUS                   Status;

  if ((Regions == NULL) || (HoistCount == NULL) || (HoistRegions == NULL)) {
    return SilInvalidParameter;
  }

  if (MaxRegions < SIL_RESERVED_0207) {
    return SilOutOfBounds;
  }

  Status = SilGetIp2IpApi (SilId_DfClass, (void **) &DfApi);
  if ((Status != SilPass) || (DfApi == NULL)) {
    assert (Status == SilPass);
    return SilUnsupported;
  }

  memset ((void *) Regions, 0x00, MaxRegions * sizeof (DF_DRAM_REGION));
  memset ((void *) HoistRegions, 0x00, DF_MAX_HOIST_REGIONS * sizeof (DF_HOIST_REGION));
  NumberOfHoisted = 0;
  PreviousLimit = 0;

  VgaEn.Value = DfApi->DfFabricRegisterAccRead (0, 0, VGAEN_FUNC, VGAEN_REG, FABRIC_REG_ACC_BC);
  if (VgaEn.Field.VgaEn_VE == 1) {
    HoistRegions[NumberOfHoisted].MemoryLost = true;
    HoistRegions[NumberOfHoisted].Base       = VGA_HOLE_BASE;
    HoistRegions[NumberOfHoisted].Limit      = VGA_HOLE_LIMIT;
    NumberOfHoisted++;
  }

  DramHoleCtrl.Value = DfApi->DfFabricRegisterAccRead (0,
    0,
    DRAMHOLECONTROL_FUNC,
    DRAMHOLECONTROL_REG,
    FABRIC_REG_ACC_BC
    );
  if (DramHoleCtrl.Field.DramHoleValid == 1) {
    HoistRegions[NumberOfHoisted].MemoryLost = false;
    HoistRegions[NumberOfHoisted].Base       = (DramHoleCtrl.Field.DramHoleBase << 8);
    HoistRegions[NumberOfHoisted].Limit      = BELOW_4GB_LIMIT;
    NumberOfHoisted++;
  }

  OneTbHoisted = OneTbRemapEnabled ? false : true;

  for (DramMapIndex = 0; DramMapIndex < SIL_RESERVED_0207; DramMapIndex++) {
    DramAddressCtl.Value = DfApi->DfFabricRegisterAccRead (0,
      0,
      DRAMADDRESSCTL_0_FUNC,
      DRAMADDRESSCTL_0_REG + (DramMapIndex * SIL_RESERVED_0176),
      SIL_RESERVED_0185
      );
    if (DramAddressCtl.Field.AddrRngVal != 1) {
      continue;
    }

    DramBaseAddr.Value = DfApi->DfFabricRegisterAccRead (0,
      0,
      DRAMBASEADDRESS_0_FUNC,
      DRAMBASEADDRESS_0_REG + (DramMapIndex * SIL_RESERVED_0176),
      SIL_RESERVED_0185
      );
    DramLimitAddr.Value = DfApi->DfFabricRegisterAccRead (0,
      0,
      DRAMLIMITADDRESS_0_FUNC,
      DRAMLIMITADDRESS_0_REG + (DramMapIndex * SIL_RESERVED_0176),
      SIL_RESERVED_0185
      );

    //
    // The range register says which fabric ID owns the range but not which CSs
    // it interleaves across, so walk every CS looking for a group programmed
    // with the same destination and remap settings.
    //
    Regions[DramMapIndex].NormalizedMap = 0;
    for (SocketIndex = 0; SocketIndex < DfApi->DfGetNumberOfProcessorsPresent (); SocketIndex++) {
      for (CsIndex = 0; CsIndex < SIL_RESERVED_0211; CsIndex++) {
        for (Group = 0; Group < CS_DRAM_MAP_GROUPS; Group++) {
          CsDramAddressCtl.Value = DfApi->DfFabricRegisterAccRead (SocketIndex,
            0,
            DRAMADDRESSCTL_0_FUNC,
            DRAMADDRESSCTL_0_REG + (Group * SIL_RESERVED_0176),
            SIL_RESERVED_0144 + CsIndex
            );
          if ((CsDramAddressCtl.Field.AddrRngVal  == 1) &&
              (CsDramAddressCtl.Field.DstFabricID == DramAddressCtl.Field.DstFabricID) &&
              (CsDramAddressCtl.Field.RemapEn     == DramAddressCtl.Field.RemapEn) &&
              (CsDramAddressCtl.Field.RemapSel    == DramAddressCtl.Field.RemapSel)) {
            Regions[DramMapIndex].NormalizedMap |=
              (1u << CsIndex) << (NORMALIZED_SOCKET_SHIFT * SocketIndex);
            break;
          }
        }
      }
    }

    Regions[DramMapIndex].RawBase  = DramBaseAddr.Field.DramBaseAddr << 12;
    Regions[DramMapIndex].RawLimit = ((DramLimitAddr.Field.DramLimitAddr << 12) | 0xFFF) + 1;
    Regions[DramMapIndex].RawSize  = Regions[DramMapIndex].RawLimit - Regions[DramMapIndex].RawBase;

    if (DramAddressCtl.Field.LgcyMmioHoleEn == 1) {
      assert (DramHoleCtrl.Field.DramHoleValid == 1);
      Regions[DramMapIndex].RawSize -= (BELOW_4GB_LIMIT - (DramHoleCtrl.Field.DramHoleBase << 8));
    }

    //
    // A range starting exactly at 1TB with a gap below it means the map was
    // hoisted over the 1TB window rather than remapped around it.
    //
    if ((Regions[DramMapIndex].RawBase == ONE_TB_BASE) &&
        (PreviousLimit != ONE_TB_BASE) &&
        !OneTbHoisted &&
        (NumberOfHoisted < DF_MAX_HOIST_REGIONS)) {
      HoistRegions[NumberOfHoisted].MemoryLost = false;
      HoistRegions[NumberOfHoisted].Base       = PreviousLimit;
      HoistRegions[NumberOfHoisted].Limit      = ONE_TB_BASE;
      NumberOfHoisted++;
      OneTbHoisted = true;
      DF_TRACEPOINT (SIL_TRACE_INFO, "DRAM map hoisted over the 1TB window\n");
    }
    PreviousLimit = Regions[DramMapIndex].RawLimit;
  }

  *HoistCount = NumberOfHoisted;
  return SilPass;
}

/**
 * BrhNpsToTableRow
 *
 * @brief Map a nodes-per-socket count onto its row in BrhCsDomainTable_Bx.
 *
 * @param[in] PhysNps  0, 1, 2 or 4
 *
 * @retval uint32_t    Row index 0 through 3
 */
static
uint32_t
BrhNpsToTableRow (
  uint32_t  PhysNps
  )
{
  switch (PhysNps) {
  case 1:  return 1;
  case 2:  return 2;
  case 4:  return 3;
  default: return 0;
  }
}

/**
 * BrhGetMaxAllowableNpsForCcxAsNuma
 *
 * @brief Largest NPS the populated CCD quadrants can express as NUMA domains.
 *
 * @param[in] DomainInfo  Domain info block holding the CCD map
 *
 * @retval uint32_t       4, 2 or 1
 */
static
uint32_t
BrhGetMaxAllowableNpsForCcxAsNuma (
  DF_DOMAIN_INFO_BLK  *DomainInfo,
  uint32_t            Sockets
  )
{
  uint32_t  CcdQuadrantMap[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t  SocketLoop;
  uint32_t  CcdLoop;
  bool      Valid;

  for (SocketLoop = 0; SocketLoop < Sockets; SocketLoop++) {
    CcdQuadrantMap[SocketLoop] = 0;
    for (CcdLoop = 0; CcdLoop < DomainInfo->CcdCount[SocketLoop]; CcdLoop++) {
      CcdQuadrantMap[SocketLoop] |=
        (1u << BrhCcdToQuadrant[DomainInfo->LogToPhysCcd[SocketLoop][CcdLoop]]);
    }
  }

  // Every quadrant populated supports four domains per socket
  Valid = true;
  for (SocketLoop = 0; SocketLoop < Sockets; SocketLoop++) {
    if ((CcdQuadrantMap[SocketLoop] & 0xF) != 0xF) {
      Valid = false;
      break;
    }
  }
  if (Valid) {
    return 4;
  }

  // Both halves populated supports two domains per socket
  Valid = true;
  for (SocketLoop = 0; SocketLoop < Sockets; SocketLoop++) {
    if (((CcdQuadrantMap[SocketLoop] & 0x3) == 0) || ((CcdQuadrantMap[SocketLoop] & 0xC) == 0)) {
      Valid = false;
      break;
    }
  }
  if (Valid) {
    return 2;
  }

  return 1;
}

/**
 * BrhGetPxmDomains
 *
 * @brief Proximity domains reachable behind one host bridge.
 *
 * @details Answers only for bridges the fabric owns. A caller that gets a zero
 *          count is expected to fall back to its own knowledge, which is how
 *          CXL attached bridges are resolved.
 *
 * @param[in]  BusBase   Segment adjusted base bus number of the host bridge
 * @param[in]  MaxCount  Capacity of Domains
 * @param[out] Count     Domains written
 * @param[out] Domains   Receives the proximity domain numbers
 *
 * @retval SilPass       Lookup completed
 */
SIL_STATUS
BrhGetPxmDomains (
  uint32_t  BusBase,
  uint32_t  MaxCount,
  uint32_t  *Count,
  uint32_t  *Domains
  )
{
  DF_DOMAIN_INFO_BLK  *DomainInfo;
  DF_IP2IP_API        *DfApi;
  const uint32_t      *CcxMaskDomainTable;
  SIL_STATUS          Status;
  uint32_t            Sockets;
  uint32_t            Socket;
  uint32_t            Bridge;
  uint32_t            HostBridges;
  uint32_t            IosIndex;
  uint32_t            NodesPerSocket;
  uint32_t            Ccd;
  uint32_t            Ccx;

  if ((Count == NULL) || (Domains == NULL)) {
    return SilInvalidParameter;
  }

  *Count = 0;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **) &DfApi);
  if ((Status != SilPass) || (DfApi == NULL)) {
    assert (Status == SilPass);
    return SilUnsupported;
  }

  DomainInfo = (DF_DOMAIN_INFO_BLK *) xUslFindStructure (SilId_DfClass, DF_DOMAIN_INSTANCE);
  if ((DomainInfo == NULL) || !DomainInfo->DomainInfoValid) {
    return SilUnsupported;
  }

  Sockets = DfApi->DfGetNumberOfProcessorsPresent ();
  if (Sockets > PROJ_MAX_SOCKETS_SUPPORTED) {
    return SilOutOfBounds;
  }

  switch (BrhGetMaxAllowableNpsForCcxAsNuma (DomainInfo, Sockets)) {
  case 4:
    CcxMaskDomainTable = BrhCcxMaskDomainTable4Domain;
    break;
  case 2:
    CcxMaskDomainTable = BrhCcxMaskDomainTable2Domain;
    break;
  default:
    CcxMaskDomainTable = BrhCcxMaskDomainTable1Domain;
    break;
  }

  for (Socket = 0; Socket < Sockets; Socket++) {
    HostBridges = DfApi->DfGetNumberOfRootBridgesOnSocket (Socket);
    for (Bridge = 0; Bridge < HostBridges; Bridge++) {
      if (DfApi->DfGetHostBridgeBusBase (Socket, 0, Bridge) != BusBase) {
        continue;
      }

      IosIndex = DfApi->DfGetPhysRootBridgeNumber (Socket, 0, Bridge);
      if (IosIndex >= SIL_RESERVED_0216) {
        assert (IosIndex < SIL_RESERVED_0216);
        return SilOutOfBounds;
      }

      if (!DomainInfo->CcxAsNuma) {
        if (MaxCount < 1) {
          return SilOutOfBounds;
        }
        Domains[0] = BrhCsDomainTable_Bx[BrhNpsToTableRow (DomainInfo->PhysNps)]
                                        [IosIndex + (Socket * SIL_RESERVED_0216)];
        *Count = 1;
        return SilPass;
      }

      //
      // With every complex its own domain a quadrant can back several domains,
      // so return all of them and let the caller pick.
      //
      NodesPerSocket = DomainInfo->CcdCount[Socket] * DomainInfo->CcxPerCcd[Socket];
      for (Ccd = 0; Ccd < DomainInfo->CcdCount[Socket]; Ccd++) {
        for (Ccx = 0; Ccx < DomainInfo->CcxPerCcd[Socket]; Ccx++) {
          if (((1u << Ccx) << (DomainInfo->LogToPhysCcd[Socket][Ccd] * DomainInfo->CcxPerCcd[Socket])
               & CcxMaskDomainTable[IosIndex]) == 0) {
            continue;
          }
          if (*Count >= MaxCount) {
            return SilOutOfBounds;
          }
          Domains[*Count] = (Ccd * DomainInfo->CcxPerCcd[Socket]) + Ccx + (Socket * NodesPerSocket);
          (*Count)++;
        }
      }
      return SilPass;
    }
  }

  return SilPass;
}
