/**
 * @file  CpuLib.h
 * @brief CPU registers type definition, CPU functions prototype
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

/// Native Die CPUID PCI Register
typedef struct {
  uint32_t Stepping:4;                 ///< Stepping
  uint32_t BaseModel:4;                ///< Base Model
  uint32_t BaseFamily:4;               ///< Base Family
  uint32_t :4;                         ///< Reserved
  uint32_t ExtModel:4;                 ///< Extended Model
  uint32_t ExtFamily:8;                ///< Extended Family
  uint32_t :4;                         ///< Reserved
} NATIVE_DIE_CPUID_REGISTER;

/// CPUID data received registers format
typedef struct {
  uint32_t  EaxReg;                    ///< CPUID instruction result in EAX
  uint32_t  EbxReg;                    ///< CPUID instruction result in EBX
  uint32_t  EcxReg;                    ///< CPUID instruction result in ECX
  uint32_t  EdxReg;                    ///< CPUID instruction result in EDX
} CPUID_DATA;

/// CPUID related registers
#define AMD_CPUID_FMF                           0x80000001ul  // Family Model Features information
#define AMD_CPUID_L2L3Cache_L2TLB               0x80000006ul
#define CPUID_ASSOCIATIVITY_DISABLED            0x00
#define CPUID_ASSOCIATIVITY_1_WAY               0x01
#define CPUID_ASSOCIATIVITY_2_WAY               0x02
#define CPUID_ASSOCIATIVITY_4_WAY               0x04
#define CPUID_ASSOCIATIVITY_8_WAY               0x06
#define CPUID_ASSOCIATIVITY_16_WAY              0x08
#define CPUID_ASSOCIATIVITY_32_WAY              0x0A
#define CPUID_ASSOCIATIVITY_48_WAY              0x0B
#define CPUID_ASSOCIATIVITY_64_WAY              0x0C
#define CPUID_ASSOCIATIVITY_96_WAY              0x0D
#define CPUID_ASSOCIATIVITY_128_WAY             0x0E
#define CPUID_ASSOCIATIVITY_FULLY               0x0F
#define AMD_CPUID_TLB_L1Cache                   0x80000005ul
#define AMD_CPUID_L1L2Tlb1GIdentifiers          0x80000019ul
#define AMD_CPUID_APM                           0x80000007ul
#define LOCAL_APIC_ID                           24
#define LOGICAL_PROCESSOR_COUNT                 16
#define AMD_CPUID_ASIZE_PCCOUNT                 0x80000008ul  // Address Size, Physical Core Count

// CPUID
#define CPUID_PROCESSOR_TOPOLOGY_INFORMATION    0x8000001Eul
#define CPUID_PROCESSOR_AND_FEATURE_IDENTIFIERS 0x00000001ul
#define CPUID_PROCESSOR_CAPACITY_PARAMETERS     0x80000008ul

//
// Memory cache types
//
typedef enum {
  CacheUncacheable    = 0,
  CacheWriteCombining = 1,
  CacheWriteThrough   = 4,
  CacheWriteProtected = 5,
  CacheWriteBack      = 6,
  CacheInvalid        = 7
} MTRR_MEMORY_CACHE_TYPE;

#define AMD_AP_MTRR_VARIABLE_BASE0              0x200
#define AMD_AP_MTRR_VARIABLE_BASE6              0x20C
#define AMD_AP_MTRR_FIX64k_00000                0x250
#define AMD_AP_MTRR_FIX16k_80000                0x258
#define AMD_AP_MTRR_FIX16k_A0000                0x259
#define AMD_AP_MTRR_FIX4k_C0000                 0x268
#define AMD_AP_MTRR_FIX4k_C8000                 0x269
#define AMD_AP_MTRR_FIX4k_D0000                 0x26A
#define AMD_AP_MTRR_FIX4k_D8000                 0x26B
#define AMD_AP_MTRR_FIX4k_E0000                 0x26C
#define AMD_AP_MTRR_FIX4k_E8000                 0x26D
#define AMD_AP_MTRR_FIX4k_F0000                 0x26E
#define AMD_AP_MTRR_FIX4k_F8000                 0x26F

#define AMD_MTRR_FIX64K_WB_DRAM                 0x1E1E1E1E1E1E1E1Eull
#define AMD_MTRR_FIX64K_WT_DRAM                 0x1C1C1C1C1C1C1C1Cull
#define AMD_MTRR_FIX64K_UC_DRAM                 0x1818181818181818ull
#define AMD_MTRR_FIX16K_WB_DRAM                 0x1E1E1E1E1E1E1E1Eull
#define AMD_MTRR_FIX16K_WT_DRAM                 0x1C1C1C1C1C1C1C1Cull
#define AMD_MTRR_FIX16K_UC_DRAM                 0x1818181818181818ull
#define AMD_MTRR_FIX4K_WB_DRAM                  0x1E1E1E1E1E1E1E1Eull
#define AMD_MTRR_FIX4K_WT_DRAM                  0x1C1C1C1C1C1C1C1Cull
#define AMD_MTRR_FIX4K_UC_DRAM                  0x1818181818181818ull

#define ENABLE_CF8_EXT_CFG                      0x00004000ul    // [46]
#define INIT_APIC_CPUID_LO                      0x00400000ul    // [54]
#define PERF_RESERVE_BIT_MASK                   0x030FFFDFFFFFull  // Mask of the Performance control Reserve bits
#define PERF_CAR_CORRUPTION_EVENT               0x040040F0E2ul    // Configure the controller to capture the
                                                    // CAR Corruption

// FUNC_0 registers
// ----------------
#define LINK_INIT_CTRL                          0x6C
#define LINK_INIT_CTRL_REQ_DIS                  0x02        // [1] = ReqDis
#define LINK_INIT_COLD_RST_DET                  BIT4
#define LINK_INIT_BIOS_RST_DET_0                BIT5
#define LINK_INIT_BIOS_RST_DET_1                BIT9
#define LINK_INIT_BIOS_RST_DET_2                BIT10
#define LINK_INIT_BIOS_RST_DET                  BIT9 | BIT10
#define EXTENDED_NODE_ID                        0x160

#define CORE_CTRL                               0x1DC
#define CORE_CTRL_CORE1_EN                      0x00000002ul
#define CORE_CTRL_CORE2_EN                      0x00000004ul
#define CORE_CTRL_CORE3_EN                      0x00000008ul
#define CORE_CTRL_CORE4_EN                      0x00000010ul
#define CORE_CTRL_CORE5_EN                      0x00000020ul
#define CORE_CTRL_CORE6_EN                      0x00000040ul
#define CORE_CTRL_CORE7_EN                      0x00000080ul
#define CORE_CTRL_CORE8_EN                      0x00000100ul
#define CORE_CTRL_CORE9_EN                      0x00000200ul

// FUNC_3 registers
// ----------------
#define HARDWARE_THERMAL_CTRL_REG               0x64
#define SOFTWARE_THERMAL_CTRL_REG               0x68
#define ACPI_PSC_0_REG                          0x80    // ACPI Power State Control Registers
#define ACPI_PSC_4_REG                          0x84
#define POWER_CTRL_MISCELLANEOUS_REG            0xA0
#define CLOCK_POWER_TIMING_CTRL2_REG            0xDC
#define NORTH_BRIDGE_CAPABILITIES_REG           0xE8
#define MULTI_NODE_CPU                          29
#define CPUID_FMR                               0xFC    // Family / Model registers
#define DOWNCORE_CTRL                           0x190   // Downcore Control Register
#define LINK_TO_XCS_TOKEN_COUNT_REG_3X148       0x148
#define REG_HT4_PHY_OFFSET_BASE_4X180           0x180
#define REG_HT4_PHY_DATA_PORT_BASE_4X184        0x184

// FUNC_4 registers
// ----------------
/* Native Die CPUID Register F4x160 */
#define NATIVE_DIE_CPUID_REG                   0x160
#define NATIVE_DIE_CPUID_PCI_ADDR              (MAKE_SBDFO (0, 0, 0x18, FUNC_4, NATIVE_DIE_CPUID_REG))

