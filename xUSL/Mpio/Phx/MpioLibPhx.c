/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioLibPhx.c
 * @brief MPIO firmware messaging interface library
 *
 */

#include <SilCommon.h>
#include "MpioCmn2Phx.h"

#define MPIOxc910554ul                        0xc910554ul

/**
 * MpioGetC2pmsgDoorbellAddressPhx
 *
 * @brief Routine to get the PCIe port strap index value
 *
 * @param  Strap        PCIe strap value
 * @param  Port         Port ID on wrapper
 *
 * @return uint16_t     Port strap index
 */
uint32_t
MpioGetC2pmsgDoorbellAddressPhx (void) {
  uint32_t MpioC2pmsgDoorbellAddress;

  MpioC2pmsgDoorbellAddress = MPIOxc910554ul;
  return MpioC2pmsgDoorbellAddress;
}
