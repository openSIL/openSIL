/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file CcxBrandString.c
 * @brief AMD CCX brand string
 *
 */

#include <SilCommon.h>
#include "CcxBrandString.h"
#include <CommonLib/CpuLib.h>
#include <SMU/SmuIp2Ip.h>
#include <MsrReg.h>
#include "Ccx.h"

// Default brand string.  Brand string is always 48 bytes
const char str_Unprogrammed_Sample[48] = "AMD Unprogrammed Engineering Sample";

/**
 * CcxSetBrandStringCommon
 * @brief Set the Processor Name String register.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 *
 */
SIL_STATUS
CcxSetBrandStringCommon (
  SIL_CONTEXT    *SilContext
  )
{
  uint32_t      MsrIndex;
  uint64_t      *MsrNameStringPtr;
  uint64_t      BrandString[6];
  SMU_IP2IP_API *SmuApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Unable to get SMU I2I API.\n");
    assert(Status == SilPass);
  } else {
    BrandString[5] = 0;
    // Normally, we get Name String from the SMU
    MsrNameStringPtr = (uint64_t *) str_Unprogrammed_Sample;
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Set brand string.\n");
    if (SmuApi->SmuReadBrandString(SilContext,
      0,
      ((sizeof (BrandString)) - 1),
      (uint8_t *) &BrandString[0]
      ) != SilPass) {
      CCX_TRACEPOINT(SIL_TRACE_INFO, "failed to read brand string from SMU.\n");
    } else {
      MsrNameStringPtr = &BrandString[0];
    }

    // Put values into name MSRs, Always write the full 48 bytes
    for (MsrIndex = MSR_CPUID_NAME_STRING0; MsrIndex <= MSR_CPUID_NAME_STRING5;
      MsrIndex++) {
      xUslWrMsr(MsrIndex, *MsrNameStringPtr);
      MsrNameStringPtr++;
    }
  }

  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}
