/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlBrh.c
 * @brief Defines CXL Turin core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/Common/CxlCmn2Rev.h>
#include <Cxl/CxlIp2Ip.h>
#include "CxlBrh.h"

extern CXL_COMMON_2_REV_XFER_BLOCK CxlXferBrh;
extern CXL_IP2IP_API CxlBrhApi;

/**
 * SetCxlApiBrh
 *
 * @brief   Initialize internal and external APIs for CXL RS
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
SetCxlApiBrh (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_CxlClass, &CxlXferBrh);
  if (Status != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "CXL BRH InitCommon2RevXferTable failed \n");
    return Status;
  }

  // Initialize CXL IP-to-IP API
  return SilInitIp2IpApi(SilId_CxlClass, (void *)&CxlBrhApi);
}

/**--------------------------------------------------------------------
 * InitializeBrhCxlTp1
 *
 * @brief This program specific function calls the common code function that initializes the CXL IP during timepoint 1 (pre-Pcie phase) .
 *
 * @details  This acts as a wrapper function to call common code from program specific code.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS
InitializeBrhCxlTp1 (void)
{
  SIL_STATUS  Status;
  Status = InitializeCxlTp1 ();
  return Status;
}
