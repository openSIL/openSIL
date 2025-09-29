/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CoreTopologyService.h
 * @brief CoreTechnology service definition.
 *
 */

#pragma once

#include <APOB/Common/ApobCmn.h>

#define CORE_TOPOLOGY_V3_ITERATION_START   ((uint32_t)-1)
typedef enum {
  CORE_TOPOLOGY_V3_NEXT_SOCKET = 0,
  CORE_TOPOLOGY_V3_NEXT_DIE = 1,
  CORE_TOPOLOGY_V3_NEXT_CCD = 2,
  CORE_TOPOLOGY_V3_NEXT_COMPLEX = 3,
  CORE_TOPOLOGY_V3_NEXT_CORE = 4,
  CORE_TOPOLOGY_V3_NEXT_THREAD = 5,
  CORE_TOPOLOGY_V3_NO_NEXT_ITEM = 6,
} CORE_TOPOLOGY_ITERATION_RESULT;

SIL_STATUS
GetCcdCountOnDie (
  SIL_CONTEXT                                   *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      Socket,
  uint32_t                                      Die,
  uint32_t                                      *NumberOfCcds
  );

SIL_STATUS
GetComplexCountOnCcd (
  SIL_CONTEXT                                  *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     *NumberOfComplexes
  );

SIL_STATUS
GetCoreCountOnComplex (
  SIL_CONTEXT                                  *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     *NumberOfCores
  );

SIL_STATUS
GetThreadCountOnCore (
  SIL_CONTEXT                                  *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     Core,
  uint32_t                                     *NumberOfThreads
  );

SIL_STATUS
LogicalToPhysicalLocation (
  SIL_CONTEXT                                  *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     *Socket,
  uint32_t                                     *Die,
  uint32_t                                     *Ccd,
  uint32_t                                     *Complex,
  uint32_t                                     *Core
  );

SIL_STATUS
GetCoreTopologyOnDieMax (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     *MaxNumberOfCcds,
  uint32_t     *MaxNumberOfComplexes,
  uint32_t     *MaxNumberOfCores,
  uint32_t     *MaxNumberOfThreads
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateSocket (
  SIL_CONTEXT  *SilContext,
  uint32_t     *Socket
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateDie (
  SIL_CONTEXT                     *SilContext,
  uint32_t                        *Socket,
  uint32_t                        *Die
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCcd (
  SIL_CONTEXT                                   *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateComplex (
  SIL_CONTEXT                                   *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCore (
  SIL_CONTEXT                                   *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateThread (
  SIL_CONTEXT                                   *SilContext,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core,
  uint32_t                                      *Thread
  );
