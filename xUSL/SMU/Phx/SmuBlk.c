/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  SmuBlk.c
 * @brief Input block setting for SMU
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/SmuClass-api.h>
#include "SmuPhx.h"
#include <string.h>

extern const SMUCLASS_INPUT_BLK  mSmuClassDflts;

/**
 * Establish SMU input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
SmuSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
)
{
  SMUCLASS_INPUT_BLK *SmuInput;

  SmuInput = (SMUCLASS_INPUT_BLK *)SilCreateInfoBlock(SilContext,
    SilId_SmuClass,
    sizeof (SMUCLASS_INPUT_BLK),
    SMUCLASS_INSTANCE,
    SMUCLASS_MAJOR_REV,
    SMUCLASS_MINOR_REV
    );

  SMU_TRACEPOINT(SIL_TRACE_INFO, "SIL SmuSetInputBlk at: 0x%x \n", SmuInput);
  if (SmuInput == NULL) {
    return SilAborted;
  }
  // fill SMU structure with defaults
  memcpy((void *)SmuInput, &mSmuClassDflts, sizeof (SMUCLASS_INPUT_BLK));

  return SilPass;
}
