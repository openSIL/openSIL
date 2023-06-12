/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  CcxC6.c
 * @brief Enables Core C6 state.
 *
 */


#include <SilCommon.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <assert.h>
#include <CcxClass-api.h>
#include <Ccx.h>

/**
 * CcxInitializeC6
 * @brief This function is called by all threads to enables Core C6.
 * @param[in] CStateBaseAddr Pointer to CState base address
 *
 *
 */
void CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock)
{
  SIL_RESERVED_UNION_0005      CstateCfg;
  CSTATE_ADDRESS_MSR  CstateAddr;
  uint8_t             Cc6Enable;

  if (CcxInputBlock->AmdCStateMode == 1) {
    if (xUslIsComputeUnitPrimary ()) {
      Cc6Enable = CcxInputBlock->AmdCc6Ctrl & 1;
      CstateCfg.Value = 0;
      CstateCfg.Field.field_bits_0_to_5 = 8;
      CstateCfg.Field.field_bit_6 = Cc6Enable;
      CstateCfg.Field.field_bits_8_to_13 = 8;
      CstateCfg.Field.field_bit_14 = Cc6Enable;
      CstateCfg.Field.field_bits_16_to_21 = 8;
      CstateCfg.Field.field_bit_22 = Cc6Enable;
      xUslWrMsr(SIL_RESERVED_0377, CstateCfg.Value);
    }

    CstateAddr.Value = 0;
    CstateAddr.Field.CstateAddr = (uint32_t) CcxInputBlock->AmdCStateIoBaseAddress;
    xUslWrMsr(MSR_CSTATE_ADDRESS, CstateAddr.Value);
  }
}
