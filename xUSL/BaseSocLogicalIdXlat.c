/**
 * @file  BaseSocLogicalIdXlat.c
 * @brief Various families of SoC xlat tables
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "BaseSocLogicalIdXlat.h"

static const SOC_LOGICAL_REVISION_XLAT SocLogicalRevisionTable[] =
{
  // NOTE: the highest supported stepping should be the first one
  {0x1, 0x1, AMD_REV_F19_GENOA_B1, AMD_CORE_F19_ZEN4_B1},
  {0x1, 0x0, AMD_REV_F19_GENOA_B0, AMD_CORE_F19_ZEN4_B0},
  {0x0, 0x1, AMD_REV_F19_GENOA_A1, AMD_CORE_F19_ZEN4_A0},
  {0x0, 0x0, AMD_REV_F19_GENOA_A0, AMD_CORE_F19_ZEN4_A0},
  SOC_LOGICAL_REVISION_XLAT_TERMINATOR
};

static const SOC_LOGICAL_FAMILY_XLAT SocLogicalFamilyTable[] =
{
  {0x19, 0x1, AMD_FAMILY_19_10,   AMD_CORE_FAMILY_19_ZEN4, SocLogicalRevisionTable },   // Family 19h, Models 10h-1Fh
  SOC_LOGICAL_FAMILY_XLAT_TERMINATOR
};


/**
 * Finds the appropriate logical ID information based on SoC family / model / stepping
 *
 * @param[in]     RawId       Raw family / model / stepping
 * @param[out]    Family      Pointer to valid family table entry if retval is TRUE
 * @param[out]    Revision    Pointer to valid revision table entry if retval is TRUE
 *
 * @retval        TRUE        Logical table entries were successfully found
 * @retval        FALSE       Family is unknown
 */
bool
SocFindApplicableLogicalTableEntries (
  uint32_t                   RawId,
  SOC_LOGICAL_FAMILY_XLAT    **Family,
  SOC_LOGICAL_REVISION_XLAT  **Revision
  )
{
  SOC_LOGICAL_FAMILY_XLAT    *LogicalFamilyTable;
  SOC_LOGICAL_REVISION_XLAT  *LogicalRevisionTable;
  uint16_t                   RawFamily;
  uint8_t                    RawStepping;
  uint8_t                    RawBaseModel;
  uint8_t                    RawExtModel;
  bool                       EntriesFound;

  EntriesFound = false;

  // get Raw CPUID
  RawFamily    = (uint16_t) (((RawId & CPUID_BASE_FAMILY_MASK) >> CPUID_BASE_FAMILY_OFFSET) +
                           ((RawId & CPUID_EXT_FAMILY_MASK)  >> CPUID_EXT_FAMILY_OFFSET));
  RawBaseModel = (uint8_t)   ((RawId & CPUID_BASE_MODEL_MASK)  >> CPUID_BASE_MODEL_OFFSET);
  RawExtModel  = (uint8_t)   ((RawId & CPUID_EXT_MODEL_MASK)   >> CPUID_EXT_MODEL_OFFSET);
  RawStepping  = (uint8_t)    (RawId & CPUID_STEPPING_MASK);

  // get logical CPUID
  LogicalFamilyTable = (SOC_LOGICAL_FAMILY_XLAT *) SocLogicalFamilyTable;
  while (LogicalFamilyTable->LogicalRevisionTable != NULL) {
    if ((RawFamily == LogicalFamilyTable->RawFamily) &&
        (RawExtModel == LogicalFamilyTable->RawExtModel)) {
      LogicalRevisionTable = (SOC_LOGICAL_REVISION_XLAT *) LogicalFamilyTable->LogicalRevisionTable;  // get logical revision table
      EntriesFound = true;
      *Family = LogicalFamilyTable;
      *Revision = LogicalRevisionTable;     // initialize SOC_LOGICAL_ID.Revision with the first one (should be
                                            // the highest supported one)
      while (LogicalRevisionTable->LogicalRevision != AMD_REVISION_UNKNOWN) {
        if ((RawBaseModel == LogicalRevisionTable->RawBaseModel) &&
            (RawStepping == LogicalRevisionTable->RawStepping)) {
          *Revision = LogicalRevisionTable;
          break;
        }
        LogicalRevisionTable++;
      }
      break;
    }
    LogicalFamilyTable++;
  }
  return EntriesFound;
}
