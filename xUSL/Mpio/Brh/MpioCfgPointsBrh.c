/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioCfgPointsBrh.c
 * @brief Callouts from MPIO initialization for BRH
 *
 */

#include <string.h>
#include <SilCommon.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/Common/Deli.h>
#include <Nbio/Brh/include/GlobalRegB0.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/include/NbioBaseReg.h>
#include <Nbio/Brh/include/PcieRcCfgReg.h>
#include <Nbio/Brh/include/Iommul2Reg.h>
#include <Nbio/Brh/include/NbifEpFnCfgReg.h>
#include <Nbio/Brh/include/NbifEpF0CfgReg.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <Nbio/Brh/NbioPcieComplexDataBrh.h>
#include <Nbio/Brh/include/PcieCoreReg.h>
#include <Nbio/NbioIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include <Mpio/Common/MpioLib.h>
#include "MpioPcieStrapsBrh.h"
#include "MpioCmn2Brh.h"
#include "MpioIp2IpBrh.h"
#include "MpioLibLocal.h"

/// Separate Reference Clock with Independent Spread Type Enumeration
typedef enum {
  SRIS_CMN = BIT_8(0),          ///< Sris common type
  SRIS_DBG = BIT_8(1),          ///< Sris debug type
  SRIS_DBG_PBS = BIT_8(2),      ///< Sris debug level 2
  SRIS_AUTODETECT = BIT_8(3)    ///< Sris autodetect type
} SRIS_CFG_TYPE;

typedef struct {
  bool  field0;
  bool  field1;
} SIL_RESERVED_STRUCT_0038;

/**--------------------------------------------------------------------
 *
 * MpioCfgGlobalConfigBrh
 *
 * @brief Callback from MpioLib to update GLOBAL CONFIG
 *
 * @details This function provides platform-specific initialization of the MPIO_GLOBAL_CONFIG
 *
 * @param [in] GnbHandle GNB_HANDLE pointer
 * @param [in] ArgList   Pointer to array of arguments  cast as MPIO_GLOBAL_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
void
MpioCfgGlobalConfigBrh (
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  )
{
  MPIO_GLOBAL_CONFIG    *GlobalConfig;

  GlobalConfig = (MPIO_GLOBAL_CONFIG *) ArgList;

  /*
   * Clock Gating
   */
  GlobalConfig->PWRMNGMT_PRFRM_CLK_GATING = SilData->CfgDxioClockGating ? 1 : 0;

  /*
   * Training Timers
   */
  if (SilData->PcieDxioTimingControlEnable) {
    GlobalConfig->LinkReceiverDetectionPolling = SilData->PCIELinkReceiverDetectionPolling;
    GlobalConfig->LinkResetToTrainingTime = SilData->PCIELinkResetToTrainingTime;
    GlobalConfig->LinkL0Polling = SilData->PCIELinkL0Polling;
  }

  /*
   * Exact Match
   */
  if (SilData->PCIeExactMatchEnable) {
    GlobalConfig->matchPortSizeExactly = 1;
  }

  /*
   * Valid PHY firmware
   */
  if (SilData->DxioPhyValid == 0) {
    GlobalConfig->ValidPhyFWFlag = 0;
  }

  /*
   * Use PHY SRAM
   */
  if (SilData->DxioPhyProgramming == 0) {
    GlobalConfig->usePhySram = 0;
  }

  /*
   * Skip Vetting
   */
  if (SilData->CfgSkipPspMessage == 1) {
    GlobalConfig->skipVetting = 1;
  }

  if (SilData->DxioSaveRestoreModes == 1) {
    GlobalConfig->saverestoremode = 1;
  }

  GlobalConfig->disableSbrTrap = (SilData->SbrBrokenLaneAvoidanceSup? 0 : 1);
  GlobalConfig->disableLaneMarginingTrap = (SilData->AutoFullMarginSup? 0 : 1);

  // Link Disable at Power Off Delay
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "LinkDisableAtPowerOffDelay: %x\n", SilData->LinkDisableAtPowerOffDelay);
  GlobalConfig->LinkDisableAtPowerOffDelay = SilData->LinkDisableAtPowerOffDelay;

  // PCIe SPC Gen4
  GlobalConfig->Enable2SpcGen4 = SilData->Enable2SpcGen4 ? 1 : 0;

  // PCIe SPC Gen5
  GlobalConfig->Enable2SpcGen5 = SilData->Enable2SpcGen5 ? 1 : 0;

  // Non-PCIe Compliant Support
  GlobalConfig->EnablePcieNonCompliantWa = SilData->PcieNonPcieCompliantTrainingFailureSupport ? 1 : 0;

  // PCIe loopback Mode
  GlobalConfig->enableLoopbackSupport = SilData->CfgPcieLoopbackMode ? 1 : 0;

  // TX FIFO Read Pointer Offset
  GlobalConfig->TxFifoRdPtrOffset = SilData->CfgDxioTxFIFORdPtrOffset;

  // Safe recovery on BERExceedErr
  GlobalConfig->RunXgmiSafeRecoveryOdt = (SilData->SafeRecoveryBER) ? 1 : 0;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Safe recovery on BERExceedErr value: %x\n", GlobalConfig->RunXgmiSafeRecoveryOdt);

  // Periodic Zcal
  GlobalConfig->RunZcal = (SilData->PeriodicCal) ? 1 : 0;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Periodic Zcal: %x\n", GlobalConfig->RunZcal);

}

/*----------------------------------------------------------------------------------------*/
/**
 * Per-Wrapper Callback for Early Trained Ports configuration before bifurcation.
 * Use function for capability settings set dynamically.  Static capabilities for early training
 * should be set in ABL.
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in]     GnbHandle GnbHandle Pointer
 */
static
void
EarlyTrainingMpioCfgBeforeReconfigWrapperBrh (
  PCIe_WRAPPER_CONFIG    *Wrapper,
  GNB_HANDLE             *GnbHandle
  )
{
  uint32_t               Index;
  MPIOCLASS_INPUT_BLK    *SilData;
  NBIOCLASS_DATA_BLOCK   *NbioData;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);
  NbioData = (NBIOCLASS_DATA_BLOCK *)xUslFindStructure(SilId_NbioClass, 0);
  assert(NbioData != NULL);
  // ACS Enablement
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1507),
    (uint32_t) ~(SIL_RESERVED_1446),
    Wrapper->AcsSupport << SIL_RESERVED_1447,
      0
    );
  // Advanced error reporting (AER)
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1507),
    (uint32_t) ~(SIL_RESERVED_1448),
    Wrapper->AdvancedErrorReporting << SIL_RESERVED_1449,
      0
    );
  // ECRC Capability
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1507),
    (uint32_t) ~(SIL_RESERVED_1456 |
    SIL_RESERVED_1454),
    (((((Wrapper->ECRCSupport & 0x1) == 0x1) ? 1 : 0) << SIL_RESERVED_1457) |
    ((((Wrapper->ECRCSupport & 0x2) == 0x2) ? 1 : 0) << SIL_RESERVED_1455)),
    0
    );

  // IDE Capability
  for (Index = 0; Index < 9; Index += 2) {
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A383000) + (Index * 2),
      (uint32_t) ~(SIL_RESERVED_1513 | \
      ((Index < 8 ? SIL_RESERVED_1513 : 0) << 16)),
      (((SilData->CfgSevSnpSupport == true) || (SilData->CfgSevTioSupport == true) || (SilData->PcieIdeCapSup == true \
      )) ?(Index < 8 ? 0x10001 : 1) : 0) << SIL_RESERVED_1514,
        0
      );
  }
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A383020),
    (uint32_t) ~(SIL_RESERVED_1509),
    (((SilData->CfgSevSnpSupport == true) || (SilData->CfgSevTioSupport == true) || (SilData->PcieIdeCapSup == true)) \
    ? 0 : 1) << SIL_RESERVED_1510,
      0
    );
}

