/**
 * @file FchHelper.h
 * @brief FCH Initialization helper functions prototype
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#pragma once

#include <xSIM.h>

/**********************************************************************************************************************
 * @brief Declare common variables here
 *
 */


/***********************************************************************************************************************
 * @brief Declare function prototypes here
 *
 */

void LibFchIndirectIoRead (uint8_t ByteCount, uint16_t IoBase, uint8_t IndexAddress, uint8_t *Value);
void LibFchIndirectIoWrite (uint8_t ByteCount, uint16_t IoBase, uint8_t IndexAddress, uint8_t *Value);
void FchGetAcpiPmBase (uint16_t *AcpiPmBase);
uint8_t SilFchReadSleepType (void);
void SilFchWriteSleepType (uint16_t SleepType);
void SilFchReadPmio (uint8_t Address, uint8_t ByteCount, uint8_t *Values);
void SilFchWritePmio (uint8_t Address, uint8_t ByteCount, uint8_t *Values);
void SilFchRwPmio(uint8_t Address, uint8_t ByteCount, uint32_t AndMask, uint32_t Ormask);
uint32_t FchGetBootMode (void);
SIL_STATUS FchUpdateBootMode (void);


/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
