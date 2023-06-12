/**
 * @file  FabricRcInitBasedOnNv3.c
 * @brief Fabric MMIO initialization based on NV variable for DFX
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "RcManager4-api.h"
#include <DF/Df.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <MsrReg.h>
#include "FabricRcInitDfX.h"
#include <string.h>

#define MMIO_QUEUE_SIZE 3

static const uint32_t IosRbLoopToRbAssignOrder [] = { 2, 3, 1, 0 };  // IOS2/3/1/0

/**
 * SilInitPciBusBasedOnNvVariable4
 *
 * Initialize PCI bus base/limit for each Root Bridge based on NV variable.
 *
*  @param[in]         SilData     openSIL input block structure for RC manager
 *                      SilData->PciBusRangeForEachRb     How much PCI bus is required for each Root Bridge
 *                      SilData->SocketNumber             System socket count
 *                      SilData->RbsPerSocket             Root Bridge number per socket
 *
 */
SIL_STATUS
SilInitPciBusBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK *SilData
  )
{
  uint32_t  SktLoop;
  uint32_t  DieLoop;
  uint32_t  CCMLoop;
  uint32_t  ACMLoop;
  uint32_t  RbLoop;
  uint32_t  DiePerSkt;
  uint8_t   RegIndex;
  uint16_t  PciBusBase;
  uint16_t  PciBusLimit;
  uint32_t  PrimarySocket;
  uint32_t  PrimaryRootBridge;
  uint32_t  PciBase;
  uint32_t  PciLimit;
  uint32_t  RbPerDie;
  uint32_t  BMCIOM;
  uint32_t  BMCSocket;
  bool      MCTPEnabled;
  bool      NeedReallocate;
  CFG_ADDRESS_CNTL_REGISTER         CfgAddrCtrl;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *CcmEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IomEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *AcmEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  DFX_FABRIC_RESOURCE_FOR_EACH_RB  *PciBusRangeForEachRb;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  PciBusRangeForEachRb = &SilData->ResourceSizeForEachRb;

  // Check if we need to re-allocate PCI bus base/limit
  NeedReallocate = false;

  RbPerDie = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie (0);
  for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
    for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
      PciBase = DfIp2IpApi->DfGetHostBridgeBusBase (SktLoop, (RbLoop % RbPerDie));
      PciLimit = DfIp2IpApi->DfGetHostBridgeBusLimit (SktLoop, (RbLoop % RbPerDie));
      if ((PciLimit - PciBase + 1) != PciBusRangeForEachRb->PciBusNumber[SktLoop][RbLoop]) {
        NeedReallocate = true;
        break;
      }
    }
  }

  if (NeedReallocate) {

    MCTPEnabled = SilData->McptEnable;
    BMCSocket = SilData->BmcSocket;
    BMCIOM = SilData->EarlyBmcLinkLaneNum;
    if ((BMCIOM < 16) || (BMCIOM == 128) || (BMCIOM == 129))
      BMCIOM = 0;
    else if (BMCIOM < 64)
      BMCIOM = 1;
    else if (BMCIOM < 96)
      BMCIOM = 3;
    else
      BMCIOM = 2;

    CcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Ccm);
    assert (CcmEntry != NULL);
    IomEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Iom);
    assert (IomEntry != NULL);
    AcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Acm);
    assert (AcmEntry != NULL);
    IosEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Ios);
    assert (IosEntry != NULL);

    // Clear all Configuration Address Map registers (up to 8)
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Init PCI bus base/limit based on NV variable\n");
    DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket ();
    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      for (DieLoop = 0; DieLoop < DiePerSkt; DieLoop++) {
        for (CCMLoop = 0; CCMLoop < CcmEntry->Count; CCMLoop++) {
          for (RegIndex = 0; RegIndex < 8; RegIndex++) {
            DfIp2IpApi->DfFabricRegisterAccWrite (SktLoop, CFGBASEADDRESS_0_FUNC,
              (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
              CcmEntry->IDs[CCMLoop].InstanceID, 0);
          }
        }
        for (ACMLoop = 0; ACMLoop < AcmEntry->Count; ACMLoop++) {
          for (RegIndex = 0; RegIndex < 8; RegIndex++) {
            DfIp2IpApi->DfFabricRegisterAccWrite (SktLoop, CFGBASEADDRESS_0_FUNC,
              (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
              AcmEntry->IDs[ACMLoop].InstanceID, 0);
          }
        }
        for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
          for (RegIndex = 0; RegIndex < 8; RegIndex++) {
            if ((!MCTPEnabled) || ((MCTPEnabled) && (SktLoop == BMCSocket && RbLoop == BMCIOM))) {
              DfIp2IpApi->DfFabricRegisterAccWrite (SktLoop, CFGBASEADDRESS_0_FUNC,
                (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
                IomEntry->IDs[RbLoop].InstanceID, 0);
            }
          }
          DfIp2IpApi->DfFabricRegisterAccWrite (SktLoop, CFGADDRESSCNTL_FUNC, CFGADDRESSCNTL_REG,
            IosEntry->IDs[RbLoop].InstanceID, 0);
        }
      }
    }

    // Set Configuration Address Map registers
    SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

    RegIndex = 0;
    PciBusBase = 0;
    PciBusLimit = (uint16_t) (PciBusBase + PciBusRangeForEachRb->PciBusNumber[PrimarySocket][PrimaryRootBridge] - 1);
    SilSetCfgAddrMapReg4 (
      SilData, (uint8_t) RbPerDie, RegIndex, PrimarySocket, PrimaryRootBridge, PciBusBase, PciBusLimit);
    CfgAddrCtrl.Value = 0;
    CfgAddrCtrl.Field.SecBusNum = PciBusBase;
    DfIp2IpApi->DfFabricRegisterAccWrite (
      (uint32_t)PrimarySocket, CFGADDRESSCNTL_FUNC, CFGADDRESSCNTL_REG,
        IosEntry->IDs[PrimaryRootBridge].InstanceID, CfgAddrCtrl.Value);

    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      for (RbLoop = SilData->RbsPerSocket; RbLoop > 0; RbLoop--) {
        if ((SktLoop == PrimarySocket) && (IosRbLoopToRbAssignOrder[RbLoop - 1] == PrimaryRootBridge)) {
          continue;
        }
        RegIndex++;
        PciBusBase = PciBusLimit + 1;
        if ((SktLoop == (uint32_t)(SilData->SocketNumber - 1)) && (RbLoop == 1)) {
          PciBusLimit = 0xFF;
        } else {
          PciBusLimit = (uint16_t) (PciBusBase +
            PciBusRangeForEachRb->PciBusNumber[SktLoop][IosRbLoopToRbAssignOrder[RbLoop - 1]] - 1);
        }
        SilSetCfgAddrMapReg4 (SilData, (uint8_t) RbPerDie, RegIndex, SktLoop, IosRbLoopToRbAssignOrder[RbLoop - 1],
          PciBusBase, PciBusLimit);
        CfgAddrCtrl.Value = 0;
        CfgAddrCtrl.Field.SecBusNum = PciBusBase;
        DfIp2IpApi->DfFabricRegisterAccWrite (
          (uint32_t)SktLoop, CFGADDRESSCNTL_FUNC, CFGADDRESSCNTL_REG,
          IosEntry->IDs[IosRbLoopToRbAssignOrder[RbLoop - 1]].InstanceID, CfgAddrCtrl.Value);
      }
    }
  }

  return SilPass;
}

/**
 * SilInitIoBasedOnNvVariable4
 *
 * @brief Initialize IO registers for each Root Bridge based on NV variable.
 *
 * @param[in]         SilData             Point to DFX_RCMGR_INPUT_BLK
 *                      SilData->FabricIoManager     Point to DFX_FABRIC_IO_MANAGER
 *                      SilData->IoSizeForEachRb     How much IO size is required for each Root Bridge
 *                      SilData->SocketNumber        System socket count
 *                      SilData->RbsPerSocket        Root Bridge number per socket
 * @param[in, out]    SpaceStaus          Current status
 * @param[in]         SetDfRegisters      true  - Set DF MMIO registers
 *                                        false - Do Not set DF MMIO registers, just calculate
 *                                                if user's requirment could be satisfied.
 *
 */
SIL_STATUS
SilInitIoBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  bool                    SetDfRegisters
  )
{
  uint32_t  PhySktLoop;
  uint32_t  PhyRbLoop;
  uint32_t  LogSktLoop;
  uint32_t  LogRbLoop;
  uint8_t   RegIndex;
  uint32_t  IoBase;
  uint32_t  IoSize;
  uint32_t  IoBaseForPrimaryRb;
  uint32_t  IoSizeForPrimaryRb;
  uint32_t  IoBaseForOthers;
  uint32_t  LegacyIoSize;
  uint32_t  PrimarySocket;
  uint32_t  PrimaryRootBridge;
  bool      LogToPhyMapInit;
  FABRIC_RB_LOG_TO_PHY_MAP LogToPhyMap[MAX_SOCKETS_SUPPORTED * DFX_MAX_HOST_BRIDGES_PER_SOCKET];
  DFX_FABRIC_RESOURCE_FOR_EACH_RB  *IoSizeForEachRb;
  DFX_FABRIC_IO_MANAGER *FabricIoManager;
  IoSizeForEachRb = &SilData->ResourceSizeForEachRb;
  FabricIoManager = &SilData->IoRcMgr;

  RegIndex = 0;
  LegacyIoSize = X86_LEGACY_IO_SIZE;
  LogToPhyMapInit = false;

  IoBaseForPrimaryRb = 0;
  IoSizeForPrimaryRb = DF_IO_LIMIT - IoBaseForPrimaryRb;
  IoBase = 0;
  IoBaseForOthers = 0;

  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);
  if ((SilData->SocketNumber != 1) || (SilData->RbsPerSocket != 1)) {
    // If we have more than one RootBridge
    // find out primary RootBridge and reserve an IO region start from 0 for it
    for (PhySktLoop = 0; PhySktLoop < SilData->SocketNumber; PhySktLoop++) {
      for (PhyRbLoop = 0; PhyRbLoop < SilData->RbsPerSocket; PhyRbLoop++) {
        if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
          IoSizeForPrimaryRb =
		    (uint32_t) (IoSizeForEachRb->IO[PhySktLoop][PhyRbLoop].Size & RCMGR_IO_SIZE_MASK) + LegacyIoSize;
          IoBaseForOthers = IoBaseForPrimaryRb + IoSizeForPrimaryRb;
        }
      }
    }
  }

  DF_TRACEPOINT (SIL_TRACE_INFO, "   reserve 0x%X IO size for legacy devices\n", LegacyIoSize);

  for (LogSktLoop = 0; LogSktLoop < SilData->SocketNumber; LogSktLoop++) {
    for (LogRbLoop = 0; LogRbLoop < SilData->RbsPerSocket; LogRbLoop++) {
      DfXSilGetPhySktRbNum (LogSktLoop, LogRbLoop, &PhySktLoop, &PhyRbLoop, &LogToPhyMapInit, &LogToPhyMap[0]);
      if (SpaceStatus != NULL) {
        SpaceStatus->IoSize += (uint32_t) IoSizeForEachRb->IO[PhySktLoop][PhyRbLoop].Size & RCMGR_IO_SIZE_MASK;
      }

      if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
        IoBase = IoBaseForPrimaryRb;
        IoSize = IoSizeForPrimaryRb;
      } else {
        IoBase = IoBaseForOthers;
        IoSize = (uint32_t) IoSizeForEachRb->IO[PhySktLoop][PhyRbLoop].Size & RCMGR_IO_SIZE_MASK;
        IoBaseForOthers += IoSize;
      }

      if (((LogSktLoop + 1) == SilData->SocketNumber) && ((LogRbLoop + 1) == SilData->RbsPerSocket)) {
        // check if it's the last RB. DF's IO space is up to 25 bits, so set limit to 0x1FFFFFF for last RB
        IoSize = DF_IO_LIMIT - IoBase;
      }

      assert (IoBase < X86IO_LIMIT);

      if (IoSize == 0) {
        continue;
      }

      if ((FabricIoManager != NULL) && SetDfRegisters) {
        SilSetIoReg4 (SilData->SocketNumber, RegIndex, PhySktLoop, PhyRbLoop, IoBase, IoSize);

        if ((PhySktLoop == PrimarySocket) && (PhyRbLoop == PrimaryRootBridge)) {
          // This root bridge has an additional IO region for legacy devices, size is LegacyIoSize
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase = IoBase + LegacyIoSize;
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize = IoSize - LegacyIoSize;
        } else {
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoBase = IoBase;
          FabricIoManager->IoRegion[PhySktLoop][PhyRbLoop].IoSize = IoSize;
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
      }

      RegIndex++;
    }
  }

  if (SpaceStatus != NULL) {
    if (SpaceStatus->IoSize > X86IO_LIMIT) {
      SpaceStatus->IoSizeReqInc = SpaceStatus->IoSize - X86IO_LIMIT;
    } else {
      SpaceStatus->IoSizeReqInc = 0;
    }
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Space Status: IoSize %X, IoSizeReqInc %X\n",
      SpaceStatus->IoSize, SpaceStatus->IoSizeReqInc);
  }

  if (FabricIoManager != NULL) {
    FabricIoManager->GlobalCtrl = true;
  }

  return SilPass;
}

