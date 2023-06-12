/**
 *  @file SdciConfig.h
 *  @brief SDCI function prototypes, structs, and defines
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include <Sdci/SdciClass-api.h>

#define SDCI_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_SDCI & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)

extern const SDCICLASS_INPUT_BLK SdciClassDflts;

SIL_STATUS
SdciClassSetInputBlock (void);

SIL_STATUS
InitializeSdciTp1 (void);
