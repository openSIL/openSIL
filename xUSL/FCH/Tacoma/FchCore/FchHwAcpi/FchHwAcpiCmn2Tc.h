/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiCmn2Tc.h
 * @brief FCH HwAcpi Common to Tc Transfer function prototypes
 *
 */

#pragma once

#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>

extern FCH_HWACPI_XFER_TABLE mFchHwAcpiXferTc;

void
FchHwAcpiPrePcieInitTc (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiAoacInitTc (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiI2cI3cInitTc (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

void
FchHwAcpiUartInitTc (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

void
FchHwAcpiSpreadSpectrumTc (
  bool Enable
  );

void
FchHwAcpiZstateTimingTc (
  void
  );