/**
 * SilFabricGetResourceDistribution
 *
 * @brief Fills in the given memory block with the resource distribution data
 *
 * @param[in]         SilData               DFX_RCMGR_INPUT_BLK
 * @param[in, out]    ResourceDistribution  Array of booleans to be filled with the data
 *
 */
static
SIL_STATUS
SilFabricGetResourceDistribution (
  DFX_RCMGR_INPUT_BLK     *SilData,
  bool                    *ResourceDistribution
)
{
  memcpy (ResourceDistribution, &SilData->ResourceDistributionNv, sizeof(bool) * DFX_MAX_HOST_BRIDGES);
  return SilPass;
}

/**
 * SilFabricSetResourceDistribution
 *
 * @brief Sets the resource distribution memory block with the provided data
 *
 * @param[in]    SilData               DFX_RCMGR_INPUT_BLK
 * @param[in]    ResourceDistribution  Array of booleans to be use for setting the data
 *
 */
static
SIL_STATUS
SilFabricSetResourceDistribution (
  DFX_RCMGR_INPUT_BLK     *SilData,
  bool                    *ResourceDistribution
)
{
  if (SilData == NULL || ResourceDistribution == NULL) return SilInvalidParameter;
  return SilUnsupported;
}

static
bool
SilArrangeMmioBelow4G (
  DFX_RCMGR_INPUT_BLK     *SilData,
  uint64_t                MmioBaseAddrAbovePcieCfg,
  uint64_t                MmioBaseAddrBelowPcieCfg,
  bool                    *MmioIsAbovePcieCfg,
  uint8_t                 PrimarySocket,
  uint8_t                 PrimaryRootBridge,
  bool                    SetDfRegisters,
  uint32_t                *OverSizeBelowPcieMin,
  uint32_t                *AlignmentMask,
  bool                    *EnoughAbovePcieSpaceForPrimaryRb
  );

static
bool
SilTryThisCombination (
  DFX_RCMGR_INPUT_BLK     *SilData,
  uint64_t                MmioBaseAddrAbovePcieCfg,
  uint64_t                MmioBaseAddrBelowPcieCfg,
  bool                    *MmioIsAbovePcieCfg,
  bool                    SetDfRegisters,
  uint32_t                *OverSizeBelowPcieMin,
  uint32_t                *AlignmentMask
  );

static
bool
SilArrangeMmioAbove4G (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  uint64_t                *MmioBaseAddrNextRb,
  uint64_t                MmioLimitAbove4G,
  bool                    SetDfRegisters
  );

/**
 * SilInitMmioBasedOnNvVariable4
 *
 * @brief Initialize MMIO registers for each Root Bridge based on NV variable.
 *
 * @param[in]         SilData        Pointer to DFX_RCMGR_INPUT_BLK
 *                      SilData->FabricMmioManager        Point to DFX_FABRIC_MMIO_MANAGER
 *                      SilData->MmioSizeForEachRb        How much MMIO size is required for each Root Bridge
 *                      SilData->SocketNumber             System socket count
 *                      SilData->RbsPerSocket             Root Bridge number per socket
 * @param[in, out]    SpaceStaus               Current status
 * @param[in]         SetDfRegisters           true  - Set DF MMIO registers
 *                                             false - Do Not set DF MMIO registers, just calculate
 *                                                     if user's requirment could be satisfied.
 *
 */
