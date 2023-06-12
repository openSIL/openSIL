/**
 * @file  FchEmmc.c
 * @brief FCH EMMC functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "FchEmmc.h"
#include <FCH/Common/FchCommonCfg.h>

static FCHEMMC_INPUT_BLK Emmc = {
  .EmmcEnable = 0,
  .EmmcType = 0x9,
  .EmmcDriverType = false,
  .EmmcBoot = false,
  .EmmcAdma2Support = true,
  .EmmcAdmaSupport = true,
  .EmmcSdmaSupport = true,
  .EmmcA64bSupport = true,
  .EmmcD3Support = false,
};

/**-----------------------------------------------------------------------------
 * Name: GetFchEmmcData
 *
 * @brief Get Fch EMMC Data
 *
 * @param None
 *
 * @retval pointer to FCHEMMC_INPUT_BLK.
*/
FCHEMMC_INPUT_BLK *GetFchEmmcData (void)
{
  return &Emmc;
}
