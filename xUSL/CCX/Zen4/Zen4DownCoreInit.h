/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Zen4DownCoreInit.h
 * @brief OpenSIL CCX IP downcore function and data declaration.
 *
 */

#pragma once
#include <CCX/CcxClass-api.h>

/******************************************************************************
 * Module data structure
 *
 */

/**
 * @brief Selection type for down core
 * @brief   Selection type for CCD down binning
 * @details The values are used the describe the number core complex dies
 *          that should be enabled.
 */

typedef enum {
  CCD_MODE_AUTO = 0,            ///< Auto = Enable all CCDs
  CCD_MODE_1_CCD,               ///< Enable  1 CCD
  CCD_MODE_MAX                  ///< Used for bounds checking
} CCD_MODE;

/**
 * @brief   Selection type for down core
 */
typedef enum {
  CCX_DOWN_CORE_AUTO = 0,     ///< Auto
  CCX_DOWN_CORE_1,            ///< 1 mode
  CCX_DOWN_CORE_2,            ///< 2 mode
  CCX_DOWN_CORE_3,            ///< 3 mode
  CCX_DOWN_CORE_4,            ///< 4 mode
  CCX_DOWN_CORE_5,            ///< 5 mode
  CCX_DOWN_CORE_6,            ///< 6 mode
  CCX_DOWN_CORE_7,            ///< 7 mode
  CCX_DOWN_CORE_MAX           ///< Used for bounds checking
} CCX_DOWN_CORE_MODE;

/******************************************************************************
 * Function prototypes
 *
 */
void
Zen4StoreDowncoreCoreCount (
  SIL_CONTEXT       *SilContext,
  CCXCLASS_DATA_BLK *CcxConfigData
  );
