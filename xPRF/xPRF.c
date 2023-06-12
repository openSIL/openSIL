
/**
 * @file xPRF.c
 * @brief Platform Reference Firmware - exposes Platform specific features
 */
/**
 * Placeholder
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "xPRF.h"
#include <ApobCmn.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

/**
 * DummyXprfFunction
 *
 * @brief   This dummy function is just a placeholder for xPRF.
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
DummyXprfFunction (void)
{

  XPRF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "xPRF dummy function is working\n");
  XPRF_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/**
 * xPrfSetSnpRmp
 *
 * @brief   Program Snp Rmp table MSRs
 *
 * @details This function will program the SNP RMP table base and size MSRs
 *          with the base and size provided by Host-FW.
 *
 * @param   SnpRmpTableBase Base address of the SNP RMP buffer allocated by
 *                          host firmware
 * @param   SnpRmpTableSize The size of the SNP RMP buffer allocated by host
 *                          firmware.
 *
 */
void
xPrfSetSnpRmp (
  uint64_t  SnpRmpTableBase,
  uint64_t  SnpRmpTableSize
  )
{
  SECURE_RMPTABLE_BASE SecureRMPTableBase;
  SECURE_RMPTABLE_END  SecureRMPTableEnd;

  SecureRMPTableBase.Value  = xUslRdMsr (MSR_LS_RMP_BASE);
  SecureRMPTableBase.Field.RmpTableBase = 0;
  SecureRMPTableBase.Value |= SnpRmpTableBase;
  xUslWrMsr (MSR_LS_RMP_BASE, SecureRMPTableBase.Value);
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "MSR RMP BASE Get updated 0x%x\n", SecureRMPTableBase.Field.RmpTableBase);

  SecureRMPTableEnd.Value  = xUslRdMsr (MSR_LS_RMP_END);
  SecureRMPTableEnd.Field.RmpTableEnd = 0;
  SecureRMPTableEnd.Value |= SnpRmpTableSize;
  xUslWrMsr (MSR_LS_RMP_END, SecureRMPTableEnd.Value);
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "MSR RMP END Get updated 0x%x\n", SecureRMPTableEnd.Field.RmpTableEnd);

}
