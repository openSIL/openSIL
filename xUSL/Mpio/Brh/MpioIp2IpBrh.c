/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioIp2IpBrh.c
 * @brief OpenSIL MPIO Breithorn I2I API table declaration
 *
 */
#include <Mpio/MpioIp2Ip.h>
#include <Mpio/Common/MpioLib.h>
#include "MpioIp2IpBrh.h"

MPIO_IP2IP_API MpioBrhApi = {
  .MpioServiceRequest = MpioServiceRequestCommon,
  .MpioWritePcieStrap = WritePcieStrapBrh,
  .MpioGetPortStrapIndex = PcieGetPortStrapIndexBrh,
  .MpioGetDpcCapabilityStrap = PcieGetPortDpcCapabilityStrapBrh,
  .MpioGetTphSupportStrap = PcieGetTphSupportStrapBrh,
  .MpioGetMembar0SizeStrap = PcieGetMembar0SizeStrapBrh,
  .MpioGetCxlModeStrap = PcieGetCxlModeStrapBrh,
  .MpioSmnPrivateRegisterRead = MpioSmnPrivateRegRead,
  .MpioSmnPrivateRegisterRMW = MpioSmnPrivateRegRMW
};
