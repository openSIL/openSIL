/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSataBlk.h
 * @brief FCH SATA Data Block Interface
 *
 */

#pragma once

#include <SilCommon.h>
#include <FCH/FchSata-api.h>

FCHSATA_INPUT_BLK *
FchSataGetInputBlk (
  void
  );

SIL_STATUS
FchSataSetInputBlk (
  void
  );
