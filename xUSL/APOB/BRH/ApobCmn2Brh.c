/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobCmn2Brh.c
 * @brief OpenSIL APOB functions
 *
 */

#include <ApobCmn2Rev.h>
#include <ApobCmn2Brh.h>

APOB_COMMON_2_REV_XFER_BLOCK mApobXferBrh = {
  .ApobGetMaxDieInfo = ApobGetMaxDieInfoBrh,
};
