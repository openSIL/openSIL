/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfClassDflts.c
 * @brief DF Class IP input defaults
 *
 * This file holds the platform default values for the DF class module.
 */

#include <DF/DfClass-api.h>

const DFCLASS_INPUT_BLK mDfClassDflts = {
  // Input block vars/values shared with the Host.
  // This becomes part of the IP API for the Host.
  .AmdFabricWdtCfg = 0xFF,
  .AmdFabricWdtCntSel = 0xFF,
  .AmdFabricImmSyncFloodOnFatalErrCtrl = true,
  .AmdFabricCcxAsNumaDomain = false,
  .AmdPciExpressBaseAddress = 0xE0000000,
  .AmdLongModePageTables = 0,
};
