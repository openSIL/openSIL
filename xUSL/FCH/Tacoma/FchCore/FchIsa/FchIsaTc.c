/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIsaTc.c
 * @brief Fch Isa initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Common/FchCore/FchIsa/FchLpc.h>
#include <Common/FchCore/FchIsa/FchSpi.h>
#include <Fch.h>
#include <FchIsa-api.h>
#include <IP/FchIsaIp.h>
#include "FchIsaTc.h"
#include "FchIsaCmn2Tc.h"

/**
 * InitializeFchIsaTcTp1
 *
 * @brief Initialize Fch IP data at timepoint 1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchIsaTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCHCLASS_INPUT_BLK *FchClassInputBlk;
  FCHISA_INPUT_BLK   *FchIsaInputBlk;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchClassInputBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchClass, 0);
  if (FchClassInputBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH Class was not found\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", FchClassInputBlk);

  FchIsaInputBlk = (FCHISA_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchIsa, 0);
  if (FchIsaInputBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH ISA was not found.\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA found blk at: 0x%x \n", FchIsaInputBlk);

  FchInitResetLpc(&(FchIsaInputBlk->LpcConfig));
  FchInitEnvLpc(FchClassInputBlk, &(FchIsaInputBlk->LpcConfig));
  FchIsaInitSpi(SilContext, FchClassInputBlk, &(FchIsaInputBlk->SpiConfig));

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchIsaSetInputBlkTc
 *
 * @brief Setup Fch Isa Input Block
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
FchIsaSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchIsaSetInputBlk(SilContext);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiFchIsaTc
 *
 * @brief   Initialize FCH ISA Cmn2Rev and IP2IP APIs
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchIsaTc (
  SIL_CONTEXT  *SilContext
  )
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable(SilContext, SilId_FchIsa, (void *)&mFchIsaXferTc);
}
