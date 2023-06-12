/**
 *  @file MpioParser.c
 *  @brief Mpio Platform Topology Parser
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <NBIO/NbioIp2Ip.h>
#include <string.h>
#include "MpioInitLib.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"
#include "MpioPcie.h"


 /**--------------------------------------------------------------------
 *
 * InitAskEntryAttributes
 *
 * @brief Init Attibutes in an AskEntry from the Platform Topology
 *
 * @param [in] AskEntry Description for AskEntry
 * @param [in] TopologyEntry Description for TopologyEntry
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
InitAskEntryAttributes (
  FW3_LINK_ATTR          *Attributes,
  MPIO_PORT_DESCRIPTOR   *TopologyEntry
  )
{
  MPIOCLASS_INPUT_BLK       *SilData;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  Attributes->maxLinkSpeedCap = TopologyEntry->Port.LinkSpeedCapability;
  Attributes->targetLinkSpeed = TopologyEntry->Port.TargetLinkSpeed;
  Attributes->devfuncnumber = (TopologyEntry->Port.DeviceNumber << 3) | TopologyEntry->Port.FunctionNumber;
  Attributes->portPresent = TopologyEntry->Port.PortPresent;
  Attributes->earlyTrainLink = TopologyEntry->Port.MiscControls.SbLink;
  if (Attributes->earlyTrainLink == 1) {
    /*
	 * force PT to GEN1
	 */
    Attributes->targetLinkSpeed = 1;
    SilData->CfgEarlyLink = true;
  }
  Attributes->linkComplianceMode = TopologyEntry->Port.MiscControls.LinkComplianceMode;
  Attributes->linkHotplugType = TopologyEntry->Port.LinkHotplug;

  Attributes->isMasterPLL = TopologyEntry->Port.IsMasterPLL;

  if (TopologyEntry->Port.TXDeEmphasisOride) {
    Attributes->txDeEmphasisEn = 1;
    Attributes->txDeEmphasis = TopologyEntry->Port.TXDeEmphasis;
  }
  Attributes->txVetting = TopologyEntry->Port.TxVetting;
  Attributes->rxVetting = TopologyEntry->Port.RxVetting;
  Attributes->invertPolarityRx = TopologyEntry->Port.InvertPolarity;
  Attributes->invertPolarityTx = TopologyEntry->Port.InvertPolarity2;
}

 /**--------------------------------------------------------------------
 *
 * MpioTranslateLanesCallback
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
static
void
MpioTranslateLanesCallback (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  GNB_HANDLE            *GnbHandle
  )
{
  MPIO_PORT_INFO_CALLBACK    *PortInfo;

  PortInfo = (MPIO_PORT_INFO_CALLBACK *) Buffer;
  if ((PortInfo->StartLane >= Wrapper->StartPhyLane) && (PortInfo->StartLane <= Wrapper->EndPhyLane)) {
    if (Wrapper->Features.CoreReversed == 0) {
      if (PortInfo->StartLane <= PortInfo->EndLane) {
        PortInfo->MpioStartLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioEndLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = false;
      } else {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Core is NOT reversed, Link IS reversed\n");
        PortInfo->MpioEndLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioStartLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = true;
      }
    } else {
      if (PortInfo->StartLane <= PortInfo->EndLane) {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Core IS reversed, Link is NOT reversed\n");
        PortInfo->MpioEndLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioStartLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = true;
      } else {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Core IS reversed, Link IS reversed\n");
        PortInfo->MpioStartLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioEndLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = false;
      }
    }
    PortInfo->LanesFound = true;
  }
}

 /**--------------------------------------------------------------------
 *
 * MpioTranslateLanes
 *
 * @brief Get translated lane numbers based on logical lane inputs
 *
 * @param [in] GnbHandle     Pointer to silicon descriptory
 * @param [in] StartLane     Start lane for this port from Platform Topology
 * @param [in] EndLane End   lane for this port from Platform Topology
 * @param [in] StartDxioLane Pointer to storage for translated start lane
 * @param [in] EndDxioLane   Pointer to storage for translated end lane
 * @param [in] Reverse       Pointer to boolean identifier for reversed lanes
 *
 * @returns True or false result regarding lane translation.
 * @retval True indicates lane translation succeeded, false indicates failure.
 **/
