/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxCmn2Brh.c
 * @brief BRH Zen5 core specific transfer table functions
 *
 */

#include <CCX/Zen5/CcxZen5.h>
#include <CcxCmn2Rev.h>
#include "CcxCmn2Brh.h"

/** ------------------------ Table ------------------------------
 * @details This is the internal common-2-Rev transfer table for Zen5
 */
CCX_XFER_TABLE CcxXferZen5Brh = {
  .Header = {
    .Version = CcxZen5
  },
  .SetMiscMsrs = Zen5SetMiscMsrs,
  .GetAmdApicMode = GetAmdApicModeBrh,
  .CalcLocalApic = CalcLocalApicBrh,
  .CalcRMPTableBase = CalculateRmpTableBaseBrh,
  .GetApMsrSyncList = Zen5GetApMsrSyncList,
  .StoreDowncoreCoreCount = Zen5StoreDowncoreCoreCount,
  .CcxPrefetcher = Zen5CcxPrefetcher,
  .CcxInitializePrefetchMode = Zen5InitializePrefetchMode,
  .CcxInitializeC6 = Zen5CcxInitializeC6
};
