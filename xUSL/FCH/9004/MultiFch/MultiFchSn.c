
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchSn.c
 * @brief OpenSIL Multi FCH specific initialization
 *
 */

#include <SilCommon.h>
#include <xSIM-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FCHClass-api.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include <FCH/Common/MultiFch/MultiFchInit.h>
#include "MultiFchCmn2Sn.h"
#include "MultiFchSn.h"

extern MULTI_FCH_COMMON_2_REV_XFER_BLOCK mMultiFchCmn2Sn;

/*
 * InitializeMultiFchSnTp1
 *
 * @brief Config FCH during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeMultiFchSnTp1 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/*
 * InitializeMultiFchApiSn
 *
 * @brief   Initialize transfer tables for Multi FCH class
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeMultiFchApiSn (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_MultiFchClass, (void *)&mMultiFchCmn2Sn);

  return Status;
}
