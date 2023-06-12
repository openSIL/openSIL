/**
 * @file FabricResourceManager.c
 * @brief Resource manager reserve functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/DfIp2Ip.h>
#include <RcMgr/FabricResourceManager.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include "RcManager4-api.h"
#include <RcMgr/RcMgrIp2Ip.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include "DfXFabricRegisterAcc.h"

void
FabricGetRemainingSizeForThisRegion (
  FABRIC_MMIO_REGION *MmioRegion,
  uint64_t           *Size,
  uint64_t           Alignment,
  uint8_t            MmioType
  );

uint64_t
FabricGetBaseAddress (
  FABRIC_MMIO_REGION *MmioRegion,
  uint64_t           Length,
  uint64_t           Alignment,
  uint8_t            MmioType
  );

/**
 * FabricReserveMmio
 *
 * @brief Program MMIO base/limit/control registers
 *
 * @param[in, out]    BaseAddress        Starting address of the requested MMIO range.
 * @param[in, out]    Length             Length of the requested MMIO range.
 * @param[in]         Alignment          Alignment bit map.
 * @param[in]         Target             PCI bus number/RootBridge number of the requestor.
 * @param[in, out]    Attributes         Attributes of the requested MMIO range indicating whether
 *                                       it is readable/writable/non-posted
 *
 * @retval   SIL_STATUS  SilOutOfResources    - The requested range could not be added because there are not
 *                                              enough mapping resources.
 *                       SilAborted           - One or more input parameters are invalid. For example, the
 *                                              PciBusNumber does not correspond to any device in the system.
 *                       SilPass              - Success to get an MMIO region
 */
