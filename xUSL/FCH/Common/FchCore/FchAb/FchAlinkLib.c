/**
 * @file  FchAlinkLib.c
 * @brief FCH A Link access functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "FchAbReg.h"
#include "FchAb.h"
#include <FCH/Common/FchCommonCfg.h>
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
  xUSLIoWrite32 (ALINK_ACCESS_INDEX, Index);
  Data = xUSLIoRead32 (ALINK_ACCESS_DATA);
  //Clear Index
  Index = 0;
  xUSLIoWrite32 (ALINK_ACCESS_INDEX, Index);
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
  xUSLIoWrite32 (ALINK_ACCESS_INDEX, Index);
  xUSLIoWrite32 (ALINK_ACCESS_DATA, Data);
  //Clear Index
  Index = 0;
  xUSLIoWrite32 (ALINK_ACCESS_INDEX, Index);
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
  WriteAlink (Index, (ReadAlink (Index) & AndMask) | OrMask);
}
