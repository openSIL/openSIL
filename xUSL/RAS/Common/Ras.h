/**
 * @file  Ras.h
 * @brief OpenSIL RAS IP initialization function declaration.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <xSIM-api.h>
#include <RAS/Common/RasClass-api.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define RASCLASS_MAJOR_REV   0
#define RASCLASS_MINOR_REV   1
#define RASCLASS_INSTANCE    0

#define RAS_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_RAS & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__); \
        }\
  } while (0)

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
