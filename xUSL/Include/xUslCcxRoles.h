/**
 * @file xUslCcxRoles.h
 * @brief cpu helper function definition.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include "Utils.h"

bool xUslIsBsp (void);
bool xUslIsComputeUnitPrimary (void);
bool xUslIsCoreComplexPrimary (void);
bool xUslIsDiePrimary (void);
bool xUslIsSocketPrimary (void);
