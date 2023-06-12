/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  Ras.h
 * @brief OpenSIL RAS IP initialization function declaration.
 *
 */

#pragma once

#include <SilCommon.h>
#include <xSIM-api.h>
#include <RAS/Common/RasClass-api.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define RAS_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_RAS & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__); \
          } \
        } while (0)


/**
 * @brief Function prototypes for common RAS functions.
 */

void
SetIpMcaCtlMask (
  uint16_t          HardwareId,
  uint16_t          McaType,
  SIL_IP_RAS_POLICY *IpMcaPolicyCfg
  );

void
ProgramCoreMcaIpIdInstanceId (
  SIL_CPU_INFO *RasCpuInfo
  );

/******************************************************************************
 * Declare Function prototypes
 *
 */

SIL_STATUS
InitializeRas (
  RASCLASS_DATA_BLK *RasConfigData
  );

SIL_STATUS
RasClassSetInputBlk (void);
