/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIsaTc.h
 * @brief Fch Isa specific function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchIsa-api.h>

#define FCHISA_INPBLK_SIZE_TC   sizeof (FCHISA_INPUT_BLK)

SIL_STATUS
InitializeFchIsaTcTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
FchIsaSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeApiFchIsaTc (
  SIL_CONTEXT  *SilContext
  );
