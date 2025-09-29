/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemPhx.c
 * @brief OpenSIL PHX MEM specific initialization functions
 *
 */

#include <SilCommon.h>
#include <MemPhx.h>
#include <SilCommon.h>
#include <MEM/Common/MemCmn2Rev.h>
#include <MEM/MemIp2Ip.h>

extern MEM_COMMON_2_REV_XFER_BLOCK MemXferPhx;
extern MEM_IP2IP_API MemPhxApi;

/**
 * SetMemApiPhx
 *
 * @brief   Initialize internal and external APIs for MEM PHX
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
SetMemApiPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;

  // Initialize MEM Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_MemClass, (void *)&MemXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize MEM IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_MemClass, (void *)&MemPhxApi);
}
