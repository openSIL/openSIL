/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxIp2IpPhx.h
 * @brief Prototypes for PHX Zen4 core specific transfer table functions
 *
 */

#pragma once

#include <SilCommon.h>
#include "Pstates.h"

uint32_t
CalcLocalApicPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Complex,
  uint32_t     Core,
  uint32_t     Thread
  );

SIL_STATUS
Zen4GetPstateInfo (
  PSTATE            Pstate,
  uint32_t          *Frequency,
  uint32_t          *VoltageInuV,
  uint32_t          *PowerInmW,
  bool              *PstateStatus
  );

SIL_STATUS
Zen4ReOrderLogicalCcdWithNumaDomainOrder (
  uint32_t   LogicalSocket,
  uint32_t   LogicalDie,
  uint32_t   NumberOfCcds,
  uint32_t  *OrderedLogicalCcd
  );
