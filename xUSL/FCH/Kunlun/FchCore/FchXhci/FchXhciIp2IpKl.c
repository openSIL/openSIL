/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciIp2IpKl.c
 * @brief Kunlun FCH XHCI IP to IP API functions
 *
 */

#include <SilCommon.h>
#include <FCH/FchUsbIp2Ip.h>

FCHUSB_IP2IP_API mFchUsbApiKl = {
  .Header = {
    .IpId = SilId_FchUsb,
    .Version = 1,
  },
};
