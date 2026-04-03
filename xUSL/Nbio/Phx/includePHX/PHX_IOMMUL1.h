/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_IOMMUL1.h
 * @brief This file contains IOMMUL1 Register definitions
 */

#pragma once

#define L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET                   1
#define L1_FEATURE_CNTRL_PMR_lock_bit_MASK                     0x2
#define L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET                   2
#define L1_FEATURE_CNTRL_EXE_lock_bit_MASK                     0x4
#define IOMMUL1x1480009c                            0x1480009cUL
#define IOMMUL1x1470009c                          0x1470009cUL

#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET                 4
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK                   0x10
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET               5
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK                 0x20
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET                8
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK                  0x100
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET              9
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK                0x200
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET                 10
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK                   0x400
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET             11
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK               0x800
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET              12
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK                0x1000
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET             13
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK               0x2000
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET             15
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK               0x7f8000
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET                  31
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK                    0x80000000
#define IOMMUL1x148000cc                               0x148000ccUL
#define IOMMUL1x18e000cc                                 0x18e000ccUL
#define IOMMUL1x147000cc                             0x147000ccUL
#define IOMMUL1x149000cc                               0x149000ccUL
#define IOMMUL1x14a000cc                               0x14a000ccUL

#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET                5
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK                  0x20
#define IOMMUL1x147000dc                      0x147000dcUL

#define BIT17_OFFSET_AAAAALS 17
#define BITS_17_TO_18_MASK_AAAAALR   0x60000
#define IOMMUL1x148001f4                        0x148001f4UL

#define IOMMUL1x147001f4                      0x147001f4UL
#define IOMMUL1x147000e4                      0x147000e4UL
#define IOMMUL1x148000e4                      0x148000e4UL
#define IOMMUL1x14800024                      0x14800024UL