/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  SilSocLogicalId.h
 * @brief OpenSIL SoC logical ID definitions
 *
 */

#pragma once
#pragma pack (push, 1)

#include <SilCommon.h>

/**********************************************************************************************************************
 * Declare common variables here
 *
 */

/// SOC logical ID structure
typedef struct {
  uint32_t Family;             ///< Indicates logical ID Family
  uint16_t Revision;           ///< Indicates logical ID Revision
} SOC_LOGICAL_ID;

/// Core logical ID structure
typedef struct {
  uint32_t CoreFamily;         ///< Indicates logical ID Core (silicon) Family
  uint16_t CoreRevision;       ///< Indicates logical ID Core (silicon) Revision
} CORE_LOGICAL_ID;

/**
 * Declare function prototypes here
 *
 */
bool SocConvertRawToLogical (uint32_t RawId, SOC_LOGICAL_ID *LogicalSocId);

/**
 *
 *  This function check SOC Family & Ext Model & Base Model & Stepping & Package Type identification ID
 *
 *     @param   SocFamilyID  - FamilyID
 *     @param   Attribute    - Attribute
 *                             If need to check the extended Model, please set FIXED_EXT_MODEL to Attribute;
 *                             If do not check the extended Model, please set ANY_EXT_MODEL to Attribute.
 *                             If need to check the base Model, please set FIXED_BASE_MODEL to Attribute;
 *                             If do not check the base Model, please set ANY_BASE_MODEL to Attribute.
 *                             If do not check the stepping, please set ANY_STEPPING to Attribute.
 *                             If do not check the package type, please set ANY_PKG_TYPE to Attribute;
 *                             If need to check the package type, please set package type to Attribute bit 0~3.
 *
 *     @return   true         - check pass.
 *     @return   false        - check fail.
 */
bool SilSocIdentificationCheck (uint32_t SocFamilyID, uint32_t Attributes);

SIL_STATUS GetSocLogicalIdOnCurrentCore (SOC_LOGICAL_ID *LogicalId);

SIL_STATUS GetCoreLogicalIdOnCurrentCore (CORE_LOGICAL_ID *CoreLogicalId);

bool SocFamilyIdCheck (uint32_t SocFamilyId);

/// Family 1A CPUID
#define F1A_BRH_B0_RAW_ID           0x00B00F10ul

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
// Attributes for SilSocIdentificationCheck
#define FIXED_EXT_MODEL  0xFE000000 // If need to check the extended Model, please input this MARCO in attribute
#define ANY_EXT_MODEL    0xFF000000 // If do not check the extended Model, please input this MARCO in attribute
#define FIXED_BASE_MODEL 0x00FE0000 // If need to check the base Model, please input this MARCO in attribute
#define ANY_BASE_MODEL   0x00FF0000 // If do not check the base Model, please input this MARCO in attribute
#define ANY_STEPPING     0x0000FF00 // If do not check the stepping, please input this MARCO in attribute
#define ANY_PKG_TYPE     0x000000FF // If do not check the package type, please input this MARCO in attribute

//----------------------------------------------------------------------------------------
// Breithorn
//----------------------------------------------------------------------------------------
// Check if it is Breithorn Bx
#define IS_SOC_BRH_BX    (SilSocIdentificationCheck ( \
                            F1A_BRH_B0_RAW_ID, \
                            (FIXED_EXT_MODEL|FIXED_BASE_MODEL|ANY_STEPPING|ANY_PKG_TYPE) \
                            ))

/**********************************************************************************************************************
 * Declare macros here
 *
 */

// UNKNOWN equates
// -------------------------------------
// Family Unknown
#define AMD_FAMILY_UNKNOWN   0x80000000ul
#define AMD_REVISION_UNKNOWN 0x8000u

// --------------------------------------------------------------------------
//   S O C   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

// SOC_LOGICAL_ID.Family equates
// -------------------------------------

/*
 * Family 1Ah SOC definitions
 */
// Family 1Ah BRH equates
#define AMD_FAMILY_1A_BRH 0x00080000ul
#define AMD_FAMILY_BRH    (AMD_FAMILY_1A_BRH)

#define AMD_FAMILY_1A          (AMD_FAMILY_BRH)

// Family 1Ah BRH steppings
#define AMD_REV_F1A_BRH_B0   0x0004u
#define AMD_REV_F1A_BRH_B1   0x0008u
#define AMD_REV_F1A_BRH_C0   0x0010u
#define AMD_REV_F1A_BRH_C1   0x0020u

#define AMD_REV_F1A_BRH_BX   (AMD_REV_F1A_BRH_B0 | AMD_REV_F1A_BRH_B1)
#define AMD_REV_F1A_BRH_CX   (AMD_REV_F1A_BRH_C0 | AMD_REV_F1A_BRH_C1)
#define AMD_REV_F1A_BRH_ALL  (AMD_REV_F1A_BRH_BX | AMD_REV_F1A_BRH_CX)

// --------------------------------------------------------------------------
//   C O R E   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

/*
 * CORE_LOGICAL_ID.CoreFamily equates
 */
// Core Family 1Ah Zen5 equates
#define AMD_CORE_FAMILY_1A_ZEN5  0x00000004ul

/*
 * CORE_LOGICAL_ID.LogicalCoreRev equates
 */
// Family 1Ah Zen5 core steppings
#define AMD_CORE_F1A_ZEN5_B0   0x0010u
#define AMD_CORE_F1A_ZEN5_B1   0x0020u
#define AMD_CORE_F1A_ZEN5_C0   0x0100u
#define AMD_CORE_F1A_ZEN5_C1   0x0200u


#pragma pack (pop)
