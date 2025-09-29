/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcInitPhx.h
 * @brief Fabric MMIO/IO init prototypes
 *
 */
#pragma once

#include <stdint.h>
#include <Sil-api.h>
#include <RcMgr/DfX/RcManager-api.h>

uint8_t
GetMaxSystemRbCountPhx (
  void
  );

SIL_STATUS
SilInitPciBusBasedOnNvVariablePhx (
  SIL_CONTEXT         *SilContext,
  DFX_RCMGR_INPUT_BLK *SilData
  );
