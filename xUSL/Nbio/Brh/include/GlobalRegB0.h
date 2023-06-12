/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file GlobalRegB0.h
 *  @brief This file contains Iohc Register definitions
 */

#pragma once

typedef union {
  struct {
    uint32_t                                         field_bits_0_to_3:4;
    uint32_t                      field_bit_4:1;
    uint32_t                      field_bit_5:1;
    uint32_t                                        field_bits_6_to_8:3;
    uint32_t                           field_bit_9:1;
    uint32_t                            field_bit_10:1;
    uint32_t                             field_bit_11:1;
    uint32_t                         field_bit_12:1;
    uint32_t                                      field_bits_13_to_14:2;
    uint32_t                       field_bit_15:1;
    uint32_t                                 field_bit_16:1;
    uint32_t                    field_bit_17:1;
    uint32_t   field_bit_18:1;
    uint32_t                          field_bit_19:1;
    uint32_t                                  field_bits_20_to_22:3;
    uint32_t                              field_bit_23:1;
    uint32_t                                      field_bit_24:1;
    uint32_t                                field_bit_25:1;
    uint32_t                                   field_bits_26_to_28:3;
    uint32_t                                  field_bit_29:1;
    uint32_t                            field_bit_30:1;
    uint32_t                                      field_bit_31:1;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0035;
#define SIL_RSVD_ADDR_1A38018C  0x1a38018cUL
#define SIL_RESERVED_0509       0x1a340044UL

#define SIL_RESERVED_0498  31
#define SIL_RESERVED_0497  0x80000000

#define SIL_RESERVED_0508  0x1a34046cUL

#define SIL_RESERVED_0500  13
#define SIL_RESERVED_0499  0x2000

#define SIL_RESERVED_0502  10
#define SIL_RESERVED_0501  0x7c00

#define SIL_RESERVED_0504  8
#define SIL_RESERVED_0503  0x100

#define SIL_RESERVED_0510  0x1a3400bcUL

#define SIL_RESERVED_0483  0
#define SIL_RESERVED_0482  0xffff

#define SIL_RESERVED_0485  16
#define SIL_RESERVED_0484  0xffff0000

#define SIL_RESERVED_0505  0x1a340254UL

#define SIL_RESERVED_0487  0
#define SIL_RESERVED_0486  0xfff

#define SIL_RESERVED_0488  16
#define BITS_16_TO_27_MASK_0002  0xfff0000

#define SIL_RESERVED_0506  0x1a340258UL

#define SIL_RESERVED_0496  0
#define SIL_RESERVED_0495  0xf

#define SIL_RESERVED_0490  4
#define SIL_RESERVED_0489  0xf0

#define SIL_RESERVED_0492  8
#define SIL_RESERVED_0491  0xf00

#define SIL_RESERVED_0494  13
#define SIL_RESERVED_0493  0x2000

#define SIL_RESERVED_0507  0x1a34025cUL
