/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxResetTableZen5.c
 *
 * Ccx Reset Tables Library - Zen5
 * @brief Apply register table settings
 *
 */

#include <SilCommon.h>
#include <CCX/Common/AmdTable.h>
#include <SilSocLogicalId.h>
#include <MsrReg.h>
#include <Ccx.h>
#include "Zen5Reg.h"

/*
 * List of MSR to synchronize from BSP to APs during AP launch
 */
volatile AP_MSR_SYNC ApMsrSyncListZen5[] =
{
  { MSR_CPUID_NAME_STRING0,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Brandstring
  { MSR_CPUID_NAME_STRING1,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Brandstring
  { MSR_CPUID_NAME_STRING2,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Brandstring
  { MSR_CPUID_NAME_STRING3,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Brandstring
  { MSR_CPUID_NAME_STRING4,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Brandstring
  { MSR_CPUID_NAME_STRING5,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5BrandString
  { MSR_DC_CFG,                     0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Prefetcher
  { MSR_L2_PREFETCH_CFG,            0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Prefetcher
  { MSR_DC_PF_CFG,                  0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5Prefetcher
  { MSR_SYS_CFG,                    0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { SIL_RESERVED_0377,              0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { SIL_RESERVED_0378,              0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { MSR_CSTATE_ADDRESS,             0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // CcxZen5InitializeC6
  { MSR_HWCR,                       0x0000000000000000, BIT_64(25)                  }, // CcxZen5InitializeCpb
  { MSR_DE_CFG,                     0x0000000000000000, 0xFFFFFFFFFFFFFFFF          }, // Zen5InitializePrefetchMode
  { MSR_IC_CFG,                     0x0000000000000000, BIT_64(5)                   }, // AmdReserved1
  { MSR_LS_CFG,                     0x0000000000000000, BIT_64(28)                  }, // AmdIbsHardwareEn
  { MSR_EXTENDED_FEATURE_EDX0_ECX0, 0x0000000000000000, BIT_64(36)                  }, // AmdEnableFSRM
  { MSR_CPUID_FN_00000007_FEATURES, 0x0000000000000000, BIT_64(9)                   }, // AmdEnableERMS
  { SIL_RESERVED_0385,              0x0000000000000000, BIT_64(15)                  },
  { MSR_CPUID_FN_00000007_FEATURES, 0x0000000000000000, BIT_64(16) | BIT_64(17) |
                                                        BIT_64(21) | BIT_64(28) |
                                                        BIT_64(30) | BIT_64(31)     }, // AmdCcxEnableAvx512
  { MSR_SVM_REV_FEAT_ID,            0x0000000000000000, BIT_64(13)                  }, // AmdEnableSvmAVIC
  { MSR_SVM_REV_FEAT_ID,            0x0000000000000000, BIT_64(18)                  }, // AmdEnableSvmX2AVIC
  { MSR_HWCR,                       0x0000000000000000, BIT_64(9)                   }, // AmdMonMwaitDis
  { MSR_LS_CFG2,                    0x0000000000000000, BIT_64(4) | BIT_64(56)      }, // AmdCcxDisFstStrErmsb
  { MSR_LS_CFG3,                    0x0000000000000000, BIT_64(19) | BIT_64(23) |
                                                        BIT_64(26)                  },
  { SIL_RESERVED_0384,              0x0000000000000000, BIT_64(0) | BIT_64(39)      },
  { MSR_L2_CFG1,                    0x0000000000000000, BIT_64(30)                  },
  { MSR_LS_CFG4,                    0x0000000000000000, BIT_64(7)                   },
  { MSR_LS_CFG,                     0x0000000000000000, BIT_64(53)                  }, // AmdSmallHammerConfiguration
  { MSR_LS_CFG3,                    0x0000000000000000, BIT_64(30)                  }, // AmdSmallHammerConfiguration
  { MSR_CPUID_FEATS,                0x0000000000000000, BIT_64(53)                  }, // PcdAmdApicMode
  { 0xC00110DE,                     0x0000000000000000, BIT_64(15)                  },
  { 0xC00110E3,                     0x0000000000000000, BIT_64(18) | BIT_64(19) |
                                                        BIT_64(20) | BIT_64(21) |
                                                        BIT_64(22) | BIT_64(23) |
                                                        BIT_64(24) | BIT_64(25)     },
  { CPU_LIST_TERMINAL,              0x0000000000000000, 0x0000000000000000          }
};

static const uint8_t CcxZen5AllCoreRegistersAfterApLaunch[] = {
// To enable x2APIC mode, we have to set ApicEn first then set x2ApicEn
// Transition from disabled mode to x2APIC mode is illegal per x2APIC spec

// APIC_BAR (0x0000001B)
// bit[11]     ApicEn = 1
  MAKE_MSR_ENTRY (MSR_APIC_BAR, 0x0000000000000800, 0x0000000000000800),

// APIC_BAR (0x0000001B)
// bit[10]     x2ApicEn = 1
  MAKE_MSR_PLATFORM_FEAT_ENTRY (AMD_PF_X2APIC, MSR_APIC_BAR, 0x0000000000000400, 0x0000000000000400),

// HWCR (0xC0010015)
// bit[32]     FastTprLoweringDis = 1 for xApic
// bit[27]     EffFreqReadOnlyLock = 1
// bit[14]     RsmSpCycDis = 1
// bit[13]     SmiSpCycDis = 1
  MAKE_MSR_ENTRY (MSR_HWCR, 0x0000000008006000, 0x0000000008006000),
  MAKE_MSR_PLATFORM_FEAT_ENTRY (AMD_PF_APIC, MSR_HWCR, 0x0000000100000000, 0x0000000100000000),

// MCA_CTL_MASK_LS (0xC0010400)
// bit [25]    SystemReadDataErrorWcb = 1
// bit [10]    SystemReadDataErrorMab = 1
// bit [09]    SystemReadDataErrorUcode = 1
  MAKE_MSR_ENTRY (MCA_CTL_MASK_LS_ADDRESS, 0x0000000002000600, 0x0000000002000600),

// MCA_CTL_MASK_IF (0xC0010401)
// bit [13]    SystemReadDataError = 1
  MAKE_MSR_ENTRY (MCA_CTL_MASK_IF_ADDRESS, 0x0000000000002000, 0x0000000000002000),

// MCA_CTL_MASK_L2 (0xC0010402)
// bit[3]     Hwa = 1
  MAKE_MSR_ENTRY (MCA_CTL_MASK_L2_ADDRESS, 0x0000000000000008, 0x0000000000000008),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_ALL,  MCA_CTL_MASK_L2_ADDRESS,
    0x0000000000000000, 0x0000000000000008),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, MCA_CTL_MASK_L2_ADDRESS,
    0x0000000000000000, 0x0000000000000008),

// MCA_CTL_MASK_FP (0xC0010406)
// bit [6]     Hwa = 1
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_ALL,  MCA_CTL_MASK_FP_ADDRESS, 0x0000000000000040,
    0x0000000000000040),

  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_ALL,  SIL_RESERVED_0004,  0x0000000000000026,
    0x0000000000000026),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, SIL_RESERVED_0004, 0x0000000000000026,
    0x0000000000000026),

  MAKE_MSR_CPU_REV_ENTRY(
    AMD_FAMILY_BRH,
    AMD_REV_F1A_BRH_ALL,
    SIL_RESERVED_0376,
    0x0000000000000080,
    0x0000000000000080
    ),
// Table Terminator
  MAKE_TABLE_TERMINATOR
};

static const uint8_t CcxZen5ComputeUnitRegistersAfterApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------
//MSR C000_0410 McaIntrCfg
// bit[3] Reserved = 1
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_ALL, MSR_MCA_INTR_CFG, BIT_64(3), BIT_64(3)),

// SYS_CFG (0xC0010010)
// bit[20]    MtrrVarDramEn = 1
// bit[18]    MtrrFixDramEn = 1
  MAKE_MSR_ENTRY(MSR_SYS_CFG, 0x0000000000140000, 0x0000000000140000),

// LS_CFG (0xC0011020)
// bit[62]    Reserved = 1 for BRH, BRHD
// bit[52:50] Reserved = 1
// bit[46]    Reserved = 1
  MAKE_MSR_ENTRY(MSR_LS_CFG, 0x0004400000000000, 0x001C400000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_ALL, MSR_LS_CFG, 0x4000000000000000, 0x4000000000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, MSR_LS_CFG, 0x4000000000000000, 0x4000000000000000),

  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_Ax, SIL_RESERVED_0387, 0x0000400000000000, 0x0000400000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_B0, SIL_RESERVED_0387, 0x0000400000000000, 0x0000400000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_Ax, SIL_RESERVED_0387, 0x0000400000000000, 0x0000400000000000),

// ChL2Cfg0 (0xC001102A)
// bit[15]    Reserved = 1
// bit[7]     Reserved = 1
  MAKE_MSR_ENTRY(MSR_L2_CFG, 0x0000000000008080, 0x0000000000008080),

// LS_CFG2 (0xC001102D)
// bit[56]    Reserved = 1 for BRH Ax/Bx, BRHD Ax
// bit[36:35] Reserved = 1
// bit[34]    Reserved = 1 for BRH, BRHD
  MAKE_MSR_ENTRY(MSR_LS_CFG2, 0x0000000800000000, 0x0000001800000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_Ax,   MSR_LS_CFG2, 0x0100000000000000, 0x0100000000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_Bx,   MSR_LS_CFG2, 0x0100000000000000, 0x0100000000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_Ax,  MSR_LS_CFG2, 0x0100000000000000, 0x0100000000000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_ALL,  MSR_LS_CFG2, 0x0000000400000000, 0x0000000400000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, MSR_LS_CFG2, 0x0000000400000000, 0x0000000400000000),

// ChL2Cfg1 (0xC00110E2)
// bit[49]    Reserved = 1
// bit[46:45] Reserved = 1
// bit[44]    Reserved = 1
// bit[4]     Reserved = 1
// bit[3]     Reserved = 1
  MAKE_MSR_ENTRY(MSR_L2_CFG1, 0x0002300000000018, 0x0002700000000018),

// LS_CFG3 (0xC00110E5)
// bit[26]    Reserved = 1
  MAKE_MSR_ENTRY(MSR_LS_CFG3, 0x0000000004000000, 0x0000000004000000),

// LS_CFG4 (0xC00110E6)
// bit[6]    Reserved = 1 for BRH
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_ALL, MSR_LS_CFG4, 0x0000000000000040, 0x0000000000000040),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, MSR_LS_CFG4, 0x0000000000000040, 0x0000000000000040),

// DC_PF_CFG (0xC00110E7)
// bit[32:30] Reserved = 1 for BRH Ax/Bx
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_Ax, MSR_DC_PF_CFG, 0x00000001C0000000, 0x00000001C0000000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_Bx, MSR_DC_PF_CFG, 0x00000001C0000000, 0x00000001C0000000),

// Reserved (0xC00110E9)
// bit[7:4]   Reserved = 0 for BRH Bx, BRH Cx
// bit[3:0]   Reserved = 0 for BRH Bx, BRH Cx
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,
                          AMD_REV_F1A_BRH_Bx,   MSR_L2_AA_PAIR_CFG0,
                          0x0000000000000000, 0x00000000000000FF),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,
                          AMD_REV_F1A_BRH_Cx,   MSR_L2_AA_PAIR_CFG0,
                          0x0000000000000000, 0x00000000000000FF),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRHD,
                          AMD_REV_F1A_BRHD_ALL, MSR_L2_AA_PAIR_CFG0,
                          0x0000000000000000, 0x00000000000000FF),
