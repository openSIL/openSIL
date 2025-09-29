/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Df.h
 * This file contains the DF private structures. The item declared here are NOT
 * seen by the Host.
 */

#pragma once

#include <xSIM.h>
#include "DfClass-api.h"

#define DF_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_DF & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

#define TIMER_DISABLE 3
