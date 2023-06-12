/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemCmn2Brh.c
 * @brief OpenSIL MEM Breithorn transfer block functions
 *
 */

#include <MEM/Common/MemCmn2Rev.h>
#include "MemCmn2RevBrh.h"

MEM_COMMON_2_REV_XFER_BLOCK MemXferBrh = {
  .GetChannelXlatTable = GetChannelXlatTableBrh,
  .ConfigureMemInfo = ConfigureMemInfoBrh,
  .PopulateSmbiosMemInfo = PopulateSmbiosMemInfoBrh,
  .ConfigureTable17DimmPresent = ConfigureTable17DimmPresentBrh
};
