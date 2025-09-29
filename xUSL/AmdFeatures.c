/* SPDX-License-Identifier: MIT */
/**
 * @file  AmdFeatures.c
 *  openSIL functions to return platform and performance features
 *
 */
/*
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <SilCommon.h>
#include <AmdFeatures.h>
#include <SilSocLogicalId.h>
#include <Pci.h>
#include <CommonLib/CpuLib.h>
#include <CCX/Common/CcxApic.h>

/**--------------------------------------------------------------------
 * GetBasePlatformFeatures
 *
 * @brief   Returns the platform features list of the currently running
 *          processor core.
 *
 * @param   SocLogicalId    Executing core's logical family and logical
 *                          revision
 * @param   Features        Output the features supported by this
 *                          platform
 * @param   ApicMode        Apic mode desired
 * @param   IbrsEn          Indirect branch prediction speculation
 *                          enable flag
 * @param   BranchSampling  Branch sampling enable flag
 *
 * @retval  void
 */
static
void
GetBasePlatformFeatures (
  SOC_LOGICAL_ID  *SocLogicalId,
  PLATFORM_FEATS  *Features,
  uint8_t         ApicMode,
  bool            IbrsEn,
  bool            BranchSampling
  )
{
  UNUSED(SocLogicalId);

  Features->PlatformValue = 0;

  if (ApicMode == xApicMode) {
    Features->PlatformFeatures.PlatformApic = 1;
  } else if (ApicMode == ApicCompatibilityMode) {
    Features->PlatformFeatures.PlatformX2Apic = 1;
    Features->PlatformFeatures.PlatformApic = 1;
  } else if (ApicMode == x2ApicMode) {
    Features->PlatformFeatures.PlatformX2Apic = 1;
  }

  Features->PlatformFeatures.PlatformDisIbrs = IbrsEn ? 0 : 1;

  Features->PlatformFeatures.PlatformBranchSampling = BranchSampling ? 1 : 0;

}

/**--------------------------------------------------------------------
 * GetBasePerformanceFeatures
 *
 * @brief   Returns the performance profile features list of the
 *          currently running processor core.
 *
 * @param   Features  The performance profile features supported by
 *                    this platform
 *
 * @retval  void
 */
static
void
GetBasePerformanceFeatures (
  PROFILE_FEATS *Features
  )
{
  // Start with none.
  Features->ProfileValue = 0;
}

/**--------------------------------------------------------------------
 * GetPlatformFeatures
 *
 * @brief   Returns the platform features list of the currently running
 *          processor core.
 *
 * @param   Features        Output the features supported by this
 *                          platform
 * @param   ApicMode        Apic mode desired
 * @param   IbrsEn          Indirect branch prediction speculation
 *                          enable flag
 * @param   BranchSampling  Branch sampling enable flag
 *
 * retval   void
 */
void
GetPlatformFeatures (
  PLATFORM_FEATS  *Features,
  uint8_t         ApicMode,
  bool            IbrsEn,
  bool            BranchSampling
  )
{
  SOC_LOGICAL_ID  SocLogicalId;

  GetSocLogicalIdOnCurrentCore(&SocLogicalId);
  GetBasePlatformFeatures(&SocLogicalId, Features, ApicMode, IbrsEn, BranchSampling);
}

/**--------------------------------------------------------------------
 * GetPerformanceFeatures
 *
 * @brief   Returns the performance profile features list of the
 *          currently running processor core.
 *
 * @param   Features  The performance profile features supported by
 *                    this platform
 *
 * @retval  void
 */
void
GetPerformanceFeatures (
  PROFILE_FEATS   *Features
  )
{
  GetBasePerformanceFeatures(Features);
}
