/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXFabricWdt.h
 * @brief Data fabric WDT header for DfX
 *
 */

#pragma once

bool DfXGetWdtInfo (
  SIL_CONTEXT  *SilContext,
  uint64_t     *DfCcmTimeout,
  uint16_t     DfGlobalCntlFunc,
  uint16_t     DfGlobalCntlReg
  );
