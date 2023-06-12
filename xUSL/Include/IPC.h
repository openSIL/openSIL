/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * @file  IPC.h
 * @brief OpenSIL IPC functions prototype
 *
 */

#pragma once

#include <Sil-api.h>

/**********************************************************************************************************************
 * @brief Declare common variables here
 *
 */



/**
 * @brief Declare function prototypes here
 *
 */

SIL_STATUS SendIPCFromx86ToASP (int IPCCommand);

/**********************************************************************************************************************
 * @brief Declare common macros here
 *
 */

#define SEND_eSIM_HOB_BASE_ADDRESS                  0xDEADBEEF
