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
  const ACPI_REG_WRITE *AcpiTbl
  );

void
FchInitEnableBootTimer (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiProgramCpuRstBTmr (
  void
  );

void
FchInitTogglePwrGdOnCf9 (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchI2cReleaseControl (
  SIL_CONTEXT         *SilContext,
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
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
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiPreliminaryPrePcieInit (
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

SIL_STATUS
FchHwAcpiSetInputBlk (
  SIL_CONTEXT          *SilContext
  );
