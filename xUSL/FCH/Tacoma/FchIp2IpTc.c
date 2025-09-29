/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIp2IpTc.c
 * @brief Defines TC Fch Class I2I API Table
 */

#include <FCH/FchIp2Ip.h>
#include <FCH/Common/FchCommon.h>
#include "FchIp2IpTc.h"

FCH_IP2IP_API
  mFchIp2IpTc = {
  .Header = {
    .IpId = SilId_FchClass
  },
  .FchStall = SilFchStall,
  .FchGpioSlotResetControl = SilFchGpioSlotResetControl
};
