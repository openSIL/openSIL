/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfX.h
 * @brief General DfX function declarations
 */

#pragma once

#include <xSIM.h>
#include <DF/DfClass-api.h>

SIL_STATUS
DfXSetInputBlk (
  SIL_CONTEXT  *SilContext
  );

// Data fabric init flow functions
void
DfXPieRasInit (
  SIL_CONTEXT       *SilContext,
  DFCLASS_INPUT_BLK *ptrToInputArguments
  );
void DfXClockGatingInit (
  SIL_CONTEXT       *SilContext,
  DFCLASS_INPUT_BLK *DfInputBlock
  );
