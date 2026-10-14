/* SPDX-License-Identifier: MIT */
/** Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuBrh.c
 * @brief OpenSIL SMU services specific to Breithorn
 *
 */

#include <SilCommon.h>
#include <SMU/SmuIp2Ip.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include "SmuBrh.h"
#include "SmuInitBrh.h"

extern SMU_COMMON_2_REV_XFER_BLOCK mSmuXferBrh;
extern SMU_IP2IP_API mSmuBrhApi;

/**--------------------------------------------------------------------
 * InitializeSmuApiBrh
 *
 * @brief   Initialize internal and external APIs for Breithorn
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeSmuApiBrh (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_SmuClass, (void *)&mSmuXferBrh);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi(SilId_SmuClass, (void *)&mSmuBrhApi);
}

/**
 * InitializeSmuTp1Brh
 *
 * @brief   Initialize the SMU IP
 *
 * @details This function initializes the SMU silicon block.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeSmuTp1Brh (void)
{
  SIL_STATUS Status;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = InitializeSmuBrh ();

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * InitializeSmuTp2Brh
 *
 * @brief   Initialize the SMU IP
 *
 * @details This function initializes the SMU silicon block.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeSmuTp2Brh (void)
{
  return SilPass;
}

/**
 * InitializeSmuTp3Brh
 *
 * @brief   Initialize the SMU IP
 *
 * @details This function initializes the SMU silicon block.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeSmuTp3Brh (void)
{
  return SilPass;
}
