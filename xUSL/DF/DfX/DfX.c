/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfX.c
 * @brief Input block setting for DfX
 *
 */


#include <SilCommon.h>
#include <DF/Df.h>
#include "DfX.h"
#include <string.h>

extern const DFCLASS_INPUT_BLK  mDfClassDflts;    // module defaults

/**
 * Establish DF input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
DfXSetInputBlk (
  SIL_CONTEXT  *SilContext
  )
{
  DFCLASS_INPUT_BLK *DfInput;

  DfInput = (DFCLASS_INPUT_BLK *)SilCreateInfoBlock(SilContext,
    SilId_DfClass,
    sizeof (DFCLASS_INPUT_BLK),
    DFCLASS_INSTANCE,
    DFCLASS_MAJOR_REV,
    DFCLASS_MINOR_REV
    );
  DF_TRACEPOINT(SIL_TRACE_INFO, "SIL DfXSetInputBlk at: 0x%x \n", DfInput);
  if (DfInput == NULL) {
    return SilAborted;
  }
  // fill DF structure with defaults
  memcpy((void *)DfInput, &mDfClassDflts, sizeof (DFCLASS_INPUT_BLK));
  // fill SILcommon with platform info ( #sockets #RB/Skt, #RB, CPI-ID, #cores, etc....)

  return SilPass;
}
