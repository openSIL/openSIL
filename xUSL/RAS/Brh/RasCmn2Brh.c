/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RasCmn2Brh.c
 * @brief RAS Common to BRH transfer table instance.
 *
 */

#include <RAS/Common/RasCmn2Rev.h>
#include "RasCmn2Brh.h"

/**
 * mRasXferBrh
 *
 * @brief RAS Internal API declaration for Brh
 *
 */
RAS_XFER_TABLE mRasXferBrh = {
  .Header = {
    .Version = 2,
  },
  .GetCoreMcaSmnAddrByte = GetCoreMcaSmnAddrByteBrh
};
