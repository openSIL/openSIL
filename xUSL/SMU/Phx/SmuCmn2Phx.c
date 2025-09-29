/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuCmn2Phx.c
 * @brief OpenSIL SMU PHX transfer block functions
 *
 */

#include <SMU/SmuDefs.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include "SmuCmn2Phx.h"

SMU_COMMON_2_REV_XFER_BLOCK mSmuXferPhx = {
  .SmuServiceRequest = SmuServiceRequestPhx
};
