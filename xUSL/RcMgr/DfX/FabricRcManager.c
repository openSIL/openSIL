/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FabricRcManagerBrh.c
 * @brief Resource manager reserve functions for BRH
 */

#include <xSIM.h>
#include <DF/DfIp2Ip.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <RcMgr/DfX/RcManager4-api.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <MsrReg.h>
#include <SilSocLogicalId.h>
#include <RcMgr/Common/RcMgrCmn2Rev.h>
#include <ProjSocConst.h>
#include <RcMgr/DfX/FabricRcManagerDfX.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>

const DFX_RCMGR_INPUT_BLK mRcMgrClassDflts = {
  // This is where you declare all input block vars/values you want to share with the Host.
  // This becomes part of the IP API for the Host.
  .AmdSmee = false,
  .AmdFabric1TbRemap = 0xFF
};

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
static
void
FabricGetRemainingSizeForThisRegion (
  SIL_FABRIC_MMIO_REGION  *MmioRegion,
  uint64_t                *Size,
  uint64_t                Alignment,
  uint8_t                 MmioType
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
    assert(false);
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
static
uint64_t
FabricGetBaseAddress (
  SIL_FABRIC_MMIO_REGION *MmioRegion,
  uint64_t               Length,
  uint64_t               Alignment,
  uint8_t                MmioType
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

/**
 * FabricReserveMmio
 *
 * @brief Program MMIO base/limit/control registers
 *
 * @param[in, out]    BaseAddress        Starting address of the requested MMIO range.
 * @param[in, out]    Length             Length of the requested MMIO range.
 * @param[in]         Alignment          Alignment bit map.
 * @param[in]         Target             PCI bus number/RootBridge number of the requester.
 * @param[in, out]    Attributes         Attributes of the requested MMIO range indicating whether
 *                                       it is readable/writable/non-posted
 *
 * @retval   SIL_STATUS  SilOutOfResources    - The requested range could not be added because there are not
 *                                              enough mapping resources.
 *                       SilAborted           - One or more input parameters are invalid. For example, the
 *                                              PciBusNumber does not correspond to any device in the system.
 *                       SilPass              - Success to get an MMIO region
 *
 */
SIL_STATUS
FabricReserveMmio (
  uint64_t                    *BaseAddress,
  uint64_t                    *Length,
  uint64_t                    Alignment,
  FABRIC_TARGET               Target,
  FABRIC_MMIO_ATTRIBUTE       *Attributes
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
  uint32_t                  PrimaryDie;
  uint32_t                  PrimaryRootBridge;
  uint32_t                  CfgLimitNumLimit;
  CFG_BASE_ADDRESS_REGISTER CfgBase;
  DFX_FABRIC_MMIO_MANAGER   *FabricMmioManager;
  SIL_FABRIC_MMIO_REGION    *MmioRegion;
  SIL_FABRIC_MMIO_REGION    *PrimaryRb2ndMmioRegion;
  DFX_RCMGR_INPUT_BLK       *RcMgrData;
  DF_IP2IP_API *DfIp2IpApi;
  uint8_t                   NumberOfBusRegions;
  SIL_STATUS                Status;
  APOB_IP2IP_API            *ApobIp2IpApi;
  APOB_SOC_DIE_INFO         SocMaxDieInfo;

  RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  openSIL FabricReserveMmio\n");

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  RcMgrData = (DFX_RCMGR_INPUT_BLK *)xUslFindStructure(SilId_RcManager, 0);
  if (RcMgrData == NULL) {
    return SilNotFound; // Could not find the IP input block
  }
  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  *BaseAddress = 0;
  Status = SilPass;
  FabricMmioManager = &RcMgrData->MmioRcMgr;

  NumberOfBusRegions = DfIp2IpApi->DfGetNumberOfBusRegions ();
  SocketCount = DfIp2IpApi->DfGetNumberOfProcessorsPresent ();
  RbPerDieCount = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie(0, 0);
  RbPerSktCount = DfIp2IpApi->DfGetNumberOfRootBridgesOnSocket(0);
  SilGetPrimaryRb(&PrimarySocket, &PrimaryDie, &PrimaryRootBridge);

  if (FabricMmioManager == NULL) {
    assert(FabricMmioManager != NULL);
    return SilAborted;
  }

  // Check input parameters
  if (*Length == 0) {
    return SilPass;
  }

  if ((Attributes->MmioType != NON_PCI_DEVICE_BELOW_4G) &&
    (Attributes->MmioType != NON_PCI_DEVICE_ABOVE_4G)) {
    RCMGR_TRACEPOINT(SIL_TRACE_WARNING,
      "  This function only support Attributes->MmioType = NON_PCI_DEVICE_BELOW_4G or NON_PCI_DEVICE_ABOVE_4G\n"
      );
    return SilAborted;
  }

  // Find out Socket/Rb
  DstFabricID = 0xFFFFFFFF;
  Socket = 0xFF;
  Rb = 0xFF;
  if (Target.TgtType == TARGET_PCI_BUS) {
    for (CfgAddrMapIndex = 0; CfgAddrMapIndex < NumberOfBusRegions; CfgAddrMapIndex++) {
      DfIp2IpApi->DfAbstractRegAcc((uint8_t)CFG_LIMIT_ADDRESS0_BusNumLimit,
        0,
        CfgAddrMapIndex,
        FABRIC_REG_ACC_BC,
        0,
        DF_ABSTRACT_REG_READ,
        0,
        &CfgLimitNumLimit
        );

      CfgBase.Value = DfIp2IpApi->DfFabricRegisterAccRead(0,
        0,
        CFGBASEADDRESS_0_FUNC,
        (uint32_t)(CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG))),
        FABRIC_REG_ACC_BC
        );
      if ((CfgBase.Field.RE == 1) && (CfgBase.Field.WE == 1) && (CfgLimitNumLimit >= Target.PciBusNum) &&
        (CfgBase.Field.BusNumBase <= Target.PciBusNum) && (CfgBase.Field.SegmentNum == Target.PciSegNum)) {
        DfIp2IpApi->DfAbstractRegAcc((uint8_t)CFG_LIMIT_ADDRESS0_DstFabricID,
          0,
          CfgAddrMapIndex,
          FABRIC_REG_ACC_BC,
          0,
          DF_ABSTRACT_REG_READ,
          0,
          &DstFabricID
          );
        break;
      }
    }

    if (CfgAddrMapIndex >= NumberOfBusRegions) {
      return SilAborted;
    }

    assert(DstFabricID != 0xFFFFFFFF);
    for (i = 0; i < SocketCount; i++) {
      for (j = 0; j < RbPerSktCount; j++) {
        if (DfIp2IpApi->DfGetHostBridgeSystemFabricID(i, (j / RbPerDieCount), (j % RbPerDieCount)) == DstFabricID) {
          Socket = i;
          Rb = j;
          break;
        }
      }
    }

    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "Socket: %X\n", Socket);
    assert(Socket != 0xFF);
    assert(Rb != 0xFF);
  } else {
    Socket = (uint8_t) (Target.SocketNum);
    Rb = (uint8_t) (Target.RbNum);
    // This is for combo support for multi/single NBIO in one IOD
    if (Rb >= RbPerSktCount) {
      Rb = (uint8_t) (RbPerSktCount - 1);
    }
  }

  if ((Socket >= SocMaxDieInfo.MaxSocSocketsSupportedValue) || (Rb >= RCMGR_MAX_RBS_PER_SOCKET)) {
    assert(false);
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

    // return EFI_ABORTED if no MMIO on this Rb
    if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[Socket][Rb] == false) {
      RCMGR_TRACEPOINT(SIL_TRACE_ERROR, "  ERROR: No below 4G MMIO on Socket %X Rb %X\n", Socket, Rb);
      return SilAborted;
    }

    // Find out MmioRegion for this Rb
    MmioRegion = &FabricMmioManager->MmioRegionBelow4G[Socket][Rb];
    FabricGetRemainingSizeForThisRegion(MmioRegion, &SizeA, Alignment, Attributes->MmioType);
    if (FabricMmioManager->PrimaryRbHas2ndMmioBelow4G && (Socket == PrimarySocket) && (Rb == PrimaryRootBridge)) {
      // primary RootBridge may have 2nd MmioRegion
      TempSocket = (FabricMmioManager->PrimaryRb2ndMmioPairBelow4G >> 4) & 0xF;
      TempRb = FabricMmioManager->PrimaryRb2ndMmioPairBelow4G & 0xF;
      PrimaryRb2ndMmioRegion = &FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb];
      FabricGetRemainingSizeForThisRegion(PrimaryRb2ndMmioRegion, &SizeB, Alignment, Attributes->MmioType);
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
      *BaseAddress = FabricGetBaseAddress(MmioRegion, *Length, Alignment, Attributes->MmioType);
    } else if (SizeB >= (*Length)) {
      *BaseAddress = FabricGetBaseAddress(PrimaryRb2ndMmioRegion, *Length, Alignment, Attributes->MmioType);
    }
  } else if ((Attributes->MmioType == MMIO_ABOVE_4G) ||
    (Attributes->MmioType == P_MMIO_ABOVE_4G) ||
    (Attributes->MmioType == NON_PCI_DEVICE_ABOVE_4G)) {
    //
    // Above 4G
    //
    // return EFI_ABORTED if no MMIO on this Rb
    if (FabricMmioManager->AllocateMmioAbove4GOnThisRb[Socket][Rb] == false) {
      RCMGR_TRACEPOINT(SIL_TRACE_ERROR, "  ERROR: No above 4G MMIO on Socket %X Rb %X\n", Socket, Rb);
      return SilAborted;
    }

    MmioRegion = &FabricMmioManager->MmioRegionAbove4G[Socket][Rb];
    FabricGetRemainingSizeForThisRegion(MmioRegion, &SizeA, Alignment, Attributes->MmioType);
    if (SizeA >= (*Length)) {
      // Have enough space
      *BaseAddress = FabricGetBaseAddress(MmioRegion, *Length, Alignment, Attributes->MmioType);
    }
  }

  if (*BaseAddress == 0) {
    *Length = (SizeA > SizeB) ? SizeA : SizeB;
    *BaseAddress = 0;
    Status = SilOutOfResources;
  }

  if (Status == SilPass) {
    Attributes->NonPosted = 0;
    Attributes->ReadEnable = 1;
    Attributes->WriteEnable = 1;
    Attributes->CpuDis = 0;
    RCMGR_TRACEPOINT(SIL_TRACE_INFO,
      "  Reserve MMIO from 0x%llX ~ 0x%llX\n",
      *BaseAddress,
      (*BaseAddress + *Length - 1)
      );
  } else if (Status == SilOutOfResources) {
    RCMGR_TRACEPOINT(SIL_TRACE_INFO,
      "  No enough space, the biggest MMIO size is 0x%X0000\n",
      (uint32_t) (*Length >> 16)
      );
    // FabricReserveMmio is restricted for NON_PCI_DEVICE_BELOW_4G & NON_PCI_DEVICE_ABOVE_4G only
    // So if there's no enough space, ASSERT here
    if (Attributes->MmioType == NON_PCI_DEVICE_BELOW_4G) {
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Please increase MmioSizePerRbForNonPciDevice\n");
      assert(false);
    }
    if (Attributes->MmioType == NON_PCI_DEVICE_ABOVE_4G) {
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Please increase Above4GMmioSizePerRbForNonPciDevice\n");
      assert(false);
    }
  }

  return Status;
}


