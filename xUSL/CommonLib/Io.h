/**
 * @file  Io.h
 * @brief OpenSIL IO access function prototypes
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once


void xUSLIoWrite8(uint16_t Port, uint8_t Value);
void xUSLIoWrite16(uint16_t Port, uint16_t Value);
void xUSLIoWrite32(uint16_t Port, uint32_t Value);
uint8_t xUSLIoRead8(uint16_t Port);
uint16_t xUSLIoRead16(uint16_t Port);
uint32_t xUSLIoRead32(uint16_t Port);
void xUSLIoReadModifyWrite8(uint16_t Port, uint8_t AndMask, uint8_t OrMask);
void xUSLIoReadModifyWrite16(uint16_t Port, uint16_t AndMask, uint16_t OrMask);
void xUSLIoReadModifyWrite32(uint16_t Port, uint32_t AndMask, uint32_t OrMask);
