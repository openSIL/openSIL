/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioBrh.h
 * @brief OpenSIL MPIO Breithorn specific initialization
 *
 */

#pragma once

#include <SilCommon.h>
#include <Mpio/MpioClass-api.h>

// These are common functions for the IP entry point
SIL_STATUS InitializeMpioBrhTp1 (void);
SIL_STATUS InitializeMpioBrhTp2 (void);
SIL_STATUS InitializeMpioBrhTp3 (void);
extern SIL_STATUS MpioClassSetInputBlock (void);

SIL_STATUS
SetMpioApiBrh (void);
