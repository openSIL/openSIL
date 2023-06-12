/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file SdxiConfig.h
 *  @brief SDXI function prototypes, structs, and defines
 */

#pragma once

#include <xSIM.h>
#include <Sdxi/SdxiClass-api.h>

#define SDXI_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (SIL_RESERVED_0388 & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

extern const SDXICLASS_INPUT_BLK SdxiClassDflts;

void
SdxiConfig (
  PCIe_PLATFORM_CONFIG  *Pcie
  );

SIL_STATUS
SdxiClassSetInputBlock (void);
