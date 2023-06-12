
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  FchSn.h
 * @brief OpenSIL FCH 9004 FCH Class entry point declarations to be used in IpBlkListxxyy.h files
 *
 */

#pragma once

#include <Sil-api.h>

/**
 * InitializeFchApiSn
 *
 * @brief   Initialize Ip-2-Ip APIs for 9004 FCH class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeFchApiSn (void);
