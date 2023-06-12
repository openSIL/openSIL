/**
 * @file  xPrfCcx.h
 * @brief Platform Reference Firmware - Structures used by CCX in xPRF.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma pack (push, 1)

/// SRAT struct type
typedef enum {
  SIL_SRAT_LOCAL_APIC_TYPE = 0,       ///< 0 - SRAT Processor Local APIC/SAPIC Affinity Structure
  SIL_SRAT_MEMORY_TYPE,               ///< 1 - SRAT Memory Affinity Structure
  SIL_SRAT_LOCAL_X2_APIC_TYPE,        ///< 2 - SRAT Processor Local x2APIC Affinity Structure
  SIL_SRAT_GICC_TYPE,                 ///< 3 - SRAT GICC Affinity Structure
  SIL_SRAT_ASAS_TYPE,                 ///< 4 - SRAT Architecture Specific Affinity Structure
  SIL_SRAT_GIAS_TYPE,                 ///< 5 - SRAT Generic Initiator Affinity Structure
} SIL_SRAT_STRUCT_TYPE;

/// CRAT HSA Processing Unit Affinity Structure
typedef struct {
  uint32_t ProximityNode;              ///< Integer that represents the proximity node to
                                       ///<     which the node belongs to
  uint32_t ProcessorIdLow;             ///< Low value  of the logical processor included in this
                                       ///<     HSA proximity domain
  uint16_t NumCPUCores;                ///< Indicates count of CCompute execution units present
                                       ///<     in this APU node.
} SIL_CRAT_HSA_PROC_INFO;

/// CRAT Cache Affinity Structure
typedef struct {
  uint32_t ProcessorIdLow;         ///< Low value of a logical processor which includes this component
  uint8_t  LinesPerTag;            ///< Cache Lines per tag
  uint16_t CacheLatency;           ///< Cost of time to access cache described in nanoseconds.
} SIL_CRAT_CACHE;

#pragma pack (pop)

