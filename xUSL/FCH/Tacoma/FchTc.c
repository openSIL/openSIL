/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchTc.c
 * @brief OpenSIL TC FCH Class specific initialization
 *
 */

#include <SilCommon.h>
#include <FCH/FchIp2Ip.h>
#include <FCH/Common/Fch.h>
#include "FchTc.h"

extern FCH_IP2IP_API mFchIp2IpTc;

/**
 * FchClassSetInputBlkTc
 *
 * @brief Setup TC Fch Class Input Block
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
FchClassSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  )
{

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchClassSetInputBlk(SilContext);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchApiTc
 *
 * @brief   Initialize Ip-2-Ip APIs for TC FCH class
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeFchApiTc (
  SIL_CONTEXT   *SilContext
  )
{
  // Initialize FCH IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_FchClass, (void *)&mFchIp2IpTc);
}
