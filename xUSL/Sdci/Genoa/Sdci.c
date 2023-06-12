/**
 * @file Sdci.c
 * @brief Defines SDCI core initialization Entry Point
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Sdci/Common/Sdci.h>
#include <Sdci/Common/SdciCmn2Rev.h>
#include "Sdci.h"

extern SDCI_COMMON_2_REV_XFER_BLOCK SdciXfer;

/**
 * SetMSdciApi
 *
 * @brief   Initialize internal and external APIs for SDCI Genoa
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
SetSdciApi (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_SdciClass, &SdciXfer);
  if (Status != SilPass) {
    SDCI_TRACEPOINT (SIL_TRACE_INFO,"SDCI Genoa InitCommon2RevXferTable failed \n");
    return Status;
  }

  // There are no IP-toIP APIs for SDCI

  return Status;
}
