/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioInitFlow.c
 *  @brief tbd
 */

#include <string.h>
#include "MpioLib.h"
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include <FCH/FchIp2Ip.h>

#define SIL_RESERVED_0481                    0x2


/**--------------------------------------------------------------------
 *
 * WaitReady
 *
 * @brief Wait for MPIO ready to process a message
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
WaitReady (
  GNB_HANDLE    *GnbHandle
  )
{
  uint32_t              MpioArg[6];
  GET_STATUS_RESULTS  *ArgPtr;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Wait for MPIO ready...\n");

  do {
    memset(MpioArg, 0x00, sizeof (MpioArg));
    ArgPtr = (GET_STATUS_RESULTS *) MpioArg;
    MpioServiceRequestCommon(GnbHandle->Address, MPIO_GET_STATUS, MpioArg, 0);
  } while (ArgPtr->CmdStatus != 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
}

/**--------------------------------------------------------------------
 *
 * SendAncData
 *
 * @brief Transfer Ancillary Data to MPIO
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SendAncData (
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t                   MpioArg[6];
  TRANSFER_EXT_ATTR_ARGS *ArgPtr;

  WaitReady(GnbHandle);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "MpioData at 0x%x\n -- ExtAttributes = 0x%x\n -- ExtAttributeSize = 0x%x\n",
    (uint32_t) ((uintptr_t) MpioData),
    (uint32_t) ((uintptr_t) MpioData->ExtAttributes),
    MpioData->ExtAttributeSize
    );

  memset(MpioArg, 0x00, sizeof (MpioArg));
  ArgPtr = (TRANSFER_EXT_ATTR_ARGS *) MpioArg;
  ArgPtr->DramAddressLo = (uint32_t) ((uintptr_t) MpioData->ExtAttributes);
  ArgPtr->DramAddressHi = 0;
  ArgPtr->NumberOfDWords = MpioData->ExtAttributeSize >> 2;
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  MpioServiceRequestCommon(GnbHandle->Address, MPIO_TRANSFER_EXTENDED_ATTRIBUTES, MpioArg, 0);

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
}

/**--------------------------------------------------------------------
 *
 * SendAsk
 *
 * @brief Transfer FW_ASK_STRUCT from MPIO
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SendAsk (
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t              MpioArg[6];
  TRANSFER_ASK_ARGS *ArgPtr;

  WaitReady(GnbHandle);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
    (uint32_t) ((uintptr_t) MpioData),
    (uint32_t) ((uintptr_t) MpioData->MpioAsk),
    MpioData->MpioAskCount
    );

  memset(MpioArg, 0x00, sizeof (MpioArg));
  ArgPtr = (TRANSFER_ASK_ARGS *) MpioArg;
  ArgPtr->DramAddressLo = (uint32_t) ((uintptr_t) MpioData->MpioAsk);
  ArgPtr->DramAddressHi = 0;
  ArgPtr->Links = SELECTED_LINKS;
  ArgPtr->Direction = DRAM_TO_MPIO;
  ArgPtr->LinkCount = MpioData->MpioAskCount;
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  MpioServiceRequestCommon(GnbHandle->Address, MPIO_TRANSFER_ASK, MpioArg, 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIO firmware rejected the Ask structure!");
    assert(false);
  }
}

/**--------------------------------------------------------------------
 *
 * GetAsk
 *
 * @brief Transfer FW_ASK_STRUCT to MPIO
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Value of argument zero from NBIO initialized MpioArg
 * @retval 32-bit configuration value
 **/
static
uint32_t
GetAsk (
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t                MpioArg[6];
  GET_ASK_RESULT_ARGS *ArgPtr;

  WaitReady(GnbHandle);
  memset(MpioArg, 0x00, sizeof (MpioArg));
  ArgPtr = (void *) MpioArg;
  ArgPtr->DestAddressLo = (uint32_t) ((uintptr_t) MpioData->MpioAsk);
  ArgPtr->DestAddressHi = 0;
  ArgPtr->LinksOnly = ALL_LINKS;
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  MpioServiceRequestCommon(GnbHandle->Address, MPIO_GET_ASK_RESULT, MpioArg, 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );

  return (MpioArg[0]);
}

