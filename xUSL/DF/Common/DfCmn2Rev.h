/**
 * @file  DfCmn2Rev.h
 * This file defines the types of all functions contained within DF's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

// Common function type definitions of Cmn2Rev funtions
// --- Fabric Register Access ---
typedef uint32_t (*DF_FABRIC_REGISTER_ACC_READ) (
  uint32_t Socket,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance
  );

typedef void (*DF_FABRIC_REGISTER_ACC_WRITE) (
  uint32_t Socket,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t Value
  );

typedef uint32_t (*DF_NUMBER_OF_PROCESSORS_PRESENT) (
  void
  );

typedef uint32_t (*DF_NUMBER_OF_SYSTEM_DIES) (
  void
  );

typedef uint32_t (*DF_NUMBER_OF_SYSTEM_ROOT_BRIDGES) (
  void
  );

typedef uint32_t (*DF_NUMBER_OF_ROOT_BRIDGES_ON_SOCKET) (
  uint32_t Socket
  );

typedef uint32_t (*DF_NUMBER_OF_ROOT_BRIDGES_ON_DIE) (
  uint32_t Socket
  );

typedef uint32_t (*DF_DIE_SYSTEM_OFFSET) (
  uint32_t Socket
  );

typedef const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* (*DF_DEVICE_MAP_ON_DIE) (
  void
  );

typedef uint32_t (*DF_HOST_BRIDGE_BUS_BASE) (
  uint32_t Socket,
  uint32_t Index
  );

typedef uint32_t (*DF_HOST_BRIDGE_BUS_LIMIT) (
  uint32_t Socket,
  uint32_t Index
  );

typedef uint32_t (*DF_PHYS_ROOT_BRIDGE_NUMBER) (
  uint32_t Index
  );

typedef const COMPONENT_LOCATION* (*DF_FIND_COMPONENT_LOCATION_MAP) (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  );

typedef SIL_STATUS (*DF_BASE_FABRIC_TOPOLOGY_CONSTRUCTOR) (
  void
  );

typedef SIL_STATUS (*DF_BUILD_DOMAIN_INFO) (
  void
  );

// Define the Cmn2Rev xfer table containing pointers to these functions
typedef struct {
  DF_FABRIC_REGISTER_ACC_READ           DfFabricRegisterAccRead;
  DF_FABRIC_REGISTER_ACC_WRITE          DfFabricRegisterAccWrite;
  DF_NUMBER_OF_PROCESSORS_PRESENT       DfGetNumberOfProcessorsPresent;
  DF_NUMBER_OF_SYSTEM_DIES              DfGetNumberOfSystemDies;
  DF_NUMBER_OF_SYSTEM_ROOT_BRIDGES      DfGetNumberOfSystemRootBridges;
  DF_NUMBER_OF_ROOT_BRIDGES_ON_SOCKET   DfGetNumberOfRootBridgesOnSocket;
  DF_NUMBER_OF_ROOT_BRIDGES_ON_DIE      DfGetNumberOfRootBridgesOnDie;
  DF_DIE_SYSTEM_OFFSET                  DfGetDieSystemOffset;
  DF_DEVICE_MAP_ON_DIE                  DfGetDeviceMapOnDie;
  DF_HOST_BRIDGE_BUS_BASE               DfGetHostBridgeBusBase;
  DF_HOST_BRIDGE_BUS_LIMIT              DfGetHostBridgeBusLimit;
  DF_PHYS_ROOT_BRIDGE_NUMBER            DfGetPhysRootBridgeNumber;
  DF_FIND_COMPONENT_LOCATION_MAP        DfFindComponentLocationMap;
  DF_BASE_FABRIC_TOPOLOGY_CONSTRUCTOR   DfBaseFabricTopologyConstructor;
  DF_BUILD_DOMAIN_INFO                  DfBuildDomainInfo;
} DF_COMMON_2_REV_XFER_BLOCK;
