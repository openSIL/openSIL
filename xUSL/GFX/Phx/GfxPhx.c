/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxPhx.c
 *  @brief GFX configuration routines
 */

#include <string.h>
#include <stdint.h>
#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include <GFX/GfxClass-api.h>
#include <GFX/Common/Gfx.h>
#include <GFX/Common/GfxDisplayPhySettings.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>
#include "GfxPhx.h"

/**
 * InitializeGfxPhxTp1
 *
 * @brief Initialize GFX IP in timepoint 1
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeGfxPhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  uint32_t   InfoN6BlockDataSize;
  uint32_t   InfoDdiBlockDataSize;

  /**
   * InitializeGfxTp1 - IP owners must review the need for this timepoint entry.  If it is not
   * needed, it should be removed.
   */
  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  GetGfxN6Config(SilContext, &InfoN6BlockDataSize);
  GetGfxDdiConfig(SilContext, &InfoDdiBlockDataSize);

  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeGfxPhxTp2
 *
 * Initialize the GFX IP during timepoint 2 (post-Pcie phase)
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeGfxPhxTp2 (
  SIL_CONTEXT  *SilContext
  )
{
  /**
   * There are no planned actions for GFX at TP2.
   */

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeGfxPhxTp3
 *
 * Initialize the GFX IP during timepoint 3 (pre-OS)
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeGfxPhxTp3 (
  SIL_CONTEXT  *SilContext
  )
{
  /**
   * There are no planned actions for GFX at TP3.
   */

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * GfxSetInputBlkPhx
 * @brief Establish GFX config data
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
GfxSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  GFXCLASS_INPUT_BLK *GfxConfigData;

  GfxConfigData = (GFXCLASS_INPUT_BLK *)SilCreateInfoBlock(SilContext,
    SilId_GfxClass,
    sizeof (GFXCLASS_INPUT_BLK),
    GFXCLASS_INSTANCE,
    GFXCLASS_MAJOR_REV,
    GFXCLASS_MINOR_REV
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "GfxSetInputBlk at: 0x%x \n", GfxConfigData);
  if (GfxConfigData == NULL) {
    return SilAborted;
  }
  // fill GFX IP data structure with defaults
  memcpy((void *)GfxConfigData, &GfxClassDflts, sizeof (GFXCLASS_INPUT_BLK));

  return SilPass;
}
