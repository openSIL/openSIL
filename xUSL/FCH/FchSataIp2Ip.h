/**
 * @file    FchSataIp2Ip.h
 * @details This file contains the definition of the FchSata Ip 2 Ip API.
 *          This API provides other openSIL IPs with any SMU functionality or data
 *          they require while also abstracting revision specific differences.
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM-api.h>

/**
 * IP-to-IP APIs
 *
 * This structure contains an API for inter IP communication.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate API function versions.
 *
 */

typedef void (*INIT_RESET_SATA_PROG_I2I) (
  uint32_t          DieBusNum,
  FCHSATA_INPUT_BLK *FchSata
  );

typedef struct {
  SIL_DATA_BLOCK_ID IpId;       ///< Block Id for this ip
  uint8_t           Version;
} FCHSATA_IP2IP_API_HEADER;

typedef struct {
  FCHSATA_IP2IP_API_HEADER  Header;
  INIT_RESET_SATA_PROG_I2I  FchInitResetSataProgram;
} FCHSATA_IP2IP_API;
