/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaCmn2Tc.h
 * @brief FCH ISA common transfer function prototypes
 *
 */

#pragma once

#include <FCH/Common/FchCore/FchIsa/FchIsa.h>
#include <FCH/Common/FchCore/FchIsa/FchIsaCmn2Rev.h>

extern FCH_ISA_XFER_TABLE mFchIsaXferTc;

void
FchIsaRelocateRom3 (
  SIL_CONTEXT *SilContext
  );

void
FchIsaTpmInit (
  FCH_SPI *FchSpiData
  );
