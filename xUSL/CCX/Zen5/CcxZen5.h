/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxZen5.h
 * @brief Prototypes for Zen5 core specific transfer table functions
 *
 */

#pragma once
#include <CCX/CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <Pstates.h>

///  The possible PAUSE instruction mode settings.
typedef enum {
  CPU_PAUSECNTSEL_1_0_16CYCLES,   ///< Thread will be idle for 16 cycles after a PAUSE instruction.
  CPU_PAUSECNTSEL_1_0_32CYCLES,   ///< Thread will be idle for 32 cycles after a PAUSE instruction.
  CPU_PAUSECNTSEL_1_0_64CYCLES,   ///< Thread will be idle for 64 cycles after a PAUSE instruction.
  CPU_PAUSECNTSEL_1_0_128CYCLES,  ///< Thread will be idle for 128 cycles after a PAUSE instruction.
  CPU_PAUSECNTSEL_1_0_AUTO = 0xFF ///< Use the recommended setting for the processor.
} CPU_PAUSECNTSEL_1_0;

typedef struct {
  uint32_t LogicalId;
  uint32_t PhysicalId;
} CCD_MAP;

/**********************************************************************************************************************
 * Declare Function prototypes
 *
 */

void
Zen5SetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

void
Zen5CcxPrefetcher (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

volatile AP_MSR_SYNC *
Zen5GetApMsrSyncList (void);

void
Zen5InitializePrefetchMode (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

SIL_STATUS
Zen5SwitchPstate (
  SWITCH_PSTATE *SwitchPstate
  );

SIL_STATUS
Zen5GetPstateInfo (
  PSTATE    Pstate,
  uint32_t  *Frequency,
  uint32_t  *VoltageInuV,
  uint32_t  *PowerInmW,
  bool      *PstateStatus
  );

SIL_STATUS
Zen5ReOrderLogicalCcdWithNumaDomainOrder (
  uint32_t   LogicalSocket,
  uint32_t   LogicalDie,
  uint32_t   NumberOfCcds,
  uint32_t  *OrderedLogicalCcd
  );

void
Zen5StoreDowncoreCoreCount (
  CCXCLASS_DATA_BLK *CcxConfigData
  );

void
Zen5CcxInitializeC6 (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );
