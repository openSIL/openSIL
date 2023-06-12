/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlRegBrh.h
 * @brief Defines CXL Brh specific used registers
 *
 */

#pragma once

#ifndef CNLI_SPACE
  #define CNLI_SPACE(HANDLE, ENGINE, ADDRESS)  \
          (ADDRESS + ((HANDLE->RBIndex & 3) << 16) + (ENGINE->Type.Port.PortId << 13))
#endif

#define SIL_RESERVED_0010      5
#define CnliCxlCorrErrMask0_Reserved_31_7_MASK        0xffffff80

#define SIL_RSVD_ADDR_1F100064          0x1f100070UL

#define BITS_17_TO_31_MASK_0001         0xfffe0000

#define SIL_RESERVED_0011           10

#define SMN_CnliCxlUncorrErrMask0_ADDRESS          0x1f100064UL
