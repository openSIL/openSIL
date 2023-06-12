/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file    FchUsbIp2Ip.h
 * @details This file contains the definition of the FchUsb Ip 2 Ip API.
 *          This API provides other openSIL IPs with any FCH USB functionality or data
 *          they require while also abstracting revision specific differences.
 */

#pragma once

#include <xSIM-api.h>
#include <FCH/FchUsb-api.h>

typedef struct {
  SIL_DATA_BLOCK_ID IpId;
  uint8_t           Version;
} FCHUSB_IP2IP_API_HEADER;

typedef struct {
  FCHUSB_IP2IP_API_HEADER Header;
} FCHUSB_IP2IP_API;
