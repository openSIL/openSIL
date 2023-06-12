/**
 * @file  SilGbls.c
 * @brief Declared Global variables
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/*********************************************************************
 * xSIM global Data Instance declaration
 *********************************************************************/
#include <SilCommon.h>

void *mSilMemoryBase = NULL; ///> 'global' var to hold memory block pointer

// Global Variable to hold pointer to host debug service routine
HOST_DEBUG_SERVICE mHostDebugService = NULL;
