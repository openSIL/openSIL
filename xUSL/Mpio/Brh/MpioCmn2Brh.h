/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioCmn2Brh.h
 * @brief OpenSIL MPIO Breithorn revision specific transfer header
 *
 */

#pragma once

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <Mpio/MpioClass-api.h>

void
MpioCfgGlobalConfigBrh (
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  );

void
MpioCleanUpEarlyInitSP5Brh (
  GNB_HANDLE            *GnbHandle,
  uint8_t               DieNumber
  );

void
MpioTopologyWorkaroundsBrh (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

void
MpioControlPortsBrh (
  PCIE_PORT_VISIBILITY    Control,
  GNB_HANDLE              *GnbHandle
  );

void
MpioEnablePortBrh (
  PCIe_ENGINE_CONFIG  *Engine,
  GNB_HANDLE          *GnbHandle
  );

void
MpioCfgWrapperBeforeBifurcationBrh (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
MpioCfgWrapperAfterBifurcationBrh (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

uint32_t PcieGetDeviceMappingSizeBrh (void);

uint8_t *PcieGetDeviceMappingBrh (void);

uint8_t
PcieGetDevFnBrh (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  );

void
PcieHotplugInitBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
PcieSetPortPciAddressMapBrh (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  );

void
PcieHotplugPreInitBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

bool
MpioIsEverTriedTrainingBrh (
  PCIe_ENGINE_CONFIG        *Engine
  );

uint32_t MpioGetC2pmsgDoorbellAddressBrh (void);

void
MpioCfgAfterDxioInitBrh (
  PCIe_PLATFORM_CONFIG                 *Pcie
  );
