/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbOemTc.h
 * @brief Tacoma FCH USB controller OEM platform table definitions
 */

#pragma once

#include <SilCommon.h>

#pragma pack (push, 1)

//
// USB2 PHY
//

typedef struct {
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
  uint8_t field4;
  uint8_t field5;
  uint8_t field6;
  uint8_t field7;
  uint8_t field8;
  uint8_t field9;
  uint8_t field10;
  uint8_t field11;
} SIL_RESERVED_STRUCT_0010;

typedef struct {
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
} SIL_RESERVED_STRUCT_0011;

typedef struct {
  uint8_t          field0;
  uint8_t          field1;
  uint8_t          field2;
  uint8_t          field3;
  SIL_RESERVED_STRUCT_0010 field4[8];
  SIL_RESERVED_STRUCT_0011 field5[3];
  uint8_t          field6;
  uint8_t          field7;
  uint8_t          field8[3];
  uint8_t          field9[3];
} SIL_RESERVED_STRUCT_0012;

//
// USB4 PHY
//

typedef struct {
  uint16_t  field_bits_0_to_11 : 12;
  uint16_t  field_bit_12 : 1;
  uint16_t  field_bit_13 : 1;
  uint16_t  field_bit_14 : 1;
  uint16_t  field_bit_15 : 1;
  uint16_t  field_bits_16_to_23 : 8;
  uint16_t  field_bits_24_to_26 : 3;
  uint16_t  field_bits_27_to_28 : 2;
  uint16_t  field_bit_29 : 1;
  uint16_t  field_bits_30_to_31 : 2;
  uint16_t  field_bits_32_to_47 : 16;
  uint16_t  field_bits_48_to_63 : 16;
  uint16_t  field_bits_64_to_79 : 16;
  uint16_t  field_bits_80_to_95 : 16;
  uint16_t  field_bits_96_to_99 : 4;
  uint16_t  field_bits_100_to_104 : 5;
  uint16_t  field_bit_105 : 1;
  uint16_t  field_bits_106_to_108 : 3;
  uint16_t  field_bit_109 : 1;
  uint16_t  field_bit_110 : 1;
  uint16_t  field_bit_111 : 1;
  uint16_t  field_bits_112_to_127 : 16;
  uint16_t  field_bits_128_to_143 : 16;
  uint16_t  field_bits_144_to_159 : 16;
  uint16_t  field0[2];
} SIL_RESERVED_STRUCT_0015;

typedef struct {
  uint16_t  field_bits_0_to_11 : 12;
  uint16_t  field_bit_12 : 1;
  uint16_t  field_bits_13_to_15 : 3;
  uint16_t  field_bits_16_to_23 : 8;
  uint16_t  field_bits_24_to_25 : 2;
  uint16_t  field_bits_26_to_30 : 5;
  uint16_t  field_bit_31 : 1;
  uint16_t  field_bits_32_to_38 : 7;
  uint16_t  field_bits_39_to_45 : 7;
  uint16_t  field_bits_46_to_47 : 2;
  uint16_t  field_bits_48_to_54 : 7;
  uint16_t  field_bits_55_to_61 : 7;
  uint16_t  field_bits_62_to_63 : 2;
  uint16_t  field_bits_64_to_79 : 16;
  uint16_t  field_bits_80_to_95 : 16;
  uint16_t  field_bits_96_to_99 : 4;
  uint16_t  field_bits_100_to_104 : 5;
  uint16_t  field_bit_105 : 1;
  uint16_t  field_bits_106_to_108 : 3;
  uint16_t  field_bit_109 : 1;
  uint16_t  field_bits_110_to_111 : 2;
  uint16_t  field_bits_112_to_127 : 16;
  uint16_t  field_bits_128_to_143 : 16;
  uint16_t  field_bits_144_to_159 : 16;
  uint16_t  field_bits_160_to_162 : 3;
  uint16_t  field_bits_163_to_164 : 2;
  uint16_t  field_bits_165_to_175 : 11;
  uint16_t  field0;
} SIL_RESERVED_STRUCT_0016;