/**--------------------------------------------------------------------
 *
 * MpioSetGlobalConfigDefaults
 *
 * @brief Set Global Config Defaults
 *
 * @param[in] ArgList Input argument list for global configuration
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioSetGlobalConfigDefaults (
  uint32_t    *ArgList
  )
{
  MPIO_GLOBAL_CONFIG    *GlobalConfig;

  GlobalConfig = (MPIO_GLOBAL_CONFIG *) ArgList;

  /*
   * DWORD 0 - General
   */
  GlobalConfig->skipVetting = 0;
  GlobalConfig->ntbhotpluginterval = 0;
  GlobalConfig->saverestoremode = 0;
  GlobalConfig->matchPortSizeExactly = 0;
  GlobalConfig->skipLinkDisableOnFailure = 0;
  GlobalConfig->usePhySram = 1;
  GlobalConfig->ValidPhyFWFlag = 1;
  GlobalConfig->enableLoopbackSupport = 0;
  GlobalConfig->EnablePcieNonCompliantWa = 0;
  GlobalConfig->DeferredMessageSupport = 0;
  GlobalConfig->CxlGpfPhase2Timeout = 0;

  /*
   * DWORD 1 - Power Management
   */
  GlobalConfig->PWRMNGMT_PRFRM_CLK_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_STATIC_PWR_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_REFCLK_SHUTDOWN = 0;
  GlobalConfig->CBSOPTIONS_ENABLEPOWERMANAGEMENT = 0;
  GlobalConfig->PWRMNGMT_PRFRM_PMA_POWER_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_PMA_CLOCK_GATING = 0;

  /*
   * DWORD 2  - PCIE Link Timeouts (in msec)
   */
  GlobalConfig->LinkReceiverDetectionPolling = 0;
  GlobalConfig->LinkResetToTrainingTime = 0;
  GlobalConfig->LinkL0Polling = 0;

  /*
   * DWORD 3 - Protocol
   */
  GlobalConfig->PCIE_ALLOW_COMPLETION_PASS = 0;
  GlobalConfig->CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL = 0;
  GlobalConfig->TxFifoRdPtrOffset = 0;

  /*
   * DWORD 4 - Trap Control
   */
  GlobalConfig->disableSbrTrap = 0;
  GlobalConfig->disableLaneMarginingTrap = 0;

}

/**--------------------------------------------------------------------
 *
 * MpioPcieUpdateAskAfterTraining
 *
 * @brief Update Mpio Ask structure after training
 *
 * @param[in]  GnbHandle              Pointer to GnbHandle pointer
 * @param[in]  PlatformTopology       Pointer to the platform BIOS supplied platform configuration
 * @param[in]  MpioData               Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioPcieUpdateAskAfterTraining (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  )
{
  SIL_STATUS                      Status;
  uint8_t                         Index;
  FW_ASK_STRUCT                   *AskEntry;
  MPIO_PORT_DESCRIPTOR            *SocketTopology;
  PCIe_ENGINE_CONFIG              *Engine;

  SocketTopology = NULL;
  Engine = NULL;

  Status = MpioGetTopologyForSocket(GnbHandle->SocketId, PlatformTopology, &SocketTopology);
  if (Status != SilPass) {
    return;
  }

  AskEntry = MpioData->MpioAsk;

  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      Engine = MpioFindEngineForAsk(GnbHandle, AskEntry);
      if (Engine != NULL) {
        /*
         * Pass MPIO LogicalBridgeId to map to correct IOHC bridge control register
         */
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "Engine->Type.Port.LogicalBridgeId: %x\n", Engine->Type.Port.LogicalBridgeId);
        AskEntry->desc.link_attributes.LogicalBridgeID = Engine->Type.Port.LogicalBridgeId;
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "AskEntry->desc.link_attributes.LogicalBridgeID: %x\n",
          AskEntry->desc.link_attributes.LogicalBridgeID
          );
        /*
         * Set any discovered UBM ports PortPresent field to 1 to ensure later NBIO code properly configures the port.
         * MPIO needs PortPresent set to 0 on empty ports that could be either SATA or PCIE.
         */
        if ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugUBM) ||
          (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugOCP)) {
          Engine->Type.Port.PortData.PortPresent = 1;
        }
      }
    }
    AskEntry++;
  }
}

