/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file Cxl.c
 * @brief Defines CXL core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/Common/CxlCmn2Rev.h>
#include "Cxl.h"

extern CXL_COMMON_2_REV_XFER_BLOCK CxlXfer;

/**
 * SetMCxlApi
 *
 * @brief   Initialize internal and external APIs for CXL Genoa
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
SetCxlApi (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_CxlClass, &CxlXfer);
  if (Status != SilPass) {
    CXL_TRACEPOINT (SIL_TRACE_INFO,"CXL Genoa InitCommon2RevXferTable failed \n");
    return Status;
  }

  // There are no IP-toIP APIs for CXL

  return Status;
}
