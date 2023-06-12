/**
 * @file  CcxC6.c
 * @brief Enables Core C6 state.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <assert.h>
#include <CcxClass-api.h>
#include <Ccx.h>

/**
 * CcxInitializeC6
 * @brief This funtion is called by all threads to enables Core C6.
 * @param[in] CStateBaseAddr Pointer to CState base address
 *
 *
 */
void CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock)
{
  SIL_RESERVED2_912_STRUCT  CstateCfg;
  CSTATE_ADDRESS_MSR        CstateAddr;
  uint8_t                   Data8;

  if (CcxInputBlock->AmdCStateMode == 1)
  {
    if (xUslIsComputeUnitPrimary ()) {
      Data8 = CcxInputBlock->AmdCc6Ctrl & 1;
      CstateCfg.Value = 0;
      CstateCfg.Field.Reserved1 = 8;
      CstateCfg.Field.Reserved2 = Data8;
      CstateCfg.Field.Reserved3 = 8;
      CstateCfg.Field.Reserved4 = Data8;
      CstateCfg.Field.Reserved5 = 8;
      CstateCfg.Field.Reserved6 = Data8;
      xUslWrMsr (SIL_RESERVED2_912, CstateCfg.Value);
    }

    CstateAddr.Value = 0;
    CstateAddr.Field.CstateAddr = (uint32_t) CcxInputBlock->AmdCStateIoBaseAddress;
    xUslWrMsr (MSR_CSTATE_ADDRESS, CstateAddr.Value);
  }
}
