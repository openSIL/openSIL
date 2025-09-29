/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PcieComplexDefaults.c
 * @brief Common Package PCIe Configuration
 *
 */

#include <SilCommon.h>
#include <Nbio/NbioIp2Ip.h>


/**
 * PcieEngineConfigDefaults
 * @brief Initialize default PCIe engine parameters
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   PcieEngine          Pcie Engine passed in by callback
 * @param   Buffer              Nothing here, will be NULL
 * @param   Pcie                PCIe platform configuration info
 **/
void
PcieEngineConfigDefaults (
  SIL_CONTEXT          *SilContext,
  PCIe_ENGINE_CONFIG   *PcieEngine,
  void                 *Buffer,
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  PcieEngine->Type.Port.SlotPowerLimit = 75;                                            ///< Slot Power Limit
  PcieEngine->Type.Port.MaxPayloadSize = 5;                                             ///< Max_Payload_Size
  PcieEngine->Type.Port.TXDeEmphasis = 0;                                               ///< TX De-emphasis
  PcieEngine->Type.Port.TXMargin = 0;                                                   ///< TX Margin
  PcieEngine->Type.Port.EqSearchMode = 1;                                               ///< Equalization Search Mode
  PcieEngine->Type.Port.BypassGen3EQ = 0;                                               ///< BypassGen3EQ
  PcieEngine->Type.Port.DisGen3EQPhase = 0;                                             ///< Disable Gen3 EQ Phase2/3
  PcieEngine->Type.Port.Gen3FixedPreset = 0;                                            ///< Gen3 Fixed Preset default value
  PcieEngine->Type.Port.EqSearchModeGen4 = 0;                                           ///< Equalization Search Mode for Gen4
  PcieEngine->Type.Port.BypassGen4EQ = 0;                                               ///< Gen4 Bypass phase3 EQ
  PcieEngine->Type.Port.DisGen4EQPhase = 0;                                             ///< Gen4 Bypass phase2/3 EQ
  PcieEngine->Type.Port.Gen4FixedPreset = 0;                                            ///< Gen4 Fixed Preset default value
  PcieEngine->Type.Port.EqSearchModeGen5 = 0;                                           ///< Equalization Search Mode for Gen5
  PcieEngine->Type.Port.BypassGen5EQ = 0;                                               ///< Gen5 Bypass phase3 EQ
  PcieEngine->Type.Port.DisGen5EQPhase = 0;                                             ///< Gen5 Bypass phase2/3 EQ
  PcieEngine->Type.Port.Gen5FixedPreset = 0;                                            ///< Gen5 Fixed Preset default value
  PcieEngine->Type.Port.ClkReq = 0;                                                     ///< ClkReq:[0:3]
  PcieEngine->Type.Port.EqPreset = 0;                                                   ///< EqPreset:[4:7]
  PcieEngine->Type.Port.SpcMode.SpcGen1 = 0;                                            ///< SPC Mode 2P5GT
  PcieEngine->Type.Port.SpcMode.SpcGen2 = 0;                                            ///< SPC Mode 5GT
  PcieEngine->Type.Port.SpcMode.SpcGen3 = 1;                                            ///< SPC Mode 8GT
  PcieEngine->Type.Port.SpcMode.SpcGen4 = 2;                                            ///< SPC Mode 16GT
  PcieEngine->Type.Port.SpcMode.SpcGen5 = 2;                                            ///< SPC Mode 32GT
  PcieEngine->Type.Port.LaneEqualizationCntl.DsTxPreset = 7;                            ///< Gen3 Downstream Tx Preset
  PcieEngine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = 3;                        ///< Gen3 Downstream Rx Preset Hint
  PcieEngine->Type.Port.LaneEqualizationCntl.UsTxPreset = 7;                            ///< Gen3 Upstream Tx Preset
  PcieEngine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = 3;                        ///< Gen3 Upstream Rx Preset Hint
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset = 7;                        ///< Gen4 Downstream Tx Preset
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset = 5;                        ///< Gen4 Upstream Tx Preset
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.DsTxPreset = 0;                        ///< Gen5 Downstream Tx Preset
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.UsTxPreset = 0;                        ///< Gen5 Upstream Tx Preset
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.PrecodeRequest = 0;                    ///< Gen5 Precoding Request
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.AdvertiseEqToHiRate = 0;               ///< Gen5 Precoding Request
  PcieEngine->Type.Port.SrisEnableMode = 0xF;                                           ///< SRIS Mode
  PcieEngine->Type.Port.SRIS_SRNS = 0;                                                  ///< SRIS SRNS
  PcieEngine->Type.Port.SRIS_LowerSKPSupport = 0;                                       ///< SRIS Lower SKP Support
  PcieEngine->Type.Port.EsmControl = 0;                                                 ///< Esm Control Bit for this port
  PcieEngine->Type.Port.CxlControl = 0;                                                 ///< Bit to enable CXL Capability
  PcieEngine->Type.Port.LowerSkpOsGenSup = 0;                                           ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  PcieEngine->Type.Port.LowerSkpOsRcvSup = 0;                                           ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  PcieEngine->Type.Port.CxlControl = 0;                                                 ///< Bit to enable CXL Capability
  PcieEngine->Type.Port.AlwaysExpose = 0;                                               ///< Always expose unused PCIE port
  PcieEngine->Type.Port.SrisSkipInterval = 0;                                           ///< SRIS Skip Interval
  PcieEngine->Type.Port.SrisAutoDetectMode = 0xF;                                       ///< SRIS Auto Detect Mode
  PcieEngine->Type.Port.SrisSkpIntervalSel = 1;
  PcieEngine->Type.Port.SrisAutodetectFactor = 0;                                       ///< SRIS Auto Detect Factor
  PcieEngine->Type.Port.LinkAspmL1_1 = 0;                                               ///< Enable PM L1 SS L1.1
  PcieEngine->Type.Port.LinkAspmL1_2 = 0;                                               ///< Enable PM L1 SS L1.2
  PcieEngine->Type.Port.SlotPowerLimitScale = 0;                                        ///< Slot Power Limit Scale
  PcieEngine->Type.Port.RxMarginPersistence = 1;                                        ///< Rx Margin persistence mode
  PcieEngine->Type.Port.SetGen3FixedPreset = 0;                                         ///< Gen3 Fixed Preset set
  PcieEngine->Type.Port.SetGen4FixedPreset = 0;                                         ///< Gen4 Fixed Preset set
  PcieEngine->Type.Port.SetGen5FixedPreset = 0;                                         ///< Gen5 Fixed Preset set
  PcieEngine->Type.Port.TxVetting = 0;                                                  ///< Gen4 Tx Vetting
  PcieEngine->Type.Port.RxVetting = 0;                                                  ///< Gen4 Rx Vetting
  PcieEngine->Type.Port.TxVettingGen5 = 0;                                              ///< Gen5 Tx Vetting
  PcieEngine->Type.Port.RxVettingGen5 = 0;                                              ///< Gen5 Rx Vetting
  PcieEngine->Type.Port.IsMasterPLL = 0;                                                ///< IsMasterPLL
  PcieEngine->Type.Port.TargetLinkSpeed = 0;                                            ///< Target Link Speed default value
  PcieEngine->Type.Port.DlfCapDisable = 0;                                              ///< DLF Capability 1:Disable 0:Enable
  PcieEngine->Type.Port.DlfExchangeDisable = 0;                                         ///< DLF Exchange 1:Disable 0:Enable
  PcieEngine->Type.Port.InvertPolarity = 0;                                             ///< Invert RX Polarity
  PcieEngine->Type.Port.InvertPolarity2 = 0;                                            ///< Invert TX Polarity
  PcieEngine->Type.Port.EsmSpeedBump = 16;                                              ///< ESM Gen4 speed (in GT/s)
  PcieEngine->Type.Port.PsppPolicyDC = 0;                                               ///< Pspp Policy DC
  PcieEngine->Type.Port.PsppPolicyAC = 0;                                               ///< Pspp Policy AC
  PcieEngine->Type.Port.PsppDeviceType = 0;                                             ///< Pspp Device Type
  PcieEngine->Type.Port.PortCapabilities.LinkSpeed = 0;
  PcieEngine->Type.Port.PortCapabilities.MaxPayloadSupport = 0x2;
  PcieEngine->Type.Port.PortCapabilities.AspmCapability = 3;                            ///< ASPM Capability default
  PcieEngine->Type.Port.PortCapabilities.PciPmL1_1 = 0;
  PcieEngine->Type.Port.PortCapabilities.PciPmL1_2 = 0;
  PcieEngine->Type.Port.PortCapabilities.AspmL1_1 = 0;
  PcieEngine->Type.Port.PortCapabilities.AspmL1_2 = 0;
  PcieEngine->Type.Port.PortCapabilities.EsmSupport = 0;
  PcieEngine->Type.Port.PortCapabilities.LtrSupport = 0;
  PcieEngine->Type.Port.PortCapabilities.SurpriseDownErrorReport = 0;
  PcieEngine->Type.Port.PortCapabilities.TenBitTagSupport = 3;                          ///< TenBitTagSupport Capability default
  PcieEngine->Type.Port.PortCapabilities.AriForwarding = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsSupport = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsSourceValidation = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsTranslationBlocking = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsP2pRequestRedirect = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsP2pCompletionRedirect = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsUpstreamForwarding = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsP2pEgressControl = 0;
  PcieEngine->Type.Port.PortCapabilities.AcsDirectTranslatedP2p = 0;
  PcieEngine->Type.Port.PortCapabilities.LaneMargining = 0;
  PcieEngine->Type.Port.PortCapabilities.DataLinkFeature = 0;
  PcieEngine->Type.Port.PortCapabilities.DownstreamPortContainment = 1;                ///< Downstream Port Containment default
  PcieEngine->Type.Port.PortCapabilities.AdvancedErrorReporting = 0;
  PcieEngine->Type.Port.PortCapabilities.ECRCSupport = 0;
  PcieEngine->Type.Port.PortCapabilities.MulticastEnable = 0;
  PcieEngine->Type.Port.PortCapabilities.NativePCIeEnclosureManagement = 0;             ///< NPEM Capability default
  PcieEngine->Type.Port.PortCapabilities.Capability1Address = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability1Data = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability2Address = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability2Data = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability3Address = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability3Data = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability4Address = 0;
  PcieEngine->Type.Port.PortCapabilities.Capability4Data = 0;
  PcieEngine->Type.Port.PortFeatures.LinkSpeedControl = 0;
  PcieEngine->Type.Port.PortFeatures.MaxPayloadSizeControl = 0;
  PcieEngine->Type.Port.PortFeatures.ESMControl = 0;
  PcieEngine->Type.Port.PortFeatures.LTRControl = 0;
  PcieEngine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl = 0;                ///< DataLinkFeatureExchangeControl default
  PcieEngine->Type.Port.PortFeatures.TenBitTagControl = 0;
  PcieEngine->Type.Port.PortFeatures.ARIControl = 0;
  PcieEngine->Type.Port.PortFeatures.ACSControl = 0;
  PcieEngine->Type.Port.PortFeatures.RxLaneMarginingControl = 0;
  PcieEngine->Type.Port.PortFeatures.DynLanesPwrState = 0;
  PcieEngine->Type.Port.PortFeatures.L1PowerDown = 0;
  PcieEngine->Type.Port.PortFeatures.L11PowerDown = 0;
  PcieEngine->Type.Port.PortFeatures.L12PowerDown = 0;
  PcieEngine->Type.Port.PortFeatures.AutoSpdChngEn = 0;
  PcieEngine->Type.Port.PortFeatures.TurnOffUnusedLanes = 0;
  PcieEngine->Type.Port.LaneEqualizationCntl.LcFapeEnable8GT = 0;
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.LcFapeEnable16GT = 0;
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.LcFapeEnable32GT = 0;
  PcieEngine->Type.Port.ForceSteering = 0;
}

/**
 * PcieWrapperConfigDefaults
 * @brief Initialize default PCIe wrapper parameters
 *
 * @param SilContext   A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 * @param Wrapper      Pcie wrapper passed in by callback
 * @param Buffer       Nothing here, will be NULL
 * @param Pcie         PCIe platform configuration info
 **/
SIL_STATUS
PcieWrapperConfigDefaults (
  SIL_CONTEXT           *SilContext,
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  Wrapper->Features.PowerOffUnusedLanes = 1;
  Wrapper->Features.PowerOffUnusedPlls = 1;
  Wrapper->Features.ClkGating = 1;
  Wrapper->Features.LclkGating = 1;
  Wrapper->Features.TxclkGatingPllPowerDown = 1;
  Wrapper->Features.PllOffInL1 = 1;
  Wrapper->Features.AccessEncoding = 1;
  Wrapper->Features.CoreReversed = 0;

  return SilPass;
}
