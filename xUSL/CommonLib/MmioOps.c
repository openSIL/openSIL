/* SPDX-License-Identifier: MIT */
/**
 * @file  MmioOps.c
 * @brief OpenSIL MMIO access functions
 *
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <SilCommon.h>
#include "Mmio.h"

void xUSLMemReadModifyWrite8 (void *Addr, uint8_t AndMask, uint8_t OrMask)
{
  uint8_t Value;
  Value = xUSLMemRead8(Addr);
  Value &= AndMask;
  Value |= OrMask;
  xUSLMemWrite8(Addr, Value);
}

void xUSLMemReadModifyWrite16 (void *Addr, uint16_t AndMask, uint16_t OrMask)
{
  uint16_t Value;
  Value = xUSLMemRead16(Addr);
  Value &= AndMask;
  Value |= OrMask;
  xUSLMemWrite16(Addr, Value);
}

void xUSLMemReadModifyWrite32 (void *Addr, uint32_t AndMask, uint32_t OrMask)
{
  uint32_t Value;
  Value = xUSLMemRead32(Addr);
  Value &= AndMask;
  Value |= OrMask;
  xUSLMemWrite32(Addr, Value);
}