/*
 *=========================================================================================
 * Timepoint after port mapping and before reconfig
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 *
 * MpioCfgBeforeReconfigCallbackBrh
 *
 * @brief Per-Engine Callback for port configuration before bifurcation
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
static
void
MpioCfgBeforeReconfigCallbackBrh (
  PCIe_ENGINE_CONFIG                *Engine,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  GNB_HANDLE            *GnbHandle;
  SIL_RESERVED_STRUCT_0038 *PortStateInWrapper;
  uint16_t              Index;
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));
  PortStateInWrapper = (SIL_RESERVED_STRUCT_0038 *) Buffer;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (PcieLibIsEngineAllocated(Engine)) {
    PortStateInWrapper->field1 = true;

    /*
     * Enable ten bit tag CAP support
     */
    if (Engine->Type.Port.PortCapabilities.TenBitTagSupport == 1) {
      Index = SIL_RESERVED_0474 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    } else if (Engine->Type.Port.PortCapabilities.TenBitTagSupport == 2 ) {
      Index = SIL_RESERVED_0475 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    } else if (Engine->Type.Port.PortCapabilities.TenBitTagSupport == 3) {
      Index = SIL_RESERVED_0474 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );

      Index = SIL_RESERVED_0475 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    }

    /*
     * Lane power state during dynamic link width change
     */
    if (Engine->Type.Port.PortFeatures.DynLanesPwrState != 0xf) {
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1480
        ),
        (uint32_t) ~(SIL_RESERVED_1378 |
        SIL_RESERVED_1384),
        ((Engine->Type.Port.PortFeatures.DynLanesPwrState & 0x3) <<
          SIL_RESERVED_1379) |
        ((Engine->Type.Port.PortFeatures.TurnOffUnusedLanes & 0x1) <<
          SIL_RESERVED_1385),
        0
        );
    }

    /*
     * Data Link feature (DLF)
     */
    if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 0) {
      Index = SIL_RESERVED_0450 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0420, 1, Wrapper->WrapId);
      WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);
    } else if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 1) {
      Index = SIL_RESERVED_0450 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0420, 1, Wrapper->WrapId);
      WritePcieStrapBrh(GnbHandle, Index, 0, Wrapper->WrapId);
    } else if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 2) {
      WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0420, 0, Wrapper->WrapId);
    }

    /*
     * Data Link feature Exchange Control(DLF)
     */
    Index = SIL_RESERVED_0421 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle,
      Index,
      Engine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl,
      Wrapper->WrapId
      );

    /*
     * For configurable SSID/SSVID
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0480,
      BIT_32(9),
      Wrapper->WrapId
      );

    /*
     * Allow Compliance
     */
    Index = SIL_RESERVED_0412 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    if (SilData->AmdAllowCompliance == 0x0F) {
      WritePcieStrapBrh(GnbHandle, Index, 0, Wrapper->WrapId);
    } else {
      WritePcieStrapBrh(GnbHandle, Index, SilData->AmdAllowCompliance, Wrapper->WrapId);
    }

    /*
     * Max Payload support
     */
    Index = SIL_RESERVED_0454 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    if (Engine->Type.Port.PortCapabilities.MaxPayloadSupport != 0xf) {
      WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.PortCapabilities.MaxPayloadSupport, Wrapper->WrapId);
    }

    /*
     * Program equalization parameters into their corresponding strap. These are written per wrapper
     * but programmed in the per engine call. Different lane CBS values within a wrapper should be
     * avoided to prevent overwriting the desired value for the whole wrapper.
     */

    /*
     * Gen3 US/DS Preset
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0458,
      Engine->Type.Port.LaneEqualizationCntl.DsRxPresetHint,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0460,
      Engine->Type.Port.LaneEqualizationCntl.UsRxPresetHint,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0459,
      Engine->Type.Port.LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0461,
      Engine->Type.Port.LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

    /*
     * Gen4 US/DS Preset
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0432,
      Engine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0434,
      Engine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

    /*
     * Gen5 US/DS Preset
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0433,
      Engine->Type.Port.Gen5LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0435,
      Engine->Type.Port.Gen5LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

    /*
     * Gen5 Precoding Request
     */
    if (Engine->Type.Port.PortData.LinkSpeedCapability == PcieGen5 ||
      Engine->Type.Port.PortData.LinkSpeedCapability == PcieGenMaxSupported) {
      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0442 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        Engine->Type.Port.Gen5LaneEqualizationCntl.PrecodeRequest,
        Wrapper->WrapId
        );
    }

    /*
     * Gen5 Advertise EQ To High Rate Support = 0 on P4 and P5
     */
    if (((GnbHandle->RBIndex & 0x1) == 0) && (Wrapper->WrapId == 2)) {
      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0436 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        0,
        Wrapper->WrapId
        );
    }

    /*
     * These are all programmed per engine
     */

    /*
     * Gen3 Disable Phase 2/3 EQ
     */
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1475),
      (uint32_t) ~(SIL_RESERVED_1364),
      Engine->Type.Port.DisGen3EQPhase << SIL_RESERVED_1365,
        0
      );

    /*
     * Gen4 Disable Phase2/3 EQ
     */
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1473),
      (uint32_t) ~(SIL_RESERVED_1344),
      Engine->Type.Port.DisGen4EQPhase << SIL_RESERVED_1345,
        0
      );

    /*
     * Gen5 Disable Phase2/3 EQ
     */
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1474),
      (uint32_t) ~(SIL_RESERVED_1354),
      Engine->Type.Port.DisGen5EQPhase << SIL_RESERVED_1355,
        0
      );

    /*
     * ASPM
     */
    if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
      if ((GnbHandle->SocketId == SilData->EarlyBmcLinkSocket) &&
        (SilData->EarlyBmcLinkLaneNum >= Wrapper->StartPhyLane) &&
        (SilData->EarlyBmcLinkLaneNum <= Wrapper->EndPhyLane)) {
        Engine->Type.Port.PortCapabilities.AspmCapability = 0;
      }
    }
    Index = SIL_RESERVED_0464 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.PortCapabilities.AspmCapability & 0x2, Wrapper->WrapId);

    /*
     * Link PM SubState
     */
    WritePcieStrapBrh(GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_0462, Engine->Type.Port.PortId),
      0,
      Wrapper->WrapId
      );

    WritePcieStrapBrh(GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_0431, Engine->Type.Port.PortId),
      0,
      Wrapper->WrapId
      );

    if ((Engine->Type.Port.PortCapabilities.AspmL1_1 == 1) || (Engine->Type.Port.PortCapabilities.AspmL1_2 == 1)) {
      if (Engine->Type.Port.PortCapabilities.AspmL1_2 == 1) {
        Index = SIL_RESERVED_0411 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF);
        WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);

        Index = SIL_RESERVED_0463 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF);
        WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);
      }

      if (Engine->Type.Port.PortCapabilities.AspmL1_1 == 1) {
        Index = SIL_RESERVED_0411 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF);
        WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);
      }
    }

    /*
     * DPC capability
     */
    if (Engine->Type.Port.PortCapabilities.DownstreamPortContainment != 0) {
      Index = SIL_RESERVED_0423 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF);
      WritePcieStrapBrh(GnbHandle,
        Index,
        Engine->Type.Port.PortCapabilities.DownstreamPortContainment,
        Wrapper->WrapId
        );
    }

    /*
     * Set SIL_RESERVED_0466/SIL_RESERVED_0465
     */
    if (((SilData->AmdHotPlugHandlingMode == 5) || (SilData->AmdHotPlugForceSFIStrap)) &&
      ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerEntSSD) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugUBM) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugOCP))) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SFI Straps Engine->Type.Port.PortId = 0x%x\n", Engine->Type.Port.PortId);

      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0466, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );

      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0465, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );
    }

    // RTM Margining Support
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0452,
      (Engine->Type.Port.RxMarginPersistence? 0 : 1),
      Wrapper->WrapId
      );

    if (Engine->Type.Port.SrisEnableMode == 1) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, " SRIS Config:\n");
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisCfgType          = 0x%x\n", SilData->SrisCfgType);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisEnableMode       = 0x%x\n", Engine->Type.Port.SrisEnableMode);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisSkipInterval     = 0x%x\n", Engine->Type.Port.SrisSkipInterval);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - LowerSkpOsGenSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsGenSup);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - LowerSkpOsRcvSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsRcvSup);
      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0449 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        ((Engine->Type.Port.SrisAutoDetectMode == 1)? 0: 1),
        Wrapper->WrapId
        );
    }

    if (Engine->Type.Port.SrisAutoDetectMode == 1) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, " SRIS AutoDetect:\n");
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisAutoDetectMode   = 0x%x\n", Engine->Type.Port.SrisAutoDetectMode);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisSkpIntervalSel   = 0x%x\n", Engine->Type.Port.SrisSkpIntervalSel);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  - SrisAutodetectFactor = 0x%x\n", Engine->Type.Port.SrisAutodetectFactor);
      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0448 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        0x1,
        Wrapper->WrapId
        );
    }

    /*
     * Symbols per Clock strap programming (All Ports)
     * SPC Gen 1
     */
    Index = SIL_RESERVED_0444 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen1, Wrapper->WrapId);

    /*
     * SPC Gen 2
     */
    Index = SIL_RESERVED_0446 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen2, Wrapper->WrapId);

    /*
     * SPC Gen 3
     */
    Index = SIL_RESERVED_0447 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen3, Wrapper->WrapId);

    /*
     * SPC Gen 4
     */
    if (SilData->Enable2SpcGen4) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Overwriting SPC_Gen4 to 2 SPC\n");
      Engine->Type.Port.SpcMode.SpcGen4 = 1;
    }
    Index = SIL_RESERVED_0443 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen4, Wrapper->WrapId);

    //SPC Gen 5
    if (SilData->Enable2SpcGen5) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Overwriting SPC_Gen5 to 2 SPC\n");
      Engine->Type.Port.SpcMode.SpcGen5 = 1;
    }
    Index = SIL_RESERVED_0445 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen5, Wrapper->WrapId);


    /*
     * PCIe Compliance
     */
    Index = SIL_RESERVED_0425 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);

    //Rcv_L0s Entry
    WritePcieStrapBrh(GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_0440, Engine->Type.Port.PortId),
      1,
      Wrapper->WrapId
      );
  }

  /*
   * SELECTABLE_DEEMPHASIS
   */
  if (Engine->Type.Port.TXDeEmphasis != 0xf) {
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0418 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF),
      Engine->Type.Port.TXDeEmphasis,
      Wrapper->WrapId
      );
  }

  /*
   * Data Object Exchange (DOE)
   */
  if (SilData->DataObjectExchange) {
    Index = SIL_RESERVED_0422 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapBrh(GnbHandle, Index, 1, Wrapper->WrapId);
  }

  /*
   * All P Links (WrapId 0, 2, 5, and 7) support CXL
   */
  if (CXL_CAPABLE_RB(GnbHandle)) {
    if (SilData->AmdCxlOnAllPorts) {
      Engine->Type.Port.CxlControl = 1;
    }
    /*
     * CXL device enumeration support
     */
    if (Engine->Type.Port.CxlControl != 0) {
      PortStateInWrapper->field0 = true;

      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0441, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );

      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0438, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );

      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0414, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );
      /*
       * STRAP_BIF_LC_ALTERNATE_PROTOCOL_DETAILS_2 = 0xF (or 0x7 to disable CXL.cache)
       */
      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0439, Engine->Type.Port.PortId),
        0x41F,
        Wrapper->WrapId
        );

      /*
       * SIL_RESERVED_0437 = 0x1
       */
      WritePcieStrapBrh(GnbHandle,
        PORT_STRAP_INDEX(SIL_RESERVED_0437, Engine->Type.Port.PortId),
        1,
        Wrapper->WrapId
        );

      // Temp Gen5 Advertisement to enumerate as a CXL device
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_0508
        ),
        (uint32_t) ~(SIL_RESERVED_0497),
        ((SilData->CxlTempGen5AdvertAltPtcl? 1 : 0 ) <<
          SIL_RESERVED_0498),
        0
        );
    }
  }

  /*
   * FAPE Enable Gen3
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1478),
    (uint32_t)  ~(SIL_RESERVED_1370),
    (Engine->Type.Port.LaneEqualizationCntl.LcFapeEnable8GT << SIL_RESERVED_1371),
    0
    );

  /*
   * FAPE Enable Gen4
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1476),
    (uint32_t)  ~(SIL_RESERVED_1366),
    (Engine->Type.Port.Gen4LaneEqualizationCntl.LcFapeEnable16GT <<
      SIL_RESERVED_1367),
    0
    );

  /*
   * FAPE Enable Gen5
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1477),
    (uint32_t)  ~(SIL_RESERVED_1368),
    (Engine->Type.Port.Gen5LaneEqualizationCntl.LcFapeEnable32GT <<
      SIL_RESERVED_1369),
    0
    );

  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1492),
    (uint32_t) ~(SIL_RESERVED_1462),
    0 << SIL_RESERVED_1463,
      0
    );

  // Set STRAP_BIF_TPH_SUPPORTED to indicate TPH and Extended TPH are supported in the downstream port
  if (SilData->AmdFabricSdxi == true) {
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0476, 1, Wrapper->WrapId);
  }

  if (Engine->Type.Port.SrisEnableMode == 1) {
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0419, 0, Wrapper->WrapId);
  }

}

/**-------------------------------------------------------------------
 * MpioCfgBeforeReconfigWrapperCallbackBrh
 *
 * @brief Per-Wrapper Callback for wrapper configuration before bifurcation
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgWrapperBeforeBifurcationBrh (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{

  uint16_t               Index;
  SIL_RESERVED_STRUCT_0038  PortStateInWrapper;
  MPIOCLASS_INPUT_BLK    *SilData;
  NBIO_IP2IP_API         *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  PortStateInWrapper.field0 = false;
  PortStateInWrapper.field1 = false;

  if (Wrapper->IsEarlyConfigured) {
    if (IsEarlyTrainedBmcInWrapperBrh(Wrapper, GnbHandle)) {
      EarlyTrainingMpioCfgBeforeReconfigWrapperBrh(Wrapper, GnbHandle);
    }
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioCfgBeforeReconfigCallbackBrh,
    (void *) &PortStateInWrapper,
    Wrapper
    );

  /*
   * All P Links (WrapId 0, 2, 5, and 7) support CXL
   */
  if (CXL_CAPABLE_RB(GnbHandle)) {
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0415,
      0x800 + GnbHandle->RBIndex,
      Wrapper->WrapId
      );
    if (PortStateInWrapper.field0) {
      /*
       * STRAP_BIF_CXL_EN = 0b1
       */
      WritePcieStrapBrh(GnbHandle,
        SIL_RESERVED_0416,
        1,
        Wrapper->WrapId
        );

      /*
       * CXL correctable error logging
       */
      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1502),
        (uint32_t) ~(SIL_RESERVED_1298),
        ((SilData->CxlCorrectableErrorLogging? 1 : 0) << SIL_RESERVED_1299),
        0
        );

      /*
       * CXL uncorrectable error logging
       */
      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1502),
        (uint32_t) ~(SIL_RESERVED_1300),
        ((SilData->CxlUnCorrectableErrorLogging? 2 : 0) << SIL_RESERVED_1301),
        0
        );
    }
  }

  if ((SilData->CfgSevSnpSupport == true) || \
    (SilData->CfgSevTioSupport == true) || \
    (SilData->PcieIdeCapSup == true)) {
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0429,
      1,
      Wrapper->WrapId
      );
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A383020),
      (uint32_t) ~(SIL_RESERVED_1509),
      0 << SIL_RESERVED_1510,
        0
      );
  } else {
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A383020),
      (uint32_t) ~(SIL_RESERVED_1509),
      1 << SIL_RESERVED_1510,
        0
      );
  }

  WritePcieStrapBrh(GnbHandle,
    SIL_RESERVED_0428,
    1,
    Wrapper->WrapId
    );

  if (PortStateInWrapper.field1) {
    /*
     * CPL Abort Error
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0413,
      SilData->CfgAEREnable? 1 : 0,
      Wrapper->WrapId
      );

    /*
     * Multicast capability
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0479,
      3,
      Wrapper->WrapId
      );

    /*
     * Receiver Error report
     */
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380008),
      (uint32_t) ~(SIL_RESERVED_1302),
      (SilData->CfgRcvErrEnable? 1 : 0) << SIL_RESERVED_1303,
        0
      );

    /*
     * ACS Enablement
     */
    for (Index = SIL_RESERVED_0408; Index <= SIL_RESERVED_0409; Index++) {
      WritePcieStrapBrh(GnbHandle,
        (uint16_t) Index,
        Wrapper->AcsSupport,
        Wrapper->WrapId
        );
    }

    /*
     * Alternative Routing-ID Interpretation Forwarding
     */
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0471, Wrapper->AriForwarding, Wrapper->WrapId);

    /*
     * Lane Margining
     */
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0453, Wrapper->LaneMargining, Wrapper->WrapId);

    /*
     * Native PCIe Enclosure Management (NPEM)
     */
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0456, Wrapper->NativePCIeEnclosureManagement, Wrapper->WrapId);

    /*
     * Advanced error reporting (AER)
     */
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0410, Wrapper->AdvancedErrorReporting, Wrapper->WrapId);
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0470, Wrapper->AdvancedErrorReporting, Wrapper->WrapId);

    /*
     * ECRC Capability
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0427,
      (((Wrapper->ECRCSupport & 0x1) == 0x1) ? 1 : 0),
      Wrapper->WrapId
      );
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0473,
      (((Wrapper->ECRCSupport & 0x1) == 0x1) ? 1 : 0),
      Wrapper->WrapId
      );
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0426,
      (((Wrapper->ECRCSupport & 0x2) == 0x2) ? 1 : 0),
      Wrapper->WrapId
      );
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0472,
      (((Wrapper->ECRCSupport & 0x2) == 0x2) ? 1 : 0),
      Wrapper->WrapId
      );

    /*
     * LTR Support
     */
    WritePcieStrapBrh(GnbHandle, SIL_RESERVED_0451, Wrapper->LtrSupport, Wrapper->WrapId);

    /*
     * Surprise Down Feature
     */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0469,
      SilData->SurpriseDownFeature? 1 : 0,
      Wrapper->WrapId
      );
  }
}

