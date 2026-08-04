/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromInit.h
 *  @brief All PROM related defines and structures
 */

#pragma once

#include <xSIM.h>
#include <SilCommon.h>
#include <PROM/PromClass-api.h>

extern const PROMCLASS_DATA_BLK PromClassDflts;

#define PROM_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_PROM & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

SIL_STATUS
PromClassSetInputBlock (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializePromTp1 (
  SIL_CONTEXT  *SilContext
  );

