/**
 * @file CcxBrandString.c
 * @brief AMD CCX brand string
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "CcxBrandString.h"
#include <CommonLib/CpuLib.h>
#include <SMU/SmuIp2Ip.h>
#include <MsrReg.h>
#include "Ccx.h"

// Default brand string.  Brand string is always 48 bytes
const char str_Unprogrammed_Sample[48] = "AMD Unprogrammed Engineering Sample";

/**
* CcxSetBrandString
* @brief Set the Processor Name String register based on fuses.
*
*
*/
SIL_STATUS
CcxSetBrandString (void) {
  uint32_t      MsrIndex;
  uint64_t      *MsrNameStringPtr;
  uint64_t      BrandString[6];
  SMU_IP2IP_API *SmuApi;

  SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if(SmuApi == NULL) {
    assert (SmuApi != NULL);
    return SilAborted;
  }

  BrandString[5] = 0;
  // Normally, we get Name String from the SMU
  MsrNameStringPtr = (uint64_t *) str_Unprogrammed_Sample;
  CCX_TRACEPOINT (SIL_TRACE_INFO, "Set brand string.\n");

  if (SmuApi->SmuReadBrandString (0,
    ((sizeof (BrandString)) - 1), (uint8_t *) &BrandString[0]) != SilPass) {
    CCX_TRACEPOINT (SIL_TRACE_INFO, "failed to read brand string from SMU.\n");
  } else {
    MsrNameStringPtr = &BrandString[0];
  }

  // Put values into name MSRs, Always write the full 48 bytes
  for (MsrIndex = MSR_CPUID_NAME_STRING0; MsrIndex <= MSR_CPUID_NAME_STRING5;
    MsrIndex++) {
    xUslWrMsr (MsrIndex, *MsrNameStringPtr);
    MsrNameStringPtr++;
  }

  CCX_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
