/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchSdTc.c
 * @brief Defines Fch core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Fch.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/FchCore/FchSd/FchSd.h>
#include "FchSdTc.h"

/**--------------------------------------------------------------------
 * InitializeFchSdTcTp1
 *
 * @brief Initialize Fch Sd IP data at timepoint 1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchSdTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCHCLASS_INPUT_BLK *LclInpFchBlk;

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", LclInpFchBlk);

  FchInitEnvSd(&LclInpFchBlk->Sd);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
