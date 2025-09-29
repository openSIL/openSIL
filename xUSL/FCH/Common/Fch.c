/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Fch.c
 * @brief Initializes Fch Reset, Env and Late stages of FCH
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FCH/FchClass-api.h>
#include "Fch.h"

extern FCHCLASS_INPUT_BLK InitEnvCfgDefault;

/**
 * FchClassSetInputBlk
 * @brief Establish FCH Parent input defaults
 *
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS FchClassSetInputBlk (
  SIL_CONTEXT  *SilContext
  )
{
  FCHCLASS_INPUT_BLK *FchInput;

  FchInput = (FCHCLASS_INPUT_BLK *)SilCreateInfoBlock(SilContext,
    SilId_FchClass,
    sizeof (FCHCLASS_INPUT_BLK),
    FCHCLASS_INSTANCE,
    FCHCLASS_MAJOR_REV,
    FCHCLASS_MINOR_REV
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchClassSetInputBlk at: 0x%x \n", FchInput);
  if (FchInput == NULL) {
    return SilAborted;
  }

  // fill FCH structure with defaults
  memcpy((void *)FchInput, &InitEnvCfgDefault, sizeof (FCHCLASS_INPUT_BLK));

  return SilPass;
}