/**--------------------------------------------------------------------
 *
 * MpioPcieAuthenticationBeforeTraining
 *
 * @brief Mpio Port Authentication before Training
 *
 * @param[in]  GnbHandle              Pointer to GnbHandle pointer
 * @param[in]  PlatformTopology       Pointer to the platform BIOS supplied platform configuration
 * @param[in]  MpioData               Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioPcieAuthenticationBeforeTraining (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  )
{
  SIL_STATUS                      Status;
  SIL_STATUS                      SilStatus;
  uint8_t                         Index;
  FW_ASK_STRUCT                   *AskEntry;
  MPIO_PORT_DESCRIPTOR            *SocketTopology;
  PCIe_ENGINE_CONFIG              *Engine;
  PCIe_WRAPPER_CONFIG             *Wrapper;
  uint8_t                         Command;
  uint8_t                         Pcie;
  uint8_t                         Port;
  uint8_t                         NbioNum;
  uint32_t                        MpioArg[6];
  MPIOCLASS_INPUT_BLK             *SilData;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  SocketTopology = NULL;
  Engine = NULL;
  Status = SilPass;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  if (Status != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Non-support PCIE Authentication\n");
    return;
  }

  SilStatus = MpioGetTopologyForSocket(GnbHandle->SocketId, PlatformTopology, &SocketTopology);
  if (SilStatus != SilPass) {
    return;
  }

  AskEntry = MpioData->MpioAsk;

  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      Engine = MpioFindEngineForAsk(GnbHandle, AskEntry);
      if (Engine != NULL) {
        bool Authenticated;

        Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
        GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon(Engine);
        NbioNum = GnbHandle->RBIndex;
        Pcie = Wrapper->WrapId;
        Port = Engine->Type.Port.PortId;

#ifdef ENABLE_MPIO_PCIE_AUTH_BEFORE_TRAIN_FUNC_BODY_1
        Authenticated = false;
        PORT_AUTHENTICATION_DESC        PortAuth;
        PortAuth.PciLocation.AsBits.Segment = Engine->Type.Port.Address.Address.Segment;
        PortAuth.PciLocation.AsBits.Bus = Engine->Type.Port.Address.Address.Bus;
        PortAuth.PciLocation.AsBits.Device = Engine->Type.Port.PortData.DeviceNumber;
        PortAuth.PciLocation.AsBits.Function = Engine->Type.Port.PortData.FunctionNumber;
        PortAuth.StartLaneIndex = Engine->EngineData.StartLane;
        PortAuth.EndLaneIndex = Engine->EngineData.EndLane;

        Status = AmdPcieAuthentication->PcieAuthenticationBeforeTraining(&PortAuth, &Authenticated);
#else
        Authenticated = true;
        Status = SilPass;
#endif //ENABLE_MPIO_PCIE_AUTH_BEFORE_TRAIN_FUNC_BODY_1

        if (Status == SilPass) {
          /* coverity[dead_error_condition] */
          if (!Authenticated) {
            AskEntry->desc.link_attributes.portPresent = 0;
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioPortAuthentication = false\n");
          } else {
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioPortAuthentication = true\n");

            /*
             * Get IP block data
             */
            SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
            assert(SilData != NULL);
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIO IP block is located at %x\n", SilData);
            if (SilData->CfgHotplugUMBSupport) {
              Command = SIL_RESERVED_0481;
              memset(MpioArg, 0x00, sizeof (MpioArg));
              MpioArg[0] = (Command << 24) | (NbioNum << 16) | (Pcie << 8) | (Port);
              MpioServiceRequestCommon(GnbHandle->Address, BIOS_MPIO_MSG_UBM_PERST_RELEASE, MpioArg, 0);
            }

          }
        }
      }
    }
    AskEntry++;
  }
}

