/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchKl.c
 * @brief Kunlun FCH multi-die functions
 *
 */

#pragma once
#include <SilCommon.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/MultiFch/MultiFch.h>
#include <FCH/FchMultiFch-api.h>

/**
 * InitializeMultiFchKlTp1
 *
 * @brief Config Kunlun FCH multi-die during timepoint 1 (pre-pcie)
 *
 */
SIL_STATUS
InitializeMultiFchKlTp1 (void);

/**
 * InitializeMultiFchKlTp2
 *
 * @brief Config Kunlun FCH multi-die during timepoint 2
 *
 */
SIL_STATUS
InitializeMultiFchKlTp2 (void);

/**
 * InitializeMultiFchKlTp3
 *
 * @brief Config Kunlun FCH multi-die during timepoint 3
 *
 */
SIL_STATUS
InitializeMultiFchKlTp3 (void);

/**
 * MultiFchSetInputBlkKl
 *
 * @brief Establish FCH multi-die input defaults for Kunlun
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SilPass
 */
SIL_STATUS
MultiFchSetInputBlkKl (void);

/**
 * InitializeMultiFchApiKl
 *
 * @brief   Initialize transfer tables for Kunlun FCH multi-die class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeMultiFchApiKl (void);

#define MULTIFCH_DATA_SIZE_KL sizeof (FCHMULTIFCH_OUTPUT_BLK)
