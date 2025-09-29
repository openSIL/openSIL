/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file RcMgrPhx.h
 * @brief Fabric MMIO/IO init prototypes
 *
 */

#pragma once

#include <Sil-api.h>
#include <RcMgr/DfX/RcManager-api.h>

#define RCMGR_DATA_SIZE_PHX  sizeof (DFX_RCMGR_INPUT_BLK)

SIL_STATUS
RcMgrSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeRcMgrPhxTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeRcMgrPhxTp2 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeRcMgrPhxTp3 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeApiRcMgrPhx (
  SIL_CONTEXT  *SilContext
  );
