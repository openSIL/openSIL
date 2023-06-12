/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchCmn2RevKl.c
 * @brief Defines Fch Class Kunlun's Cmn2Rev Xfer Table
 */

#include <Common/FchCmn2Rev.h>
#include "FchCmn2RevKl.h"

/**
 * FchXferKl
 *
 * @brief  A temporary Common2Rev Transfer definition for Fch Kunlun Class
 *
 * @details This is a template common-2-Rev transfer table definition
 *          for FCH Kunlun.
 *
 */
FCH_COMMON_2_REV_XFER_BLOCK
  FchXferKl = {
  .FchCmn2RevSample = FchCmn2RevSampleKl      // Fcn for Rev Special Init
    // Additional Common-2-Rev functions inserted here
};
