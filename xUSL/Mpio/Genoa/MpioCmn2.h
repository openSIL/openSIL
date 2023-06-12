/**
 * @file  MpioCmn2.h
 * @brief OpenSIL MPIO transfer header
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <NBIO/GnbDxio.h>
#include <Mpio/MpioClass-api.h>


void
MpioCfgGlobalConfig (
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  );

void
MpioCfgWrapperBeforeBifurcation (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
MpioCfgWrapperAfterBifurcation (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
SubsystemIdSetting (
  GNB_HANDLE       *GnbHandle
  );

void
GetCcdInfo (
    uint32_t  *CcdBitfield
  );

SIL_STATUS
PcieCommonCoreConfiguration (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
PcieCommonEngineGetDeliInfo (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
);

void
PcieCommonEngineConfiguration (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
MpioControlPorts (
    PCIE_PORT_VISIBILITY    Control,
    GNB_HANDLE              *GnbHandle
  );

void
MpioEnablePort (
  PCIe_ENGINE_CONFIG      *Engine,
  GNB_HANDLE              *GnbHandle
);

void
InitBusRangesAndAri (
  GNB_HANDLE                           *GnbHandle,
  PCIe_PLATFORM_CONFIG                 *Pcie,
  bool                                 CfgPcieAriSupport
  );

bool
MpioIsEverTriedTraining (
  PCIe_ENGINE_CONFIG        *Engine
);

uint16_t
PcieGetPortStrapIndex (
  uint16_t Strap,
  uint16_t Port
  );

uint16_t PcieGetPortDpcCapabilityStrap (void);

uint16_t PcieGetTphSupportStrap (void);

uint32_t
PcieGetDeviceMappingSize (void);

uint8_t
*PcieGetDeviceMapping (void);

uint8_t
PcieGetDevFn (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  );

void
MpioCleanUpEarlyInitSP5 (
  GNB_HANDLE            *GnbHandle,
  uint8_t               DieNumber
  );

void
PcieSetPortPciAddressMap (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  );

void
MpioTopologyWorkarounds (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );
