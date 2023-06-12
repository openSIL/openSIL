/**
 * @file Zen4Reg.h
 * @brief Zen4 core specific register definitions
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#define ZEN4_MAX_CORES_PER_CCD     8
#define ZEN4_MAX_CORES_PER_SKT   128 //64

/// Init Package Register
typedef union {
  struct {
    uint32_t  :11;                  ///< Reserved
    uint32_t  Apic16TMode:1;        ///< APIC 16T mode
    uint32_t  :4;                   ///< Reserved
    uint32_t  NumOfCores:4;         ///< Number of logical cores in a complex
    uint32_t  NumOfComplexes:1;     ///< Number of logical complexes in a die
    uint32_t  NumOfCcds:4;          ///< Number of logical die in a socket
    uint32_t  NumOfSockets:2;       ///< Number of sockets in a system
    uint32_t  :5;                   ///< Reserved
  } Field;
  uint32_t  Value;
} PMREG_INITPKG7_STRUCT;
