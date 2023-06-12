/**
 * @file  CCXIp2Ip.h
 * This file contains the definition of the CCX Ip 2 Ip API.
 * This API provides other openSIL IPs with any CCX functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>

// Common function type definitions for functions in CCX's Ip2Ip API

typedef uint32_t (*CCX_CALC_LOCAL_APIC) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Ccd,
  uint32_t Complex,
  uint32_t Core,
  uint32_t Thread
  );

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  CCX_CALC_LOCAL_APIC   CalcLocalApic;  ///< The Info function
} CCX_IP2IP_API;
