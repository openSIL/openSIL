/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  MemCmn2Rev.h
 * This file defines the types of all functions contained within MEM's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */
#pragma once

#include <APOB/Common/ApobCmn.h>
#include <SilCommon.h>
#include <xPRF-api.h>
#include <MEM/MemClass-api.h>
#include "MemTableTypes.h"

typedef HOST_TO_APCB_CHANNEL_XLAT * (*GET_CHANNEL_XLAT_TABLE) (void);

typedef SIL_STATUS (*CONFIGURE_MEM_INFO) (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummary
  );

typedef SIL_STATUS (*POPULATE_SMBIOS_MEM_INFO) (
  SIL_CONTEXT         *SilContext,
  SIL_DMI_INFO        *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  );

typedef void (*CONFIGURE_TABLE_17_DIMM_PRESENT) (
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  );

typedef struct {
  GET_CHANNEL_XLAT_TABLE              GetChannelXlatTable;
  CONFIGURE_MEM_INFO                  ConfigureMemInfo;
  POPULATE_SMBIOS_MEM_INFO            PopulateSmbiosMemInfo;
  CONFIGURE_TABLE_17_DIMM_PRESENT     ConfigureTable17DimmPresent;
} MEM_COMMON_2_REV_XFER_BLOCK;
