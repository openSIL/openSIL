/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfRcMgr.c
 * @brief Define xPrf Functions
 *        These Function returns Silicon Data of the OpenSIL Modules.
 *        These Silicon Data will send to HOST through service provided by OpenSIL.
 */

#include <SilCommon.h>
#include "xPRF.h"
#include <RcMgr/DfX/RcManager-api.h>

/*
 * xPrfGetResourceManagerTable
 *
 * @brief   This function is responsible for returning the desired resource manager table to the host firmware.
 *
 * @details It is the responsibility of the caller to:
 *          1. Specify the resource manager type defined in RcManager4-api.h.
 *          2. Cast the void * output to the appropriate resource manager table type defined in RcManager4-api.h.
 *
 * @param   SilContext      A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible for initializing
 *                          the SIL_CONTEXT structure.
 * @param   RcMgrTableType  The resource manager table type to be returned to the host firmware. Refer to
 *                          SIL_RC_MGR_TYPE for supported table types.
 *
 * @return  void *          A pointer to the requested resource manager table.
 *
 * @retval  NULL            If the resource manager table type is invalid or if the resource manager table is not found.
 */
void *
xPrfGetResourceManagerTable (
  SIL_CONTEXT   *SilContext,
  uint32_t      RcMgrTableType
  )
{
  void                     *RcMgrTable;
  DFX_RCMGR_INPUT_BLK      *RcMgrData;

  if (SilContext == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "SilContext in invalid!\n");
    return NULL;
  }

  RcMgrData = (DFX_RCMGR_INPUT_BLK *)SilFindStructure(SilContext, SilId_RcManager, RCMGRCLASS_INSTANCE);
  if (RcMgrData == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "SIL RC Init memory block was not found.\n");
    return NULL; // Could not find the IP input block
  }
  XPRF_TRACEPOINT(SIL_TRACE_INFO, "SIL RC Init memory block is found blk at: 0x%x \n", RcMgrData);

  switch (RcMgrTableType) {
  case IoMgrType:
    RcMgrTable = (void *)&RcMgrData->IoRcMgr;
    break;
  case MmioMgrType:
    RcMgrTable = (void *)&RcMgrData->MmioRcMgr;
    break;
  default:
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Invalid Resource Manager table type requested\n");
    RcMgrTable = NULL;
    break;
  }

  return RcMgrTable;
}
