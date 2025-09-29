/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfPeiRasInitPhx.c
 * @brief Data fabric Ras initialization.
 *
 */

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/DfCmn2Rev.h>
#include "SilFabricRegistersDfX.h"
#include "DfSilFabricRegistersPhx.h"
#include "DfPhx.h"

/**
 *
 *  DfXCsInit
 *
 *  Description:
 *    This function initializes the DF clock gating feature.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
void DfPhxRasInit (
  SIL_CONTEXT        *SilContext,
  DFCLASS_INPUT_BLK *DfData
  )
{
  HARDWARE_ASSERT_STATUS_HIGH_REGISTER HwaStsHi;
  HARDWARE_ASSERT_STATUS_LOW_REGISTER  HwaStsLow;
  HARDWARE_ASSERT_MASK_HIGH_REGISTER   HwaMaskHi;
  HARDWARE_ASSERT_MASK_LOW_REGISTER    HwaMaskLow;
  DF_GLOBAL_CTRL_REGISTER              DfGlblCtrl;
  FABRIC_BLOCK_INSTANCE_COUNT_REGISTER FabricBlkInstCount;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  uint32_t                             WdtCfgRequest;
  uint32_t                             WdtCntSelRequest;
  uint32_t                             i;
  uint32_t                             j;
  uint32_t                             k;
  uint32_t                             InstancesAccountedFor;
  SIL_STATUS                           Status;
  const uint32_t *DfGlblCtrlInstanceIds;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  WdtCfgRequest = (uint32_t)DfData->AmdFabricWdtCfg;
  WdtCntSelRequest = (uint32_t)DfData->AmdFabricWdtCntSel;

  if (WdtCntSelRequest == 0xFF) {
    WdtCntSelRequest = 6;
  }
  for (i = 0; i < DfXfer->DfGetNumberOfProcessorsPresent(SilContext); i++) {
    for (j = 0; j < DfGetNumberOfDiesOnSocket(SilContext, i); j++) {
      FabricBlkInstCount.Value = DfXFabricRegisterAccRead(SilContext,
        i,
        j,
        FABRICBLOCKINSTANCECOUNT_FUNC,
        DFXFABx040,
        FABRIC_REG_ACC_BC
        );
      for (InstancesAccountedFor = 0; InstancesAccountedFor < FabricBlkInstCount.Field.BlkInstCount;
        InstancesAccountedFor++) {
        HwaStsLow.Value = DfXFabricRegisterAccRead(SilContext,
          i,
          j,
          HARDWAREASSERTSTATUSLOW_FUNC,
          HARDWAREASSERTSTATUSLOW_REG,
          InstancesAccountedFor
          );
        if (HwaStsLow.Value != 0) {
          HwaMaskLow.Value = DfXFabricRegisterAccRead(SilContext,
            i,
            j,
            HARDWAREASSERTMASKLOW_FUNC,
            HARDWAREASSERTMASKLOW_REG,
            InstancesAccountedFor
            );
          HwaStsLow.Value &= ~HwaMaskLow.Value;
          if (HwaStsLow.Value != 0) {
            DF_TRACEPOINT(SIL_TRACE_INFO,
              "DF Hardware Assert Low: Socket %d, Die %d, InstanceID %d, Value %x\n",
              i,
              j,
              InstancesAccountedFor,
              HwaStsLow.Value
              );
            HwaStsLow.Value = 0;
          }
          DfXFabricRegisterAccWrite(SilContext,
            i,
            j,
            HARDWAREASSERTSTATUSLOW_FUNC,
            HARDWAREASSERTSTATUSLOW_REG,
            InstancesAccountedFor,
            HwaStsLow.Value
            );
        }
        HwaStsHi.Value = DfXFabricRegisterAccRead(SilContext,
          i,
          j,
          HARDWAREASSERTSTATUSHIGH_FUNC,
          HARDWAREASSERTSTATUSHIGH_REG,
          InstancesAccountedFor
          );
        if (HwaStsHi.Value != 0) {
          HwaMaskHi.Value = DfXFabricRegisterAccRead(SilContext,
            i,
            j,
            HARDWAREASSERTMASKHIGH_FUNC,
            HARDWAREASSERTMASKHIGH_REG,
            InstancesAccountedFor
            );
          HwaStsHi.Value &= ~HwaMaskHi.Value;
          if (HwaStsHi.Value != 0) {
            DF_TRACEPOINT(SIL_TRACE_INFO,
              "DF Hardware Assert High: Socket %d, Die %d, InstanceID %d, Value %x\n",
              i,
              j,
              InstancesAccountedFor,
              HwaStsHi.Value
              );
            HwaStsHi.Value = 0;
          }
          DfXFabricRegisterAccWrite(SilContext,
            i,
            j,
            HARDWAREASSERTSTATUSHIGH_FUNC,
            HARDWAREASSERTSTATUSHIGH_REG,
            InstancesAccountedFor,
            HwaStsHi.Value
            );
        }
      }
      DfGlblCtrlInstanceIds = DfXfer->DfGetGlblCtrlInstanceIds ();
      for (k = 0; DfGlblCtrlInstanceIds[k] != 0xFFFFFFFF; k++) {
        DfGlblCtrl.Value = DfXFabricRegisterAccRead(SilContext,
          i,
          j,
          DFGLOBALCTRL_FUNC,
          DFGLOBALCTRL_REG,
          DfGlblCtrlInstanceIds[k]
          );
        if (WdtCfgRequest <= 3) {
          DfGlblCtrl.Field.WDTBaseSel = WdtCfgRequest;
        }
        if (WdtCntSelRequest <= 7) {
          DfGlblCtrl.Field.PIEWDTCntSel = WdtCntSelRequest;
          DfGlblCtrl.Field.IOMWDTCntSel = WdtCntSelRequest;
          DfGlblCtrl.Field.CCMWDTCntSel = WdtCntSelRequest;
        }
        DfGlblCtrl.Field.DisImmSyncFloodOnFatalErr = DfData->AmdFabricImmSyncFloodOnFatalErrCtrl ? 0 : 1;
        DfXFabricRegisterAccWrite(SilContext,
          i,
          j,
          DFGLOBALCTRL_FUNC,
          DFGLOBALCTRL_REG,
          DfGlblCtrlInstanceIds[k],
          DfGlblCtrl.Value
          );
      }
    }
  }

  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
