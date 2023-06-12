
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchSn.h
 * @brief OpenSIL Multi FCH specific definitions
 *
 */

#pragma once

#include <Sil-api.h>

/**
 * InitializeMultiFchSnTp1
 *
 * @brief Config FCH during timepoint 1
 *
 */
SIL_STATUS
InitializeMultiFchSnTp1 (void);

/**
 * InitializeMultiFchSnTp2
 *
 * @brief Config FCH during timepoint 2
 *
 */
SIL_STATUS
InitializeMultiFchSnTp2 (void);

/**
 * InitializeMultiFchSnTp3
 *
 * @brief Config FCH during timepoint 3
 *
 */
SIL_STATUS
InitializeMultiFchSnTp3 (void);

/**
 * InitializeMultiFchApiSn
 *
 * @brief   Initialize Ip-2-Ip APIs for FCH class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeMultiFchApiSn (void);
