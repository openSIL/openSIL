/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_PCIERCCFG.h
 * @brief This file contains PCIERCCFG Register definitions
 */

#pragma once

#define PCIE_CAP_SLOT_IMPLEMENTED_OFFSET                       8
#define PCIE_CAP_SLOT_IMPLEMENTED_MASK                         0x100

#define BIT0_OFFSET_AAAAAZB                    0
#define BIT0_MASK_AAAAAZA                      0x1
#define BIT1_OFFSET_AAAAAZR                 1
#define BIT1_MASK_AAAAAZQ                   0x2
#define BIT2_OFFSET_AAAAAZL                     2
#define BIT2_MASK_AAAAAZK                       0x4
#define BIT3_OFFSET_AAAAAZD                 3
#define BIT3_MASK_AAAAAZC                   0x8
#define BIT4_OFFSET_AAAAAZT                  4
#define BIT4_MASK_AAAAAZS                    0x10
#define BIT5_OFFSET_AAAAAZJ                       5
#define BIT5_MASK_AAAAAZI                         0x20
#define BIT6_OFFSET_AAAAAZH                        6
#define BIT6_MASK_AAAAAZG                          0x40
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET                   7
#define BITS_7_TO_14_MASK_AAAAAZW                     0x7f80
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET                   15
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK                     0x18000
#define BIT17_OFFSET_AAAAAZF          17
#define BIT17_MASK_AAAAAZE            0x20000
#define BIT18_OFFSET_AAAAAZN         18
#define BIT18_MASK_AAAAAZM           0x40000
#define SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET                      19
#define SLOT_CAP_PHYSICAL_SLOT_NUM_MASK                        0xfff80000
#define PCIERCCFGx1110006c                             0x1110006cUL

#define BIT3_OFFSET_AAAABAL            3
#define BIT3_MASK_AAAABAK              0x8
#define BIT5_OFFSET_AAAABAH                       5
#define BIT5_MASK_AAAABAG                         0x20
#define BIT12_OFFSET_AAAABAD                   12
#define BIT12_MASK_AAAABAC                     0x1000
#define BIT16_OFFSET_AAAAAZZ                   16
#define BIT16_MASK_AAAAAZY                     0x10000
#define BIT17_OFFSET_AAAABAP                    17
#define BIT17_MASK_AAAABAO                      0x20000
#define BIT18_OFFSET_AAAABAJ                    18
#define BIT18_MASK_AAAABAI                      0x40000
#define BIT19_OFFSET_AAAABAN               19
#define BIT19_MASK_AAAABAM                 0x80000
#define BIT20_OFFSET_AAAABAB                     20
#define BIT20_MASK_AAAABAA                       0x100000
#define BIT24_OFFSET_AAAABAF                      24
#define BIT24_MASK_AAAABAE                        0x1000000


#define BIT0_OFFSET_AAAAAYW                  0
#define BITS_0_TO_3_MASK_AAAAAYV                    0xf
#define BIT6_OFFSET_AAAAAYU                6
#define BIT6_MASK_AAAAAYT                  0x40
#define BIT7_OFFSET_AAAAAYS           7
#define BIT7_MASK_AAAAAYR             0x80
#define PCIERCCFGx11100080                   0x11100080UL

#define BITS_7_TO_9_MASK_AAAAAYX                            0x380
#define PCIERCCFGx11100088                     0x11100088UL

#define PCIERCCFGx11100058    0x11100058UL

#define PCIERCCFGx11100068    0x11100068UL

#define PCIERCCFGx11100388    0x11100388UL
