/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricResourceInitBrh.c
 * @brief OpenSIL DataFabric MMIO and IO resource map initialization.
 *
 */

#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/BRH/SilFabricRegistersBrh.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/BRH/SilFabricInfoBrh.h>
#include <RcMgr/DfX/FabricRcManagerDfX.h>
#include <SilSocLogicalId.h>
#include "FabricRcInitBrh.h"
#include <MsrReg.h>

static const uint32_t BrhIosRbLoopToRbAssignOrder [] = { 4, 5, 6, 7, 3, 2, 1, 0 };  // IOS 4/5/6/7/3/2/1/0

/**
 * SetCfgAddrMapRegBrh
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
static
void
SetCfgAddrMapRegBrh (
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
  uint16_t  SegmentNumber;
  CFG_BASE_ADDRESS_REGISTER  CfgAddrMapReg;
  CFG_LIMIT_ADDRESS_REGISTER CfgAddrLimitReg;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  SegmentNumber = Base / MAX_PCI_BUS_NUMBER_PER_SEGMENT;
  Base %= MAX_PCI_BUS_NUMBER_PER_SEGMENT;
  assert(SegmentNumber == Limit / MAX_PCI_BUS_NUMBER_PER_SEGMENT);
  Limit %= MAX_PCI_BUS_NUMBER_PER_SEGMENT;
  assert(Base <= Limit);

  CfgAddrMapReg.Value = 0;
  CfgAddrMapReg.Field.RE = 1;
  CfgAddrMapReg.Field.WE = 1;
  CfgAddrMapReg.Field.BusNumBase = Base;
  CfgAddrMapReg.Field.SegmentNum = SegmentNumber;

  CfgAddrLimitReg.Value = 0;
  CfgAddrLimitReg.Field.DstFabricID =
    DfIp2IpApi->DfGetHostBridgeSystemFabricID(SktNum, (RbNum / RbPerDie), (RbNum % RbPerDie));
  CfgAddrLimitReg.Field.BusNumLimit = Limit;

  for (i = 0; i < SilData->SocketNumber; i++) {
    DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket(i);
    for (j = 0; j < DiePerSkt; j++) {
      DfIp2IpApi->DfFabricRegisterAccWrite(i,
        j,
        CFGBASEADDRESS_0_FUNC,
        (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
        FABRIC_REG_ACC_BC,
        CfgAddrMapReg.Value
        );
      DfIp2IpApi->DfFabricRegisterAccWrite(i,
        j,
        CFGLIMITADDRESS_0_FUNC,
        (CFGLIMITADDRESS_0_REG + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
        FABRIC_REG_ACC_BC,
        CfgAddrLimitReg.Value
        );
    }
  }
  RCMGR_TRACEPOINT(SIL_TRACE_INFO,
    "  openSIL set CfgAddrMap #%X, 0x%2X ~ 0x%2X DstFabricID: 0x%X\n",
    RegIndex,
    CfgAddrMapReg.Field.BusNumBase,
    CfgAddrLimitReg.Field.BusNumLimit,
    CfgAddrLimitReg.Field.DstFabricID
    );
}

/**
 * SilInitPciBusBasedOnNvVariableBrh
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
SilInitPciBusBasedOnNvVariableBrh (
  DFX_RCMGR_INPUT_BLK *SilData
  )
{
  uint32_t  SktLoop;
  uint32_t  DieLoop;
  uint32_t  RbLoop;
  uint32_t  IomLoop;
  uint8_t   IosRbLoop;
  uint8_t   IosLoop;
  uint32_t  DiePerSkt;
  uint8_t   RegIndex;
  uint16_t  PciBusBase;
  uint16_t  PciBusLimit;
  uint32_t  PrimarySocket;
  uint32_t  PrimaryDie;
  uint32_t  PrimaryRootBridge;
  uint32_t  PciBase;
  uint32_t  PciLimit;
  uint32_t  RbPerDie;
  bool      NeedReallocate;
  uint32_t  IosRbLoopToRbAssignOrder[8] = {0};
  CFG_ADDRESS_CNTL_REGISTER         CfgAddrCtrl;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  uint32_t                         NumberOfBusRanges;
  uint64_t                         PciCfgSpace;
  uint32_t                          Remainder;
  uint32_t                          IomSegmentNum;
  uint32_t                          SegmentCount;
  uint32_t                          n;
  CFG_BASE_ADDRESS_REGISTER         CfgBaseAddr;
  CFG_LIMIT_ADDRESS_REGISTER        CfgLimitAddr;
  DFX_FABRIC_RESOURCE_FOR_EACH_RB  *PciBusRangeForEachRb;
  DF_IP2IP_API  *DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  PciBusRangeForEachRb = &SilData->ResourceSizeForEachRb;

  // Check if we need to re-allocate PCI bus base/limit
  NeedReallocate = false;

  RbPerDie = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie(0, 0);

  for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
    for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
      PciBase = DfIp2IpApi->DfGetHostBridgeBusBase(SktLoop, (RbLoop / RbPerDie), (RbLoop % RbPerDie));
      PciLimit = DfIp2IpApi->DfGetHostBridgeBusLimit(SktLoop, (RbLoop / RbPerDie), (RbLoop % RbPerDie));
      if ((PciLimit - PciBase + 1) != PciBusRangeForEachRb->PciBusNumber[SktLoop][RbLoop]) {
        NeedReallocate = true;
        break;
      }
    }
  }

  if (NeedReallocate) {
    NumberOfBusRanges = DfIp2IpApi->DfGetNumberOfBusRegions ();
    IosEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Ios);
    assert(IosEntry != NULL);

    // Clear all Configuration Address Map registers
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init PCI bus base/limit based on NV variable\n");
    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket(SktLoop);
      for (DieLoop = 0; DieLoop < DiePerSkt; DieLoop++) {
        for (RegIndex = 0; RegIndex < NumberOfBusRanges; RegIndex++) {
          DfIp2IpApi->DfFabricRegisterAccWrite(SktLoop,
            DieLoop,
            CFGBASEADDRESS_0_FUNC,
            (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
            FABRIC_REG_ACC_BC,
            0
            );
        }
        for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
          DfIp2IpApi->DfFabricRegisterAccWrite(SktLoop,
            DieLoop,
            CFGADDRESSCNTL_FUNC,
            CFGADDRESSCNTL_REG,
            IosEntry->IDs[RbLoop].InstanceID,
            0
            );
        }
      }
    }

    // Set Configuration Address Map registers
    SilGetPrimaryRb(&PrimarySocket, &PrimaryDie, &PrimaryRootBridge);

    RegIndex = 0;
    PciBusBase = 0;
    PciBusLimit = (uint16_t) (PciBusBase + PciBusRangeForEachRb->PciBusNumber[PrimarySocket][PrimaryRootBridge] - 1);
    SetCfgAddrMapRegBrh(SilData,
      (uint8_t) RbPerDie,
      RegIndex,
      PrimarySocket,
      PrimaryRootBridge,
      PciBusBase,
      PciBusLimit
      );
    CfgAddrCtrl.Value = 0;
    CfgAddrCtrl.Field.SecBusNum = PciBusBase;
    DfIp2IpApi->DfFabricRegisterAccWrite((uint32_t)PrimarySocket,
      (uint32_t)PrimaryDie,
      CFGADDRESSCNTL_FUNC,
      CFGADDRESSCNTL_REG,
      IosEntry->IDs[PrimaryRootBridge].InstanceID,
      CfgAddrCtrl.Value
      );

    // Calculate actual RB assign order from fabric topology map
    RbLoop = 0;
    for (IosRbLoop = 0; IosRbLoop < (sizeof (BrhIosRbLoopToRbAssignOrder) / sizeof (BrhIosRbLoopToRbAssignOrder[0]));
      IosRbLoop++) {
      for (IosLoop = 0; IosLoop < IosEntry->Count; IosLoop++) {
        if (BrhIosRbLoopToRbAssignOrder[IosRbLoop] == (IosEntry->IDs[IosLoop].InstanceID - SIL_RESERVED_0191)) {
          IosRbLoopToRbAssignOrder[RbLoop++] = IosLoop;
        }
      }
    }

    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
        if ((SktLoop == PrimarySocket) && (IosRbLoopToRbAssignOrder[RbLoop] == PrimaryRootBridge)) {
          continue;
        }
        RegIndex++;
        PciBusBase = PciBusLimit + 1;
        if ((SktLoop == (uint32_t)(SilData->SocketNumber - 1)) && (RbLoop == (uint32_t)(SilData->RbsPerSocket - 1))) {
          PciBusLimit = 0xFF + (PciBusLimit / MAX_PCI_BUS_NUMBER_PER_SEGMENT * MAX_PCI_BUS_NUMBER_PER_SEGMENT);
        } else {
          PciBusLimit = (uint16_t) (PciBusBase +
            PciBusRangeForEachRb->PciBusNumber[SktLoop][IosRbLoopToRbAssignOrder[RbLoop]] - 1);
        }
        SetCfgAddrMapRegBrh(SilData,
          (uint8_t) RbPerDie,
          RegIndex,
          SktLoop,
          IosRbLoopToRbAssignOrder[RbLoop],
          PciBusBase,
          PciBusLimit
          );
        CfgAddrCtrl.Value = 0;
        CfgAddrCtrl.Field.SecBusNum = PciBusBase % MAX_PCI_BUS_NUMBER_PER_SEGMENT;
        DfIp2IpApi->DfFabricRegisterAccWrite((uint32_t)SktLoop,
          RbLoop / RbPerDie,
          CFGADDRESSCNTL_FUNC,
          CFGADDRESSCNTL_REG,
          IosEntry->IDs[IosRbLoopToRbAssignOrder[RbLoop]].InstanceID,
          CfgAddrCtrl.Value
          );
      }
    }

    // WA for IOD C0 to set BIT7 of CfgBaseAddress[SegmentNum] to inform IOM which range is its local segment
    if (!IS_SOC_BRH_Ax && !IS_SOC_BRH_Bx && !IS_SOC_BRHD_Ax) {
      PciCfgSpace = (xUslRdMsr(MSR_MMIO_CFG_BASE) >> 2) & 0xF;// Get bus range from MSR_C001_0058[5:2]
      PciCfgSpace = ((uint64_t) 1 << PciCfgSpace);
      Remainder = (uint32_t) (PciCfgSpace % MAX_PCI_BUS_NUMBER_PER_SEGMENT);
      SegmentCount = (uint32_t) (PciCfgSpace / MAX_PCI_BUS_NUMBER_PER_SEGMENT);
      if (Remainder) {
        SegmentCount++;
      }
      if (SegmentCount > 1) {
        for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
          for (DieLoop = 0; DieLoop < DiePerSkt; DieLoop++) {
            for (IomLoop = 0; IomLoop < SIL_RESERVED_0215; IomLoop++) {
              IomSegmentNum = 0xFFFFFFFF;
              // for each IOM, get the Segment Number
              for (n = 0; n < SIL_RESERVED_0205; n++) {
                CfgLimitAddr.Value = DfXFabricRegisterAccRead(SktLoop,
                  DieLoop,
                  CFGLIMITADDRESS_0_FUNC + n * (CFGLIMITADDRESS_1_FUNC - CFGLIMITADDRESS_0_FUNC),
                  CFGLIMITADDRESS_0_REG + n * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG),
                  SIL_RESERVED_0183 + IomLoop
                  );
                if (((SktLoop << (BRH_FABRIC_ID_SOCKET_SHIFT)) |
                  (SIL_RESERVED_0183 + IomLoop)) == CfgLimitAddr.Field.DstFabricID) {
                  CfgBaseAddr.Value = DfXFabricRegisterAccRead(SktLoop,
                    DieLoop,
                    CFGBASEADDRESS_0_FUNC + n * (CFGBASEADDRESS_1_FUNC - CFGBASEADDRESS_0_FUNC),
                    CFGBASEADDRESS_0_REG + n * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG),
                    SIL_RESERVED_0183 + IomLoop
                    );
                  IomSegmentNum = CfgBaseAddr.Field.SegmentNum;
                  break;
                }
              }
              assert(IomSegmentNum != 0xFFFFFFFF);
              if (IomSegmentNum == 0xFFFFFFFF) {
                continue;
              }
              //Find the CfgBaseAddress_nx with the same segment number, and set BIT7 for CfgBaseAddress[SegmentNum]
              for (n = 0; n < SIL_RESERVED_0205; n++) {
                CfgBaseAddr.Value = DfXFabricRegisterAccRead(SktLoop,
                  DieLoop,
                  CFGBASEADDRESS_0_FUNC + n * (CFGBASEADDRESS_1_FUNC - CFGBASEADDRESS_0_FUNC),
                  CFGBASEADDRESS_0_REG + n * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG),
                  SIL_RESERVED_0183 + IomLoop
                  );
                if (CfgBaseAddr.Field.SegmentNum == IomSegmentNum) {
                  CfgBaseAddr.Field.SegmentNum |= BIT_32(7);
                  DfXFabricRegisterAccWrite(SktLoop,
                    DieLoop,
                    CFGBASEADDRESS_0_FUNC + n * (CFGBASEADDRESS_1_FUNC - CFGBASEADDRESS_0_FUNC),
                    CFGBASEADDRESS_0_REG + n * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG),
                    SIL_RESERVED_0183 + IomLoop,
                    CfgBaseAddr.Value
                    );
                }
              }
            }
          }
        }
      }
    }
  }

  return SilPass;
}
