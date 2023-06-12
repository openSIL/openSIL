
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file CcxCmn2Zen4.h
 * @brief Zen4 core specific headers and function declarations
 *
 */

#pragma once


/**********************************************************************************************************************
 * Declare Function prototypes
 *
 */

SIL_STATUS InitializeCcxZen4 (void);
SIL_STATUS InitializeApiZen4 (void);

/**
 * Zen4CalcLocalApic
 *
 * @brief   Returns the APIC Id based on the provided inputs
 *
 * @param[in]  Socket     Socket # of APIC Id to calculate
 * @param[in]  Die        Die # of APIC Id to calculate
 * @param[in]  Ccd        CCD # of APIC Id to calculate
 * @param[in]  Complex    Complex # of APIC Id to calculate
 * @param[in]  Core       Core # of APIC Id to calculate
 * @param[in]  Thread     Thread # of APIC Id to calculate
 *
 * @retval     uint32_t   Calculate Local APIC Id
 *
 */
uint32_t
Zen4CalcLocalApic (
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    Ccd,
  uint32_t    Complex,
  uint32_t    Core,
  uint32_t    Thread
  );

/**
 * Zen4CalculateRMPTableBase
 *
 * @brief   Calculate RMP Table Base address using Socket boundary.
 *
 * @param[in]  RmpTableSize        RMP table size
 * @param[in]  RmpTableBase        RMP table Base
 * @param[in]  SnpMemSizeToCover
 *
 */
void Zen4CalculateRMPTableBase (
  uint64_t    *RmpTableSize,
  uint64_t    *RmpTableBase,
  uint64_t    *SnpMemSizeToCover,
  uint8_t     SplitRmpTableValue
  );
