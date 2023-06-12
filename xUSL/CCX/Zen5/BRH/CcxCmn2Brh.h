/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxCmn2Brh.h
 * @brief Prototypes for BRH Zen5 core specific transfer table functions
 *
 */

#pragma once

#include <CcxCmn2Rev.h>

APIC_MODE
GetAmdApicModeBrh (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  );

uint32_t
CalcLocalApicBrh (
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    Ccd,
  uint32_t    Complex,
  uint32_t    Core,
  uint32_t    Thread
  );

void CalculateRmpTableBaseBrh (
  uint64_t    *RmpTableSize,
  uint64_t    *RmpTableBase,
  uint64_t    *SnpMemSizeToCover,
  uint8_t     SplitRmpTableValue
  );

volatile AP_MSR_SYNC * GetApMsrSyncListBrh (void);