bool
MpioTranslateLanes (
  GNB_HANDLE    *GnbHandle,
  uint8_t       StartLane,
  uint8_t       EndLane,
  uint8_t       *StartDxioLane,
  uint8_t       *EndDxioLane,
  bool          *Reverse
  )
{
  MPIO_PORT_INFO_CALLBACK    PortInfo;
  GNB_HANDLE                 *Handle;
  NBIO_IP2IP_API             *NbioIp2Ip;

  PortInfo.StartLane = StartLane;
  PortInfo.EndLane = EndLane;
  PortInfo.LanesFound = false;
  PortInfo.MpioStartLane = 0;
  PortInfo.MpioEndLane = 0;
  PortInfo.LinkReversal = false;
  Handle = GnbHandle;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return false;
  }

  while (Handle != NULL) {
    if (Handle->SocketId == GnbHandle->SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS,
                                                        MpioTranslateLanesCallback,
                                                        (void *)
                                                        &PortInfo,
                                                        Handle
                                                        );
    }
    if (PortInfo.LanesFound) {
      break;
    }
    Handle = GnbGetNextHandle (Handle);
  }

  if (PortInfo.LanesFound) {
    *StartDxioLane = PortInfo.MpioStartLane;
    *EndDxioLane = PortInfo.MpioEndLane;
    *Reverse = PortInfo.LinkReversal;
  } else {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Invalid request for StartLane %d, EndLane %d\n", StartLane, EndLane);
    *StartDxioLane = 0;
    *EndDxioLane = 0;
  }
  return PortInfo.LanesFound;
}

 /**--------------------------------------------------------------------
 *
 * GetGlobalAncillaryDataSize
 *
 * @brief Calculate the size of the global ancillary data
 *
 * @param [in] Pcie          Pointer to PCIe platform config structure
 * @param [in] SocketTopology  Pointer to Platform Topology
 * @param [in] StrapSize     Pointer to uint32_t to store size of PCIe strap values
 *
 * @returns Ancillary data size
 * @retval 32-bit size value
 **/
static
uint32_t
GetGlobalAncillaryDataSize (
  GNB_HANDLE              *GnbHandle,
  MPIO_PORT_DESCRIPTOR    *SocketTopology
  )
{
//  uint32_t                  StrapListSize;
  uint32_t                  GlobalDataSize;

  GlobalDataSize = 0;

//  MPIO_PHYOVR_TABLE     *PhyOverrideData;
  /*
   * Total Ancillary Data size includes per-port size calculated above plus additional storage for
   * Add Ancillary Data allocation for PSPP
   */
//  if (Pcie->PsppPolicy != SilPsppDisabled) {
//    GlobalDataSize += sizeof (MPIO_ANCILLARYPORTDATA_PSPP) + sizeof (MPIO_ANCILLARYDATA_SUBHEAP);
//    if (Pcie->PsppTuningParams2 != 0) {
//      GlobalDataSize += sizeof (MPIO_ANCILLARYPORTDATA_PSPP_2);
//    }
//  }
  /*
   * Add Ancillary Data allocation for PHY DATA (server)
   */
//  PhyOverrideData = (MPIO_PHYOVR_TABLE *)Pcie->PhyConfigData;
//  if (PhyOverrideData != NULL) {
//    GlobalDataSize += sizeof (MPIO_ANCILLARYDATA_SUBHEAP);
//    GlobalDataSize += PhyOverrideData->size; // size of config block
//  }

  /*
   * Add Ancillary Data allocation for STRAP settings
   */
//  StrapListSize = MpioCfgGetStrapListSize (GnbHandle, SocketTopology);
//  if (StrapListSize != 0) {
//    GlobalDataSize += StrapListSize;
//  }

  /*
   * Anciallary Data calculation is complete
   */
//  if (GlobalDataSize != 0) {
    /*
	 * Adding header size for whole ancillary blob
	 */
//    GlobalDataSize += sizeof (GENERIC_ANC_HEADER); // For Ancillary Data Header
//  }
  return GlobalDataSize;
}

 /**--------------------------------------------------------------------
 *
 * InitGlobalAncillaryData
 *
 * @brief Initialize the global ancillary data
 *
 * @param [in] GnbHandle       Pointer to silicon descriptory
 * @param [in] SocketTopology  Pointer to Platform Topology
 *
 * @returns Global data size
 * @retval 32-bit size value in bytes?
 **/
