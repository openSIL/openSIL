/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioIp2IpPhx.h
 * @brief Prototypes for PHX MPIO core specific transfer table functions
 *
 */

#pragma once

#include <SilCommon.h>
#include <Mpio/MpioClass-api.h>

void
WritePcieStrapPhx (
  SIL_CONTEXT    *SilContext,
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  );

uint16_t
PcieGetPortStrapIndexPhx (
  uint16_t Strap,
  uint16_t Port
  );

uint16_t
PcieGetPortDpcCapabilityStrapPhx (void);

uint16_t
PcieGetTphSupportStrapPhx (void);

uint16_t PcieGetMembar0SizeStrapPhx (void);

uint16_t PcieGetCxlModeStrapPhx (void);
