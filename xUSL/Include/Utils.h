/**
 * @file  Utils.h
 * @brief OpenSIL utilities header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#pragma once


/**********************************************************************************************************************
 * @brief Declare common variables here
 *
 */

/**
 * @brief Declare function prototypes here
 *
 */
void
xUslDumpBuffer (
  uint8_t     *Buffer,
  uint32_t    Size
  );

int xUslLowBitSet32 (uint32_t Operand);
int xUslHighBitSet32 (uint32_t Operand);
uint8_t xUslBitWidth (size_t Number);
uint32_t xUslGetSetBitCount (uint32_t Value);

/**********************************************************************************************************************
 * @brief Declare common macros here
 *
 */