SIL_STATUS
FabricReserveMmio (
  uint64_t              *BaseAddress,
  uint64_t              *Length,
  uint64_t              Alignment,
  FABRIC_TARGET         Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  )
{
  uint8_t                   i;
  uint8_t                   j;
  uint8_t                   Socket;
  uint8_t                   Rb;
  uint8_t                   TempSocket;
  uint8_t                   TempRb;
  uint32_t                  DstFabricID;
  uint32_t                  CfgAddrMapIndex;
  uint64_t                  SizeA;
  uint64_t                  SizeB;
  uint64_t                  TempSize;
  uint32_t                  SocketCount;
  uint32_t                  RbPerDieCount;
  uint32_t                  RbPerSktCount;
  uint32_t                  PrimarySocket;
  uint32_t                  PrimaryRootBridge;
  CFG_LIMIT_ADDRESS_REGISTER CfgLimit;
  CFG_BASE_ADDRESS_REGISTER CfgBase;
  DFX_FABRIC_MMIO_MANAGER   *FabricMmioManager;
  FABRIC_MMIO_REGION        *MmioRegion;
  FABRIC_MMIO_REGION        *PrimaryRb2ndMmioRegion;
  SIL_STATUS                ReturnStatus;
  DFX_RCMGR_INPUT_BLK       *RcMgrData;
  DF_IP2IP_API*             DfIp2IpApi;

  DF_TRACEPOINT (SIL_TRACE_INFO, "  openSIL FabricReserveMmio\n");

  ReturnStatus = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (ReturnStatus == SilPass);

  RcMgrData = (DFX_RCMGR_INPUT_BLK *)SilFindStructure(SilId_RcManager,  0);
  // assert (RcMgrData);
  if (RcMgrData == NULL) {
    return SilNotFound; // Could not find the IP input block
  }

  *BaseAddress = 0;
  ReturnStatus = SilPass;
  FabricMmioManager = &RcMgrData->MmioRcMgr;

  SocketCount = DfIp2IpApi->DfGetNumberOfProcessorsPresent ();
  RbPerDieCount = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie (0);
  RbPerSktCount = DfIp2IpApi->DfGetNumberOfRootBridgesOnSocket (0);
  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

  // Check input parameters
  if (*Length == 0) {
    return SilPass;
  }

  if ((Attributes->MmioType != NON_PCI_DEVICE_BELOW_4G) &&
      (Attributes->MmioType != NON_PCI_DEVICE_ABOVE_4G)) {
    DF_TRACEPOINT (SIL_TRACE_WARNING,
      "  This function only support Attributes->MmioType = NON_PCI_DEVICE_BELOW_4G or NON_PCI_DEVICE_ABOVE_4G\n");
    return SilAborted;
  }

  // Find out Socket/Rb
  DstFabricID = 0xFFFFFFFF;
  Socket = 0xFF;
  Rb = 0xFF;
  if (Target.TgtType == TARGET_PCI_BUS) {
    for (CfgAddrMapIndex = 0; CfgAddrMapIndex < DFX_NUMBER_OF_BUS_REGIONS; CfgAddrMapIndex++) {
      CfgLimit.Value = DfIp2IpApi->DfFabricRegisterAccRead (0, CFGLIMITADDRESS_0_FUNC,
        (uint32_t)(CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG))),
        FABRIC_REG_ACC_BC);
      CfgBase.Value =  DfIp2IpApi->DfFabricRegisterAccRead (0, CFGBASEADDRESS_0_FUNC,
        (uint32_t)(CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG))),
        FABRIC_REG_ACC_BC);
      if ((CfgBase.Field.RE == 1) && (CfgBase.Field.WE == 1) && (CfgLimit.Field.BusNumLimit >= Target.PciBusNum) &&
          (CfgBase.Field.BusNumBase <= Target.PciBusNum) && (CfgBase.Field.SegmentNum == Target.PciSegNum)) {
        DstFabricID = CfgLimit.Field.DstFabricID;
        break;
      }
    }

    if (CfgAddrMapIndex >= DFX_NUMBER_OF_BUS_REGIONS) {
      return SilAborted;
    }
    assert (DstFabricID != 0xFFFFFFFF);
    for (i = 0; i < SocketCount; i++) {
      for (j = 0; j < RbPerSktCount; j++) {
        if (DfIp2IpApi->DfGetHostBridgeSystemFabricID (i, j % RbPerDieCount) == DstFabricID) {
          Socket = i;
          Rb = j;
          break;
        }
      }
    }
    assert (Socket != 0xFF);
    assert (Rb != 0xFF);
  } else {
    Socket = (uint8_t) (Target.SocketNum);
    Rb     = (uint8_t) (Target.RbNum);
    // This is for combo support for multi/single NBIO in one IOD
    if (Rb >= RbPerSktCount) {
      Rb = (uint8_t) (RbPerSktCount - 1);
    }
  }

  if ((Socket >= MAX_SOCKETS_SUPPORTED) || (Rb >= DFX_MAX_RBS_PER_SOCKET)) {
    assert (false);
    return SilAborted;
  }

  SizeA = 0;
  SizeB = 0;
  PrimaryRb2ndMmioRegion = NULL;
  if ((Attributes->MmioType == MMIO_BELOW_4G) ||
      (Attributes->MmioType == P_MMIO_BELOW_4G) ||
      (Attributes->MmioType == NON_PCI_DEVICE_BELOW_4G)) {
    //
    // Below 4G
    //

    // return SilAborted if no MMIO on this Rb
    if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[Socket][Rb] == false) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, "  ERROR: No below 4G MMIO on Socket %X Rb %X\n", Socket, Rb);
      return SilAborted;
    }

    // Find out MmioRegion for this Rb
    MmioRegion = &FabricMmioManager->MmioRegionBelow4G[Socket][Rb];
    FabricGetRemainingSizeForThisRegion (MmioRegion, &SizeA, Alignment, Attributes->MmioType);
    if (FabricMmioManager->PrimaryRbHas2ndMmioBelow4G && (Socket == PrimarySocket) && (Rb == PrimaryRootBridge)) {
      // primary RootBridge may have 2nd MmioRegion
      TempSocket = (FabricMmioManager->PrimaryRb2ndMmioPairBelow4G >> 4) & 0xF;
      TempRb = FabricMmioManager->PrimaryRb2ndMmioPairBelow4G & 0xF;
      PrimaryRb2ndMmioRegion = &FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb];
      FabricGetRemainingSizeForThisRegion (PrimaryRb2ndMmioRegion, &SizeB, Alignment, Attributes->MmioType);
      if ((SizeA > SizeB) && (SizeB != 0)) {
        // Make sure MmioRegion & SizeA points to the smaller one
        // PrimaryRb2ndMmioRegion & SizeB points to the larger one
        MmioRegion = PrimaryRb2ndMmioRegion;
        PrimaryRb2ndMmioRegion = &FabricMmioManager->MmioRegionBelow4G[Socket][Rb];

        TempSize = SizeA;
        SizeA = SizeB;
        SizeB = TempSize;
      }
    }
    // Try to reserve MMIO space from the smaller one first
    if (SizeA >= (*Length)) {
      *BaseAddress = FabricGetBaseAddress (MmioRegion, *Length, Alignment, Attributes->MmioType);
    } else if (SizeB >= (*Length)) {
      *BaseAddress = FabricGetBaseAddress (PrimaryRb2ndMmioRegion, *Length, Alignment, Attributes->MmioType);
    }
  } else if ((Attributes->MmioType == MMIO_ABOVE_4G) ||
             (Attributes->MmioType == P_MMIO_ABOVE_4G) ||
             (Attributes->MmioType == NON_PCI_DEVICE_ABOVE_4G)) {
    //
    // Above 4G
    //
    // return SilAborted if no MMIO on this Rb
    if (FabricMmioManager->AllocateMmioAbove4GOnThisRb[Socket][Rb] == false) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, "  ERROR: No above 4G MMIO on Socket %X Rb %X\n", Socket, Rb);
      return SilAborted;
    }

    MmioRegion = &FabricMmioManager->MmioRegionAbove4G[Socket][Rb];
    FabricGetRemainingSizeForThisRegion (MmioRegion, &SizeA, Alignment, Attributes->MmioType);
    if (SizeA >= (*Length)) {
      // Have enough space
      *BaseAddress = FabricGetBaseAddress (MmioRegion, *Length, Alignment, Attributes->MmioType);
    }
  }

  if (*BaseAddress == 0) {
    *Length = (SizeA > SizeB) ? SizeA : SizeB;
    *BaseAddress = 0;
    ReturnStatus = SilOutOfResources;
  }

  if (ReturnStatus == SilPass) {
    Attributes->NonPosted = 0;
    Attributes->ReadEnable = 1;
    Attributes->WriteEnable = 1;
    Attributes->CpuDis = 0;
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Reserve MMIO from 0x%llX ~ 0x%llX\n",
        *BaseAddress, (*BaseAddress + *Length - 1));
    // Check if we need to report it to GCD
    //if (FabricMmioManager->ReportToGcd) {
    //  ReportMmioToGcd (BaseAddress, *Length, FabricMmioManager);
    //}
  } else if (ReturnStatus == SilOutOfResources) {
    DF_TRACEPOINT (SIL_TRACE_INFO, "  No enough space, the biggest MMIO size is 0x%X0000\n",
      (uint32_t) (*Length >> 16));
    // FabricReserveMmio is restricted for NON_PCI_DEVICE_BELOW_4G & NON_PCI_DEVICE_ABOVE_4G only
    // So if there's no enough space, ASSERT here
    if (Attributes->MmioType == NON_PCI_DEVICE_BELOW_4G) {
      DF_TRACEPOINT (SIL_TRACE_INFO, "  Please increase MmioSizePerRbForNonPciDevice\n");
      assert (false);
    }
    if (Attributes->MmioType == NON_PCI_DEVICE_ABOVE_4G) {
      DF_TRACEPOINT (SIL_TRACE_INFO, "  Please increase Above4GMmioSizePerRbForNonPciDevice\n");
      assert (false);
    }
  }

  return ReturnStatus;
}

