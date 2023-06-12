/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpi.h
 * @brief FCH ACPI function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/FchHwAcpi-api.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiReg.h>

void
FchHwAcpiWriteMmioTable (
  ACPI_REG_WRITE *AcpiTbl
  );

void
FchInitEnableBootTimer (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitTogglePwrGdOnCf9 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchI2cReleaseControl (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitEnableBootTimerV80 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
ProgramFchEnvSpreadSpectrumV80 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchUartInitV80 (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

void
FchInitResetHwAcpiP (
  FCHCLASS_INPUT_BLK *FchData
  );

void
FchInitEnvHwAcpiP (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitResetHwAcpi (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

void
FchInitEnvHwAcpi (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiPreliminaryPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

SIL_STATUS
FchHwAcpiPreliminarySetInputBlk (
  void
  );

#define FCHHWACPI_MAJOR_REV 0
#define FCHHWACPI_MINOR_REV 1
#define FCHHWACPI_INSTANCE  0
