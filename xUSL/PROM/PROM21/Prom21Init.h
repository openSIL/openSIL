/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Init.h
 *  @brief Promontory21 related defines and structures
 */

#pragma once

#include <xSIM.h>
#include <SilCommon.h>
#include <PROM/PromClass-api.h>

SIL_STATUS
InitializePromontoryChipset (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  );
