/**
 * @file  DfXBaseFabricTopology.h
 * @brief DfX specific declarations of BaseFabricTopology functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

uint32_t DfXGetNumberOfProcessorsPresent (void);
uint32_t DfXGetNumberOfSystemDies (void);
uint32_t DfXGetNumberOfSystemRootBridges (void);
uint32_t
DfXGetNumberOfRootBridgesOnSocket (
  uint32_t Socket
  );
uint32_t
DfXGetNumberOfRootBridgesOnDie (
  uint32_t Socket
  );
uint32_t
DfXGetDieSystemOffset (
  uint32_t Socket
  );
const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP*
DfXGetDeviceMapOnDie (
  void
  );
uint32_t
DfXGetHostBridgeBusBase (
  uint32_t Socket,
  uint32_t Index
  );
uint32_t
DfXGetHostBridgeBusLimit (
  uint32_t Socket,
  uint32_t Index
  );
uint32_t
DfXGetPhysRootBridgeNumber (
  uint32_t Index
  );
const
COMPONENT_LOCATION*
DfXFindComponentLocationMap (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  );
SIL_STATUS DfXBaseFabricTopologyConstructor (void);
