/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchKl.c
 * @brief Kunlun FCH multi-die functions
 *
 */

#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include "MultiFchKl.h"

extern SIL_RESERVED_STRUCT_0026 mMultiFchXferKl;

/**
 * InitializeMultiFchKlTp1
 *
 * @brief Config Kunlun FCH multi-die during timepoint 1 (pre-pcie)
 *
 */
SIL_STATUS
InitializeMultiFchKlTp1 (void)
{
  uint32_t            RbIndex;
  SIL_STATUS          Status;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = FchMultiFchGetFchRbIndexOnDieCmn(&RbIndex);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "FCH RB index on IOD is 0x%x.\n", RbIndex);

  Status = FchMultiFchInitAcpiMmioAllocation ();
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  Status = FchMultiFchDispatchSecondaryInits ();
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * InitializeMultiFchKlTp2
 *
 * @brief Config Kunlun FCH multi-die during timepoint 2
 *
 */
SIL_STATUS
InitializeMultiFchKlTp2 (void)
{
  return SilPass;
}

/**
 * InitializeMultiFchKlTp3
 *
 * @brief Config Kunlun FCH multi-die during timepoint 3
 *
 */
SIL_STATUS
InitializeMultiFchKlTp3 (void)
{
  return SilPass;
}

/**
 * MultiFchSetInputBlkKl
 *
 * @brief Establish FCH multi-die input defaults for Kunlun
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SilPass
 */
SIL_STATUS
MultiFchSetInputBlkKl (void)
{
  SIL_STATUS          Status;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = FchMultiFchSetInputBlk ();

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * InitializeMultiFchApiKl
 *
 * @brief   Initialize transfer tables for Kunlun FCH multi-die class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeMultiFchApiKl (void)
{
  SIL_STATUS  Status;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Initialize Common to Rev specific transfer table
  Status = SilInitCommon2RevXferTable(SilId_MultiFchClass, (void *)&mMultiFchXferKl);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}
