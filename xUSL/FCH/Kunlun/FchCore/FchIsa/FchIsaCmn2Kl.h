/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaCmn2Kl.h
 * @brief FCH ISA common to Kunlun transfer function prototypes
 *
 */

#pragma once

#include <FCH/Common/FchCore/FchIsa/FchIsa.h>
#include <FCH/Common/FchCore/FchIsa/FchIsaCmn2Rev.h>

extern FCH_ISA_XFER_TABLE mFchIsaXferKl;

void
FchIsaDriveSpiClockEarlyKl (
  void
  );

void
FchIsaEnableTpmBufferFlushKl (
  void
  );
