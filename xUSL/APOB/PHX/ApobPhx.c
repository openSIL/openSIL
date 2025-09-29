/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobPhx.c
 * @brief OpenSIL APOB functions
 *
 */
#include <xSIM.h>
#include <SilCommon.h>
#include <APOB/Common/ApobCmn2Rev.h>
#include <ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>
#include <ApobPhx.h>

extern APOB_COMMON_2_REV_XFER_BLOCK mApobXferPhx;
extern APOB_IP2IP_API mApobPhxApi;

/**
 * InitializeApobApiPhx
 *
 * @brief   Initialize internal and external APIs for Apob
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApobApiPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_ApobClass, (void *)&mApobXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_ApobClass, (void *)&mApobPhxApi);
}
