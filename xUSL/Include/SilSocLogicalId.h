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
 * common variables
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
 * function prototypes
 *
 */
bool SocConvertRawToLogical (uint32_t RawId, SOC_LOGICAL_ID *LogicalSocId);

/**
 *  SilSocIdentificationCheck
 *
 *  @brief This function check SOC Family & Ext Model & Base Model & Stepping & Package Type identification ID
 *
 *  @param   SocFamilyID  - FamilyID
 *  @param   Attribute    - Attribute
 *                          If need to check the extended Model, please set FIXED_EXT_MODEL to Attribute;
 *                          If do not check the extended Model, please set ANY_EXT_MODEL to Attribute.
 *                          If need to check the base Model, please set FIXED_BASE_MODEL to Attribute;
 *                          If do not check the base Model, please set ANY_BASE_MODEL to Attribute.
 *                          If do not check the stepping, please set ANY_STEPPING to Attribute.
 *                          If do not check the package type, please set ANY_PKG_TYPE to Attribute;
 *                          If need to check the package type, please set package type to Attribute bit 0~3.
 *
 *  @return   true         - check pass.
 *  @return   false        - check fail.
 */
bool SilSocIdentificationCheck (uint32_t SocFamilyID, uint32_t Attributes);

SIL_STATUS GetSocLogicalIdOnCurrentCore (SOC_LOGICAL_ID *LogicalId);

SIL_STATUS GetCoreLogicalIdOnCurrentCore (CORE_LOGICAL_ID *CoreLogicalId);

bool SocFamilyIdCheck (uint32_t SocFamilyId);

bool
SocCheck (
  uint32_t SocFamilyID,
  uint32_t Attributes
  );

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
// Attributes for SilSocIdentificationCheck
#define FIXED_EXT_MODEL  0xFE000000 // If need to check the extended Model, please input this MACRO in attribute
#define ANY_EXT_MODEL    0xFF000000 // If do not check the extended Model, please input this MACRO in attribute
#define FIXED_BASE_MODEL 0x00FE0000 // If need to check the base Model, please input this MACRO in attribute
#define ANY_BASE_MODEL   0x00FF0000 // If do not check the base Model, please input this MACRO in attribute
#define ANY_STEPPING     0x0000FF00 // If do not check the stepping, please input this MACRO in attribute
#define ANY_PKG_TYPE     0x000000FF // If do not check the package type, please input this MACRO in attribute


/**********************************************************************************************************************
 * Macros
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
 * Family 19h SOC definitions
 */

// Family 19h PHX equates
#define AMD_FAMILY_19_PHX 0x00002000ul
#define AMD_FAMILY_PHX    (AMD_FAMILY_19_PHX)

// Family 19h PHX steppings
#define AMD_REV_F19_PHX_A0   0x0001u
#define AMD_REV_F19_PHX_A1   0x0002u
#define AMD_REV_F19_PHX2_A0  0x0004u
#define AMD_REV_F19_PHX_A2   0x0008u
#define AMD_REV_F19_PHX_B0   0x0010u
#define AMD_REV_F19_PHX2_B0  0x0040u
#define AMD_REV_F19_HPT_A2   0x0080u
#define AMD_REV_F19_HPT2_A0  0x0100u

#define AMD_REV_F19_PHX_AX   (AMD_REV_F19_PHX_A0 | AMD_REV_F19_PHX_A1 | AMD_REV_F19_PHX_A2)
#define AMD_REV_F19_PHX_BX   (AMD_REV_F19_PHX_B0)

#define AMD_REV_F19_PHX_ALL  (AMD_REV_F19_PHX_AX | AMD_REV_F19_PHX_BX)

#define AMD_REV_F19_PHX2_AX  (AMD_REV_F19_PHX2_A0)
#define AMD_REV_F19_PHX2_BX  (AMD_REV_F19_PHX2_B0)

#define AMD_REV_F19_PHX2_ALL (AMD_REV_F19_PHX2_AX | AMD_REV_F19_PHX2_BX)

#define AMD_REV_F19_HPT_AX   (AMD_REV_F19_HPT_A2)
#define AMD_REV_F19_HPT_ALL  (AMD_REV_F19_HPT_AX)
#define AMD_REV_F19_HPT2_AX  (AMD_REV_F19_HPT2_A0)
#define AMD_REV_F19_HPT2_ALL (AMD_REV_F19_HPT2_AX)
#define AMD_REV_F19_PHX_PHX2_HPT_HPT2_ALL (AMD_REV_F19_PHX_ALL | AMD_REV_F19_PHX2_ALL | AMD_REV_F19_HPT_ALL | \
        AMD_REV_F19_HPT2_ALL)


/// --------------------------------------------------------------------------
//   C O R E   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

/*
 * CORE_LOGICAL_ID.CoreFamily equates
 */
// Core Family 19h Zen4 equates
#define AMD_CORE_FAMILY_19_ZEN4  0x00000003ul

/*
 * CORE_LOGICAL_ID.LogicalCoreRev equates
 */
// Family 19h Zen4 core steppings
#define AMD_CORE_F19_ZEN4_A0   0x0100u
#define AMD_CORE_F19_ZEN4_A1   0x0200u
#define AMD_CORE_F19_ZEN4_A2   0x0400u
#define AMD_CORE_F19_ZEN4_B0   0x1000u
#define AMD_CORE_F19_ZEN4_B1   0x2000u

#define AMD_CORE_F19_ZEN4_AX   (AMD_CORE_F19_ZEN4_A0 | AMD_CORE_F19_ZEN4_A1 | AMD_CORE_F19_ZEN4_A2)

/// Family 19 package type
#define ZEN4_PKG_AM5                0

#define F19_PHX_RAW_ID         0x00A70F00ul
#define F19_PHX2_RAW_ID        0x00A70F80ul

// Check for Phoenix AM5, Phoenix2 AM5
#define ISSOCPHXAM5      (SocCheck(F19_PHX_RAW_ID, (FIXED_EXT_MODEL | ANY_BASE_MODEL | ANY_STEPPING | ZEN4_PKG_AM5)))
// Check for Phoenix2
#define ISSOCPHX2        (SocCheck(F19_PHX2_RAW_ID, (FIXED_EXT_MODEL | FIXED_BASE_MODEL | ANY_STEPPING | ANY_PKG_TYPE)))

#pragma pack (pop)
