/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  SilBaseFabricTopologyLib.h
 * @brief OpenSIL DataFabric Topology functions header.
 *
 */

#pragma once

#include "SilSocLogicalId.h"
#include <Utils.h>
#include <CommonLib/SmnAccess.h>
#include <BaseSocLogicalIdXlat.h>
#include <CommonLib/CpuLib.h>
#include <ProjSocConst.h>
#include <xPrfFabricTypes.h>

#pragma pack (push, 1)

#define SIL_RESERVED_0020  2

#define MAX_PCI_BUS_NUMBER_PER_SEGMENT         0x100
#define MAX_NUMBER_OF_DEVICE_IDS               0x100
#define SIL_MAX_UINT32                         ((uint32_t)0xFFFFFFFF)

#define DF_GET_SEGMENT(a)                      (a / MAX_PCI_BUS_NUMBER_PER_SEGMENT)
#define DF_GET_BUS(a)                          (a % MAX_PCI_BUS_NUMBER_PER_SEGMENT)

/// Root Bridge location
typedef XPRF_ROOT_BRIDGE_LOCATION ROOT_BRIDGE_LOCATION;

/// Fabric Device Types
typedef enum {
  PrimaryFch,                  ///< Primary FCH
  SlaveFch,                    ///< Slave FCH
  FchTypeMax,                  ///< Not a valid entry type, use for limit checking.
  PrimarySmu,                  ///< Primary SMU
  SlaveSmu,                    ///< Slave SMU
  SmuTypeMax,                  ///< Not a valid entry type, use for limit checking.
  ComponentTypeMax             ///< Not a valid entry type, use for limit checking.
} COMPONENT_TYPE;

/// Component location
typedef struct {
  uint32_t             Socket;
  uint32_t             Die;
  uint32_t             IomsFabricId;
  COMPONENT_TYPE       Type;
} COMPONENT_LOCATION;

/// Fabric Device Types
typedef enum {
  Ccm = XprfFabricCcm,
  Gcm = XprfFabricGcm,
  Ncs = XprfFabricNcs,
  Ncm = XprfFabricNcm,
  Pie = XprfFabricPie,
  Ioms = XprfFabricIoms,
  Cs = XprfFabricCs,
  Tcdx = XprfFabricTcdx,
  Cake = XprfFabricCake,
  CsUmc = XprfFabricCsUmc,
  CsCcix = XprfFabricCsCcix,
  CsCmp = XprfFabricCsCmp,
  Acm = XprfFabricAcm,
  Iom = XprfFabricIom,
  Ios = XprfFabricIos,
  Icng = XprfFabricIcng,
  Cnli = XprfFabricCnli,
  Pfx = XprfFabricPfx,
  Spf = XprfFabricSpf,
  NcmIommu = XprfFabricNcmIommu,
  Giom = XprfFabricGiom,
  Htdm = XprfFabricHtdm,
  Htds = XprfFabricHtds,
  Xgmi = XprfFabricXgmi,
  FabricDeviceTypeMax = XprfFabricDeviceTypeMax
} FABRIC_DEVICE_TYPE;


/// Structure for the device map to be stored to the heap
typedef struct {
  FABRIC_DEVICE_TYPE     Type;             ///< The instance type of the fabric device
  uint32_t               Count;            ///< The count of the fabric devices with the same instance type
  uint32_t               IDsIndex;         ///< The index of the array of the FABRIC_TOPOLOGY_MAP[DeviceIds]
} DEVICE_MAP;

/// Device ID structure
typedef XPRF_FABRIC_DEVICE_IDS DEVICE_IDS;

/// Processor neighbor information
typedef struct {
  uint32_t   SocketNumber;       ///< Socket Number
} AMD_FABRIC_TOPOLOGY_PROCESSOR_NEIGHBOR_INFO;

/// Fabric topology structure
typedef XPRF_FABRIC_DEVICE_MAP AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP;

/// Fabric topology neighbor information structure
typedef struct {
  uint32_t   SocketNumber;             ///< Socket Number
  uint32_t   InternalDieNumber;        ///< Internal Die Number
  uint32_t   HostCake;                 ///< Host Cake
  uint32_t   NeighborCake;             ///< Neighbor Cake
} AMD_FABRIC_TOPOLOGY_DIE_NEIGHBOR_INFO;

/// Logical to physical map
typedef struct _FABRIC_RB_LOG_TO_PHY_MAP {
  uint32_t PhySktNum;
  uint32_t PhyRbNum;
  uint32_t PciBusBase;
  uint32_t PciBusLimit;
} FABRIC_RB_LOG_TO_PHY_MAP;

typedef struct {
  uint32_t                             field0;
  uint32_t                             field1;
  uint32_t                             field2;
  uint32_t                             field3;
  uint32_t                             field4;
  uint32_t                             field5;
  uint32_t                             field6;
} SIL_RESERVED_STRUCT_0010;

typedef struct {
  SIL_RESERVED_STRUCT_0010  field0;
  DEVICE_MAP                    Map[PROJ_MAX_IO_DIES_PER_SOCKET][FabricDeviceTypeMax + 1];        ///< The buffer for
                                                                                                  ///< the device map
  DEVICE_IDS                    DeviceIds[PROJ_MAX_IO_DIES_PER_SOCKET][MAX_NUMBER_OF_DEVICE_IDS]; ///< The buffer for
                                                                                                  ///< the device IDs
  } SIL_RESERVED_STRUCT_0009;

#pragma pack (pop)

/*
 * Keep the internal aliases compatible with the public topology layout.
 */
_Static_assert (
  sizeof (ROOT_BRIDGE_LOCATION) == 3 * sizeof (uint32_t),
  "ROOT_BRIDGE_LOCATION must stay three uint32_t fields"
  );
_Static_assert (
  sizeof (DEVICE_IDS) == 2 * sizeof (uint32_t),
  "DEVICE_IDS must stay two uint32_t fields"
  );
_Static_assert (
  sizeof (AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP) ==
    2 * sizeof (uint32_t) + sizeof (const DEVICE_IDS *),
  "AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP gained padding or a field"
  );
