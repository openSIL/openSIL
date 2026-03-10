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

/**--------------------------------------------------------------------
 *
 * WaitReady
 *
 * @brief Wait for MPIO ready to process a message
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle            Pointer to the silicon descriptor for this NBIO
 * @param  MpioData             Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
WaitReady (
  SIL_CONTEXT   *SilContext,
  GNB_HANDLE    *GnbHandle
  )
{
  uint32_t              MpioArg[6];
  GET_STATUS_RESULTS  *ArgPtr;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Wait for MPIO ready...\n");

  do {
    memset(MpioArg, 0x00, sizeof (MpioArg));
    ArgPtr = (GET_STATUS_RESULTS *) MpioArg;
    MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_GET_STATUS, MpioArg, 0);
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
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle            Pointer to the silicon descriptor for this NBIO
 * @param  MpioData             Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SendAncData (
  SIL_CONTEXT   *SilContext,
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t                   MpioArg[6];
  TRANSFER_EXT_ATTR_ARGS *ArgPtr;

  WaitReady(SilContext, GnbHandle);
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
  MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_TRANSFER_EXTENDED_ATTRIBUTES, MpioArg, 0);

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
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle            Pointer to the silicon descriptor for this NBIO
 * @param  MpioData             Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SendAsk (
  SIL_CONTEXT   *SilContext,
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t              MpioArg[6];
  TRANSFER_ASK_ARGS *ArgPtr;

  WaitReady(SilContext, GnbHandle);
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
  MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_TRANSFER_ASK, MpioArg, 0);
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
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle            Pointer to the silicon descriptor for this NBIO
 * @param  MpioData             Pointer to the ASK structure descriptor for this Instance
 *
 * @returns Value of argument zero from NBIO initialized MpioArg
 * @retval 32-bit configuration value
 **/
static
uint32_t
GetAsk (
  SIL_CONTEXT   *SilContext,
  GNB_HANDLE    *GnbHandle,
  MPIO_DATA     *MpioData
  )
{
  uint32_t                MpioArg[6];
  GET_ASK_RESULT_ARGS *ArgPtr;

  WaitReady(SilContext, GnbHandle);
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
  MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_GET_ASK_RESULT, MpioArg, 0);
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
  GlobalConfig->enable_dlwm = 0;
  GlobalConfig->combine_training_early_and_normal = 0;

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
 * @param  SilContext        A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param  SilDataCommon     Mpio input block pointer
 * @param  Pcie              Pointer to the platform complex
 * @param  StartHandle       GNB_HANDLE structure pointer
 * @param  PlatformTopology  Pointer to the platform BIOS supplied platform configuration
 *
 * @returns SIL_STATUS general values for OpenSIL
 * @retval SilPass
 * @retval SilNotFound  Fch Ip-2-Ip API was not found
 **/
SIL_STATUS
MpioEarlyInitV1 (
  SIL_CONTEXT                      *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK       *SilDataCommon,
  PCIe_PLATFORM_CONFIG             *Pcie,
  GNB_HANDLE                       *StartHandle,
  MPIO_COMPLEX_DESCRIPTOR          *PlatformTopology
  )
{
  GNB_HANDLE                    *GnbHandle;
  uint32_t                      MpioArg[6];
  void                          *ArgPtr;
  uint8_t                       InstanceIndex = 0;
  uint16_t                      InstanceId;
  MPIO_DATA                     MpioData[MAX_INSTANCE_ID];
  uint32_t                      ResetDelay;
  SIL_STATUS                    Status;
  FCH_IP2IP_API                 *FchApi;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilContext, SilId_FchClass, (void **)&FchApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "FCH API not found!\n");
    return Status;
  }

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
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

    MpioParsePlatformTopology(SilContext,
      GnbHandle,
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
      MpioXferTable->MpioCfgGlobalConfig(SilContext, SilDataCommon, GnbHandle, MpioArg);

      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_SET_GLOBAL_CONFIG, MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );

      if (SilDataCommon->MPIOAncDataSupport) {
        SendAncData(SilContext, GnbHandle, &MpioData[InstanceIndex]);
      }

      SendAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);

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

      MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      GetAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);
      MpioPortMapping(SilContext, SilDataCommon, GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
      MpioCfgBeforeReconfig(SilContext, GnbHandle);

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
      MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);
      MpioCfgAfterReconfig(SilContext, GnbHandle);

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
      MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);
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
  ResetDelay = SilDataCommon->AfterResetDelay;

  if (ResetDelay != 0) {
    FchApi->FchStall(ResetDelay * 1000);
  }
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Reset Deassert Request for GpioId 0x%x\n", MpioData->MpioAsk->desc.gpioHandle);

  if (SilDataCommon->CfgEarlyLink) {

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
    MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
    GetAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);
    MpioProcessEarlyTrain(SilContext, GnbHandle, PlatformTopology, &MpioData[InstanceIndex]);
  }

  GnbHandle = StartHandle;
  InstanceId = 0xFFFF;
  while (GnbHandle != NULL) {

    InstanceId = (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance;

    InstanceIndex = GetInstanceIndex(GnbHandle);

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
      MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(MPIO_SETUP_LINK), MpioArg, 0);
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

      GetAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);
      MpioUpdatePortTrainingStatus(SilContext, SilDataCommon, GnbHandle, &MpioData[InstanceIndex]);
      SendAsk(SilContext, GnbHandle, &MpioData[InstanceIndex]);

      if (SilDataCommon->CfgHotplugUMBSupport) {
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
        MpioServiceRequestCommon(SilContext, GnbHandle->Address, POSTED_MSG(BIOS_MPIO_MSG_UPDATE_LINK), MpioArg, 0);
      }
    }

    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == (GnbHandle->SocketId << 6) + GnbHandle->MP_Instance));
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
