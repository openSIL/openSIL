/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  DfX.h
 * @brief General DfX function declarations
 */

#pragma once

#include <xSIM.h>
#include <DF/DfClass-api.h>

SIL_STATUS
DfXSetInputBlk (void);

// Data fabric init flow functions
void
DfXPieRasInit (
  DFCLASS_INPUT_BLK *ptrToInputArguments
  );
void DfXClockGatingInit (
  DFCLASS_INPUT_BLK *DfInputBlock
  );
