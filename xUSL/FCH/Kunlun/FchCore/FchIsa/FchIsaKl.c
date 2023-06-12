/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIsaKl.c
 * @brief Kunlun Fch Isa initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Common/FchCore/FchIsa/FchIsa.h>
#include <Fch.h>
#include "FchIsaKl.h"
#include "FchIsaCmn2Kl.h"

/**
 * InitializeFchIsaKlTp1
 *
 * @brief Initialize Fch Kl Kunlun IP data at timepoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchIsaKlTp1 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  InitializeFchIsaTp1 ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaKlTp2
 *
 * @brief Initialize Fch Isa Kunlun IP data at timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchIsaKlTp2 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  InitializeFchIsaTp2 ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaKlTp3
 *
 * @brief Initialize Fch Ab Kunlun IP data at timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchIsaKlTp3 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  InitializeFchIsaTp3 ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchIsaSetInputBlkKl
 *
 * @brief Setup Fch Isa Kunlun Input Block
 *
 * @return SIL_STATUS
 */
SIL_STATUS
FchIsaSetInputBlkKl (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchIsaSetInputBlk ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiFchIsaKl
 *
 * @brief   Initialize FCH Kunlun ISA Cmn2Rev and IP2IP APIs
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchIsaKl (
  void
  )
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable(SilId_FchIsa, (void *)&mFchIsaXferKl);
}
