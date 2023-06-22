/**
 *  @file MpioCfgPoints.c
 *  @brief Callouts from MPIO initialization
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <xSIM.h>
#include <SilSocLogicalId.h>
#include <Mpio/Common/Deli.h>
#include <NBIO/NbioIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include "MpioInitLib.h"
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"
#include "MpioLib.h"

/**--------------------------------------------------------------------
 * PcieAriInit
 *
 * @brief Interface to configure AriCapabilitySize on PCIE interface
 *
 * @details tbd
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 */
void
PcieAriInit (
  GNB_HANDLE  *GnbHandle
  )
{
  NBIO_IP2IP_API        *NbioIp2Ip;

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "PcieAriInit Enter\n");

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
}

/**--------------------------------------------------------------------
 * MpioCfgGetStrapListSize
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket and is used
 * to configure ports after ports are mapped and before reconfig.
 *
 * @param[in]  GnbHandle    Pointer to Silicon Descriptor for this node
 * @param[in]  PortPointer  Pointer to first topology entry
 *
 * @returns Size of ancillary data required to store the strap subheap
 * @retval uint32_t Numeric heap size returned in bytes? (not sure)
 **/
uint32_t
MpioCfgGetStrapListSize (
  GNB_HANDLE            *GnbHandle,
  MPIO_PORT_DESCRIPTOR  *PortPointer
  )
{
  return 0;
}

/**--------------------------------------------------------------------
 * MpioCfgAddStrapList
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket and is used
 * after ports are mapped and before reconfig.
 *
 * @param[in]  GnbHandle      Pointer to Silicon Descriptor for this node
 * @param[in]  AncillaryData  Pointer to next available entry in Ancillary Data for this node
 * @param[in]  PortList       Pointer to topology structures for this node
 *
 * @returns The size of the strap list after adding ancillary data.
 * @retval uint32_t Numeric strap list size returned in bytes? (not sure)
 **/
uint32_t
MpioCfgAddStrapList (
  GNB_HANDLE             *GnbHandle,
  void                   *AncillaryData,
  MPIO_PORT_DESCRIPTOR   *PortList
  )
{
  return 0;
}

/**-------------------------------------------------------------------
 * MpioCfgBeforeReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket after ports
 * are mapped and before reconfig
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeReconfig (
  GNB_HANDLE       *GnbHandle
  )
{

  GNB_HANDLE                *LocalHandle;
  uint32_t                  SocketId;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API            *NbioIp2Ip;

  MPIO_TRACEPOINT (SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio (
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgBeforeReconfigWrapper,
        NULL,
        LocalHandle
        );

    }
    LocalHandle = GnbGetNextHandle (LocalHandle);
  }
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports after reconfig and before training
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterReconfig (
  GNB_HANDLE       *GnbHandle
  )
{
  PCIe_DPC_STATUS_DATA            *DpcStatusData;
  GNB_HANDLE                      *LocalHandle;
  uint32_t                        SocketId;
  MPIOCLASS_INPUT_BLK             *SilData;
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  MPIO_TRACEPOINT (SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);
  DpcStatusData = &SilData->DpcStatusData;

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio (
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgAfterReconfigWrapper,
        DpcStatusData,
        LocalHandle
        );
    }
    LocalHandle = GnbGetNextHandle (LocalHandle);
  }
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

//=========================================================================================
// Timepoint before MPIO firmware begins training (after reset deasserts)
//=========================================================================================

/**-------------------------------------------------------------------
 * MpioCfgBeforeTraining
 *
 * @brief Interface to configure DXIO/PCIe ports after ports are reconfig and resets deasserted
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeTraining (
  GNB_HANDLE       *GnbHandle
  )
{
}

/*
 *=========================================================================================
 * Timepoint before DXIO firmware intialization begins
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgBeforeDxioInit
 *
 * @brief Configuration Timepoint before DXIO firmware initialization starts
 *
 * @param[in]  Pcie                 Pointer silicon complex descriptor
 * @param[in]  ComplexDescriptor    Pointer to platform complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeDxioInit (
  PCIe_PLATFORM_CONFIG      *Pcie,
  MPIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  )
{

  GNB_HANDLE                *GnbHandle;
  MPIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, " Socket %d\n", LocalDescriptor->SocketId);
    MpioManageTopology (LocalDescriptor);
    MpioXferTable->MpioTopologyWorkarounds (LocalDescriptor);

    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      if (GnbHandle->SocketId == LocalDescriptor->SocketId) {
      }
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
  }
}

/*
 *=========================================================================================
 * Timepoint after DXIO firmware initialization completes
 *=========================================================================================
 */
 /**-------------------------------------------------------------------
 *
 * PcieCommonEngineGetDeliInfoCallback
 *
 * @brief tbd
 *
 * @details tbd
 *
 * @param[in] PCIe_PLATFORM_CONFIG       Pointer silicon complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterDxioInit (
  PCIe_PLATFORM_CONFIG                 *Pcie
  )
{
  GNB_HANDLE                         *GnbHandle;
  uint32_t                           PackageType;
  MPIOCLASS_INPUT_BLK                *SilData;
  MPIO_COMMON_2_REV_XFER_BLOCK       *MpioXferTable;
  NBIO_IP2IP_API                     *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  PackageType = xUslGetPackageType ();

  // Workaround to enable GCC build
  unsigned long tempWa = 0;
  tempWa += tempWa + (unsigned long) PackageType;
  tempWa += 0x2;
  // workaround

  NbioIp2Ip->PcieConfigDebugDump(Pcie);
  NbioIp2Ip->PcieConfigRunProcForAllWrappers (
               DESCRIPTOR_ALL_WRAPPERS,
               MpioXferTable->PcieCommonCoreConfiguration,
               NULL,
               Pcie);
  NbioIp2Ip->PcieConfigRunProcForAllEngines (
               DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
               MpioXferTable->PcieCommonEngineConfiguration, NULL, Pcie);
               PcieConfigRunProcForAllEngines (DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
               MpioXferTable->PcieCommonEngineGetDeliInfo, NULL, Pcie);

  GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  while (GnbHandle != NULL) {
    MpioXferTable->InitBusRangesAndAri (GnbHandle, Pcie, SilData->CfgPcieAriSupport);
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }
}

 /**-------------------------------------------------------------------
 *
 * Usb4LaneNumberCfgCallback
 *
 * @brief Configuration Usb4 Lane number before DXIO firmware initialization
 *
 * @details tbd
 *
 * @param[in]  Counter                 Counter of Controller number
 * @param[in]  AskEntry                Pointer to ASK entry
 * @param[in]  TopologyEntry           Pointer to engine topology
 *
 * @returns SIL_STATUS
 * @retval SilUnsupported should always be returned.
 **/
SIL_STATUS
Usb4LaneNumberCfgCallback(
  uint8_t                      Counter,
  FW_ASK_STRUCT                *AskEntry,
  MPIO_PORT_DESCRIPTOR         *TopologyEntry
)
{
  return SilUnsupported;
}