static
uint32_t
InitGlobalAncillaryData (
  GNB_HANDLE              *GnbHandle,
  MPIO_PORT_DESCRIPTOR    *SocketTopology,
  void                    *AncillaryData
  )
{
  uint32_t              StrapListSize;
  uint32_t              GlobalDataSize = 0;
  GENERIC_ANC_HEADER    *AncDataHeader;

  /*
   * Add Ancillary Data for STRAP settings
   */
  StrapListSize = MpioCfgAddStrapList (GnbHandle,
    ((uint8_t *)AncillaryData) + sizeof(GENERIC_ANC_HEADER), SocketTopology);
  StrapListSize += MpioAddAncDataOverride (GnbHandle, (uint8_t *)AncillaryData, SocketTopology);

  if (StrapListSize != 0) {
    GlobalDataSize = StrapListSize + sizeof(GENERIC_ANC_HEADER);
    AncDataHeader = (GENERIC_ANC_HEADER *) AncillaryData;
    AncDataHeader->type = ANCILLARY; // Anc Data blob header
    AncDataHeader->id = GnbHandle->SocketId; // unique id
    AncDataHeader->length = (uint8_t) (GlobalDataSize >> 2); // in DW
  }

  MpioAncDataDump (GnbHandle, AncillaryData, GlobalDataSize);

  return GlobalDataSize;
}

/**--------------------------------------------------------------------
 *
 * MpioParsePlatformTopology
 *
 * @brief Platform Topology Parser for MPIO Ask
 *
 * @param [in]  GnbHandle        Pointer to the Silicon Descriptor for this node
 * @param [in]  PlatformTopology Pointer to the platform Host Firmware supplied platform configuration
 * @param [out] MpioData         Pointer to the data structure to describe this "node"
 *
 * @returns OpenSIL standard status.
 * @retval SilPass indicates success.
 **/
