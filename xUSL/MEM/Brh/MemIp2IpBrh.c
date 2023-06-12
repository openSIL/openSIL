/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemIp2IpBrh.c
 * @brief OpenSIL MEM BRH I2I API Table
 *
 */

#include <MEM/MemIp2Ip.h>
#include <MEM/Common/Mem.h>

MEM_IP2IP_API MemBrhApi = {
  .TranslateChannelInfo = TranslateChannelInfo,
  .GetMemInfo = GetMemInfo,
  .GetSmbiosMemInfo = GetSmbiosTable
};
