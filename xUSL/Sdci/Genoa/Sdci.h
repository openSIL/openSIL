/**
 * @file  Sdci.h
 * @brief OpenSIL SDCI
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <Sdci/SdciClass-api.h>

// These are common functions for the IP entry point
extern SIL_STATUS InitializeSdciTp1 (void);
extern SIL_STATUS SdciClassSetInputBlock (void);

SIL_STATUS
SetSdciApi (void);
