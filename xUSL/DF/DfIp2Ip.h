/**
 * @file  DfIp2Ip.h
 * This file contains the definition of the Data Fabric Ip 2 Ip API.
 * This API provides other openSIL IPs with any DF functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>

// Common function type definitions for functions in DF's Ip2Ip API
// Program specific code
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

typedef bool (*DF_GET_WDT_INFO) (
  uint64_t *DfCcmTimeout,
  uint16_t  DfGlobalCntlFunc,
  uint16_t  DfGlobalCntlReg
  );

// From common code
typedef uint32_t (*DF_NUMBER_OF_DIES_ON_SOCKET) (
  void
  );

typedef uint32_t (*DF_GET_HOST_BRIDGE_SYSTEM_FABRIC_ID) (
  uint32_t Socket,
  uint32_t Index
  );

typedef bool (*DF_GET_SYSTEM_COMPONENT_ROOT_BRIDGE_LOCATION) (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION* Location
  );

typedef bool (*DF_HAS_FCH) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

typedef bool (*DF_HAS_SMU) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

typedef const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* (*DF_FIND_DEVICE_TYPE_ENTRY_IN_MAP) (
  FABRIC_DEVICE_TYPE  Type
  );

typedef SIL_STATUS (*DF_GET_ROOT_BRIDGE_INFO) (
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

typedef SIL_STATUS (*DF_GET_DIE_INFO) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfRootBridges,
  uint32_t *SystemIdOffset,
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP **FabricIdMap
  );

typedef SIL_STATUS (*DF_GET_PROCESSOR_INFO) (
  uint32_t SocketIndex,
  uint32_t *DieCount,
  uint32_t *RootBridgeCount
  );

typedef SIL_STATUS (*DF_GET_SYSTEM_INFO) (
  uint32_t *NumberOfInstalledProcessors,
  uint32_t *TotalNumberOfDie,
  uint32_t *TotalNumberOfRootBridges,
  ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  );

typedef SIL_STATUS (*DF_GET_DOMAINXLAT_INFO) (
  uint32_t   Socket,
  uint32_t   Die,
  uint32_t   Ccd,
  uint32_t   Ccx,
  uint32_t   *Domain
  );

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  DF_NUMBER_OF_PROCESSORS_PRESENT        DfGetNumberOfProcessorsPresent;
  DF_NUMBER_OF_SYSTEM_DIES               DfGetNumberOfSystemDies;
  DF_NUMBER_OF_SYSTEM_ROOT_BRIDGES       DfGetNumberOfSystemRootBridges;
  DF_NUMBER_OF_ROOT_BRIDGES_ON_SOCKET    DfGetNumberOfRootBridgesOnSocket;
  DF_NUMBER_OF_ROOT_BRIDGES_ON_DIE       DfGetNumberOfRootBridgesOnDie;
  DF_DIE_SYSTEM_OFFSET                   DfGetDieSystemOffset;
  DF_DEVICE_MAP_ON_DIE                   DfGetDeviceMapOnDie;
  DF_HOST_BRIDGE_BUS_BASE                DfGetHostBridgeBusBase;
  DF_HOST_BRIDGE_BUS_LIMIT               DfGetHostBridgeBusLimit;
  DF_PHYS_ROOT_BRIDGE_NUMBER             DfGetPhysRootBridgeNumber;
  DF_NUMBER_OF_DIES_ON_SOCKET            DfGetNumberOfDiesOnSocket;
  DF_GET_HOST_BRIDGE_SYSTEM_FABRIC_ID    DfGetHostBridgeSystemFabricID;
  DF_GET_SYSTEM_COMPONENT_ROOT_BRIDGE_LOCATION DfGetSystemComponentRootBridgeLocation;
  DF_HAS_FCH                             DfHasFch;
  DF_HAS_SMU                             DfHasSmu;
  DF_FIND_DEVICE_TYPE_ENTRY_IN_MAP       DfFindDeviceTypeEntryInMap;
  DF_FABRIC_REGISTER_ACC_READ            DfFabricRegisterAccRead;
  DF_FABRIC_REGISTER_ACC_WRITE           DfFabricRegisterAccWrite;
  DF_GET_WDT_INFO                        DfGetWdtInfo;
  DF_GET_ROOT_BRIDGE_INFO                DfGetRootBridgeInfo;
  DF_GET_DIE_INFO                        DfGetDieInfo;
  DF_GET_PROCESSOR_INFO                  DfGetProcessorInfo;
  DF_GET_SYSTEM_INFO                     DfGetSystemInfo;
  DF_GET_DOMAINXLAT_INFO                 DfDomainXlat;
} DF_IP2IP_API;
