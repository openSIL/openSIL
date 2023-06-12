/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfCmn2Rev.h
 * This file defines the types of all functions contained within DF's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once

#include <SilCommon.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

// Common function type definitions of Cmn2Rev functions
// --- Fabric Register Access ---
typedef uint32_t (*DF_FABRIC_REGISTER_ACC_READ) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance
  );

typedef void (*DF_FABRIC_REGISTER_ACC_WRITE) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t Value
  );

// --- Base Fabric Topology ---

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
  uint32_t Socket,
  uint32_t  Die
  );

typedef uint32_t (*DF_DIE_SYSTEM_OFFSET) (
  uint32_t Socket,
  uint32_t Die
  );

typedef const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP * (*DF_DEVICE_MAP_ON_DIE) (
  uint32_t Socket,
  uint32_t Die
  );

typedef uint8_t (*DF_GET_NUMBER_OF_BUS_REGIONS) (
  void
  );

typedef uint32_t (*DF_HOST_BRIDGE_BUS_BASE) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

typedef uint32_t (*DF_HOST_BRIDGE_BUS_LIMIT) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

typedef uint32_t (*DF_PHYS_ROOT_BRIDGE_NUMBER) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

typedef const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP * (*DF_FIND_DEVICE_TYPE_ENTRY_IN_MAP) (
  uint32_t            Socket,
  uint32_t            Die,
  FABRIC_DEVICE_TYPE  Type
  );

typedef const COMPONENT_LOCATION * (*DF_FIND_COMPONENT_LOCATION_MAP) (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  );

typedef SIL_STATUS (*DF_BASE_FABRIC_TOPOLOGY_CONSTRUCTOR) (
  void
  );

typedef SIL_STATUS (*DF_ABSTRACT_REG_ACC) (
  uint8_t                  Field,
  uint32_t                 Socket,
  uint32_t                 OffsetIndex,
  uint32_t                 InstanceId,
  uint8_t                  Die,
  uint8_t                  Rw,
  uint32_t                 WriteValue,
  uint32_t                 *ReadValue
  );

typedef const uint32_t * (*DF_GET_GLBL_CTRL_INSTANCE_IDS) (
  void
  );

typedef void (*DF_GET_NPS_MAP_DATA) (
  uint32_t *Nps0,
  uint32_t *Nps1,
  uint32_t *Nps2,
  uint32_t *Nps4
  );

typedef uint32_t (*DF_GET_PHYS_DOMAIN_OF_CCD) (
  uint32_t             Socket,
  uint32_t             PhysCcd,
  uint32_t             NumberOfPhysicalDomains,
  void *DomainInfo
  );

typedef void (*DF_BUILD_CCD_INFO) (
  uint32_t             SocketCount,
  uint32_t             DieCount,
  void *DomainInfo
  );

typedef bool (*DF_HAS_FCH) (
  uint32_t             Socket,
  uint32_t             Die,
  uint32_t             Index
  );

typedef bool (*DF_HAS_SMU) (
  uint32_t             Socket,
  uint32_t             Die,
  uint32_t             Index
  );

typedef bool (*DF_GET_SYS_COMPONENT_RB_LOC) (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION *Location
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
  DF_GET_NUMBER_OF_BUS_REGIONS          DfGetNumberOfBusRegions;
  DF_HOST_BRIDGE_BUS_BASE               DfGetHostBridgeBusBase;
  DF_HOST_BRIDGE_BUS_LIMIT              DfGetHostBridgeBusLimit;
  DF_PHYS_ROOT_BRIDGE_NUMBER            DfGetPhysRootBridgeNumber;
  DF_FIND_DEVICE_TYPE_ENTRY_IN_MAP      DfFindDeviceTypeEntryInMap;
  DF_ABSTRACT_REG_ACC                   DfAbstractRegAcc;
  DF_GET_GLBL_CTRL_INSTANCE_IDS         DfGetGlblCtrlInstanceIds;
  DF_GET_NPS_MAP_DATA                   DfGetNpsMapData;
  DF_GET_PHYS_DOMAIN_OF_CCD             DfGetPhysDomainOfCcd;
  DF_BUILD_CCD_INFO                     DfBuildCcdInfo;
  DF_GET_SYS_COMPONENT_RB_LOC           DfGetSystemComponentRootBridgeLocation;
  DF_HAS_FCH                            DfHasFch;
  DF_HAS_SMU                            DfHasSmu;
} DF_COMMON_2_REV_XFER_BLOCK;