SIL_STATUS
SilInitMmioBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  bool                    SetDfRegisters
  )
{
  uint64_t MmioLimitAbove4G;
  uint64_t MmioBaseAddrNextRb;
  uint64_t MmioBaseAddrAbovePcieCfg;
  uint64_t MmioBaseAddrBelowPcieCfg;
  uint64_t MmioLimitAbovePcieCfg;
  uint64_t TOM;
  uint64_t TOM2;
  uint64_t PciCfgSpace;
  uint64_t MmioSizeBelowHole;
  uint64_t MmioSizeAboveHole;
  uint64_t SizeNonPci;
  uint32_t PrimarySocket;
  uint32_t PrimaryRootBridge;
  SIL_STATUS Status;
  bool MmioIsAbovePcieCfg[DFX_MAX_HOST_BRIDGES];
  bool LastCombinationWork;
  bool EnoughAbovePcieSpaceForPrimaryRb;
  uint32_t OverSizeBelowPcieMin;
  uint32_t AlignmentMask;
  DFX_FABRIC_RESOURCE_FOR_EACH_RB  *MmioSizeForEachRb;

  Status = SilPass;
  MmioSizeForEachRb = &SilData->ResourceSizeForEachRb;

  OverSizeBelowPcieMin = 0xFFFFFFFF;
  AlignmentMask = 0;
  EnoughAbovePcieSpaceForPrimaryRb = false;

  // System information
  PciCfgSpace = (xUslRdMsr (0xC0010058) >> 2) & 0xF;            // Get bus range from MSR_C001_0058[5:2][BusRange]
  PciCfgSpace = ((uint64_t) 1 << PciCfgSpace) * (1024 * 1024);  // 1MB times the number of buses
  TOM = xUslRdMsr (MSR_TOM);
  TOM2 = xUslRdMsr (MSR_TOM2);
  DF_TRACEPOINT (SIL_TRACE_INFO, "  TOM: 0x%llX, TOM2: 0x%llX, Pcie configuration space: 0x%llX ~ 0x%llX\n",
        TOM, TOM2, SilData->PciExpressBaseAddress, SilData->PciExpressBaseAddress + PciCfgSpace);

  if (SilData->PciExpressBaseAddress < 0x100000000) {
    assert (SilData->BottomMmioReservedForPrimaryRb >= (SilData->PciExpressBaseAddress + PciCfgSpace));
    assert (SilData->PciExpressBaseAddress >= TOM);
  }

  // Calculate size of above 4G
  MmioBaseAddrNextRb = (TOM2 > 0x100000000)? TOM2 : 0x100000000; // Check if TOM2 > 4G
  MmioLimitAbove4G = (uint64_t)xUslGetPhysAddrSize ();
  MmioLimitAbove4G -= (uint64_t)xUslGetPhysAddrReduction ();
  MmioLimitAbove4G = (1ull << MmioLimitAbove4G) - 0x300000000;

  if (SilData->MmioAbove4GLimit  < MmioLimitAbove4G) {
    MmioLimitAbove4G = (SilData->MmioAbove4GLimit + 1) & 0xFFFFFFFFFFFF0000;
    if (MmioLimitAbove4G <= MmioBaseAddrNextRb) {
      MmioLimitAbove4G = MmioBaseAddrNextRb;
    }
  }

  //there's a PCIE hole if PciExpressBaseAddress is above 4G
  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    assert (SilData->PciExpressBaseAddress >= TOM2);
    assert ((SilData->PciExpressBaseAddress + PciCfgSpace) < MmioLimitAbove4G);
    if ((SilData->PciExpressBaseAddress - TOM2) < (MmioLimitAbove4G - (SilData->PciExpressBaseAddress + PciCfgSpace))) {
      MmioBaseAddrNextRb = SilData->PciExpressBaseAddress + PciCfgSpace;
    } else {
      MmioLimitAbove4G = SilData->PciExpressBaseAddress;
    }
  }

  // there's a hole at 0xFD_0000_0000 ~ 0x100_0000_0000
  MmioSizeBelowHole = 0;
  MmioSizeAboveHole = 0;

  // calculate Mmio size below/above this hole
  if (MmioBaseAddrNextRb < 0xFD00000000) {
    MmioSizeBelowHole = (MmioLimitAbove4G < 0xFD00000000) ? MmioLimitAbove4G : 0xFD00000000;
    MmioSizeBelowHole = MmioSizeBelowHole - MmioBaseAddrNextRb;
  }

  if (MmioLimitAbove4G > 0x10000000000) {
    MmioSizeAboveHole = (MmioBaseAddrNextRb > 0x10000000000)? MmioBaseAddrNextRb : 0x10000000000;
    MmioSizeAboveHole = MmioLimitAbove4G - MmioSizeAboveHole;
  }

  if ((MmioSizeBelowHole == 0) && (MmioSizeAboveHole == 0)) {
    MmioLimitAbove4G = 0; // set limit to 0, so there's no MMIO space above 4G
    DF_TRACEPOINT (SIL_TRACE_WARNING, "    There's no MMIO space above 4G\n");
  } else {
    if (MmioSizeAboveHole > MmioSizeBelowHole) {
      MmioBaseAddrNextRb = (TOM2 > 0x10000000000) ? TOM2 : 0x10000000000; // Check if TOM2 > 1T
    } else {
      MmioLimitAbove4G = (MmioLimitAbove4G < 0xFD00000000) ? MmioLimitAbove4G : 0xFD00000000;
    }
  }

  DF_TRACEPOINT (SIL_TRACE_INFO, "    Above 4G MMIO base is 0x%llX, limit is 0x%llX\n",
    MmioBaseAddrNextRb, MmioLimitAbove4G);

  if (!SilArrangeMmioAbove4G (SilData, SpaceStatus, &MmioBaseAddrNextRb, MmioLimitAbove4G, SetDfRegisters)) {
    return SilOutOfResources;
  }

  // Calculate size of below 4G
  if (SilData->PciExpressBaseAddress >= 0x100000000) {
    MmioBaseAddrAbovePcieCfg = SilData->BottomMmioReservedForPrimaryRb;
    MmioBaseAddrBelowPcieCfg = TOM;
    MmioLimitAbovePcieCfg    = SilData->BottomMmioReservedForPrimaryRb;
  } else {
    MmioBaseAddrAbovePcieCfg = SilData->PciExpressBaseAddress + PciCfgSpace;
    MmioBaseAddrBelowPcieCfg = TOM;
    MmioLimitAbovePcieCfg    = SilData->BottomMmioReservedForPrimaryRb;
  }

  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

  SizeNonPci = SilData->MmioSizePerRbForNonPciDevice;
  if ((MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[PrimarySocket][PrimaryRootBridge].Size +
        MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[PrimarySocket][PrimaryRootBridge].Size + SizeNonPci) <=
        (MmioLimitAbovePcieCfg - MmioBaseAddrAbovePcieCfg)) {
    EnoughAbovePcieSpaceForPrimaryRb = true;
  }

  LastCombinationWork = false;
  if (SilFabricGetResourceDistribution (SilData, &MmioIsAbovePcieCfg[0]) == SilPass) {
    // Get distribution information from NV, try it first
    if (SilTryThisCombination (SilData, MmioBaseAddrAbovePcieCfg, MmioBaseAddrBelowPcieCfg, MmioIsAbovePcieCfg,
                            SetDfRegisters, &OverSizeBelowPcieMin, &AlignmentMask)) {
      // It works! No need to find out a new combination that which RootBridge is above Pcie Cfg
      DF_TRACEPOINT (SIL_TRACE_INFO, "  Use combination of RB resources from NV.\n");
      LastCombinationWork = true;
    }
  }

  if (!LastCombinationWork) {
    if (SilArrangeMmioBelow4G (SilData, MmioBaseAddrAbovePcieCfg, MmioBaseAddrBelowPcieCfg, MmioIsAbovePcieCfg,
                            (uint8_t) PrimarySocket, (uint8_t) PrimaryRootBridge, SetDfRegisters,
                            &OverSizeBelowPcieMin, &AlignmentMask, &EnoughAbovePcieSpaceForPrimaryRb)) {
      DF_TRACEPOINT (SIL_TRACE_INFO, "  Save combination to NV\n");
      SilFabricSetResourceDistribution (SilData, &MmioIsAbovePcieCfg[0]);
    } else {
      if (EnoughAbovePcieSpaceForPrimaryRb) {
        EnoughAbovePcieSpaceForPrimaryRb = false;
        if (SilArrangeMmioBelow4G (SilData, MmioBaseAddrAbovePcieCfg, MmioBaseAddrBelowPcieCfg, MmioIsAbovePcieCfg,
                                (uint8_t) PrimarySocket, (uint8_t) PrimaryRootBridge, SetDfRegisters,
                                &OverSizeBelowPcieMin, &AlignmentMask, &EnoughAbovePcieSpaceForPrimaryRb)) {
          DF_TRACEPOINT (SIL_TRACE_INFO, "  Save combination to NV\n");
          SilFabricSetResourceDistribution (SilData, &MmioIsAbovePcieCfg[0]);
        } else {
          DF_TRACEPOINT (SIL_TRACE_WARNING, "  Not enough recources below 4G\n");
          Status = SilOutOfResources;
        }
      } else {
        DF_TRACEPOINT (SIL_TRACE_WARNING, "  Not enough resources below 4G\n");
        Status = SilOutOfResources;
      }
    }
  }

  // Update SpaceStatus
  if (SpaceStatus != NULL) {
    assert (OverSizeBelowPcieMin != 0xFFFFFFFF);
    SpaceStatus->MmioSizeAbove4GReqInc = (MmioBaseAddrNextRb > MmioLimitAbove4G) ?
      (MmioBaseAddrNextRb - MmioLimitAbove4G) : 0;
    SpaceStatus->MmioSizeBelow4GReqInc = OverSizeBelowPcieMin;
    if (SpaceStatus->MmioSizeBelow4GReqInc != 0) {
      SpaceStatus->MmioSizeBelow4GReqInc = (SpaceStatus->MmioSizeBelow4GReqInc + AlignmentMask) & (~AlignmentMask);
    }
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Space Status: MmioSizeAbove4G %llX, MmioSizeAbove4GReqInc %llX\n",
      SpaceStatus->MmioSizeAbove4G, SpaceStatus->MmioSizeAbove4GReqInc);
    DF_TRACEPOINT (SIL_TRACE_INFO, "  Space Status: MmioSizeBelow4G %llX, MmioSizeBelow4GReqInc %llX\n",
      SpaceStatus->MmioSizeBelow4G, SpaceStatus->MmioSizeBelow4GReqInc);
  }

  return Status;
}

/**
 * SilArrangeMmioAbove4G
 *
 * @brief Try to arrange MMIO above 4G
 *
 * @param[in]         SilData     openSIL input block structure for RC manager
 *                      SilData->MmioSizeForEachRb   Required MMIO size for each RootBridge
 *                      SilData->SocketNumber        System socket count
 *                      SilData->RbsPerSocket        RootBridge number per socket
 *                      SilData->MmioRcMgr           Point to DFX_FABRIC_MMIO_MANAGER
 * @param[in, out]    SpaceStatus                 Current status
 * @param[in, out]    MmioBaseAddrNextRb          MmioBaseAddr for next RB, be used to check overflow
 * @param[in]         MmioLimitAbove4G            MmioLimitAbove4G
 * @param[in]         SetDfRegisters              true  - Set DF MMIO registers
 *                                                false - Do Not set DF MMIO registers, just calculate if
 *                                                        user's requirment could be satisfied.
 *
 * @retval            true
 *                    false
 */
bool
SilArrangeMmioAbove4G (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  uint64_t                *MmioBaseAddrNextRb,
  uint64_t                MmioLimitAbove4G,
  bool                    SetDfRegisters
  )
{
  uint8_t  i;
  uint8_t  j;
  uint8_t  SktLoop;
  uint8_t  RbLoop;
  uint64_t MmioBaseAddr;
  uint64_t MmioBaseAddrPrefetchable;
  uint64_t MmioBaseAddrNonPrefetchable;
  uint64_t MmioBaseAddrNonPci;
  uint64_t SizePrefetchable;
  uint64_t SizeNonPrefetchable;
  uint64_t SizeNonPciAbove4G;
  uint64_t AlignMask;
  uint64_t AlignMaskP;
  uint64_t AlignMaskNonPci;
  bool EnoughSpaceAbove4G;
  bool BigAlignFirst;
  FABRIC_MMIO_ABOVE_4G_QUEUE  MmioQueue[MMIO_QUEUE_SIZE];
  FABRIC_MMIO_ABOVE_4G_QUEUE  Temp;
  DFX_FABRIC_RESOURCE_FOR_EACH_RB *MmioSizeForEachRb;
  DFX_FABRIC_MMIO_MANAGER *FabricMmioManager;

  FabricMmioManager = &SilData->MmioRcMgr;
  MmioSizeForEachRb = &SilData->ResourceSizeForEachRb;

  EnoughSpaceAbove4G = true;
  BigAlignFirst = true;
  AlignMaskNonPci = RCMGR_NON_PCI_MMIO_ALIGN_MASK_ABOVE4G;
  MmioBaseAddrPrefetchable = 0;
  MmioBaseAddrNonPrefetchable = 0;
  MmioBaseAddrNonPci = 0;
  SizePrefetchable = 0;
  SizeNonPrefetchable = 0;
  SizeNonPciAbove4G = (SilData->Above4GMmioSizePerRbForNonPciDevice > MMIO_MIN_NON_PCI_SIZE_ABOVE4G) ?
    SilData->Above4GMmioSizePerRbForNonPciDevice: MMIO_MIN_NON_PCI_SIZE_ABOVE4G;
  DF_TRACEPOINT (SIL_TRACE_INFO, " # of sockets %d, RB per skt %d\n", SilData->SocketNumber, SilData->RbsPerSocket);
  for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
    for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
      // Calculate reqiured size, it's for output parameter 'SpaceStatus'
      if (SpaceStatus != NULL) {
        SpaceStatus->MmioSizeAbove4G +=
          MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size +
            MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size;
        SpaceStatus->MmioSizeBelow4G +=
          (uint32_t) (MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SktLoop][RbLoop].Size +
            MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SktLoop][RbLoop].Size);
      }

      // If there's no MMIO request for above 4G, try next one
      if ((MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size +
            MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size + SizeNonPciAbove4G) == 0) {
        continue;
      }

      AlignMask  = MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment;
      AlignMaskP = MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment;
      MmioBaseAddr = *MmioBaseAddrNextRb;

      // default sequence should be Non Pci -> Prefetchable -> Non Prefetchable
      // this would match the sequence used in FabricInitMmioEqually3 ()
      i = 0;
      MmioQueue[i].MmioType = NON_PCI_DEVICE_ABOVE_4G;
      MmioQueue[i++].AlignBit = AlignMaskNonPci;
      MmioQueue[i].MmioType = P_MMIO_ABOVE_4G;
      MmioQueue[i++].AlignBit = AlignMaskP;
      MmioQueue[i].MmioType = MMIO_ABOVE_4G;
      MmioQueue[i++].AlignBit = AlignMask;
      assert (i <= MMIO_QUEUE_SIZE);

      if (BigAlignFirst) {
        // MmioQueue[0] has the biggest align bit
        for (i = 0; i < MMIO_QUEUE_SIZE; i++) {
          for (j = 0; j < (MMIO_QUEUE_SIZE - i - 1); j++) {
            if (MmioQueue[j].AlignBit < MmioQueue[j + 1].AlignBit) {
              Temp = MmioQueue[j + 1];
              MmioQueue[j + 1] = MmioQueue[j];
              MmioQueue[j] = Temp;
            }
          }
        }
      } else {
        // MmioQueue[0] has the smallest align bit (except align bit = 0)
        // always put MMIO region whose align bit is 0 on high address
        // this would match the algorithm used in FabricInitMmioEqually3 ()
        for (i = 0; i < MMIO_QUEUE_SIZE; i++) {
          for (j = 0; j < (MMIO_QUEUE_SIZE - i - 1); j++) {
            if (((MmioQueue[j].AlignBit > MmioQueue[j + 1].AlignBit) || (MmioQueue[j].AlignBit == 0)) &&
                (MmioQueue[j + 1].AlignBit != 0)) {
              Temp = MmioQueue[j + 1];
              MmioQueue[j + 1] = MmioQueue[j];
              MmioQueue[j] = Temp;
            }
          }
        }
      }
      BigAlignFirst = !BigAlignFirst;

      MmioBaseAddr = *MmioBaseAddrNextRb;
      for (i = 0; i < MMIO_QUEUE_SIZE; i++) {
        switch (MmioQueue[i].MmioType) {
        case MMIO_ABOVE_4G:
          MmioBaseAddrNonPrefetchable = (*MmioBaseAddrNextRb + AlignMask) & (~AlignMask);
          SizeNonPrefetchable = MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size;
          *MmioBaseAddrNextRb = MmioBaseAddrNonPrefetchable + SizeNonPrefetchable;
          break;
        case P_MMIO_ABOVE_4G:
          MmioBaseAddrPrefetchable = (*MmioBaseAddrNextRb + AlignMaskP) & (~AlignMaskP);
          SizePrefetchable = MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size;
          *MmioBaseAddrNextRb = MmioBaseAddrPrefetchable + SizePrefetchable;
          break;
        case NON_PCI_DEVICE_ABOVE_4G:
          MmioBaseAddrNonPci = (*MmioBaseAddrNextRb + AlignMaskNonPci) & (~AlignMaskNonPci);
          *MmioBaseAddrNextRb = MmioBaseAddrNonPci + SizeNonPciAbove4G;
          break;
        default:
          assert (false);
          break;
        };
      }

      if (SizeNonPrefetchable == 0) {
        MmioBaseAddrNonPrefetchable = 0;
      }
      if (SizePrefetchable == 0) {
        MmioBaseAddrPrefetchable = 0;
      }

      // Check if space is enough
      if (*MmioBaseAddrNextRb > MmioLimitAbove4G) {
        EnoughSpaceAbove4G = false;
        DF_TRACEPOINT (SIL_TRACE_INFO, "  Insuffecient space for MMIO above 4GB.\n");
      }

      if (SetDfRegisters && EnoughSpaceAbove4G && (FabricMmioManager != NULL)) {
        //   Set MMIO above 4G
        if ((MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size == 0) &&
            (MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size == 0) &&
            (SizeNonPciAbove4G == 0)) {
          continue;
        }

        DF_TRACEPOINT (SIL_TRACE_INFO, "---Socket%x RootBridge%x---\n", SktLoop, RbLoop);
        DF_TRACEPOINT (SIL_TRACE_INFO, "  Request above 4G\n");
        DF_TRACEPOINT (SIL_TRACE_INFO, "        Prefetch Size 0x%llX, AlignBit 0x%X\n",
          MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size,
          MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment);
        DF_TRACEPOINT (SIL_TRACE_INFO, "    Non Prefetch Size 0x%llX, AlignBit 0x%X\n\n",
          MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Size,
          MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment);
        SilSetMmioReg4 (SilData->SocketNumber, ((SktLoop * SilData->RbsPerSocket + RbLoop) * 2 + 1),
          SktLoop, RbLoop, MmioBaseAddr, (*MmioBaseAddrNextRb - MmioBaseAddr));
        FabricMmioManager->AllocateMmioAbove4GOnThisRb[SktLoop][RbLoop] = true;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPci = MmioBaseAddrNonPci;  // No Non Pci MMIO >4G
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BasePrefetch = MmioBaseAddrPrefetchable;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPrefetch = MmioBaseAddrNonPrefetchable;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPci = SizeNonPciAbove4G;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizePrefetch = SizePrefetchable;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPrefetch = SizeNonPrefetchable;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].UsedSizeNonPci = 0;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].UsedSizePrefetch = 0;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].UsedSizeNonPrefetch = 0;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].AlignNonPrefetch =
          MmioSizeForEachRb->NonPrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment;
        FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].AlignPrefetch =
          MmioSizeForEachRb->PrefetchableMmioSizeAbove4G[SktLoop][RbLoop].Alignment;

        DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPci,
          (FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPci +
            FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPci),
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPci);
        DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BasePrefetch,
          (FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BasePrefetch +
            FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizePrefetch),
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizePrefetch);
        DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n\n",
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPrefetch,
          (FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].BaseNonPrefetch +
            FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPrefetch),
          FabricMmioManager->MmioRegionAbove4G[SktLoop][RbLoop].SizeNonPrefetch);
      }
    }
  }

  return EnoughSpaceAbove4G;
}

