/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  MpioCmn2Phx.h
 * @brief OpenSIL MPIO revision specific transfer header
 *
 */

#pragma once

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/MpioClass-api.h>

void
MpioCfgGlobalConfigPhx (
  SIL_CONTEXT                   *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  );

void
MpioCfgWrapperBeforeBifurcationPhx (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
MpioCfgWrapperAfterBifurcationPhx (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
MpioControlPortsPhx (
  PCIE_PORT_VISIBILITY    Control,
  GNB_HANDLE              *GnbHandle
  );

void
MpioEnablePortPhx (
  PCIe_ENGINE_CONFIG      *Engine,
  GNB_HANDLE              *GnbHandle
  );

bool
MpioIsEverTriedTrainingPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_ENGINE_CONFIG        *Engine
  );

uint16_t
PcieGetPortStrapIndexPhx (
  uint16_t Strap,
  uint16_t Port
  );

uint16_t PcieGetPortDpcCapabilityStrapPhx (void);

uint16_t PcieGetTphSupportStrapPhx (void);

uint32_t
PcieGetDeviceMappingSizePhx (void);

uint8_t
*PcieGetDeviceMappingPhx (void);

uint8_t
PcieGetDevFnPhx (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  );

void
PcieSetPortPciAddressMapPhx (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  );

void
PcieHotplugPreInitPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
PcieHotplugInitPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );


void
MpioTopologyWorkaroundsPhx (
  SIL_CONTEXT                 *SilContext,
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

void
MpioCfgAfterDxioInitPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  );

uint32_t
MpioGetC2pmsgDoorbellAddressPhx (void);

void MpioConfigurePsppPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  );


void
PcieEarlyTrainFixupsPhx (
  SIL_CONTEXT                 *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK  *SilDataCommon,
  GNB_HANDLE                  *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR     *PcieTopologyData
  );

void
IommuInitAfterMpioInitPhx (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

void
MpioHotplugConfigureUSB4Phx (
  GNB_HANDLE  *GnbHandle
  );

SIL_STATUS
Usb4LaneNumberCfgPhx (
  uint8_t                      Counter,
  FW_ASK_STRUCT                *AskEntry,
  MPIO_PORT_DESCRIPTOR         *TopologyEntry
  );

void
SwapLogicalBridgeIdPhx (
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t                NewBdgIdx
  );

void
MpioLaneMappingFixupsPhx (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  );

void
NbifDeviceEarlyControlPhx (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

uint32_t
MpioCfgAddStrapListPhx (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  void                   *AncillaryData,
  MPIO_PORT_DESCRIPTOR   *PortList
  );

uint32_t
MpioCfgGetStrapListSizePhx (
  SIL_CONTEXT           *SilContext,
  GNB_HANDLE            *GnbHandle,
  MPIO_PORT_DESCRIPTOR  *PortPointer
  );

void
MpioReleasePortPhx (
  GNB_HANDLE             *GnbHandle,
  PCIe_ENGINE_CONFIG     *Engine
  );

uint8_t MpioGetPortIdPhx (
  PCIe_ENGINE_CONFIG     *Engine
  );

void
MpioRemoveCxlLinksPhx (
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  MPIO_COMPLEX_DESCRIPTOR       *PcieTopologyData,
  PCIe_PLATFORM_CONFIG          *Pcie
  );

SIL_STATUS
MpioPcieSetSpeed (
  SIL_CONTEXT                   *SilContext,
  PCIe_PLATFORM_CONFIG          *Pcie,
  uint8_t                       PciDevice,
  uint8_t                       PciFunction,
  uint8_t                       TargetSpeed
  );

SIL_STATUS
MpioGetEarlyLinkConfig (
  SIL_CONTEXT                   *SilContext,
  EARLY_LINK_STATUS             *EarlyLinkStatus
  );
