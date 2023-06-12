/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIp2Ip.h
 * This file contains the definition of the FCH Ip 2 Ip API.
 * This API provides other openSIL IPs with any common FCH functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once
#include <xSIM-api.h>
#include <stdint.h>
#include <FCH/FchClass-api.h>

// Common function type definitions for functions in SMU's Ip2Ip API
typedef void (*FCH_STALL) (
  uint32_t  MicroSeconds
  );

typedef SIL_STATUS (*FCH_GPIO_SLOT_RST_CTRL) (
  void
  );

typedef void (*FCH_SEC_FCH_INIT_AB) (
  uint8_t             DieBusNum,
  FCHCLASS_INPUT_BLK  *FchData,
  FCHAB_INPUT_BLK     *FchAbData
  );

typedef struct {
  SIL_DATA_BLOCK_ID     IpId;       ///< Block Id for this ip
  uint32_t              Version;
} FCH_IP2IP_API_HEADER;

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  FCH_IP2IP_API_HEADER          Header;
  FCH_STALL                     FchStall;
  FCH_GPIO_SLOT_RST_CTRL        FchGpioSlotResetControl;
} FCH_IP2IP_API;
