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
#define FCH_AOAC_I2C0         0x05
#define FCH_AOAC_I2C1         0x06
#define FCH_AOAC_I2C2         0x07
#define FCH_AOAC_I2C3         0x08
#define FCH_AOAC_I2C4         0x09
#define FCH_AOAC_I2C5         0x0A
#define AOACDEVx15         0x15
#define AOACDEVx0D         0x0D
#define AOACDEVx0E         0x0E
#define AOACDEVx0F         0x0F
#define FCH_AOAC_UART0        0x0B
#define FCH_AOAC_UART1        0x0C
#define FCH_AOAC_UART2        0x10
#define FCH_AOAC_UART3        0x1A
#define FCH_AOAC_UART4        0x14

void
FchAoacPowerOnDev (
  uint8_t Device,
  uint8_t On
  );
