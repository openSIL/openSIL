/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  Io.h
 * @brief OpenSIL IO access function prototypes
 *
 *
 */

#pragma once


NASM_ABI void xUSLIoWrite8 (uint16_t Port, uint8_t Value);
NASM_ABI void xUSLIoWrite16 (uint16_t Port, uint16_t Value);
NASM_ABI void xUSLIoWrite32 (uint16_t Port, uint32_t Value);
NASM_ABI uint8_t xUSLIoRead8 (uint16_t Port);
NASM_ABI uint16_t xUSLIoRead16 (uint16_t Port);
NASM_ABI uint32_t xUSLIoRead32 (uint16_t Port);
NASM_ABI void xUSLIoReadModifyWrite8 (uint16_t Port, uint8_t AndMask, uint8_t OrMask);
NASM_ABI void xUSLIoReadModifyWrite16 (uint16_t Port, uint16_t AndMask, uint16_t OrMask);
NASM_ABI void xUSLIoReadModifyWrite32 (uint16_t Port, uint32_t AndMask, uint32_t OrMask);
