/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCioInitTc.h
 * @brief Tacoma FCH CIO Module silicon init function prototypes
 */

#pragma once

#include <SilCommon.h>
#include <FchUsb-api.h>

void
FchInitPrePcieCioTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  );
