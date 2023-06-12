/**
 * @file  FchIp2Ip.h
 * This file contains the definition of the FCH Ip 2 Ip API.
 * This API provides other openSIL IPs with any common FCH functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

// Common function type definitions for functions in SMU's Ip2Ip API
typedef void (*FCH_STALL) (
  uint32_t  MicroSeconds
  );

typedef struct {
  SIL_DATA_BLOCK_ID     IpId;       ///< Block Id for this ip
} FCH_IP2IP_API_HEADER;

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  FCH_IP2IP_API_HEADER          Header;
  FCH_STALL                     FchStall;
} FCH_IP2IP_API;