/**
 * SilInitMmioEqually4
 *
 * @brief Initialize MMIO registers for each RootBridge evenly.
 *
 * @param[in]         SilData               Point to DFX_RCMGR_INPUT_BLK data structure
 * @param[in, out]    SilData->MmioRcMgr    Point to DFX_FABRIC_MMIO_MANAGER
 * @param[in]         SilData->SocketNumber System socket count
 * @param[in]         SilData->RbsPerSocket RootBridge number per socket
 * @param[in]         SilData->PciExpressBaseAddress
 *
 */
SIL_STATUS
SilInitMmioEqually4 (
  DFX_RCMGR_INPUT_BLK *SilData
  )
{
  uint8_t  i;
  uint8_t  j;
  uint8_t  SystemRbNumber;
  uint8_t  RbNumberAbovePcieCfg;
  uint8_t  RbNumberAbovePcieCfgCopy;
  uint8_t  RbNumberMmioHasInitialized;
  uint64_t TotalAvailableSize;
  uint64_t SizeAbovePcieCfg;
  uint64_t SizeBelowPcieCfg;
  uint64_t MmioBaseAddr;
  uint64_t MmioSize;
  uint64_t MmioSize16MAligned;
  uint64_t MmioSizeRemained;
  uint64_t MmioLimitAbove4G;
  uint64_t TOM;
  uint64_t TOM2;
  uint64_t PciCfgSpace;
  uint64_t BottomOfCompat;
  uint64_t MmioSizeBelowHole;
  uint64_t MmioSizeAboveHole;
  uint32_t PrimarySocket;
  uint32_t PrimaryDie;
  uint32_t PrimaryRootBridge;
  bool AbovePcieCfgIsTooSmall;
  bool BelowPcieCfgIsTooSmall;
  bool ReservedRegionAlreadySet;
  DFX_FABRIC_MMIO_MANAGER       *FabricMmioManager;
  uint32_t                      MaxSystemRbCount;
  SIL_STATUS                    Status;
  DF_IP2IP_API                  *DfIp2IpApi;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  if (Status == SilPass) {
    MaxSystemRbCount = DfIp2IpApi->DfGetNumberOfSystemRootBridges ();
  } else {
    assert(Status == SilPass);
    return Status;
  }

  FabricMmioManager = &SilData->MmioRcMgr;

  SilGetPrimaryRb(&PrimarySocket, &PrimaryDie, &PrimaryRootBridge);

  SystemRbNumber = SilData->RbsPerSocket * SilData->SocketNumber;
  BottomOfCompat = BOTTOM_OF_COMPAT;
  ReservedRegionAlreadySet = false;  // Indicate if AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat is set

  // System information
  PciCfgSpace = (xUslRdMsr(MSR_MMIO_CFG_BASE) >> 2) & 0xF;     // Get bus range from MSR_C001_0058[5:2][BusRange]
  PciCfgSpace = ((uint64_t) 1 << PciCfgSpace) * (1024 * 1024); // The config space is 1MB times the number of buses
  TOM = xUslRdMsr(MSR_TOM);
  TOM2 = xUslRdMsr(MSR_TOM2);
  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "  TOM: 0x%llX, TOM2: 0x%llX, Pcie configuration space: 0x%llX ~ 0x%llX\n",
    TOM,
    TOM2,
    SilData->PciExpressBaseAddress,
    SilData->PciExpressBaseAddress + PciCfgSpace
    );

  // Below 4G
  //    +---------------------------------+ +--+ 4G
  //    |  BIOS FW                        |
  //    |  Local APIC and etc.            |          Leave as undescribed, so it's a COMPAT region
  //    |       Undescribed               |
  //    +---------------------------------+ +--+ 0xFEC0_0000 (BottomOfCompat), from TOM to BottomOfCompat is MMIO space
  //    |  Reserved for Primary RootBridge|         Reserved this region for some devices which need a fixed MMIO space
  //    +---------------------------------+ +--+ 0xFE00_0000 (AmdBottomMmioReservedForPrimaryRb), from TOM to
  //    |  MMIO Above PCIe Cfg            |      AmdBottomMmioReservedForPrimaryRb is the region that ReserveMmio
  //    |                                 |      function could use
  //    |                                 |
  //    +---------------------------------+ +--+ PciExpressBaseAddress + Size (defined by MSR_C001_0058[5:2][BusRange])
  //    |  PCIe Configuration Space       |
  //    |                                 |
  //    +---------------------------------+ +--+ PciExpressBaseAddress
  //    |                                 |        (recommend to set PciExpressBaseAddress equal to TOM)
  //    |  MMIO Below PCIe Cfg            |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+ +--+ TOM
  //    |                                 |
  //    |  DRAM                           |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+ +--+ 0x0000_0000

  // Above 4G
  //    +---------------------------------+ +--+ 0xFFFD_0000_0000 (2^48 - 12G) or 0x7FD_0000_0000 (8T - 12G)
  //    |                                 |
  //    |   MMIO FOR RootBridge 7         |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+
  //    |                                 |
  //    |   ......                        |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+
  //    |                                 |
  //    |   MMIO FOR RootBridge 2         |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+
  //    |                                 |
  //    |   MMIO FOR RootBridge 1         |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+
  //    |                                 |
  //    |   MMIO FOR RootBridge 0         |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    |                                 |
  //    +---------------------------------+ +--+ TOM2 or 0x100_0000_0000

  AbovePcieCfgIsTooSmall = false;
  BelowPcieCfgIsTooSmall = false;

  if (SilData->PciExpressBaseAddress < 0x100000000) {
    assert(SilData->BottomMmioReservedForPrimaryRb >= (SilData->PciExpressBaseAddress + PciCfgSpace));
    if (SilData->BottomMmioReservedForPrimaryRb < (SilData->PciExpressBaseAddress + PciCfgSpace)) {
      RCMGR_TRACEPOINT(SIL_TRACE_ERROR,
        " BottomMmioReservedForPrimaryRb (0x%x) is less than PCIeBase+PciCfgSpace (0x%x+0x%x)\n",
        SilData->BottomMmioReservedForPrimaryRb,
        SilData->PciExpressBaseAddress,
        PciCfgSpace
        );
      return SilAborted;
    }

    assert(SilData->PciExpressBaseAddress >= TOM);
    if (SilData->PciExpressBaseAddress < TOM) {
      RCMGR_TRACEPOINT(SIL_TRACE_ERROR,
        " PciExpressBaseAddress (0x%x) is less than TOM (0x%x)\n",
        SilData->PciExpressBaseAddress,
        TOM
        );
      return SilAborted;
    }
  }

  assert(BottomOfCompat >= SilData->BottomMmioReservedForPrimaryRb);
  if (BottomOfCompat < SilData->BottomMmioReservedForPrimaryRb) {
    RCMGR_TRACEPOINT(SIL_TRACE_ERROR,
      " BottomOfCompat (0x%x) is less than BottomMmioReservedForPrimaryRb (0x%x)\n",
      BottomOfCompat,
      SilData->BottomMmioReservedForPrimaryRb
      );
    return SilAborted;
  }

  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    SizeBelowPcieCfg = SilData->BottomMmioReservedForPrimaryRb - TOM;
    SizeAbovePcieCfg = 0;
  } else {
    SizeBelowPcieCfg = SilData->PciExpressBaseAddress - TOM;
    SizeAbovePcieCfg = SilData->BottomMmioReservedForPrimaryRb - SilData->PciExpressBaseAddress - PciCfgSpace;
  }
  TotalAvailableSize = SizeAbovePcieCfg + SizeBelowPcieCfg;

  if (SystemRbNumber > 1) {
    // For single RootBridge system, we don't want to check MMIO size above/below PcieCfg

    MmioSize = SizeBelowPcieCfg / SystemRbNumber;
    if (SizeAbovePcieCfg < ((MmioSize * 8) / 10)) {
      // If SizeAbovePcieCfg < 80% of (SizeBelowPcieCfg / SystemRbNumber), ignore this region
      AbovePcieCfgIsTooSmall = true;
      TotalAvailableSize -= SizeAbovePcieCfg;
      RCMGR_TRACEPOINT(SIL_TRACE_WARNING, "  AbovePcieCfgIsTooSmall\n");
    }

    MmioSize = SizeAbovePcieCfg / SystemRbNumber;
    if (SizeBelowPcieCfg < ((MmioSize * 8) / 10)) {
      // If SizeBelowPcieCfg < 80% of (SizeAbovePcieCfg / SystemRbNumber), ignore this region
      BelowPcieCfgIsTooSmall = true;
      TotalAvailableSize -= SizeBelowPcieCfg;
      RCMGR_TRACEPOINT(SIL_TRACE_WARNING, "  BelowPcieCfgIsTooSmall\n");
    }
    if (TotalAvailableSize == 0) {
      RCMGR_TRACEPOINT(SIL_TRACE_ERROR, " Total available MMIO size is 0.\n");
      return SilOutOfResources;
    }
  }
  MmioSize = TotalAvailableSize / SystemRbNumber;
  MmioSize16MAligned = MmioSize & SIZE_16M_ALIGN;
  if (AbovePcieCfgIsTooSmall && BelowPcieCfgIsTooSmall) {
    RCMGR_TRACEPOINT(SIL_TRACE_ERROR, " Insufficient MMIO for RB below and above PCIe configuration space.\n");
    return SilOutOfResources;
  }

  // Setup MMIO below 4G
  MmioBaseAddr = 0;
  if (!AbovePcieCfgIsTooSmall) {
    //   1. find out how many RootBridges would have MMIO above PCIe Configuration Space
    RbNumberAbovePcieCfg = (uint8_t) (SizeAbovePcieCfg / (uint32_t) MmioSize);
    if ((SizeAbovePcieCfg - (MmioSize * RbNumberAbovePcieCfg)) > (MmioSize / 2)) {
      RbNumberAbovePcieCfg++;
    }
    if (RbNumberAbovePcieCfg == 0) {
      // it could make sure single RootBridge system would always set the space above PcieCfg as MMIO in step 3
      RbNumberAbovePcieCfg++;
    }
    MmioSize = SizeAbovePcieCfg / RbNumberAbovePcieCfg;
    MmioSize16MAligned = MmioSize & SIZE_16M_ALIGN;
    MmioSizeRemained = SizeAbovePcieCfg - (MmioSize16MAligned * (RbNumberAbovePcieCfg - 1));
    MmioBaseAddr = SilData->BottomMmioReservedForPrimaryRb;
  } else {
    //   2. if there's no MMIO above PCIe Configuration Space,
    RbNumberAbovePcieCfg = 0;
    MmioSizeRemained = SizeBelowPcieCfg - (MmioSize16MAligned * (SystemRbNumber - 1));
    MmioBaseAddr = (SilData->PciExpressBaseAddress >= 0x100000000) ?
      SilData->BottomMmioReservedForPrimaryRb : SilData->PciExpressBaseAddress;
  }

  RbNumberAbovePcieCfgCopy = RbNumberAbovePcieCfg;
  //   3. allocate MMIO for primary RootBridge first, make sure it's always on top
  i = (uint8_t) PrimarySocket;
  j = (uint8_t) PrimaryRootBridge;
  FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] = true;

  if (RbNumberAbovePcieCfg != 0) {
    // above PCIe Configuration Space
    MmioBaseAddr = MmioBaseAddr - MmioSizeRemained;
    RbNumberAbovePcieCfg--;
  } else if (!BelowPcieCfgIsTooSmall) {
    // below PCIe Configuration Space
    MmioBaseAddr = MmioBaseAddr - MmioSizeRemained;
  } else {
    RCMGR_TRACEPOINT(SIL_TRACE_ERROR, " Insufficient MMIO for RB.\n");
    return SilOutOfResources;
  }

  if (!AbovePcieCfgIsTooSmall) {
    // if primary RootBridge's MMIO is above PCIe Configuration Space,
    // include PcdAmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat as well
    Status = SilSetMmioReg4(DfIp2IpApi,
      SilData->SocketNumber,
      ((i * SilData->RbsPerSocket + j) * 2),
      i,
      j,
      MmioBaseAddr,
      (MmioSizeRemained + BottomOfCompat - SilData->BottomMmioReservedForPrimaryRb)
      );
    ReservedRegionAlreadySet = true;
  } else {
    Status = SilSetMmioReg4(DfIp2IpApi,
      SilData->SocketNumber,
      ((i * SilData->RbsPerSocket + j) * 2),
      i,
      j,
      MmioBaseAddr,
      MmioSizeRemained
      );
  }
  // Prefetchable -> Non Prefetchable -> Non Pci
  // Prefetchable/Non Prefetchable 80/20 Ratio
  FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci = SilData->MmioSizePerRbForNonPciDevice;
  FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch =
    (((MmioSizeRemained - FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci) * 4) / 5) & SIZE_16M_ALIGN;
  FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch = MmioBaseAddr;
  FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch =
    FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch + FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch;
  FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci =
    (MmioBaseAddr + MmioSizeRemained - FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci) &
    (~RCMGR_NON_PCI_MMIO_ALIGN_MASK);
  FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch =
    FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci - FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch;
  FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPci = 0;
  FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizePrefetch = 0;
  FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPrefetch = 0;
  FabricMmioManager->MmioRegionBelow4G[i][j].AlignNonPrefetch = 0;
  FabricMmioManager->MmioRegionBelow4G[i][j].AlignPrefetch = 0;

  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch + FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch
    );
  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
    FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch
    );
  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci + FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci
    );

  //   4. allocate MMIO for other RootBridges
  RbNumberMmioHasInitialized = 1;

  if (SilData->RbsPerSocket > PROJ_MAX_RBS_PER_SOCKET) {
    return SilOutOfBounds;
  } else {
    for (i = 0; i < SilData->SocketNumber; i++) {
      for (j = 0; j < SilData->RbsPerSocket; j++) {
        if ((i == PrimarySocket) && (j == PrimaryRootBridge)) {
          continue;
        }

        FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] = true;
        if (RbNumberAbovePcieCfg != 0) {
          // above PCIe Configuration Space
          if (RbNumberMmioHasInitialized == 1) {
            MmioSizeRemained = MmioSize16MAligned;
          }
          MmioBaseAddr = MmioBaseAddr - MmioSizeRemained;
          RbNumberAbovePcieCfg--;
        } else if (!BelowPcieCfgIsTooSmall) {
          // below PCIe Configuration Space
          if (RbNumberAbovePcieCfgCopy == RbNumberMmioHasInitialized) {
            MmioSize = SizeBelowPcieCfg / (SystemRbNumber - RbNumberMmioHasInitialized);
            MmioSize16MAligned = MmioSize & SIZE_16M_ALIGN;
            MmioSizeRemained = SizeBelowPcieCfg -
              (MmioSize16MAligned * (SystemRbNumber - RbNumberMmioHasInitialized - 1));
            MmioBaseAddr = (SilData->PciExpressBaseAddress >= 0x100000000) ?
              SilData->BottomMmioReservedForPrimaryRb : SilData->PciExpressBaseAddress;
          }
          if (RbNumberMmioHasInitialized == (RbNumberAbovePcieCfgCopy + 1)) {
            MmioSizeRemained = MmioSize16MAligned;
          }
          MmioBaseAddr = MmioBaseAddr - MmioSizeRemained;
        } else {
          RCMGR_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocation error.\n");
          return SilOutOfResources;
        }

        Status = SilSetMmioReg4(DfIp2IpApi,
          SilData->SocketNumber,
          ((i * SilData->RbsPerSocket + j) * 2),
          i,
          j,
          MmioBaseAddr,
          MmioSizeRemained
          );
        // Prefetchable -> Non Prefetchable -> Non Pci
        // Prefetchable/Non Prefetchable 80/20 Ratio
        FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci = SilData->MmioSizePerRbForNonPciDevice;
        FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch =
          (((MmioSizeRemained - FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci) * 4) / 5) & SIZE_16M_ALIGN;
        FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch = MmioBaseAddr;
        FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch =
          FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch +
          FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch;
        FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci =
          (MmioBaseAddr + MmioSizeRemained - FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci) &
          (~RCMGR_NON_PCI_MMIO_ALIGN_MASK);
        FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch =
          FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci -
          FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch;
        FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPci = 0;
        FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizePrefetch = 0;
        FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPrefetch = 0;
        FabricMmioManager->MmioRegionBelow4G[i][j].AlignNonPrefetch = 0;
        FabricMmioManager->MmioRegionBelow4G[i][j].AlignPrefetch = 0;

        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
          (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch +
          FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
          FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch
          );
        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
          (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
          FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
          FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch
          );
        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
          FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
          (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci
          + FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
          FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci
          );

        RbNumberMmioHasInitialized++;
      }
    }
  }

  //   4. if there's a spare MMIO register pair, try to set undescribed space (above or below PCIe Configuration)
  //      as primary RootBridge's 2nd MMIO
  if (SystemRbNumber < MaxSystemRbCount) {
    if ((AbovePcieCfgIsTooSmall && (SizeAbovePcieCfg != 0)) ||
      ((SizeBelowPcieCfg != 0) && (BelowPcieCfgIsTooSmall || (SystemRbNumber == 1)))) {
      for (i = 0; i < RCMGR_MAX_SOCKETS; i++) {
        for (j = 0; j < SilData->RbsPerSocket; j++) {
          if (SilData->RbsPerSocket > PROJ_MAX_RBS_PER_SOCKET) {
            return SilOutOfBounds;
          }

          if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] == false) {
            RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  socket 0 last root bridge has 2nd MMIO below 4G\n");
            FabricMmioManager->PrimaryRb2ndMmioPairBelow4G = (uint8_t) ((i << 4) | j);
            if (AbovePcieCfgIsTooSmall) {
              MmioBaseAddr = SilData->PciExpressBaseAddress + PciCfgSpace;
              MmioSize = SizeAbovePcieCfg;
              Status = SilSetMmioReg4(DfIp2IpApi,
                SilData->SocketNumber,
                ((i * SilData->RbsPerSocket + j) * 2),
                PrimarySocket,
                PrimaryRootBridge,
                MmioBaseAddr,
                (MmioSize + BottomOfCompat - SilData->BottomMmioReservedForPrimaryRb)
                );
              ReservedRegionAlreadySet = true;
            } else {
              // for single RootBridge system, primary RootBridge always set the space above PcieCfg as MMIO in step 3
              MmioBaseAddr = TOM;
              MmioSize = SizeBelowPcieCfg;
              Status = SilSetMmioReg4(DfIp2IpApi,
                SilData->SocketNumber,
                ((i * SilData->RbsPerSocket + j) * 2),
                PrimarySocket,
                PrimaryRootBridge,
                MmioBaseAddr,
                MmioSize
                );
            }

            FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch = MmioSize;
            FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch = MmioBaseAddr;
            FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPci = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizePrefetch = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPrefetch = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].AlignNonPrefetch = 0;
            FabricMmioManager->MmioRegionBelow4G[i][j].AlignPrefetch = 0;

            RCMGR_TRACEPOINT(SIL_TRACE_INFO,
              "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch +
              FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch
              );
            RCMGR_TRACEPOINT(SIL_TRACE_INFO,
              "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch
              );
            RCMGR_TRACEPOINT(SIL_TRACE_INFO,
              "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci +
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci
              );

            FabricMmioManager->PrimaryRbHas2ndMmioBelow4G = true;
            break;
          }
        }
        if (FabricMmioManager->PrimaryRbHas2ndMmioBelow4G) {
          break;
        }
      }
    }
  }

  //   5. Distribute MMIO above 4G evenly
  MmioBaseAddr = (TOM2 > 0x100000000) ? TOM2 : 0x100000000; // Check if TOM2 > 4G
  MmioLimitAbove4G = (uint64_t)xUslGetPhysAddrSize ();
  if (SilData->AmdSmee) {
    MmioLimitAbove4G -= (uint64_t)xUslGetPhysAddrReduction ();
  }
  MmioLimitAbove4G = (1ull << (MmioLimitAbove4G & 0x3F)) - 0x300000000;

  if (SilData->MmioAbove4GLimit < MmioLimitAbove4G) {
    MmioLimitAbove4G = (SilData->MmioAbove4GLimit + 1) & 0xFFFFFFFFFFFF0000;
  }

  if (SilData->AmdFabric1TbRemap) {
    // there's a hole at 0xFD_0000_0000 ~ 0x100_0000_0000
    MmioSizeBelowHole = 0;
    MmioSizeAboveHole = 0;

    // calculate Mmio size below/above this hole
    if (MmioBaseAddr < 0xFD00000000) {
      MmioSizeBelowHole = (MmioLimitAbove4G < 0xFD00000000)? MmioLimitAbove4G : 0xFD00000000;
      MmioSizeBelowHole = MmioSizeBelowHole - MmioBaseAddr;
    }

    if (MmioLimitAbove4G > 0x10000000000) {
      MmioSizeAboveHole = (MmioBaseAddr > 0x10000000000)? MmioBaseAddr : 0x10000000000;
      MmioSizeAboveHole = MmioLimitAbove4G - MmioSizeAboveHole;
    }

    if ((MmioSizeBelowHole == 0) && (MmioSizeAboveHole == 0)) {
      MmioLimitAbove4G = 0; // set limit to 0, so there's no MMIO space above 4G
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "    WARNING: There's no MMIO space above 4G\n");
    } else {
      if (MmioSizeAboveHole > MmioSizeBelowHole) {
        MmioBaseAddr = (TOM2 > 0x10000000000) ? TOM2 : 0x10000000000; // Check if TOM2 > 1T
      } else {
        MmioLimitAbove4G = (MmioLimitAbove4G < 0xFD00000000) ? MmioLimitAbove4G : 0xFD00000000;
      }
    }
  }
  //there's a PCIE hole if PcdPciExpressBaseAddress is above 4G
  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    assert(SilData->PciExpressBaseAddress >= TOM2);
    assert((SilData->PciExpressBaseAddress + PciCfgSpace) < MmioLimitAbove4G);
    if ((SilData->PciExpressBaseAddress <= MmioBaseAddr) &&
      ((SilData->PciExpressBaseAddress + PciCfgSpace) >= MmioBaseAddr)) {
      MmioBaseAddr = SilData->PciExpressBaseAddress + PciCfgSpace;
    } else if ((SilData->PciExpressBaseAddress <= MmioLimitAbove4G) &&
      ((SilData->PciExpressBaseAddress + PciCfgSpace) >= MmioLimitAbove4G)) {
      MmioLimitAbove4G = SilData->PciExpressBaseAddress;
    } else if ((SilData->PciExpressBaseAddress >= MmioBaseAddr) &&
      (SilData->PciExpressBaseAddress <= MmioLimitAbove4G) &&
      ((SilData->PciExpressBaseAddress + PciCfgSpace) >= MmioBaseAddr) &&
      (SilData->PciExpressBaseAddress + PciCfgSpace) <= MmioLimitAbove4G) {
      if ((SilData->PciExpressBaseAddress - MmioBaseAddr) <
        (MmioLimitAbove4G - (SilData->PciExpressBaseAddress + PciCfgSpace))) {
        MmioBaseAddr = SilData->PciExpressBaseAddress + PciCfgSpace;
      } else {
        MmioLimitAbove4G = SilData->PciExpressBaseAddress;
      }
    } else {
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "    MMIO region is completed in the PCIE configuration space.\n");
      assert(false);
    }
  }

  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "    Above 4G MMIO base is 0x%llX, limit is 0x%llX\n",
    MmioBaseAddr,
    MmioLimitAbove4G
    );

  if (MmioLimitAbove4G > MmioBaseAddr) {
    MmioSize = MmioLimitAbove4G - MmioBaseAddr;
    MmioSize16MAligned = (MmioSize / SystemRbNumber) & SIZE_16M_ALIGN;
    MmioSizeRemained = MmioSize - (MmioSize16MAligned * (SystemRbNumber - 1));

    for (i = 0; i < SilData->SocketNumber; i++) {
      for (j = 0; j < SilData->RbsPerSocket; j++) {
        if (SilData->RbsPerSocket > PROJ_MAX_RBS_PER_SOCKET) {
          return SilOutOfBounds;
        }

        FabricMmioManager->AllocateMmioAbove4GOnThisRb[i][j] = true;
        if ((i == (SilData->SocketNumber - 1)) && (j == (SilData->RbsPerSocket - 1))) {
          MmioSize16MAligned = MmioSizeRemained;
        }
        if (MmioSize16MAligned < MMIO_MIN_NON_PCI_SIZE_ABOVE4G) {
          continue;
        }
        if ((i == (SilData->SocketNumber - 1)) && (j == (SilData->RbsPerSocket - 1))) {
          // workaround to set last RootBridge's MMIO limit to 0xFFFF_FFFF_FFFF
          Status = SilSetMmioReg4(DfIp2IpApi,
            SilData->SocketNumber,
            ((i * SilData->RbsPerSocket + j) * 2 + 1),
            i,
            j,
            MmioBaseAddr,
            (MmioLimitAbove4G - MmioBaseAddr)
            );
        } else {
          Status = SilSetMmioReg4(DfIp2IpApi,
            SilData->SocketNumber,
            ((i * SilData->RbsPerSocket + j) * 2 + 1),
            i,
            j,
            MmioBaseAddr,
            MmioSize16MAligned
            );
        }
        // Non Pci -> Prefetchable -> Non Prefetchable
        // Prefetchable/Non Prefetchable 80/20 Ratio
        FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci =
          (MmioBaseAddr + RCMGR_NON_PCI_MMIO_ALIGN_MASK_ABOVE4G) & (~RCMGR_NON_PCI_MMIO_ALIGN_MASK_ABOVE4G);
        FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci =
          (SilData->Above4GMmioSizePerRbForNonPciDevice > MMIO_MIN_NON_PCI_SIZE_ABOVE4G) ?
          SilData->Above4GMmioSizePerRbForNonPciDevice : MMIO_MIN_NON_PCI_SIZE_ABOVE4G;
        FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch =
          FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci + FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci;
        FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch =
          (((MmioBaseAddr + MmioSize16MAligned - FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci -
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci) * 4) / 5) & SIZE_16M_ALIGN;
        FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch =
          FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch +
          FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch;
        FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch =
          MmioBaseAddr + MmioSize16MAligned - FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch;
        FabricMmioManager->MmioRegionAbove4G[i][j].UsedSizeNonPci = 0;
        FabricMmioManager->MmioRegionAbove4G[i][j].UsedSizePrefetch = 0;
        FabricMmioManager->MmioRegionAbove4G[i][j].UsedSizeNonPrefetch = 0;
        FabricMmioManager->MmioRegionAbove4G[i][j].AlignNonPrefetch = 0;
        FabricMmioManager->MmioRegionAbove4G[i][j].AlignPrefetch = 0;

        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci +
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci
          );
        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch +
          FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch
          );
        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch +
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch
          );

        MmioBaseAddr += MmioSize16MAligned;
      }
    }
  }

  //   6. if there's a spare MMIO register pair, set 0xFED0_0000 ~ 0xFED0_FFFF as posted
  SilAdditionalMmioSetting4(SilData, BottomOfCompat, ReservedRegionAlreadySet);
  return SilPass;
}

