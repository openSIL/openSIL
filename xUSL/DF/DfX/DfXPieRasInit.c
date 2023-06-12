/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXPieRasInit.c
 * @brief Data Fabric PIE RAS initialization code for DfX
 *
 */

#include <DF/Df.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

void DfXPieRasInit (DFCLASS_INPUT_BLK *ptrToInputArguments)
{
  uint32_t Socket;
  uint32_t Die;
  uint32_t Index;
  uint32_t InstancesAccountedFor;
  uint32_t WdtCfgRequest;
  uint32_t WdtCntSelRequest;
  HARDWARE_ASSERT_STATUS_HIGH_REGISTER       HwaStsHi;
  HARDWARE_ASSERT_STATUS_LOW_REGISTER        HwaStsLow;
  HARDWARE_ASSERT_MASK_HIGH_REGISTER         HwaMaskHi;
  HARDWARE_ASSERT_MASK_LOW_REGISTER          HwaMaskLow;
  uint32_t BlkInstCount;
  const uint32_t *DfGlblCtrlInstanceIds;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS Status;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  WdtCfgRequest = ptrToInputArguments->AmdFabricWdtCfg;
  WdtCntSelRequest = ptrToInputArguments->AmdFabricWdtCntSel;
  if (WdtCntSelRequest == 0xFF) {
    // If AmdFabricWdtCntSel is 'Auto', set WDTCntSel to 6 (5 seconds) for all IPs including PIE
    WdtCntSelRequest = 6;
  }

  for (Socket = 0; Socket < DfXfer->DfGetNumberOfProcessorsPresent (); Socket++) {
    DfXfer->DfAbstractRegAcc((uint8_t)FABRIC_BLOCK_INSTANCE_COUNT_BlkInstCount,
      Socket,
      0,
      FABRIC_REG_ACC_BC,
      0,
      DF_ABSTRACT_REG_READ,
      0,
      &BlkInstCount
      );
    for (Die = 0; Die < DfGetNumberOfDiesOnSocket(Socket); Die++) {
      for (InstancesAccountedFor = 0;
        InstancesAccountedFor < BlkInstCount;
        InstancesAccountedFor++) {
        HwaStsLow.Value = DfXFabricRegisterAccRead(Socket,
          Die,
          HARDWAREASSERTSTATUSLOW_FUNC,
          HARDWAREASSERTSTATUSLOW_REG,
          InstancesAccountedFor
          );
        if (HwaStsLow.Value != 0) {
          HwaMaskLow.Value = DfXFabricRegisterAccRead(Socket,
            Die,
            HARDWAREASSERTMASKLOW_FUNC,
            HARDWAREASSERTMASKLOW_REG,
            InstancesAccountedFor
            );
          HwaStsLow.Value &= ~HwaMaskLow.Value;
          if (HwaStsLow.Value != 0) {
            DF_TRACEPOINT(SIL_TRACE_INFO,
              "  DF Hardware Assert Low: Socket %d, Die %d, InstanceID %d, Value %x\n",
              Socket,
              Die,
              InstancesAccountedFor,
              HwaStsLow.Value
              );
            HwaStsLow.Value = 0;
          }
          DfXFabricRegisterAccWrite(Socket,
            Die,
            HARDWAREASSERTSTATUSLOW_FUNC,
            HARDWAREASSERTSTATUSLOW_REG,
            InstancesAccountedFor,
            HwaStsLow.Value
            );
        }
        HwaStsHi.Value = DfXFabricRegisterAccRead(Socket,
          Die,
          HARDWAREASSERTSTATUSHIGH_FUNC,
          HARDWAREASSERTSTATUSHIGH_REG,
          InstancesAccountedFor
          );
        if (HwaStsHi.Value != 0) {
          HwaMaskHi.Value = DfXFabricRegisterAccRead(Socket,
            Die,
            HARDWAREASSERTMASKHIGH_FUNC,
            HARDWAREASSERTMASKHIGH_REG,
            InstancesAccountedFor
            );
          HwaStsHi.Value &= ~HwaMaskHi.Value;
          if (HwaStsHi.Value != 0) {
            DF_TRACEPOINT(SIL_TRACE_INFO,
              "  DF Hardware Assert High: Socket %d, Die %d, InstanceID %d, Value %x\n",
              Socket,
              Die,
              InstancesAccountedFor,
              HwaStsHi.Value
              );
            HwaStsHi.Value = 0;
          }
          DfXFabricRegisterAccWrite(Socket,
            Die,
            HARDWAREASSERTSTATUSHIGH_FUNC,
            HARDWAREASSERTSTATUSHIGH_REG,
            InstancesAccountedFor,
            HwaStsHi.Value
            );
        }
      }
    }

    DfGlblCtrlInstanceIds = DfXfer->DfGetGlblCtrlInstanceIds ();
    for (Index = 0; DfGlblCtrlInstanceIds[Index] != 0xFFFFFFFF; Index++) {
      if (WdtCfgRequest <= 3) {
        DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_WDTBaseSel,
          Socket,
          0,
          DfGlblCtrlInstanceIds[Index],
          0,
          DF_ABSTRACT_REG_WRITE,
          WdtCfgRequest,
          NULL
          );
      }
      if (WdtCntSelRequest <= 7) {
        DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_PIEWDTCntSel,
          Socket,
          0,
          DfGlblCtrlInstanceIds[Index],
          0,
          DF_ABSTRACT_REG_WRITE,
          WdtCntSelRequest,
          NULL
          );
        DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_IOMWDTCntSel,
          Socket,
          0,
          DfGlblCtrlInstanceIds[Index],
          0,
          DF_ABSTRACT_REG_WRITE,
          WdtCntSelRequest,
          NULL
          );
        DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_CCMWDTCntSel,
          Socket,
          0,
          DfGlblCtrlInstanceIds[Index],
          0,
          DF_ABSTRACT_REG_WRITE,
          WdtCntSelRequest,
          NULL
          );
      }
      DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_DisImmSyncFloodOnFatalErr,
        Socket,
        0,
        DfGlblCtrlInstanceIds[Index],
        0,
        DF_ABSTRACT_REG_WRITE,
        ptrToInputArguments->AmdFabricImmSyncFloodOnFatalErrCtrl ? 0 : 1,
        NULL
        );
    }
  }

  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
