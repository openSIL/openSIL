/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Gfx.h
 *  @brief GFX common function prototypes, structs, and defines
 */


#include <xSIM.h>
#include <GFX/GfxClass-api.h>
#include <GFX/Common/GfxDisplayPhySettings.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>

#define GFX_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_GFX & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

void *
GetGfxN6Config (
  SIL_CONTEXT  *SilContext,
  uint32_t     *InfoN6BlockDataSize
  );

void
SilDumpN6Table (
  ATOM_DISPLAY_PHY_TUNING_INFO  *GfxN6OpenSilInputData
  );

void
SilDumpDdiTable (
  DDI_DESCRIPTOR       *DdiConfigData
  );

void *
GetGfxDdiConfig (
  SIL_CONTEXT  *SilContext,
  uint32_t     *InfoDdiBlockDataSize
  );