// FUNC_5 registers
// ----------------
#define COMPUTE_UNIT_STATUS                    0x80
#define NORTH_BRIDGE_CAPABILITIES_2_REG        0x84

// Misc. defines.
#define PCI_DEV_BASE                           24

#define CPUID_STEPPING_MASK                    0x0000000Ful
#define CPUID_BASE_MODEL_MASK                  0x000000F0ul
#define CPUID_BASE_FAMILY_MASK                 0x00000F00ul
#define CPUID_EXT_MODEL_MASK                   0x000F0000ul
#define CPUID_EXT_FAMILY_MASK                  0x0FF00000ul
#define CPUID_BASE_MODEL_OFFSET                4
#define CPUID_BASE_FAMILY_OFFSET               8
#define CPUID_EXT_MODEL_OFFSET                 16
#define CPUID_EXT_FAMILY_OFFSET                20

#define LAPIC_BASE_ADDR_MASK                   0x0000FFFFFFFFF000ull
#define APIC_EXT_BRDCST_MASK                   0x000E0000ul
#define APIC_ENABLE_BIT                        0x00000800ul
#define LOCAL_APIC_ADDR                        0xFEE00000ul
#define INT_CMD_REG_LO                         0x300
#define INT_CMD_REG_HI                         0x310
#define REMOTE_MSG_REG                         0x380
#define REMOTE_READ_REG                        0xC0
#define APIC_ID_REG                            0x20
#define APIC20_ApicId                          24
#define CMD_REG_TO_READ_DATA                   0x338

#define MAX_CORE_ID_SIZE                       8
#define MAX_CORE_ID_MASK                       ((1 << MAX_CORE_ID_SIZE) - 1)

/*-------------------------
 * Default definitions
 *-------------------------
 */
#define DELIVERY_STATUS                        BIT13
#define REMOTE_READ_STAT_MASK                  0x00030000ul
#define REMOTE_DELIVERY_PENDING                0x00010000ul
#define REMOTE_DELIVERY_DONE                   0x00020000ul

void xUslCpuSleep (void);
uint8_t xUslGetThreadsPerCore (void);
uint32_t xUslGetPackageType (void);
uint32_t xUslGetInitialApicId (void);
void xUslWrMsr (uint32_t MsrAddress, uint64_t MsrValue);
uint64_t xUslRdMsr (uint32_t MsrAddress);
uint8_t xUslGetPhysAddrSize (void);
uint8_t xUslGetPhysAddrReduction (void);
uint32_t xUslGetRawIdOnExecutingCore (void);
void xUslMsrAnd (uint32_t Index, uint64_t AndData);
uint32_t xUslIsSmtDisabled(void);
uint32_t xUslGetProcessorId(void);
void xUslMsrOr (uint32_t Index, uint64_t OrData);
void xUslMsrAndThenOr (uint32_t Index, uint64_t AndData, uint64_t OrData);
void xUslWbinvd(void);
uint32_t xUslGetSecureEncryption(void);
uint64_t xUslReadCr3(void);
