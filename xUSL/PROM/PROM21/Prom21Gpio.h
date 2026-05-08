/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Gpio.h
 *  @brief Promontory21 GPIO related defines
 */


#pragma once

#include <SilCommon.h>
#include <PROM/PromClass-api.h>

typedef struct {
  uint32_t GpioPinDir;
  uint32_t GpioInputData;
  uint32_t GpioOutputData;
} PT_GPIO_REG;

SIL_STATUS
Prom21GpioWrite (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint8_t            Pin,
  uint8_t            Value
  );

SIL_STATUS
Prom21GpioRead (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint8_t            Pin,
  uint8_t            *Value
  );

SIL_STATUS
Prom21ProgramGpiosInitial (
  PROMCLASS_DATA_BLK *PromDataBlk
  );
