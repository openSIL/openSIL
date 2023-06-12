/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchInit.h
 * @brief Secondary FCH initialization functions prototype
 *
 */

#pragma once

#include <FCH/Common/FchCommonCfg.h>
#include <Fch.h>

typedef struct _FCH_MULITI_FCH_RESET_DATA_BLOCK {
  uint64_t FchAcpiMmioBase[8];            /* FCH ACPI MMIO Base */
} FCH_MULITI_FCH_RESET_DATA_BLOCK;

SIL_STATUS InitializeMultiFch (void);

void
FchSecondaryFchInitAB (
  uint8_t        DieBusNum,
  FCHCLASS_INPUT_BLK *FchData,
  FCHAB_INPUT_BLK *FchAbData
);