/*
 *=========================================================================================
 * Timepoint after reconfig and before port training
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfigCallbackBrh
 *
 * @brief Per-Engine Callback for configuration after bifurcation
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgAfterReconfigCallbackBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_WRAPPER_CONFIG   *Wrapper
  )
{
  GNB_HANDLE                                    *GnbHandle;
  uint32_t                                      Value32;
  PCIe_DPC_STATUS_DATA                          *DpcStatusData;
  SIL_RESERVED_UNION_0023                     LinkCap;
  SIL_RESERVED_UNION_0012  DvsecFlexBusPort;
  PCI_ADDR                                      PciDevice;
  MPIOCLASS_INPUT_BLK                           *SilData;
  NBIO_IP2IP_API                                *NbioIp2Ip;
  SIL_RESERVED_UNION_0035                          PcieSdpCtrl;
  uint32_t                                      Value;
  uint32_t                                      Index;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));

  MpioSmnPrivateRegRead(GnbHandle,
    WRAP_SPACE(GnbHandle,
    Wrapper,
    SIL_RSVD_ADDR_1A38018C
    ),
    &PcieSdpCtrl.Value
    );

  switch (Wrapper->WrapId) {
  case SIL_RESERVED_1738:
    PcieSdpCtrl.Field.field_bits_26_to_28 = SIL_RESERVED_1737 & 0x7;
    PcieSdpCtrl.Field.field_bits_0_to_3 = SIL_RESERVED_1737 >> 3;
    break;
  case SIL_RESERVED_1733:
    PcieSdpCtrl.Field.field_bits_0_to_3 = SIL_RESERVED_1732;
    // For 4x4, use per-port UNIT_ID
    for (Index = 0; Index < SIL_RESERVED_1744; Index++) {
      Value = SIL_RESERVED_1730 + Index;
      MpioSmnPrivateRegWrite(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, Index, SIL_RESERVED_0509),
        &Value,
        GNB_REG_ACC_FLAG_S3SAVE
        );
    }
    break;
  case SIL_RESERVED_1753:
    PcieSdpCtrl.Field.field_bits_0_to_3 = SIL_RESERVED_1752;
    // For 3x3, use per-port UNIT_ID
    for (Index = 0; Index < SIL_RESERVED_1745; Index++) {
      Value = SIL_RESERVED_1751 + Index;
      MpioSmnPrivateRegWrite(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, Index, SIL_RESERVED_0509),
        &Value,
        GNB_REG_ACC_FLAG_S3SAVE
        );
    }
    break;
  default:
    assert(false);
  }

  MpioSmnPrivateRegWrite(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A38018C),
    &PcieSdpCtrl.Value,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  if (((AFTER_RECONFIG_BUFFER *) Buffer)->DpcStatusData != NULL) {
    DpcStatusData = (((AFTER_RECONFIG_BUFFER *) Buffer)->DpcStatusData);

    Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1624)
      );

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "SocketId=%d, DieNumber=%d, BRIndex=%d Bus=0x%x CoreID=%d PortID=%d PCIE_DPC_STATUS(0x388) = 0x%x\n",
      GnbHandle->SocketId,
      GnbHandle->DieNumber,
      GnbHandle->RBIndex,
      GnbHandle->Address.Address.Bus,
      Wrapper->WrapId,
      (Engine->Type.Port.PortId),
      Value32
      );

    if (Value32 & BIT_32(0)) {
      if (DpcStatusData->size < MAX_NUMBER_DPCSTATUS) {
        DpcStatusData->DpcStatusArray[DpcStatusData->size].SocketId = (uint8_t) GnbHandle->SocketId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DieID = (uint8_t) GnbHandle->DieNumber;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].RBIndex = (uint8_t) GnbHandle->RBIndex;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].BusNumber = (uint8_t) GnbHandle->Address.Address.Bus;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIeCoreID = (uint8_t) Wrapper->WrapId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIePortID = (uint8_t) (Engine->Type.Port.PortId);
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DpcStatus = (uint16_t) Value32;

        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1624),
          Value32
          );

        DpcStatusData->size++;
      }
    }
  }

  Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1464)
    );
  if ((Value32 >> 16) & SIL_RESERVED_1250) {
    ((AFTER_RECONFIG_BUFFER *) Buffer)->IsPortActive = true;
  }
  /*
   * Gen3 EQ Search Mode
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1475),
    (uint32_t) ~(SIL_RESERVED_1358),
    Engine->Type.Port.EqSearchMode << SIL_RESERVED_1359,
      0
    );

  /*
   * Gen4 EQ Search Mode
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1473),
    (uint32_t) ~(SIL_RESERVED_1338),
    Engine->Type.Port.EqSearchModeGen4 << SIL_RESERVED_1339,
      0
    );

  /*
   * Gen5 EQ Search Mode
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1474),
    (uint32_t) ~(SIL_RESERVED_1348),
    Engine->Type.Port.EqSearchModeGen5 << SIL_RESERVED_1349,
      0
    );

  /*
   * Gen3 Bypass Phase3 EQ
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1475),
    (uint32_t) ~(SIL_RESERVED_1356),
    Engine->Type.Port.BypassGen3EQ << SIL_RESERVED_1357,
      0
    );

  /*
   * Gen4 Bypass Phase3 EQ
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1473),
    (uint32_t) ~(SIL_RESERVED_1336),
    Engine->Type.Port.BypassGen4EQ << SIL_RESERVED_1337,
      0
    );

  /*
   * Gen5 Bypass Phase3 EQ
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1474),
    (uint32_t) ~(SIL_RESERVED_1346),
    Engine->Type.Port.BypassGen5EQ << SIL_RESERVED_1347,
      0
    );

  /*
   * Preset Search Mask Gen3
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1481),
    (uint32_t)  ~(SIL_RESERVED_1390),
    (Engine->Type.Port.LaneEqualizationCntl.LcPresetMask8Gt << SIL_RESERVED_1391),
    0
    );

  /*
   * Preset Search Mask Gen4
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1481),
    (uint32_t)  ~(SIL_RESERVED_1386),
    (Engine->Type.Port.Gen4LaneEqualizationCntl.LcPresetMask16Gt <<
      SIL_RESERVED_1387),
    0
    );

  /*
   * Preset Search Mask Gen5
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1481),
    (uint32_t)  ~(SIL_RESERVED_1388),
    (Engine->Type.Port.Gen5LaneEqualizationCntl.LcPresetMask32Gt <<
      SIL_RESERVED_1389),
    0
    );

  /*
   * Gen3 Fixed Preset
   */
  if (Engine->Type.Port.SetGen3FixedPreset) {
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1475),
      (uint32_t)  ~(SIL_RESERVED_1360),
      (1 << SIL_RESERVED_1361),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1475),
      (uint32_t)  ~(SIL_RESERVED_1362),
      (Engine->Type.Port.Gen3FixedPreset << SIL_RESERVED_1363),
      0
      );
  }

  /*
   * Gen4 Fixed Preset
   */
  if (Engine->Type.Port.SetGen4FixedPreset) {
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1473),
      (uint32_t)  ~(SIL_RESERVED_1340),
      (1 << SIL_RESERVED_1341),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1473),
      (uint32_t)  ~(SIL_RESERVED_1342),
      (Engine->Type.Port.Gen4FixedPreset << SIL_RESERVED_1343),
      0
      );
  }

  /*
   * Gen5 Fixed Preset
   */
  if (Engine->Type.Port.SetGen5FixedPreset) {
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1474),
      (uint32_t)  ~(SIL_RESERVED_1350),
      (1 << SIL_RESERVED_1351),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1474),
      (uint32_t)  ~(SIL_RESERVED_1352),
      (Engine->Type.Port.Gen5FixedPreset << SIL_RESERVED_1353),
      0
      );
  }

  /*
   * Multi Upstream Auto Speed Change
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1482),
    (uint32_t) ~(SIL_RESERVED_1392),
    Engine->Type.Port.PortFeatures.AutoSpdChngEn << SIL_RESERVED_1393,
      0
    );

  /*
   * Multi Auto Speed Change On Last Rate
   */
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1469),
    (uint32_t) ~(SIL_RESERVED_1322),
    ((SilData->LcMultAutoSpdChgOnLastRateEnable? 1 : 0) << SIL_RESERVED_1323),
    0
    );

  // BIFC.LINK_CNTL_32GT.EQ_BYPASS_TO_HIGHEST_RATE_DIS = !(AdvertiseEqToHiRate)
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1618),
    (uint32_t) ~(SIL_RESERVED_0820),
    !(Engine->Type.Port.Gen5LaneEqualizationCntl.AdvertiseEqToHiRate) <<
      SIL_RESERVED_0821
    );
  /*
   * CXL Alternate Protocol Support
   */
  if (Engine->Type.Port.CxlControl != 0) {
    /*
     * BIFC.AP_CNTL.AP_NEGOTIATION_GLOBAL_EN = 0b1
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1610),
      (uint32_t) ~(SIL_RESERVED_1521),
      1 << AP_CNTL_AP_NEGOTIATION_GLOBAL_EN_OFFSET
      );

    /*
     * BIFC.LINK_CNTL_32GT.MODIFIED_TS_USAGE_MODE_SELECTED = 0x2
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1618),
      (uint32_t) ~(SIL_RESERVED_1536),
      2 << SIL_RESERVED_1537
      );

    PciDevice = Engine->Type.Port.Address;
    xUSLPciRead(PciDevice.AddressValue | (SIL_RESERVED_1615 & 0xFFF),
      AccessWidth16,
      &(DvsecFlexBusPort.Value)
      );

    DvsecFlexBusPort.Field.field_bit_2 = 1;
    DvsecFlexBusPort.Field.field_bit_5 = 1;

    xUSLPciWrite(PciDevice.AddressValue | (SIL_RESERVED_1615 & 0xFFF),
      AccessWidth16,
      &(DvsecFlexBusPort.Value)
      );

    /*
     * BIFC.DVSEC_FLEX_BUS_PORT_CONTROL.CACHE_EN = 0x1 to enable CXL.cache
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1615
      ),
      (uint32_t) ~(SIL_RESERVED_1526),
      1 << SIL_RESERVED_1527
      );

    LinkCap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1616)
      );
    // Disable Sync Header Bypass for x16 ports
    if (LinkCap.Field.field_bits_4_to_9 < 0x10) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1615
        ),
        (uint32_t) ~(SIL_RESERVED_1530),
        ((SilData->SyncHeaderByPass? 1 : 0) << SIL_RESERVED_1531)
        );
    } else {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1615
        ),
        (uint32_t) ~(SIL_RESERVED_1530),
        0 << SIL_RESERVED_1531
        );
    }
  }

  //
  // Following registers are labeled as CXL in PPR but should be applied to PCIE and CXL devices
  //

  // QosNormalLimit
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0505
    ),
    (uint32_t) ~(SIL_RESERVED_0484),
    (SilData->CxlQosNormalLimit << SIL_RESERVED_0485),
    0
    );

  // QosHighLimit
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0505
    ),
    (uint32_t) ~(SIL_RESERVED_0482),
    (SilData->CxlQosHighLimit << SIL_RESERVED_0483),
    0
    );

  // QosTimerLimit
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0506
    ),
    (uint32_t) ~(BITS_16_TO_27_MASK_0002),
    (SilData->CxlQosTimerLimit << SIL_RESERVED_0488),
    0
    );

  // QosSchedGap
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0506
    ),
    (uint32_t) ~(SIL_RESERVED_0486),
    (SilData->CxlQosSchedGap << SIL_RESERVED_0487),
    0
    );

  // QosVariableGap
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0507
    ),
    (uint32_t) ~(SIL_RESERVED_0493),
    ((SilData->CxlQosVariableGap? 1 : 0) << SIL_RESERVED_0494),
    0
    );

  // QosRdspIncMode
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0507
    ),
    (uint32_t) ~(SIL_RESERVED_0489),
    ((SilData->CxlQosRdspIncMode? 1 : 0) << SIL_RESERVED_0490),
    0
    );

  // QosTimerDecNum
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0507
    ),
    (uint32_t) ~(SIL_RESERVED_0491),
    (SilData->CxlQosTimerDecNum << SIL_RESERVED_0492),
    0
    );

  // QosRdspIncNum
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0507
    ),
    (uint32_t) ~(SIL_RESERVED_0489),
    (SilData->CxlQosRdspIncNum << SIL_RESERVED_0490),
    0
    );

  // QosWrrspIncNum
  MpioSmnPrivateRegRMW(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_0507
    ),
    (uint32_t) ~(SIL_RESERVED_0495),
    (SilData->CxlQosWrrspIncNum << SIL_RESERVED_0496),
    0
    );

  return;
}

/**-------------------------------------------------------------------
 * MpioCfgWrapperAfterBifurcationBrh
 *
 * @brief Per-Wrapper Callback for configuration after bifurcation
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
void
MpioCfgWrapperAfterBifurcationBrh (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  if (Wrapper->IsEarlyConfigured && (IsEarlyTrainedBmcInWrapperBrh(Wrapper, GnbHandle) == false)) {
    return;
  }

  ((AFTER_RECONFIG_BUFFER *) Buffer)->IsPortActive = false;

  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioCfgAfterReconfigCallbackBrh,
    Buffer,
    Wrapper
    );

  if (((AFTER_RECONFIG_BUFFER *) Buffer)->IsPortActive) {
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A3801B0),
      (uint32_t) ~(SIL_RESERVED_1434 |
      SIL_RESERVED_1432),
      (uint32_t) (1 << SIL_RESERVED_1435) |
      (1 << SIL_RESERVED_1433),
      0
      );
  }

  if (SilData->AmdRxMarginEnabled) {
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1506),
      (uint32_t) ~(SIL_RESERVED_1444),
      (0x1 << SIL_RESERVED_1445),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1504),
      (uint32_t) ~(SIL_RESERVED_1440 | SIL_RESERVED_1438),
      (0x10 << SIL_RESERVED_1441) |
      (0x19 << SIL_RESERVED_1439),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1505),
      (uint32_t) ~(SIL_RESERVED_1442),
      (0xF << SIL_RESERVED_1443),
      0
      );
  }

  if (SilData->Master7bitSteeringTag) {
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380714),
      (uint32_t) ~(SIL_RESERVED_1400),
      (0x1 << SIL_RESERVED_1401),
      0
      );
    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380064),
      (uint32_t) ~(SIL_RESERVED_1436),
      (0x1 << SIL_RESERVED_1437),
      0
      );
  }
}

//=========================================================================================
// Timepoint after MPIO firmware initialization completes
//=========================================================================================

/**-------------------------------------------------------------------
 *
 * SubsystemIdSettingBrh
 *
 * @brief PCIE interface to configure register setting after Mpio init done
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
static
void
SubsystemIdSettingBrh (
  GNB_HANDLE       *GnbHandle
  )
{

  uint32_t      Value;
  uint32_t      SubsystemDeviceId;
  uint32_t      SubsystemVendorId;
  MPIOCLASS_INPUT_BLK   *SilData;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    " Enter for Socket %d Nbio %d\n",
    GnbHandle->SocketId,
    GnbHandle->RBIndex
    );

  /*
   * NB ADAPTER D0F0
   */
  Value = SilData->CfgNbioSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNbioSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1609),
      Value
      );
  }

  /*
   * IOMMU
   */
  Value = SilData->CfgIommuSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgIommuSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13F00078),
      Value
      );
  }

  if ((GnbHandle->RBIndex * 1) == 0) {
    /*
     * PSPCCP
     */
    Value = SilData->CfgPspccpSsid;
    if (Value != 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgPspccpSsid = %x\n", Value);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0858),
        Value
        );
    }

    /*
     * NTBCCP / PTDMA
     */
    Value = SilData->CfgNtbccpSsid;
    if (Value != 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNtbccpSsid = %x\n", Value);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0861),
        Value
        );
    }

    /*
     * NBIF Dummy Functions
     */
    Value = SilData->CfgNbifF0Ssid;
    if (Value != 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNbifF0Ssid = %x\n", Value);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0822),
        Value
        );

      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0823),
        Value
        );

      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0824),
        Value
        );
    }

    /*
     * NTB
     */
    Value = SilData->CfgNtbSsid;
    if (Value != 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNtbSsid = %x\n", Value);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0851),
        Value
        );

      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_0859),
        Value
        );
    }
  }

  /*
   * PCIE
   */
  SubsystemDeviceId = (uint32_t) SilData->AmdPcieSubsystemDeviceID;
  if (SubsystemDeviceId != 0) {
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0467,
      SubsystemDeviceId,
      0
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0467,
      SubsystemDeviceId,
      1
      );
  }

  SubsystemVendorId = (uint32_t) SilData->AmdPcieSubsystemVendorID;
  if (SubsystemVendorId != 0) {
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0468,
      SubsystemVendorId,
      0
      );

    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0468,
      SubsystemVendorId,
      1
      );
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 * PcieCommonCoreConfigurationBrh
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns SIL_STATUS
 * @retval Should always return SilPass
 **/
