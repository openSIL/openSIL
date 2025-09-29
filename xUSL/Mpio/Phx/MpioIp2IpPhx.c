/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioIp2IpPhx.c
 * @brief PHX MPIO core specific transfer table functions
 *
 */

#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/MpioIp2Ip.h>
#include "MpioPcieStrapsPhx.h"
#include "MpioIp2IpPhx.h"

MPIO_IP2IP_API MpioPhxApi = {
  .MpioServiceRequest = MpioServiceRequestCommon,
  .MpioWritePcieStrap = WritePcieStrapPhx,
  .MpioGetPortStrapIndex = PcieGetPortStrapIndexPhx,
  .MpioGetDpcCapabilityStrap = PcieGetPortDpcCapabilityStrapPhx,
  .MpioGetTphSupportStrap = PcieGetTphSupportStrapPhx,
  .MpioGetMembar0SizeStrap = PcieGetMembar0SizeStrapPhx,
  .MpioGetCxlModeStrap = PcieGetCxlModeStrapPhx,
  .MpioSmnPrivateRegisterRead = MpioSmnPrivateRegRead,
  .MpioSmnPrivateRegisterRMW = MpioSmnPrivateRegRMW
};
