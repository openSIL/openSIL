/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
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
 */
void DfXClockGatingInit (DFCLASS_INPUT_BLK *DfData)
{
  uint32_t                    i;
  uint32_t                    j;
  SIL_RESERVED_UNION_0011  DfGlblClkGater;
  SIL_RESERVED_UNION_0011  TmpDfGlblClkGater;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  for (i = 0; i < DfXfer->DfGetNumberOfProcessorsPresent (); i++) {
    for (j = 0; j < DfGetNumberOfDiesOnSocket(i); j++) {
      DfGlblClkGater.Value = DfXFabricRegisterAccRead(i,
        j,
        SIL_RESERVED_0271,
        SIL_RESERVED_0272,
        FABRIC_REG_ACC_BC
        );
      TmpDfGlblClkGater = DfGlblClkGater;
      if (DfData->AmdPerformanceTracing) {
        DF_TRACEPOINT(SIL_TRACE_INFO, "Performance Tracing disable medium grain clock gating\n");
        DfGlblClkGater.Field.field_bits_0_to_3 = 0;
      }
      if (TmpDfGlblClkGater.Value != DfGlblClkGater.Value) {
        DfXFabricRegisterAccWrite(i,
          j,
          SIL_RESERVED_0271,
          SIL_RESERVED_0272,
          FABRIC_REG_ACC_BC,
          DfGlblClkGater.Value
          );
      }
    }
  }
  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
