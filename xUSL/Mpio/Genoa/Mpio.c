/**
 * @file  Mpio.c
 * @brief OpenSIL MPIO
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/MpioIp2Ip.h>
#include "Mpio.h"

extern MPIO_COMMON_2_REV_XFER_BLOCK MpioXfer;
extern MPIO_IP2IP_API  MpioApi;

/**
 * InitializeMpio
 *
 * @brief   Initialize internal and external APIs for MPIO Genoa
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
SetMpioApi (void)
{
  SIL_STATUS  Status;

  // Initialize MPIO Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_MpioClass, (void *)&MpioXfer);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize MPIO IP-to-IP API
  return SilInitIp2IpApi (SilId_MpioClass, (void *)&MpioApi);
}