static
SIL_STATUS
PcieCommonCoreConfigurationBrh (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                *GnbHandle;
  uint32_t                  Value;
  MPIOCLASS_INPUT_BLK       *SilData;
  NBIO_IP2IP_API            *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Enter for Socket %d Nbio %d Wrapper %d\n",
    GnbHandle->SocketId,
    GnbHandle->RBIndex,
    Wrapper->WrapId
    );

  Value = (Wrapper->WrapId == 0) ? (uint32_t) SilData->GppAtomicOps : (uint32_t) SilData->GfxAtomicOps;
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1507),
    (uint32_t) ~(SIL_RESERVED_1450 | SIL_RESERVED_1452),
    (Value << SIL_RESERVED_1451) | (Value << SIL_RESERVED_1453),
    0
    );

  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1508),
    (uint32_t) ~(SIL_RESERVED_1458 | SIL_RESERVED_1460),
    (0x0 << SIL_RESERVED_1459 | 0x1 << SIL_RESERVED_1461),
    0
    );

  if (SilData->AmdNbioReportEdbErrors ) {
    Value = 0;
  } else {
    Value = 1;
  }

  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1503),
    (uint32_t) ~(SIL_RESERVED_1404 | SIL_RESERVED_1402),
    Value << SIL_RESERVED_1405 | 0x1 << SIL_RESERVED_1403,
      0
    );

  // Change SLV_PHDR_CREDITS_RSVD and SLV_CTRL_1_SLV_PHDR from default(4) to 9
  MpioSmnPrivateRegRMW(GnbHandle,
    WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A38007C),
    (uint32_t) ~(BITS_8_TO_15_MASK_0005 | BITS_24_TO_31_MASK_0004),
    9 << SIL_RESERVED_1512 | 9 << SIL_RESERVED_1511,
      0
    );
  return SilPass;
}

