/**
 * @file    RasDefs.h
 * @details This file contains Structures and definitions used to define the RAS internal and ip-to-ip APIs
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <RAS/Common/RasClass-api.h>
#include <RAS/Common/RasReg-api.h>
#include <RAS/Common/RasReg.h>

/**
 * @brief DRAM device width x4
 *
 * @details DRAM device width refers to the number of bits sourced simultaneously from a single memory chip.
 */
#define DEVICE_WIDTH_X4                 (4)

/**
 * @brief DRAM device width x8
 *
 * @details DRAM device width refers to the number of bits sourced simultaneously from a single memory chip.
 */
#define DEVICE_WIDTH_X8                 (8)

/**
 * @brief ECC symbol size x4
 *
 * @details DRAM is protected by an error correcting code (ECC). The memory controller supports multiple error
 *          correcting codes. Each error correcting code features an ECC word formed by a symbol based code.
 *          The x4 code uses thirty-six 4-bit symbols to make a 144-bit ECC word made up of 128 data bits
 *          and 16 check bits.
 */
#define ECC_SYMBOL_SIZE_X4              (0)
#define SIL_RESERVED_787                (1)
/**
 * @brief ECC symbol size x16
 *
 * @details DRAM is protected by an error correcting code (ECC). The memory controller supports multiple error
 *          correcting codes. Each error correcting code features an ECC word formed by a symbol based code.
 *          The x16 code uses eighteen 16-bit symbols to make a 288-bit ECC word made up of 256 data bits
 *          and 32 check bits.
 */
#define ECC_SYMBOL_SIZE_X16             (2)
/**
 * @brief ECC Bit interleaving disabled
 *
 * @details Bit interleaving refers to the way bits from the two memory beats are organized to form an ECC word.
 *          When specified, even and odd bits from multiple beats can be interleaved to create an ECC word.
 *          When bit interleaving is not enabled, a partially failing device contributes failures to two
 *          different symbols.
 */
#define ECC_BIT_INTERLEAVING_DISABLED   (0)
/**
 * @brief ECC Bit interleaving enabled
 *
 * @details Bit interleaving refers to the way bits from the two memory beats are organized to form an ECC word.
 *          When specified, even and odd bits from multiple beats can be interleaved to create an ECC word.
 *          When bit interleaving is enabled, a partially failing device (e.g., pin failure) contributes
 *          two incorrect bits to the same symbol.
 */
#define ECC_BIT_INTERLEAVING_ENABLED    (1)

/**
 * The number of Core MCA Bank
 *
 * The Core MCA banks (7 banks in total) are present in the address space of every logical core.
 */
#define MAX_CORE_MCA_BANK_COUNT (7)

#define LS_THR0_SMNADDR_BYTE1       0xAE
#define IF_THR0_SMNADDR_BYTE1       0xAA
#define L2_THR0_SMNADDR_BYTE1       0x8A
#define DE_THR0_SMNADDR_BYTE1       0x96
#define EX_THR0_SMNADDR_BYTE1       0x9A
#define FP_THR0_SMNADDR_BYTE1       0x86
