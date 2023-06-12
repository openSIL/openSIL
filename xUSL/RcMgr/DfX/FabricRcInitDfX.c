/**
 * @file  FabricResourceInitDfX.c
 * @brief OpenSIL DataFabric MMIO and IO resource map initialization.
 *
 */
 /* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Utils.h>
#include <DF/Df.h>
#include <DF/Common/SilFabricInfo.h>
#include <RcMgr/FabricResourceManager.h>
#include "RcManager4-api.h"
#include <MsrReg.h>
#include "FabricRcInitDfX.h"


/**
 * DfXRcMgrSetInputBlk
 *
 * @brief Input block API that assigns host memory block to the IP.
 *
 */
SIL_STATUS
DfXRcMgrSetInputBlk ( void )
{
  SilCreateInfoBlock (SilId_RcManager,
                      sizeof(DFX_RCMGR_DATA_BLK),
                      RCMGR_INIT_INSTANCE,
                      RCMGR_INIT_MAJOR_REV,
                      RCMGR_INIT_MINOR_REV);

  return SilPass;
}

/**
 * InitializeResourceManagerDfXTp1
 *
 * @brief Initialize DFX resource registers for each RootBridge.
 *
 */
SIL_STATUS
InitializeResourceManagerDfXTp1 (void)
{
  DFX_RCMGR_INPUT_BLK *SilData;
  SIL_STATUS          Status;
  SIL_STATUS          CalledStatus;

  // Locate the IP block
  SilData = (DFX_RCMGR_INPUT_BLK *) SilFindStructure(SilId_RcManager,  0);
  DF_TRACEPOINT (SIL_TRACE_INFO, "openSIL RC Init memory block is found blk at: 0x%x \n", SilData);
  if (SilData == NULL) {
    return SilNotFound; // Could not find the IP input block
  }

  // These asserts prevent tainted scalar coverity warnings by checking values
  // are within permissible ranges
  assert ((SilData->SocketNumber > 0) && (SilData->SocketNumber < MAX_SOCKETS_SUPPORTED));
  assert ((SilData->RbsPerSocket > 0) && (SilData->RbsPerSocket < MAX_RBS_PER_SOCKET));

  Status = SilPass;
  CalledStatus = SilPass;

  // Initialize MMIO
  if (SilData->SetRcBasedOnNv) {
    // Got NvVariable successfully, try to init MMIO based on it
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Init MMIO based on NV variable\n");
    CalledStatus = SilInitMmioBasedOnNvVariable4 (SilData, NULL, true);
  }
  if ((CalledStatus != SilPass) || (!SilData->SetRcBasedOnNv)) {
    DF_TRACEPOINT (SIL_TRACE_INFO, " Can't get NV variable or init MMIO based on NV variable failed.\n");
    DF_TRACEPOINT (SIL_TRACE_INFO, " Init MMIO equally.\n");
    CalledStatus = SilInitMmioEqually4 (SilData);
    Status = CalledStatus;
  }

  // Initialize IO
  if (SilData->SetRcBasedOnNv) {
    // Get NvVariable successfully, try to init IO base on it
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Init IO based on NV variable\n");
    CalledStatus = SilInitIoBasedOnNvVariable4 (SilData, NULL, true);
  }
  if ((CalledStatus != SilPass) || (!SilData->SetRcBasedOnNv)) {
    DF_TRACEPOINT (SIL_TRACE_INFO, " Can't get NV variable or init IO based on NV variable failed.\n");
    DF_TRACEPOINT (SIL_TRACE_INFO, "                      Init IO equally.\n");
    CalledStatus = SilInitIoEqually4 (SilData);
    Status = CalledStatus;
  }

  if ((SilData->SetRcBasedOnNv) && (!SilData->McptEnable)) {
    CalledStatus = SilInitPciBusBasedOnNvVariable4 (SilData);
    Status = CalledStatus;
  }

  return Status;
}

