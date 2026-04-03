/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_NBIFMM.h
 * @brief This file contains NBIFMM Register definitions
 */

#pragma once

#define BIT2_OFFSET_AAAABEU       2
#define BIT2_MASK_AAAABET         0x4
#define NBIFMMSx10131000                       0x10131000UL

#define BIT2_OFFSET_AAAABEY   2
#define BIT2_MASK_AAAABEX     0x4
#define BIT7_OFFSET_AAAABFA 7
#define BIT7_MASK_AAAABEZ 0x80
#define NBIFMMx10131004                0x10131004UL
#define NBIFMMx10131008                       0x10131008UL

#define BIT7_OFFSET_AAAABFC 7
#define BIT7_MASK_AAAABFB 0x80
#define NBIFMMx1013100c                       0x1013100cUL

#define BIT18_OFFSET_AAAABFQ 18
#define BIT18_MASK_AAAABFP 0x40000
#define BIT23_OFFSET_AAAABFK 23
#define BIT23_MASK_AAAABFJ 0x800000
#define BIT24_OFFSET_AAAABFM 24
#define BIT24_MASK_AAAABFL 0x1000000
#define BIT25_OFFSET_AAAABFI 25
#define BIT25_MASK_AAAABFH 0x2000000
#define BIT26_OFFSET_AAAABFG 26
#define BIT26_MASK_AAAABFF 0x4000000
#define BIT27_OFFSET_AAAABFO 27
#define BIT27_MASK_AAAABFN 0x8000000
#define BIT29_OFFSET_AAAABFE 29
#define BIT29_MASK_AAAABFD 0x20000000
#define NBIFMMx10131014                       0x10131014UL

#define BIT16_OFFSET_AAAABFW       16
#define BITS_16_TO_23_MASK_AAAABFV         0xff0000
#define BIT24_OFFSET_AAAABFS       24
#define BITS_24_TO_28_MASK_AAAABFR         0x1f000000
#define BIT29_OFFSET_AAAABFU       29
#define BITS_29_TO_31_MASK_AAAABFT         0xe0000000
#define NBIFMMx1013101c                       0x1013101cUL

#define BIT0_OFFSET_AAAABCZ    0
#define BITS_0_TO_15_MASK_AAAABCY      0xffff
#define BIT28_OFFSET_AAAABDA      28
#define BIT9_OFFSET_AAAABDO 9
#define BITS_9_TO_13_MASK_AAAABDN 0x3e00
#define BIT16_OFFSET_AAAABDK       16
#define BIT16_MASK_AAAABDJ         0x10000
#define BIT17_OFFSET_AAAABDI       17
#define BIT17_MASK_AAAABDH         0x20000
#define BIT18_OFFSET_AAAABDM       18
#define BIT18_MASK_AAAABDL         0x40000
#define BIT24_OFFSET_AAAABDQ 24
#define BITS_24_TO_26_MASK_AAAABDP  0x7000000
#define BIT27_OFFSET_AAAABDS  27
#define BIT27_MASK_AAAABDR    0x8000000
#define BIT28_OFFSET_AAAABDU     28
#define BIT28_MASK_AAAABDT       0x10000000
#define BIT29_OFFSET_AAAABDW 29
#define BIT29_MASK_AAAABDV 0x20000000
#define BIT31_OFFSET_AAAABDY 31
#define BIT31_MASK_AAAABDX 0x80000000
#define NBIFMMx10134000                0x10134000UL
#define NBIFMMx10134008                       0x10134008UL

#define BIT18_OFFSET_AAAABEC      18
#define BIT18_MASK_AAAABEB        0x40000
#define BIT20_OFFSET_AAAABEA      20
#define BIT20_MASK_AAAABDZ        0x100000

#define BIT20_OFFSET_AAAABEE 20
#define BIT20_MASK_AAAABED 0x100000
#define BIT21_OFFSET_AAAABEG    21
#define BIT21_MASK_AAAABEF      0x200000
#define BIT23_OFFSET_AAAABEK      23
#define BITS_23_TO_27_MASK_AAAABEJ        0xf800000
#define NBIFMMx1013400c                0x1013400cUL
#define NBIFMMSx10134010                       0x10134010UL

#define BIT28_OFFSET_AAAABEM      28
#define BIT28_MASK_AAAABEL        0x10000000

#define BIT0_OFFSET_AAAABCN                     0
#define BIT0_MASK_AAAABCL                       0x1

#define BIT0_OFFSET_AAAABCX 0
#define BITS_0_TO_15_MASK_AAAABCW 0xffff
#define NBIFMMx101236bc    0x101236bcUL

#define BIT7_OFFSET_AAAABGA 7
#define BIT7_MASK_AAAABFZ 0x80
#define NBIFMMx1013120c                       0x1013120cUL

#define BIT16_OFFSET_AAAABGG       16
#define BITS_16_TO_23_MASK_AAAABGF         0xff0000
#define BIT24_OFFSET_AAAABGC       24
#define BITS_24_TO_28_MASK_AAAABGB         0x1f000000
#define BIT29_OFFSET_AAAABGE       29
#define BITS_29_TO_31_MASK_AAAABGD         0xe0000000
#define NBIFMMx1013121c                       0x1013121cUL

#define BIT7_OFFSET_AAAABGS 7
#define BIT7_MASK_AAAABGR 0x80
#define NBIFMMx1013140c                       0x1013140cUL