/**-------------------------------------------------------------------
 *
 * PcieCommonEngineConfigurationBrh
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @details tbd
 *
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
PcieCommonEngineConfigurationBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  SIL_RESERVED_UNION_0036 TxRequesterId;
  uint32_t                    Value32;
  MPIOCLASS_INPUT_BLK         *SilData;
  NBIO_IP2IP_API              *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Value32 = 0;
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
    ((Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled) &&
    (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugInboard))) {

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      " Enter for Socket %d Nbio %d Wrapper %d Engine\n",
      GnbHandle->SocketId,
      GnbHandle->RBIndex,
      Wrapper->WrapId,
      Engine->Type.Port.PortId
      );

    MpioSmnPrivateRegRead(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1493
      ),
      &TxRequesterId.Value
      );

    TxRequesterId.Field.field_bits_0_to_2 = Engine->Type.Port.PortData.FunctionNumber;
    TxRequesterId.Field.field_bits_3_to_7 = Engine->Type.Port.PortData.DeviceNumber;
    TxRequesterId.Field.field_bits_8_to_15 = GnbHandle->Address.Address.Bus;

    MpioSmnPrivateRegWrite(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1493
      ),
      &TxRequesterId.Value,
      GNB_REG_ACC_FLAG_S3SAVE
      );

    /*
     * Set Slot Power Limit
     */
    xUSLPciRMW((GnbHandle->Address.AddressValue |
      MAKE_SBDFO(0,
      0,
      Engine->Type.Port.PortData.DeviceNumber,
      Engine->Type.Port.PortData.FunctionNumber,
      PCICFG_OFFSET(SIL_RESERVED_1630)
      )),
      AccessWidth32,
      (uint32_t) ~(SIL_RESERVED_1594 |
      SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK),
      (uint32_t) ((Engine->Type.Port.SlotPowerLimit << SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET) |
      (Engine->Type.Port.SlotPowerLimitScale << SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET))
      );


    /*
     * Set slot_implemented
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1622),
      (uint32_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
      (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16))
      );

    /*
     * Set Physical Slot Number
     */
    xUSLPciRMW((GnbHandle->Address.AddressValue |
      MAKE_SBDFO(0,
      0,
      Engine->Type.Port.PortData.DeviceNumber,
      Engine->Type.Port.PortData.FunctionNumber,
      PCICFG_OFFSET(SIL_RESERVED_1630)
      )),
      AccessWidth32,
      (uint32_t) ~SLOT_CAP_PHYSICAL_SLOT_NUM_MASK,
      (uint32_t) (Engine->Type.Port.PortData.SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET)
      );

    /*
     * Set Completion Timeout
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1613),
      (uint32_t) ~(SIL_RESERVED_1522),
      (0x6 << SIL_RESERVED_1523)
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1472),
      (uint32_t) ~(SIL_RESERVED_1334),
      (0x1 << SIL_RESERVED_1335),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1480
      ),
      (uint32_t) ~(SIL_RESERVED_1376 |
      SIL_RESERVED_1382),
      (0x1 << SIL_RESERVED_1377) |
      (0x1 << SIL_RESERVED_1383),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1480
      ),
      (uint32_t) ~(SIL_RESERVED_1380),
      (0x1 << SIL_RESERVED_1381),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1468
      ),
      (uint32_t) ~(SIL_RESERVED_1316),
      (0x1 << SIL_RESERVED_1317),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1468),
      (uint32_t) ~(SIL_RESERVED_1312),
      (0x1 << SIL_RESERVED_1313),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1469),
      (uint32_t) ~(SIL_RESERVED_1320),
      (0x1 << SIL_RESERVED_1321),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1485),
      (uint32_t) ~(SIL_RESERVED_1398),
      1 << SIL_RESERVED_1399,
        0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1489 ),
      (uint32_t) ~(SIL_RESERVED_1420),
      (0x1 << SIL_RESERVED_1421),
      0
      );

    /*
     * Set Tx Margin
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1617),
      (uint32_t) ~(SIL_RESERVED_1534),
      (Engine->Type.Port.TXMargin << SIL_RESERVED_1535)
      );

    /*
     * Set Powerdown state in L1/L1.1/L1.2
     */
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1470),
      (uint32_t) ~(SIL_RESERVED_1326),
      ((Engine->Type.Port.PortFeatures.L1PowerDown & 1) << SIL_RESERVED_1327),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1479),
      (uint32_t) ~(SIL_RESERVED_1372),
      ((Engine->Type.Port.PortFeatures.L11PowerDown & 7) << SIL_RESERVED_1373),
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1479),
      (uint32_t) ~(SIL_RESERVED_1374),
      ((Engine->Type.Port.PortFeatures.L12PowerDown & 7) << SIL_RESERVED_1375),
      0
      );

    /*
     * SRNS Enable Mode
     */
    if (Engine->Type.Port.SrisEnableMode == 1 || Engine->Type.Port.SrisAutoDetectMode == 1) {
      MpioSmnPrivateRegRead(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1483),
        &Value32
        );

      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PCIE_LC_SPEED_CNTL = 0x%x\n", Value32);

      Value32 = ((Value32 & SIL_RESERVED_1397)? 0x1F : \
        ((Value32 & SIL_RESERVED_1396)? 0xF : \
        ((Value32 & SIL_RESERVED_1395)? 0x7 : \
        ((Value32 & SIL_RESERVED_1394)? 0x3 : 0x1))));

      MPIO_TRACEPOINT(SIL_TRACE_INFO, "set STRAP_LOWER_SKP_OS_GEN_SUPPORT = 0x%x\n", Value32);

      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0477 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        ((Engine->Type.Port.SrisAutoDetectMode != 1 && (SilData->SrisCfgType & (SRIS_DBG | SRIS_DBG_PBS)))?
        ((Engine->Type.Port.LowerSkpOsGenSup <= Value32)? Engine->Type.Port.LowerSkpOsGenSup : Value32) : 0),
        Wrapper->WrapId
        );

      WritePcieStrapBrh(GnbHandle,
        (SIL_RESERVED_0478 + (STRAP_BIF_PORT_DIFF * Engine->Type.Port.PortId)),
        ((Engine->Type.Port.SrisAutoDetectMode != 1 && (SilData->SrisCfgType & (SRIS_DBG | SRIS_DBG_PBS)))?
        ((Engine->Type.Port.LowerSkpOsRcvSup <= Value32)? Engine->Type.Port.LowerSkpOsRcvSup : Value32) : 0),
        Wrapper->WrapId
        );

      MpioSmnPrivateRegRead(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1471),
        &Value32
        );

      if (Engine->Type.Port.SrisAutoDetectMode == 1) {
        /*
         * Set AutoDetect Mode & Autodetection factor
         */
        Value32 &= ~(SIL_RESERVED_1331 | SIL_RESERVED_1329);
        Value32 |= (Engine->Type.Port.SrisSkpIntervalSel << SIL_RESERVED_1332) | \
          (Engine->Type.Port.SrisAutodetectFactor << SIL_RESERVED_1330) | \
          (1 << SIL_RESERVED_1328);
      } else {
        /*
         * SRIS
         */
        if (Engine->Type.Port.SrisEnableMode == 1) {
          Value32 |= (1 << SIL_RESERVED_1333);
        }
      }

      MpioSmnPrivateRegWrite(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1471),
        &Value32,
        0
        );
    }

    /*
     * PCI-E specification states L0 Exit latency should be 7
     * to discourage older software from enabling Rx-L0s (ie. enabling TX-L0s in the EP)
     * */
    WritePcieStrapBrh(GnbHandle,
      SIL_RESERVED_0430 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF),
      7,
      Wrapper->WrapId
      );
  } else {
    /*
     * Expose Unused PCIE port
     */
    if (PcieConfigCheckPortStatus(Engine, INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE)) {
      /*
       * Set slot_implemented
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SIL_RESERVED_1622),
        (uint32_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
        (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16))
        );

      /*
       * Assign slot number
       */
      xUSLPciRMW(GnbHandle->Address.AddressValue |
        MAKE_SBDFO(0,
        0,
        Engine->Type.Port.PortData.DeviceNumber,
        Engine->Type.Port.PortData.FunctionNumber,
        PCICFG_OFFSET(SIL_RESERVED_1630)
        ),
        AccessWidth32,
        (uint32_t) ~(SLOT_CAP_PHYSICAL_SLOT_NUM_MASK),
        (uint32_t)(Engine->Type.Port.PortData.SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET)
        );
    }
  }

  if (SilData->CfgPcieCVTestWA == 1) {
    Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1616)
      );

    if ((Value32 & 0xF) >= 0x03) {
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1468),
        (uint32_t) ~(SIL_RESERVED_1314),
        (1 << SIL_RESERVED_1315),
        0
        );
    }
  }
}

