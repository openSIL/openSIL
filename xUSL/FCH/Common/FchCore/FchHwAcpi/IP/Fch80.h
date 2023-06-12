/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Fch80.h
 * @brief FCH 8.0 function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchHwAcpi-api.h>

void
FchHwAcpiEnableBootTimerV80 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiProgramSpreadSpectrumV80 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );
