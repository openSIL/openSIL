/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CpuLib.h
 * @brief CPU registers type definition, CPU functions prototype
 *
 */

#pragma once

#include <SilCommon.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef union {
  struct {
    uint32_t  CoreMaskWidth:5;
    uint32_t  Reserved:27;
  } Bits;
  uint32_t Value;
} EXT_TOP_ENUM_EAX0;

#define SIL_RESERVED_0006 0x0000000Bul

typedef union {
  struct {
    uint32_t  field_bits_0_to_4:5;
    uint32_t  Reserved:24;
    uint32_t  field_bit_37:1;
    uint32_t  field_bit_38:1;
    uint32_t  field_bit_39:1;
  } Bits;
  uint32_t Value;
} SIL_RESERVED_UNION_0014;

typedef union {
  struct {
    uint32_t  field_bits_0_to_7:8;
    uint32_t  field_bits_8_to_15:8;
    uint32_t  Reserved:16;
  } Bits;
  uint32_t Value;
} SIL_RESERVED_UNION_0015;

#define SIL_RESERVED_0005         0x80000026ul
#define SIL_RESERVED_0007           4

#define LEVEL_TYPE_CORE                         0x1
#define LEVEL_TYPE_COMPLEX                      0x2
#define LEVEL_TYPE_CCD                          0x3
#define LEVEL_TYPE_SOCKET                       0x4

/// CPUID related registers
#define AMD_CPUID_FMF                           0x80000001ul  // Family Model Features information
#define AMD_CPUID_ASIZE_PCCOUNT                 0x80000008ul  // Address Size, Physical Core Count


//
// Memory cache types
//
typedef enum {
  CacheUncacheable = 0,
  CacheWriteCombining = 1,
  CacheWriteThrough = 4,
  CacheWriteProtected = 5,
  CacheWriteBack = 6,
  CacheInvalid = 7
} MTRR_MEMORY_CACHE_TYPE;

/**
 * Memory-Typing MSRs
 * Defined in AMD64 Architecture Programmer's Manual Volume 2: System Programming
 * Table A-3
 */
#define AMD_MTRR_VARIABLE_BASE0                 0x200
#define AMD_MTRR_VARIABLE_MASK0                 0x201
#define AMD_MTRR_VARIABLE_BASE1                 0x202
#define AMD_MTRR_VARIABLE_MASK1                 0x203
#define AMD_MTRR_VARIABLE_BASE2                 0x204
#define AMD_MTRR_VARIABLE_MASK2                 0x205
#define AMD_MTRR_VARIABLE_BASE3                 0x206
#define AMD_MTRR_VARIABLE_MASK3                 0x207
#define AMD_MTRR_VARIABLE_BASE4                 0x208
#define AMD_MTRR_VARIABLE_MASK4                 0x209
#define AMD_MTRR_VARIABLE_BASE5                 0x20A
#define AMD_MTRR_VARIABLE_MASK5                 0x20B
#define AMD_MTRR_VARIABLE_BASE6                 0x20C
#define AMD_MTRR_VARIABLE_MASK6                 0x20D
#define AMD_MTRR_VARIABLE_BASE7                 0x20E
#define AMD_MTRR_VARIABLE_MASK7                 0x20F
#define AMD_AP_MTRR_FIX64K_00000                0x250
#define AMD_AP_MTRR_FIX16K_80000                0x258
#define AMD_AP_MTRR_FIX16K_A0000                0x259
#define AMD_AP_MTRR_FIX4K_C0000                 0x268
#define AMD_AP_MTRR_FIX4K_C8000                 0x269
#define AMD_AP_MTRR_FIX4K_D0000                 0x26A
#define AMD_AP_MTRR_FIX4K_D8000                 0x26B
#define AMD_AP_MTRR_FIX4K_E0000                 0x26C
#define AMD_AP_MTRR_FIX4K_E8000                 0x26D
#define AMD_AP_MTRR_FIX4K_F0000                 0x26E
#define AMD_AP_MTRR_FIX4K_F8000                 0x26F

#define AMD_MTRR_FIX64K_WB_DRAM                 0x1E1E1E1E1E1E1E1Eull
#define AMD_MTRR_FIX16K_WB_DRAM                 0x1E1E1E1E1E1E1E1Eull

// FUNC_4 registers
// ----------------
/* Native Die CPUID Register F4x160 */
#define NATIVE_DIE_CPUID_REG                   0x160

// Misc. defines.

#define CPUID_STEPPING_MASK                    0x0000000Ful
#define CPUID_BASE_MODEL_MASK                  0x000000F0ul
#define CPUID_BASE_FAMILY_MASK                 0x00000F00ul
#define CPUID_EXT_MODEL_MASK                   0x000F0000ul
#define CPUID_EXT_FAMILY_MASK                  0x0FF00000ul
#define CPUID_BASE_MODEL_OFFSET                4
#define CPUID_BASE_FAMILY_OFFSET               8
#define CPUID_EXT_MODEL_OFFSET                 16
#define CPUID_EXT_FAMILY_OFFSET                20

#define APIC_ID_REG                            0x20

/*
 * Prototypes for CpuOps.c
 */
uint8_t
xUslGetThreadsPerCore (void);

uint32_t
xUslGetRawIdOnExecutingCore (void);

bool
xUslIsSmtDisabled (void);

uint16_t
xUslGetProcessorId (void);

uint32_t
xUslGetInitialApicId (void);

uint32_t
xUslGetPackageType (void);

uint8_t
xUslGetPhysAddrSize (void);

uint8_t
xUslGetPhysAddrReduction (void);

uint32_t
xUslGetSecureEncryption (void);

uint8_t
xUslGetCpuidCoreMaskWidth (void);

void
xUslMsrOr (
  uint32_t Index,
  uint64_t OrData
  );

void
xUslGetCpuidExtCpuTopology (
  CPUID_DATA  *Regs,
  uint8_t     SubIndex
  );

void
xUslMsrAnd (
  uint32_t Index,
  uint64_t AndData
  );

void
xUslMsrAndThenOr (
  uint32_t Index,
  uint64_t AndData,
  uint64_t OrData
  );

/*
 * Prototypes for CpuOpsXX.nasm
 */
NASM_ABI void
xUslCpuId (
  uint32_t EaxIn,
  uint32_t ExtId,
  CPUID_DATA *CpuRegs
  );

NASM_ABI uint64_t
xUslRdMsr (
  uint32_t MsrAddress
  );

NASM_ABI void
xUslWrMsr (
  uint32_t MsrAddress,
  uint64_t MsrValue
  );

NASM_ABI void
xUslWbinvd (void);

NASM_ABI void
xUslWriteCr0 (uintptr_t  Cr0);

NASM_ABI void
xUslWriteCr3 (uintptr_t  Cr3);

NASM_ABI uintptr_t
xUslReadCr0 (void);

NASM_ABI uintptr_t
xUslReadCr3 (void);

NASM_ABI uint32_t
xUslCountTrailingZeroes (
  uint32_t Val
  );

/*
 * Prototypes for CpuLibXX.nasm
 */
NASM_ABI void
xUslCpuSleep (void);

NASM_ABI void
xUslDisableCache (void);

NASM_ABI void
xUslEnableCache (void);