static
void
CheckDeliIfEngineIsBmcBrh (
  GNB_HANDLE            *GnbHandle,
  PCIe_ENGINE_CONFIG    *Engine
  )
{
  uint32_t                        Response;
  uint32_t                        MpioArg[6];
  uint8_t                         DELIBuf[sizeof (DXIO_FW_MP1_DELI_INFO)];
  DXIO_FW_MP1_DELI_INFO           *pDeliBuf;
  MPIOCLASS_INPUT_BLK             *SilData;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIO IP block is located at %x\n", SilData);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset((void *)DELIBuf, 0, sizeof (DELIBuf));

  memset(MpioArg, 0x00, sizeof (MpioArg));
  MpioArg[1] = 0;                                        // Address High
  MpioArg[2] = (uint32_t) (uintptr_t) DELIBuf;           // Address Low
  MpioArg[4] = (uint32_t) SilData->EarlyBmcLinkLaneNum;  // StartLane

  Response = MpioServiceRequestCommon(NbioGetHostPciAddress(GnbHandle), BIOS_MPIO_MSG_GET_DELI_INFO, MpioArg, 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);
  pDeliBuf = (DXIO_FW_MP1_DELI_INFO *) ((uintptr_t)MpioArg[2]);

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Port Number DELI = %d, Port ID Engine = %d\n",
    pDeliBuf->Proto.Pcie.PCIE_port_num,
    Engine->Type.Port.PortId
    );

  if (pDeliBuf->Proto.Pcie.PCIE_port_num == Engine->Type.Port.PortId) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "DELI INFO for InstanceId = %d, StartLane = %d \n",
      GnbHandle->InstanceId,
      SilData->EarlyBmcLinkLaneNum
      );
    xUslDumpBuffer((void *)((uintptr_t)MpioArg[2]), sizeof (DELIBuf), 1);
    PrintDeliInfo(pDeliBuf);
    Engine->Type.Port.IsBmcLocation = 1;
  }
}


