/**
 * @file  RasCmn2.c
 * @brief openSIL RAS IP  transfer block functions.
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <RAS/Common/RasCmn2Rev.h>

/**
 * mRasXferGenoa
 *
 * @brief RAS Internal API declaration for Genoa
 *
 */
RAS_XFER_TABLE mRasXferGenoa = {
  .Header = {
    .Version    = 1,
  },
};
