/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAoacLib.h
 * @brief FCH AOAC function prototypes
 *
 */

#pragma once

#include <SilCommon.h>

#define FCH_AOAC_ESPI         0x1B
#define FCH_AOAC_RETRY        100
#define FCH_AOAC_UART0        0x0B
#define FCH_AOAC_UART1        0x0C
#define FCH_AOAC_UART2        0x10

void
FchAoacPowerOnDev (
  uint8_t Device,
  uint8_t On
  );