/**-------------------------------------------------------------------
 *
 * PcieCommonEngineGetDeliInfoBrh
 *
 * @brief Get the DXIO Error Log Interface (DELI) information for PCIe engine
 *
 * @details tbd
 *
 * @param[in] PCIe_ENGINE_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
PcieCommonEngineGetDeliInfoBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  ) {
  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  NBIO_IP2IP_API              *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

  GetDeliInfo(GnbHandle, Engine);

  if (IsEarlyTrainedBmcInWrapperBrh(Wrapper, GnbHandle)) {
    CheckDeliIfEngineIsBmcBrh(GnbHandle, Engine);
  }

  return;
}
/**-------------------------------------------------------------------
 * InitBusRangesAndAriBrh
 *
 * @brief After Pcie Training Enumerate all Pcie connectors for register setting.
 *
 * @param[in]     GnbHandle          Pointer to GnbHandle
 * @param[in]     Pcie               PCIe configuration info
 * @param[in]     CfgPcieAriSupport  ARI support
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
InitBusRangesAndAriBrh (
  GNB_HANDLE                           *GnbHandle,
  PCIe_PLATFORM_CONFIG                 *Pcie,
  bool                                 CfgPcieAriSupport
  )
{
  GNB_HANDLE                      *InnerHandle;
  PCIE_VDM_NODE_CTRL4_STRUCT      VdmNode;
  uint32_t                        SystemFabricID;
  uint32_t                        BusNumberBase;
  uint32_t                        BusNumberLimit;
  uint32_t                        VdmNodeAddress;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS                      Status;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Enter for Socket %d RB %d\n", GnbHandle->SocketId, GnbHandle->RBIndex);

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  assert(Status == SilPass);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  InnerHandle = NbioIp2Ip->NbioGetHandle(Pcie);

  while (InnerHandle != NULL) {
    DfIp2IpApi->DfGetRootBridgeInfo(InnerHandle->SocketId,
      InnerHandle->DieNumber,
      InnerHandle->RBIndex,
      &SystemFabricID,
      &BusNumberBase,
      &BusNumberLimit,
      NULL,
      NULL,
      NULL
      );

    VdmNode.Field.BUS_RANGE_BASE = BusNumberBase;
    VdmNode.Field.BUS_RANGE_LIMIT = BusNumberLimit;

    if (InnerHandle->InstanceId == GnbHandle->InstanceId) {
      VdmNode.Field.NODE_PRESENT = 0;
    } else {
      VdmNode.Field.NODE_PRESENT = 1;
    }

    VdmNodeAddress = SIL_RSVD_ADDR_13B10190 + (InnerHandle->InstanceId * 0x10);

    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, VdmNodeAddress),
      VdmNode.Value
      );

    InnerHandle = GnbGetNextHandle(InnerHandle);
  }

  SubsystemIdSettingBrh(GnbHandle);
  /*
   * Initialize ARI
   */
  if (true == CfgPcieAriSupport) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOHC_FEATURE_CNTL_ADDRESS),
      (uint32_t) ~(IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK),
      1 << IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET
      );
    PcieAriInit(GnbHandle);
  }
  return;
}