/**
 * FabricGetRemainingSizeForThisRegion
 *
 * @brief Sub-routine of FabricAllocateMmio
 * @details Get remaining size of one MMIO region
 *
 * @param[in]         MmioRegion                  Point to a data structure which contains MMIO space information
 * @param[in]         Size                        Remaining size
 * @param[in]         Alignment                   Alignment bit map
 * @param[in]         MmioType                    Mmio type
 *
 */
void
FabricGetRemainingSizeForThisRegion (
  FABRIC_MMIO_REGION *MmioRegion,
  uint64_t           *Size,
  uint64_t           Alignment,
  uint8_t            MmioType
  )
{
  uint64_t Base;
  uint64_t BaseAligned;
  uint64_t AlignMask;

  Base = 0;

  if ((MmioType == MMIO_BELOW_4G) || (MmioType == MMIO_ABOVE_4G)) {
    Base = MmioRegion->BaseNonPrefetch + MmioRegion->UsedSizeNonPrefetch;
    *Size = MmioRegion->SizeNonPrefetch - MmioRegion->UsedSizeNonPrefetch;
  } else if ((MmioType == P_MMIO_BELOW_4G) || (MmioType == P_MMIO_ABOVE_4G)) {
    Base = MmioRegion->BasePrefetch + MmioRegion->UsedSizePrefetch;
    *Size = MmioRegion->SizePrefetch - MmioRegion->UsedSizePrefetch;
  } else if ((MmioType == NON_PCI_DEVICE_BELOW_4G) || (MmioType == NON_PCI_DEVICE_ABOVE_4G)) {
    Base = MmioRegion->BaseNonPci + MmioRegion->UsedSizeNonPci;
    *Size = MmioRegion->SizeNonPci - MmioRegion->UsedSizeNonPci;
  } else {
    assert (false);
    *Size = 0;
  }

  AlignMask = Alignment;
  BaseAligned = (Base + AlignMask) & (~AlignMask);
  if ((*Size) >= (BaseAligned - Base)) {
    *Size -= BaseAligned - Base;
  } else {
    *Size = 0;
  }
}

