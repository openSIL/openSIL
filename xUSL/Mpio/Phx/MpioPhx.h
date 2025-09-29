/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioPhx.h
 * @brief MPIO PHX specific initialization function prototype defined.
 *
 */

#pragma once

#include <SilCommon.h>
#include <Mpio/MpioClass-api.h>
#include "MpioPhxData.h"

#define MPIO_DATA_SIZE_PHX sizeof (MPIOCLASS_COMMON_INPUT_BLK) + sizeof (MPIOCLASS_PHX_INPUT_BLK)

// These are common functions for the IP entry point
SIL_STATUS
InitializeMpioPhxTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeMpioPhxTp2 (
  void
  );

SIL_STATUS
InitializeMpioPhxTp3 (
  void
  );

SIL_STATUS
MpioSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeApiMpioPhx (
  SIL_CONTEXT  *SilContext
  );
