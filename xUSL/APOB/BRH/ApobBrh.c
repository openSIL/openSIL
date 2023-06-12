/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobBrh.c
 * @brief OpenSIL APOB functions
 *
 */
#include <xSIM.h>
#include <SilCommon.h>
#include <APOB/Common/ApobCmn2Rev.h>
#include <ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>
#include <ApobBrh.h>

extern APOB_COMMON_2_REV_XFER_BLOCK mApobXferBrh;
extern APOB_IP2IP_API mApobBrhApi;

/**
 * InitializeApobApiBrh
 *
 * @brief   Initialize internal and external APIs for Apob
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApobApiBrh (
  void
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_ApobClass, (void *)&mApobXferBrh);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi(SilId_ApobClass, (void *)&mApobBrhApi);
}
