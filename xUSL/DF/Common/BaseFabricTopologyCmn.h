/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BaseFabricTopologyCmn.h
 * @brief Common declarations of BaseFabricTopology functions
 */

#pragma once

#include <DF/Common/SilBaseFabricTopologyLib.h>

uint32_t
DfGetNumberOfPciSegments (
  SIL_CONTEXT    *SilContext
  );
uint32_t
DfGetNumberOfDiesOnSocket (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket
  );
uint32_t
DfGetHostBridgeSystemFabricID (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       Index
  );
SIL_STATUS
DfGetRootBridgeInfo (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Index,
  uint32_t     *SystemFabricID,
  uint32_t     *BusNumberBase,
  uint32_t     *BusNumberLimit,
  uint32_t     *PhysicalRootBridgeNumber,
  bool         *HasFchDevice,
  bool         *HasSystemMgmtUnit
  );
SIL_STATUS
DfGetDieInfo (
  SIL_CONTEXT        *SilContext,
  uint32_t           Socket,
  uint32_t           Die,
  uint32_t           *NumberOfRootBridges,
  uint32_t           *SystemIdOffset,
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP **FabricIdMap
  );
SIL_STATUS
DfGetProcessorInfo (
  SIL_CONTEXT        *SilContext,
  uint32_t           SocketIndex,
  uint32_t           *DieCount,
  uint32_t           *RootBridgeCount
  );
SIL_STATUS
DfGetSystemInfo (
  SIL_CONTEXT          *SilContext,
  uint32_t             *NumberOfInstalledProcessors,
  uint32_t             *TotalNumberOfDie,
  uint32_t             *TotalNumberOfRootBridges,
  ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  );
