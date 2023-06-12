/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIsaSn.h
 * @brief Fch Isa Kunlun specific function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchIsa-api.h>

#define FCHISA_INPBLK_SIZE_KL   sizeof (FCHISA_INPUT_BLK)

SIL_STATUS
InitializeFchIsaKlTp1 (
  void
  );

SIL_STATUS
InitializeFchIsaKlTp2 (
  void
  );

SIL_STATUS
InitializeFchIsaKlTp3 (
  void
  );

SIL_STATUS
FchIsaSetInputBlkKl (
  void
  );

SIL_STATUS
InitializeApiFchIsaKl (
  void
  );