static
bool
SilGetNextCombination (
  uint8_t   NumberOfRbBelowPcieCfg,
  bool      *MmioIsAbovePcieCfg,
  uint8_t   SocketNumber,
  uint8_t   RbsPerSocket
                       );

/**
 * SilArrangeMmioBelow4G
 *
 * @brief Try to arrange MMIO below 4G
 *
 *
 * @param[in]         SilData                     openSIL input block structure for RC manager
 * @param[in]         MmioBaseAddrAbovePcieCfg    MmioBaseAddrAbovePcieCfg
 * @param[in]         MmioBaseAddrBelowPcieCfg    MmioBaseAddrBelowPcieCfg
 * @param[in, out]    MmioIsAbovePcieCfg          An bool array, indicate which RootBridge's MMIO is above Pcie Cfg
 * @param[in]         PrimarySocket               Which socket has the primary root bridge
 * @param[in]         PrimaryRootBridge           Primary root bridge indxe
 * @param[in]         SetDfRegisters              true  - Set DF MMIO registers
 *                                                false - Do Not set DF MMIO registers, just calculate if
 *                                                        user's requirment could be satisfied.
 * @param[in, out]    OverSizeBelowPcieMin        OverSizeBelowPcieMin
 * @param[in, out]    AlignmentMask               AlignmentMask
 * @param[in]         EnoughAbovePcieSpaceForPrimaryRb  There's enough space above PCIE for primary root bridge
 *
 * @retval            true
 *                    false
 */
static
bool
SilArrangeMmioBelow4G (
  DFX_RCMGR_INPUT_BLK     *SilData,
  uint64_t                MmioBaseAddrAbovePcieCfg,
  uint64_t                MmioBaseAddrBelowPcieCfg,
  bool                    *MmioIsAbovePcieCfg,
  uint8_t                 PrimarySocket,
  uint8_t                 PrimaryRootBridge,
  bool                    SetDfRegisters,
  uint32_t                *OverSizeBelowPcieMin,
  uint32_t                *AlignmentMask,
  bool                    *EnoughAbovePcieSpaceForPrimaryRb
  )
{
  uint8_t   NumberOfRbBelowPcieCfg;
  bool      GetAnCombination;
  bool      NextCombination;

  GetAnCombination = false;

  // Try to put 0 RootBridge, 1 RootBridge, 2 RootBridges... MAX_SYSTEM_RB_COUNT RootBridges below Pcie Cfg
  for (NumberOfRbBelowPcieCfg = 0;
        NumberOfRbBelowPcieCfg <= (SilData->SocketNumber * SilData->RbsPerSocket); NumberOfRbBelowPcieCfg++) {
    // 1. Default, all RootBridges are above Pcie Cfg
    memset (MmioIsAbovePcieCfg, true, (sizeof (bool) * (DFX_MAX_HOST_BRIDGES)));
    NextCombination = true;

    // 2. First, try to put RootBridge 7 to RootBridge (VH_MAX_SYSTEM_RB_COUNT - NumberOfRbBelowPcieCfg) below Pcie Cfg
    if (NumberOfRbBelowPcieCfg > 0 ) {
      memset ((MmioIsAbovePcieCfg + (SilData->SocketNumber * SilData->RbsPerSocket) - NumberOfRbBelowPcieCfg),
                      false, (sizeof (bool) * NumberOfRbBelowPcieCfg));
    }

    while (NextCombination) {
      if (((*EnoughAbovePcieSpaceForPrimaryRb) &&
          (*(MmioIsAbovePcieCfg + PrimarySocket * DFX_MAX_HOST_BRIDGES_PER_SOCKET + PrimaryRootBridge))) ||
          ((!(*EnoughAbovePcieSpaceForPrimaryRb)) &&
          (!*(MmioIsAbovePcieCfg + PrimarySocket * DFX_MAX_HOST_BRIDGES_PER_SOCKET + PrimaryRootBridge)))) {
        // 3. Try this combination
        GetAnCombination = SilTryThisCombination (SilData, MmioBaseAddrAbovePcieCfg, MmioBaseAddrBelowPcieCfg,
          MmioIsAbovePcieCfg, false, OverSizeBelowPcieMin, AlignmentMask);
        if (GetAnCombination) {
          break;
        }
      }
      // 4. If we can't make it, try to another combination
      NextCombination = SilGetNextCombination (NumberOfRbBelowPcieCfg, MmioIsAbovePcieCfg,
        SilData->SocketNumber, SilData->RbsPerSocket);
    }
    if ((GetAnCombination) || (SilData->PciExpressBaseAddress == MmioBaseAddrBelowPcieCfg)) {
      // If we already got a combination or there's no space above Pcie Cfg, then break
      break;
    }

  }

  if (GetAnCombination) {
    SilTryThisCombination (SilData, MmioBaseAddrAbovePcieCfg, MmioBaseAddrBelowPcieCfg, MmioIsAbovePcieCfg,
      SetDfRegisters, OverSizeBelowPcieMin, AlignmentMask);
  }

  return GetAnCombination;
}

/**
 * SilGetNextCombination
 *
 * @brief Get a different resource combination
 *
 *
 * @param[in]         NumberOfRbBelowPcieCfg      How many RootBridges MMIO should be put below Pcie Cfg
 * @param[in, out]    MmioIsAbovePcieCfg          An bool array, indicate which RootBridge's MMIO is above Pcie Cfg
 * @param[in]         SocketNumber                System socket count
 * @param[in]         RbsPerSocket                RootBridge number per socket
 *
 * @retval            true                        Get a combination
 *                    false                       All combinations have been tried
 */
static
bool
SilGetNextCombination (
  uint8_t   NumberOfRbBelowPcieCfg,
  bool      *MmioIsAbovePcieCfg,
  uint8_t   SocketNumber,
  uint8_t   RbsPerSocket
  )
{
  uint8_t   MoveThisOne;
  uint8_t   ResetTheseRbs;

  // From RootBridge 0, there're total number of 'ResetTheseRbs' sequential RootBridge, whose MMIO is below Pcie Cfg
  for (ResetTheseRbs = 0; ResetTheseRbs < NumberOfRbBelowPcieCfg; ResetTheseRbs++) {
    if (*(MmioIsAbovePcieCfg + ResetTheseRbs) == true) {
      break;
    }
  }

  if (ResetTheseRbs == NumberOfRbBelowPcieCfg) {
    return false; // All combinations have been tried.
  }

  // After number of ResetTheseRbs RootBridges, find out the first RootBridge whose MMIO is below Pcie Cfg.
  for (MoveThisOne = ResetTheseRbs + 1; MoveThisOne < (SocketNumber * RbsPerSocket); MoveThisOne++) {
    if (*(MmioIsAbovePcieCfg + MoveThisOne) == false) {
      break;
    }
  }
  assert (MoveThisOne < (SocketNumber * RbsPerSocket));
  *(MmioIsAbovePcieCfg + MoveThisOne) = true;
  *(MmioIsAbovePcieCfg + MoveThisOne - 1) = false;
  memset (MmioIsAbovePcieCfg, true, (sizeof (bool) * ResetTheseRbs));
  memset ((MmioIsAbovePcieCfg + MoveThisOne - 1 - ResetTheseRbs), false, (sizeof (bool) * ResetTheseRbs));

  return true;
}

