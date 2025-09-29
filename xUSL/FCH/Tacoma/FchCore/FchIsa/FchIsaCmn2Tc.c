/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaCmn2Tc.c
 * @brief FCH ISA common transfer functions
 *
 */

#include <SilCommon.h>
#include <FchIsa-api.h>
#include <FchCore/FchIsa/FchIsaCmn2Rev.h>
#include "FchIsaCmn2Tc.h"

FCH_ISA_XFER_TABLE mFchIsaXferTc = {
  .Header = {
    .Version = 2
  },
  .FchIsaRelocateRom3 = FchIsaRelocateRom3,
  .FchIsaTpmInit = FchIsaTpmInit
};
