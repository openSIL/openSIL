/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Zen4Reg.h
 * @brief Zen4 core specific register definitions
 *
 */

#define ZEN4_MAX_CORES_PER_CCD     8
#define ZEN4_MAX_CORES_PER_SKT     8

#define SMN_PMREG_INITPKG7_ADDRESS      0x20002FECUL
#define SMN_PMREG_INITPKG7_CDD_OFFSET   0x800000UL
#define SMN_PMREG_INITPKG7_CORE_OFFSET  0x20000UL
#define SMN_PMREG_INITPKG7_CCX_OFFSET   0x400000UL

typedef union {
  struct {
    uint64_t CpuFid:8;
    uint64_t CpuDfsId:6;
    uint64_t CpuVid:8;
    uint64_t IddValue:8;
    uint64_t IddDiv:2;
    uint64_t :31;
    uint64_t PstateEn:1;
  } Field;
  uint64_t  Value;
} PSTATE_DEF_STRUCT_ZEN4;

typedef union {
  struct {
    uint32_t  :11;
    uint32_t  Apic16TMode:1;
    uint32_t  :4;
    uint32_t  NumOfCores:4;
    uint32_t  NumOfComplexes:1;
    uint32_t  NumOfCcds:4;
    uint32_t  NumOfSockets:2;
    uint32_t  :5;
  } Field;
  uint32_t  Value;
} PMREG_INITPKG7_STRUCT;