/**
 * SilTryThisCombination
 *
 * @brief This function tries the given combination of MMIO resources.
 *
 * @details If successful, it may be requested to program DF MMIO registers.
 *
 * @param[in]         SilData                     IP input block data
 * @param[in]         MmioBaseAddrAbovePcieCfg    MmioBaseAddrAbovePcieCfg
 * @param[in]         MmioBaseAddrBelowPcieCfg    MmioBaseAddrBelowPcieCfg
 * @param[in, out]    MmioIsAbovePcieCfg          An bool array, indicate which RootBridge's MMIO is above Pcie Cfg
 * @param[in]         SetDfRegisters              true  - Set DF MMIO registers
 *                                                false - Do Not set DF MMIO registers, just calculate if
 *                                                        user's requirment could be satisfied.
 * @param[in, out]    OverSizeBelowPcieMin        OverSizeBelowPcieMin
 * @param[in, out]    AlignmentMask               AlignmentMask
 *
 * @retval            true                        Successful resource combination
 *                    false                       Unsuccessful resource combination
 */
static
bool
SilTryThisCombination (
  DFX_RCMGR_INPUT_BLK     *SilData,
  uint64_t                MmioBaseAddrAbovePcieCfg,
  uint64_t                MmioBaseAddrBelowPcieCfg,
  bool                    *MmioIsAbovePcieCfg,
  bool                    SetDfRegisters,
  uint32_t                *OverSizeBelowPcieMin,
  uint32_t                *AlignmentMask
  )
{
  uint8_t  i;
  uint8_t  j;
  uint8_t  SocketLoop;
  uint8_t  RbLoop;
  uint32_t PrimarySocket;
  uint32_t PrimaryRootBridge;
  uint64_t MmioBaseAddr; // To caculate oversize, we must use uint64_t here for all address, size
  uint64_t MmioCeiling;
  uint64_t MmioBaseAddrPrefetchable;
  uint64_t MmioBaseAddrNonPrefetchable;
  uint64_t MmioBaseAddrNonPci;
  uint64_t PrimaryRbSecondMmioSize;
  uint64_t SizePrefetchable;
  uint64_t SizeNonPrefetchable;
  uint64_t SizeNonPci;
  uint64_t AlignMask;
  uint64_t AlignMaskP;
  uint64_t AlignMaskNonPci;
  uint64_t BottomOfCompat;
  uint64_t OverSizeAbovePcieCfg;
  uint64_t OverSizeBelowPcieCfg;
  uint64_t AlignForFirstMmioRegionAbovePcieCfg;
  uint64_t AlignForFirstMmioRegionBelowPcieCfg;
  uint64_t DeltaSize; // this variable keeps the data how much size would be reduced if there's no NON-PCI MMIO
  bool AlreadyGotAlignForFirstMmioRegionAbovePcieCfg;
  bool AlreadyGotAlignForFirstMmioRegionBelowPcieCfg;
  bool BigAlignFirstAbovePcieCfg;
  bool BigAlignFirstBelowPcieCfg;
  bool BigAlignFirst;
  bool ReservedRegionAlreadySet;
  bool OverSizeFlag;
  bool PrimaryRbNonPciMmioAt2ndRegion;
  bool HasSpaceBelowPcie;

  DFX_FABRIC_RESOURCE_FOR_EACH_RB *MmioSizeForEachRb = &SilData->ResourceSizeForEachRb;
  uint32_t MmioLimitAbovePcieCfg = SilData->BottomMmioReservedForPrimaryRb;
  uint64_t MmioLimitBelowPcieCfg = SilData->PciExpressBaseAddress;
  DFX_FABRIC_MMIO_MANAGER *FabricMmioManager = &SilData->MmioRcMgr;

  HasSpaceBelowPcie = (MmioLimitBelowPcieCfg > MmioBaseAddrBelowPcieCfg);
  BigAlignFirstAbovePcieCfg = true;
  BigAlignFirstBelowPcieCfg = true;
  OverSizeFlag = false;
  DeltaSize = 0;
  PrimaryRbNonPciMmioAt2ndRegion = false;
  AlreadyGotAlignForFirstMmioRegionAbovePcieCfg = false;
  AlreadyGotAlignForFirstMmioRegionBelowPcieCfg = false;
  SizeNonPci = SilData->MmioSizePerRbForNonPciDevice;
  AlignMaskNonPci = RCMGR_NON_PCI_MMIO_ALIGN_MASK;
  AlignForFirstMmioRegionAbovePcieCfg = 0;
  AlignForFirstMmioRegionBelowPcieCfg = 0;

  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

  BottomOfCompat = BOTTOM_OF_COMPAT;
  ReservedRegionAlreadySet = false;  // Indicate if AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat is set

  for (i = 0; i <= (SilData->SocketNumber * SilData->RbsPerSocket); i++) {
    SocketLoop = (SilData->SocketNumber * SilData->RbsPerSocket - i - 1) / SilData->RbsPerSocket;
    RbLoop = (SilData->SocketNumber * SilData->RbsPerSocket - i - 1) % SilData->RbsPerSocket;

    // Skip allocating MMIO for primary RootBridge, and will allocate later
    if ((SocketLoop == PrimarySocket) && (RbLoop == PrimaryRootBridge)) {
      continue;
    }

    // This is the last loop, now try to allocate MMIO for primary RootBridge
    if (i == (SilData->SocketNumber * SilData->RbsPerSocket)) {
      SocketLoop = (uint8_t) PrimarySocket;
      RbLoop = (uint8_t) PrimaryRootBridge;
    }

    if ((MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size +
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + SizeNonPci) == 0) {
      continue;
    }

    AlignMask  = MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment;
    AlignMaskP = MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment;

    if (*(MmioIsAbovePcieCfg + SocketLoop * DFX_MAX_HOST_BRIDGES_PER_SOCKET + RbLoop)) {
      // Mmio is above PcieCfg
      BigAlignFirst = BigAlignFirstAbovePcieCfg;
      MmioBaseAddr = MmioBaseAddrAbovePcieCfg;
      BigAlignFirstAbovePcieCfg = !BigAlignFirstAbovePcieCfg;

      if (!AlreadyGotAlignForFirstMmioRegionAbovePcieCfg) {
        // Save alignment for the first MMIO region, it will be used for calculate oversize
        AlignForFirstMmioRegionAbovePcieCfg = (AlignMask > AlignMaskP) ? AlignMask : AlignMaskP;
        AlreadyGotAlignForFirstMmioRegionAbovePcieCfg = true;
      }
    } else {
      // Mmio is below PcieCfg
      BigAlignFirst = BigAlignFirstBelowPcieCfg;
      MmioBaseAddr = MmioBaseAddrBelowPcieCfg;
      BigAlignFirstBelowPcieCfg = !BigAlignFirstBelowPcieCfg;

      if (!AlreadyGotAlignForFirstMmioRegionBelowPcieCfg) {
        // Save alignment for the first MMIO region, it will be used for calculate oversize
        AlignForFirstMmioRegionBelowPcieCfg = (AlignMask > AlignMaskP) ? AlignMask: AlignMaskP;
        AlreadyGotAlignForFirstMmioRegionBelowPcieCfg = true;
      }
    }

    if (BigAlignFirst) {
      if (MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment >=
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment) {
        // Prefetchable -> Non Prefetchable -> Non Pci
        MmioBaseAddrPrefetchable = (MmioBaseAddr + AlignMaskP) & (~AlignMaskP);
        MmioBaseAddrNonPrefetchable = (MmioBaseAddrPrefetchable +
          MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMask) &
          (~AlignMask);
        MmioBaseAddrNonPci = (MmioBaseAddrNonPrefetchable +
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMaskNonPci) &
          (~AlignMaskNonPci);
        SizePrefetchable = MmioBaseAddrNonPrefetchable - MmioBaseAddrPrefetchable;
        SizeNonPrefetchable = MmioBaseAddrNonPci - MmioBaseAddrNonPrefetchable;
        MmioCeiling = MmioBaseAddrNonPci + SizeNonPci;
        DeltaSize = MmioBaseAddrNonPci - MmioBaseAddrNonPrefetchable -
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + SizeNonPci;
      } else {
        // Non Prefetchable -> Prefetchable -> Non Pci
        MmioBaseAddrNonPrefetchable = (MmioBaseAddr + AlignMask) & (~AlignMask);
        MmioBaseAddrPrefetchable = (MmioBaseAddrNonPrefetchable +
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMaskP) &
          (~AlignMaskP);
        MmioBaseAddrNonPci = (MmioBaseAddrPrefetchable +
          MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMaskNonPci) &
          (~AlignMaskNonPci);
        SizeNonPrefetchable = MmioBaseAddrPrefetchable - MmioBaseAddrNonPrefetchable;
        SizePrefetchable = MmioBaseAddrNonPci - MmioBaseAddrPrefetchable;
        MmioCeiling = MmioBaseAddrNonPci + SizeNonPci;
        DeltaSize = MmioBaseAddrNonPci - MmioBaseAddrPrefetchable -
          MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + SizeNonPci;
      }
    } else {
      if (MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment <=
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment) {
        // Non Pci -> Prefetchable -> Non Prefetchable
        MmioBaseAddrNonPci = (MmioBaseAddr + AlignMaskNonPci) & (~AlignMaskNonPci);
        MmioBaseAddrPrefetchable = (MmioBaseAddrNonPci + SizeNonPci + AlignMaskP) & (~AlignMaskP);
        MmioBaseAddrNonPrefetchable = (MmioBaseAddrPrefetchable +
          MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMask) & (~AlignMask);
        SizePrefetchable = MmioBaseAddrNonPrefetchable - MmioBaseAddrPrefetchable;
        SizeNonPrefetchable = MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size;
        MmioCeiling = MmioBaseAddrNonPrefetchable + SizeNonPrefetchable;
        DeltaSize = MmioBaseAddrPrefetchable - ((MmioBaseAddr + AlignMaskP) & (~AlignMaskP));
      } else {
        // Non Pci -> Non Prefetchable -> Prefetchable
        MmioBaseAddrNonPci = (MmioBaseAddr + AlignMaskNonPci) & (~AlignMaskNonPci);
        MmioBaseAddrNonPrefetchable = (MmioBaseAddrNonPci + SizeNonPci + AlignMask) & (~AlignMask);
        MmioBaseAddrPrefetchable = (MmioBaseAddrNonPrefetchable +
          MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size + AlignMaskP) & (~AlignMaskP);
        SizeNonPrefetchable = MmioBaseAddrPrefetchable - MmioBaseAddrNonPrefetchable;
        SizePrefetchable = MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size;
        MmioCeiling = MmioBaseAddrPrefetchable + SizePrefetchable;
        DeltaSize = MmioBaseAddrNonPrefetchable - ((MmioBaseAddr + AlignMask) & (~AlignMask));
      }
    }

    if (SizeNonPrefetchable == 0) {

      MmioBaseAddrNonPrefetchable = 0;
    }
    if (SizePrefetchable == 0) {

      MmioBaseAddrPrefetchable = 0;
    }

    // Check if space is enough
    if (*(MmioIsAbovePcieCfg + SocketLoop * DFX_MAX_HOST_BRIDGES_PER_SOCKET + RbLoop)) {
      // Mmio is above PcieCfg
      MmioBaseAddrAbovePcieCfg = MmioCeiling;
      if (MmioBaseAddrAbovePcieCfg > MmioLimitAbovePcieCfg) {
        OverSizeFlag = true;
      }
    } else {
      // Mmio is below PcieCfg
      MmioBaseAddrBelowPcieCfg = MmioCeiling;
      if (MmioBaseAddrBelowPcieCfg > MmioLimitBelowPcieCfg) {
        OverSizeFlag = true;
      }
    }

    // (OverSize) && (Primary RootBridge has 2nd MMIO), try to see if moving primary RootBridge's Non-Pci MMIO
    // from 1st region to 2nd region could work
    // Always put Non-Pci at top of 2nd MMIO
    if ((OverSizeFlag) && (SocketLoop == PrimarySocket) && (RbLoop == PrimaryRootBridge) &&
        ((SilData->SocketNumber * SilData->RbsPerSocket) < (DFX_MAX_HOST_BRIDGES_PER_SOCKET * MAX_SOCKETS_SUPPORTED))) {
      if (*(MmioIsAbovePcieCfg + SocketLoop * DFX_MAX_HOST_BRIDGES_PER_SOCKET + RbLoop)) {
        if (((MmioBaseAddrAbovePcieCfg - DeltaSize) <= MmioLimitAbovePcieCfg) &&
            (MmioBaseAddrBelowPcieCfg <= ((MmioLimitBelowPcieCfg - SizeNonPci) & (~AlignMaskNonPci)))) {
          OverSizeFlag = false;
          PrimaryRbNonPciMmioAt2ndRegion = true;
          MmioBaseAddrAbovePcieCfg -= DeltaSize;
          MmioCeiling -= DeltaSize;
        }
      } else {
        if (((MmioBaseAddrBelowPcieCfg - DeltaSize) <= MmioLimitBelowPcieCfg) &&
            (MmioBaseAddrAbovePcieCfg <= ((MmioLimitAbovePcieCfg - SizeNonPci) & (~AlignMaskNonPci)))) {
          OverSizeFlag = false;
          PrimaryRbNonPciMmioAt2ndRegion = true;
          MmioBaseAddrBelowPcieCfg -= DeltaSize;
          MmioCeiling -= DeltaSize;
        }
      }

      // Adjust MmioBaseAddrNonPrefetchable & MmioBaseAddrPrefetchable if we move primary RootBridge's NON-PCI
      // from 1st region to 2nd region and NON-PCI is at lower address
      if (PrimaryRbNonPciMmioAt2ndRegion && (!BigAlignFirst)) {
        MmioBaseAddrNonPrefetchable -= DeltaSize;
        MmioBaseAddrPrefetchable -= DeltaSize;
      }
    }

    // Set DF MMIO registers
    if ((!OverSizeFlag) && SetDfRegisters && (FabricMmioManager != NULL)) {
      DF_TRACEPOINT (SIL_TRACE_INFO, "---Socket%x RootBridge%x---\n", SocketLoop, RbLoop);
      DF_TRACEPOINT (SIL_TRACE_INFO, "  Request below 4G\n        Prefetch Size 0x%llX, AlignBit 0x%X\n",
        MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size,
        MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment);
      DF_TRACEPOINT (SIL_TRACE_INFO, "    Non Prefetch Size 0x%llX, AlignBit 0x%X\n\n",
        MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Size,
        MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment);
      assert (BottomOfCompat >= SilData->BottomMmioReservedForPrimaryRb);

      if ((SocketLoop == PrimarySocket) && (RbLoop == PrimaryRootBridge)) {
        if (*(MmioIsAbovePcieCfg + SocketLoop * DFX_MAX_HOST_BRIDGES_PER_SOCKET + RbLoop)) {
          SilSetMmioReg4 (SilData->SocketNumber, ((SocketLoop * SilData->RbsPerSocket + RbLoop) * 2), SocketLoop,
                             RbLoop, MmioBaseAddr, (BottomOfCompat - MmioBaseAddr));
          MmioBaseAddrAbovePcieCfg = BottomOfCompat;
          ReservedRegionAlreadySet = true;
        } else {
          SilSetMmioReg4 (SilData->SocketNumber, ((SocketLoop * SilData->RbsPerSocket + RbLoop) * 2), SocketLoop,
                             RbLoop, MmioBaseAddr, (MmioLimitBelowPcieCfg - MmioBaseAddr));
          MmioBaseAddrBelowPcieCfg = MmioLimitBelowPcieCfg;
        }
      } else {
        SilSetMmioReg4 (SilData->SocketNumber, ((SocketLoop * SilData->RbsPerSocket + RbLoop) * 2), SocketLoop,
                           RbLoop, MmioBaseAddr, (MmioCeiling - MmioBaseAddr));
      }

      FabricMmioManager->AllocateMmioBelow4GOnThisRb[SocketLoop][RbLoop] = true;
      if ((SocketLoop == PrimarySocket) && (RbLoop == PrimaryRootBridge) && PrimaryRbNonPciMmioAt2ndRegion) {
        DF_TRACEPOINT (SIL_TRACE_INFO, "    NON-PCI MMIO of Primary RootBridge has been moved to its 2nd region\n");
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPci = 0;
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPci = 0;
      } else {
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPci = MmioBaseAddrNonPci;
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPci = SizeNonPci;
      }
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BasePrefetch = MmioBaseAddrPrefetchable;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPrefetch = MmioBaseAddrNonPrefetchable;

      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizePrefetch = SizePrefetchable;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPrefetch = SizeNonPrefetchable;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].UsedSizeNonPci = 0;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].UsedSizePrefetch = 0;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].UsedSizeNonPrefetch = 0;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].AlignNonPrefetch =
        MmioSizeForEachRb->NonPrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment;
      FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].AlignPrefetch =
        MmioSizeForEachRb->PrefetchableMmioSizeBelow4G[SocketLoop][RbLoop].Alignment;

      DF_TRACEPOINT (SIL_TRACE_INFO, "    Prefetch    0x%llX ~ 0x%llX, Size 0x%llX\n",
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BasePrefetch,
        (FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BasePrefetch +
          FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizePrefetch),
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizePrefetch);
      DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPrefetch 0x%llX ~ 0x%llX, Size 0x%llX\n",
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPrefetch,
        (FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPrefetch +
          FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPrefetch),
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPrefetch);
      DF_TRACEPOINT (SIL_TRACE_INFO, "    NonPci      0x%llX ~ 0x%llX, Size 0x%llX\n\n",
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPci,
        (FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].BaseNonPci +
          FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPci),
        FabricMmioManager->MmioRegionBelow4G[SocketLoop][RbLoop].SizeNonPci);
    }
  }

  // If SetDfRegisters if FALSE, this function is called by FabricReallocateResourceForEachRb
  // And we should record which combination has the minimum gap
  if (!SetDfRegisters) {
    if (OverSizeFlag) {
      if (MmioBaseAddrAbovePcieCfg > MmioLimitAbovePcieCfg) {
        OverSizeAbovePcieCfg = MmioBaseAddrAbovePcieCfg - MmioLimitAbovePcieCfg;
      } else {
        OverSizeAbovePcieCfg = 0;
      }
      if (MmioBaseAddrBelowPcieCfg > MmioLimitBelowPcieCfg) {
        OverSizeBelowPcieCfg = MmioBaseAddrBelowPcieCfg - MmioLimitBelowPcieCfg;
      } else {
        OverSizeBelowPcieCfg = 0;
      }
      if (OverSizeAbovePcieCfg == 0) {
        // Since user would not change PcieCfg space location, they would only change TOM,
        // so we should make sure there's no oversize above PcieCfg
        if (OverSizeBelowPcieCfg < *OverSizeBelowPcieMin) {
          *OverSizeBelowPcieMin = (uint32_t) OverSizeBelowPcieCfg;
          *AlignmentMask = (uint32_t) AlignForFirstMmioRegionBelowPcieCfg;
        }
      } else if (!HasSpaceBelowPcie) {
        // If there's no space below PcieCfg, then OverSizeAbovePcieCfg is the size we need
        *OverSizeBelowPcieMin = (uint32_t) OverSizeAbovePcieCfg;
        *AlignmentMask = (uint32_t) AlignForFirstMmioRegionAbovePcieCfg;
      }
    } else {
      *OverSizeBelowPcieMin = 0;
    }
  }

  // Primary RootBridge's 2nd MMIO
  if ((!OverSizeFlag) && SetDfRegisters && (FabricMmioManager != NULL)) {
    if (((MmioBaseAddrAbovePcieCfg + RCMGR_MMIO_MIN_SIZE) <= MmioLimitAbovePcieCfg) ||
        ((MmioBaseAddrBelowPcieCfg + RCMGR_MMIO_MIN_SIZE) <= MmioLimitBelowPcieCfg)) {
      if ((MmioSizeForEachRb->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Size != 0) ||
          (MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Size != 0) || PrimaryRbNonPciMmioAt2ndRegion) {
        if ((SilData->SocketNumber * SilData->RbsPerSocket) <
              (DFX_MAX_HOST_BRIDGES_PER_SOCKET * MAX_SOCKETS_SUPPORTED)) {
          for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
            for (j = 0; j < DFX_MAX_HOST_BRIDGES_PER_SOCKET; j++) {
              if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] == false) {
                DF_TRACEPOINT (SIL_TRACE_INFO, "  Primary RB has 2nd MMIO below 4G\n");
                FabricMmioManager->PrimaryRb2ndMmioPairBelow4G = (uint8_t) ((i << 4) | j);
                if (MmioBaseAddrAbovePcieCfg < MmioLimitAbovePcieCfg) {
                  SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2), PrimarySocket,
                    PrimaryRootBridge, MmioBaseAddrAbovePcieCfg, (BottomOfCompat - MmioBaseAddrAbovePcieCfg));
                  MmioBaseAddr = MmioBaseAddrAbovePcieCfg;
                  PrimaryRbSecondMmioSize = MmioLimitAbovePcieCfg - MmioBaseAddrAbovePcieCfg;
                  ReservedRegionAlreadySet = true;
                } else {
                  SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2), PrimarySocket,
                    PrimaryRootBridge, MmioBaseAddrBelowPcieCfg, (MmioLimitBelowPcieCfg - MmioBaseAddrBelowPcieCfg));
                  MmioBaseAddr = MmioBaseAddrBelowPcieCfg;
                  PrimaryRbSecondMmioSize = MmioLimitBelowPcieCfg - MmioBaseAddrBelowPcieCfg;
                }
                if (PrimaryRbNonPciMmioAt2ndRegion) {
                  // Always put Non-Pci at top of 2nd MMIO
                  FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci =
                    (MmioBaseAddr + PrimaryRbSecondMmioSize - SizeNonPci) & (~AlignMaskNonPci);
                  FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci = SizeNonPci;
                  PrimaryRbSecondMmioSize = FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci - MmioBaseAddr;
                } else {
                  FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci = 0;
                  FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci = 0;
                }
                // Distribute Primary RB's 2nd MMIO base on user request
                AlignMask  = MmioSizeForEachRb->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Alignment;
                AlignMaskP = MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Alignment;
                if (MmioSizeForEachRb->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Size == 0) {
                  SizePrefetchable = PrimaryRbSecondMmioSize;
                  SizeNonPrefetchable = 0;
                } else if (MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Size == 0) {
                  SizePrefetchable = 0;
                  SizeNonPrefetchable = PrimaryRbSecondMmioSize;
                } else {
                  SizePrefetchable = (PrimaryRbSecondMmioSize /
                    ((uint32_t) (MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Size +
                      MmioSizeForEachRb->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Size))) *
                        MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Size;
                  SizeNonPrefetchable = PrimaryRbSecondMmioSize - SizePrefetchable;
                }

                if (SizePrefetchable != 0) {
                  MmioBaseAddrPrefetchable = (MmioBaseAddr + AlignMaskP) & (~AlignMaskP);
                  SizePrefetchable = SizePrefetchable;
                } else {
                  MmioBaseAddrPrefetchable = MmioBaseAddr;
                  SizePrefetchable = 0;
                }

                if (SizeNonPrefetchable != 0) {
                  MmioBaseAddrNonPrefetchable = (MmioBaseAddrPrefetchable + SizePrefetchable + AlignMask) &
                  (~AlignMask);
                  SizeNonPrefetchable = MmioBaseAddr + PrimaryRbSecondMmioSize - MmioBaseAddrNonPrefetchable;
                } else {
                  MmioBaseAddrNonPrefetchable = MmioBaseAddrPrefetchable + SizePrefetchable;
                  SizeNonPrefetchable = 0;
                }

                if (SizeNonPrefetchable == 0) {

                  MmioBaseAddrNonPrefetchable = 0;
                }
                if (SizePrefetchable == 0) {

                  MmioBaseAddrPrefetchable = 0;
                }

                FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch = MmioBaseAddrPrefetchable;
                FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch = MmioBaseAddrNonPrefetchable;
                FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch = SizePrefetchable;
                FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch = SizeNonPrefetchable;
                FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizePrefetch = 0;
                FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPrefetch = 0;
                FabricMmioManager->MmioRegionBelow4G[i][j].UsedSizeNonPci = 0;
                FabricMmioManager->MmioRegionBelow4G[i][j].AlignNonPrefetch =
                  MmioSizeForEachRb->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Alignment;
                FabricMmioManager->MmioRegionBelow4G[i][j].AlignPrefetch =
                  MmioSizeForEachRb->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Alignment;

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
    }

    // If there's a spare MMIO register pair, set 0xFED0_0000 ~ 0xFED0_FFFF as posted
    SilAdditionalMmioSetting4 (SilData, BottomOfCompat, ReservedRegionAlreadySet);
  }

  return (!OverSizeFlag);
}

