/**
 * @file  FabricDfClkGatingInit.c
 * @brief OpenSIL DataFabric Clock Gating initialization.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "FabricDfClkGatingInit.h"
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>

/**
 *
 *  FabricClockGatingInit
 *
 *  Description:
 *    This function initializes the DF clock gating feature.
 *
 */
void FabricClockGatingInit(DFCLASS_INPUT_BLK* DfData)
{
  uint32_t            i;
  uint32_t            j;
  SIL_RESERVED_825    DfGlblClkGater;
  SIL_RESERVED_825    TmpDfGlblClkGater;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  for (i = 0; i < DfXGetNumberOfProcessorsPresent (); i++) {
    for (j = 0; j < DfGetNumberOfDiesOnSocket (); j++) {
      DfGlblClkGater.Value = DfXFabricRegisterAccRead (i,
        SIL_RESERVED_821, SIL_RESERVED_820, FABRIC_REG_ACC_BC);
      TmpDfGlblClkGater = DfGlblClkGater;
      if (TmpDfGlblClkGater.Value != DfGlblClkGater.Value) {
        DfXFabricRegisterAccWrite (i,
            SIL_RESERVED_821, SIL_RESERVED_820, FABRIC_REG_ACC_BC, DfGlblClkGater.Value);
      }
    }
  }
  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


