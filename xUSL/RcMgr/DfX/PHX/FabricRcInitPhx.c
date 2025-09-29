/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcInitPhx.c
 * @brief OpenSIL DataFabric MMIO and IO resource map initialization.
 *
 */

#include <Utils.h>
#include <DF/Df.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/PHX/DfSilFabricRegistersPhx.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include "FabricRcInitPhx.h"
#include "FabricRcManagerPhx.h"
#include <RcMgr/DfX/RcManager-api.h>
#include <ProjSocConst.h>
#include <RcMgr/DfX/FabricRcManagerDfX.h>

static const uint32_t PhxIosRbLoopToRbAssignOrder [] = { 2, 3, 1, 0 };  // IOS2/3/1/0

/**
 * SetCfgAddrMapRegPhx
 *
 * @brief Set config address map registers according to input parameters
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   SilData             openSIL input block structure for RC manager
 * @param   RbPerDie            Rb count per Die
 * @param   RegIndex            Which CFG Addr Map register should be set
 * @param   SktNum              Socket number
 * @param   RbNum               Root bridge number
 * @param   Base                PCI bus base
 * @param   Limit               PCI bus Limit
 *
 */
static
void
SetCfgAddrMapRegPhx (
  SIL_CONTEXT         *SilContext,
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
  uint32_t  RbLoop;
  uint32_t  RbsPerSocket;
  bool  MCTPEnabled;
  CFG_BASE_ADDRESS_REGISTER  CfgAddrMapReg;
  CFG_LIMIT_ADDRESS_REGISTER CfgAddrLimitReg;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *CcmEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IomEntry;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  assert(Base < 0x100);
  assert(Limit < 0x100);

  CfgAddrMapReg.Value = 0;
  CfgAddrMapReg.Field.RE = 1;
  CfgAddrMapReg.Field.WE = 1;
  CfgAddrMapReg.Field.BusNumBase = Base;

  CfgAddrLimitReg.Value = 0;
  CfgAddrLimitReg.Field.DstFabricID = (uint32_t) DfIp2IpApi->DfGetHostBridgeSystemFabricID(SilContext,
    SktNum,
    0,
    (RbNum % RbPerDie)
    );
  CfgAddrLimitReg.Field.BusNumLimit = Limit;

  RbsPerSocket = DfIp2IpApi->DfGetNumberOfRootBridgesOnSocket(SilContext, 0);
  MCTPEnabled = SilData->McptEnable;

  CcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Ccm);
  assert(CcmEntry != NULL);
  IomEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Iom);
  assert(IomEntry != NULL);

  for (i = 0; i < SilData->SocketNumber; i++) {
    DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket(SilContext, i);
    for (j = 0; j < DiePerSkt; j++) {
      for (CCMLoop = 0; CCMLoop < CcmEntry->Count; CCMLoop++) {
        DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
          i,
          j,
          CFGBASEADDRESS_0_FUNC,
          (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
          CcmEntry->IDs[CCMLoop].InstanceID,
          CfgAddrMapReg.Value
          );
        DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
          i,
          j,
          CFGLIMITADDRESS_0_FUNC,
          (CFGLIMITADDRESS_0_REG + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
          CcmEntry->IDs[CCMLoop].InstanceID,
          CfgAddrLimitReg.Value
          );
      }
      for (RbLoop = 0; RbLoop < RbsPerSocket; RbLoop++) {
        if (!MCTPEnabled) {
          DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
            i,
            j,
            CFGBASEADDRESS_0_FUNC,
            (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
            IomEntry->IDs[RbLoop].InstanceID,
            CfgAddrMapReg.Value
            );
          DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
            i,
            j,
            CFGLIMITADDRESS_0_FUNC,
            (CFGLIMITADDRESS_0_REG + RegIndex * (CFGLIMITADDRESS_1_REG - CFGLIMITADDRESS_0_REG)),
            IomEntry->IDs[RbLoop].InstanceID,
            CfgAddrLimitReg.Value
            );
        }
      }
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
 * SilInitPciBusBasedOnNvVariablePhx
 *
 * Initialize PCI bus base/limit for each Root Bridge based on NV variable.
 *
 * @param   SilContext                     A context structure through which host firmware defined data
 *                                         can be passed to openSIL. The host firmware is responsible
 *                                         for initializing the SIL_CONTEXT structure.
 * @param   SilData                        openSIL input block structure for RC manager
 *          SilData->PciBusRangeForEachRb  How much PCI bus is required for each Root Bridge
 *          SilData->SocketNumber          System socket count
 *          SilData->RbsPerSocket          Root Bridge number per socket
 *
 */
SIL_STATUS
SilInitPciBusBasedOnNvVariablePhx (
  SIL_CONTEXT         *SilContext,
  DFX_RCMGR_INPUT_BLK *SilData
  )
{
  uint32_t  SktLoop;
  uint32_t  DieLoop;
  uint32_t  CCMLoop;
  uint32_t  RbLoop;
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
  bool      MCTPEnabled;
  bool      NeedReallocate;
  CFG_ADDRESS_CNTL_REGISTER         CfgAddrCtrl;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *CcmEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IomEntry;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  DFX_FABRIC_RESOURCE_FOR_EACH_RB  *PciBusRangeForEachRb;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  PciBusRangeForEachRb = &SilData->ResourceSizeForEachRb;

  // Check if we need to re-allocate PCI bus base/limit
  NeedReallocate = false;

  RbPerDie = DfIp2IpApi->DfGetNumberOfRootBridgesOnDie(SilContext, 0, 0);
  for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
    for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
      PciBase = DfIp2IpApi->DfGetHostBridgeBusBase(SilContext, SktLoop, (RbLoop / RbPerDie), (RbLoop % RbPerDie));
      PciLimit = DfIp2IpApi->DfGetHostBridgeBusLimit(SilContext, SktLoop, (RbLoop / RbPerDie), (RbLoop % RbPerDie));
      if ((PciLimit - PciBase + 1) != PciBusRangeForEachRb->PciBusNumber[SktLoop][RbLoop]) {
        NeedReallocate = true;
        break;
      }
    }
  }

  if (NeedReallocate) {

    MCTPEnabled = SilData->McptEnable;

    CcmEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Ccm);
    assert(CcmEntry != NULL);
    IomEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Iom);
    assert(IomEntry != NULL);
    IosEntry = DfIp2IpApi->DfFindDeviceTypeEntryInMap(0, 0, Ios);
    assert(IosEntry != NULL);

    // Clear all Configuration Address Map registers (up to 8)
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init PCI bus base/limit based on NV variable\n");
    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      DiePerSkt = DfIp2IpApi->DfGetNumberOfDiesOnSocket(SilContext, SktLoop);
      for (DieLoop = 0; DieLoop < DiePerSkt; DieLoop++) {
        for (CCMLoop = 0; CCMLoop < CcmEntry->Count; CCMLoop++) {
          for (RegIndex = 0; RegIndex < 8; RegIndex++) {
            DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
              SktLoop,
              DieLoop,
              CFGBASEADDRESS_0_FUNC,
              (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
              CcmEntry->IDs[CCMLoop].InstanceID,
              0
              );
          }
        }
        for (RbLoop = 0; RbLoop < SilData->RbsPerSocket; RbLoop++) {
          for (RegIndex = 0; RegIndex < 8; RegIndex++) {
            if (!MCTPEnabled) {
              DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
                SktLoop,
                DieLoop,
                CFGBASEADDRESS_0_FUNC,
                (CFGBASEADDRESS_0_REG + RegIndex * (CFGBASEADDRESS_1_REG - CFGBASEADDRESS_0_REG)),
                IomEntry->IDs[RbLoop].InstanceID,
                0
                );
            }
          }
          DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
            SktLoop,
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
    SilGetPrimaryRb(SilContext, &PrimarySocket, &PrimaryDie, &PrimaryRootBridge);

    RegIndex = 0;
    PciBusBase = 0;
    PciBusLimit = (uint16_t) (PciBusBase + PciBusRangeForEachRb->PciBusNumber[PrimarySocket][PrimaryRootBridge] - 1);
    SetCfgAddrMapRegPhx(SilContext,
      SilData,
      (uint8_t) RbPerDie,
      RegIndex,
      PrimarySocket,
      PrimaryRootBridge,
      PciBusBase,
      PciBusLimit
      );
    CfgAddrCtrl.Value = 0;
    CfgAddrCtrl.Field.SecBusNum = PciBusBase;
    DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
      (uint32_t)PrimarySocket,
      0,
      CFGADDRESSCNTL_FUNC,
      CFGADDRESSCNTL_REG,
      IosEntry->IDs[PrimaryRootBridge].InstanceID,
      CfgAddrCtrl.Value
      );

    for (SktLoop = 0; SktLoop < SilData->SocketNumber; SktLoop++) {
      for (RbLoop = SilData->RbsPerSocket; RbLoop > 0; RbLoop--) {
        if ((SktLoop == PrimarySocket) && (PhxIosRbLoopToRbAssignOrder[RbLoop - 1] == PrimaryRootBridge)) {
          continue;
        }
        RegIndex++;
        PciBusBase = PciBusLimit + 1;
        if ((SktLoop == (uint32_t)(SilData->SocketNumber - 1)) && (RbLoop == 1)) {
          PciBusLimit = 0xFF;
        } else {
          PciBusLimit = (uint16_t) (PciBusBase +
            PciBusRangeForEachRb->PciBusNumber[SktLoop][PhxIosRbLoopToRbAssignOrder[RbLoop - 1]] - 1);
        }
        SetCfgAddrMapRegPhx(SilContext,
          SilData,
          (uint8_t) RbPerDie,
          RegIndex,
          SktLoop,
          PhxIosRbLoopToRbAssignOrder[RbLoop - 1],
          PciBusBase,
          PciBusLimit
          );
        CfgAddrCtrl.Value = 0;
        CfgAddrCtrl.Field.SecBusNum = PciBusBase;
        DfIp2IpApi->DfFabricRegisterAccWrite(SilContext,
          SktLoop,
          0,
          CFGADDRESSCNTL_FUNC,
          CFGADDRESSCNTL_REG,
          IosEntry->IDs[PhxIosRbLoopToRbAssignOrder[RbLoop - 1]].InstanceID,
          CfgAddrCtrl.Value
          );
      }
    }
  }

  return SilPass;
}

/**
 * GetMaxSystemRbCountPhx
 *
 * @brief Return the maximum system Root bridge count for PHX.
 *
 * @return uint8_t  The maximum system Root bridge count for PHX
 */
uint8_t
GetMaxSystemRbCountPhx (void)
{
  return (uint8_t)PHX_MAX_HOST_BRIDGES_PER_SOCKET;
}
