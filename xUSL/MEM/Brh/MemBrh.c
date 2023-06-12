/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemBrh.c
 * @brief OpenSIL BRH MEM specific initialization functions
 *
 */

#include <SilCommon.h>
#include <MemBrh.h>
#include <SilCommon.h>
#include <MEM/Common/MemCmn2Rev.h>
#include <MEM/MemIp2Ip.h>

extern MEM_COMMON_2_REV_XFER_BLOCK MemXferBrh;
extern MEM_IP2IP_API MemBrhApi;

/**--------------------------------------------------------------------
 * SetMemApiBrh
 *
 * @brief   Initialize internal and external APIs for MEM BRH
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
SetMemApiBrh (void)
{
  SIL_STATUS Status;

  // Initialize MEM Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_MemClass, (void *)&MemXferBrh);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize MEM IP-to-IP API
  return SilInitIp2IpApi(SilId_MemClass, (void *)&MemBrhApi);
}
