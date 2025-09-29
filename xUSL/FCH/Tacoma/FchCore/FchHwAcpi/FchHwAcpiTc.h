/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiTc.h
 * @brief TC FCH HwAcpi Module function prototypes
 *
 */

#pragma once


#include <SilCommon.h>
#include <FchHwAcpi-api.h>

#define FCHACPI_INPBLK_SIZE_TC  (sizeof (FCHHWACPI_INPUT_BLK))

SIL_STATUS
InitializeFchHwAcpiPreliminaryTcTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeFchHwAcpiTcTp1 (
  SIL_CONTEXT  *SilContext
  );


SIL_STATUS
InitializeFchHwAcpiTcTp3 (
  void
  );

SIL_STATUS
FchHwAcpiSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeApiFchHwAcpiTc (
  SIL_CONTEXT  *SilContext
  );