/**
 * SilInitIoEqually4
 *
 * @brief Initialize IO registers for each RootBridge evenly.
 *
 * @param             SilData         Pointer to DFX_RCMGR_INPUT_BLK data structure:
 * @param[in, out]    SilData->IoRcMgr          Point to DFX_FABRIC_IO_MANAGER
 * @param[in]         SilData->SocketNumber             System socket count
 * @param[in]         SilData->RbsPerSocket             RootBridge number per socket
 *
 */
SIL_STATUS
SilInitIoEqually4 (
  DFX_RCMGR_INPUT_BLK *SilData
  )
{
  uint32_t    PhySktLoop;
  uint32_t    PhyRbLoop;
  uint32_t    LogSktLoop;
  uint32_t    LogRbLoop;
  uint32_t    MinIoRbCnt;
  uint8_t     RegIndex;
  uint32_t    IoBase;
  uint32_t    IoSize;
  uint32_t    IoSizeForThisRb;
  uint32_t    IoBaseForPrimaryRb;
  uint32_t    IoSizeForPrimaryRb;
  uint32_t    IoBaseForOthers;
  uint32_t    LegacyIoSize;
  uint32_t    PrimarySocket;
  uint32_t    PrimaryDie;
  uint32_t    PrimaryRootBridge;
  bool        LogToPhyMapInit;
  FABRIC_RB_LOG_TO_PHY_MAP LogToPhyMap[PROJ_MAX_SOCKETS_SUPPORTED * PROJ_MAX_HOST_BRIDGES_PER_SOCKET];
  DFX_FABRIC_IO_MANAGER *FabricIoManager;
  DF_IP2IP_API          *DfIp2IpApi;
  SIL_STATUS            Status;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  FabricIoManager = &SilData->IoRcMgr;

  RegIndex = 0;
  LegacyIoSize = X86_LEGACY_IO_SIZE;
  LogToPhyMapInit = false;
  MinIoRbCnt = (SilData->SocketNumber * SilData->RbsPerSocket);
  RCMGR_TRACEPOINT(SIL_TRACE_INFO, "    reserve 0x%X IO size for legacy devices\n", LegacyIoSize);

  // most IO registers have a 16bit limit
  // reserve LegacyIoSize for primary RootBridge
  IoSize = ((X86IO_LIMIT - LegacyIoSize) / MinIoRbCnt) & RCMGR_IO_SIZE_MASK; // 12 bit alignment

  // if not enough IO space to distribute evenly among all RBs
  // distribute evenly among maximum number of RBs
  if (IoSize < 1) {
    // find max # RBs that can get minimum amount of IO space
    IoSize = ~((uint32_t) RCMGR_IO_SIZE_MASK) + 1;
    assert(IoSize != 0);
    // minus one to be sure we have IO on last RB
    MinIoRbCnt = ((X86IO_LIMIT - LegacyIoSize) / IoSize) - 1;
    assert(MinIoRbCnt < (uint32_t) (SilData->SocketNumber * SilData->RbsPerSocket));
  }

  // Get base & size for primary RootBridge
  SilGetPrimaryRb(&PrimarySocket, &PrimaryDie, &PrimaryRootBridge);

  IoBaseForPrimaryRb = 0;
  IoSizeForPrimaryRb = DF_IO_LIMIT - IoBaseForPrimaryRb;
  IoBaseForOthers = 0;
  if ((SilData->SocketNumber != 1) || (SilData->RbsPerSocket != 1)) {
    // If we have more than one RootBridge
    // find out primary RootBridge and reserve an IO region start from 0 for it
    for (PhySktLoop = 0; PhySktLoop < SilData->SocketNumber; PhySktLoop++) {
      for (PhyRbLoop = 0; PhyRbLoop < SilData->RbsPerSocket; PhyRbLoop++) {
        if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
          IoSizeForPrimaryRb = IoSize + LegacyIoSize;
          IoBaseForOthers = IoBaseForPrimaryRb + IoSizeForPrimaryRb;
          // found primary RB so break loops
          PhySktLoop = SilData->SocketNumber;
          break;
        }
      }
    }
  }

  for (LogSktLoop = 0; LogSktLoop < SilData->SocketNumber; LogSktLoop++) {
    for (LogRbLoop = 0; LogRbLoop < SilData->RbsPerSocket; LogRbLoop++) {
      DfXSilGetPhySktRbNum(DfIp2IpApi,
        LogSktLoop,
        LogRbLoop,
        &PhySktLoop,
        &PhyRbLoop,
        &LogToPhyMapInit,
        &LogToPhyMap[0]
        );

      if (MinIoRbCnt-- || (((LogSktLoop + 1) == SilData->SocketNumber) && ((LogRbLoop + 1) == SilData->RbsPerSocket))) {
        if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
          IoBase = IoBaseForPrimaryRb;
          IoSizeForThisRb = IoSizeForPrimaryRb;
        } else {
          IoBase = IoBaseForOthers;
          IoSizeForThisRb = IoSize;
          IoBaseForOthers += IoSizeForThisRb;
        }

        if (((LogSktLoop + 1) == SilData->SocketNumber) && ((LogRbLoop + 1) == SilData->RbsPerSocket)) {
          // check if it's the last RB. DF's IO space is up to 25 bits, so set limit to 0x1FFFFFF for last RB
          IoSizeForThisRb = DF_IO_LIMIT - IoBase;
        }

        SilSetIoReg4(SilData->SocketNumber, RegIndex, PhySktLoop, PhyRbLoop, IoBase, IoSizeForThisRb);

        if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
          // This root bridge has an additional IO region for legacy devices, size is LegacyIoSize
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase = IoBase + LegacyIoSize;
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize = IoSizeForThisRb - LegacyIoSize;
        } else {
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase = IoBase;
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize = IoSizeForThisRb;
        }
        FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoUsed = 0;

        if (((LogSktLoop + 1) == SilData->SocketNumber) && ((LogRbLoop + 1) == SilData->RbsPerSocket)) {
          // When report to X86, IO region should be IoBase ~ X86IO_LIMIT since X86 only support 16 bits IO space
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize =
            X86IO_LIMIT - FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase;
        }

        RCMGR_TRACEPOINT(SIL_TRACE_INFO,
          "  Socket%x RootBridge%x has IO base 0x%X size 0x%X\n\n",
          PhySktLoop,
          PhyRbLoop,
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase,
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize
          );

        RegIndex++;
      }
    }
  }

  FabricIoManager->GlobalCtrl = true;

  return SilPass;
}
