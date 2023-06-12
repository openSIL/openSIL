/**
 * @file  FchHwAcpiHs.h
 * @brief FCH ACPI controller functions header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define FCH_VERSION_9004                 0x0800

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */
void
FchInitPrePcieHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPrePcieHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPostPcieHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPostPcieHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPreOsHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

void
FchInitPreOsHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

SIL_STATUS
InitializeApiFchHwAcpiSn (void);

/*******************************************************************************
 * @brief Module data structure
 *
 */


/*******************************************************************************
 * @brief Declare macros here
 *
 */
