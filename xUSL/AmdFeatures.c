/**  @file
 *  openSIL functions to return platform and performance features
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <AmdFeatures.h>
#include <SilSocLogicalId.h>
#include <Pci.h>
#include <CommonLib/CpuLib.h>

/**--------------------------------------------------------------------
 * GetBasePlatformFeatures
 *
 * @brief   Returns the platform features list of the currently running
 *          processor core.
 *
 * @param   SocLogicalId    Executing core's logical faily and logical
 *                          revision
 * @param   Features        Output the features supported by this
 *                          platform
 * @param   ApicMode        Apic mode desired
 * @param   IbrsEn          Indirect branch prediction specualtion
 *                          enable flag
 * @param   EnvironmentFlag Emulation environment flag
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
  uint32_t        EnvironmentFlag,
  bool            BranchSampling
  )
{
  Features->PlatformValue = 0;

  /*
   * PlatformFeatures bit 4 - PlatformX2Apic: x2APIC mode desired
   * PlatformFeatures bit 5 - PlatformApic: APIC mode desired
   */
  if (ApicMode == 0x1) {
    Features->PlatformFeatures.PlatformApic = 1;
  } else if (ApicMode == 0x2) {
    Features->PlatformFeatures.PlatformX2Apic = 1;
  }

  // bit 6 - PlatformDisIbrs: IBRS (Indirect Branch Prediction Speculation) should be disabled
  Features->PlatformFeatures.PlatformDisIbrs = IbrsEn ? 0 : 1;

  // bit 8 - PlatformBranchSampling: Branch Sampling desired
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
 * @param   IbrsEn          Indirect branch prediction specualtion
 *                          enable flag
 * @param   EnvironmentFlag Emulation environment flag
 * @param   BranchSampling  Branch sampling enable flag
 *
 * retval   void
 */
void
GetPlatformFeatures (
  PLATFORM_FEATS  *Features,
  uint8_t         ApicMode,
  bool            IbrsEn,
  uint32_t        EnvironmentFlag,
  bool            BranchSampling
  )
{
  SOC_LOGICAL_ID  SocLogicalId;

  GetSocLogicalIdOnCurrentCore (&SocLogicalId);
  GetBasePlatformFeatures (&SocLogicalId, Features, ApicMode, IbrsEn, EnvironmentFlag, BranchSampling);
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
  GetBasePerformanceFeatures (Features);
}
