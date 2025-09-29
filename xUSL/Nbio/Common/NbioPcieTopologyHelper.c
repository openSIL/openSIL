/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPcieTopologyHelper.c
 * @brief This file host helper function to walk the NBIO topology
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <Utils.h>
#include <Nbio/NbioIp2Ip.h>

/*----------------------------------------------------------------------------------------*/
/**
 * PcieConfigGetChild
 *
 * Get child descriptor of specific type
 *
 * @param       Type            Descriptor type
 * @param       Descriptor      Pointer to buffer to pass information to callback
 * @retval      Return address to list of child descriptors
 */

PCIe_DESCRIPTOR_HEADER * PcieConfigGetChild (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor)
{
  while ((Descriptor->DescriptorFlags & Type) == 0) {
    if (Descriptor->Child != 0) {
      Descriptor = (PCIe_DESCRIPTOR_HEADER *) ((uint8_t *) Descriptor + Descriptor->Child);
    } else {
      return NULL;
    }
  }
  return Descriptor;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PcieConfigGetPeer
 *
 * Get peer descriptor of specific type
 *
 * @param       Type            Descriptor type
 * @param       Descriptor      Pointer to buffer to pass information to callback
 * @retval  Return address to list of peer descriptors
 */
PCIe_DESCRIPTOR_HEADER * PcieConfigGetPeer (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor)
{
  while ((Descriptor->DescriptorFlags & Type) == 0) {
    if (Descriptor->Peer != 0) {
      Descriptor = (PCIe_DESCRIPTOR_HEADER *) ((uint8_t *) Descriptor + Descriptor->Peer);
    } else {
      return NULL;
    }
  }
  return Descriptor;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get parent descriptor of specific type
 *
 *
 * @param[in]       Type            Descriptor type
 * @param[in]       Descriptor      Pointer to buffer to pass information to callback
 */
PCIe_DESCRIPTOR_HEADER * PcieConfigGetParent (uint32_t Type, PCIe_DESCRIPTOR_HEADER *Descriptor)
{
  while ((Descriptor->DescriptorFlags & Type) == 0) {
    if (Descriptor->Parent != 0) {
      Descriptor = (PCIe_DESCRIPTOR_HEADER *) ((uint8_t *) Descriptor - Descriptor->Parent);
    } else {
      return NULL;
    }
  }
  return Descriptor;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GetGnbHandle
 *
 * Get base address to NBIO IOHC config data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  Return address to to NBIO IOHC config data
 */
GNB_HANDLE * GetGnbHandle (
  SIL_CONTEXT *SilContext
  )
{
  GNB_HANDLE            *GnbHandle;
  NORTH_BRIDGE_PCIE_SIB *Pcie = (NORTH_BRIDGE_PCIE_SIB *)NbioGetPcieTopology(SilContext);
  PCIe_PLATFORM_CONFIG  *PciePlatformConfig = &Pcie->PciePlatformConfig;
  GnbHandle = (GNB_HANDLE *) PcieConfigGetChild(DESCRIPTOR_SILICON, &PciePlatformConfig->Header);
  return GnbHandle;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get child descriptor of specific type
 *
 *
 * @param[in]       Type            Descriptor type
 * @param[in]       Descriptor      Pointer to buffer to pass information to callback
 */
static
PCIe_DESCRIPTOR_HEADER *
NbioGetPeer (
  uint32_t                      Type,
  PCIe_DESCRIPTOR_HEADER        *Descriptor
  )
{
  while ((Descriptor->DescriptorFlags & Type) == 0) {
    if (Descriptor->Peer != 0) {
      Descriptor = (PCIe_DESCRIPTOR_HEADER *) ((uint8_t *) Descriptor + Descriptor->Peer);
    } else {
      return NULL;
    }
  }
  return Descriptor;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get GNB handle of alternate host bridge (e.g. MI200)
 *
 *
 * @param[in]       Pcie           Pointer to global PCIe configuration
 */
GNB_HANDLE *
NbioGetAltHandle (
  PCIe_PLATFORM_CONFIG            *Pcie
  )
{
  GNB_HANDLE            *NbioHandle;
  NbioHandle = (GNB_HANDLE *) NbioGetPeer(DESCRIPTOR_SILICON, &Pcie->Header);
  return NbioHandle;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get GNB handle from PCIe platform configuration
 *
 *
 * @param[in]       Pcie           Pointer to global PCIe configuration
 */
GNB_HANDLE * NbioGetHandle (PCIe_PLATFORM_CONFIG *Pcie)
{
  GNB_HANDLE            *NbioHandle;
  NbioHandle = (GNB_HANDLE *) PcieConfigGetChild(DESCRIPTOR_SILICON, &Pcie->Header);
  return NbioHandle;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get IOHC pointer on next socket
 *
 *
 * @param   Iohc        Pointer to current Iohc instance
 * @retval  pointer IOHC on next socket
 */
GNB_HANDLE * NbioGetNextSocket (GNB_HANDLE *GnbHandle)
{
  GNB_HANDLE           *CurrentGnbHandle;
  uint32_t             CurrentSocket;

  CurrentGnbHandle = GnbHandle;
  CurrentSocket = CurrentGnbHandle->SocketId;
  do {
    CurrentGnbHandle = GnbGetNextHandle(CurrentGnbHandle);
    if (CurrentGnbHandle != NULL) {
      if (CurrentSocket != CurrentGnbHandle->SocketId) {
        break;
      }
    }
  } while (CurrentGnbHandle != NULL);
  return CurrentGnbHandle;
}

/*----------------------------------------------------------------------------------------*/
/*
 * Get PCI_ADDR of IOHC
 *
 *
 * @param    Iohc  Pointer to IOHC config data
 * @retval   PCI Address of IOHC
 */

PCI_ADDR NbioGetHostPciAddress (GNB_HANDLE *GnbHandle)
{
  return GnbHandle->Address;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check Port Status
 *
 * @param[in]  Engine          Pointer to engine config descriptor
 * @param[in]  PortStatus      Check if status asserted for port
 * @retval                     true if status asserted
 */
bool
PcieConfigCheckPortStatus (PCIe_ENGINE_CONFIG *Engine, uint32_t PortStatus)
{
  return (Engine->InitStatus & PortStatus) == 0 ? false : true;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set/Reset port status
 *
 * @param[in]  Engine          Pointer to engine config descriptor
 * @param[in]  SetStatus       SetStatus
 * @param[in]  ResetStatus     ResetStatus
 *
 */
uint16_t
PcieConfigUpdatePortStatus (
  PCIe_ENGINE_CONFIG               *Engine,
  PCIe_ENGINE_INIT_STATUS          SetStatus,
  PCIe_ENGINE_INIT_STATUS          ResetStatus
  )
{
  Engine->InitStatus |= SetStatus;
  Engine->InitStatus &= (~ResetStatus);
  return Engine->InitStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute callback on all descriptor of specific type
 *
 * @param   SilContext           A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param   InDescriptorFlags    Include descriptor flags
 * @param   OutDescriptorFlags   Exclude descriptor flags
 * @param   TerminationFlags     Termination flags
 * @param   Callback             Pointer to callback function
 * @param   Buffer               Pointer to buffer to pass information to callback
 * @param   Pcie                 Pointer to global PCIe configuration
 */

SIL_STATUS
PcieConfigRunProcForAllDescriptors (
  SIL_CONTEXT           *SilContext,
  uint32_t              InDescriptorFlags,
  uint32_t              OutDescriptorFlags,
  uint32_t              TerminationFlags,
  PCIe_RUN_ON_DESCRIPTOR_CALLBACK  Callback,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  SIL_STATUS              ConfigStatus;
  SIL_STATUS              Status;
  PCIe_DESCRIPTOR_HEADER  *Descriptor;

  ConfigStatus = SilPass;
  Descriptor = PcieConfigGetChild(InDescriptorFlags & DESCRIPTOR_ALL_TYPES, &Pcie->Header);
  while (Descriptor != NULL) {
    if ((InDescriptorFlags & Descriptor->DescriptorFlags) != 0 &&
      (OutDescriptorFlags && Descriptor->DescriptorFlags) == 0) {
      Status = Callback(SilContext, Descriptor, Buffer, Pcie);
      SIL_STATUS_UPDATE(Status, ConfigStatus);
    }
    Descriptor = (PCIe_DESCRIPTOR_HEADER *) PcieConfigGetNextTopologyDescriptor(Descriptor, TerminationFlags);
  }
  return ConfigStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute callback on all wrappers in topology
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DescriptorFlags     Wrapper Flags
 * @param   Callback            Pointer to callback function
 * @param   Pcie                Pointer to global PCIe configuration
 */

SIL_STATUS
PcieConfigRunProcForAllWrappers (
  SIL_CONTEXT                   *SilContext,
  uint32_t                      DescriptorFlags,
  PCIe_RUN_ON_WRAPPER_CALLBACK  Callback,
  void                          *Buffer,
  PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  SIL_STATUS            SilStatus;
  SIL_STATUS            Status;
  PCIe_WRAPPER_CONFIG   *Wrapper = NULL;
  GNB_HANDLE            *GnbHandle;

  SilStatus = SilPass;
  GnbHandle = NbioGetHandle(Pcie);
  //Find first wrapper that is not NULL
  while (GnbHandle != NULL) {
    Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &Pcie->Header);
    if (Wrapper != NULL) {
      break;  // Terminate the loop when we find a Wrapper;
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }
  while (Wrapper != NULL) {
    if ((DescriptorFlags & DESCRIPTOR_ALL_WRAPPERS & Wrapper->Header.DescriptorFlags) != 0) {
      Status = Callback(SilContext, Wrapper, Buffer, Pcie);
      SIL_STATUS_UPDATE(Status, SilStatus);
    }
    Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor(Wrapper, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
  return SilStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute callback on all wrappers in NBIO
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DescriptorFlags     Wrapper Flags
 * @param   Callback            Pointer to callback function
 * @param   Buffer              Pointer to buffer to pass information to callback
 * @param   Pcie                Pointer to global PCIe configuration
 */

void
PcieConfigRunProcForAllWrappersInNbio (
  SIL_CONTEXT                   *SilContext,
  uint32_t                      DescriptorFlags,
  PCIe_RUN_ON_WRAPPER_CALLBACK2 Callback,
  void                          *Buffer,
  GNB_HANDLE                    *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;

  Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(GnbHandle->Header));
  while ((Wrapper != NULL) && (GnbHandle == (GNB_HANDLE *) PcieConfigGetParent(DESCRIPTOR_SILICON,
    &(Wrapper->Header)
    ))) {
    if ((DescriptorFlags & DESCRIPTOR_ALL_WRAPPERS & Wrapper->Header.DescriptorFlags) != 0) {
      Callback(SilContext, Wrapper, Buffer, GnbHandle);
    }
    Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor(Wrapper, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Execute callback on all engine in topology
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DescriptorFlags     Engine flags.
 * @param   Callback            Pointer to callback function
 * @param   Buffer              Pointer to buffer to pass information to callback
 * @param   Pcie                Pointer to global PCIe configuration
 */

void
PcieConfigRunProcForAllEngines (
  SIL_CONTEXT                   *SilContext,
  uint32_t                      DescriptorFlags,
  PCIe_RUN_ON_ENGINE_CALLBACK   Callback,
  void                          *Buffer,
  PCIe_PLATFORM_CONFIG          *Pcie
  )
{

  PCIe_ENGINE_CONFIG  *Engine;
  GNB_HANDLE          *GnbHandle;

  Engine = NULL;
  GnbHandle = NbioGetHandle(Pcie);
  //Find first wrapper that is not NULL
  while (GnbHandle != NULL) {
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &GnbHandle->Header);
    if (Engine != NULL) {
      break;  // Terminate the loop when we find an Engine;
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  while (Engine != NULL) {
    if (!((DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0 && !PcieLibIsEngineAllocated(Engine))) {
      if ((Engine->Header.DescriptorFlags & DESCRIPTOR_ALL_ENGINES & DescriptorFlags) != 0) {
        Callback(SilContext, Engine, Buffer, Pcie);
      }
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute callback on all engine in wrapper
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DescriptorFlags     Engine flags.
 * @param   Callback            Pointer to callback function
 * @param   Buffer              Pointer to buffer to pass information to callback
 * @param   Pcie                Pointer to global PCIe configuration
 */

void
PcieConfigRunProcForAllEnginesInWrapper (
  SIL_CONTEXT                   *SilContext,
  uint32_t                      DescriptorFlags,
  PCIe_RUN_ON_ENGINE_CALLBACK2  Callback,
  void                          *Buffer,
  PCIe_WRAPPER_CONFIG           *Wrapper
  )
{

  PCIe_ENGINE_CONFIG  *Engine;
  Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header));

  while ((Engine != NULL) && (Wrapper == (PCIe_WRAPPER_CONFIG *) PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS,
    &(Engine->Header)
    ))) {
    if (!((DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0 && !PcieLibIsEngineAllocated(Engine))) {// @suppress("Symbol is not resolved")
      if ((Engine->Header.DescriptorFlags & DESCRIPTOR_ALL_ENGINES & DescriptorFlags) != 0) {
        Callback(SilContext, Engine, Buffer, Wrapper);
      }
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check is engine is active or potentially active
 *
 *
 *
 * @param[in]  Engine      Pointer to engine descriptor
 * @retval                 true  - engine active
 * @retval                 false - engine not active
 */
bool PcieConfigIsActivePcieEngine (PCIe_ENGINE_CONFIG *Engine)
{
  bool Result;

  assert(PcieConfigIsPcieEngine(Engine));
  Result = false;
  if (PcieConfigIsEngineAllocated(Engine)) {
    if (PcieConfigCheckPortStatus(Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
      (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled &&
      Engine->Type.Port.PortData.LinkHotplug != PcieHotplugInboard)) {
      Result = true;
    }
  }
  return Result;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Locate SB engine on wrapper
 *
 *
 *
 * @param[in]  Wrapper     Pointer to wrapper config descriptor
 * @retval                 SB engine pointer or NULL
 */
PCIe_ENGINE_CONFIG *PcieConfigLocateSbEngine (PCIe_WRAPPER_CONFIG *Wrapper)
{
  PCIe_ENGINE_CONFIG  *EngineList;
  EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header));
  while (EngineList != NULL) {
    if (PcieConfigIsSbPcieEngine(EngineList)) {
      return EngineList;
    }
    /* coverity[ptr_arith] */
    EngineList = PcieLibGetNextDescriptor(EngineList);
  }
  return NULL;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get number of complexes in platform topology configuration
 *
 *
 *
 * @param[in] ComplexList  First complex configuration in complex configuration array
 * @retval                 Number of Complexes
 *
 */
size_t
PcieInputParserGetNumberOfComplexes (
  PCIe_COMPLEX_DESCRIPTOR       *ComplexList
  )
{
  size_t                Result;
  Result = 0;
  while (ComplexList != NULL) {
    Result++;
    ComplexList = PcieInputParsetGetNextDescriptor(ComplexList);
  }
  return Result;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get number of PCIe engines in given complex
 *
 *
 *
 * @param[in] Complex     Complex configuration
 * @retval                Number of Engines
 */
static
size_t
PcieInputParserGetLengthOfPcieEnginesList (
  PCIe_COMPLEX_DESCRIPTOR       *Complex
  )
{
  size_t                 Result;
  PCIe_PORT_DESCRIPTOR  *PciePortList;
  Result = 0;
  PciePortList = Complex->PciePortList;
  while (PciePortList != NULL) {
    Result++;
    PciePortList = PcieInputParsetGetNextDescriptor(PciePortList);
  }
  return Result;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get number of DDI engines in given complex
 *
 *
 *
 * @param[in] Complex     Complex configuration
 * @retval                Number of Engines
 */
static
size_t
PcieInputParserGetLengthOfDdiEnginesList (
  PCIe_COMPLEX_DESCRIPTOR       *Complex
  )
{
  size_t                 Result;
  PCIe_DDI_DESCRIPTOR  *DdiLinkList;
  Result = 0;
  DdiLinkList = Complex->DdiLinkList;
  while (DdiLinkList != NULL) {
    Result++;
    DdiLinkList = PcieInputParsetGetNextDescriptor(DdiLinkList);
  }
  return Result;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get number of engines in given complex
 *
 *
 *
 * @param[in] Complex     Complex configuration header
 * @retval                Number of Engines
 */
size_t
PcieInputParserGetNumberOfEngines (
  PCIe_COMPLEX_DESCRIPTOR         *Complex
  )
{
  size_t                     Result;

  Result = PcieInputParserGetLengthOfDdiEnginesList(Complex) +
    PcieInputParserGetLengthOfPcieEnginesList(Complex);
  return Result;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get Complex descriptor by index from given Platform configuration
 *
 *
 *
 * @param[in] ComplexList Platform topology configuration
 * @param[in] Index       Complex descriptor Index
 * @retval                Pointer to Complex Descriptor
 */
PCIe_COMPLEX_DESCRIPTOR *
PcieInputParserGetComplexDescriptor (
  PCIe_COMPLEX_DESCRIPTOR         *ComplexList,
  size_t                          Index
  )
{
  assert(Index < (PcieInputParserGetNumberOfComplexes(ComplexList)));
  return &ComplexList[Index];
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get Engine descriptor from given complex by index
 *
 *
 *
 * @param[in] Complex     Complex descriptor
 * @param[in] Index       Engine descriptor index
 * @retval                Pointer to Engine Descriptor
 */
PCIe_ENGINE_DESCRIPTOR *
PcieInputParserGetEngineDescriptor (
  PCIe_COMPLEX_DESCRIPTOR         *Complex,
  size_t                           Index
  )
{
  size_t PcieListlength;

  assert(Index < (PcieInputParserGetNumberOfEngines(Complex)));
  PcieListlength = PcieInputParserGetLengthOfPcieEnginesList(Complex);
  if (Index < PcieListlength) {
    return (PCIe_ENGINE_DESCRIPTOR *) &((Complex->PciePortList)[Index]);
  } else {
    return (PCIe_ENGINE_DESCRIPTOR *) &((Complex->DdiLinkList)[Index - PcieListlength]);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump engine configuration
 *
 *
 * @param[in]  EngineList           Engine Configuration
 */
void PcieConfigEngineDebugDump (PCIe_ENGINE_CONFIG *EngineList)
{
  if (EngineList->EngineData.StartLane != 255) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Descriptor Flags - 0x%08x\n", EngineList->Header.DescriptorFlags);
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "        Engine Type - PCIe Port\n        Start Lane - %d  |||  End Lane - %d\n",
      EngineList->EngineData.StartLane,
      EngineList->EngineData.EndLane
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Scratch        - %d\n", EngineList->Scratch);
    if (EngineList->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS ||
      EngineList->InitStatus == INIT_STATUS_ALLOCATED) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Init Status    - 0x%08x\n", EngineList->InitStatus);
    } else {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Init Status    - Unknown (0x%x)\n", EngineList->InitStatus);
    }
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "        PCIe port configuration:\n");
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Port Training - %s\n",
      (EngineList->Type.Port.PortData.PortPresent == PortDisabled) ? "Disable" : "Enabled"
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Requested PCI Dev Number   - %d\n",
      EngineList->Type.Port.PortData.DeviceNumber
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Requested PCI Func Number  - %d\n",
      EngineList->Type.Port.PortData.FunctionNumber
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          PCI Address - %d:%d:%d\n",
      EngineList->Type.Port.Address.Address.Bus,
      EngineList->Type.Port.Address.Address.Device,
      EngineList->Type.Port.Address.Address.Function
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          EngineId - %d\n",
      EngineList->Type.Port.PcieBridgeId
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Misc Control  - 0x%02x\n",
      EngineList->Type.Port.PortData.MiscControls
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Hotplug - %s\n",
      (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugDisabled) ? "Disabled" : (
        (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugBasic) ? "Basic" : (
          (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugServerExpress) ? "Server" : (
            (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugEnhanced) ? "Enhanced" : (
              (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugServerEntSSD) ? "ServerSsd" : (
                (EngineList->Type.Port.PortData.LinkHotplug == PcieHotplugInboard) ? "Inboard" : "Unknown")))))
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          ASPM    - %s\n",
      (EngineList->Type.Port.PortData.LinkAspm == AspmDisabled) ? "Disabled" : (
        (EngineList->Type.Port.PortData.LinkAspm == AspmL0s) ? "L0s" : (
          (EngineList->Type.Port.PortData.LinkAspm == AspmL1) ? "L1" :  (
            (EngineList->Type.Port.PortData.LinkAspm == AspmL0sL1) ? "L0s & L1" : "Unknown")))
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          Speed   - %d\n",
      EngineList->Type.Port.PortData.LinkSpeedCapability
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "          SlotNum - %d\n",
      EngineList->Type.Port.PortData.SlotNum
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      EsmSpeedBump   - %d\n", EngineList->Type.Port.EsmSpeedBump);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump wrapper configuration
 *
 *
 * @param[in]  WrapperList           Wrapper Configuration
 */
void PcieConfigWrapperDebugDump (PCIe_WRAPPER_CONFIG *WrapperList)
{
  PCIe_ENGINE_CONFIG    *EngineList;
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "      <---------Wrapper - %d Config ------->\n", WrapperList->WrapId);
  NBIO_TRACEPOINT( SIL_TRACE_INFO,
    "        PHY lane range  - %d - %d\n",
    WrapperList->StartPhyLane,
    WrapperList->EndPhyLane
    );
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Start DXIO lane - %02d\n", WrapperList->StartDxioLane);
  EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(WrapperList->Header));
  while (EngineList != NULL) {
    PcieConfigEngineDebugDump(EngineList);
    /* coverity[ptr_arith] */
    EngineList = PcieLibGetNextDescriptor(EngineList);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump configuration to debug out
 *
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
void PcieConfigDebugDump (PCIe_PLATFORM_CONFIG *Pcie)
{
  PCIe_SILICON_CONFIG   *SiliconList;
  PCIe_WRAPPER_CONFIG   *WrapperList;
  PCIe_COMPLEX_CONFIG   *ComplexList;
  int16_t               Current_DieNumber;
  int16_t               Current_SocketId;
  ComplexList = (PCIe_COMPLEX_CONFIG *) PcieConfigGetChild(DESCRIPTOR_COMPLEX, &(Pcie->Header));
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "<-------------- PCIe Config Start------------>\n");
#if DEBUG_HEX_DUMP
  xUslDumpBuffer((void *) ComplexList, 0x1800, 1);
#endif
  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "  PSPP Policy - %s\n",
    (Pcie->PsppPolicy == SilPsppPowerSaving) ? "Power Saving" :
    (Pcie->PsppPolicy == SilPsppBalance) ? "Balanced Mode" : (
      (Pcie->PsppPolicy == SilPsppPerformance) ? "Performance" : (
        (Pcie->PsppPolicy == SilPsppDisabled) ? "Disabled" : "Unknown"))
    );

  Current_DieNumber = -1;
  Current_SocketId = -1;

  while (ComplexList != NULL) {
    SiliconList = (PCIe_SILICON_CONFIG *) PcieConfigGetChild(DESCRIPTOR_SILICON, &(ComplexList->Header));
    while (SiliconList != NULL) {
      if (Current_DieNumber != SiliconList->DieNumber || Current_SocketId != SiliconList->SocketId) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO,
          " <-------------Socket %d Die %d Start ------------->\n",
          SiliconList->SocketId,
          SiliconList->DieNumber
          );
        Current_DieNumber = SiliconList->DieNumber;
        Current_SocketId = SiliconList->SocketId;
      }
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        "         RBIndex      - %d    (Logical RB %d)\n",
        SiliconList->RBIndex,
        SiliconList->LogicalRBIndex
        );
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        "         Bus Range    - %x - %x\n",
        SiliconList->Address.Address.Bus,
        SiliconList->BusNumberLimit
        );
      if (SiliconList->SmuPresent != 0) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "         SmuPresent   - YES\n");
      }
      if (SiliconList->SbPresent != 0) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "         SbPresent    - YES\n", SiliconList->SbPresent);
      }
      WrapperList = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(SiliconList->Header));
      while (WrapperList != NULL) {
        PcieConfigWrapperDebugDump(WrapperList);
        /* coverity[ptr_arith] */
        WrapperList = PcieLibGetNextDescriptor(WrapperList);
      }
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "    <---------- Silicon Config End ---------->\n");
      /* coverity[ptr_arith] */
      SiliconList = PcieLibGetNextDescriptor(SiliconList);
    }
    if ((ComplexList->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_TOPOLOGY) == 0) {
      /* coverity[ptr_arith] */
      ComplexList++;
    } else {
      ComplexList = NULL;
    }
  }
  SiliconList = NbioGetAltHandle(Pcie);
  while (SiliconList != NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "    <---------- Silicon Config Start -------->\n");
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Descriptor Flags - 0x%08x\n", SiliconList->Header.DescriptorFlags);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Socket Number  - %d\n", SiliconList->SocketId);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Die Number     - %d\n", SiliconList->DieNumber);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      RBIndex        - %d\n", SiliconList->RBIndex);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      LogicalRBIndex - %d\n", SiliconList->LogicalRBIndex);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Instance ID    - %d\n", SiliconList->InstanceId);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Start Lane     - %d\n", SiliconList->StartLane);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      End Lane       - %d\n", SiliconList->EndLane);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Start Bus      - %d\n", SiliconList->Address.Address.Bus);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      End Bus        - %d\n", SiliconList->BusNumberLimit);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      SmuPresent     - %d\n", SiliconList->SmuPresent);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "      SbPresent      - %d\n", SiliconList->SbPresent);
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "      Host PCI Address - %d:%d:%d\n",
      SiliconList->Address.Address.Bus,
      SiliconList->Address.Address.Device,
      SiliconList->Address.Address.Function
      );

    WrapperList = ((PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &((SiliconList)->Header)));
    while (WrapperList != NULL) {
      PcieConfigWrapperDebugDump(WrapperList);
      WrapperList = PcieLibGetNextDescriptor(WrapperList);
    }
    SiliconList = PcieLibGetNextDescriptor(SiliconList);
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "<-------------- PCIe Config End-------------->\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to user engine descriptor
 *
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 */
void PcieUserDescriptorConfigDump (PCIe_ENGINE_DESCRIPTOR *EngineDescriptor)
{

  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "  Engine Type - %s\n",
    (EngineDescriptor->EngineData.EngineType == PciePortEngine) ? "PCIe Port" : (
      (EngineDescriptor->EngineData.EngineType == PcieDdiEngine) ? "DDI Link" : (
        (EngineDescriptor->EngineData.EngineType == PcieUnusedEngine) ? "Unused" : "Invalid"))
    );
  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
    EngineDescriptor->EngineData.StartLane,
    EngineDescriptor->EngineData.EndLane
    );
  if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, \
      "    PortPresent - %d\n    ChannelType  - %d\n    DeviceNumber - %d\n \
                FunctionNumber - %d\n    LinkSpeedCapability - %d\n    LinkAspm - %d\n \
                LinkHotplug - %d\n    ResetId - %d\n    SB link - %d\n    MiscControls - 0x%02x\n",
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.PortPresent,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.ChannelType,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.DeviceNumber,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.FunctionNumber,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkSpeedCapability,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspm,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkHotplug,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.ResetId,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.SbLink,
      ((PCIe_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls
      );
  }
  if (EngineDescriptor->EngineData.EngineType == PcieDdiEngine) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "    ConnectorType - %d\n    AuxIndex  - %d\n    HdpIndex - %d\n",
      ((PCIe_DDI_DESCRIPTOR *) EngineDescriptor)->Ddi.ConnectorType,
      ((PCIe_DDI_DESCRIPTOR *) EngineDescriptor)->Ddi.AuxIndex,
      ((PCIe_DDI_DESCRIPTOR *) EngineDescriptor)->Ddi.HdpIndex
      );
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Helper function to dump input configuration to debug out
 *
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 */
void PcieUserConfigConfigDump (PCIe_COMPLEX_DESCRIPTOR *ComplexDescriptor)
{
  PCIe_ENGINE_DESCRIPTOR      *EngineDescriptor;
  PCIe_COMPLEX_DESCRIPTOR     *CurrentComplexDescriptor;
  size_t                      ComplexIndex;
  size_t                      Index;
  size_t                      NumberOfEngines;
  size_t                      NumberOfComplexes;

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "<---------- PCIe User Config Start------------->\n");

  NumberOfComplexes = PcieInputParserGetNumberOfComplexes(ComplexDescriptor);
  for (ComplexIndex = 0; ComplexIndex < NumberOfComplexes; ++ComplexIndex) {
    CurrentComplexDescriptor = PcieInputParserGetComplexDescriptor(ComplexDescriptor, ComplexIndex);
    NumberOfEngines = PcieInputParserGetNumberOfEngines(CurrentComplexDescriptor);
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "  ComplexDescriptor Node - %d\n  NumberOfEngines - %d\n",
      ComplexDescriptor->SocketId,
      NumberOfEngines
      );

    for (Index = 0; Index < NumberOfEngines; Index++) {
      EngineDescriptor = PcieInputParserGetEngineDescriptor(ComplexDescriptor, Index);
      PcieUserDescriptorConfigDump(EngineDescriptor);
    }
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "<---------- PCIe User Config End-------------->\n");
}
