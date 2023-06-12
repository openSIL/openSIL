/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  SocUtil.h
 * @brief OpenSIL SoC utilities prototype
 *
 *
 */


#pragma once

#include "xSIM.h"

/**********************************************************************************************************************
 * @brief Declare common variables here
 *
 */

typedef struct {
  uint8_t Family;
  uint8_t ExtFamily;
  uint8_t Model;
  uint8_t ExtModel;
  uint8_t Stepping;
} SilSocCommonData;


/*********************************************************************
 * API Function prototypes
 *********************************************************************/

/**
 * @brief IdentifySoC - Collect Info on the SOC
 *
 * @return int
 */
void IdentifySoC (SilSocCommonData *SocData);
