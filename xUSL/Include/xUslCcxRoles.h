/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file xUslCcxRoles.h
 * @brief cpu helper function definition.
 *
 */

#pragma once
#include "Utils.h"

bool xUslIsBsp (void);
bool xUslIsComputeUnitPrimary (void);
bool xUslIsCoreComplexPrimary (void);
bool xUslIsDiePrimary (void);
bool xUslIsSocketPrimary (void);
