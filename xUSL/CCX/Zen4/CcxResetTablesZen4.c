/**
 * @file CcxResetTableZen4.c
 *
 * Ccx Reset Tables Library - Zen4
 * @brief Apply register table settings
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CCX/Common/AmdTable.h>
#include <SilSocLogicalId.h>
#include <MsrReg.h>
#include <RAS/RasDefs.h>

static const uint8_t CcxZen4AllCoreRegistersAfterApLaunch[] = {
// To enable x2APIC mode, we have to set ApicEn first then set x2ApicEn
// Transition from disabled mode to x2APIC mode is illegal per x2APIC spec

// APIC_BAR (0x0000001B)
// bit[11]     ApicEn = 1
  MAKE_MSR_ENTRY (MSR_APIC_BAR, 0x0000000000000800, 0x0000000000000800),

// APIC_BAR (0x0000001B)
// bit[10]     x2ApicEn = 1
  MAKE_MSR_PLATFORM_FEAT_ENTRY (AMD_PF_X2APIC, MSR_APIC_BAR, 0x0000000000000400,
    0x0000000000000400),

// HWCR (0xC0010015)
// bit[27]     EffFreqReadOnlyLock = 1
  MAKE_MSR_ENTRY (MSR_HWCR, 0x0000000008000000,
    0x0000000008000000),

// HWCR (0xC0010015)
// bit[14]     RsmSpCycDis = 1
// bit[13]     SmiSpCycDis = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_PF_APIC, AMD_REV_F19_GENOA_ALL,
    MSR_HWCR, 0x0000000000006000, 0x0000000000006000),

// MCA_CTL_MASK_LS (0xC0010400)
// bit [21]    L2_SOC15_WCB = 1
// bit [20]    L2_SOC15_SCB = 1
// bit [19]    L2_SOC15_LOAD = 1
// bit [12]    LDQ = 0 for A0
// bit [2]     DC_DATA_RMW = 0 for A0
  MAKE_MSR_ENTRY (MCA_CTL_MASK_LS_ADDRESS, 0x0000000000381000,
    0x0000000000381000),
  MAKE_MSR_CORE_REV_ENTRY (AMD_CORE_FAMILY_19_ZEN4, AMD_CORE_F19_ZEN4_A0,
    MCA_CTL_MASK_LS_ADDRESS, 0x0000000000000000, 0x0000000000001004),

// MCA_CTL_MASK_IF (0xC0010401)
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL,
    MCA_CTL_MASK_IF_ADDRESS, 0x0000000000010800, 0x0000000000010800),

// MCA_CTL_MASK_L2 (0xC0010402)
// bit[3]     Hwa = 1
  MAKE_MSR_ENTRY (MCA_CTL_MASK_L2_ADDRESS, 0x0000000000000008,
    0x0000000000000008),

  // bit[3]     Hwa = 0 for Genoa (bit[3] is 0 in S3 reset, here overriding the value set in command above)
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL,
  MCA_CTL_MASK_L2_ADDRESS, 0x0000000000000000, 0x0000000000000008),

// MCA_CTL_MASK_FP (0xC0010406)
// bit [6]     Hwa = 1
  MAKE_MSR_ENTRY (MCA_CTL_MASK_FP_ADDRESS, 0x0000000000000040,
    0x0000000000000040),

  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL,
    SIL_RESERVED2_919, 0x0000000033000000, 0x00000000FF000000),

  MAKE_MSR_ENTRY (SIL_RESERVED2_918, 0x0000000000000000,
    0x0000000000400000),
  MAKE_MSR_CPU_REV_PLATFORM_FEAT_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL,
    AMD_PF_BRANCH_SAMPLING, SIL_RESERVED2_918, BIT_32(31), BIT_32(31)),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL, SIL_RESERVED2_918,
    0x0000000000000100, 0x0000000000000100),

  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL,
    SIL_RESERVED2_917, 0x00, 0x10),

// MSR_OSVW_ID_Length (0xC0010140)
// bit[15:0] = 5
  MAKE_MSR_ENTRY (MSR_OSVW_ID_Length, 0x0000000000000005, 0xFFFFFFFFFFFFFFFF),

// OSVW_Status (0xC0010141)
// bit[63:0] = 0
  MAKE_MSR_ENTRY (MSR_OSVW_Status, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

static const uint8_t CcxZen4ComputeUnitRegistersAfterApLaunch[] =
{
  MAKE_MSR_ENTRY (SIL_RESERVED2_926, 0x8680000000000000, 0x8680000000000000),

  MAKE_TABLE_TERMINATOR
};

static const uint8_t CcxZen4ComplexRegistersAfterApLaunch[] = {
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL, SIL_RESERVED2_908,
    0x0000000000501680, 0x0000000000DC1F80),

  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL, SIL_RESERVED2_922,
    0x0000000000001106, 0x00000000000011C6),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_GENOA, AMD_REV_F19_GENOA_ALL, SIL_RESERVED2_922,
    0x00000000001E0000, 0x00000000001E0000),
  MAKE_TABLE_TERMINATOR
};

static const REGISTER_TABLE CcxZen4ComplexRegTableAfterApLaunch = {
  ComplexPrimary,
  CcxZen4ComplexRegistersAfterApLaunch,
};

static const REGISTER_TABLE CcxZen4CURegTableAfterApLaunch = {
  ComputeUnitPrimary,
  CcxZen4ComputeUnitRegistersAfterApLaunch,
};

static const REGISTER_TABLE CcxZen4AllCoreRegTableAfterApLaunch = {
  AllCores,
  CcxZen4AllCoreRegistersAfterApLaunch,
};

static const REGISTER_TABLE *CcxZen4RegisterTablesAfterApLaunch[] = {
  &CcxZen4AllCoreRegTableAfterApLaunch,
  &CcxZen4CURegTableAfterApLaunch,
  &CcxZen4ComplexRegTableAfterApLaunch,
  NULL
};

static const REGISTER_TABLE *CcxZen4RegisterTablesAfterApLaunchSecureS3[] = {
  NULL
};

const REGISTER_TABLE_AT_GIVEN_TP CcxZen4RegTableListAtGivenTP[] = {
  {AmdRegisterTableTpAfterApLaunch, CcxZen4RegisterTablesAfterApLaunch},
  {AmdRegisterTableTpAfterApLaunchSecureS3,
    CcxZen4RegisterTablesAfterApLaunchSecureS3},
  {MaxAmdRegisterTableTps, NULL}
};
