/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioParser.c
 *  @brief Mpio Platform Topology Parser
 */

#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include <string.h>
#include "MpioInitLib.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"
#include "MpioCmn2Rev.h"

/**--------------------------------------------------------------------
 *
 * InitAskEntryAttributes
 *
 * @brief Init Attributes in an AskEntry from the Platform Topology
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   AskEntry            Description for AskEntry
 * @param   TopologyEntry       Description for TopologyEntry
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
InitAskEntryAttributes (
  SIL_CONTEXT            *SilContext,
  FW3_LINK_ATTR          *Attributes,
  MPIO_PORT_DESCRIPTOR   *TopologyEntry
  )
{
  MPIOCLASS_COMMON_INPUT_BLK       *SilDataCommon;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

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
    Attributes->EnabledLTR = 1;
    SilDataCommon->CfgEarlyLink = true;
  }
  if (TopologyEntry->Port.CsLink == 2) {
    // enable LTR for USB Camera link
    Attributes->EnabledLTR = 1;
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
  Attributes->SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
  Attributes->SwingMode = TopologyEntry->Port.SwingMode;
}

/**--------------------------------------------------------------------
 *
 * MpioTranslateLanesCallback
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Pointer to wrapper config descriptor
 * @param   Buffer              Not used
 * @param   GnbHandle           Pointer to GNB_HANDLE
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioTranslateLanesCallback (
  SIL_CONTEXT           *SilContext,
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
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Core is NOT reversed, Link IS reversed\n");
        PortInfo->MpioEndLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioStartLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = true;
      }
    } else {
      if (PortInfo->StartLane <= PortInfo->EndLane) {
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Core IS reversed, Link is NOT reversed\n");
        PortInfo->MpioEndLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->MpioStartLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = true;
      } else {
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Core IS reversed, Link IS reversed\n");
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to silicon descriptor
 * @param   StartLane           Start lane for this port from Platform Topology
 * @param   EndLane End         lane for this port from Platform Topology
 * @param   StartDxioLane       Pointer to storage for translated start lane
 * @param   EndDxioLane         Pointer to storage for translated end lane
 * @param   Reverse             Pointer to boolean identifier for reversed lanes
 *
 * @returns True or false result regarding lane translation.
 * @retval True indicates lane translation succeeded, false indicates failure.
 **/
