/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioCfgPoints.c
 *  @brief Callouts from MPIO initialization
 */

#include <string.h>
#include <xSIM.h>
#include <SilSocLogicalId.h>
#include <Nbio/NbioIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include "MpioInitLib.h"
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"
#include "MpioLib.h"
#include "Deli.h"

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
  PCIe_ENGINE_CONFIG   *PcieEngine;
  PCIe_WRAPPER_CONFIG  *PcieWrapper;
  NBIO_IP2IP_API        *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieAriInit Enter\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

#ifdef ENABLE_PCIE_ARI_INIT_1
  if ((PcdGetBool(PcdCfgPcieAriSupport)) && (PcdGetBool(PcdPcieAriForwardingEnable))) {
#else //ENABLE_PCIE_ARI_INIT_1
  if (0) {
#endif //ENABLE_PCIE_ARI_INIT_1
    PcieWrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS,
      &(GnbHandle->Header)
      ));
    while (PcieWrapper != NULL) {
      PcieEngine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES,
        &(PcieWrapper->Header)
        ));
      while (PcieEngine != NULL) {
        if (PcieLibIsEngineAllocated(PcieEngine)) {
#ifdef ENABLE_PCIE_ARI_INIT_2
          PcieAriInitCallback(PcieEngine, NULL);
#endif
        }
        PcieEngine = PcieLibGetNextDescriptor(PcieEngine);
      }
      PcieWrapper = PcieLibGetNextDescriptor(PcieWrapper);
    }
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
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

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgBeforeReconfigWrapper,
        NULL,
        LocalHandle
        );

    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
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
  GNB_HANDLE                      *LocalHandle;
  uint32_t                        SocketId;
  AFTER_RECONFIG_BUFFER           AfterReconfigBuffer;
  MPIOCLASS_INPUT_BLK             *SilData;
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  AfterReconfigBuffer.DpcStatusData = NULL;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);
  AfterReconfigBuffer.DpcStatusData = &SilData->DpcStatusData;

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgAfterReconfigWrapper,
        &AfterReconfigBuffer,
        LocalHandle
        );
    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
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

/**-------------------------------------------------------------------
 *
 * PrintDeliInfo
 *
 * @brief Print the DXIO Error Log Interface (DELI) information for PCIe engine
 *
 * @details tbd
 *
 * @param[in] DXIO_FW_MP1_DELI_INFO
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
PrintDeliInfo (DXIO_FW_MP1_DELI_INFO *pDeliBuf)
{
  uint8_t LGSCount;

  if (pDeliBuf->Hdr.engType == 0) {  // Print the DELI log only for PCIe engine
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "\n //////// Header Data ////////\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Number of Dwords = %d\n", pDeliBuf->Hdr.numDWords);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Hdr.version);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "////////// Lane Group Data //////////\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Number of Lane Groupgs = %d\n", pDeliBuf->Dxio.numLGroups);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Dxio.version);
    for (LGSCount = 0; LGSCount < DXIO_FW_MP1_DELI_MAX_NUM_DXIO_LGS; LGSCount++) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Phy Start Lane = %d\n", pDeliBuf->Dxio.LGS[LGSCount].KPNP_startPhyLane);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Phy End Lane = %d\n", pDeliBuf->Dxio.LGS[LGSCount].KPNP_endPhyLane);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Lane Req Status = %x\n", pDeliBuf->Dxio.LGS[LGSCount].KPNP_LANE_REQ_STATUS);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Hardware Debug = %x\n", pDeliBuf->Dxio.LGS[LGSCount].KPMX_HWDEBUG);
    }
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "//////// PCIe Training Data /////////\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Proto.Pcie.version);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Link State = %x\n", pDeliBuf->Proto.Pcie.PCIE_link_state);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Link Speed = %d\n", pDeliBuf->Proto.Pcie.PCIE_link_speed);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Link Width = %d\n", pDeliBuf->Proto.Pcie.PCIE_link_width);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Link Active = %x\n", pDeliBuf->Proto.Pcie.PCIE_link_active);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Port Number = %x\n", pDeliBuf->Proto.Pcie.PCIE_port_num);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "SWRST_CONTROL = %x\n", pDeliBuf->Proto.Pcie.SWRST_CONTROL_6);
  }
}

/**-------------------------------------------------------------------
 *
 * GetDeliInfo
 *
 * @brief Get the DXIO Error Log Interface (DELI) information to print the log
 *
 * @details tbd
 *
 * @param[in] GNB_HANDLE
 * @param[in] PCIe_ENGINE_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
void
GetDeliInfo (
  GNB_HANDLE                *GnbHandle,
  PCIe_ENGINE_CONFIG        *Engine
  )
{
  uint32_t                        Response;
  uint32_t                        MpioArg[6];
  uint8_t                         DELIBuf[sizeof (DXIO_FW_MP1_DELI_INFO)];
  DXIO_FW_MP1_DELI_INFO           *pDeliBuf;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  memset((void *)DELIBuf, 0, sizeof (DELIBuf));

  memset(MpioArg, 0x00, sizeof (MpioArg));
  MpioArg[1] = 0;                                       // Address High
  MpioArg[2] = (uint32_t) (uintptr_t) DELIBuf;          //Address Low
  MpioArg[4] = (uint32_t) Engine->EngineData.StartLane; //StartLane

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "DELI INFO for InstanceId = %d, StartLane = %d \n",
    GnbHandle->InstanceId,
    Engine->EngineData.StartLane
    );

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  Response = MpioServiceRequestCommon(NbioIp2Ip->GetHostPciAddress(GnbHandle),
    BIOS_MPIO_MSG_GET_DELI_INFO,
    MpioArg,
    0
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);

  xUslDumpBuffer((void *)((uintptr_t)MpioArg[2]), sizeof (DELIBuf), 1);
  pDeliBuf = (DXIO_FW_MP1_DELI_INFO *) ((uintptr_t)MpioArg[2]);
  PrintDeliInfo(pDeliBuf);

}
/**----------------------------------------------------------------------------------------
 * MpioCfgEarlyLinkAuthentication
 *
 * @brief Interface to perform early link authentication
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns   Nothing
 * @retval    Nothing
 **/
void
MpioCfgEarlyLinkAuthentication (
  GNB_HANDLE       *GnbHandle,
  PCI_ADDR         EarlyLinkAddress
  )
{
  return;
}

/*
 *=========================================================================================
 * Timepoint before DXIO firmware initialization begins
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

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", LocalDescriptor->SocketId);
    MpioManageTopology(LocalDescriptor);
    MpioXferTable->MpioTopologyWorkarounds(LocalDescriptor);

    GnbHandle = NbioGetHandle(Pcie);
    while (GnbHandle != NULL) {
      if (GnbHandle->SocketId == LocalDescriptor->SocketId) {
      }
      GnbHandle = GnbGetNextHandle(GnbHandle);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
  }
}

/*
 *=========================================================================================
 * Timepoint after DXIO firmware initialization completes
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 *
 * Usb4LaneNumberCfg
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
Usb4LaneNumberCfg (
  uint8_t                      Counter,
  FW_ASK_STRUCT                *AskEntry,
  MPIO_PORT_DESCRIPTOR         *TopologyEntry
  )
{
  return SilUnsupported;
}
