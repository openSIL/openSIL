/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiCmn2Kl.h
 * @brief Kunlun FCH ACPI Common to Kunlun Transfer function prototypes
 *
 */

#pragma once

#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>

extern FCH_HWACPI_XFER_TABLE mFchHwAcpiXferKl;

void
FchInitPreliminaryPrePcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPrePcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPreliminaryPostPcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPostPcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPreliminaryPreOsHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPreOsHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchHwAcpiEnableIxCKl (
  FCHCLASS_INPUT_BLK *FchDataBlock
  );

void
FchHwAcpiResetRtcExtKl (
  void
  );

void
FchHwAcpiUartLegacyIoInitKl (
  FCHCLASS_INPUT_BLK *FchDataBlock
  );

void
FchHwAcpiAoacInitKl (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );
