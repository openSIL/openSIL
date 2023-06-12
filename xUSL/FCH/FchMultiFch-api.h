/* SPDX-License-Identifier: MIT */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file
 * @brief openSIL-Host Multi FCH IP interface
 *
 */

#include <xSIM-api.h>

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_MULTI_FCH  FCH (FCH) Multi FCH API
 *
 *   The MULTI_FCH component contains initialization for a secondary FCH, if
 *   present
 *
 *   See the 'Files - @ref MultiFch-api.h' section of this document for
 *   further details.
 *
 * @endcond
 */

#pragma once

#define SIL_RESERVED_0353                   0
#define SIL_RESERVED_0357                   2
#define SIL_RESERVED_0358             1
#define SIL_RESERVED_0356          8
#define SIL_RESERVED_0354   8
#define SIL_RESERVED_0355      (SIL_RESERVED_0356 * SIL_RESERVED_0354)

typedef struct {
  uint32_t MajorRevistion;
  uint32_t MinorRevision;
  uint32_t NumberOfSockets;
  uint32_t NumberOfDiePerSocket;
  uint32_t NumberOfRbPerDie;
  uint32_t ArraySize;
  uint8_t  HasFchModule[SIL_RESERVED_0355];
  uint32_t FchIohcBusNumber[SIL_RESERVED_0355];
  uint64_t FchAcpiMmioBase[SIL_RESERVED_0355];
} FCHMULTIFCH_OUTPUT_BLK;
