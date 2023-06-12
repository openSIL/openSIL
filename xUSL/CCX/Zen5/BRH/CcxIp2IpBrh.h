/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxIp2IpBrh.h
 * @brief Prototypes for BRH Zen5 core specific I2I API table functions
 *
 */

#pragma once

#include <SilCommon.h>
#include "Pstates.h"

uint32_t
CalcLocalApicBrh (
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    Ccd,
  uint32_t    Complex,
  uint32_t    Core,
  uint32_t    Thread
  );

SIL_STATUS
Zen5GetPstateInfo (
  PSTATE                Pstate,
  uint32_t              *Frequency,
  uint32_t              *VoltageInuV,
  uint32_t              *PowerInmW,
  bool                  *PstateStatus
  );

SIL_STATUS
Zen5ReOrderLogicalCcdWithNumaDomainOrder (
  uint32_t   LogicalSocket,
  uint32_t   LogicalDie,
  uint32_t   NumberOfCcds,
  uint32_t  *OrderedLogicalCcd
  );
