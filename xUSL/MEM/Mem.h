/**
 * @file  Mem.h
 * @brief This file contains the MEM private structures.
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include "MemClass-api.h"

#define MEM_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_MEM & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)


/* Function prototypes  */

SIL_STATUS
MemSetInputBlk (void);

SIL_STATUS
InitializeMemBlk (void);
