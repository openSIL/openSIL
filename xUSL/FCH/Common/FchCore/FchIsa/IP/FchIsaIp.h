/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaIp.h
 * @brief FCH ISA function prototypes
 *
 *
 */

#pragma once

#include <FchIsa-api.h>
#include <FchClass-api.h>

void
FchInitResetLpc (
  FCH_LPC             *FchLpc
  );

void
FchIsaRelocateRom3 (
  SIL_CONTEXT *SilContext
  );

void
FchIsaTpmInit (
  FCH_SPI *FchSpiData
  );
