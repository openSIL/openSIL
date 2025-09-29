/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAb.h
 * @brief FCH AB Entry point definitions
 *
 * This file holds the declarations for the AB sub-component entry points for
 * use in the IpBlkListFxxMxx.c files.
 * Only the information needed by for the entry point is included here.
 */

#pragma once

#include <FchAb-api.h>

void
FchSecondaryFchInitAB (
  uint32_t           IohcBusNumber,
  uint64_t           AcpiMmioBaseAddr,
  FCHAB_INPUT_BLK    *FchAbInputBlock
  );

SIL_STATUS
FchAbSetInputBlk (
  SIL_CONTEXT  *SilContext
  );

void
FchInitResetAb (
  void
  );

void
FchInitEnvAb (
  FCHAB_INPUT_BLK *FchAbInputBlk
  );
