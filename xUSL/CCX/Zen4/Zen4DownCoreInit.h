/**
 * @file
 * @brief OpenSIL CCX IP downcore function and data declaration.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <CCX/CcxClass-api.h>

/******************************************************************************
 * Module data structure
 *
 */

/******************************************************************************
 * Function prototypes
 *
 */
void GetZen4DesiredCcdCount (uint8_t CcdMode,
    uint32_t *CcdDisMask,
    uint32_t *DesiredCcdCount);

/******************************************************************************
 * Declare macros here
 *
 */
