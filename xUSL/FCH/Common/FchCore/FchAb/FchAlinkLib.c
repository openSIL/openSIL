/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * @file  FchAlinkLib.c
 * @brief FCH A Link access functions
 *
 */

#include <SilCommon.h>
#include "FchAbReg.h"
#include "FchAlinkLib.h"
#include <FCH/FchClass-api.h>
#include <CommonLib/Io.h>

/**
 * ReadAlink - Read the Alink Registers
 *
 *
 * @param[in] Index - The index of the Alink register
 *
 *
 */
uint32_t
ReadAlink (
  uint32_t Index
  )
{
  uint32_t Data;
  xUSLIoWrite32(ALINK_ACCESS_INDEX, Index);
  Data = xUSLIoRead32(ALINK_ACCESS_DATA);
  //Clear Index
  Index = 0;
  xUSLIoWrite32(ALINK_ACCESS_INDEX, Index);
  return Data;
}

/**
 * WriteAlink - Write the Alink Registers
 *
 *
 * @param[in] Index - The index of the Alink register
 * @param[in] Data - The Data of the Alink register
 *
 *
 */
void
WriteAlink (
  uint32_t Index,
  uint32_t Data
  )
{
  xUSLIoWrite32(ALINK_ACCESS_INDEX, Index);
  xUSLIoWrite32(ALINK_ACCESS_DATA, Data);
  //Clear Index
  Index = 0;
  xUSLIoWrite32(ALINK_ACCESS_INDEX, Index);
}

/**
 * RwAlink - Modify the Alink Registers
 *
 *
 * @param[in] Index - The index of the Alink register
 * @param[in] AndMask - The AND Mask of the Alink register
 * @param[in] OrMask - The OR Mask of the Alink register
 *
 *
 */
void
RwAlink (
  uint32_t Index,
  uint32_t AndMask,
  uint32_t OrMask
  )
{
  uint32_t AccessType;

  AccessType = Index & 0xE0000000;
  if (AccessType == (AXINDC << 29)) {
    WriteAlink((SIL_RESERVED_0280 | AccessType), Index & 0x1FFFFFFF);
    Index = SIL_RESERVED_0278 | AccessType;
  } else if (AccessType == (AXINDP << 29)) {
    WriteAlink((FCH_AX_INDXP_REG38 | AccessType), Index & 0x1FFFFFFF);
    Index = SIL_RESERVED_0279 | AccessType;
  }
  WriteAlink(Index, (ReadAlink(Index) & AndMask) | OrMask);
}
