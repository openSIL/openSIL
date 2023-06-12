/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  CCXIp2Ip.h
 * This file contains the definition of the CCX Ip 2 Ip API.
 * This API provides other openSIL IPs with any CCX functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once

#include <SilCommon.h>
#include "Pstates.h"

// Common function type definitions for functions in CCX's Ip2Ip API

typedef uint32_t (*CCX_CALC_LOCAL_APIC) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Ccd,
  uint32_t Complex,
  uint32_t Core,
  uint32_t Thread
  );

typedef SIL_STATUS (*CCX_GET_P_STATE_INFO) (
  PSTATE                Pstate,
  uint32_t              *Frequency,
  uint32_t              *VoltageInuV,
  uint32_t              *PowerInmW,
  bool                  *PstateStatus
  );

typedef SIL_STATUS (*CCX_REORDER_LOG_CCD) (
  uint32_t   LogicalSocket,
  uint32_t   LogicalDie,
  uint32_t   NumberOfCcds,
  uint32_t  *OrderedLogicalCcd
  );

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  CCX_CALC_LOCAL_APIC   CalcLocalApic;
  CCX_GET_P_STATE_INFO  GetPstateInfo;
  CCX_REORDER_LOG_CCD   ReOrderLogicalCcdWithNumaDomainOrder;
} CCX_IP2IP_API;
