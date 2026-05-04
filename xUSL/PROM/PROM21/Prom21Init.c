/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromInit.c
 *  @brief Promontory21 related functions
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <SilSocLogicalId.h>
#include <PROM/PromClass-api.h>
#include <PROM/Common/PromInit.h>
#include "Prom21Init.h"


/**--------------------------------------------------------------------
 * InitializePromontoryChipset
 *
 * @brief This function initializes the PROM21 IP during timepoint 1 (pre-Pcie phase).
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The PROM Silicon block is initialized by calling this function
 *           in the IP block list.
 *
 * @param   SilContext     A context structure through which host firmware defined data
 *                         can be passed to openSIL. The host firmware is responsible
 *                         for initializing the SIL_CONTEXT structure.
 * @param   PromInputBlk   A pointer to the PROM configuration data.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS
InitializePromontoryChipset (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromInputBlk
  )
{
  SIL_STATUS                    Status;

  Status = SilPass;
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");


  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}