/**
 * SilAdditionalMmioSetting4
 *
 * @brief Additional MMIO settings
 *
 * @details If reserved region for primary RB (AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat) has not been set,
 * or that region doesn't include
 * ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START ~ ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END,
 * set ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START ~ ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END as POSTED
 *
 * @param[in]   SilData                     IP input block data
 * @param[in]   BottomOfCompat              From BottomOfCompat to 4G is COMPAT region
 * @param[in]   ReservedRegionAlreadySet    AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat has been set or not
 *
 */
void
SilAdditionalMmioSetting4 (
  DFX_RCMGR_INPUT_BLK *SilData,
  uint64_t            BottomOfCompat,
  bool                ReservedRegionAlreadySet
  )
{
  uint8_t   i;
  uint8_t   j;
  uint64_t  Base;
  uint64_t  Size;
  uint32_t  PrimarySocket;
  uint32_t  PrimaryRootBridge;
  bool      AlreadySet;
  DFX_FABRIC_MMIO_MANAGER *FabricMmioManager = &SilData->MmioRcMgr;

  SilGetPrimaryRb (&PrimarySocket, &PrimaryRootBridge);

  AlreadySet = false;
  Base = ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START;
  Size = ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END - ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START + 1;

  // If reserved region for primary RB (AmdBottomMmioReservedForPrimaryRb ~ BottomOfCompat) has not been set
  // or that region does not include
  // ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START ~ ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END
  if ((!ReservedRegionAlreadySet) ||
      ((ReservedRegionAlreadySet) && (BottomOfCompat <= ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END))) {
    if (Size >= RCMGR_MMIO_MIN_SIZE) {
      for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
        for (j = 0; j < DFX_MAX_HOST_BRIDGES_PER_SOCKET; j++) {
          // Find out a spare MMIO register pair
          if (FabricMmioManager->AllocateMmioBelow4GOnThisRb[i][j] == false) {
            if ((FabricMmioManager->PrimaryRbHas2ndMmioBelow4G) &&
                (((FabricMmioManager->PrimaryRb2ndMmioPairBelow4G >> 4) & 0xF) == i) &&
                ((FabricMmioManager->PrimaryRb2ndMmioPairBelow4G & 0xF) == j)) {
              continue;
            }
            SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2),
              PrimarySocket, PrimaryRootBridge, Base, Size);
            AlreadySet = true;
            break;
          }

          if (FabricMmioManager->AllocateMmioAbove4GOnThisRb[i][j] == false) {
            SilSetMmioReg4 (SilData->SocketNumber, ((i * SilData->RbsPerSocket + j) * 2 + 1),
              PrimarySocket, PrimaryRootBridge, Base, Size);
            AlreadySet = true;
            break;
          }
        }
        if (AlreadySet) {
          break;
        }
      }
    }
  }
}

