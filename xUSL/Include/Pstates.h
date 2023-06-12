/**
 * @file  Pstates.h
 * @brief OpenSIL Pstate register definitions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include "stdint.h"

/**********************************************************************************************************************
 * Declare macros here
 *
 */

/**********************************************************************************************************************
 * Module data structure
 *
 */

/**
 * @details This enum defines the possible Pstate values/register [0:7].
 *
 */
typedef enum {
  Pstate0 = 0,       ///< Software P0
  Pstate1,           ///< Software P1
  Pstate2,           ///< Software P2
  Pstate3,           ///< Software P3
  Pstate4,           ///< Software P4
  Pstate5,           ///< Software P5
  Pstate6,           ///< Software P6
  Pstate7,           ///< Software P7
} PSTATE;

/**
 * @brief Switch Core Pstate.  This structure contains the target pstate as well as a flag the determine whether fw
 *        should wait for pstate change to complete.
 *
 */
typedef struct {
  PSTATE    Pstate;             ///< Target Pstate
  bool      WaitForCompletion;  ///< Flag to indicate if code should wait for Pstate
                                ///< transition to complete or immediately return
} SWITCH_PSTATE;

/***********************************************************************************************************************/

SIL_STATUS
GetPstateInfo (
  PSTATE            Pstate,
  uint32_t          *Frequency,
  uint32_t          *VoltageInuV,
  uint32_t          *PowerInmW,
  bool              *PstateStatus
  );

void
GetPstateNumber (
  uint32_t             *NumberOfBoostPstates,
  uint32_t             *NumberOfPstates
  );

SIL_STATUS
SwitchPstate (
  SWITCH_PSTATE     *SwitchPstate
  );

uint8_t
GetCurrentPstate (void);


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
  );
