/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchLpc.c
 * @brief FCH LPC data and functions
 *
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchReg.h>
#include "FchLpc.h"

/**
 * FchInitEnvLpcPciTable - PCI device registers initial
 * during early POST.
 *
 */
REG8_MASK FchInitEnvLpcPciTable[] =
{
  {0x00, LPC_BUS_DEV_FUN, 0},
  {FCHD14F3x078, 0xFC, 00},
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitEnvLpc
 * @brief Config LPC controller before PCI
 *
 *
 * @param[in] FchData FchDataBlock configuration structure pointer.
 * @param[in] FchLpc Fch LPC configuration structure pointer.
 *
 */
void
FchInitEnvLpc (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_LPC            *FchLpc
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // LPC CFG programming
  if (FchLpc->LpcEnable) {
    FchDataPtr->FchRunTime.FchDeviceEnableMap |= BIT_32(4);
  } else {
    FchDataPtr->FchRunTime.FchDeviceEnableMap &= (~BIT_32(4));
    return; //return if LPC is disabled
  }

  ProgramPciByteTable(FchInitEnvLpcPciTable, sizeof (FchInitEnvLpcPciTable) / sizeof (FchInitEnvLpcPciTable[0]));

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
