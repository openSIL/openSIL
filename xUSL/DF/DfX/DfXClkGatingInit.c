/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXClkGatingInit.c
 * @brief Data fabric clock gating initialization for DfX
 *
 */

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/DfCmn2Rev.h>
#include "SilFabricRegistersDfX.h"

/**
 *
 *  DfXClockGatingInit
 *
 *  Description:
 *    This function initializes the DF clock gating feature.
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 *  @param  DfData      Input pointer to the Data Fabric input block.
 *
 */
void
DfXClockGatingInit (
  SIL_CONTEXT       *SilContext,
  DFCLASS_INPUT_BLK *DfData
  )
{
  uint32_t                    i;
  uint32_t                    j;
  uint32_t                    k;
  SIL_RESERVED_UNION_0008  DfGlblClkGater;
  SIL_RESERVED_UNION_0008  TmpDfGlblClkGater;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  for (i = 0; i < DfXfer->DfGetNumberOfProcessorsPresent(SilContext); i++) {
    for (j = 0; j < DfGetNumberOfDiesOnSocket(SilContext, i); j++) {
      for (k = 0; k < 0x1B; k++) {
        DfGlblClkGater.Value = DfXFabricRegisterAccRead(SilContext,
          i,
          j,
          DFXFABFUNCx5,
          DFXFABFUNC5x104,
          k
          );
        TmpDfGlblClkGater = DfGlblClkGater;
        if (TmpDfGlblClkGater.Value != DfGlblClkGater.Value) {
          DfXFabricRegisterAccWrite(SilContext,
            i,
            j,
            DFXFABFUNCx5,
            DFXFABFUNC5x104,
            k,
            DfGlblClkGater.Value
            );
        }
      }
    }
  }
  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
