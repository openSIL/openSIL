/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
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

#pragma pack (push, 1)

#define MAX_PCI_BUS_NUMBER_PER_SEGMENT         0x100
#define DF_GET_SEGMENT(a)                      (a / MAX_PCI_BUS_NUMBER_PER_SEGMENT)
#define DF_GET_BUS(a)                          (a % MAX_PCI_BUS_NUMBER_PER_SEGMENT)

/// Root Bridge location
typedef struct {
  uint32_t       Socket;
  uint32_t       Die;
  uint32_t       Index;
} ROOT_BRIDGE_LOCATION;

/// Fabric Device Types
typedef enum {
  PrimaryFch,                  ///< Primary FCH
  SecondaryFch,                ///< Secondary FCH
  FchTypeMax,                  ///< Not a valid entry type, use for limit checking.
  PrimarySmu,                  ///< Primary SMU
  SecondarySmu,                ///< Secondary SMU
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
  Ccm,                         ///< Processor Family Specific Workarounds which are @b not
                               ///< practical using the other types.
  Gcm,                         ///< Processor Config Space registers via SMN.
  Ncs,                         ///< Processor Config Space registers via SMN.
  Ncm,                         ///< Processor Config Space registers via SMN.
  Pie,                         ///< Processor Config Space registers via SMN.
  Ioms,                        ///< Processor Config Space registers via SMN.
  Cs,                          ///< Processor Config Space registers via SMN.
  Tcdx,                        ///< Processor Config Space registers via SMN.
  Cake,                        ///< Processor Config Space registers via SMN.
  CsUmc,                       ///< Processor Config Space registers via SMN.
  CsCcix,                      ///< Processor Config Space registers via SMN.
  CsCmp,                       ///< Processor Config Space registers via SMN.
  Acm,                         ///< Processor Config Space registers via SMN.
  Iom,                         ///< Processor Config Space registers via SMN.
  Ios,                         ///< Processor Config Space registers via SMN.
  Icng,                        ///< Processor Config Space registers via SMN.
  Cnli,                        ///< Processor Config Space registers via SMN.
  Pfx,                         ///< Processor Config Space registers via SMN.
  Spf,                         ///< Processor Config Space registers via SMN.
  NcmIommu,                    ///< Processor Config Space registers via SMN.
  Giom,                        ///< Processor Config Space registers via SMN.
  Htdm,                        ///< Processor Config Space registers via SMN.
  Htds,                        ///< Processor Config Space registers via SMN.
  Xgmi,                        ///< Processor Config Space registers via SMN.
  FabricDeviceTypeMax          ///< Not a valid entry type, use for limit checking.
} FABRIC_DEVICE_TYPE;

/// Structure for the device map to be stored to the heap
typedef struct {
  FABRIC_DEVICE_TYPE     Type;             ///< The instance type of the fabric device
  uint32_t               Count;            ///< The count of the fabric devices with the same instance type
  uint32_t               IDsIndex;         ///< The index of the array of the FABRIC_TOPOLOGY_MAP[DeviceIds]
} DEVICE_MAP;

/// Device ID structure
typedef struct {
  uint32_t   FabricID;           ///< Fabric ID
  uint32_t   InstanceID;         ///< Instance ID
} DEVICE_IDS;

/// Fabric topology structure
typedef struct {
  FABRIC_DEVICE_TYPE   Type;          ///< Type
  uint32_t             Count;         ///< Count
  const DEVICE_IDS     *IDs;          ///< Device IDs
} AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP;

/// Logical to physical map
typedef struct _FABRIC_RB_LOG_TO_PHY_MAP {
  uint32_t PhySktNum;
  uint32_t PhyRbNum;
  uint32_t PciBusBase;
  uint32_t PciBusLimit;
} FABRIC_RB_LOG_TO_PHY_MAP;

#pragma pack (pop)
