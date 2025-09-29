/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaDefaults.c
 * @brief FCH ISA Default Settings
 *
 */

#include <SilCommon.h>
#include <FchIsa-api.h>

const FCHISA_INPUT_BLK FchIsaDefaults = {
  .LpcConfig = {
    .LpcEnable = CONFIG_LPC_BUS_ENABLE,
    .LpcClk0 = true,
    .LpcClk1 = true
  },
  .SpiConfig = {
    .SpiSpeed = CONFIG_SPI_STD_SPEED,
    .WriteSpeed = CONFIG_SPI_WRT_SPEED,
    .SpiTpmSpeed = CONFIG_SPI_TPM_SPEED,
    .SystemTpmConfig = 0xFF
  }
};
