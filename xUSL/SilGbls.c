/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  SilGbls.c
 * @brief Declared Global variables
 *
 */

/*********************************************************************
 * xSIM global Data Instance declaration
 *********************************************************************/
#include <SilCommon.h>

SIL_BLOCK_VARIABLES *mSilMemoryBase = NULL; ///> 'global' var to hold memory block pointer

// Global Variable to hold pointer to host debug service routine
HOST_DEBUG_SERVICE mHostDebugService = NULL;
