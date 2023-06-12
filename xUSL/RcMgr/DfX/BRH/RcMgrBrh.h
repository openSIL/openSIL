/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RcMgrBrh.h
 * @brief OpenSIL Resource Manager Silicon Init. Functions declarations
 *
 */

#pragma once

#include <SilCommon.h>
#include <RcMgr/DfX/RcManager4-api.h>

#define RCMGR_DATA_SIZE_BRH  sizeof (DFX_RCMGR_INPUT_BLK)

SIL_STATUS
RcMgrSetInputBlkBrh (void);

SIL_STATUS
InitializeResourceManagerBrhTp1 (void);

SIL_STATUS
InitializeResourceManagerBrhTp2 (void);

SIL_STATUS
InitializeResourceManagerBrhTp3 (void);

SIL_STATUS
InitializeRcMgrApiBrh (void);
