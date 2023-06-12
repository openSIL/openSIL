/**
 * @file  MpioPcieStraps.h
 * @brief This file contains prototypes for OpenSIL MPIO PCIe Read Write straps
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define SIL_RESERVED_881                 (HOLD_TRAINING_B_INDEX - SIL_RESERVED_834)

#define PORT_STRAP_INDEX(Strap, Port)    (Strap + (Port * SIL_RESERVED_881))

#define SIL_RESERVED_833    0x1000
#define SIL_RESERVED_832    0xFFFE0000

void
WritePcieStrap (
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  );

#define SIL_RESERVED_838       0x6
#define SIL_RESERVED_893       0xe
#define SIL_RESERVED_844       0x13
#define SIL_RESERVED_870       0x15
#define SIL_RESERVED_873       0x17
#define SIL_RESERVED_868       0x21
#define SIL_RESERVED_849       0x4c
#define SIL_RESERVED_848       0x4d
#define SIL_RESERVED_841       0x5f
#define SIL_RESERVED_869       0x61
#define SIL_RESERVED_886       0x63
#define SIL_RESERVED_836       0x77
#define SIL_RESERVED_837       0x7d
#define SIL_RESERVED_872       0x89
#define SIL_RESERVED_883       0xa3
#define SIL_RESERVED_842       0xbb
#define SIL_RESERVED_885       0xc3
#define SIL_RESERVED_884       0xc4
#define SIL_RESERVED_876       0xc5
#define SIL_RESERVED_878       0xc6
#define SIL_RESERVED_877       0xc7
#define SIL_RESERVED_879       0xc8
#define SIL_RESERVED_851       0xc9
#define SIL_RESERVED_853       0xca
#define SIL_RESERVED_852       0xcb
#define SIL_RESERVED_854       0xcc
#define SIL_RESERVED_887       0xe6
#define SIL_RESERVED_889       0xe7
#define SIL_RESERVED_890       0xe8
#define SIL_RESERVED_888       0xf2
#define SIL_RESERVED_834       0xfd
#define SIL_RESERVED_835       0x1
#define SIL_RESERVED_855       0x112
#define SIL_RESERVED_880       0x11d
#define SIL_RESERVED_850       0x11f
#define SIL_RESERVED_843       0x127
#define SIL_RESERVED_859       0x128
#define SIL_RESERVED_862       0x12d
#define SIL_RESERVED_863       0x12e
#define SIL_RESERVED_864       0x12f
#define SIL_RESERVED_861       0x130
#define SIL_RESERVED_866       0x132
#define SIL_RESERVED_865       0x133
#define SIL_RESERVED_860       0x139
#define SIL_RESERVED_871       0x157
#define SIL_RESERVED_847       0x15a
#define SIL_RESERVED_894       0x15d
#define SIL_RESERVED_895       0x15e
#define SIL_RESERVED_891       0x15f
#define SIL_RESERVED_892       0x160
#define SIL_RESERVED_840       0x169
#define SIL_RESERVED_875       0x174
#define SIL_RESERVED_839       0x177
#define SIL_RESERVED_867       0x17f
#define SIL_RESERVED_845       0x180
#define SIL_RESERVED_882       0x183
#define SIL_RESERVED_874       0x184
#define SIL_RESERVED_846       0x186
#define SIL_RESERVED_856       0x189
#define SIL_RESERVED_770       0x18c
#define SIL_RESERVED_857       0x198
#define SIL_RESERVED_858       0x19e
#define HOLD_TRAINING_B_INDEX  0x1a0
