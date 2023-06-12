/**
 *  @file MpioMappingResults.c
 *  @brief Mpio port mapping update
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <SilCommon.h>
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include "MpioPcie.h"
#include "MpioInitLib.h"
#include <NBIO/NbioIp2Ip.h>


 /**--------------------------------------------------------------------
 *
 * MpioFindTopoForAsk
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 * @returns Topology Entry
 * @retval Valid Dxio Port Descriptor
 **/
static
MPIO_PORT_DESCRIPTOR *
MpioFindTopoForAsk (
  MPIO_PORT_DESCRIPTOR  *SocketTopology,
  FW_ASK_STRUCT         *AskEntry
  )
{
  MPIO_PORT_DESCRIPTOR      *TopologyEntry;               // Local updatable pointer for SocketTopology processing
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Looking for StartLane %d\n", AskEntry->desc.startLaneIndex);

  /*
   * Find the PlatformTopology entry for this DXIO engine
   */
  TopologyEntry = SocketTopology;       // Start of the port list for this socket
  while (TopologyEntry != NULL) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Checking User Entry StartLane %d\n", TopologyEntry->EngineData.MpioStartLane);

    /*
	 * Ignore USB4 OVER PCIE
	 */

    if (TopologyEntry->EngineData.EngineType == MpioUSB_OVER_PCIE) {
     break;
    }

    if (TopologyEntry->EngineData.EngineType == MpioPcieEngine) {
      if (TopologyEntry->EngineData.MpioStartLane == AskEntry->desc.startLaneIndex) {
        break;
      }
    }

    TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
  }
  assert (TopologyEntry != NULL);
  return (TopologyEntry);
}

 /**--------------------------------------------------------------------
 *
 * SrisPlatformConfig
 *
 * @brief Pcie SRIS Platform Configuration
 *
 * @param[in]       TopologyEntry   Pointer to platform config
 * @param[in, out]  Engine          Pointer to Engine config descriptor
 * @param[in]       AskEntry        Not used
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SrisPlatformConfig (
  MPIOCLASS_INPUT_BLK           *SilData,
  MPIO_PORT_DESCRIPTOR    *TopologyEntry,
  PCIe_ENGINE_CONFIG      *Engine,
  FW_ASK_STRUCT           *AskEntry
  )
{

  /*
   * SRIS Mode
   * Default config for all port
   */
  if (((SilData->SrisEnableMode != 0xFF) && (SilData->SrisCfgType) & 0x7) == 0) {
    Engine->Type.Port.SrisEnableMode = SilData->SrisEnableMode;
    Engine->Type.Port.SrisSkipInterval = SilData->SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = SilData->SrisLowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = SilData->SrisLowerSkpOsRcvSup;
  }

  /*
   * PORT_PARAM per-port config
   */
  if (TopologyEntry->Port.SrisEnableMode && ((SilData->SrisCfgType) & 0x7) == 0) {
    Engine->Type.Port.SrisEnableMode = TopologyEntry->Port.SrisEnableMode;
    Engine->Type.Port.SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = TopologyEntry->Port.LowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = TopologyEntry->Port.LowerSkpOsRcvSup;
  }

  if (SilData->SrisCfgType & 0x7) {
    Engine->Type.Port.SrisEnableMode = TopologyEntry->Port.SrisEnableMode;
  }

  if (SilData->SrisCfgType & 0x6) {
    Engine->Type.Port.SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = TopologyEntry->Port.LowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = TopologyEntry->Port.LowerSkpOsRcvSup;
  }

  /*
   * SRIS Autodetect
   * Default config for all port
   */
  if ((SilData->SrisAutoDetectMode != 0xFF) && ((SilData->SrisCfgType & 0x8) == 0)) {
    Engine->Type.Port.SrisAutoDetectMode = SilData->SrisAutoDetectMode;
    Engine->Type.Port.SrisSkpIntervalSel = SilData->SrisSkpIntervalSel;
    Engine->Type.Port.SrisAutodetectFactor = SilData->SrisAutodetectFactor;
  }

  /*
   * PORT_PARAM per-port config
   */
  if (TopologyEntry->Port.SrisAutoDetectMode && ((SilData->SrisCfgType & 0x8) == 0)) {
    Engine->Type.Port.SrisAutoDetectMode = TopologyEntry->Port.SrisAutoDetectMode;
    Engine->Type.Port.SrisSkpIntervalSel = TopologyEntry->Port.SrisSkpIntervalSel;
    Engine->Type.Port.SrisAutodetectFactor = TopologyEntry->Port.SrisAutodetectFactor;
  }

  if (SilData->SrisCfgType & 0x8) {
    Engine->Type.Port.SrisAutoDetectMode = TopologyEntry->Port.SrisAutoDetectMode;
    Engine->Type.Port.SrisSkpIntervalSel = TopologyEntry->Port.SrisSkpIntervalSel;
    Engine->Type.Port.SrisAutodetectFactor = TopologyEntry->Port.SrisAutodetectFactor;
  }

}

 /**--------------------------------------------------------------------
 *
 * MpioInitEngineStruct
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioInitEngineStruct (
  MPIOCLASS_INPUT_BLK           *SilData,
  MPIO_PORT_DESCRIPTOR    *TopologyEntry,
  PCIe_ENGINE_CONFIG      *Engine,
  FW_ASK_STRUCT           *AskEntry
  )
{
  Engine->EngineData.EngineType = MpioPcieEngine;

  /*
   * From Platform Topology
   */
  Engine->EngineData.StartLane = (uint16_t) TopologyEntry->EngineData.StartLane;
  Engine->EngineData.EndLane = (uint16_t) TopologyEntry->EngineData.EndLane;

  Engine->Type.Port.PortData.DeviceNumber = TopologyEntry->Port.DeviceNumber;
  Engine->Type.Port.PortData.FunctionNumber = TopologyEntry->Port.FunctionNumber;
  Engine->Type.Port.PortData.LinkSpeedCapability = TopologyEntry->Port.LinkSpeedCapability;
  Engine->Type.Port.PortData.LinkAspm = TopologyEntry->Port.LinkAspm;
  Engine->Type.Port.PortData.LinkHotplug = TopologyEntry->Port.LinkHotplug;
  Engine->Type.Port.LinkAspmL1_1 = TopologyEntry->Port.LinkAspmL1_1;
  Engine->Type.Port.LinkAspmL1_2 = TopologyEntry->Port.LinkAspmL1_2;
  Engine->Type.Port.DlfCapDisable = ((TopologyEntry->Port.Gen4Features & BIT_8(1))? 1 : 0);
  Engine->Type.Port.DlfExchangeDisable = (TopologyEntry->Port.Gen4Features & BIT_8(0));

  if (TopologyEntry->Port.SlotPowerLimit != 0) {
    Engine->Type.Port.SlotPowerLimit = TopologyEntry->Port.SlotPowerLimit;
  }
  if (TopologyEntry->Port.SlotPowerLimitScale != 0) {
    Engine->Type.Port.SlotPowerLimitScale = TopologyEntry->Port.SlotPowerLimitScale;
  }

  Engine->Type.Port.ClkReq = TopologyEntry->Port.ClkReq;
  Engine->Type.Port.PortData.MiscControls.ClkPmSupport = TopologyEntry->Port.MiscControls.ClkPmSupport;
  Engine->Type.Port.PortData.MiscControls.SbLink = TopologyEntry->Port.MiscControls.SbLink;
  Engine->Type.Port.PortData.MiscControls.CsLink = (uint8_t) (TopologyEntry->Port.CsLink);
  Engine->Type.Port.IsMasterPLL = TopologyEntry->Port.IsMasterPLL;
  Engine->Type.Port.EqSearchMode = TopologyEntry->Port.EqPreset;
  Engine->Type.Port.PortData.SlotNum = TopologyEntry->Port.SlotNum;
  Engine->Type.Port.EsmControl = TopologyEntry->Port.EsmControl;
  Engine->Type.Port.RxVetting = TopologyEntry->Port.RxVetting;
  Engine->Type.Port.TxVetting = TopologyEntry->Port.TxVetting;
  Engine->Type.Port.EsmSpeedBump = TopologyEntry->Port.EsmSpeedBump;

  if (TopologyEntry->Port.SetGen3LaneEq) {
    Engine->Type.Port.LaneEqualizationCntl.DsTxPreset = TopologyEntry->Port.LaneEqualizationCntl.DsTxPreset;
    Engine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = TopologyEntry->Port.LaneEqualizationCntl.DsRxPresetHint;
    Engine->Type.Port.LaneEqualizationCntl.UsTxPreset = TopologyEntry->Port.LaneEqualizationCntl.UsTxPreset;
    Engine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = TopologyEntry->Port.LaneEqualizationCntl.UsRxPresetHint;
  }
  if (TopologyEntry->Port.SetGen4LaneEq) {
    Engine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset = TopologyEntry->Port.Gen4LaneEqualizationCntl.DsTxPreset;
    Engine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset = TopologyEntry->Port.Gen4LaneEqualizationCntl.UsTxPreset;
  }
  if (TopologyEntry->Port.SetGen3FixedPreset) {
    Engine->Type.Port.SetGen3FixedPreset = TopologyEntry->Port.SetGen3FixedPreset;
    Engine->Type.Port.Gen3FixedPreset = TopologyEntry->Port.Gen3FixedPreset;
  }
  if (TopologyEntry->Port.SetGen4FixedPreset) {
    Engine->Type.Port.SetGen4FixedPreset = TopologyEntry->Port.SetGen4FixedPreset;
    Engine->Type.Port.Gen4FixedPreset = TopologyEntry->Port.Gen4FixedPreset;
  }
  if (TopologyEntry->Port.SetGen5FixedPreset) {
    Engine->Type.Port.SetGen5FixedPreset = TopologyEntry->Port.SetGen5FixedPreset;
    Engine->Type.Port.Gen5FixedPreset = TopologyEntry->Port.Gen5FixedPreset;
  }
  if (TopologyEntry->Port.PresetMaskCntl.SetPresetMask8Gt) {
    Engine->Type.Port.LaneEqualizationCntl.LcPresetMask8Gt = TopologyEntry->Port.PresetMaskCntl.PresetMask8Gt;
  }
  if (TopologyEntry->Port.PresetMaskCntl.SetPresetMask16Gt) {
    Engine->Type.Port.Gen4LaneEqualizationCntl.LcPresetMask16Gt = TopologyEntry->Port.PresetMaskCntl.PresetMask16Gt;
  }
  if (TopologyEntry->Port.PresetMaskCntl.SetPresetMask32Gt) {
    Engine->Type.Port.Gen5LaneEqualizationCntl.LcPresetMask32Gt = TopologyEntry->Port.PresetMaskCntl.PresetMask32Gt;
  }

  Engine->Type.Port.PsppPolicyDC = TopologyEntry->Port.PsppPolicyDC;
  Engine->Type.Port.PsppPolicyAC = TopologyEntry->Port.PsppPolicyAC;
  Engine->Type.Port.InvertPolarity = TopologyEntry->Port.InvertPolarity;
  Engine->Type.Port.InvertPolarity2 = TopologyEntry->Port.InvertPolarity2;
  Engine->Type.Port.DisGen3EQPhase = TopologyEntry->Port.DisGen3EQPhase;
  Engine->Type.Port.DisGen4EQPhase = TopologyEntry->Port.DisGen4EQPhase;
  Engine->Type.Port.TargetLinkSpeed = TopologyEntry->Port.TargetLinkSpeed;
  Engine->Type.Port.BypassGen3EQ = TopologyEntry->Port.BypassGen3EQ;
  Engine->Type.Port.BypassGen4EQ = TopologyEntry->Port.BypassGen4EQ;
  Engine->Type.Port.AlwaysExpose = TopologyEntry->Port.AlwaysExpose;
  Engine->Type.Port.I2CMuxInfo = TopologyEntry->Port.I2CMuxInfo;
  Engine->Type.Port.IsBmcLocation = TopologyEntry->Port.IsBmcLocation;

  SrisPlatformConfig (SilData, TopologyEntry, Engine, AskEntry);

  PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_ALLOCATED);
  Engine->Type.Port.PortData.PortPresent = TopologyEntry->Port.PortPresent;
}

 /**--------------------------------------------------------------------
 *
 * MpioPortMapping
 *
 * @brief DXIO Update Pcie Data Structure
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  PlatformTopology     Pointer to the platform Host Firmware supplied platform configuration
 * @param[in]  ThisNode       Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioPortMapping (
  MPIOCLASS_INPUT_BLK       *SilData,
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  )
{
  GNB_HANDLE                    *Handle;
  SIL_STATUS                    Status;
  uint32_t                      PortDevSize;
  uint8_t                       *PortDevMap;
  uint8_t                       Index;
  NBIO_IP2IP_API                *NbioIp2Ip;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  /*
   * DXIO Firmware Structures
   */
  FW_ASK_STRUCT             *AskEntry;

  /*
   * Platform Topology Pointers
   */
  MPIO_PORT_DESCRIPTOR      *SocketTopology; // Local pointer to start of User Config for this socket
  MPIO_PORT_DESCRIPTOR      *TopologyEntry;  // Local updatable pointer for SocketTopology processing

  /*
   * PCIe Complex Pointers
   */
  PCIe_ENGINE_CONFIG        *Engine;
  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }
  Status = MpioGetTopologyForSocket (GnbHandle->SocketId, PlatformTopology, &SocketTopology);

  if (SilPass == Status) {
    AskEntry = MpioData->MpioAsk;

    PortDevSize = MpioXferTable->PcieGetDeviceMappingSize();
    PortDevMap = SilData->PortDevMap;
    memset (PortDevMap, 0, PortDevSize * MAX_INSTANCE_ID);

   	/*
     * Sync Platform Topology, Ask, and Complex Entries
	   */
    for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
      if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
        TopologyEntry = MpioFindTopoForAsk (SocketTopology, AskEntry);
        if (TopologyEntry != NULL) {
          Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
          if (Engine != NULL) {
            MpioInitEngineStruct (SilData, TopologyEntry, Engine, AskEntry);
            Handle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
            PcieMapPortPciAddress (Engine, PortDevMap + (Handle->RBIndex * PortDevSize));
            Engine->Type.Port.Address.AddressValue = MAKE_SBDFO (
                                         Handle->Address.Address.Segment,
                                         Handle->Address.Address.Bus,
                                         Engine->Type.Port.PortData.DeviceNumber,
                                         Engine->Type.Port.PortData.FunctionNumber,
                                         0
                                         );
            if (Engine->Type.Port.IsBmcLocation && (SilData->AmdMCTPEnable)) {
              SilData->AmdMCTPMasterSeg = (uint8_t) Handle->Address.Address.Segment;
              SilData->AmdMCTPMasterID = (uint16_t)((Handle->Address.Address.Bus << 8)
                                                  | (Engine->Type.Port.PortData.DeviceNumber << 3)
                                                  | Engine->Type.Port.PortData.FunctionNumber);
            }
          }
        }
      }
      AskEntry++;
    }

    /*
	   * Sync Early Training remap setting
	   */
    Handle = GnbHandle;
    while (Handle != NULL) {
      if (Handle->SocketId == GnbHandle->SocketId) {
        Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &(Handle->Header)));
        while (Engine != NULL) {
          SyncEarlyTrainRemap (Handle, PortDevMap + (Handle->RBIndex * PortDevSize), Engine);
          Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_GNB);
        }
      }
      Handle = GnbGetNextHandle(Handle);
    }

    Handle = GnbHandle;
    while (Handle != NULL) {
      if (Handle->SocketId == GnbHandle->SocketId) {
        MpioXferTable->PcieSetPortPciAddressMap(Handle, PortDevMap + (Handle->RBIndex * PortDevSize));
      }
      Handle = GnbGetNextHandle(Handle);
    }
  }
}
