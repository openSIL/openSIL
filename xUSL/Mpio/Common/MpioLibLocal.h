/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioLibLocal.h
 *  @brief Prototypes of local functions in MpioInitLib and event handlers
 */

#pragma once

#include <xSIM.h>
#include "MpioInitLib.h"
#include "MpioStructs.h"
#include <Mpio/MpioClass-api.h>


typedef struct {
  FW_ASK_STRUCT     *MpioAsk;
  uint32_t          MpioAskCount;
  uint8_t           *ExtAttributes;
  uint32_t          ExtAttributeSize;
} MPIO_DATA;


/*
 * DXIO Library Functions
 */
SIL_STATUS
MpioParsePlatformTopology (
  SIL_CONTEXT               *SilContext,
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  );

void
MpioPortMapping (
  SIL_CONTEXT                          *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK           *SilDataCommon,
  GNB_HANDLE                           *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR              *PlatformTopology,
  MPIO_DATA                            *MpioData
  );

void
MpioUpdatePortTrainingStatus (
  SIL_CONTEXT                          *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK           *SilDataCommon,
  GNB_HANDLE                           *GnbHandle,
  MPIO_DATA                            *MpioData
  );

uint32_t
MpioAncDataCalcSize (
  MPIO_PORT_DESCRIPTOR    *PortPointer
  );

uint32_t
MpioAddAncDataOverride (
  GNB_HANDLE                *GnbHandle,
  void                      *AncillaryData,
  MPIO_PORT_DESCRIPTOR      *PortList
  );

void
MpioAncDataDump (
  GNB_HANDLE                *GnbHandle,
  uint8_t                   *AncillaryData,
  uint32_t                  AncillaryDataSize
  );

// MPIO Debug Output Functions
void
MpioDebugDump (
  MPIO_DATA       *MpioData
  );


SIL_STATUS
PcieMapPortPciAddress (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  uint8_t               *PortDevMap
  );

void
SyncEarlyTrainRemap (
  SIL_CONTEXT             *SilContext,
  GNB_HANDLE              *GnbHandle,
  uint8_t                 *PortDevMap,
  PCIe_ENGINE_CONFIG      *Engine
  );

SIL_STATUS
MpioGetTopologyForSocket (
  uint8_t                   SocketNumber,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_PORT_DESCRIPTOR      **PortList
  );

// Common support functions
void
FindEngine (
  SIL_CONTEXT          *SilContext,
  PCIe_ENGINE_CONFIG   *Engine,
  void                 *Buffer,        // Pointer to ENGINE_LOCATION_CALLBACK
  PCIe_WRAPPER_CONFIG  *Wrapper
  );

void
FindWrapper (
  SIL_CONTEXT           *SilContext,
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,        // Pointer to ENGINE_LOCATION_CALLBACK
  GNB_HANDLE            *GnbHandle
  );

PCIe_ENGINE_CONFIG *
MpioFindEngineForAsk (
  SIL_CONTEXT         *SilContext,
  GNB_HANDLE          *GnbHandle,
  FW_ASK_STRUCT       *AskEntry
  );

void
MpioProcessEarlyTrain (
  SIL_CONTEXT               *SilContext,
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  );

// Configuration Points from APU specific driver
void
MpioCfgBeforeReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

void
MpioCfgAfterReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  );

void
MpioCfgEarlyLinkAuthentication (
  GNB_HANDLE       *GnbHandle,
  PCI_ADDR         EarlyLinkAddress
  );

bool
MpioIsEverTriedTraining (
  SIL_CONTEXT               *SilContext,
  PCIe_ENGINE_CONFIG        *Engine
  );

void
MpioManageTopology (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

void
MpioCfgBeforeDxioInit (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie,
  MPIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  );
