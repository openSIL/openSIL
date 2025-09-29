/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSpi.h
 * @brief FCH SPI function prototypes
 *
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchIsa-api.h>
#include <FchClass-api.h>

void FchIsaInitSpi (
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_SPI            *FchSpiData
  );
