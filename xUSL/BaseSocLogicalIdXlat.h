/**
 * @file  BaseSocLogicalIdXlat.h
 * @brief Various families of SoC xlat tables prototype
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "SilSocLogicalId.h"
#include "CommonLib/CpuLib.h"

#pragma once

/**********************************************************************************************************************
 * variable declaration
 *
 */

/// Translation table entry for revision determination
typedef struct {
  uint8_t  RawBaseModel;             ///< CPUID Fn0000_0001_EAX[BaseModel]
  uint8_t  RawStepping;              ///< CPUID Fn0000_0001_EAX[Stepping]
  uint16_t LogicalRevision;          ///< Logical revision
  uint16_t LogicalCoreRev;           ///< Logical Core revision
} SOC_LOGICAL_REVISION_XLAT;

/// Translation table entry for family determination
typedef struct {
  uint16_t RawFamily;                ///< CPUID Fn0000_0001_EAX[BaseFamily] + [ExtFamily]
  uint8_t  RawExtModel;              ///< CPUID Fn0000_0001_EAX[ExtModel]
  uint32_t LogicalFamily;            ///< Logical family
  uint32_t LogicalCoreFamily;        ///< Logical Core family
  const SOC_LOGICAL_REVISION_XLAT *LogicalRevisionTable;  ///< Pointer to CPU_LOGICAL_REVISION_XLAT table
} SOC_LOGICAL_FAMILY_XLAT;

/**********************************************************************************************************************
 * Function prototypes
 *
 */

bool
SocFindApplicableLogicalTableEntries (
  uint32_t                   RawId,
  SOC_LOGICAL_FAMILY_XLAT    **Family,
  SOC_LOGICAL_REVISION_XLAT  **Revision
  );

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define SOC_LOGICAL_FAMILY_XLAT_TERMINATOR   {0, 0, AMD_FAMILY_UNKNOWN, AMD_FAMILY_UNKNOWN, NULL}
#define SOC_LOGICAL_REVISION_XLAT_TERMINATOR {0, 0, AMD_REVISION_UNKNOWN}
