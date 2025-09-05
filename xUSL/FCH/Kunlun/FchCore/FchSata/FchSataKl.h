/**
 * @file  FchSataKl.h
 * @brief Kunlun FCH SATA controller functions header
 */
/* Copyright 2025 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCore/FchSata/FchSata.h>

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

SIL_STATUS
InitializeFchSataKlTp1 (void);

SIL_STATUS
InitializeFchSataKlTp2 (void);

SIL_STATUS
InitializeFchSataKlTp3 (void);

SIL_STATUS
InitializeApiFchSataKl (void);