static
bool
MpioTranslateLanes (
  SIL_CONTEXT   *SilContext,
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
  uint16_t                   GnbInstanceId;
  uint16_t                   LocalInstanceId;
  NBIO_IP2IP_API             *NbioIp2Ip;

  PortInfo.StartLane = StartLane;
  PortInfo.EndLane = EndLane;
  PortInfo.LanesFound = false;
  PortInfo.MpioStartLane = 0;
  PortInfo.MpioEndLane = 0;
  PortInfo.LinkReversal = false;
  Handle = GnbHandle;
  GnbInstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return false;
  }

  while (Handle != NULL) {
    LocalInstanceId = (Handle->SocketId << 6) + Handle->MP_Instance;

    if (LocalInstanceId == GnbInstanceId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
        DESCRIPTOR_ALL_WRAPPERS,
        MpioTranslateLanesCallback,
        (void *)
        &PortInfo,
        Handle
        );
    }
    if (PortInfo.LanesFound) {
      break;
    }
    Handle = GnbGetNextHandle(Handle);
  }

  if (PortInfo.LanesFound) {
    *StartDxioLane = PortInfo.MpioStartLane;
    *EndDxioLane = PortInfo.MpioEndLane;
    *Reverse = PortInfo.LinkReversal;
  } else {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Invalid request for StartLane %d, EndLane %d\n", StartLane, EndLane);
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                Pointer to PCIe platform config structure
 * @param   SocketTopology      Pointer to Platform Topology
 * @param   StrapSize           Pointer to uint32_t to store size of PCIe strap values
 *
 * @returns Ancillary data size
 * @retval 32-bit size value
 **/
static
uint32_t
GetGlobalAncillaryDataSize (
  SIL_CONTEXT             *SilContext,
  GNB_HANDLE              *GnbHandle,
  MPIO_PORT_DESCRIPTOR    *SocketTopology
  )
{
  uint32_t                       StrapListSize;
  uint32_t                      GlobalDataSize;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  GlobalDataSize = 0;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  /*
   * Add Ancillary Data allocation for STRAP settings
   */
  StrapListSize = MpioXferTable->MpioCfgGetStrapListSize(SilContext, GnbHandle, SocketTopology);
  if (StrapListSize != 0) {
    GlobalDataSize += StrapListSize;
  }

  /*
   * Ancillary Data calculation is complete
   */
  if (GlobalDataSize != 0) {
    /*
     * Adding header size for whole ancillary blob
     */
    GlobalDataSize += sizeof (GENERIC_ANC_HEADER); // For Ancillary Data Header
  }
  return GlobalDataSize;
}

/**--------------------------------------------------------------------
 *
 * InitGlobalAncillaryData
 *
 * @brief Initialize the global ancillary data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to silicon descriptor
 * @param   SocketTopology      Pointer to Platform Topology
 *
 * @returns Global data size
 * @retval 32-bit size value in bytes?
 **/
static
uint32_t
InitGlobalAncillaryData (
  SIL_CONTEXT             *SilContext,
  GNB_HANDLE              *GnbHandle,
  MPIO_PORT_DESCRIPTOR    *SocketTopology,
  void                    *AncillaryData
  )
{
  uint32_t                      StrapListSize;
  uint32_t                      GlobalDataSize = 0;
  GENERIC_ANC_HEADER            *AncDataHeader;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  StrapListSize = MpioAddAncDataOverride(GnbHandle, (uint8_t *)AncillaryData, SocketTopology);
  /*
   * Add Ancillary Data for STRAP settings
   */
  StrapListSize += MpioXferTable->MpioCfgAddStrapList(SilContext,
    GnbHandle,
    ((uint8_t *)AncillaryData) + sizeof (GENERIC_ANC_HEADER) + StrapListSize,
    SocketTopology
    );

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  StrapListSize %d\n", StrapListSize);

  if (StrapListSize != 0) {
    GlobalDataSize = StrapListSize + sizeof (GENERIC_ANC_HEADER);
    AncDataHeader = (GENERIC_ANC_HEADER *) AncillaryData;
    AncDataHeader->type = ANCILLARY;                         // Anc Data blob header
    AncDataHeader->id = GnbHandle->SocketId;                 // unique id
    AncDataHeader->length = (uint8_t) (GlobalDataSize >> 2); // in DW
  }

  MpioAncDataDump(GnbHandle, AncillaryData, GlobalDataSize);

  return GlobalDataSize;
}

/**--------------------------------------------------------------------
 *
 * MpioParsePlatformTopology
 *
 * @brief Platform Topology Parser for MPIO Ask
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   PlatformTopology    Pointer to the platform BIOS supplied platform configuration
 * @param   MpioData            Pointer to the data structure to describe this "node"
 *
 * @returns OpenSIL standard status.
 * @retval SilPass indicates success.
 **/
SIL_STATUS
MpioParsePlatformTopology (
  SIL_CONTEXT               *SilContext,
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
  uint32_t                   TotalSize = 0;
  uint8_t                    *AncillaryData = NULL;
  uint32_t                   AncillaryDataSize;
  uint8_t                    PortParamIndex;
  uint8_t                    FoundAllParams;
  uint8_t                    Counter = 0;
  uint32_t                   OverrideSize = 0;
  bool                       Socket0AncData = false;
  bool                       Socket1AncData = false;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  /*
   * Platform Topology Pointers
   */
  MPIO_PORT_DESCRIPTOR  *SocketTopology; // Local pointer to start of User Config for this socket
  MPIO_PORT_DESCRIPTOR  *TopologyEntry;  // Local updatable pointer for SocketTopology processing

  MPIOCLASS_COMMON_INPUT_BLK   *SilDataCommon;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Enter with MpioData = 0x%x\n", (uint32_t) ((uintptr_t) MpioData));

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  Status = SilPass;
  SocketTopology = NULL;
  MpioData->MpioAskCount = 0;
  MpioData->MpioAsk = NULL;
  MpioData->ExtAttributes = NULL;
  MpioData->ExtAttributeSize = 0;

  Status = MpioGetTopologyForSocket(GnbHandle->SocketId, PlatformTopology, &SocketTopology);

  assert(Status == SilPass);
  if (SilPass != Status) return Status;


  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  /*
   * Parse structure and count entries for this node
   */
  AskEntry = NULL;
  AskInstanceSize = 0;
  AskInstanceCount = 0;
  AncillaryDataSize = 0;

  /*
   * Initialize AncillaryDataSize to the size of the global ancillary data
   */

  if (SilDataCommon->MPIOAncDataSupport) {
    GlobalSize = GetGlobalAncillaryDataSize(SilContext, GnbHandle, SocketTopology);
    AncillaryDataSize += GlobalSize;
  } else {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIOAncDataSupport = false\n");
  }

  /*
   * Calculate size of Platform Structure and Ancillary Data for this node
   */
  TopologyEntry = SocketTopology;
  while (TopologyEntry != NULL) {
    if (TopologyEntry->EngineData.EngineType != MpioUnusedEngine) {
      if (MpioTranslateLanes(SilContext,
        GnbHandle,
        (uint8_t)TopologyEntry->EngineData.StartLane,
        (uint8_t)TopologyEntry->EngineData.EndLane,
        &PortStartLane,
        &PortEndLane,
        &Reverse
        )) {
        AskInstanceSize += sizeof (FW_ASK_STRUCT);
        AskInstanceCount++;

        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
          AskInstanceSize,
          AskInstanceCount
          );

        OverrideSize = MpioAncDataCalcSize(TopologyEntry);
        if (OverrideSize != 0) {
          if ((Socket0AncData == false) && (GnbHandle->SocketId == 0)) {
            Socket0AncData = true;
          }
          if ((Socket1AncData == false) && (GnbHandle->SocketId == 1)) {
            Socket1AncData = true;
          }
          AncillaryDataSize += OverrideSize;
        }
      }
    }
    TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
  }
  if (Socket0AncData == true) {
    AncillaryDataSize += sizeof (GENERIC_ANC_HEADER);
  }
  if (Socket1AncData == true) {
    AncillaryDataSize += sizeof (GENERIC_ANC_HEADER);
  }
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Total Ancillary Data Size 0x%x\n", AncillaryDataSize);

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Final AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
    AskInstanceSize,
    AskInstanceCount
    );

  /*
   * Initialize values
   */
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioData at 0x%x\n", (uint32_t) ((uintptr_t) MpioData));
  MpioData->MpioAsk = NULL;
  MpioData->MpioAskCount = 0;
  MpioData->ExtAttributes = NULL;
  MpioData->ExtAttributeSize = 0;
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
    (uint32_t) ((uintptr_t) MpioData),
    (uint32_t) ((uintptr_t) MpioData->MpioAsk),
    MpioData->MpioAskCount
    );

  /*
   * Build data structures for this node if there is any configuration required
   */
  if (AskInstanceCount != 0) {
    /*
     * Allocate memory for this node ancillary data
     */
    if (AncillaryDataSize != 0) {
      AncillaryData = SilDataCommon->AncillaryData;
      /*
       * Store the information in MPIO_DATA
       */
      MpioData->ExtAttributes = AncillaryData;
      MpioData->ExtAttributeSize = AncillaryDataSize;

      memset(AncillaryData, 0, AncillaryDataSize);
      TotalSize = InitGlobalAncillaryData(SilContext, GnbHandle, SocketTopology, AncillaryData);

      if (TotalSize != GlobalSize) {
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "Ancillary Data Size Mismatch! Update Ancillary Data Size(0x%x : 0x%x)\n",
          AncillaryDataSize,
          TotalSize
          );
        MpioData->ExtAttributeSize = TotalSize;
        assert(TotalSize <= AncillaryDataSize);
      }
    }

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
      AskInstanceSize,
      AskInstanceCount
      );

    /*
     * Allocate memory for this node platform structure
     */
    AskBufferStart = (FW_ASK_STRUCT *)&SilDataCommon->AskBuffer[GnbHandle->SocketId];

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "AskBufferStart allocated at 0x%x with size 0x%x\n",
      (uint32_t) ((uintptr_t) AskBufferStart),
      AskInstanceSize
      );
    if (AskBufferStart != NULL) {
      MpioData->MpioAsk = AskBufferStart;             // Save pointer to allocated memory
      MpioData->MpioAskCount = AskInstanceCount;

      /*
       * Initialize platform level entries for this structure
       */
      memset(AskBufferStart, 0, AskInstanceSize);

      MPIO_TRACEPOINT(SIL_TRACE_INFO,
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
          if (MpioTranslateLanes(SilContext,
            GnbHandle,
            (uint8_t)TopologyEntry->EngineData.StartLane,
            (uint8_t)TopologyEntry->EngineData.EndLane,
            &PortStartLane,
            &PortEndLane,
            &Reverse
            )) {
            TopologyEntry->EngineData.MpioStartLane = PortStartLane;
            TopologyEntry->EngineData.MpioEndLane = PortEndLane;
            AskEntry->desc.startLaneIndex = PortStartLane;
            if (TopologyEntry->Port.LinkHotplug == PcieHotplugOCP && TopologyEntry->Port.PortPresent == false) {
              AskEntry->desc.numberOfLanes = 0;
              MPIO_TRACEPOINT(SIL_TRACE_INFO,
                "Found empty OCP Port, setting width to = %d\n",
                AskEntry->desc.numberOfLanes
                );
            } else {
              AskEntry->desc.numberOfLanes = (PortEndLane - PortStartLane) + 1;
            }
            Attributes = &AskEntry->desc.link_attributes;
            if (Reverse) {
              AskEntry->desc.reversed = 1;
            }
            switch (TopologyEntry->EngineData.EngineType) {
            case MpioUSB_OVER_PCIE:
              MpioXferTable->Usb4LaneNumberCfg(Counter, AskEntry, TopologyEntry);
              MPIO_TRACEPOINT(SIL_TRACE_INFO, "Usb4 StartLane = %d\n", TopologyEntry->EngineData.MpioStartLane);
              MPIO_TRACEPOINT(SIL_TRACE_INFO, "Usb4 EndLane = %d\n", TopologyEntry->EngineData.MpioEndLane);

              AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
              AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
              InitAskEntryAttributes(SilContext, Attributes, TopologyEntry);

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

              if (SilDataCommon->MPIOAncDataSupport) {
                /*
                 * Updating DataIdx from InitGlobalAncillaryData
                 */
                AskEntry->desc.ancillaryDataIdx = TopologyEntry->Port.MpioAncDataIdx;
                MPIO_TRACEPOINT(SIL_TRACE_INFO,
                  "Ask ancDataIdx=0x%x StartLane=%d\n",
                  AskEntry->desc.ancillaryDataIdx,
                  TopologyEntry->EngineData.MpioStartLane
                  );
              }

              Counter++;
              break;
            case MpioPcieEngine:
              AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
              AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
              InitAskEntryAttributes(SilContext, Attributes, TopologyEntry);

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
              if (SilDataCommon->MPIOAncDataSupport) {
                AskEntry->desc.ancillaryDataIdx = TopologyEntry->Port.MpioAncDataIdx; // Updating DataIdx
                                                                                      // from InitGlobalAncillaryData
                MPIO_TRACEPOINT(SIL_TRACE_INFO,
                  "Ask ancDataIdx=0x%x StartLane=%d\n",
                  AskEntry->desc.ancillaryDataIdx,
                  TopologyEntry->EngineData.MpioStartLane
                  );
              }
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
  }
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
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
