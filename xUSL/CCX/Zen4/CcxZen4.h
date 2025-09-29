/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxZen4.h
 * @brief Prototypes for Zen4 core specific transfer table functions
 *
 */

#pragma once
#include <CCX/CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <Pstates.h>

/**********************************************************************************************************************
 * Function Prototypes
 *
 */

void
Zen4SetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

void
Zen4CcxPrefetcher (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

SIL_STATUS
Zen4SwitchPstate (
  SWITCH_PSTATE *SwitchPstate
  );

volatile AP_MSR_SYNC *
Zen4GetApMsrSyncList (void);

void
Zen4InitializePrefetchMode (
  CCXCLASS_INPUT_BLK *CcxInputBlock
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

void
Zen4CcxInitializeC6 (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );
