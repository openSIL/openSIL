/**
 * @file
 * @brief OpenSIL CCX IP downcore function and data declaration.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <xSIM.h>
#include "CCX/CcxClass-api.h"

/**********************************************************************************************************************
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
  CCD_MODE_AUTO = 0,            ///< Auto = Enable all CCDs (12 CCDs)
  CCD_MODE_1_CCD,               ///< Enable  1 CCD
  CCD_MODE_2_CCDS,              ///< Enable  2 CCDs
  CCD_MODE_3_CCDS,              ///< Enable  3 CCDs
  CCD_MODE_4_CCDS,              ///< Enable  4 CCDs
  CCD_MODE_5_CCDS,              ///< Enable  5 CCDs
  CCD_MODE_6_CCDS,              ///< Enable  6 CCDs
  CCD_MODE_7_CCDS,              ///< Enable  7 CCDs
  CCD_MODE_8_CCDS,              ///< Enable  8 CCDs
  CCD_MODE_9_CCDS,              ///< Enable  9 CCDs
  CCD_MODE_10_CCDS,             ///< Enable 10 CCDs
  CCD_MODE_11_CCDS,             ///< Enable 11 CCDs
  CCD_MODE_MAX                  ///< Used for bounds checking
} CCD_MODE;

/**
 * @brief   Selection type for down core
 * @details This is used to describe SOCs where there are 2 CCX in the CCD.
 * So each number is used to described the number of cores enabled on each CCX
 * example: 7 + 0 indicates 7 cores enabled on CCX0 and all cores on CCX1 are disabled
*/
typedef enum {
  CCX_DOWN_CORE_AUTO = 0,       ///< Auto
  CCX_DOWN_CORE_1_0,            ///< 1 + 0 mode
  CCX_DOWN_CORE_1_1,            ///< 1 + 1 mode
  CCX_DOWN_CORE_2_0,            ///< 2 + 0 mode
  CCX_DOWN_CORE_3_0,            ///< 3 + 0 mode
  CCX_DOWN_CORE_2_2,            ///< 2 + 2 mode
  CCX_DOWN_CORE_4_0,            ///< 4 + 0 mode
  CCX_DOWN_CORE_3_3,            ///< 3 + 3 mode
  CCX_DOWN_CORE_5_0,            ///< 5 + 0 mode
  CCX_DOWN_CORE_6_0,            ///< 6 + 0 mode
  CCX_DOWN_CORE_7_0,            ///< 7 + 0 mode
  CCX_DOWN_CORE_4_4,            ///< 4 + 4 mode
  CCX_DOWN_CORE_5_5,            ///< 5 + 5 mode
  CCX_DOWN_CORE_6_6,            ///< 6 + 6 mode
  CCX_DOWN_CORE_7_7,            ///< 7 + 7 mode
  CCX_DOWN_CORE_6_5,            ///< 6 + 5 mode
  CCX_DOWN_CORE_MAX             ///< Used for bounds checking
} CCX_DOWN_CORE_MODE;

/**********************************************************************************************************************
 * Function prototypes
 *
 */
SIL_STATUS CcxDownCoreInit (
  CCXCLASS_DATA_BLK *CcxConfigData,
  uint32_t CcdDisMask,
  uint32_t DesiredCcdCount);

/**********************************************************************************************************************
 * Declare macros here
 *
 */
