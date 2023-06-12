/**
 * @file  Mpio.c
 * @brief OpenSIL MPIO
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <Mpio/MpioClass-api.h>

// These are common functions for the IP entry point
extern SIL_STATUS InitializeMpioTp1 (void);
extern SIL_STATUS InitializeMpioTp2 (void);
extern SIL_STATUS InitializeMpioTp3 (void);
extern SIL_STATUS MpioClassSetInputBlock (void);

SIL_STATUS
SetMpioApi (void);
