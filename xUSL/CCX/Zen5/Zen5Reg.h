/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Zen5Reg.h
 * @brief Zen5 core specific register definitions
 *
 */

#define SIL_RESERVED_0004           0xC001041Bul

typedef union {
  struct {
    uint64_t field_bits_0_to_11:12;
    uint64_t :2;
    uint64_t field_bits_14_to_21:8;
    uint64_t field_bits_22_to_29:8;
    uint64_t field_bits_30_to_31:2;
    uint64_t field_bit_32:1;
    uint64_t :30;
    uint64_t field_bit_63:1;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0039;

typedef union {
  struct {
    uint64_t field_bits_0_to_11:12;
    uint64_t :2;
    uint64_t field_bits_14_to_21:8;
    uint64_t field_bits_22_to_24:3;
    uint64_t field_bit_25:1;
    uint64_t :6;
    uint64_t field_bits_32_to_41:10;
    uint64_t :22;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0020;

typedef union {
  struct {
    uint64_t field_bits_0_to_2:3;
    uint64_t field_bits_3_to_5:3;
    uint64_t field_bit_6:1;
    uint64_t field_bits_7_to_14:8;
    uint64_t field_bits_15_to_20:6;
    uint64_t field_bits_21_to_25:5;
    uint64_t field_bits_26_to_28:3;
    uint64_t field_bit_29:1;
    uint64_t field_bit_30:1;
    uint64_t :1;
    uint64_t field_bit_32:1;
    uint64_t :31;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0037;
