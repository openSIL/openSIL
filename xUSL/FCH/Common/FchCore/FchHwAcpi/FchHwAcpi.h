/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
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

//
// On-demand FCH services. These back the xPRF FCH service entry points, which
// the host firmware calls at UEFI dependency order rather than at an openSIL
// time point -- FCH runtime services cannot be sequenced by TP1/TP2/TP3.
//

void
FchHwAcpiServicePowerButton (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiServiceAcpiOn (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiServiceAcpiOff (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiServiceSmiTimerStart (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiServiceSmiTimerStop (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

#define FCHHWACPI_MAJOR_REV 0
#define FCHHWACPI_MINOR_REV 1
#define FCHHWACPI_INSTANCE  0
