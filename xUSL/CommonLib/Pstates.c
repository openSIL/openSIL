/* SPDX-License-Identifier: MIT */
/**
 * @file  Pstates.c
 * @brief OpenSIL Pstates Library
 *
 */
/*
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <SilCommon.h>
#include "Pstates.h"
#include <CommonLib/CpuLib.h>
#include <MsrReg.h>

/**--------------------------------------------------------------------
 * GetPstateNumber
 *
 * @brief   Get number of Pstates
 *
 * @details Returns the maximum number of Pstates in NumberOfPstate
 *
 * @param   NumberOfBoostPstate Output number of boost Pstate
 * @param   NumberOfPstate      Output number of Pstates
 *
 * @retval  None
 */
void
GetPstateNumber (
  uint32_t             *NumberOfBoostPstates,
  uint32_t             *NumberOfPstates
  )
{
  PSTATE_CURLIM_STRUCT  CurLim;

  *NumberOfBoostPstates = 0;

  CurLim.Value = xUslRdMsr(MSR_PSTATE_CURLIM_ADDRESS);
  *NumberOfPstates = (uint32_t) CurLim.Field.PstateMaxVal;
}

/**--------------------------------------------------------------------
 * GetCurrentPstate
 *
 * @brief   Get current Pstate
 *
 * @details Return the value of the current pstate CurPstate.
 *
 * @retval  uint8_t  current Pstate
 */
uint8_t
GetCurrentPstate (void)
{
  PSTATE_STATUS_STRUCT PstateSts;

  PstateSts.Value = xUslRdMsr(MSR_PSTATE_STATUS_ADDRESS);
  return (uint8_t) PstateSts.Field.CurPstate;
}

/**--------------------------------------------------------------------
 * GetPstateTransLatency
 *
 * @brief   call to get Pstate Transition Latency
 *
 * @param[out] TransitionLatency  The transition latency.
 *
 * @retval  SIL_STATUS  Always succeeds.
 */
SIL_STATUS
GetPstateTransLatency (
  uint32_t   *TransitionLatency
  )
{
  *TransitionLatency = 0;

  return (SilPass);
}
