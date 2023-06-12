/**
 * @file Zen4DownCoreInit.c
 * @brief CCX down core and SMT initialization for Zen4
 * Reduces the number of threads in the system, if desired.
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "Zen4DownCoreInit.h"
#include <CcxDownCoreInit.h>
#include <SocServices.h>
#include <CcxClass-api.h>
#include "Ccx.h"

/**
 * GetZen4DesiredCcdCount
 *
 * @brief   Get the desired ccd count and the ccd disable mask
 *
 * @details This function will use the CcdMode input to calculate and return
 *          desired ccd cound and ccd disable mask for Zen4.
 *
 * @param   CcdMode         Input of the CcdMode requested.  For Zen4, valid modes
 *                          are 1-11.
 * @param   CcdDisMask      Output of the disable mask for the requested ccd mode
 * @param   DesiredCcdCount Output of the core count for the requested ccd mode
 *
 * @returns void
 */
void
GetZen4DesiredCcdCount (
  uint8_t     CcdMode,
  uint32_t    *CcdDisMask,
  uint32_t    *DesiredCcdCount
)
{
  switch (CcdMode) {
  case CCD_MODE_11_CCDS:
    /*
     * CcdDisMask:  Disable mask. '0' enables the CCD, '1' disables it
     * (downs) the CCD
     */
    *DesiredCcdCount = 11;      // disable 1 of 12 CCDs
    *CcdDisMask = 0x00000800;   // disable CDD11
    break;
  case CCD_MODE_10_CCDS:
    *DesiredCcdCount = 10;      // disable 2 of 12 CCDs
    *CcdDisMask = 0x00000C00;   // disable CDD11 CCD10
    break;
  case CCD_MODE_9_CCDS:
    *DesiredCcdCount = 9;       // disable 3 of 12 CCDs
    *CcdDisMask = 0x00000E00;   // disable CDD11 CCD10 CCD9
    break;
  case CCD_MODE_8_CCDS:
    *DesiredCcdCount = 8;       // disable 4 of 12 CCDs
    *CcdDisMask = 0x00000F00;   // disable CDD11 CCD10 CCD9 CCD8
    break;
  case CCD_MODE_7_CCDS:
    *DesiredCcdCount = 7;       // disable 5 of 12 CCD
    *CcdDisMask = 0x00000F80;   // disable CDD11 CCD10 CCD9 CCD8 CCD7
    break;
  case CCD_MODE_6_CCDS:
    *DesiredCcdCount = 6;       // disable 6 of 12 CCDs
    *CcdDisMask = 0x00000FC0;   // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6
    break;
  case CCD_MODE_5_CCDS:
    *DesiredCcdCount = 5;       // disable 7 of 12 CCDs
    *CcdDisMask = 0x00000FE0;   // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6 CCD5
    break;
  case CCD_MODE_4_CCDS:
    *DesiredCcdCount = 4;       // disable 8 of 12 CCDs
    *CcdDisMask = 0x00000FF0;   // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6 CCD5 CCD4
    break;
  case CCD_MODE_3_CCDS:
    *DesiredCcdCount = 3;       // disable 9 of 12 CCDs
    *CcdDisMask = 0x00000FF8;   // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6 CCD5 CCD4 CCD3
    break;
  case CCD_MODE_2_CCDS:
    *DesiredCcdCount = 2;       // disable 10 of 12 CCDs
    *CcdDisMask = 0x00000FFC;   // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6 CCD5 CCD4 CCD3 CCD2
    break;
  case CCD_MODE_1_CCD:
    if (SocIsOneCcdModeSupported ()) {
      *DesiredCcdCount = 1;      // disable 11 of 12 CCDs
      *CcdDisMask = 0x00000FFE;  // disable CDD11 CCD10 CCD9 CCD8 CCD7 CCD6 CCD5 CCD4 CCD3 CCD2 CCD1
    }
    break;
  default:
    if(CcdMode != CCD_MODE_AUTO) {
      CCX_TRACEPOINT (SIL_TRACE_INFO,"Zen4 Not Supported CCD Mode %d\n",CcdMode);
    }
    *DesiredCcdCount = 0xFFFFFFFF;
    *CcdDisMask = 0x0;
    break;
  }

  CCX_TRACEPOINT (SIL_TRACE_INFO,
    "Zen4: DesiredCcdCount 0x%X CcdDisMask 0x%X\n",
    *DesiredCcdCount,
    *CcdDisMask);
}