/**
 * FabricGetBaseAddress
 *
 * @brief Sub-routine of FabricAllocateMmio
 * @details Get base address of available MMIO space, and assign it to caller
 *
 * @param[in]         MmioRegion                  Point to a data structure which contains MMIO space information
 * @param[in]         Length                      Length of MMIO region which is required by user
 * @param[in]         Alignment                   Alignment bit map
 * @param[in]         MmioType                    Mmio type
 *
 */
uint64_t
FabricGetBaseAddress (
  FABRIC_MMIO_REGION *MmioRegion,
  uint64_t           Length,
  uint64_t           Alignment,
  uint8_t            MmioType
  )
{
  uint64_t MmioBaseAddress;
  uint64_t MmioBaseAddressAligned;
  uint64_t MmioRemainingSize;
  uint64_t AlignMask;

  MmioBaseAddress = 0;
  MmioBaseAddressAligned = 0;
  MmioRemainingSize = 0;

  if ((MmioType == MMIO_BELOW_4G) || (MmioType == MMIO_ABOVE_4G)) {
    MmioBaseAddress = MmioRegion->BaseNonPrefetch + MmioRegion->UsedSizeNonPrefetch;
    MmioRemainingSize = MmioRegion->SizeNonPrefetch - MmioRegion->UsedSizeNonPrefetch;
  } else if ((MmioType == P_MMIO_BELOW_4G) || (MmioType == P_MMIO_ABOVE_4G)) {
    MmioBaseAddress = MmioRegion->BasePrefetch + MmioRegion->UsedSizePrefetch;
    MmioRemainingSize = MmioRegion->SizePrefetch - MmioRegion->UsedSizePrefetch;
  } else if ((MmioType == NON_PCI_DEVICE_BELOW_4G) || (MmioType == NON_PCI_DEVICE_ABOVE_4G)) {
    MmioBaseAddress = MmioRegion->BaseNonPci + MmioRegion->UsedSizeNonPci;
    MmioRemainingSize = MmioRegion->SizeNonPci - MmioRegion->UsedSizeNonPci;
  }

  if ((MmioBaseAddress != 0) && (MmioRemainingSize != 0)) {
    AlignMask = Alignment;
    MmioBaseAddressAligned = (MmioBaseAddress + AlignMask) & (~AlignMask);
    if (MmioRemainingSize >= (Length + MmioBaseAddressAligned - MmioBaseAddress)) {
      if ((MmioType == MMIO_BELOW_4G) || (MmioType == MMIO_ABOVE_4G)) {
        MmioRegion->UsedSizeNonPrefetch += Length + MmioBaseAddressAligned - MmioBaseAddress;
      } else if ((MmioType == P_MMIO_BELOW_4G) || (MmioType == P_MMIO_ABOVE_4G)) {
        MmioRegion->UsedSizePrefetch += Length + MmioBaseAddressAligned - MmioBaseAddress;
      } else if ((MmioType == NON_PCI_DEVICE_BELOW_4G) || (MmioType == NON_PCI_DEVICE_ABOVE_4G)) {
        MmioRegion->UsedSizeNonPci += Length + MmioBaseAddressAligned - MmioBaseAddress;
      }
    } else {
      MmioBaseAddressAligned = 0;
    }
  }

  return MmioBaseAddressAligned;
}

