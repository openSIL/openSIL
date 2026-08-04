/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  MpioCmn2Rev.h
 * This file defines the types of all functions contained within MPIO's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once

#include <Mpio/MpioClass-api.h>

// Common function type definitions of Cmn2Rev functions

typedef void (*MPIO_CFG_GLOBAL_CONFIG) (
  SIL_CONTEXT                   *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  );

typedef void (*MPIO_CONTROL_PORTS) (
  PCIE_PORT_VISIBILITY    Control,
  GNB_HANDLE              *GnbHandle
  );

typedef void (*MPIO_ENABLE_PORT) (
  PCIe_ENGINE_CONFIG      *Engine,
  GNB_HANDLE              *GnbHandle
  );

typedef void (*MPIO_TOPOLOGY_WORKAROUNDS) (
  SIL_CONTEXT                 *SilContext,
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

typedef void (*MPIO_CFG_BEFORE_RECONFIG_WRAPPER) (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

typedef void (*MPIO_CFG_AFTER_RECONFIG_WRAPPER) (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

typedef uint32_t (*PCIE_GET_DEVICE_MAPPING_SIZE) (void);

typedef uint8_t * (*PCIE_GET_DEVICE_MAPPING) (void);

typedef uint8_t (*PCIE_GET_DEVICE_FUNCTION) (
  GNB_HANDLE        *GnbHandle,
  uint8_t           Index
  );

typedef void (*PCIE_SET_PORT_PCI_ADDRESS_MAP) (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  );

typedef void (*PCIE_HOTPLUG_PRE_INIT) (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef void (*PCIE_HOTPLUG_INIT) (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef bool (*MPIO_ISEVER_TRIED_TRAINING) (
  SIL_CONTEXT               *SilContext,
  PCIe_ENGINE_CONFIG        *Engine
  );

typedef uint32_t (*MPIO_GET_C2PMSG_DOORBELL_ADDRESS) (void);

typedef void (*MPIO_CFG_AFTER_DXIO_INIT) (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  );

typedef void (*MPIO_CONFIGURE_PSPP) (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  );

typedef void (*PCIE_EARLY_TRAIN_FIXUPS) (
  SIL_CONTEXT                     *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK      *SilDataCommon,
  GNB_HANDLE                      *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR         *PcieTopologyData
  );

typedef void (*IOMMU_INIT_AFTER_MPIO_INIT) (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

typedef void (*MPIO_HOTPLUG_CONFIGURE_USB4) (
  GNB_HANDLE  *GnbHandle
  );

typedef SIL_STATUS (*USB4_LANE_NUMBER_CFG) (
  uint8_t                      Counter,
  FW_ASK_STRUCT                *AskEntry,
  MPIO_PORT_DESCRIPTOR         *TopologyEntry
  );

typedef void (*SWAP_LOGICAL_BRIDGE_ID) (
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t                NewBdgIdx
  );

typedef void (*MPIO_LANE_MAPPING_FIXUPS) (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  );

typedef void (*NBIF_DEVICE_EARLY_CONTROL) (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

typedef uint32_t (*MPIO_CFG_ADD_STRAP_LIST) (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  void                   *AncillaryData,
  MPIO_PORT_DESCRIPTOR   *PortList
  );

typedef uint32_t (*MPIO_CFG_GET_STRAP_LIST_SIZE) (
  SIL_CONTEXT           *SilContext,
  GNB_HANDLE            *GnbHandle,
  MPIO_PORT_DESCRIPTOR  *PortPointer
  );

typedef void (*MPIO_RELEASE_PORT) (
  GNB_HANDLE             *GnbHandle,
  PCIe_ENGINE_CONFIG     *Engine
  );

typedef uint8_t (*MPIO_GET_PORT_ID) (
  PCIe_ENGINE_CONFIG     *Engine
  );

typedef void (*MPIO_REMOVE_CXL_LINKS) (
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  MPIO_COMPLEX_DESCRIPTOR       *PcieTopologyData,
  PCIe_PLATFORM_CONFIG          *Pcie
  );

typedef SIL_STATUS (*MPIO_PCIE_SET_SPEED) (
  SIL_CONTEXT                   *SilContext,
  PCIe_PLATFORM_CONFIG          *Pcie,
  uint8_t                       PciDevice,
  uint8_t                       PciFunction,
  uint8_t                       TargetSpeed
  );

typedef struct {
  bool      EarlyLinkStatus;
  uint8_t   PhysicalRootBridge;
  uint8_t   LogicalRootBridge;
  uint8_t   RootPortBus;
  uint8_t   RootPortDevice;
  uint8_t   RootPortFunction;
} EARLY_LINK_STATUS;

typedef SIL_STATUS (*MPIO_GET_EARLY_LINK_CONFIG) (
  SIL_CONTEXT                   *SilContext,
  EARLY_LINK_STATUS             *EarlyLinkStatus
  );

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  MPIO_CFG_GLOBAL_CONFIG            MpioCfgGlobalConfig;
  MPIO_CONTROL_PORTS                MpioControlPorts;
  MPIO_ENABLE_PORT                  MpioEnablePort;
  MPIO_TOPOLOGY_WORKAROUNDS         MpioTopologyWorkarounds;
  MPIO_CFG_BEFORE_RECONFIG_WRAPPER  MpioCfgBeforeReconfigWrapper;
  MPIO_CFG_AFTER_RECONFIG_WRAPPER   MpioCfgAfterReconfigWrapper;
  PCIE_GET_DEVICE_MAPPING_SIZE      PcieGetDeviceMappingSize;
  PCIE_GET_DEVICE_MAPPING           PcieGetDeviceMapping;
  PCIE_GET_DEVICE_FUNCTION          PcieGetDevFn;
  PCIE_SET_PORT_PCI_ADDRESS_MAP     PcieSetPortPciAddressMap;
  PCIE_HOTPLUG_PRE_INIT             PcieHotplugPreInit;
  PCIE_HOTPLUG_INIT                 PcieHotplugInit;
  MPIO_ISEVER_TRIED_TRAINING        MpioIsEverTriedTraining;
  MPIO_CFG_AFTER_DXIO_INIT          MpioCfgAfterDxioInit;
  MPIO_GET_C2PMSG_DOORBELL_ADDRESS  MpioGetC2pmsgDoorbellAddress;
  MPIO_CONFIGURE_PSPP               MpioConfigurePspp;
  PCIE_EARLY_TRAIN_FIXUPS           PcieEarlyTrainFixups;
  IOMMU_INIT_AFTER_MPIO_INIT        IommuInitAfterMpioInit;
  MPIO_HOTPLUG_CONFIGURE_USB4       MpioHotplugConfigureUSB4;
  USB4_LANE_NUMBER_CFG              Usb4LaneNumberCfg;
  SWAP_LOGICAL_BRIDGE_ID            SwapLogicalBridgeId;
  MPIO_LANE_MAPPING_FIXUPS          MpioLaneMappingFixups;
  NBIF_DEVICE_EARLY_CONTROL         NbifDeviceEarlyControl;
  MPIO_CFG_ADD_STRAP_LIST           MpioCfgAddStrapList;
  MPIO_CFG_GET_STRAP_LIST_SIZE      MpioCfgGetStrapListSize;
  MPIO_RELEASE_PORT                 MpioReleasePort;
  MPIO_GET_PORT_ID                  MpioGetPortId;
  MPIO_REMOVE_CXL_LINKS             MpioRemoveCxlLinks;
  MPIO_PCIE_SET_SPEED               MpioPcieSetSpeed;
  MPIO_GET_EARLY_LINK_CONFIG        MpioGetEarlyLinkConfig;
} MPIO_COMMON_2_REV_XFER_BLOCK;
