/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file  Cxl.h
 * @brief OpenSIL CXL
 *
 */

#pragma once

#include <SilCommon.h>
#include <Cxl/CxlClass-api.h>
#include <Cxl/Common/CxlInit.h>

// These are common functions for the IP entry point
extern SIL_STATUS InitializeCxlTp1 (void);
extern SIL_STATUS CxlClassSetInputBlock (void);

SIL_STATUS
SetCxlApi (void);