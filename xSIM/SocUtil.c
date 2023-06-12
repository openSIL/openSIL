/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  SocUtil.c
 * @brief OpenSIL SoC utilities
 *
 *
 */

#include <SilCommon.h>
#include "SocUtil.h"
#include <CommonLib/CpuLib.h>
#include <SilSocLogicalId.h>

/**
 * @brief IdentifySoC - Confirms if the SoC identified is supported in this revision of the xSIM module.
 *
 * @return int
 */
void IdentifySoC (SilSocCommonData *SocData)
{
  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "Entry\n");

  SocData->Family = 0xA;
  SocData->ExtFamily = 0xB;
  SocData->Model = 0xC;
  SocData->ExtModel = 0xD;
  SocData->Stepping = 0xE;

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Exit\n");

}

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
bool
SilSocIdentificationCheck (
  uint32_t SocFamilyID,
  uint32_t Attributes
  )
{


  CPUID_DATA  Regs;

  Regs.EaxReg = 0;
  Regs.EbxReg = 0;
  Regs.EcxReg = 0;
  Regs.EdxReg = 0;

  xUslCpuId(AMD_CPUID_FMF, 0, &Regs);

  // Check Family
  if ((SocFamilyID & (CPUID_EXT_FAMILY_MASK | CPUID_BASE_FAMILY_MASK)) !=
    (Regs.EaxReg & (CPUID_EXT_FAMILY_MASK | CPUID_BASE_FAMILY_MASK))) {
    return false;
  }

  // Check Ext Model
  if ((Attributes & ANY_EXT_MODEL) == FIXED_EXT_MODEL) {
    if ((SocFamilyID & CPUID_EXT_MODEL_MASK) != (Regs.EaxReg & CPUID_EXT_MODEL_MASK)) {
      return false;
    }
  }

  // Check Base Model
  if ((Attributes & ANY_BASE_MODEL) == FIXED_BASE_MODEL) {
    if ((SocFamilyID & CPUID_BASE_MODEL_MASK) != (Regs.EaxReg & CPUID_BASE_MODEL_MASK)) {
      return false;
    }
  }

  // Check Stepping
  if ((Attributes & ANY_STEPPING) != ANY_STEPPING) {
    if ((SocFamilyID & CPUID_STEPPING_MASK) != (Regs.EaxReg & CPUID_STEPPING_MASK)) {
      return false;
    }
  }

  // Check Package Type
  if ((Attributes & ANY_PKG_TYPE) != ANY_PKG_TYPE) {
    if ((Attributes & ANY_PKG_TYPE) != ((Regs.EbxReg >> 28) & 0xF)) {
      return false;
    }
  }

  return true;
}
