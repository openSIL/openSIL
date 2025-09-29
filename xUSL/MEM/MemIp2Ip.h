/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemIp2Ip.h
 * This file contains the definition of the MEM Ip 2 Ip API.
 * This API provides other openSIL IPs with any MEM functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once

#include <SilCommon.h>
#include <xPRF-api.h>
#include "MemClass-api.h"

// Common function type definitions for functions in MEM's Ip2Ip API

typedef SIL_STATUS (*TRANSLATE_CHANNEL_INFO) (
  SIL_CONTEXT      *SilContext,
  uint8_t          RequestedChannelId,
  uint8_t          *TranslatedChannelId
  );

typedef SIL_STATUS (*GET_MEM_INFO) (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummaryTable
  );

typedef SIL_STATUS (*GET_SMBIOS_MEM_INFO) (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  );

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  TRANSLATE_CHANNEL_INFO        TranslateChannelInfo;
  GET_MEM_INFO                  GetMemInfo;
  GET_SMBIOS_MEM_INFO           GetSmbiosMemInfo;
} MEM_IP2IP_API;
