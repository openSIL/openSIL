/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsa.h
 * @brief FCH ISA related function prototypes and other definitions
 *
 */

#pragma once

#define SIL_RESERVED_0283   0
#define FCHISA_MAJOR_REV    0
#define FCHISA_MINOR_REV    0

SIL_STATUS
FchIsaSetInputBlk (
  void
  );

SIL_STATUS
InitializeFchIsaTp1 (
  void
  );

SIL_STATUS
InitializeFchIsaTp2 (
  void
  );

SIL_STATUS
InitializeFchIsaTp3 (
  void
  );
