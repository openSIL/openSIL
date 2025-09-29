/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxCmn2Phx.c
 * @brief core specific transfer table functions
 *
 */

#include <SilCommon.h>
#include <CcxClass-api.h>
#include <CCX/Zen4/CcxZen4.h>
#include <CCX/Zen4/Zen4DownCoreInit.h>
#include <CCX/Common/CcxCmn2Rev.h>
#include "CcxCmn2Phx.h"

CCX_XFER_TABLE CcxXferZen4Phx = {
  .Header = {
    .Version = CcxZen4
  },
  .SetMiscMsrs = Zen4SetMiscMsrs,
  .GetAmdApicMode = GetAmdApicModePhx,
  .CalcLocalApic = CalcLocalApicPhx,
  .CalcRMPTableBase = CalculateRmpTableBasePhx,
  .GetApMsrSyncList = Zen4GetApMsrSyncList,
  .StoreDowncoreCoreCount = Zen4StoreDowncoreCoreCount,
  .CcxPrefetcher = Zen4CcxPrefetcher,
  .CcxInitializePrefetchMode = Zen4InitializePrefetchMode,
  .CcxInitializeC6 = Zen4CcxInitializeC6,
  .CcxLaunchThread = CcxLaunchThreadPhx,
  .CcxCacheInit = CcxCacheInitPhx,
  .SetBrandString = SetBrandStringPhx,
  .IsS3Supported = IsS3SupportedPhx,
  .IsGameModeSupported = IsGameModeSupportedPhx,
  .IsOneCcdModeSupported = IsOneCcdModeSupportedPhx
};
