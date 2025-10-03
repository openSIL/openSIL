/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file PcieCoreReg.h
 *  @brief This file contains all Pcie Core Register definitions
 */

#pragma once

typedef union {
  struct {
    uint32_t                                     LC_PREV_STATE40:6;
    uint32_t                                        Reserved_7_6:2;
    uint32_t                                     LC_PREV_STATE41:6;
    uint32_t                                      Reserved_15_14:2;
    uint32_t                                     LC_PREV_STATE42:6;
    uint32_t                                      Reserved_23_22:2;
    uint32_t                                     LC_PREV_STATE43:6;
    uint32_t                                      Reserved_31_30:2;
  } Field;
  uint32_t Value;
} PCIE_LC_STATE10_STRUCT;

#define SIL_RSVD_ADDR_1A380428    0x1a380428UL

#define SIL_RESERVED_1512           8
#define BITS_8_TO_15_MASK_0005      0xFF00
#define SIL_RESERVED_1511           24
#define BITS_24_TO_31_MASK_0004     0xFF000000

#define SIL_RSVD_ADDR_1A38007C      0x1a38007CUL

#define SIL_RESERVED_1514           0
#define SIL_RESERVED_1513           0x1
#define SIL_RESERVED_1788           3
#define SIL_RESERVED_1787           0x8

#define SIL_RSVD_ADDR_1A383000      0x1a383000UL

#define SIL_RESERVED_1510           17
#define SIL_RESERVED_1509           0x20000

#define SIL_RSVD_ADDR_1A383020      0x1a383020UL