#define BIT16_OFFSET_AAAABGY       16
#define BITS_16_TO_23_MASK_AAAABGX         0xff0000
#define BIT24_OFFSET_AAAABGU       24
#define BITS_24_TO_28_MASK_AAAABGT         0x1f000000
#define BIT29_OFFSET_AAAABGW       29
#define BITS_29_TO_31_MASK_AAAABGV         0xe0000000
#define NBIFMMx1013141c                       0x1013141cUL

#define BIT30_OFFSET_AAAABEO 30
#define BIT30_MASK_AAAABEN 0x40000000
#define NBIFMMx1013460c                       0x1013460cUL

#define BIT30_OFFSET_AAAABEQ 30
#define BIT30_MASK_AAAABEP 0x40000000
#define NBIFMMx1013480c                       0x1013480cUL

#define BIT8_OFFSET_AAAABES 8
#define BITS_8_TO_15_MASK_AAAABER 0xff00
#define NBIFMMx10134a34                      0x10134a34UL

#define NBIFMMx10135008                       0x10135008UL

#define NBIFMMx10135208                       0x10135208UL

#define BIT22_OFFSET_AAAABFY       22
#define BIT22_MASK_AAAABFX         0x400000
#define NBIFMMx10135210                       0x10135210UL

#define NBIFMMx10136008                       0x10136008UL

#define BIT30_OFFSET_AAAABGI 30
#define BIT30_MASK_AAAABGH 0x40000000
#define NBIFMMx1013600c                       0x1013600cUL
#define NBIFMMx10136608                       0x10136608UL

#define BIT30_OFFSET_AAAABGK 30
#define BIT30_MASK_AAAABGJ 0x40000000
#define NBIFMMx1013660c                       0x1013660cUL

#define BIT30_OFFSET_AAAABGM 30
#define BIT30_MASK_AAAABGL 0x40000000
#define NBIFMMx1013680c                       0x1013680cUL

#define BIT30_OFFSET_AAAABGO 30
#define BIT30_MASK_AAAABGN 0x40000000
#define NBIFMMx10136a0c                       0x10136a0cUL

#define BIT30_OFFSET_AAAABGQ 30
#define BIT30_MASK_AAAABGP 0x40000000
#define NBIFMMx10136c0c                       0x10136c0cUL

#define BIT0_OFFSET_AAAABCP          0
#define BIT8_OFFSET_AAAABCQ          8
#define BIT16_OFFSET_AAAABCR          16
#define NBIFMMx1013a008                            0x1013a008UL
#define NBIFMMIx1013a008                           0x1013a008UL

#define BIT28_OFFSET_AAAABBU                28
#define BIT28_MASK_AAAABBT                  0x10000000
#define NBIFMMx1013a010                            0x1013a010UL

#define BIT0_OFFSET_AAAABBW       0
#define BITS_0_TO_7_MASK_AAAABBV         0xff
#define BIT8_OFFSET_AAAABBY       8
#define BITS_8_TO_15_MASK_AAAABBX         0xff00
#define BIT16_OFFSET_AAAABCA       16
#define BITS_16_TO_23_MASK_AAAABBZ         0xff0000
#define BIT24_OFFSET_AAAABCC       24
#define BITS_24_TO_31_MASK_AAAABCB         0xff000000
#define NBIFMMx1013a124                        0x1013a124UL

#define BIT0_OFFSET_AAAABCE       0
#define BITS_0_TO_7_MASK_AAAABCD         0xff
#define BIT8_OFFSET_AAAABCG       8
#define BITS_8_TO_15_MASK_AAAABCF         0xff00
#define BIT16_OFFSET_AAAABCI       16
#define BITS_16_TO_23_MASK_AAAABCH         0xff0000
#define BIT24_OFFSET_AAAABCK       24
#define BITS_24_TO_31_MASK_AAAABCJ         0xff000000
#define NBIFMMx1013a128                        0x1013a128UL

#define BIT2_OFFSET_AAAABCV   2
#define BITS_2_TO_9_MASK_AAAABCU     0x3fc
#define NBIFMMx1013a21c                        0x1013a21cUL

#define BIT0_OFFSET_AAAABCT               0
#define BIT0_MASK_AAAABCS                 0x1
#define NBIFMMx1013a220    0x1013a220UL

#define BIT31_OFFSET_AAAABEW      31
#define BIT31_MASK_AAAABEV        0x80000000
#define NBIFMMx10131000    0x10131000UL

#define BIT0_OFFSET_AAAABDE      0
#define BITS_0_TO_7_MASK_AAAABDD        0xff
#define BIT8_OFFSET_AAAABDG      8
#define BITS_8_TO_15_MASK_AAAABDF        0xff00
#define BIT16_OFFSET_AAAABDC      16
#define BITS_16_TO_23_MASK_AAAABDB        0xff0000
#define NBIFMMx10134034    0x10134034UL

#define BIT28_OFFSET_AAAABEI      28
#define BITS_28_TO_30_MASK_AAAABEH        0x70000000
#define NBIFMMx10134010    0x10134010UL

#define NBIFMMx10134200    0x10134200UL

#define NBIFMMx10131404    0x10131404UL

#define NBIFMMx10131204    0x10131204UL
#define NBIFMMx101311ac    0x101311acUL
#define NBIFMMx101313ac    0x101313acUL
#define NBIFMMx101315ac    0x101315acUL
#define NBIFMMx1013a0e0    0x1013a0e0UL
#define NBIFMMx10134038    0x10134038UL
#define NBIFMMx1013a1d0    0x1013a1d0UL
#define NBIFMMx10134c00    0x10134c00UL
