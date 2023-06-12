/**
 * @file  FabricPieRasInit.c
 * @brief OpenSIL DataFabric PIE RAS initialization.
 *
 */
 /* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "FabricPieRasInit.h"
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>

void FabricPieRasInit(DFCLASS_INPUT_BLK *xSimData)
{
    uint32_t i;
    uint32_t j;
    uint32_t k;
    uint32_t InstancesAccountedFor;
    uint32_t WdtCfgRequest;
    uint32_t WdtCntSelRequest;
    HARDWARE_ASSERT_STATUS_HIGH_REGISTER       HwaStsHi;
    HARDWARE_ASSERT_STATUS_LOW_REGISTER        HwaStsLow;
    HARDWARE_ASSERT_MASK_HIGH_REGISTER         HwaMaskHi;
    HARDWARE_ASSERT_MASK_LOW_REGISTER          HwaMaskLow;
    DF_GLOBAL_CTRL_REGISTER                    DfGlblCtrl;
    FABRIC_BLOCK_INSTANCE_COUNT_REGISTER       FabricBlkInstCount;

    DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

    WdtCfgRequest = xSimData->AmdFabricWdtCfg;
    WdtCntSelRequest = xSimData->AmdFabricWdtCntSel;
    if (WdtCntSelRequest == 0xFF) {
        // If AmdFabricWdtCntSel is 'Auto', set WDTCntSel to 6 (5 seconds) for all IPs including PIE
        WdtCntSelRequest = 6;
    }

    for (i = 0; i < DfXGetNumberOfProcessorsPresent(); i++) {
        FabricBlkInstCount.Value = DfXFabricRegisterAccRead (
            i, FABRICBLOCKINSTANCECOUNT_FUNC, FABRICBLOCKINSTANCECOUNT_REG, FABRIC_REG_ACC_BC);
        for (j = 0; j < DfGetNumberOfDiesOnSocket(); j++) {
            InstancesAccountedFor = 0;
            while (InstancesAccountedFor < FabricBlkInstCount.Field.BlkInstCount) {
                HwaStsLow.Value = DfXFabricRegisterAccRead (
                    i, HARDWAREASSERTSTATUSLOW_FUNC, HARDWAREASSERTSTATUSLOW_REG, InstancesAccountedFor);
                if (HwaStsLow.Value != 0) {
                    HwaMaskLow.Value = DfXFabricRegisterAccRead (
                        i, HARDWAREASSERTMASKLOW_FUNC, HARDWAREASSERTMASKLOW_REG, InstancesAccountedFor);
                    HwaStsLow.Value &= ~HwaMaskLow.Value;
                    if (HwaStsLow.Value != 0) {
                        DF_TRACEPOINT(
                          SIL_TRACE_INFO,
                          "  DF Hardware Assert Low: Socket %d, Die %d, InstanceID %d, Value %x\n",
                          i,
                          j,
                          InstancesAccountedFor,
                          HwaStsLow.Value
                          );
                        HwaStsLow.Value = 0;
                    }
                    DfXFabricRegisterAccWrite (
                      i,
                      HARDWAREASSERTSTATUSLOW_FUNC,
                      HARDWAREASSERTSTATUSLOW_REG,
                      InstancesAccountedFor,
                      HwaStsLow.Value
                      );
                }
                HwaStsHi.Value = DfXFabricRegisterAccRead (
                    i, HARDWAREASSERTSTATUSHIGH_FUNC, HARDWAREASSERTSTATUSHIGH_REG, InstancesAccountedFor);
                if (HwaStsHi.Value != 0) {
                    HwaMaskHi.Value = DfXFabricRegisterAccRead (
                        i, HARDWAREASSERTMASKHIGH_FUNC, HARDWAREASSERTMASKHIGH_REG, InstancesAccountedFor);
                    HwaStsHi.Value &= ~HwaMaskHi.Value;
                    if (HwaStsHi.Value != 0) {
                        DF_TRACEPOINT(SIL_TRACE_INFO,
                            "  DF Hardware Assert High: Socket %d, Die %d, InstanceID %d, Value %x\n",
                            i, j, InstancesAccountedFor, HwaStsHi.Value);
                        HwaStsHi.Value = 0;
                    }
                    DfXFabricRegisterAccWrite (
                        i,
                        HARDWAREASSERTSTATUSHIGH_FUNC,
                        HARDWAREASSERTSTATUSHIGH_REG, InstancesAccountedFor, HwaStsHi.Value);
                }
                InstancesAccountedFor++;
            }
        }

        for (k = 0; xSimData->DfGlblCtrlInstanceIds[k] != 0xFFFFFFFF; k++) {
            DfGlblCtrl.Value = DfXFabricRegisterAccRead (
              i,
              DFGLOBALCTRL_FUNC,
              DFGLOBALCTRL_REG,
              xSimData->DfGlblCtrlInstanceIds[k]
              );
            if (WdtCfgRequest <= 3) {
                DfGlblCtrl.Field.WDTBaseSel = WdtCfgRequest;
            }
            if (WdtCntSelRequest <= 7) {
                DfGlblCtrl.Field.PIEWDTCntSel = WdtCntSelRequest;
                DfGlblCtrl.Field.IOMWDTCntSel = WdtCntSelRequest;
                DfGlblCtrl.Field.CCMWDTCntSel = WdtCntSelRequest;
            }
            DfGlblCtrl.Field.DisImmSyncFloodOnFatalErr = xSimData->AmdFabricImmSyncFloodOnFatalErrCtrl ? 0 : 1;
            DfXFabricRegisterAccWrite(
                i, DFGLOBALCTRL_FUNC, DFGLOBALCTRL_REG, xSimData->DfGlblCtrlInstanceIds[k], DfGlblCtrl.Value);
        }
    }

    DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
