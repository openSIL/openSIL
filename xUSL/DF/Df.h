/**
 * @file  Df.h
 * This file contains the DF private structures. The item declared here are NOT
 * seen by the Host.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include "DfClass-api.h"

#define DF_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_DF & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)


/* Function prototypes  */

SIL_STATUS
DfSetInputBlk ( void );

SIL_STATUS
InitializeDataFabricTp1 ( void );

SIL_STATUS
InitializeDataFabricTp2 ( void );

SIL_STATUS
InitializeDataFabricTp3 ( void );

SIL_STATUS DfBaseFabricTopologyConstructor (void);
void FabricSocInit (DFCLASS_INPUT_BLK* DfInputBlock);

/// Timer disable value for DF::DfGlobalCtrl::WDTBaseSel
#define TIMER_DISABLE 3
