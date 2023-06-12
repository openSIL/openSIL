/**
 * @file  NbioPcieTopologyHelpers.h
 * @brief This file declare helper function to walk the NBIO topology
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
//#include <NBIO/IOD/Nbio.h>
//#include <SilPcie.h>
#include "GnbDxio.h"

/* SIL_STATUS_UPDATE checks if the current status indicates an error. If so, it updates the aggregated
   status with the error code.
*/
#define SIL_STATUS_UPDATE(Current, Aggregated) \
if (Current > Aggregated) { \
  Aggregated = Current; \
}


typedef SIL_STATUS (*PCIe_RUN_ON_DESCRIPTOR_CALLBACK)
  (PCIe_DESCRIPTOR_HEADER *Descriptor, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie);
typedef SIL_STATUS (*PCIe_RUN_ON_WRAPPER_CALLBACK)
  (PCIe_WRAPPER_CONFIG *Wrapper, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie);
typedef void (*PCIe_RUN_ON_WRAPPER_CALLBACK2)
  (PCIe_WRAPPER_CONFIG *Wrapper, void *Buffer, GNB_HANDLE *GnbHandle);
typedef void (*PCIe_RUN_ON_ENGINE_CALLBACK)
  (PCIe_ENGINE_CONFIG *Engine, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie);
typedef void (*PCIe_RUN_ON_ENGINE_CALLBACK2)
  (PCIe_ENGINE_CONFIG *Engine, void *Buffer, PCIe_WRAPPER_CONFIG *Wrapper);


/**
 * Declare function prototypes here
 *
 */
PCIe_DESCRIPTOR_HEADER* PcieConfigGetChild (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);
PCIe_DESCRIPTOR_HEADER* PcieConfigGetPeer (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);
PCIe_DESCRIPTOR_HEADER* PcieConfigGetParent (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);

GNB_HANDLE* NbioGetNextSocket (GNB_HANDLE *GnbHandle);
GNB_HANDLE* NbioGetHandle (PCIe_PLATFORM_CONFIG *Pcie);
GNB_HANDLE* GetGnbHandle (void);
PCI_ADDR NbioGetHostPciAddress (GNB_HANDLE *GnbHandle);
void PcieConfigDebugDump (PCIe_PLATFORM_CONFIG *Pcie);

void PcieConfigRunProcForAllEngines (
  uint32_t DescriptorFlags, PCIe_RUN_ON_ENGINE_CALLBACK Callback, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie
  );
void PcieConfigRunProcForAllEnginesInWrapper (
  uint32_t DescriptorFlags, PCIe_RUN_ON_ENGINE_CALLBACK2 Callback, void *Buffer, PCIe_WRAPPER_CONFIG *Wrapper
  );
SIL_STATUS PcieConfigRunProcForAllWrappers (
  uint32_t DescriptorFlags, PCIe_RUN_ON_WRAPPER_CALLBACK Callback, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie
  );
void PcieConfigRunProcForAllWrappersInNbio (
  uint32_t DescriptorFlags, PCIe_RUN_ON_WRAPPER_CALLBACK2 Callback, void *Buffer, GNB_HANDLE *GnbHandle
  );
size_t PcieInputParserGetNumberOfComplexes (PCIe_COMPLEX_DESCRIPTOR *ComplexList);
PCIe_COMPLEX_DESCRIPTOR* PcieInputParserGetComplexDescriptor (PCIe_COMPLEX_DESCRIPTOR *ComplexList, size_t Index);
size_t PcieInputParserGetNumberOfEngines (PCIe_COMPLEX_DESCRIPTOR *Complex);
PCIe_ENGINE_DESCRIPTOR* PcieInputParserGetEngineDescriptor (PCIe_COMPLEX_DESCRIPTOR *Complex, size_t Index);
bool PcieConfigCheckPortStatus (PCIe_ENGINE_CONFIG *Engine, uint32_t PortStatus);
bool PcieConfigIsActivePcieEngine (PCIe_ENGINE_CONFIG *Engine);
uint16_t PcieConfigUpdatePortStatus (
  PCIe_ENGINE_CONFIG *Engine, PCIe_ENGINE_INIT_STATUS SetStatus, PCIe_ENGINE_INIT_STATUS ResetStatus
  );
SIL_STATUS PcieConfigRunProcForAllDescriptors (
  uint32_t InDescriptorFlags, uint32_t OutDescriptorFlags, uint32_t TerminationFlags,
  PCIe_RUN_ON_DESCRIPTOR_CALLBACK  Callback, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie
  );
PCIe_ENGINE_CONFIG* PcieConfigLocateSbEngine (PCIe_WRAPPER_CONFIG *Wrapper);
void PcieConfigEngineDebugDump (PCIe_ENGINE_CONFIG *EngineList);
void PcieConfigWrapperDebugDump (PCIe_WRAPPER_CONFIG *WrapperList);
void PcieUserConfigConfigDump (PCIe_COMPLEX_DESCRIPTOR *ComplexDescriptor);
void PcieUserDescriptorConfigDump (PCIe_ENGINE_DESCRIPTOR *EngineDescriptor);
