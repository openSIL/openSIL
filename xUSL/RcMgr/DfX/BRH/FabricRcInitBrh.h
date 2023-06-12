/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcInitBrh.h
 * @brief Fabric MMIO/IO init prototypes for BRH
 *
 */

#pragma once

#include <SilCommon.h>
#include <RcMgr/DfX/RcManager4-api.h>

SIL_STATUS
SilInitPciBusBasedOnNvVariableBrh (
  DFX_RCMGR_INPUT_BLK *SilData
  );