typedef struct {
  uint16_t  field_bits_0_to_7 : 8;
  uint16_t  field_bits_8_to_10 : 3;
  uint16_t  field_bits_11_to_15 : 5;
  uint16_t  field_bits_16_to_20 : 5;
  uint16_t  field_bits_21_to_29 : 9;
  uint16_t  field_bits_30_to_31 : 2;
  uint16_t  field_bits_32_to_36 : 5;
  uint16_t  field_bits_37_to_45 : 9;
  uint16_t  field_bits_46_to_47 : 2;
  uint16_t  field_bits_48_to_57 : 10;
  uint16_t  field_bits_58_to_63 : 6;
} SIL_RESERVED_STRUCT_0004;

typedef struct {
  uint16_t  field_bits_0_to_2 : 3;
  uint16_t  field_bits_3_to_5 : 3;
  uint16_t  field_bit_6 : 1;
  uint16_t  field_bit_7 : 1;
  uint16_t  field_bit_8 : 1;
  uint16_t  field_bit_9 : 1;
  uint16_t  field_bits_10_to_13 : 4;
  uint16_t  field_bit_14 : 1;
  uint16_t  field_bit_15 : 1;
  uint16_t  field_bits_16_to_23 : 8;
  uint16_t  field_bits_24_to_27 : 4;
  uint16_t  field_bit_28 : 1;
  uint16_t  field_bits_29_to_31 : 3;
  uint16_t  field_bits_32_to_39 : 8;
  uint16_t  field_bits_40_to_47 : 8;
  uint16_t  field0;
} SIL_RESERVED_STRUCT_0024;

typedef struct _RX_CONTEXT_ {
  uint16_t  field_bits_0_to_2 : 3;
  uint16_t  field_bits_3_to_6 : 4;
  uint16_t  field_bits_7_to_8 : 2;
  uint16_t  field_bits_9_to_13 : 5;
  uint16_t  field_bits_14_to_15 : 2;
  uint16_t  field_bits_16_to_18 : 3;
  uint16_t  field_bits_19_to_21 : 3;
  uint16_t  field_bits_22_to_26 : 5;
  uint16_t  field_bits_27_to_28 : 2;
  uint16_t  field_bits_29_to_31 : 3;
  uint16_t  field_bits_32_to_39 : 8;
  uint16_t  field_bits_40_to_47 : 8;
  uint16_t  field_bit_48 : 1;
  uint16_t  field_bit_49 : 1;
  uint16_t  field_bits_50_to_51 : 2;
  uint16_t  field_bits_52_to_59 : 8;
  uint16_t  field_bits_60_to_63 : 4;
  uint16_t  field_bits_64_to_75 : 12;
  uint16_t  field_bits_76_to_79 : 4;
  uint16_t  field_bits_80_to_82 : 3;
  uint16_t  field_bits_83_to_89 : 7;
  uint16_t  field_bit_90 : 1;
  uint16_t  field_bits_91_to_95 : 5;
  uint16_t  field_bits_96_to_98 : 3;
  uint16_t  field_bits_99_to_111 : 13;
  uint16_t  field_bits_112_to_114 : 3;
  uint16_t  field_bits_115_to_117 : 3;
  uint16_t  field_bit_118 : 1;
  uint16_t  field_bits_119_to_121 : 3;
  uint16_t  field_bit_122 : 1;
  uint16_t  field_bit_123 : 1;
  uint16_t  field_bit_124 : 1;
  uint16_t  field_bit_125 : 1;
  uint16_t  field_bits_126_to_127 : 2;
  uint16_t  field_bits_128_to_137 : 10;
  uint16_t  field_bits_138_to_143 : 6;
  uint16_t  field0[3];
} SIL_RESERVED_STRUCT_0022;

typedef struct {
  uint64_t            field0;

  uint64_t            field1;
  SIL_RESERVED_STRUCT_0015    field2;
  SIL_RESERVED_STRUCT_0015    field3;

  uint64_t            field4;
  SIL_RESERVED_STRUCT_0016    field5;
  SIL_RESERVED_STRUCT_0016    field6;

  uint64_t            field7;
  SIL_RESERVED_STRUCT_0004      field8;
  SIL_RESERVED_STRUCT_0004      field9;

  uint64_t            field10;
  SIL_RESERVED_STRUCT_0024       field11;
  SIL_RESERVED_STRUCT_0024       field12;

  uint64_t            field13;
  SIL_RESERVED_STRUCT_0022       field14;
  SIL_RESERVED_STRUCT_0022       field15;
} SIL_RESERVED_STRUCT_0003;

#pragma pack (pop)
