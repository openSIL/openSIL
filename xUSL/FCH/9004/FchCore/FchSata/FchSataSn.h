/**
 * @file  FchSataSn.h
 * @brief 9004 FCH SATA controller functions header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCore/FchSata/FchSata.h>

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

SIL_STATUS
InitializeFchSataSnTp1 (void);

SIL_STATUS
InitializeFchSataSnTp2 (void);

SIL_STATUS
InitializeFchSataSnTp3 (void);

SIL_STATUS
InitializeApiFchSataSn (void);