/**
 * SilSetMmioReg4
 *
 * @brief Set MMIO register pairs according to input parameters
 *
 * @param[in]         TotalSocket                 System socket count
 * @param[in]         MmioPairIndex               Which MMIO register pair should be set
 * @param[in]         SktNum                      Socket number
 * @param[in]         RbNum                       Root bridge number
 * @param[in]         BaseAddress                 Base address of MMIO region
 * @param[in]         Length                      Length of MMIO region
 *
 */
void
SilSetMmioReg4 (
  uint8_t   TotalSocket,
  uint8_t   MmioPairIndex,
  uint32_t  SktNum,
  uint32_t  RbNum,
  uint64_t  BaseAddress,
  uint64_t  Length
  )
{
  uint32_t  i;
  uint32_t  j;
  uint32_t  DiePerSkt;
  MMIO_ADDRESS_CONTROL_REGISTER MmioAddrCtrlReg;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  assert (Length >= RCMGR_MMIO_MIN_SIZE);
  BaseAddress = (BaseAddress + 0xFFFF) & 0xFFFFFFFFFFFF0000;

  MmioAddrCtrlReg.Value = 0;
  MmioAddrCtrlReg.Field.RE = 1;
  MmioAddrCtrlReg.Field.WE = 1;
  MmioAddrCtrlReg.Field.DstFabricID = (uint32_t) DfIp2IpApi->DfGetHostBridgeSystemFabricID (SktNum, RbNum);

  DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket ();
  for (i = 0; i < TotalSocket; i++) {
    for (j = 0; j < DiePerSkt; j++) {
      DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, MMIOBASEADDRESS_0_FUNC,
        (MMIOBASEADDRESS_0_REG  + MmioPairIndex * (MMIOBASEADDRESS_1_REG - MMIOBASEADDRESS_0_REG)),
        FABRIC_REG_ACC_BC, (uint32_t) (BaseAddress >> 16));
      DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, MMIOLIMITADDRESS_0_FUNC,
        (MMIOLIMITADDRESS_0_REG + MmioPairIndex * (MMIOLIMITADDRESS_1_REG - MMIOLIMITADDRESS_0_REG)),
        FABRIC_REG_ACC_BC, (uint32_t) ((BaseAddress + Length - 1) >> 16));
      DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, MMIOADDRESSCONTROL_0_FUNC,
        (MMIOADDRESSCONTROL_0_REG + MmioPairIndex * (MMIOADDRESSCONTROL_1_REG - MMIOADDRESSCONTROL_0_REG)),
        FABRIC_REG_ACC_BC, MmioAddrCtrlReg.Value);
    }
  }
  DF_TRACEPOINT (SIL_TRACE_INFO, "        Set MMIO pair #%X, 0x%X0000 ~ 0x%XFFFF DstFabricID: 0x%X\n",
    MmioPairIndex, (uint32_t) (BaseAddress >> 16), (uint32_t) ((BaseAddress + Length - 1) >> 16),
    MmioAddrCtrlReg.Field.DstFabricID);
}

/**
 * SilSetIoReg4
 *
 * @brief Set IO register pairs according to input parameters
 *
 * @param[in]         TotalSocket                 System socket count
 * @param[in]         RegIndex                    Which IO register should be set
 * @param[in]         SktNum                      Socket number
 * @param[in]         RbNum                       Root bridge number
 * @param[in]         IoBase                      Base address of IO region
 * @param[in]         IoSize                      Length of IO region
 *
 */
