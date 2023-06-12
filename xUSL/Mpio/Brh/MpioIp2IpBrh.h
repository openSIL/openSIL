/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioIp2IpBrh.h
 * @brief OpenSIL MPIO Breithorn I2I API function declarations
 *
 */

#pragma once

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <Mpio/MpioClass-api.h>

void
WritePcieStrapBrh (
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  );

uint16_t
PcieGetPortStrapIndexBrh (
  uint16_t Strap,
  uint16_t Port
  );

uint16_t PcieGetPortDpcCapabilityStrapBrh (void);

uint16_t PcieGetTphSupportStrapBrh (void);

uint16_t PcieGetMembar0SizeStrapBrh (void);

uint16_t PcieGetCxlModeStrapBrh (void);