// Reserved (0xC00110EA)
// bit[7:4]   Reserved = 3 for BRH Bx, BRH Cx
// bit[3:0]   Reserved = 2 for BRH Bx, BRH Cx
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,
                          AMD_REV_F1A_BRH_Bx,   MSR_L2_AA_PAIR_CFG1,
                          0x0000000000000032, 0x00000000000000FF),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRH,
                          AMD_REV_F1A_BRH_Cx,   MSR_L2_AA_PAIR_CFG1,
                          0x0000000000000032, 0x00000000000000FF),
  MAKE_MSR_CPU_REV_ENTRY (AMD_FAMILY_BRHD,
                          AMD_REV_F1A_BRHD_ALL, MSR_L2_AA_PAIR_CFG1,
                          0x0000000000000032, 0x00000000000000FF),

  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_A0, SIL_RESERVED_0383, 0x0000000000040000, 0x0000000000040000),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH, AMD_REV_F1A_BRH_B0, SIL_RESERVED_0383, 0x0000000000040000, 0x0000000000040000),

// Reserved (0xC00110EC)
// bit[9:5] Reserved = 0x1F for BRH Bx, BRH Cx, BRHD
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRH,  AMD_REV_F1A_BRH_Cx,   SIL_RESERVED_0384, 0x00000000000003E0, 0x00000000000003E0),
  MAKE_MSR_CPU_REV_ENTRY(AMD_FAMILY_BRHD, AMD_REV_F1A_BRHD_ALL, SIL_RESERVED_0384, 0x00000000000003E0, 0x00000000000003E0),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

