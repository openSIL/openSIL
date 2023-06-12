/**
 * @file  NbioIp2Ip.h
 * This file contains the definition of the NBIO Ip 2 Ip API.
 * This API provides other openSIL IPs with any NBIO functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include "GnbDxio.h"
#include "NbioPcieTopologyHelper.h"
#include "NbioCommon.h"


typedef GNB_HANDLE* (*NBIO_GET_HANDLE) (
  PCIe_PLATFORM_CONFIG           *Pcie
  );

typedef GNB_HANDLE* (*NBIO_GET_GNB_HANDLE) (
  void
  );

typedef void (*NBIO_PCIE_CONFIG_DEBUG_DUMP) (
  PCIe_PLATFORM_CONFIG          *Pcie
  );

typedef PCI_ADDR (*NBIO_GET_HOST_PCI_ADDRESS) (
  GNB_HANDLE                     *GnbHandle);

typedef void (*NBIO_PCIE_CONFIG_RUNPROC_FORALL_ENGINES) (
  uint32_t                        DescriptorFlags,
  PCIe_RUN_ON_ENGINE_CALLBACK     Callback,
  void                            *Buffer,
  PCIe_PLATFORM_CONFIG            *Pcie
  );

typedef void (*NBIO_PCIE_CONFIG_RUNPROC_FORALL_ENGINES_WRAPPER) (
  uint32_t                         DescriptorFlags,
  PCIe_RUN_ON_ENGINE_CALLBACK2     Callback,
  void                             *Buffer,
  PCIe_WRAPPER_CONFIG              *Wrapper
  );

typedef SIL_STATUS (*NBIO_PCIE_CONFIG_RUNPROC_FORALL_WRAPPERS) (
  uint32_t                         DescriptorFlags,
  PCIe_RUN_ON_WRAPPER_CALLBACK     Callback,
  void                             *Buffer,
  PCIe_PLATFORM_CONFIG             *Pcie
  );

typedef void (*NBIO_PCIE_CONFIG_RUNPROC_FORALL_WRAPPERS_NBIO) (
  uint32_t                         DescriptorFlags,
  PCIe_RUN_ON_WRAPPER_CALLBACK2    Callback,
  void                             *Buffer,
  GNB_HANDLE                       *GnbHandle
  );

typedef bool (*NBIO_PCIE_CONFIG_CHECK_PORT_STATUS) (
  PCIe_ENGINE_CONFIG               *Engine,
  uint32_t                         PortStatus
  );

typedef SIL_STATUS (*NBIO_GET_VERSION_INFO) (
  uint8_t                          *VerString
  );

typedef void (*NBIO_SERVICE_COMMON_INIT_ARGUMENTS) (
  uint32_t                          *NbioArg
  );

typedef PCIe_DESCRIPTOR_HEADER* (*NBIO_PCIE_CONFIG_GET_CHILD) (
  uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor
  );

typedef PCIe_DESCRIPTOR_HEADER* (*NBIO_PCIE_CONFIG_GET_PARENT) (
  uint32_t                          Type,
  PCIe_DESCRIPTOR_HEADER            *Descriptor
  );

// Define the Ip2Ip API as a struct containing pointers to the above functions

typedef struct {
  NBIO_GET_HANDLE                                  NbioGetHandle;
  NBIO_GET_GNB_HANDLE                              GetGnbHandle;
  NBIO_PCIE_CONFIG_DEBUG_DUMP                      PcieConfigDebugDump;
  NBIO_GET_HOST_PCI_ADDRESS                        GetHostPciAddress;
  NBIO_PCIE_CONFIG_GET_CHILD                       PcieConfigGetChild;
  NBIO_PCIE_CONFIG_GET_PARENT                      PcieConfigGetParent;
  NBIO_PCIE_CONFIG_RUNPROC_FORALL_ENGINES          PcieConfigRunProcForAllEngines;
  NBIO_PCIE_CONFIG_RUNPROC_FORALL_ENGINES_WRAPPER  PcieConfigRunProcForAllEnginesInWrapper;
  NBIO_PCIE_CONFIG_RUNPROC_FORALL_WRAPPERS         PcieConfigRunProcForAllWrappers;
  NBIO_PCIE_CONFIG_RUNPROC_FORALL_WRAPPERS_NBIO    PcieConfigRunProcForAllWrappersInNbio;
  NBIO_PCIE_CONFIG_CHECK_PORT_STATUS               PcieConfigCheckPortStatus;
  NBIO_GET_VERSION_INFO                            GetVersionInfo;
} NBIO_IP2IP_API;