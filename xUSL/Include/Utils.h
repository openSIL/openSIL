/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Utils.h
 * @brief OpenSIL utilities header
 *
 */


#pragma once

#include <SilCommon.h>

void
xUslDumpBuffer (
  void        *Buffer,
  uint32_t    Count,
  uint8_t     DataWidth
  );

int xUslLowBitSet32 (uint32_t Operand);
int xUslHighBitSet32 (uint32_t Operand);
uint8_t xUslBitWidth (size_t Number);
uint32_t xUslGetSetBitCount (uint32_t Value);
