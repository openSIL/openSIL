/**
 * @file  CcxSetMca.c
 * @brief Initialize MCA registers
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <RAS/RasDefs.h>

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

  /// Get mca bank count Core::X86::Msr::MCG_CAP::Count (Global Machine Check Capabilities)
  McaCount = (xUslRdMsr (MSR_MCG_CAP) & MCA_BANKS_VISIBLE_MASK);
  for (McaIndex = 0; McaIndex < McaCount; McaIndex++) {
    McaCfg.Value = xUslRdMsr (MSR_MCA_CFG_BANK0 + MCA_BANK_SIZE * McaIndex);

    /// MCi_CONFIG[32][McaXEnable] = 1 if MGC_CAP[Count] <= 32, else 0
    McaCfg.Field.McaXEnable = (McaCount <= 32) ? 1 : 0;

    /// MCi_CONFIG[34][LogDeferredInMcaStat] = 1 if supported
    if (McaCfg.Field.DeferredErrorLoggingSupported == 1) {
      McaCfg.Field.LogDeferredInMcaStat = 1;
    }
    xUslWrMsr ((MSR_MCA_CFG_BANK0 + MCA_BANK_SIZE * McaIndex), McaCfg.Value);

    /// MSR read to MCA_IPID (MSRC000_2005 + (0x10 (bank size) * bank number))
    McaIpId.Value = xUslRdMsr (MCA_IPID_LS_ADDRESS + (MCA_BANK_SIZE * McaIndex));
    /// This is a CS bank
    if ((McaIpId.Field.HardwareID == CS_MCA_BANK_HARDWARE_ID) && (McaIpId.Field.McaType == CS_MCA_TYPE)) {
      /// Program the MCA_CTL_MASK_CS register
      McaCtlMaskAddr = MCA_CTL_MASK_LS_ADDRESS + McaIndex;
      McaCtlMsk.Value = xUslRdMsr (McaCtlMaskAddr);
      McaCtlMsk.Field.FTI_ADDR_VIOL = 1;
      xUslWrMsr (McaCtlMaskAddr, McaCtlMsk.Value);
    }
  }
}

