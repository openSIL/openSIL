/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  Mem.h
 * @brief This file contains the MEM private structures.
 */

#pragma once

#include <xSIM.h>
#include <SilCommon.h>
#include <xPRF-api.h>
#include "MemTableTypes.h"
#include <MEM/MemClass-api.h>

#define MEM_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_MEM & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

/* Function prototypes  */
SIL_STATUS
MemSetInputBlk (void);

void
IntToString (
  char       *String,
  uint8_t    *Integer,
  uint8_t    SizeInByte
  );

SIL_STATUS
TranslateChannelInfo (
  SIL_CONTEXT                *SilContext,
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  );

SIL_STATUS
GetMemInfo (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummaryTable
  );

SIL_STATUS
GetSmbiosTable (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  );

void
SilInitSmbios32Type17 (
  SIL_CONTEXT            *SilContext,
  bool                   DimmPresent,
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  );

void
SilInitSmbios33Type17 (
  bool                       DimmPresent,
  uint8_t                    *SpdData,
  SIL_TYPE17_DMI_INFO        *T17
  );
