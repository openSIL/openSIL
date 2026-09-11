/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFabric.h
 * @brief Fabric topology xPRF services.
 *
 * @details Exposes four fabric topology queries sourced from openSIL's DF
 *          Ip-2-Ip API.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <Sil-api.h>
#include <xPrfFabricTypes.h>

SIL_STATUS
xPrfFabricGetSystemInfo (
  uint32_t                   *NumberOfInstalledProcessors,
  uint32_t                   *TotalNumberOfDie,
  uint32_t                   *TotalNumberOfRootBridges,
  XPRF_ROOT_BRIDGE_LOCATION   *SystemFchRootBridgeLocation,
  XPRF_ROOT_BRIDGE_LOCATION   *SystemSmuRootBridgeLocation
  );

SIL_STATUS
xPrfFabricGetProcessorInfo (
  uint32_t  Socket,
  uint32_t  *DieCount,
  uint32_t  *RootBridgeCount
  );

SIL_STATUS
xPrfFabricGetDieInfo (
  uint32_t                       Socket,
  uint32_t                       Die,
  uint32_t                       *RootBridgeCount,
  uint32_t                       *SystemIdOffset,
  const XPRF_FABRIC_DEVICE_MAP  **FabricIdMap
  );

SIL_STATUS
xPrfFabricGetRootBridgeInfo (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Index,
  uint32_t  *SystemFabricID,
  uint32_t  *BusNumberBase,
  uint32_t  *BusNumberLimit,
  uint32_t  *PhysicalRootBridgeNumber,
  bool      *HasFchDevice,
  bool      *HasSystemMgmtUnit
  );
