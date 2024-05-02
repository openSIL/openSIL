/**
 * @file  SilSocLogicalId.h
 * @brief OpenSIL SoC logical ID definitions
 */
/* Copyright 2021-2024 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <Sil-api.h>

#pragma pack (push, 1)


/**********************************************************************************************************************
 * Declare common variables here
 *
 */

/// SOC logical ID structure
typedef struct _SOC_LOGICAL_ID {
  uint32_t Family;             ///< Indicates logical ID Family
  uint16_t Revision;           ///< Indicates logical ID Revision
} SOC_LOGICAL_ID;

/// Core logical ID structure
typedef struct _CORE_LOGICAL_ID {
  uint32_t CoreFamily;         ///< Indicates logical ID Core (silicon) Family
  uint16_t CoreRevision;       ///< Indicates logical ID Core (silicon) Revision
} CORE_LOGICAL_ID;

/**
 * Declare function prototypes here
 *
 */
bool SocConvertRawToLogical (uint32_t RawId, SOC_LOGICAL_ID *LogicalSocId);

SIL_STATUS GetSocLogicalIdOnCurrentCore (SOC_LOGICAL_ID *LogicalId);

SIL_STATUS GetCoreLogicalIdOnCurrentCore (CORE_LOGICAL_ID *CoreLogicalId);

bool SocFamilyIdCheck (uint32_t SocFamilyId);

/**********************************************************************************************************************
 * Declare macros here
 *
 */

// UNKNOWN equates
// -------------------------------------
// This define should be equal to the total number of families
// in the cpuFamily enum.
#define MAX_CPU_FAMILIES    31
#define MAX_CPU_REVISIONS   15    // Max Cpu Revisions Per Family
// Family Unknown
#define AMD_FAMILY_UNKNOWN   0x80000000ul
#define AMD_REVISION_UNKNOWN 0x8000u

// --------------------------------------------------------------------------
//   S O C   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

// SOC_LOGICAL_ID.Family equates
// -------------------------------------

// Family 19h GENOA equates
#define AMD_FAMILY_19_10         0x00001000ul
#define AMD_FAMILY_GENOA         (AMD_FAMILY_19_10)
#define AMD_F19_GENOA_RAW_ID     0x00AA0F00ul

  // Family 19h GENOA steppings
#define AMD_REV_F19_GENOA_A0    0x0001u
#define AMD_REV_F19_GENOA_A1    0x0002u
#define AMD_REV_F19_GENOA_B0    0x0010u
#define AMD_REV_F19_GENOA_B1    0x0020u

#define AMD_REV_F19_GENOA_AX    (AMD_REV_F19_GENOA_A0 | AMD_REV_F19_GENOA_A1)
#define AMD_REV_F19_GENOA_BX    (AMD_REV_F19_GENOA_B0 | AMD_REV_F19_GENOA_B1)
#define AMD_REV_F19_GENOA_ALL   (AMD_REV_F19_GENOA_AX | AMD_REV_F19_GENOA_BX)

// --------------------------------------------------------------------------
//   C O R E   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

// -------------------------------------
// CORE_LOGICAL_ID.CoreFamily equates
// -------------------------------------
  // Core Family 19h Zen4 equates
#define AMD_CORE_FAMILY_19_ZEN4  0x00000003ul
#define AMD_CORE_FAMILY_ZEN4     (AMD_CORE_FAMILY_19_ZEN4)

// -------------------------------------
// CORE_LOGICAL_ID.LogicalCoreRev equates
// -------------------------------------
  // Family 19h Zen4 core steppings
#define AMD_CORE_F19_ZEN4_A0   0x0100u
#define AMD_CORE_F19_ZEN4_A1   0x0200u
#define AMD_CORE_F19_ZEN4_B0   0x1000u
#define AMD_CORE_F19_ZEN4_B1   0x2000u

#define AMD_CORE_F19_ZEN4_AX   (AMD_CORE_F19_ZEN4_A0 | AMD_CORE_F19_ZEN4_A1)

#define AMD_CORE_F19_ZEN4_ALL  (AMD_CORE_F19_ZEN4_AX)

  // All
#define AMD_CORE_F19_ALL      (AMD_CORE_F19_ZEN4_ALL)

#pragma pack (pop)

