/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxCmn2Phx.h
 * @brief Prototypes for PHX Zen4 core specific transfer table functions
 *
 */

#pragma once

#include <CcxCmn2Rev.h>

uint32_t
CalcLocalApicPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Complex,
  uint32_t     Core,
  uint32_t     Thread
  );

void CalculateRmpTableBasePhx (
  SIL_CONTEXT   *SilContext,
  uint64_t      *RmpTableSize,
  uint64_t      *RmpTableBase,
  uint64_t      *SnpMemSizeToCover,
  uint8_t       SplitRmpTableValue
  );

APIC_MODE
GetAmdApicModePhx (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  );

SIL_STATUS
CcxLaunchThreadPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     LogicalCcd,
  uint32_t     LogicalComplex,
  uint32_t     LogicalCore,
  uint32_t     LogicalThread
  );

void
CcxCacheInitPhx (void);

void
SetBrandStringPhx (
  SIL_CONTEXT    *SilContext
  );

bool
IsS3SupportedPhx (
  void
  );

bool
IsGameModeSupportedPhx (
  void
  );

bool
IsOneCcdModeSupportedPhx (
  void
  );
