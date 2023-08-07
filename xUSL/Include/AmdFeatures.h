/**  @file
 *  This file contains definitions required to get platform and performance features.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#pragma pack (push, 1)

/**
 * Platform Features
 *
 * @details This structure is used to return the platform features.  These
 *          values are used to ensure platform feature dependent MSR entries
 *          should be applied to the current platform.
 *
 *          The comments with the bit number are useful for computing
 *          the reserved member size, but do not write code that assumes
 *          you know what the bit number of these members are.
 *
 *          These platform features are standard for all logical families
 *          and models.
 *
 */
typedef union {
  uint32_t            PlatformValue;              ///< Describe Platform Features in uint32_t
  struct {
    uint32_t          :4;                         ///< BIT_0 - BIT_3 Reserved
    uint32_t          PlatformX2Apic:1;           ///< BIT_4 x2APIC mode desired
    uint32_t          PlatformApic:1;             ///< BIT_5 APIC mode desired
    uint32_t          PlatformDisIbrs:1;          ///< BIT_6 IBRS (Indirect Branch Prediction Speculation) should be disabled
    uint32_t          :1;                         ///< BIT_7 Reserved
    uint32_t          PlatformBranchSampling:1;   ///< BIT_8 Branch Sampling desired
    uint32_t          :(31 - 9);                  ///< The possibilities are (not quite) endless.
    uint32_t          AndPlatformFeats:1;         ///< BIT_31
  } PlatformFeatures;
} PLATFORM_FEATS;

// Initializer bit patterns for PROFILE_FEATS.

#define PERFORMANCE_AND           BIT_32(31)
#define PERFORMANCE_PROFILE_ALL   0

/**
 * Profile Features
 *
 * @details This structure is used to return Profile specific Type Features.
 *          These values are used to ensure entries for Performance
 *          Profile SMU Index/Data Registers and Performance Profile PCI
 *          Registers should be applied for the current profile.
 *
 *          Currently these are designated for future use.
 *
 */
typedef union {
  uint32_t            ProfileValue;           ///< The profile features in uint32_t
  struct {
    uint32_t          :31;                    ///< Available for future expansion.
    uint32_t          AndPerformanceFeats:1;  ///< BIT31.  AND other selected features.
  } ProfileFeatures;                          ///< The profile features.
} PROFILE_FEATS;

/*
 *  Declare function prototypes here
 *
 */

void
GetPlatformFeatures (
  PLATFORM_FEATS  *Features,
  uint8_t         ApicMode,
  bool            IbrsEn,
  uint32_t        EnvironmentFlag,
  bool            BranchSampling
  );

void
GetPerformanceFeatures (
  PROFILE_FEATS   *Features
  );

#pragma pack (pop)
