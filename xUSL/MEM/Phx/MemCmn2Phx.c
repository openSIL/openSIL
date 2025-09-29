/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemCmn2Phx.c
 * @brief OpenSIL MEM transfer block functions
 *
 */

#include <MEM/Common/MemCmn2Rev.h>
#include "MemCmn2Phx.h"

MEM_COMMON_2_REV_XFER_BLOCK MemXferPhx = {
  .GetChannelXlatTable = GetChannelXlatTablePhx,
  .ConfigureMemInfo = ConfigureMemInfoPhx,
  .PopulateSmbiosMemInfo = PopulateSmbiosMemInfoPhx,
  .ConfigureTable17DimmPresent = ConfigureTable17DimmPresentPhx
};