/**--------------------------------------------------------------------
 *
 * GetInstanceIndex
 *
 * @brief Utility function to get MPIO instance index
 *
 * @param [in] GnbHandle - Pointer to the silicon descriptor for this NBIO.
 *
 * @returns MPIO instance index
 * @retval 8-bit index value.
 **/
static
uint8_t
GetInstanceIndex (
  GNB_HANDLE    *GnbHandle
  )
{
  uint8_t Index = 0;

  if (GnbHandle->MP_Instance != 0) {
    Index = GnbHandle->MP_Instance;
  } else {
    Index = GnbHandle->SocketId;
  }
  return Index;
}

/**
 * MpioEarlyInitV1
 *
 * @brief Mpio Early Initialization
 *
 * @param[in]  SilData           Mpio input block pointer
 * @param[in]  Pcie              Pointer to the platform complex
 * @param[in]  StartHandle       GNB_HANDLE structure pointer
 * @param[in]  PlatformTopology  Pointer to the platform BIOS supplied platform configuration
 *
 * @returns SIL_STATUS general values for OpenSIL
 * @retval SilPass
 * @retval SilNotFound  Fch Ip-2-Ip API was not found
 **/
SIL_STATUS
MpioEarlyInitV1 (
  MPIOCLASS_INPUT_BLK       *SilData,
  PCIe_PLATFORM_CONFIG      *Pcie,
  GNB_HANDLE                *StartHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology
  )
{
  GNB_HANDLE                    *GnbHandle;
  uint32_t                      MpioArg[6];
  void                          *ArgPtr;
  uint8_t                       InstanceIndex;
  uint16_t                      InstanceId;
  MPIO_DATA                     MpioData[MAX_INSTANCE_ID];
  uint32_t                      ResetDelay;
  SIL_STATUS                    Status;
  FCH_IP2IP_API                 *FchApi;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_FchClass, (void **)&FchApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "FCH API not found!\n");
    return Status;
  }

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }
  /*
   * Test/Debug implementation
   */
  GnbHandle = StartHandle;
  InstanceId = 0xFFFF;
  while (GnbHandle != NULL) {

    InstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;

    InstanceIndex = GetInstanceIndex(GnbHandle);

    MpioParsePlatformTopology(GnbHandle,
      PlatformTopology,
      &MpioData[InstanceIndex]
      );

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
      (uint32_t) ((uintptr_t) &(MpioData[InstanceIndex])),
      (uint32_t) ((uintptr_t) (MpioData[InstanceIndex].MpioAsk)),
      MpioData[InstanceIndex].MpioAskCount
      );

    if (MpioData[InstanceIndex].MpioAsk != NULL) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Platform Descriptor for Instance %d\n", InstanceIndex);
      GnbHandle->NumEngineDesc = (uint8_t)MpioData[InstanceIndex].MpioAskCount;
      MpioDebugDump(&(MpioData[InstanceIndex]));

      /*
       * Set Global Config
       */
      memset(MpioArg, 0x00, sizeof (MpioArg));
      MpioSetGlobalConfigDefaults(MpioArg);
      MpioXferTable->MpioCfgGlobalConfig(SilData, GnbHandle, MpioArg);

      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequestCommon(GnbHandle->Address, MPIO_SET_GLOBAL_CONFIG, MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );

      if (SilData->MPIOAncDataSupport) {
        SendAncData(GnbHandle, &MpioData[InstanceIndex]);
      }

      SendAsk(GnbHandle, &MpioData[InstanceIndex]);

      memset(MpioArg, 0x00, sizeof (MpioArg));
      ArgPtr = (void *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Map = 1;
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );

      MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      GetAsk(GnbHandle, &MpioData[InstanceIndex]);
      MpioPortMapping(SilData, GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
      MpioCfgBeforeReconfig(GnbHandle);

      memset(MpioArg, 0x00, sizeof (MpioArg));
      ArgPtr = (void *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Configure = 1;
      ((SETUP_LINK_ARGS *) ArgPtr)->Reconfigure = 1;
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk(GnbHandle, &MpioData[InstanceIndex]);
      MpioCfgAfterReconfig(GnbHandle);

      memset(MpioArg, 0x00, sizeof (MpioArg));
      ArgPtr = (void *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->PerstReq = 1;
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk(GnbHandle, &MpioData[InstanceIndex]);
    }

    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance));

  }

  GnbHandle = StartHandle;

  /*
   * de-assert PCIe reset all Engines
   */
  FchApi->FchGpioSlotResetControl ();
  ResetDelay = SilData->AfterResetDelay;

  if (ResetDelay != 0) {
    FchApi->FchStall(ResetDelay * 1000);
  }
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Reset Deassert Request for GpioId 0x%x\n", MpioData->MpioAsk->desc.gpioHandle);

  if (SilData->CfgEarlyLink) {

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Early link training\n");

    memset(MpioArg, 0x00, sizeof (MpioArg));
    ArgPtr = (void *) MpioArg;
    ((SETUP_LINK_ARGS *) ArgPtr)->Training = 1;
    ((SETUP_LINK_ARGS *) ArgPtr)->Enumerate = 1;
    MpioArg[1] = 1;
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
      MpioArg[0],
      MpioArg[1],
      MpioArg[2],
      MpioArg[3],
      MpioArg[4],
      MpioArg[5]
      );

    GnbHandle = StartHandle;
    InstanceIndex = GetInstanceIndex(GnbHandle);
    MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
    GetAsk(GnbHandle, &MpioData[InstanceIndex]);
    MpioProcessEarlyTrain(GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
  }

  GnbHandle = StartHandle;
  InstanceId = 0xFFFF;
  while (GnbHandle != NULL) {

    InstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;

    InstanceIndex = GetInstanceIndex(GnbHandle);

    if (MpioData[InstanceIndex].MpioAsk != NULL) {

      /*
       * PCIE port authentication before training
       */
      MpioPcieAuthenticationBeforeTraining(GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
      MpioCfgBeforeTraining(GnbHandle);
    }

    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance));
  }

  GnbHandle = StartHandle;
  InstanceId = 0xFFFF;
  while (GnbHandle != NULL) {
    InstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;
    InstanceIndex = GetInstanceIndex(GnbHandle);
    if (MpioData[InstanceIndex].MpioAsk != NULL) {

      memset(MpioArg, 0x00, sizeof (MpioArg));
      ArgPtr = (void *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Training = 1;
      ((SETUP_LINK_ARGS *) ArgPtr)->Enumerate = 1;
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
    }

    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance));

  }

  GnbHandle = StartHandle;
  InstanceId = 0xFFFF;
  while (GnbHandle != NULL) {

    InstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;

    InstanceIndex = GetInstanceIndex(GnbHandle);

    if (MpioData[InstanceIndex].MpioAsk != NULL) {

      GetAsk(GnbHandle, &MpioData[InstanceIndex]);
      MpioUpdatePortTrainingStatus(SilData, GnbHandle, &MpioData[InstanceIndex]);
      MpioPcieUpdateAskAfterTraining(GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
      SendAsk(GnbHandle, &MpioData[InstanceIndex]);

      if (SilData->CfgHotplugUMBSupport) {
        memset(MpioArg, 0x00, sizeof (MpioArg));
        /*
         * Call BIOS_MPIO_MSG_UPDATE_LINK with MpioArg[0] = 0 to request structure update.
         */
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
          MpioArg[0],
          MpioArg[1],
          MpioArg[2],
          MpioArg[3],
          MpioArg[4],
          MpioArg[5]
          );
        MpioServiceRequestCommon(GnbHandle->Address, POSTED_MSG(BIOS_MPIO_MSG_UPDATE_LINK), MpioArg, 0);
      }
    }

    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance));
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
