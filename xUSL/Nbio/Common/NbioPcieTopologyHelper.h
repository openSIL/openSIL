/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  NbioPcieTopologyHelpers.h
 * @brief This file declare helper function to walk the NBIO topology
 *
 */

#pragma once

#include <xSIM.h>
#include "GnbDxio.h"
#include "NbioPcieTopologyHelper.h"

/* SIL_STATUS_UPDATE checks if the current status indicates an error. If so, it updates the aggregated
   status with the error code.
 */
#define SIL_STATUS_UPDATE(Current, Aggregated) \
        if (Current > Aggregated) { \
          Aggregated = Current; \
        }

typedef struct {
  uint32_t             PcieCoreNum;
  PCIe_WRAPPER_CONFIG  *ThisPcieCore;
  PCIe_WRAPPER_CONFIG  *PrevPcieCore;
  PCIe_WRAPPER_CONFIG  *NextPcieCore;
  PCIe_ENGINE_CONFIG   *FirstPciePortOnNext; // First PciePort on NextPcieCore
  PCIe_ENGINE_CONFIG   *LastPciePortOnThis;  // Last PciePort on ThisPcieCore
  PCIe_ENGINE_CONFIG   *LastPciePortOnPrev;  // Last PciePort on PrevPcieCore
} HIDE_WRAPPER_INFO;

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
PCIe_DESCRIPTOR_HEADER * PcieConfigGetChild (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);
PCIe_DESCRIPTOR_HEADER * PcieConfigGetPeer (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);
PCIe_DESCRIPTOR_HEADER * PcieConfigGetParent (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor);
#define PcieConfigGetPlatform(Descriptor)    \
        ((PCIe_PLATFORM_CONFIG *) PcieConfigGetParent(DESCRIPTOR_PLATFORM, &((Descriptor)->Header)))
#define PcieConfigGetChildSilicon(Descriptor) \
        ((PCIe_SILICON_CONFIG *) PcieConfigGetChild(DESCRIPTOR_SILICON, &((Descriptor)->Header)))
#define PcieConfigGetChildWrapper(Descriptor) \
        ((PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &((Descriptor)->Header)))
#define PcieConfigGetParentWrapper(Descriptor) \
        ((PCIe_WRAPPER_CONFIG *) PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &((Descriptor)->Header)))
#define PcieConfigGetParentSilicon(Descriptor) \
        ((PCIe_SILICON_CONFIG *) PcieConfigGetParent(DESCRIPTOR_SILICON, &((Descriptor)->Header)))
#define PcieConfigGetChildEngine(Descriptor)   \
        ((PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &((Descriptor)->Header)))
#define PcieConfigAttachChild(P, C) \
        (P)->Child = (uint16_t) ((uint8_t *) C - (uint8_t *) P);
#define PcieConfigAttachParent(P, C) \
        (C)->Parent = (uint16_t) ((uint8_t *) C - (uint8_t *) P);
#define PcieConfigSetDescriptorFlags(Descriptor, SetDescriptorFlags) \
        if (Descriptor != NULL) (Descriptor)->Header.DescriptorFlags |= SetDescriptorFlags
#define PcieConfigGetNextDescriptor(Descriptor) \
        ((((Descriptor->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : ((Descriptor + 1))))
#define PcieConfigIsPcieEngine(Descriptor) \
        ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_PCIE_ENGINE) != 0) : false)
#define PcieConfigIsSbPcieEngine(Engine) \
        ((Engine != NULL) ? ((bool) (Engine->Type.Port.PortData.MiscControls.SbLink)) : false)
#define PcieConfigIsEngineAllocated(Descriptor) \
        ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0) : false)
#define PcieConfigResetDescriptorFlags(Descriptor, ResetDescriptorFlags) \
        if (Descriptor != NULL) ((PCIe_DESCRIPTOR_HEADER *) Descriptor)->DescriptorFlags &= (~(ResetDescriptorFlags))
#define PcieInputParsetGetNextDescriptor(Descriptor) \
        ((Descriptor == NULL) ? NULL : ((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (Descriptor + 1))
#define PcieConfigGetNextTopologyDescriptor(Descriptor, Termination) \
        ((Descriptor == NULL) ? NULL : ((((PCIe_DESCRIPTOR_HEADER *) Descriptor)->DescriptorFlags & Termination) != 0) ? \
        NULL : ((uint8_t *) Descriptor + ((PCIe_DESCRIPTOR_HEADER *) Descriptor)->Peer))
#define GnbGetNextHandle(Descriptor) \
        (GNB_HANDLE *) PcieConfigGetNextTopologyDescriptor(Descriptor, DESCRIPTOR_TERMINATE_TOPOLOGY)
#define PcieConfigGetNextDataDescriptor(Descriptor) \
        ((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0 ? NULL : (Descriptor + 1))

GNB_HANDLE * NbioGetNextSocket (GNB_HANDLE *GnbHandle);
GNB_HANDLE * NbioGetHandle (PCIe_PLATFORM_CONFIG *Pcie);
GNB_HANDLE * NbioGetAltHandle (PCIe_PLATFORM_CONFIG *Pcie);
GNB_HANDLE * GetGnbHandle (void);
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
PCIe_COMPLEX_DESCRIPTOR * PcieInputParserGetComplexDescriptor (PCIe_COMPLEX_DESCRIPTOR *ComplexList, size_t Index);
size_t PcieInputParserGetNumberOfEngines (PCIe_COMPLEX_DESCRIPTOR *Complex);
PCIe_ENGINE_DESCRIPTOR * PcieInputParserGetEngineDescriptor (PCIe_COMPLEX_DESCRIPTOR *Complex, size_t Index);
bool PcieConfigCheckPortStatus (PCIe_ENGINE_CONFIG *Engine, uint32_t PortStatus);
bool PcieConfigIsActivePcieEngine (PCIe_ENGINE_CONFIG *Engine);
uint16_t PcieConfigUpdatePortStatus (
  PCIe_ENGINE_CONFIG *Engine, PCIe_ENGINE_INIT_STATUS SetStatus, PCIe_ENGINE_INIT_STATUS ResetStatus
  );
SIL_STATUS PcieConfigRunProcForAllDescriptors (
  uint32_t InDescriptorFlags, uint32_t OutDescriptorFlags, uint32_t TerminationFlags,
  PCIe_RUN_ON_DESCRIPTOR_CALLBACK  Callback, void *Buffer, PCIe_PLATFORM_CONFIG *Pcie
  );
PCIe_ENGINE_CONFIG * PcieConfigLocateSbEngine (PCIe_WRAPPER_CONFIG *Wrapper);
void PcieConfigEngineDebugDump (PCIe_ENGINE_CONFIG *EngineList);
void PcieConfigWrapperDebugDump (PCIe_WRAPPER_CONFIG *WrapperList);
void PcieUserConfigConfigDump (PCIe_COMPLEX_DESCRIPTOR *ComplexDescriptor);
void PcieUserDescriptorConfigDump (PCIe_ENGINE_DESCRIPTOR *EngineDescriptor);
