/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfCsInitPhx.c
 * @brief Data fabric CS devices initialization.
 *
 */

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/DfCmn2Rev.h>
#include "DfSilFabricInfoPhx.h"
#include "DfPhx.h"

/**
 * DfPhxCsInit
 *
 * @brief This function initializes the DF CS devices.
 *
 * @param   SilContext         A context structure through which host firmware defined data
 *                             can be passed to openSIL. The host firmware is responsible
 *                             for initializing the SIL_CONTEXT structure.
 *
 */
void
DfPhxCsInit (
  SIL_CONTEXT *SilContext
  )
{
  uint32_t    CsInstance;
  uint32_t    Value;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  for (CsInstance = PHX_CS0_INSTANCE_ID; CsInstance < (PHX_CS0_INSTANCE_ID + PHX_NUM_CS_BLOCKS);
    CsInstance++) {

    Value = DfXFabricRegisterAccRead(SilContext, 0, 0, 0x2, 0x48, CsInstance);
    DfXFabricRegisterAccWrite(SilContext, 0, 0, 0x2, 0x48, CsInstance, Value);
  }

  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
