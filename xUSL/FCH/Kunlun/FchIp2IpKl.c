/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIp2IpKl.c
 * @brief Defines Fch Class Kunlun's I2I API Table
 */

#include <FCH/FchIp2Ip.h>
#include <FCH/Common/FchCommon.h>

FCH_IP2IP_API
  mFchIp2IpKl = {
  .Header = {
    .IpId = SilId_FchClass,
    .Version = 2              // 2 = KL
  },
  .FchStall = SilFchStall,
  .FchGpioSlotResetControl = SilFchGpioSlotResetControl,
};
