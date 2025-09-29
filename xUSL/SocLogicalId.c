/* SPDX-License-Identifier: MIT */
/**
 * @file  SocLogicalId.c
 * @brief OpenSIL SoC logical ID functions
 *
 */

/*
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <SilCommon.h>
#include <BaseSocLogicalIdXlat.h>

/**
 * SocConvertRawToLogical
 * @brief Translates the given raw ID into the appropriate logical
 *        family / logical revision
 *
 * @param[in]     RawId       Raw CPU ID to convert
 * @param[out]    LogicalId   Logical family and logical revision for the given raw ID
 *
 * @retval        true        Logical ID was successfully found
 * @retval        false       Family is unknown
 */
bool
SocConvertRawToLogical (
  uint32_t          RawId,
  SOC_LOGICAL_ID    *LogicalId
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable;
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable;
  bool                       LogicalIdValid;

  LogicalIdValid = SocFindApplicableLogicalTableEntries(RawId,
    &LogicalFamilyTable,
    &LogicalRevisionTable
    );
  if (LogicalIdValid) {
    LogicalId->Family = LogicalFamilyTable->LogicalFamily;
    LogicalId->Revision = LogicalRevisionTable->LogicalRevision;
  }

  return LogicalIdValid;
}

/**
 * BaseCoreConvertRawToLogical
 * @brief Translates the given raw ID into the appropriate
 * logical family / logical revision
 *
 * @param[in]     RawId       Raw CPU ID to convert
 * @param[out]    LogicalId   Logical family and logical revision for
 *                            the given raw ID
 *
 * @retval        true        Logical ID was successfully found
 * @retval        false       Family is unknown
 */
static bool
BaseCoreConvertRawToLogical (
  uint32_t          RawId,
  CORE_LOGICAL_ID   *LogicalId
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable = {0};
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable = {0};
  bool                       LogicalIdValid;

  LogicalIdValid = SocFindApplicableLogicalTableEntries(RawId,
    &LogicalFamilyTable,
    &LogicalRevisionTable
    );
  if (LogicalIdValid) {
    LogicalId->CoreFamily = LogicalFamilyTable->LogicalCoreFamily;
    LogicalId->CoreRevision = LogicalRevisionTable->LogicalCoreRev;
  }

  return LogicalIdValid;
}

/**
 * GetSocLogicalIdOnCurrentCore
 * @brief This service retrieves the logical family and revision for the executing core.
 *
 * @param[out] LogicalId        Pointer to the logical ID for the executing core.
 *
 * @retval SilPass              The executing core's logical ID was successfully retrieved.
 * @retval SilInvalidParameter  All output parameter pointers are NULL.
 * @retval SilDeviceError       Unknown processor family found.
 *
 **/
SIL_STATUS GetSocLogicalIdOnCurrentCore (SOC_LOGICAL_ID *LogicalId)
{
  if (LogicalId == NULL) {
    return SilInvalidParameter;
  }

  if (SocConvertRawToLogical(xUslGetRawIdOnExecutingCore (), LogicalId)) {
    return SilPass;
  } else {
    return SilDeviceError;
  }
}

/**
 * SocFamilyIdCheck
 *
 * @brief   SocFamilyIDCheck function compare the logical family of the
 *          executing core received  from "GetSocLogicalIdOnCurrentCore"
 *          function with input logic family parameter.
 *          if both logical family info match, return true as SOC Family ID check pass.
 *         else return false as SOC Family ID check fail.
 *
 * @param[in]  SocFamilyId        FamailyId
 *
 * @return     TRUE  -  SOC Family ID check pass.
 *             FALSE -  SOC Family ID check fail.
 *
 */
bool
SocFamilyIdCheck (
  uint32_t SocFamilyId
  )
{
  SOC_LOGICAL_ID    LogicalId;
  SIL_STATUS        Status;

  // Get logical family and revision for the executing core
  Status = GetSocLogicalIdOnCurrentCore(&LogicalId);
  if (Status == SilPass) {
    if ( LogicalId.Family == SocFamilyId ) {
      return true;
    }
  }
  return false;
}

/**
 * SocCheck
 *
 * @brief       This function checks SOC Family & Ext Model & Base Model & Stepping & Package Type identification ID
 *
 * @param[in]   SocFamilyID  - FamilyID
 * @param[in]   Attributes   - Attribute
 *                             To check for the extended Model, set Attribute to FIXED_EXT_MODEL
 *                             To not check for the extended Model, set Attribute to ANY_EXT_MODEL
 *                             To check for the base Model, set Attribute to FIXED_BASE_MODEL
 *                             To not check for the base Model, set Attribute to ANY_BASE_MODEL
 *                             To not check for the stepping, set Attribute to ANY_STEPPING
 *                             To not check for the package type, set Attribute to ANY_PKG_TYPE
 *                             To check the package type, please set package type to Attribute bit 0~3
 *
 * @retval      true         - check passed.
 *              false        - check failed.
 */
bool
SocCheck (
  uint32_t SocFamilyID,
  uint32_t Attributes
  )
{
  CPUID_DATA    Regs = {0};

  xUslCpuId(0x80000001, 0, &Regs);

  if ((SocFamilyID & (CPUID_EXT_FAMILY_MASK | CPUID_BASE_FAMILY_MASK)) !=
    (Regs.EaxReg & (CPUID_EXT_FAMILY_MASK | CPUID_BASE_FAMILY_MASK))) {
    return false;
  }

  if ((Attributes & ANY_EXT_MODEL) == FIXED_EXT_MODEL) {
    if ((SocFamilyID & CPUID_EXT_MODEL_MASK) != (Regs.EaxReg & CPUID_EXT_MODEL_MASK)) {
      return false;
    }
  }

  if ((Attributes & ANY_BASE_MODEL) == FIXED_BASE_MODEL) {
    if ((SocFamilyID & CPUID_BASE_MODEL_MASK) != (Regs.EaxReg & CPUID_BASE_MODEL_MASK)) {
      return false;
    }
  }

  if ((Attributes & ANY_STEPPING) != ANY_STEPPING) {
    if ((SocFamilyID & CPUID_STEPPING_MASK) != (Regs.EaxReg & CPUID_STEPPING_MASK)) {
      return false;
    }
  }

  if ((Attributes & ANY_PKG_TYPE) != ANY_PKG_TYPE) {
    if ((Attributes & ANY_PKG_TYPE) != ((Regs.EbxReg >> 28) & 0xF)) {
      return false;
    }
  }

  return true;
}

/**
 * GetCoreLogicalIdOnCurrentCore
 * @brief This service retrieves the core logical family and revision for the executing core.
 *
 * @param[out] CoreLogicalId    Pointer to the core logical ID for the executing core.
 *
 * @retval SilPass              The executing core's logical ID was successfully retrieved.
 * @retval SilInvalidParameter  All output parameter pointers are NULL.
 * @retval SilDeviceError       Unknown processor family found.
 *
 **/
SIL_STATUS
GetCoreLogicalIdOnCurrentCore (
  CORE_LOGICAL_ID *CoreLogicalId
  )
{
  if (CoreLogicalId == NULL) {
    return SilInvalidParameter;
  }

  if ((BaseCoreConvertRawToLogical(xUslGetRawIdOnExecutingCore (), CoreLogicalId))) {
    return SilPass;
  } else {
    return SilDeviceError;
  }
}