SIL_STATUS
MpioParsePlatformTopology (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  )
{
  SIL_STATUS                Status;

  uint8_t                   PortStartLane;
  uint8_t                   PortEndLane;
  bool                      Reverse;
  /*
   * DXIO Firmware Structures
   */
  FW_ASK_STRUCT              *AskBufferStart;
  FW_ASK_STRUCT              *AskEntry;
  FW3_LINK_ATTR              *Attributes;
  uint32_t                   AskInstanceCount;
  uint32_t                   AskInstanceSize;
  /*
   * Ancillary Data Structures
   */
  uint32_t                   GlobalSize = 0;
  uint8_t                    *AncillaryData = NULL;
  uint32_t                   AncillaryDataSize;
  uint8_t                    PortParamIndex;
  uint8_t                    FoundAllParams;
  uint8_t                    Counter = 0;

  /*
   * Platform Topology Pointers
   */
  MPIO_PORT_DESCRIPTOR  *SocketTopology; // Local pointer to start of User Config for this socket
  MPIO_PORT_DESCRIPTOR  *TopologyEntry;  // Local updatable pointer for SocketTopology processing

  MPIOCLASS_INPUT_BLK   *SilData;

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Enter with MpioData = 0x%x\n", (uint32_t) ((uintptr_t) MpioData));

  Status = SilPass;
  SocketTopology = NULL;

  Status = MpioGetTopologyForSocket (GnbHandle->SocketId, PlatformTopology, &SocketTopology);

  assert (Status == SilPass);
  if (SilPass != Status) return Status;


  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  /*
	 * Parse structure and count entries for this node
	 */
  AskEntry = NULL;
  AskInstanceSize = 0;
  AskInstanceCount = 0;
  AncillaryDataSize = 0;

  if (SilData->MPIOAncDataSupport) {
    GlobalSize = GetGlobalAncillaryDataSize (GnbHandle, SocketTopology);
    AncillaryDataSize = GlobalSize;
  } else {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "MPIOAncDataSupport = false\n");
  }

  /*
	 * Calculate size of Platform Structure and Ancillary Data for this node
   */
  TopologyEntry = SocketTopology;
  while (TopologyEntry != NULL) {
    if (TopologyEntry->EngineData.EngineType != MpioUnusedEngine) {
      if (MpioTranslateLanes(GnbHandle,
                              (uint8_t)TopologyEntry->EngineData.StartLane,
                              (uint8_t)TopologyEntry->EngineData.EndLane,
                              &PortStartLane,
                              &PortEndLane,
                              &Reverse
                              )) {
        AskInstanceSize += sizeof (FW_ASK_STRUCT);
        AskInstanceCount++;

        MPIO_TRACEPOINT (SIL_TRACE_INFO,
                          "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                          AskInstanceSize,
                          AskInstanceCount
                          );
        AncillaryDataSize += MpioAncDataCalcSize (TopologyEntry);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "Total Ancillary Data Size 0x%x\n", AncillaryDataSize);
        assert (AncillaryDataSize <= MPIO_MAX_ANCILLARY_DATA_SIZE);
      }
  else {
        /*
      * Could not translate lanes.  This entry will be ignored.
      * Note that it would assert earlier.
      */
        TopologyEntry->EngineData.EngineType = MpioUnusedEngine;
      }
    }
    TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
  }
  MPIO_TRACEPOINT (SIL_TRACE_INFO,
                    "Final AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                    AskInstanceSize,
                    AskInstanceCount
                    );

  /*
  * Initialize values
  */
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "MpioData at 0x%x\n", (uint32_t) ((uintptr_t) MpioData));
  MpioData->MpioAsk = NULL;
  MpioData->MpioAskCount = 0;
  MpioData->ExtAttributes = NULL;
  MpioData->ExtAttributeSize = 0;
  MPIO_TRACEPOINT (SIL_TRACE_INFO,
                    "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                    (uint32_t) ((uintptr_t) MpioData),
                    (uint32_t) ((uintptr_t) MpioData->MpioAsk),
                    MpioData->MpioAskCount
                    );

  /*
  * Build data structures for this node if there is any configuration required
  */
  if (AskInstanceCount != 0) {
    if (AncillaryDataSize != 0) {
      AncillaryData = SilData->AncillaryData;
      /*
       * Store the information in MPIO_DATA
       */
      MpioData->ExtAttributes = AncillaryData;
      MpioData->ExtAttributeSize = AncillaryDataSize;

      memset (AncillaryData, 0, AncillaryDataSize);

      if (GlobalSize!= InitGlobalAncillaryData (GnbHandle, SocketTopology, AncillaryData)) {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "Ancillary Data Size Mismatch!\n");
        assert (false);
      }
    }

    MPIO_TRACEPOINT (SIL_TRACE_INFO,
                      "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                      AskInstanceSize,
                      AskInstanceCount
                      );

    AskBufferStart = (FW_ASK_STRUCT *)&SilData->AskBuffer[GnbHandle->SocketId];

    MPIO_TRACEPOINT (SIL_TRACE_INFO,
                      "AskBufferStart allocated at 0x%x with size 0x%x\n",
                      (uint32_t) ((uintptr_t) AskBufferStart),
                      AskInstanceSize
                      );
      MpioData->MpioAsk = AskBufferStart;             // Save pointer to allocated memory
      MpioData->MpioAskCount = AskInstanceCount;

    /*
     * Initialize platform level entries for this structure
     */
    memset (AskBufferStart, 0, AskInstanceSize);

    MPIO_TRACEPOINT (SIL_TRACE_INFO,
                      "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                      AskInstanceSize,
                      AskInstanceCount
                      );

      /*
    * Parse structure and copy entries for this node to this node platform structure
    */
    AskEntry = (FW_ASK_STRUCT *)AskBufferStart;       // Initialize engine pointer to first engine
    AncillaryDataSize = 0;
    TopologyEntry = SocketTopology;
    while (TopologyEntry != NULL) {
      if (TopologyEntry->EngineData.EngineType != MpioUnusedEngine) {
          if (MpioTranslateLanes(GnbHandle,
                                  (uint8_t)TopologyEntry->EngineData.StartLane,
                                  (uint8_t)TopologyEntry->EngineData.EndLane,
                                  &PortStartLane,
                                  &PortEndLane,
                                  &Reverse
                                  )) {
            TopologyEntry->EngineData.MpioStartLane = PortStartLane;
            TopologyEntry->EngineData.MpioEndLane = PortEndLane;
            AskEntry->desc.startLaneIndex = PortStartLane;
            AskEntry->desc.numberOfLanes = (PortEndLane - PortStartLane) + 1;
            Attributes = &AskEntry->desc.link_attributes;
            if (Reverse) {
              AskEntry->desc.reversed = 1;
            }
            switch (TopologyEntry->EngineData.EngineType) {
              case MpioUSB_OVER_PCIE:
                Usb4LaneNumberCfgCallback (Counter, AskEntry, TopologyEntry);
                MPIO_TRACEPOINT (SIL_TRACE_INFO, "Usb4 StartLane = %d\n", TopologyEntry->EngineData.MpioStartLane);
                MPIO_TRACEPOINT (SIL_TRACE_INFO, "Usb4 EndLane = %d\n", TopologyEntry->EngineData.MpioEndLane);

                AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
                AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
                InitAskEntryAttributes (Attributes, TopologyEntry);

                /*
                * Add HFC and DFC indices to link attributes of AskEntry
                */
                PortParamIndex = 0;
                FoundAllParams = 0;

                while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT && FoundAllParams < 2) {
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_HFC_INDEX) {
                    AskEntry->desc.link_attributes.hfc_index =
                      (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_DFC_INDEX) {
                    AskEntry->desc.link_attributes.dfc_index =
                      (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  PortParamIndex++;
                }

                Counter++;
                break;
              case MpioPcieEngine:
                AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
                AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
                InitAskEntryAttributes (Attributes, TopologyEntry);

                /*
                * Add HFC and DFC indices to link attributes of AskEntry
                */
                PortParamIndex = 0;
                FoundAllParams = 0;

                while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT && FoundAllParams < 2) {
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_HFC_INDEX) {
                    AskEntry->desc.link_attributes.hfc_index =
                      (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_DFC_INDEX) {
                    AskEntry->desc.link_attributes.dfc_index =
                      (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  PortParamIndex++;
                }
                if (SilData->MPIOAncDataSupport) {
                  AskEntry->desc.ancillaryDataIdx = TopologyEntry->Port.MpioAncDataIdx; // Updating DataIdx
                                                                                        // from InitGlobalAncillaryData
                  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Ask ancDataIdx=0x%x StartLane=%d\n",
                    AskEntry->desc.ancillaryDataIdx, TopologyEntry->EngineData.MpioStartLane);
                }

//                if (TopologyEntry->Port.CsLink == 1) {
//                  AskEntry->forceKpnpReset = 1;
//                  MPIO_TRACEPOINT (SIL_TRACE_INFO,
//                    "Dev%d Func%d: forceKpnpReset=%d CsLink=%d \n",
//                    TopologyEntry->Port.DeviceNumber,
//                    TopologyEntry->Port.FunctionNumber,
//                    AskEntry->forceKpnpReset,
//                    TopologyEntry->Port.CsLink
//                    );
//                }

                /*
                * Populate Ancillary Data for this port
                */
//                DataSize = MpioAncDataPopulate (AskEntry, TopologyEntry, AncillaryData);
//                if (0 != DataSize) {
//                  AskEntry->desc.ancillaryDataIdx = AncillaryDataOffset >> 2;
//                  AncillaryData += DataSize;
//                  AncillaryDataOffset += DataSize;
//                }

                break;
              case MpioSATAEngine:
                AskEntry->desc.ctrlType = ASK_TYPE_SATA;
                AskEntry->desc.channelType = TopologyEntry->Port.MiscControls.ChannelType;
                Attributes->invertPolarityRx = TopologyEntry->Port.InvertPolarity;
                Attributes->invertPolarityTx = TopologyEntry->Port.InvertPolarity2;
                InitAskEntryAttributes (Attributes, TopologyEntry);

                // Add HFC and DFC indices to link attributes of AskEntry
                PortParamIndex = 0;
                FoundAllParams = 0;
                while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT && FoundAllParams < 2) {
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_HFC_INDEX) {
                    AskEntry->desc.link_attributes.hfc_index =
                    (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_UBM_DFC_INDEX) {
                    AskEntry->desc.link_attributes.dfc_index =
                    (uint8_t) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  PortParamIndex++;
                }
                if (SilData->MPIOAncDataSupport) {
                  //Updating DataIdx from InitGlobalAncillaryData
                  AskEntry->desc.ancillaryDataIdx = TopologyEntry->Port.MpioAncDataIdx;
                  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Ask ancDataIdx=0x%x StartLane=%d\n",
                    AskEntry->desc.ancillaryDataIdx, TopologyEntry->EngineData.MpioStartLane);
                }

                // Populate Ancillary Data for this port
                // DataSize = MpioAncDataPopulate (AskEntry, TopologyEntry, AncillaryData);
                // if (0 != DataSize) {
                  // AskEntry->desc.ancillaryDataIdx = AncillaryDataOffset >> 2;
                  // AncillaryData += DataSize;
                  // AncillaryDataOffset += DataSize;
                // }
                break;

              default:
                break;
            }
            AskEntry++;
          }

        }
        TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
      }

  }
  MPIO_TRACEPOINT (SIL_TRACE_INFO,
                   "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                   (uint32_t) ((uintptr_t) MpioData),
                   (uint32_t) ((uintptr_t) MpioData->MpioAsk),
                   MpioData->MpioAskCount
                   );
  /*
   * Return pointer to this node platform structure
   */
  return Status;
}
