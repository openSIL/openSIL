/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Mem.h
 * @brief This file contains the MEM private structures.
 */

#pragma once

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

void
DumpMemoryInfoData (
  AMD_MEMORY_SUMMARY   *MemSummary
  );

SIL_STATUS
TranslateChannelInfo (
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  );

SIL_STATUS
GetMemInfo (
  AMD_MEMORY_SUMMARY *MemSummaryTable
  );

SIL_STATUS
GetSmbiosTable (
  SIL_DMI_INFO    *DmiInfoTable
  );

void
SilInitSmbios32Type17 (
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

SIL_STATUS
SilReadSpd (
  uint8_t    SocketId,
  uint8_t    MemChannelId,
  uint8_t    DimmId,
  uint8_t    *SpdBufPtr
  );
