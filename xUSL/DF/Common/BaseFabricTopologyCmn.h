/**
 * @file  BaseFabricTopologyCmn.h
 * @brief Common declarations of BaseFabricTopology functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <DF/Common/SilBaseFabricTopologyLib.h>

uint32_t DfGetNumberOfDiesOnSocket (void);
uint32_t
DfGetHostBridgeSystemFabricID (
  uint32_t Socket,
  uint32_t Index
  );
bool
DfGetSystemComponentRootBridgeLocation (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION* Location
  );
bool
DfHasFch (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );
bool
DfHasSmu (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );
const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
DfFindDeviceTypeEntryInMap (
  FABRIC_DEVICE_TYPE  Type
  );
SIL_STATUS DfGetRootBridgeInfo (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index,
  uint32_t *SystemFabricID,
  uint32_t *BusNumberBase,
  uint32_t *BusNumberLimit,
  uint32_t *PhysicalRootBridgeNumber,
  bool *HasFchDevice,
  bool *HasSystemMgmtUnit
  );
SIL_STATUS DfGetDieInfo (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfRootBridges,
  uint32_t *SystemIdOffset,
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP **FabricIdMap
  );
SIL_STATUS DfGetProcessorInfo (
  uint32_t SocketIndex,
  uint32_t *DieCount,
  uint32_t *RootBridgeCount
  );
SIL_STATUS DfGetSystemInfo (
  uint32_t *NumberOfInstalledProcessors,
  uint32_t *TotalNumberOfDie,
  uint32_t *TotalNumberOfRootBridges,
  ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  );
