/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  CcxSetMca.c
 * @brief Initialize MCA registers
 *
 */


#include <SilCommon.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <Mca.h>
#include "Ccx.h"

/**
 * CcxSetMca
 *
 * @brief This routine sets all MCA registers
 *
 */
void CcxSetMca (void)
{
  uint8_t                   McaIndex;
  uint8_t                   McaCount;
  uint32_t                  McaCtlMaskAddr;
  SIL_MCA_CONFIG_MSR        McaCfg;
  MCA_CTL_MASK_CS_STRUCT    McaCtlMsk;
  MCA_IPID_LS_STRUCT        McaIpId;

  /// Get mca bank count
  McaCount = (xUslRdMsr(MSR_MCG_CAP) & MCA_BANKS_VISIBLE_MASK);
  for (McaIndex = 0; McaIndex < McaCount; McaIndex++) {
    McaCfg.Value = xUslRdMsr(MSR_MCA_CFG_BANK0 + MCA_BANK_SIZE * McaIndex);

    McaCfg.Field.McaXEnable = (McaCount <= 32) ? 1 : 0;

    if (McaCfg.Field.DeferredErrorLoggingSupported == 1) {
      McaCfg.Field.LogDeferredInMcaStat = 1;
    }
    xUslWrMsr((MSR_MCA_CFG_BANK0 + MCA_BANK_SIZE * McaIndex), McaCfg.Value);

    McaIpId.Value = xUslRdMsr(MCA_IPID_LS_ADDRESS + (MCA_BANK_SIZE * McaIndex));
    /// This is a CS bank
    if ((McaIpId.Field.HardwareID == CS_MCA_BANK_HARDWARE_ID) && (McaIpId.Field.McaType == CS_MCA_TYPE)) {
      McaCtlMaskAddr = MCA_CTL_MASK_LS_ADDRESS + McaIndex;
      McaCtlMsk.Value = xUslRdMsr(McaCtlMaskAddr);
      McaCtlMsk.Field.FTI_ADDR_VIOL = 1;
      xUslWrMsr(McaCtlMaskAddr, McaCtlMsk.Value);
    }
  }
}
