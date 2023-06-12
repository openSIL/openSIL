/**
 * @file  DfXFabricWdt.h
 * @brief OpenSIL Fabric WDT header for DfX.
 *
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

bool DfXGetWdtInfo (
  uint64_t *DfCcmTimeout,
  uint16_t DfGlobalCntlFunc,
  uint16_t DfGlobalCntlReg
  );

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
