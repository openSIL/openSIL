/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuPhx.c
 * @brief OpenSIL SMU services specific to Phoenix
 *
 */
#include <SilCommon.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include <SmuIp2Ip.h>
#include "SmuPhx.h"

extern SMU_COMMON_2_REV_XFER_BLOCK mSmuXferPhx;
extern SMU_IP2IP_API mSmuPhxApi;

/**
 * InitializeApiSmuPhx
 *
 * @brief   Initialize internal and external APIs
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiSmuPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_SmuClass, (void *)&mSmuXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_SmuClass, (void *)&mSmuPhxApi);
}
