/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaCmn2Kl.c
 * @brief FCH ISA common to Kunlun transfer functions
 *
 */

#include <SilCommon.h>
#include <FchIsa-api.h>
#include <FchCore/FchIsa/FchIsaCmn2Rev.h>
#include "FchIsaCmn2Kl.h"

FCH_ISA_XFER_TABLE mFchIsaXferKl = {
  .Header = {
    .Version = 2
  },
  .FchIsaDriveSpiClockEarly = FchIsaDriveSpiClockEarlyKl,
  .FchIsaEnableTpmBufferFlush = FchIsaEnableTpmBufferFlushKl
};
