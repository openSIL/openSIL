/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  FchSn.c
 * @brief OpenSIL FCH 9004 FCH Class specific initialization
 *
 */

#include <SilCommon.h>
#include <xSIM-api.h>
#include <FCH/FchIp2Ip.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FCHClass-api.h>
#include "FchSn.h"

static FCH_IP2IP_API mFchIp2IpSn = {
  .Header = {
    .IpId    = SilId_FchClass
  },
  .FchStall  = SilFchStall
};

/*
 * InitializeFchApiSn
 *
 * @brief   Initialize Ip-2-Ip APIs for 9004 FCH class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeFchApiSn (void)
{
  // Initialize FCH IP-to-IP API
  return SilInitIp2IpApi (SilId_FchClass, (void *)&mFchIp2IpSn);
}
