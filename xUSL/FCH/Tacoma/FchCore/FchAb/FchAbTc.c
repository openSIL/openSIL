/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchAbTc.c
 * @brief Defines Fch core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Common/FchCore/FchAb/FchAb.h>
#include <Fch.h>
#include "FchAbTc.h"

/**--------------------------------------------------------------------
 * InitializeFchAbTcTp1
 *
 * @brief Initialize Fch Ab IP data at timepoint 1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchAbTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCHAB_INPUT_BLK *FchAbInputBlk; //pointer to input blk

  FchAbInputBlk = (FCHAB_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchAb, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH AB found blk at: 0x%x \n", FchAbInputBlk);
  if (FchAbInputBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchInitResetAb ();
  FchInitEnvAb(FchAbInputBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**--------------------------------------------------------------------
 * FchAbSetInputBlkTc
 *
 * @brief Setup Fch Ab Input Block
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
FchAbSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchAbSetInputBlk(SilContext);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
