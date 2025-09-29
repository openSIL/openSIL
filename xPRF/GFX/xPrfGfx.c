/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfGfx.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        GFX
 */

#include "xPRF-api.h"
#include "xSIM.h"
#include <GFX/GfxClass-api.h>

/**
 * xPrfGetGfxN6InfoStructure
 *
 * @brief   Returns the base address of GFX Info Structure and its size
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetGfxN6InfoStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoBlockDataSize
  )
{
  *InfoBlockDataSize = xUslFindStructureSize(SilContext, SilId_GfxClass, GFXCLASS_N6_INSTANCE);
  return (void *)xUslFindStructure(SilContext, SilId_GfxClass, GFXCLASS_N6_INSTANCE);
}


/**
 * xPrfGetGfxDdiInfoStructure
 *
 * @brief   Returns the base address of GFX Info Structure and its size
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetGfxDdiInfoStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoDdiBlockDataSize
  )
{
  *InfoDdiBlockDataSize = xUslFindStructureSize(SilContext, SilId_GfxClass, GFXCLASS_DDI_INSTANCE);
  return (void *)xUslFindStructure(SilContext, SilId_GfxClass, GFXCLASS_DDI_INSTANCE);
}
