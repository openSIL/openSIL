/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemIp2IpPhx.c
 * @brief OpenSIL MEM I2I API Table
 *
 */

#include <MEM/MemIp2Ip.h>
#include <MEM/Common/Mem.h>

MEM_IP2IP_API MemPhxApi = {
  .TranslateChannelInfo = TranslateChannelInfo,
  .GetMemInfo = GetMemInfo,
  .GetSmbiosMemInfo = GetSmbiosTable
};