static const uint8_t CcxZen5ComplexRegistersAfterApLaunch[] = {
// Table Terminator
  MAKE_TABLE_TERMINATOR
};

static const REGISTER_TABLE CcxZen5ComplexRegTableAfterApLaunch = {
  ComplexPrimary,
  CcxZen5ComplexRegistersAfterApLaunch,
};

static const REGISTER_TABLE CcxZen5CURegTableAfterApLaunch = {
  ComputeUnitPrimary,
  CcxZen5ComputeUnitRegistersAfterApLaunch,
};

static const REGISTER_TABLE CcxZen5AllCoreRegTableAfterApLaunch = {
  AllCores,
  CcxZen5AllCoreRegistersAfterApLaunch,
};

static const REGISTER_TABLE *CcxZen5RegisterTablesAfterApLaunch[] = {
  &CcxZen5AllCoreRegTableAfterApLaunch,
  &CcxZen5CURegTableAfterApLaunch,
  &CcxZen5ComplexRegTableAfterApLaunch,
  NULL
};

static const REGISTER_TABLE *CcxZen5RegisterTablesAfterApLaunchSecureS3[] = {
  NULL
};

const REGISTER_TABLE_AT_GIVEN_TP CcxZen5RegTableListAtGivenTP[] = {
  {AmdRegisterTableTpAfterApLaunch, CcxZen5RegisterTablesAfterApLaunch},
  {AmdRegisterTableTpAfterApLaunchSecureS3,
    CcxZen5RegisterTablesAfterApLaunchSecureS3},
  {MaxAmdRegisterTableTps, NULL}
};
