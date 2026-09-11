/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
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
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  );

typedef void (*MPIO_CLEANUP_EARLY_INIT_SP5)  (
  GNB_HANDLE            *GnbHandle,
  uint8_t               DieNumber
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
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

typedef void (*PROGRAM_FAPE_COEFF_LIST) (
  GNB_HANDLE           *GnbHandle,
  PCIe_WRAPPER_CONFIG  *Wrapper,
  PCIe_ENGINE_CONFIG   *Engine
  );

typedef void (*MPIO_CFG_BEFORE_RECONFIG_WRAPPER) (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

typedef void (*MPIOCFG_AFTER_RECONFIG) (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_WRAPPER_CONFIG   *Wrapper
  );

typedef void (*MPIO_CFG_AFTER_RECONFIG_WRAPPER) (
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
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef void (*PCIE_HOTPLUG_INIT) (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef bool (*MPIO_ISEVER_TRIED_TRAINING) (
  PCIe_ENGINE_CONFIG        *Engine
  );

typedef uint32_t (*MPIO_GET_C2PMSG_DOORBELL_ADDRESS) (void);

typedef void (*MPIO_CFG_AFTER_DXIO_INIT) (
  PCIe_PLATFORM_CONFIG                 *Pcie
  );

typedef SIL_STATUS (*MPIO_SERVER_HOTPLUG_INIT) (
  PCIe_PLATFORM_CONFIG                 *Pcie
  );

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  MPIO_CFG_GLOBAL_CONFIG            MpioCfgGlobalConfig;
  MPIO_CLEANUP_EARLY_INIT_SP5       MpioCleanUpEarlyInitSP5;
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
  MPIO_SERVER_HOTPLUG_INIT          MpioServerHotplugInit;
} MPIO_COMMON_2_REV_XFER_BLOCK;
