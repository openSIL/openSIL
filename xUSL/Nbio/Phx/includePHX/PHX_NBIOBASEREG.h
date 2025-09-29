/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_NBIOBASEREG.h
 * @brief This file contains all Nbio Base Register definitions
 */

#pragma once

#define BIT0_OFFSET_AAAAADU      0
#define BIT0_MASK_AAAAADT        0x1
#define NBIOBASEREGx1470001c                0x1470001cUL

#define BIT29_OFFSET_AAAAAEJ      29
#define BIT29_MASK_AAAAAEI        0x20000000
#define BIT31_OFFSET_AAAAAEH      31
#define BIT31_MASK_AAAAAEG        0x80000000
#define NBIOBASEREGx11180080    0x11180080UL

#define BIT17_OFFSET_AAAAAEL      17
#define BIT17_MASK_AAAAAEK        0x20000
#define BIT21_OFFSET_AAAAAEN      21
#define BIT21_MASK_AAAAAEM        0x200000
#define NBIOBASEREGx11180040                 0x11180040UL

#define BIT6_OFFSET_AAAAAGJ      6
#define BIT6_MASK_AAAAAGI        0x40
#define BIT14_OFFSET_AAAAAGH      14
#define BITS_14_TO_15_MASK_AAAAAGG        0xc000
#define NBIOBASEREGx11180100    0x11180100UL

#define BIT18_OFFSET_AAAAAGL      18
#define BIT18_MASK_AAAAAGK        0x40000
#define BIT20_OFFSET_AAAAAGN      20
#define BIT20_MASK_AAAAAGM        0x100000
#define NBIOBASEREGx111802c0    0x111802c0UL

#define BIT3_OFFSET_AAAAADX      3
#define BIT3_MASK_AAAAADW        0x8
#define BIT4_OFFSET_AAAAAEB      4
#define BIT4_MASK_AAAAAEA        0x10
#define BIT7_OFFSET_AAAAADZ  7
#define BIT7_MASK_AAAAADY    0x80
#define NBIOBASEREGx11140040    0x11140040UL

#define BIT16_OFFSET_AAAAAFF         16
#define BIT16_MASK_AAAAAFE           0x10000
#define BIT23_OFFSET_AAAAAFD      23
#define BIT23_MASK_AAAAAFC        0x800000
#define NBIOBASEREGx11140280    0x11140280UL

#define BIT14_OFFSET_AAAAAET      14
#define BITS_14_TO_15_MASK_AAAAAES        0xc000
#define BIT17_OFFSET_AAAAAEP      17
#define BIT17_MASK_AAAAAEO        0x20000
#define BIT18_OFFSET_AAAAAER      18
#define BIT18_MASK_AAAAAEQ        0x40000
#define BIT0_OFFSET_AAAAAGT                0
#define BIT0_MASK_AAAAAGS                  0x1
#define BIT29_OFFSET_AAAAADV                  29
#define NBIOBASEREGx111402c4          0x111402c4UL

#define BIT12_OFFSET_AAAAAEV      12
#define BIT12_MASK_AAAAAEU        0x1000
#define BIT14_OFFSET_AAAAAEX      14
#define BIT14_MASK_AAAAAEW        0x4000
#define NBIOBASEREGx111402d4         0x111402d4UL

#define BIT4_OFFSET_AAAAAEZ      4
#define BIT4_MASK_AAAAAEY        0x10
#define NBIOBASEREGx111402d8     0x111402d8UL

#define BIT2_OFFSET_AAAAAFJ      2
#define BITS_2_TO_3_MASK_AAAAAFI        0xc
#define BIT6_OFFSET_AAAAAFL      6
#define BIT6_MASK_AAAAAFK        0x40
#define BIT9_OFFSET_AAAAAFH      9
#define BIT9_MASK_AAAAAFG        0x200
#define NBIOBASEREGx11140394    0x11140394UL

#define BIT2_OFFSET_AAAAAFP      2
#define BITS_2_TO_3_MASK_AAAAAFO        0xc
#define BIT6_OFFSET_AAAAAFR      6
#define BIT6_MASK_AAAAAFQ        0x40
#define BIT9_OFFSET_AAAAAFN      9
#define BIT9_MASK_AAAAAFM        0x200
#define NBIOBASEREGx11140390    0x11140390UL

#define BIT16_OFFSET_AAAAAFT      16
#define BITS_16_TO_18_MASK_AAAAAFS        0x70000
#define BIT20_OFFSET_AAAAAFV      20
#define BITS_20_TO_22_MASK_AAAAAFU        0x700000
#define NBIOBASEREGx11140318       0x11140318UL

#define BIT19_OFFSET_AAAAAFX      19
#define BIT19_MASK_AAAAAFW        0x80000
#define BIT21_OFFSET_AAAAAFZ      21
#define BITS_21_TO_22_MASK_AAAAAFY        0x600000
#define BIT30_OFFSET_AAAAAGB      30
#define BIT30_MASK_AAAAAGA        0x40000000
#define NBIOBASEREGx11140288    0x11140288UL

#define BIT12_OFFSET_AAAAAGD      12
#define BIT12_MASK_AAAAAGC        0x1000
#define NBIOBASEREGx11140414                       0x11140414UL

typedef union {
  struct {
    uint32_t    field_bits_0_to_5:6;
    uint32_t    field_bits_6_to_7:2;
    uint32_t    field_bits_8_to_13:6;
    uint32_t    field_bits_14_to_15:2;
    uint32_t    field_bits_16_to_21:6;
    uint32_t    field_bits_22_to_23:2;
    uint32_t    field_bits_24_to_29:6;
    uint32_t    field_bits_30_to_31:2;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0017;

#define NBIOBASEREGx11140294    0x11140294UL

#define BIT15_OFFSET_AAAAAGR      15
#define BIT15_MASK_AAAAAGQ        0x8000
#define BIT20_OFFSET_AAAAAGP      20
#define BIT20_MASK_AAAAAGO        0x100000
#define NBIOBASEREGx11140600   0x11140600UL
#define BIT6_OFFSET_AAAAAGF      6
#define BIT6_MASK_AAAAAGE        0x40
#define NBIOBASEREGx11140284                0x11140284UL

typedef union {
  struct {
    uint32_t                            field_bits_0_to_2:3;
    uint32_t                            field_bits_3_to_7 :5;
    uint32_t                            field_bits_8_to_15:8;
    uint32_t                            field_bits_16_to_18:3;
    uint32_t                            field_bits_19_to_23:5;
    uint32_t                            field_bits_24_to_31:8;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0020;
#define NBIOBASEREGx11140084    0x11140084UL

typedef union {
  struct {
    uint32_t                            field0;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0023;

#define BIT1_OFFSET_AAAAAFB      1
#define BIT1_MASK_AAAAAFA        0x2
#define NBIOBASEREGx11140378    0x11140378UL

#define BIT1_OFFSET_AAAAAED      1
#define BIT1_MASK_AAAAAEC        0x2
#define BIT2_OFFSET_AAAAAEF      2
#define BIT2_MASK_AAAAAEE        0x4
#define NBIOBASEREGx11140304    0x11140304UL
