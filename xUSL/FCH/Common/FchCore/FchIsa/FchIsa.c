/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsa.c
 * @brief FCH ISA sub-controller functions
 *
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FchIsa-api.h>
#include <FchClass-api.h>
#include <FCH/Common/Fch.h>
#include "FchIsa.h"

extern const FCHISA_INPUT_BLK FchIsaDefaults;

/**
 * FchIsaSetInputBlk
 * @brief Establish FCH ISA input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval SIL_STATUS
 */
SIL_STATUS FchIsaSetInputBlk (
  SIL_CONTEXT  *SilContext
  )
{
  FCHISA_INPUT_BLK *FchIsaInput;

  FchIsaInput = (FCHISA_INPUT_BLK *) SilCreateInfoBlock(SilContext,
    SilId_FchIsa,
    sizeof (FCHISA_INPUT_BLK),
    FCHISA_INPUT_BLK_INSTANCE,
    FCHISA_MAJOR_REV,
    FCHISA_MINOR_REV
    );

  if (FchIsaInput == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL Fch ISA input block was not found.\n");
    return SilAborted;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA input block is at: 0x%x \n", FchIsaInput);

  // Fill Fch ISA structure with defaults
  memcpy((void *)FchIsaInput, &FchIsaDefaults, sizeof (FCHISA_INPUT_BLK));

  return SilPass;
}
