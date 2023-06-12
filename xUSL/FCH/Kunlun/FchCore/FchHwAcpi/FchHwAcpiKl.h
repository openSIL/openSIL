/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiKl.h
 * @brief Kunlun FCH ACPI Module function prototypes
 *
 */

#pragma once


#include <SilCommon.h>
#include <FchHwAcpi-api.h>

#define FCHACPI_INPBLK_SIZE_KL (sizeof (FCHHWACPI_INPUT_BLK))

SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp1 (
  void
  );

SIL_STATUS
InitializeFchHwAcpiKlTp1 (
  void
  );

SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp2 (
  void
  );

SIL_STATUS
InitializeFchHwAcpiKlTp2 (
  void
  );

SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp3 (
  void
  );

SIL_STATUS
InitializeFchHwAcpiKlTp3 (
  void
  );

SIL_STATUS
FchHwAcpiPreliminarySetInputBlkKl (
  void
  );

SIL_STATUS
InitializeApiFchHwAcpiKl (
  void
  );
