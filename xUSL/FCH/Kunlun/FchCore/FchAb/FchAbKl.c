/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchAbKl.c
 * @brief Kunlun Fch AB initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Common/FchCore/FchAb/FchAb.h>
#include <Fch.h>
#include "FchAbKl.h"

/**
 * InitializeFchAbKlTp1
 *
 * @brief Initialize Fch Kl Kunlun IP data at timepoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchAbKlTp1 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  InitializeFchAbTp1 ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchAbKlTp2
 *
 * @brief Initialize Fch Ab Kunlun IP data at timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchAbKlTp2 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchAbKlTp3
 *
 * @brief Initialize Fch Ab Kunlun IP data at timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeFchAbKlTp3 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchAbSetInputBlkKl
 *
 * @brief Setup Fch Ab Kunlun Input Block
 *
 * @return SIL_STATUS
 */
SIL_STATUS
FchAbSetInputBlkKl (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchAbSetInputBlk ();
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiFchAbKl
 *
 * @brief   Initialize internal and external Fch Core APIs for Fch Ab
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchAbKl (void)
{
  return SilPass;
}