void
SilSetIoReg4 (
  uint8_t  TotalSocket,
  uint8_t  RegIndex,
  uint32_t SktNum,
  uint32_t RbNum,
  uint32_t IoBase,
  uint32_t IoSize
  )
{
  uint32_t  i;
  uint32_t  j;
  uint32_t  DiePerSkt;
  X86_IOBASE_ADDRESS_REGISTER  IoBaseReg;
  X86_IOLIMIT_ADDRESS_REGISTER IoLimitReg;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  IoBaseReg.Value  = 0;
  IoBaseReg.Field.RE = 1;
  IoBaseReg.Field.WE = 1;
  IoBaseReg.Field.IOBase = (IoBase & 0xFFFFF000) >> 12;

  IoLimitReg.Value = 0;
  IoLimitReg.Field.DstFabricID = (uint32_t) DfIp2IpApi->DfGetHostBridgeSystemFabricID (SktNum, RbNum);
  IoLimitReg.Field.IOLimit = ((IoBase + IoSize - 1) & 0xFFFFF000) >> 12;
  DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket ();

  for (i = 0; i < TotalSocket; i++) {
    for (j = 0; j < DiePerSkt; j++) {
      DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, X86IOLIMITADDRESS_0_FUNC,
        (X86IOLIMITADDRESS_0_REG + RegIndex * (X86IOLIMITADDRESS_1_REG - X86IOLIMITADDRESS_0_REG)),
        FABRIC_REG_ACC_BC, IoLimitReg.Value);
      // Enable after limit is set
      DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, X86IOBASEADDRESS_0_FUNC,
        (X86IOBASEADDRESS_0_REG + RegIndex * (X86IOBASEADDRESS_1_REG - X86IOBASEADDRESS_0_REG)),
        FABRIC_REG_ACC_BC, IoBaseReg.Value);
    }
  }
  DF_TRACEPOINT (SIL_TRACE_INFO, "  openSIL set IO pair #%X, 0x%X000 ~ 0x%XFFF DstFabricID: 0x%X\n",
    RegIndex, (uint32_t) (IoBase >> 12), (uint32_t) ((IoBase + IoSize - 1) >> 12), IoLimitReg.Field.DstFabricID);

}

/**
 * SilSetCfgAddrMapReg4
 *
 * @brief Set config address map registers according to input parameters
 *
 * @param[in]         SilData                     openSIL input block structure for RC manager
 * @param[in]         RbPerDie                    Rb count per Die
 * @param[in]         RegIndex                    Which CFG Addr Map register should be set
 * @param[in]         SktNum                      Socket number
 * @param[in]         RbNum                       Root bridge number
 * @param[in]         Base                        PCI bus base
 * @param[in]         Limit                       PCI bus Limit
 *
 */
void
SilSetCfgAddrMapReg4 (
  DFX_RCMGR_INPUT_BLK *SilData,
  uint8_t             RbPerDie,
  uint8_t             RegIndex,
  uint32_t            SktNum,
  uint32_t            RbNum,
  uint16_t            Base,
  uint16_t            Limit
  )
{
  uint32_t  i;
  uint32_t  j;
  uint32_t  DiePerSkt;
  uint32_t  CCMLoop;
  uint32_t  ACMLoop;
  uint32_t  RbLoop;
  uint32_t  RbsPerSocket;
  uint32_t  BMCIOM;
  uint32_t  BMCSocket;
  bool  MCTPEnabled;
  CFG_BASE_ADDRESS_REGISTER  CfgAddrMapReg;
  CFG_LIMIT_ADDRESS_REGISTER CfgAddrLimitReg;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *CcmEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IomEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *AcmEntry;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  assert (Base < 0x100);
  assert (Limit < 0x100);

  CfgAddrMapReg.Value = 0;
  CfgAddrMapReg.Field.RE = 1;
  CfgAddrMapReg.Field.WE = 1;
  CfgAddrMapReg.Field.BusNumBase = Base;

  CfgAddrLimitReg.Value = 0;
  CfgAddrLimitReg.Field.DstFabricID = (uint32_t) DfIp2IpApi->DfGetHostBridgeSystemFabricID (SktNum, (RbNum % RbPerDie));
  CfgAddrLimitReg.Field.BusNumLimit = Limit;

  RbsPerSocket = DfIp2IpApi->DfGetNumberOfRootBridgesOnSocket (0);
  MCTPEnabled = SilData->McptEnable;
  BMCSocket = SilData->BmcSocket;
  BMCIOM = SilData->EarlyBmcLinkLaneNum;
  if ((BMCIOM < 16) || (BMCIOM == 128) || (BMCIOM == 129))
    BMCIOM = 0;
  else if (BMCIOM < 64)
    BMCIOM = 1;
  else if (BMCIOM < 96)
    BMCIOM = 3;
  else
    BMCIOM = 2;

  CcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Ccm);
  assert (CcmEntry != NULL);
  IomEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Iom);
  assert (IomEntry != NULL);
  AcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap (Acm);
  assert (AcmEntry != NULL);

  DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket ();
  for (i = 0; i < SilData->SocketNumber; i++) {
    for (j = 0; j < DiePerSkt; j++) {
      for (CCMLoop = 0; CCMLoop < CcmEntry->Count; CCMLoop++) {
        DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGBASEADDRESS_0_FUNC,
          (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
          CcmEntry->IDs[CCMLoop].InstanceID, CfgAddrMapReg.Value);
        DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGLIMITADDRESS_0_FUNC,
          (CFGLIMITADDRESS_0_REG  + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
          CcmEntry->IDs[CCMLoop].InstanceID, CfgAddrLimitReg.Value);
      }
      for (ACMLoop = 0; ACMLoop < AcmEntry->Count; ACMLoop++) {
        DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGBASEADDRESS_0_FUNC,
          (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
          AcmEntry->IDs[ACMLoop].InstanceID, CfgAddrMapReg.Value);
        DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGLIMITADDRESS_0_FUNC,
          (CFGLIMITADDRESS_0_REG  + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
          AcmEntry->IDs[ACMLoop].InstanceID, CfgAddrLimitReg.Value);
      }
      for (RbLoop = 0; RbLoop < RbsPerSocket; RbLoop++) {
        if ((!MCTPEnabled) || ((MCTPEnabled) && (i == BMCSocket && RbLoop == BMCIOM))) {
          DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGBASEADDRESS_0_FUNC,
            (CFGBASEADDRESS_0_REG  + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
            IomEntry->IDs[RbLoop].InstanceID, CfgAddrMapReg.Value);
          DfIp2IpApi->DfFabricRegisterAccWrite ((uint32_t)i, CFGLIMITADDRESS_0_FUNC,
            (CFGLIMITADDRESS_0_REG  + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
            IomEntry->IDs[RbLoop].InstanceID, CfgAddrLimitReg.Value);
        }
      }
    }
  }
  DF_TRACEPOINT (SIL_TRACE_INFO, "  openSIL set CfgAddrMap #%X, 0x%2X ~ 0x%2X DstFabricID: 0x%X\n",
    RegIndex, CfgAddrMapReg.Field.BusNumBase, CfgAddrLimitReg.Field.BusNumLimit, CfgAddrLimitReg.Field.DstFabricID);
}

/**
 * DfXSilGetPhySktRbNum
 *
 * @brief Returns primary RB socket number and RB number
 *
 * @param[in]  LogSktNum    Logical socket number
 * @param[in]  LogRbNum     Logical root bridge number
 * @param[out] PhySktNum    Physical socket number
 * @param[out] PhyRbNum     Physical root bridge number
 * @param[in,out] LogToPhyMapInit Logical-to-physical map initialized indicator
 * @param[in,out] pLogToPhyMap  Logical-to-physical map
 *
 */
void
DfXSilGetPhySktRbNum (
  uint32_t LogSktNum,
  uint32_t LogRbNum,
  uint32_t *PhySktNum,
  uint32_t *PhyRbNum,
  bool *LogToPhyMapInit,
  FABRIC_RB_LOG_TO_PHY_MAP *pLogToPhyMap
  )
{
  uint32_t i;
  uint32_t j;
  uint32_t k;
  uint32_t SysRbCount;
  uint32_t SocketCount;
  uint32_t DieCount;
  uint32_t RbCount;
  uint32_t RbNumInSkt;
  FABRIC_RB_LOG_TO_PHY_MAP Temp;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  if (!(*LogToPhyMapInit)) {
    SysRbCount = 0;
    SocketCount = DfIp2IpApi->DfGetNumberOfProcessorsPresent ();
    DieCount = DfIp2IpApi->DfGetNumberOfDiesOnSocket ();
    for (i = 0; i < SocketCount; i++) {
      RbNumInSkt = 0;
      RbCount = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie ((uint32_t)i);
      for (j = 0; j < DieCount; j++) {
        for (k = 0; k < RbCount; k++) {
          (pLogToPhyMap + SysRbCount)->PhySktNum = i;
          (pLogToPhyMap + SysRbCount)->PhyRbNum = RbNumInSkt;
          (pLogToPhyMap + SysRbCount)->PciBusBase = DfIp2IpApi->DfGetHostBridgeBusBase ((uint32_t)i, k);
          (pLogToPhyMap + SysRbCount)->PciBusLimit = DfIp2IpApi->DfGetHostBridgeBusLimit ((uint32_t)i, k);
          if ((pLogToPhyMap + SysRbCount)->PciBusBase > (pLogToPhyMap + SysRbCount)->PciBusLimit) {
            (pLogToPhyMap + SysRbCount)->PciBusBase = 0xFFFFFFFF;
            (pLogToPhyMap + SysRbCount)->PciBusLimit = 0;
          }

          RbNumInSkt++;
          SysRbCount++;
        }
      }
    }

    for (i = 0; i < SysRbCount; i++) {
      for (j = (SysRbCount - 1); j > i; j--) {
        if ((pLogToPhyMap + j)->PciBusBase < (pLogToPhyMap + j - 1)->PciBusBase) {
          Temp.PhySktNum = (pLogToPhyMap + j)->PhySktNum;
          Temp.PhyRbNum = (pLogToPhyMap + j)->PhyRbNum;
          Temp.PciBusBase = (pLogToPhyMap + j)->PciBusBase;
          Temp.PciBusLimit = (pLogToPhyMap + j)->PciBusLimit;

          (pLogToPhyMap + j)->PhySktNum = (pLogToPhyMap + j - 1)->PhySktNum;
          (pLogToPhyMap + j)->PhyRbNum = (pLogToPhyMap + j - 1)->PhyRbNum;
          (pLogToPhyMap + j)->PciBusBase = (pLogToPhyMap + j - 1)->PciBusBase;
          (pLogToPhyMap + j)->PciBusLimit = (pLogToPhyMap + j - 1)->PciBusLimit;

          (pLogToPhyMap + j - 1)->PhySktNum = Temp.PhySktNum;
          (pLogToPhyMap + j - 1)->PhyRbNum = Temp.PhyRbNum;
          (pLogToPhyMap + j - 1)->PciBusBase = Temp.PciBusBase;
          (pLogToPhyMap + j - 1)->PciBusLimit = Temp.PciBusLimit;
        }
      }
    }
    *LogToPhyMapInit = true;
  }

  RbNumInSkt = DfIp2IpApi->DfGetNumberOfRootBridgesOnSocket (0);
  *PhySktNum = (pLogToPhyMap + LogSktNum * RbNumInSkt + LogRbNum)->PhySktNum;
  *PhyRbNum = (pLogToPhyMap + LogSktNum * RbNumInSkt + LogRbNum)->PhyRbNum;
  DF_TRACEPOINT (SIL_TRACE_INFO, "  Logical Socket %x, Rb %x is Physical Socket%x, Rb %x\n",
    LogSktNum, LogRbNum, *PhySktNum, *PhyRbNum);
}
