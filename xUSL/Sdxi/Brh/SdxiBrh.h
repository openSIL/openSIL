/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  SdxiBrh.h
 * @brief OpenSIL SDXI Breithorn specific initialization
 *
 */

#pragma once

#include <SilCommon.h>
#include <Sdxi/SdxiClass-api.h>

// These are common functions for the IP entry point
SIL_STATUS InitializeSdxiBrhTp1 (void);
extern SIL_STATUS SdxiClassSetInputBlock (void);

SIL_STATUS
SetSdxiApiBrh (void);
