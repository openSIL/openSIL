/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioInitFlowPhx.c
 *  @brief tbd
 */

#include <Mpio/Common/MpioInitLib.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include "MpioCmn2Phx.h"

/**--------------------------------------------------------------------
 *
 * MpioReleasePortPhx
 *
 * @brief      Mpio Port Authentication before Training
 *
 * @details    This function is not supported for Phx
 *
 * @param[in]  GnbHandle              Pointer to GnbHandle pointer
 * @param[in]  PlatformTopology       Pointer to the platform BIOS supplied platform configuration
 * @param[in]  MpioData               Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioReleasePortPhx (
  GNB_HANDLE             *GnbHandle,
  PCIe_ENGINE_CONFIG     *Engine
  )
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  UNUSED(GnbHandle);
  UNUSED(Engine);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