/**-------------------------------------------------------------------
 *
 * MpioIsEverTriedTrainingBrh
 *
 * @brief  Check each Engine that ever tried training
 *
 * @details tbd
 *
 * @param[in]     Engine  Engine configuration info
 *
 * @returns boolean
 * @retval true  - This engine is ever tried training
 *         false - This engine is not tried training
 **/
bool
MpioIsEverTriedTrainingBrh (
  PCIe_ENGINE_CONFIG        *Engine
  )
{
  GNB_HANDLE                    *GnbHandle;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      Value32;
  uint32_t                      SmnAddress;
  uint32_t                      i, j;
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  Value32 = 0;
  SmnAddress = SIL_RESERVED_1484;
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

  /*
   * Check LC_STATE0 ~ LC_STATE5
   */
  for (i = 0; i < 6; i++) {
    MpioSmnPrivateRegRead(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, Engine->Type.Port.PortId, SmnAddress + i * 4),
      &Value32
      );

    /*
     * Check LC_CURRENT_STATE and all LC_PREV_STATE
     */
    for (j = 0; j < 4; j++) {
      if (((Value32 >> (j * 8)) & 0xFF) > 0x4) {
        return true;
      }
    }
  }

  return false;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgAfterDxioInitBrh
 *
 * @brief Configuration Timepoint before DXIO firmware initialization starts
 *
 * @details tbd
 *
 * @param[in] PCIe_PLATFORM_CONFIG       Pointer silicon complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterDxioInitBrh (
  PCIe_PLATFORM_CONFIG                 *Pcie
  )
{
  GNB_HANDLE                         *GnbHandle;
  MPIOCLASS_INPUT_BLK                *SilData;
  NBIO_IP2IP_API                     *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigDebugDump(Pcie);
  NbioIp2Ip->PcieConfigRunProcForAllWrappers(DESCRIPTOR_ALL_WRAPPERS,
    PcieCommonCoreConfigurationBrh,
    NULL,
    Pcie
    );

  NbioIp2Ip->PcieConfigRunProcForAllEngines(DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieCommonEngineConfigurationBrh,
    NULL,
    Pcie
    );


  NbioIp2Ip->PcieConfigRunProcForAllEngines(DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieCommonEngineGetDeliInfoBrh,
    NULL,
    Pcie
    );

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);
  while (GnbHandle != NULL) {
    if (GnbHandle->RBIndex < 4) {
      InitBusRangesAndAriBrh(GnbHandle, Pcie, SilData->CfgPcieAriSupport);
    } else {
      if (true == SilData->CfgPcieAriSupport) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D410118),
          (uint32_t) ~(IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK),
          1 << IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET
          );
        PcieAriInit(GnbHandle);
      }
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }
}
