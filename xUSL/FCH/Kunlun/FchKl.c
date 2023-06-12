/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchKl.c
 * @brief OpenSIL FCH Kunlun FCH Class specific initialization
 *
 */

#include <SilCommon.h>
#include <FCH/FchIp2Ip.h>
#include <FCH/Common/Fch.h>
#include "FchKl.h"

extern FCH_IP2IP_API mFchIp2IpKl;

/**
 * FchClassSetInputBlkKl
 *
 * @brief Setup Fch Class Kunlun Input Block
 *
 * @return SIL_STATUS
 */
SIL_STATUS
FchClassSetInputBlkKl (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchClassSetInputBlk ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchApiKl
 *
 * @brief   Initialize Ip-2-Ip APIs for Kunlun FCH class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeFchApiKl (void)
{

  // Initialize FCH IP-to-IP API
  return SilInitIp2IpApi(SilId_FchClass, (void *)&mFchIp2IpKl);
}
