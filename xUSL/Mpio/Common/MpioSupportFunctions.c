/**
 *  @file MpioSupportFunctions.c
 *  @brief Support functions for MPIO initialization library
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <NBIO/NbioIp2Ip.h>
#include "MpioLibLocal.h"
#include "MpioStructs.h"

/// Engine location callback
typedef struct {
  FW_ASK_STRUCT        *AskEntry; ///< Ask entry type
  PCIe_ENGINE_CONFIG   *Engine;   ///< Engine pointer
} ENGINE_LOCATION_CALLBACK;

/**--------------------------------------------------------------------
 *
 * FindEngineCallback
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
FindEngineCallback (
  PCIe_ENGINE_CONFIG   *Engine,
  void                 *Buffer,
  PCIe_WRAPPER_CONFIG  *Wrapper
  )
{
  ENGINE_LOCATION_CALLBACK    *EngineStruct;

  EngineStruct = (ENGINE_LOCATION_CALLBACK *) Buffer;
  if (EngineStruct->AskEntry->status.port == Engine->Type.Port.PortId) {
    EngineStruct->Engine = Engine;
  }
}

/**--------------------------------------------------------------------
 *
 * FindWrapperCallback
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
FindWrapperCallback (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  GNB_HANDLE            *GnbHandle
  )
{
  ENGINE_LOCATION_CALLBACK    *EngineStruct;
  NBIO_IP2IP_API              *NbioIp2Ip;

  EngineStruct = (ENGINE_LOCATION_CALLBACK *) Buffer;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  if ((EngineStruct->AskEntry->desc.startLaneIndex >= Wrapper->StartDxioLane) &&
      (EngineStruct->AskEntry->desc.startLaneIndex <= Wrapper->EndDxioLane)) {
    NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper (
                 DESCRIPTOR_PCIE_ENGINE,
                 FindEngineCallback,
                 EngineStruct,
                 Wrapper
                 );
  }
}

/**--------------------------------------------------------------------
 *
 * MpioFindEngineForAsk
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 * @returns Pcie engine pointer.
 * @retval Valid handle.
 **/
PCIe_ENGINE_CONFIG *
MpioFindEngineForAsk (
  GNB_HANDLE          *GnbHandle,
  FW_ASK_STRUCT       *AskEntry
  )
{
  ENGINE_LOCATION_CALLBACK  EngineStruct;
  GNB_HANDLE                *Handle;
  NBIO_IP2IP_API            *NbioIp2Ip;

  Handle = GnbHandle;
  EngineStruct.AskEntry = AskEntry;
  EngineStruct.Engine = NULL;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return NULL;
  }

  while (Handle != NULL) {
    NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio (
                 DESCRIPTOR_ALL_WRAPPERS,
                 FindWrapperCallback, (void *)
                 &EngineStruct,
                 Handle
                 );
    if (EngineStruct.Engine != NULL) {
      break;
    }
    Handle = GnbGetNextHandle (Handle);
  }
  return (EngineStruct.Engine);
}

/**--------------------------------------------------------------------
 *
 * MpioGetTopologyForSocket
 *
 * @brief Function to find the topology structure for a specific socket
 *
 * @param[in]     SocketNumber    Socket number to look for
 * @param[in]     UserComplex     Pointer to the top of the topology complex
 * @param[in]     PortList        Pointer to address of port list (for return)
 *
 * @returns Standard Opensil return status.
 * @retval SilPass indicates success.
 **/
SIL_STATUS
MpioGetTopologyForSocket (
  uint8_t                   SocketNumber,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_PORT_DESCRIPTOR      **PortList
  )
{
  SIL_STATUS               Status;
  MPIO_COMPLEX_DESCRIPTOR  *SocketTopology;

  Status = SilNotFound;
  SocketTopology = PlatformTopology;

  while (SocketTopology != NULL) {
    if (SocketTopology->SocketId == SocketNumber) {
      *PortList = SocketTopology->PciePortList;
      Status = SilPass;
      break;
    }
    SocketTopology = PcieConfigGetNextDataDescriptor(SocketTopology);
  }
  assert (Status == SilPass);
  return Status;
}