/**
 * InitializeResourceManagerDfXTp2
 *
 * @brief Initialize DFX at Tp2.
 *
 */
SIL_STATUS
InitializeResourceManagerDfXTp2 (void)
{
  DF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeResourceManagerDfXTp3
 *
 * @brief Initialize DFX at Tp3.
 *
 */
SIL_STATUS
InitializeResourceManagerDfXTp3 (void)
{
  DF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
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
  uint32_t PrimaryRootBridge;
  bool AbovePcieCfgIsTooSmall;
  bool BelowPcieCfgIsTooSmall;
  bool ReservedRegionAlreadySet;
  DFX_FABRIC_MMIO_MANAGER *FabricMmioManager;

  FabricMmioManager = &SilData->MmioRcMgr;

  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

  SystemRbNumber = SilData->RbsPerSocket * SilData->SocketNumber;
  BottomOfCompat = BOTTOM_OF_COMPAT;
  ReservedRegionAlreadySet = false;  // Indicate if AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat is set

  // System information
  PciCfgSpace = (xUslRdMsr (MSR_MMIO_Cfg_Base) >> 2) & 0xF;     // Get bus range from MSR_C001_0058[5:2][BusRange]
  PciCfgSpace = ((uint64_t) 1 << PciCfgSpace) * (1024 * 1024);  // The config space is 1MB times the number of buses
  TOM = xUslRdMsr (MSR_TOM);
  TOM2 = xUslRdMsr (MSR_TOM2);
  DF_TRACEPOINT (SIL_TRACE_INFO, "  TOM: 0x%llX, TOM2: 0x%llX, Pcie configuration space: 0x%llX ~ 0x%llX\n",
    TOM, TOM2, SilData->PciExpressBaseAddress, SilData->PciExpressBaseAddress + PciCfgSpace);

  // Below 4G
  //    +---------------------------------+ +--+ 4G
  //    |  Host Firmware FW                        |
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
    assert (SilData->BottomMmioReservedForPrimaryRb >= (SilData->PciExpressBaseAddress + PciCfgSpace));
    if (SilData->BottomMmioReservedForPrimaryRb < (SilData->PciExpressBaseAddress + PciCfgSpace)) {
      DF_TRACEPOINT (SIL_TRACE_ERROR,
        " BottomMmioReservedForPrimaryRb (0x%x) is less than PCIeBase+PciCfgSpace (0x%x+0x%x)\n",
        SilData->BottomMmioReservedForPrimaryRb, SilData->PciExpressBaseAddress, PciCfgSpace);
      return SilAborted;
    }

    assert (SilData->PciExpressBaseAddress >= TOM);
    if (SilData->PciExpressBaseAddress < TOM) {
      DF_TRACEPOINT (SIL_TRACE_ERROR,
        " PciExpressBaseAddress (0x%x) is less than TOM (0x%x)\n",
        SilData->PciExpressBaseAddress, TOM);
      return SilAborted;
    }
  }

  assert (BottomOfCompat >= SilData->BottomMmioReservedForPrimaryRb);
  if (BottomOfCompat < SilData->BottomMmioReservedForPrimaryRb) {
    DF_TRACEPOINT (SIL_TRACE_ERROR, " BottomOfCompat (0x%x) is less than BottomMmioReservedForPrimaryRb (0x%x)\n",
        BottomOfCompat, SilData->BottomMmioReservedForPrimaryRb);
    return SilAborted;
  }

  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    SizeAbovePcieCfg = 0;
    SizeBelowPcieCfg = SilData->BottomMmioReservedForPrimaryRb - TOM;
  } else {
    SizeAbovePcieCfg = SilData->BottomMmioReservedForPrimaryRb - SilData->PciExpressBaseAddress - PciCfgSpace;
    SizeBelowPcieCfg = SilData->PciExpressBaseAddress - TOM;
  }
  TotalAvailableSize = SizeAbovePcieCfg + SizeBelowPcieCfg;

  if (SystemRbNumber > 1) {
    // For single RootBridge system, we don't want to check MMIO size above/below PcieCfg

    MmioSize = SizeBelowPcieCfg / SystemRbNumber;
    if (SizeAbovePcieCfg < ((MmioSize * 8) / 10)) {
      // If SizeAbovePcieCfg < 80% of (SizeBelowPcieCfg / SystemRbNumber), ignore this region
      AbovePcieCfgIsTooSmall = true;
      TotalAvailableSize -= SizeAbovePcieCfg;
      DF_TRACEPOINT (SIL_TRACE_WARNING, "  AbovePcieCfgIsTooSmall\n");
    }

    MmioSize = SizeAbovePcieCfg / SystemRbNumber;
    if (SizeBelowPcieCfg < ((MmioSize * 8) / 10)) {
      // If SizeBelowPcieCfg < 80% of (SizeAbovePcieCfg / SystemRbNumber), ignore this region
      BelowPcieCfgIsTooSmall = true;
      TotalAvailableSize -= SizeBelowPcieCfg;
      DF_TRACEPOINT (SIL_TRACE_WARNING, "  BelowPcieCfgIsTooSmall\n");
    }
    if (TotalAvailableSize == 0) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, " Total available MMIO size is 0.\n");
      return SilOutOfResources;
    }
  }
  MmioSize = TotalAvailableSize / SystemRbNumber;
  MmioSize16MAligned = MmioSize & SIZE_16M_ALIGN;
  if (AbovePcieCfgIsTooSmall && BelowPcieCfgIsTooSmall) {
    DF_TRACEPOINT (SIL_TRACE_ERROR, " Insufficient MMIO for RB below and above PCIe configuration space.\n");
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
    MmioBaseAddr = SilData->PciExpressBaseAddress >= 0x100000000?
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
    DF_TRACEPOINT (SIL_TRACE_ERROR, " Insufficient MMIO for RB.\n");
    return SilOutOfResources;
  }

  if (!AbovePcieCfgIsTooSmall) {
    // if primary RootBridge's MMIO is above PCIe Configuration Space,
    // include AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat as well
    SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2), i, j,
                        MmioBaseAddr, (MmioSizeRemained + BottomOfCompat - SilData->BottomMmioReservedForPrimaryRb));
    ReservedRegionAlreadySet = true;
  } else {
    SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2), i, j, MmioBaseAddr, MmioSizeRemained);
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

  DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch + FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch);
  DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
      FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch);
  DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
    FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
    (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci + FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
    FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci);

  //   4. allocate MMIO for other RootBridges
  RbNumberMmioHasInitialized = 1;
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
          MmioBaseAddr = SilData->PciExpressBaseAddress >= 0x100000000?
            SilData->BottomMmioReservedForPrimaryRb : SilData->PciExpressBaseAddress;
        }
        if (RbNumberMmioHasInitialized == (RbNumberAbovePcieCfgCopy + 1)) {
          MmioSizeRemained = MmioSize16MAligned;
        }
        MmioBaseAddr = MmioBaseAddr - MmioSizeRemained;
      } else {
        DF_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocation error.\n");
        return SilOutOfResources;
      }

      SilSetMmioReg4 (SilData->SocketNumber,
        ((i * SilData->RbsPerSocket + j) * 2), i, j, MmioBaseAddr, MmioSizeRemained);
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

      DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
        FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
        (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch +
          FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
        FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch);
      DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
        FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
        (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
          FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
        FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch);
      DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
        FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
        (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci + FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
        FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci);

      RbNumberMmioHasInitialized++;
    }
  }

  //   4. if there's a spare MMIO register pair, try to set undescribed space (above or below PCIe Configuration)
  //      as primary RootBridge's 2nd MMIO
  if (SystemRbNumber < (DFX_MAX_HOST_BRIDGES_PER_SOCKET * RCMGR_MAX_SOCKETS)) {
    if ((AbovePcieCfgIsTooSmall && (SizeAbovePcieCfg != 0)) ||
        ((SizeBelowPcieCfg != 0) && (BelowPcieCfgIsTooSmall || (SystemRbNumber == 1)))) {
      for (i = 0; i < RCMGR_MAX_SOCKETS; i++) {
        for (j = 0; j < DFX_MAX_HOST_BRIDGES_PER_SOCKET; j++) {
          if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] == false) {
            DF_TRACEPOINT (SIL_TRACE_INFO, "  socket 0 last root bridge has 2nd MMIO below 4G\n");
            FabricMmioManager->PrimaryRb2ndMmioPairBelow4G = (uint8_t) ((i << 4) | j);
            if (AbovePcieCfgIsTooSmall) {
              MmioBaseAddr = SilData->PciExpressBaseAddress + PciCfgSpace;
              MmioSize = SizeAbovePcieCfg;
              SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2),
                PrimarySocket, PrimaryRootBridge, MmioBaseAddr,
                (MmioSize + BottomOfCompat - SilData->BottomMmioReservedForPrimaryRb));
              ReservedRegionAlreadySet = true;
            } else {
              // for single RootBridge system, primary RootBridge always set the space above PcieCfg as MMIO in step 3
              MmioBaseAddr = TOM;
              MmioSize = SizeBelowPcieCfg;
              SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2),
                PrimarySocket, PrimaryRootBridge, MmioBaseAddr, MmioSize);
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

            DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch +
                FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch);
            DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch +
                FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch);
            DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
              FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
              (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci +
                FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci),
              FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci);

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
  MmioLimitAbove4G -= (uint64_t)xUslGetPhysAddrReduction ();

  MmioLimitAbove4G = (1ull << (MmioLimitAbove4G & 0x3F)) - 0x300000000;

  if (SilData->MmioAbove4GLimit < MmioLimitAbove4G) {
    MmioLimitAbove4G = (SilData->MmioAbove4GLimit + 1) & 0xFFFFFFFFFFFF0000;
  }

  //there's a PCIE hole if PciExpressBaseAddress is above 4G
  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    assert (SilData->PciExpressBaseAddress >= TOM2);
    assert ((SilData->PciExpressBaseAddress + PciCfgSpace) < MmioLimitAbove4G);
    if ((SilData->PciExpressBaseAddress - TOM2) < (MmioLimitAbove4G - (SilData->PciExpressBaseAddress + PciCfgSpace))) {
      MmioBaseAddr = SilData->PciExpressBaseAddress + PciCfgSpace;
    } else {
      MmioLimitAbove4G = SilData->PciExpressBaseAddress;
    }
  }

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
    DF_TRACEPOINT (SIL_TRACE_INFO, "    WARNING: There's no MMIO space above 4G\n");
  } else {
    if (MmioSizeAboveHole > MmioSizeBelowHole) {
      MmioBaseAddr = (TOM2 > 0x10000000000) ? TOM2 : 0x10000000000; // Check if TOM2 > 1T
    } else {
      MmioLimitAbove4G = (MmioLimitAbove4G < 0xFD00000000) ? MmioLimitAbove4G : 0xFD00000000;
    }
  }

  DF_TRACEPOINT (SIL_TRACE_INFO, "    Above 4G MMIO base is 0x%llX, limit is 0x%llX\n",
    MmioBaseAddr, MmioLimitAbove4G);

  if (MmioLimitAbove4G > MmioBaseAddr) {
    MmioSize = MmioLimitAbove4G - MmioBaseAddr;
    MmioSize16MAligned = (MmioSize / SystemRbNumber) & SIZE_16M_ALIGN;
    MmioSizeRemained = MmioSize - (MmioSize16MAligned * (SystemRbNumber - 1));

    for (i = 0; i < SilData->SocketNumber; i++) {
      for (j = 0; j < SilData->RbsPerSocket; j++) {
        FabricMmioManager->AllocateMmioAbove4GOnThisRb[i][j] = true;
        if ((i == (SilData->SocketNumber - 1)) && (j == (SilData->RbsPerSocket - 1))) {
          MmioSize16MAligned = MmioSizeRemained;
        }
        if (MmioSize16MAligned < MMIO_MIN_NON_PCI_SIZE_ABOVE4G) {
          continue;
        }
        if ((i == (SilData->SocketNumber - 1)) && (j == (SilData->RbsPerSocket - 1))) {
          // workaround to set last RootBridge's MMIO limit to 0xFFFF_FFFF_FFFF
          SilSetMmioReg4 (SilData->SocketNumber,
            ((i * SilData->RbsPerSocket + j) * 2 + 1), i, j, MmioBaseAddr, (MmioLimitAbove4G - MmioBaseAddr));
        } else {
          SilSetMmioReg4 (SilData->SocketNumber,
            ((i * SilData->RbsPerSocket + j) * 2 + 1), i, j, MmioBaseAddr, MmioSize16MAligned);
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

        DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci +
            FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci);
        DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch +
            FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch);
        DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n\n",
          FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch,
          (FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch +
            FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch),
          FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch);

        MmioBaseAddr += MmioSize16MAligned;
      }
    }
  }

  //   6. if there's a spare MMIO register pair, set 0xFED0_0000 ~ 0xFED0_FFFF as posted
  SilAdditionalMmioSetting4 (SilData, BottomOfCompat, ReservedRegionAlreadySet);
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
  uint8_t     RegIndex;
  uint32_t    IoBase;
  uint32_t    IoSize;
  uint32_t    IoSizeForThisRb;
  uint32_t    IoBaseForPrimaryRb;
  uint32_t    IoSizeForPrimaryRb;
  uint32_t    IoBaseForOthers;
  uint32_t    LegacyIoSize;
  uint32_t    PrimarySocket;
  uint32_t    PrimaryRootBridge;
  bool        LogToPhyMapInit;
  FABRIC_RB_LOG_TO_PHY_MAP LogToPhyMap[DFX_MAX_HOST_BRIDGES];
  DFX_FABRIC_IO_MANAGER *FabricIoManager;

  FabricIoManager = &SilData->IoRcMgr;

  RegIndex = 0;
  LegacyIoSize = X86_LEGACY_IO_SIZE;
  LogToPhyMapInit = false;
  DF_TRACEPOINT (SIL_TRACE_INFO, "    reserve 0x%X IO size for legacy devices\n", LegacyIoSize);

  // most IO registers have a 16bit limit
  // reserve LegacyIoSize for primary RootBridge
  IoSize = ((0x10000 - LegacyIoSize) / (SilData->SocketNumber * SilData->RbsPerSocket)) & 0xFFFFF000; // 12 bit aligned

  // Get base & size for primary RootBridge
  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

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
        }
      }
    }
  }

  for (LogSktLoop = 0; LogSktLoop < SilData->SocketNumber; LogSktLoop++) {
    for (LogRbLoop = 0; LogRbLoop < SilData->RbsPerSocket; LogRbLoop++) {
      DfXSilGetPhySktRbNum (LogSktLoop, LogRbLoop, &PhySktLoop, &PhyRbLoop, &LogToPhyMapInit, &LogToPhyMap[0]);

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

      SilSetIoReg4 (SilData->SocketNumber, RegIndex, PhySktLoop, PhyRbLoop, IoBase, IoSizeForThisRb);

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

      DF_TRACEPOINT (SIL_TRACE_INFO, "  Socket%x RootBridge%x has IO base 0x%X size 0x%X\n\n",
        PhySktLoop, PhyRbLoop, FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase,
        FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize);

      RegIndex++;
    }
  }

  FabricIoManager->GlobalCtrl = true;

  return SilPass;
}
