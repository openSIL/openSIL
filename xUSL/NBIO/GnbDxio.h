/**
 * @file  GnbDxio.h
 * @brief This file contains PCIe component definitions
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#pragma pack (push, 1)

#include "PciStructs.h"
#include <stddef.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_COMPLEXES                16

#define DESCRIPTOR_TERMINATE_GNB               0x40000000ull
#define DESCRIPTOR_TERMINATE_TOPOLOGY          0x20000000ull
#define DESCRIPTOR_ALLOCATED                   0x10000000ull
#define DESCRIPTOR_PLATFORM                    0x08000000ull
#define DESCRIPTOR_COMPLEX                     0x04000000ull
#define DESCRIPTOR_SILICON                     0x02000000ull
#define DESCRIPTOR_PCIE_WRAPPER                0x01000000ull
#define DESCRIPTOR_PCIE_ENGINE                 0x00800000ull
#define DESCRIPTOR_CXL_ENGINE                  0x00200000ull

#define DESCRIPTOR_ALL_WRAPPERS                (DESCRIPTOR_PCIE_WRAPPER)
#define DESCRIPTOR_ALL_ENGINES                 (DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_CXL_ENGINE)

#define DESCRIPTOR_ALL_TYPES                   (DESCRIPTOR_ALL_WRAPPERS | DESCRIPTOR_ALL_ENGINES | DESCRIPTOR_SILICON | DESCRIPTOR_PLATFORM)

#define UNUSED_LANE_ID                         255

#define SILICON_NOT_PRESENT_IOHC               0x00000001ull
#define SILICON_NOT_PRESENT_SYSHUB             0x00000002ull
#define SILICON_NOT_PRESENT_NBIF0              0x00000004ull
#define SILICON_NOT_PRESENT_NBIF1              0x00000008ull
#define SILICON_NOT_PRESENT_NBIF2              0x00000010ull
#define SILICON_NOT_PRESENT_IOMMU              0x00000020ull
#define SILICON_NOT_PRESENT_L1IOAGR            0x00000040ull
#define SILICON_NOT_PRESENT_L1PCIE0            0x00000080ull
#define SILICON_NOT_PRESENT_L1PCIE1            0x00000100ull
#define SILICON_NOT_PRESENT_L1NBIF0            0x00000200ull
#define SILICON_NOT_PRESENT_NB                 0x00000400ull
#define SILICON_NOT_PRESENT_PCIE0              0x00000800ull
#define SILICON_NOT_PRESENT_PCIE1              0x00001000ull
#define SILICON_NOT_PRESENT_PCIE2              0x00002000ull
#define SILICON_NOT_PRESENT_SDPMUX             0x00004000ull
#define SILICON_NOT_PRESENT_SST                0x00008000ull


/// PCIe Link Training State
typedef enum {
  LinkStateResetAssert,                                 ///< Assert port GPIO reset
  LinkStateResetDuration,                               ///< Timeout for reset duration
  LinkStateResetExit,                                   ///< Deassert port GPIO reset
  LinkTrainingResetTimeout,                             ///< Port GPIO reset timeout
  LinkStateReleaseTraining,                             ///< Release link training
  LinkStateDetectPresence,                              ///< Detect device presence
  LinkStateDetecting,                                   ///< Detect link training.
  LinkStateBrokenLane,                                  ///< Check and handle broken lane
  LinkStateGen2Fail,                                    ///< Check and handle device that fail training if GEN2 capability advertised
  LinkStateL0,                                          ///< Device trained to L0
  LinkStateVcoNegotiation,                              ///< Check VCO negotiation complete
  LinkStateRetrain,                                     ///< Force retrain link.
  LinkStateTrainingFail,                                ///< Link training fail
  LinkStateTrainingSuccess,                             ///< Link training success
  LinkStateGfxWorkaround,                               ///< GFX workaround
  LinkStateCompliance,                                  ///< Link in compliance mode
  LinkStateDeviceNotPresent,                            ///< Link is not connected
  LinkStateTrainingCompleted                            ///< Link training completed
} PCIE_LINK_TRAINING_STATE;

/// PCIe PSPP Power policy
typedef enum  {
  SilPsppDisabled,                                           ///< PSPP disabled
  SilPsppPerformance = 1,                                    ///< Performance
  SilPsppBalance,                                            ///< Balance
  SilPsppPowerSaving,                                        ///< Power Saving
  SilMaxPspp                                                 ///< Max Pspp for boundary check
} PCIE_PSPP_POLICY;


#define INIT_STATUS_PCIE_PORT_GEN2_RECOVERY          0x00000001ull
#define INIT_STATUS_PCIE_PORT_BROKEN_LANE_RECOVERY   0x00000002ull
#define INIT_STATUS_PCIE_PORT_TRAINING_FAIL          0x00000004ull
#define INIT_STATUS_PCIE_TRAINING_SUCCESS            0x00000008ull
#define INIT_STATUS_PCIE_EP_NOT_PRESENT              0x00000010ull
#define INIT_STATUS_PCIE_PORT_IN_COMPLIANCE          0x00000020ull
#define INIT_STATUS_ALLOCATED                        0x00000080ull
#define INIT_STATUS_PCIE_PORT_BROKEN_LANE_X1         0x00000100ull
#define INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE          0x00000200ull

#define PcieLibIsEngineAllocated(Descriptor) ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0) : false)
// Check if descriptor a PCIe engine
#define PcieLibIsPcieEngine(Descriptor) ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_PCIE_ENGINE) != 0) : false)
// Check if descriptor a PCIe wrapper
#define PcieLibGetNextDescriptor(Descriptor) ((Descriptor == NULL) ? NULL : ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (Descriptor + 1))

// PCIe functions to avoid hardcoding these values where they are used
#define PCIE_FUNCTION0  0
#define PCIE_FUNCTION1  1
#define PCIE_FUNCTION2  2
#define PCIE_FUNCTION3  3
#define PCIE_FUNCTION4  4
#define PCIE_FUNCTION5  5
#define PCIE_FUNCTION6  6
#define PCIE_FUNCTION7  7

/// PCIe port State
typedef enum {
  UnhidePorts,                                          ///< Command to unhide port
  HidePorts,                                            ///< Command to hide unused ports
} PCIE_PORT_VISIBILITY;

/// Table Register Entry
typedef struct {
  uint16_t          Reg;                                  ///< Address
  uint32_t          Mask;                                 ///< Mask
  uint32_t          Data;                                 ///< Data
} PCIE_PORT_REGISTER_ENTRY;

/// Table Register Entry
typedef struct {
  PCIE_PORT_REGISTER_ENTRY  *Table;                     ///< Table
  uint32_t                    Length;                     ///< Length
} PCIE_PORT_REGISTER_TABLE_HEADER;

/// Table Register Entry
typedef struct {
  uint32_t          Reg;                                  ///< Address
  uint32_t          Mask;                                 ///< Mask
  uint32_t          Data;                                 ///< Data
} PCIE_HOST_REGISTER_ENTRY;

/// Table Register Entry
typedef struct {
  PCIE_HOST_REGISTER_ENTRY  *Table;                     ///< Table
  uint32_t                    Length;                     ///< Length
} PCIE_HOST_REGISTER_TABLE_HEADER;

///Link ASPM info
typedef struct {
  PCI_ADDR        DownstreamPort;                       ///< PCI address of downstream port
  PCIE_ASPM_TYPE  DownstreamAspm;                       ///< Downstream Device Aspm
  PCI_ADDR        UpstreamPort;                         ///< PCI address of upstream port
  PCIE_ASPM_TYPE  UpstreamAspm;                         ///< Upstream Device Capability
  PCIE_ASPM_TYPE  RequestedAspm;                        ///< Requested ASPM
  bool            BlockList;                            ///< Blocklist device
} PCIe_LINK_ASPM;

///PCIe ASPM Latency Information
typedef struct {
  uint8_t  MaxL0sExitLatency;                             ///< Max L0s exit latency in us
  uint8_t  MaxL1ExitLatency;                              ///< Max L1 exit latency in us
} PCIe_ASPM_LATENCY_INFO;

/// PCI address association
typedef struct {
  uint8_t NewDeviceAddress;                                ///< New PCI address (Device,Fucntion)
  uint8_t NativeDeviceAddress;                             ///< Native PCI address (Device,Fucntion)
} PCI_ADDR_LIST;

typedef uint16_t PCIe_ENGINE_INIT_STATUS;

/// Engine Configuration
typedef struct {
         uint8_t                EngineType;               /**< Engine type
                                                           *  @li @b 0 -  Ignore engine configuration
                                                           *  @li @b 1 -  PCIe port
                                                           */
         uint16_t               StartLane;                /**< Start Lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */
         uint16_t               EndLane;                  /**< End lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */

} PCIe_ENGINE;

/// PCIe port misc extended controls
typedef struct  {
        uint8_t                     SbLink             :1;  /**< PCIe link type
                                                             *  @li @b 0 - General purpose port
                                                             *  @li @b 1 - Port connected to SB
                                                             */
        uint8_t                     ClkPmSupport       :1;  /**< Clock Power Management Support
                                                             *  @li @b 0 - Clock Power Management not configured
                                                             *  @li @b 1 - Clock Power Management configured according to PCIe device capability
                                                             */
        uint8_t                     CsLink             :1;  /**< PCIe link type
                                                             *  @li @b 0 - General purpose port
                                                             *  @li @b 1 - Port connected to chipset
                                                             */
        uint8_t                     Reserved0          :5;  ///< Unused space
} PORT_MISC_CONTROL;



/// PCIe port configuration data
typedef struct  {
         uint8_t                   PortPresent;              ///< Enable PCIe port for initialization.
         uint8_t                   FunctionNumber       :3;  ///< Reserved for future use
         uint8_t                   DeviceNumber         :5;   /**< PCI Device number for port.
                                                               *   @li @b 0 - Native port device number
                                                               *   @li @b N - Port device number (See available configurations in BKDG
                                                               */
         uint8_t                   LinkSpeedCapability  :4;   /**< PCIe link speed/
                                                               *  @li @b 0 - Maximum supported by silicon
                                                               *  @li @b 1 - Gen1
                                                               *  @li @b 2 - Gen2
                                                               *  @li @b 3 - Gen3
                                                               */
         uint8_t                   LinkAspm             :4;   /**< ASPM control. (see SilPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - L0s only
                                                               *  @li @b 2 - L1 only
                                                               *  @li @b 3 - L0s and L1
                                                               */
         uint8_t                   LinkHotplug;               /**< Hotplug control.
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Basic
                                                               *  @li @b 2 - Server
                                                               *  @li @b 3 - Enhanced
                                                               */
         uint16_t                  SlotNum;                  ///< Physical Slot Number
         PORT_MISC_CONTROL       MiscControls;             ///< Misc extended controls
         uint8_t                   Reserved1;                ///< Reserved for future use
} PORT_DATA;

typedef struct {
    uint8_t                LinkSpeed;
    uint8_t                MaxPayloadSupport;
    uint8_t                AspmCapability;
    uint8_t                PciPmL1_1;
    uint8_t                PciPmL1_2;
    uint8_t                AspmL1_1;
    uint8_t                AspmL1_2;
    uint8_t                EsmSupport;
    uint8_t                LtrSupport;
    uint8_t                SurpriseDownErrorReport;
    uint8_t                TenBitTagSupport;
    uint8_t                AriForwarding;
    uint8_t                AcsSupport;
    uint8_t                AcsSourceValidation;
    uint8_t                AcsTranslationBlocking;
    uint8_t                AcsP2pRequestRedirect;
    uint8_t                AcsP2pCompletionRedirect;
    uint8_t                AcsUpstreamForwarding;
    uint8_t                AcsP2pEgressControl;
    uint8_t                AcsDirectTranslatedP2p;
    uint8_t                LaneMargining;
    uint8_t                DataLinkFeature;
    uint8_t                DownstreamPortContainment;
    uint8_t                AdvancedErrorReporting;
    uint8_t                ECRCSupport;
    uint8_t                MulticastEnable;
    uint8_t                NativePCIeEnclosureManagement;
    uint8_t                Capability1Address;
    uint8_t                Capability1Data;
    uint8_t                Capability2Address;
    uint8_t                Capability2Data;
    uint8_t                Capability3Address;
    uint8_t                Capability3Data;
    uint8_t                Capability4Address;
    uint8_t                Capability4Data;
} PORT_CAPABILITIES;

typedef struct {
    uint8_t                LinkSpeedControl;
    uint8_t                MaxPayloadSizeControl;
    uint8_t                ESMControl;
    uint8_t                LTRControl;
    uint8_t                DataLinkFeatureExchangeControl;
    uint8_t                TenBitTagControl;
    uint8_t                ARIControl;
    uint8_t                ACSControl;
    uint8_t                RxLaneMarginingControl;
    uint8_t                DynLanesPwrState;
    uint8_t                L1PowerDown;
    uint8_t                L11PowerDown;
    uint8_t                L12PowerDown;
    uint8_t                AutoSpdChngEn;
    uint8_t                TurnOffUnusedLanes;
} PORT_FEATURES;

/// PCIe port configuration info
typedef struct {
  PORT_DATA               PortData;                     ///< Port data
  uint8_t                   StartCoreLane;                ///< Start Core Lane
  uint8_t                   EndCoreLane;                  ///< End Core lane
  uint8_t                   NativeDevNumber :5;           ///< Native PCI device number of the port
  uint8_t                   NativeFunNumber :3;           ///< Native PCI function number of the port
  uint8_t                   CoreId :4;                    ///< PCIe core ID
  uint8_t                   PortId :4;                    ///< Port ID on wrapper
  PCI_ADDR                Address;                      ///< PCI address of the port
  uint8_t                   PcieBridgeId:7;               ///< IOC PCIe bridge ID
  uint8_t                   IsBmcLocation:1;              ///< Port Location of BMC
  uint8_t                   LogicalBridgeId;              ///< Logical Bridge ID
  uint8_t                   SlotPowerLimit;               ///< Slot Power Limit
  uint8_t                   MaxPayloadSize;               ///< Max_Payload_Size

  uint8_t                   TXDeEmphasis:4;               ///< TX De-emphasis
  uint8_t                   TXMargin:3;                   ///< TX Margin
  uint8_t                   UNUSED1:1;                    ///< Currently unassigned - for alignment

  uint8_t                   EqSearchMode:2;               ///< Equalization Search Mode
  uint8_t                   BypassGen3EQ:1;               ///< BypassGen3EQ
  uint8_t                   DisGen3EQPhase:1;             ///< Disable Gen3 EQ Phase2/3
  uint8_t                   Gen3FixedPreset:4;            ///< Gen3 Fixed Preset value

  uint8_t                   EqSearchModeGen4:2;           ///< Equalization Search Mode for Gen4
  uint8_t                   BypassGen4EQ:1;               ///< Gen4 Bypass phase3 EQ
  uint8_t                   DisGen4EQPhase:1;             ///< Gen4 Bypass phase2/3 EQ
  uint8_t                   Gen4FixedPreset:4;            ///< Gen4 Fixed Preset value
  uint8_t                   EqSearchModeGen5:2;           ///< Equalization Search Mode for Gen5
  uint8_t                   BypassGen5EQ:1;               ///< Gen5 Bypass phase3 EQ
  uint8_t                   DisGen5EQPhase:1;             ///< Gen5 Bypass phase2/3 EQ
  uint8_t                   Gen5FixedPreset:4;            ///< Gen5 Fixed Preset value

  uint8_t                   ClkReq:4;                     ///< ClkReq:[0:3]
  uint8_t                   EqPreset:4;                   ///< EqPreset:[4:7]

  struct {
    uint8_t                 SpcGen1:1;                    ///< SPC Mode 2P5GT
    uint8_t                 SpcGen2:1;                    ///< SPC Mode 5GT
    uint8_t                 SpcGen3:2;                    ///< SPC Mode 8GT
    uint8_t                 SpcGen4:2;                    ///< SPC Mode 16GT
    uint8_t                 SpcGen5:2;                    ///< SPC Mode 32GT
  } SpcMode;
  struct {
    uint32_t                DsTxPreset:4;                 ///< Gen3 Downstream Tx Preset
    uint32_t                DsRxPresetHint:3;             ///< Gen3 Downstream Rx Preset Hint
    uint32_t                UsTxPreset:4;                 ///< Gen3 Upstream Tx Preset
    uint32_t                UsRxPresetHint:3;             ///< Gen3 Upstream Rx Preset Hint
    uint32_t                LcPresetMask8Gt:10;           ///< Gen3 Preset Mask
    uint32_t                LcFapeEnable8GT:1;            ///< Gen3 FapeEnable
    uint32_t                UNUSED2:7;                    ///< Currently unassigned - for alignment
  } LaneEqualizationCntl;
  struct {
    uint32_t                 DsTxPreset:4;                 ///< Gen4 Downstream Tx Preset
    uint32_t                 UsTxPreset:4;                 ///< Gen4 Upstream Tx Preset
    uint32_t                 LcPresetMask16Gt:10;          ///< Gen4 Preset Mask
    uint32_t                 LcFapeEnable16GT:1;           ///< Gen4 FapeEnable
    uint32_t                 UNUSED3:13;                   ///< Currently unassigned - for alignment
  } Gen4LaneEqualizationCntl;
  struct {
    uint32_t                DsTxPreset:4;                 ///< Gen5 Downstream Tx Preset
    uint32_t                UsTxPreset:4;                 ///< Gen5 Upstream Tx Preset
    uint32_t                LcPresetMask32Gt:10;          ///< Gen5 Preset Mask
    uint32_t                LcFapeEnable32GT:1;           ///< Gen4 FapeEnable
    uint32_t                PrecodeRequest:1;             ///< Precoding Request
    uint32_t                AdvertiseEqToHiRate:1;        ///< Advertise EQ To High Rate Support
    uint32_t                UNUSED4:11;                   ///< Currently unassigned - for alignment
  } Gen5LaneEqualizationCntl;
  uint8_t                   SrisEnableMode:4;             ///< 0:Disable 1:SRIS 2:SRNS 3:SRNS in SRIS 0xF:Auto
  uint8_t                   SRIS_SRNS:1;                  ///< SRIS SRNS
  uint8_t                   SRIS_LowerSKPSupport:1;       ///< SRIS Lower SKP Support
  uint8_t                   EsmControl:1;                 ///< Bit to enable/disable ESM
  uint8_t                   LowerSkpOsGenSup;             ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  uint8_t                   LowerSkpOsRcvSup;             ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  uint8_t                   CxlControl:1;                 ///< Bit to enable CXL Capability
  uint8_t                   AlwaysExpose:1;               ///< Always expose unused PCIE port
  uint8_t                   SrisSkipInterval:2;           ///< Controls SRIS SKP generation interval
  uint8_t                   SrisAutoDetectMode:4;         ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  uint8_t                   SrisSkpIntervalSel:2;         ///< Controls SRIS SKIP Interval Selection Mode
  uint8_t                   SrisAutodetectFactor:2;       ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  uint8_t                   LinkAspmL1_1:1;               ///< Enable PM L1 SS L1.1
  uint8_t                   LinkAspmL1_2:1;               ///< Enable PM L1 SS L1.2
  uint8_t                   SlotPowerLimitScale:2;        ///< Slot Power Limit Scale

  uint8_t                   RxMarginPersistence:1;        ///< Bit to enable/disable Rx Margin persistence mode
  uint8_t                   SetGen3FixedPreset:1;         ///< Gen3 Fixed Preset Set
  uint8_t                   SetGen4FixedPreset:1;         ///< Gen4 Fixed Preset Set
  uint8_t                   SetGen5FixedPreset:1;         ///< Gen5 Fixed Preset Set
  uint8_t                   TxVetting:1;                  ///< Gen4 Tx Vetting
  uint8_t                   RxVetting:1;                  ///< Gen4 Rx Vetting
  uint8_t                   TxVettingGen5:1;              ///< Gen5 Tx Vetting
  uint8_t                   RxVettingGen5:1;              ///< Gen5 Rx Vetting

  uint8_t                   IsMasterPLL:1;                ///< IsMasterPLL
  uint8_t                   TargetLinkSpeed:3;            ///< Target Link Speed
  uint8_t                   DlfCapDisable:1;              ///< DLF Capability 1:Disable 0:Enable
  uint8_t                   DlfExchangeDisable:1;         ///< DLF Exchange 1:Disable 0:Enable
  uint8_t                   InvertPolarity:1;             ///< Invert RX Polarity
  uint8_t                   InvertPolarity2:1;            ///< Invert TX Polarity

  uint8_t                   EsmSpeedBump;                 ///< Speed bump for ESM
  uint16_t                  PsppPolicyDC;                 ///< Pspp Policy DC
  uint16_t                  PsppPolicyAC;                 ///< Pspp Policy AC
  uint8_t                   PsppDeviceType;               ///< Pspp Device Type
  uint8_t                   I2CMuxInfo;                   ///< First I2c Mux on Bus
  PORT_CAPABILITIES         PortCapabilities;             ///< Port Capabilities
  PORT_FEATURES             PortFeatures;                 ///< Port Features

  struct {
    uint32_t                 LcFapeReqPostCursor0:5;      ///< PostCursor0
    uint32_t                 LcFapeReqPreCursor0:4;       ///< PreCursor0
    uint32_t                 LcFapeReqPostCursor1:5;      ///< PostCursor1
    uint32_t                 LcFapeReqPreCursor1:4;       ///< PreCursor1
    uint32_t                 LcFapeReqPostCursor2:5;      ///< PostCursor2
    uint32_t                 LcFapeReqPreCursor2:4;       ///< PreCursor2
    uint32_t                 UNUSED5:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup0;

  struct {
    uint32_t                 LcFapeReqPostCursor3:5;      ///< PostCursor3
    uint32_t                 LcFapeReqPreCursor3:4;       ///< PreCursor3
    uint32_t                 LcFapeReqPostCursor4:5;      ///< PostCursor4
    uint32_t                 LcFapeReqPreCursor4:4;       ///< PreCursor4
    uint32_t                 LcFapeReqPostCursor5:5;      ///< PostCursor5
    uint32_t                 LcFapeReqPreCursor5:4;       ///< PreCursor5
    uint32_t                 UNUSED6:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup1;

  struct {
    uint32_t                 LcFapeReqPostCursor6:5;      ///< PostCursor6
    uint32_t                 LcFapeReqPreCursor6:4;       ///< PreCursor6
    uint32_t                 LcFapeReqPostCursor7:5;      ///< PostCursor7
    uint32_t                 LcFapeReqPreCursor7:4;       ///< PreCursor7
    uint32_t                 LcFapeReqPostCursor8:5;      ///< PostCursor8
    uint32_t                 LcFapeReqPreCursor8:4;       ///< PreCursor8
    uint32_t                 UNUSED7:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup2;

  struct {
    uint32_t                 LcFapeReqPostCursor9:5;      ///< PostCursor9
    uint32_t                 LcFapeReqPreCursor9:4;       ///< PreCursor9
    uint32_t                 LcFapeReqPostCursor10:5;     ///< PostCursor10
    uint32_t                 LcFapeReqPreCursor10:4;      ///< PreCursor10
    uint32_t                 LcFapeReqPostCursor11:5;     ///< PostCursor11
    uint32_t                 LcFapeReqPreCursor11:4;      ///< PreCursor11
    uint32_t                 UNUSED8:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup3;

  struct {
    uint32_t                 LcFapeReqPostCursor12:5;     ///< PostCursor12
    uint32_t                 LcFapeReqPreCursor12:4;      ///< PreCursor12
    uint32_t                 LcFapeReqPostCursor13:5;     ///< PostCursor13
    uint32_t                 LcFapeReqPreCursor13:4;      ///< PreCursor13
    uint32_t                 LcFapeReqPostCursor14:5;     ///< PostCursor14
    uint32_t                 LcFapeReqPreCursor14:4;      ///< PreCursor14
    uint32_t                 UNUSED9:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup4;

  struct {
    uint32_t                 LcFapeReqPostCursor15:5;     ///< PostCursor15
    uint32_t                 LcFapeReqPreCursor15:4;      ///< PreCursor15
    uint32_t                 LcFapeReqPostCursor16:5;     ///< PostCursor16
    uint32_t                 LcFapeReqPreCursor16:4;      ///< PreCursor16
    uint32_t                 LcFapeReqPostCursor17:5;     ///< PostCursor17
    uint32_t                 LcFapeReqPreCursor17:4;      ///< PreCursor17
    uint32_t                 UNUSED10:5;                   ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup5;

  struct {
    uint32_t                 LcFapeReqPostCursor18:5;     ///< PostCursor18
    uint32_t                 LcFapeReqPreCursor18:4;      ///< PreCursor18
    uint32_t                 LcFapeReqPostCursor19:5;     ///< PostCursor19
    uint32_t                 LcFapeReqPreCursor19:4;      ///< PreCursor19
    uint32_t                 LcFapeReqPostCursor20:5;     ///< PostCursor20
    uint32_t                 LcFapeReqPreCursor20:4;      ///< PreCursor20
    uint32_t                 UNUSED11:5;                  ///< Currently unassigned - for alignment
  } LcFapeSettingsGroup6;
  uint8_t                   ForceSteering:1;              ///< Steering is forced
  uint8_t                   UNUSED12:7;                   ///< Currently unassigned - for alignment

} PCIe_PORT_CONFIG;


/// CXL port configuration info
typedef struct {
  PORT_DATA               PortData;                     ///< Port data
  uint8_t                   StartCoreLane;                ///< Start Core Lane
  uint8_t                   EndCoreLane;                  ///< End Core lane
  uint8_t                   NativeDevNumber :5;           ///< Native PCI device number of the port
  uint8_t                   NativeFunNumber :3;           ///< Native PCI function number of the port
  uint8_t                   CoreId :4;                    ///< PCIe core ID
  uint8_t                   PortId :4;                    ///< Port ID on wrapper
  PCI_ADDR                Address;                      ///< PCI address of the port
  uint8_t                   PcieBridgeId:7;               ///< IOC PCIe bridge ID
  uint8_t                   UNUSED0:1;                    ///< Currently unassigned - for alignment
  uint8_t                   LogicalBridgeId;              ///< Logical Bridge ID
  uint8_t                   SlotPowerLimit;               ///< Slot Power Limit
  uint8_t                   MaxPayloadSize;               ///< Max_Payload_Size

  uint8_t                   UnusedTXDeEmphasis:4;         ///< TX De-emphasis
  uint8_t                   UnusedTXMargin:3;             ///< TX Margin
  uint8_t                   UnusedUNUSED1:1;              ///< Currently unassigned - for alignment

  uint8_t                   UnusedEqSearchMode:2;         ///< Equalization Search Mode
  uint8_t                   UnusedBypassGen3EQ:1;         ///< BypassGen3EQ
  uint8_t                   UnusedDisGen3EQPhase:1;       ///< Disable Gen3 EQ Phase2/3
  uint8_t                   UnusedGen3FixedPreset:4;      ///< Gen3 Fixed Preset value

  uint8_t                   UnusedEqSearchModeGen4:2;     ///< Equalization Search Mode for Gen4
  uint8_t                   UnusedBypassGen4EQ:1;         ///< Gen4 Bypass phase3 EQ
  uint8_t                   UnusedDisGen4EQPhase:1;       ///< Gen4 Bypass phase2/3 EQ
  uint8_t                   UnusedGen4FixedPreset:4;      ///< Gen4 Fixed Preset value
  uint8_t                   UnusedEqSearchModeGen5:2;     ///< Equalization Search Mode for Gen5
  uint8_t                   UnusedBypassGen5EQ:1;         ///< Gen5 Bypass phase3 EQ
  uint8_t                   UnusedDisGen5EQPhase:1;       ///< Gen5 Bypass phase2/3 EQ
  uint8_t                   UnusedGen5FixedPreset:4;      ///< Gen5 Fixed Preset value

  uint8_t                   CxlIndex;

  struct {
    uint8_t                 SpcGen1:1;                    ///< SPC Mode 2P5GT
    uint8_t                 SpcGen2:1;                    ///< SPC Mode 5GT
    uint8_t                 SpcGen3:2;                    ///< SPC Mode 8GT
    uint8_t                 SpcGen4:2;                    ///< SPC Mode 16GT
    uint8_t                 SpcGen5:2;                    ///< SPC Mode 32GT
  } SpcMode;
  struct {
    uint16_t                UnusedDsTxPreset:4;           ///< Gen3 Downstream Tx Preset
    uint16_t                UnusedDsRxPresetHint:3;       ///< Gen3 Downstream Rx Preset Hint
    uint16_t                UnusedUsTxPreset:4;           ///< Gen3 Upstream Tx Preset
    uint16_t                UnusedUsRxPresetHint:3;       ///< Gen3 Upstream Rx Preset Hint
  } LaneEqualizationCntl;
  struct {
    uint8_t                 UnusedDsTxPreset:4;           ///< Gen4 Downstream Tx Preset
    uint8_t                 UnusedUsTxPreset:4;           ///< Gen4 Upstream Tx Preset
  } Gen4LaneEqualizationCntl;
  struct {
    uint8_t                 UnusedDsTxPreset:4;           ///< Gen5 Downstream Tx Preset
    uint8_t                 UnusedUsTxPreset:4;           ///< Gen5 Upstream Tx Preset
  } Gen5LaneEqualizationCntl;
  uint8_t                   SrisEnableMode:4;             ///< 0:Disable 1:SRIS 2:SRNS 3:SRNS in SRIS 0xF:Auto
  uint8_t                   SRIS_SRNS:1;                  ///< SRIS SRNS
  uint8_t                   SRIS_LowerSKPSupport:1;       ///< SRIS Lower SKP Support
  uint8_t                   EsmControl:1;                 ///< Bit to enable/disable ESM
  uint8_t                   LowerSkpOsGenSup;             ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  uint8_t                   LowerSkpOsRcvSup;             ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  uint8_t                   UNUSED3:2;                    ///< Currently Unassigned for alignment
  uint8_t                   SrisSkipInterval:2;           ///< Controls SRIS SKP generation interval
  uint8_t                   SrisAutoDetectMode:4;         ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  uint8_t                   SrisSkpIntervalSel:2;         ///< Controls SRIS SKIP Interval Selection Mode
  uint8_t                   SrisAutodetectFactor:2;       ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  uint8_t                   LinkAspmL1_1:1;               ///< Enable PM L1 SS L1.1
  uint8_t                   LinkAspmL1_2:1;               ///< Enable PM L1 SS L1.2
  uint8_t                   SlotPowerLimitScale:2;        ///< Slot Power Limit Scale

  uint8_t                   RxMarginPersistence:1;        ///< Bit to enable/disable Rx Margin persistence mode
  uint8_t                   SetGen3FixedPreset:1;         ///< Gen3 Fixed Preset Set
  uint8_t                   SetGen4FixedPreset:1;         ///< Gen4 Fixed Preset Set
  uint8_t                   SetGen5FixedPreset:1;         ///< Gen5 Fixed Preset Set
  uint8_t                   TxVetting:1;                  ///< Gen4 Tx Vetting
  uint8_t                   RxVetting:1;                  ///< Gen4 Rx Vetting
  uint8_t                   TxVettingGen5:1;              ///< Gen5 Tx Vetting
  uint8_t                   RxVettingGen5:1;              ///< Gen5 Rx Vetting

  uint8_t                   IsMasterPLL:1;                ///< IsMasterPLL
  uint8_t                   TargetLinkSpeed:3;            ///< Target Link Speed
  uint8_t                   DlfCapDisable:1;              ///< DLF Capability 1:Disable 0:Enable
  uint8_t                   DlfExchangeDisable:1;         ///< DLF Exchange 1:Disable 0:Enable
  uint8_t                   CxlDeviceType:2;              ///< Type of CXL device connected

  uint8_t                   EsmSpeedBump;                 ///< Speed bump for ESM
  uint32_t                  UsRcrb;                       ///< Upstream Port RCRB address
  uint32_t                  DsRcrb;                       ///< Downstream Port RCRB address
  uint32_t                  UsMemBar0;                    ///< Upstream port MEMBAR0
  uint32_t                  DsMemBar0;                    ///< Downstream port MEMBAR0
  uint32_t                  Mmio32Base;
  uint32_t                  Mmio32Size;
  uint64_t                  Mmio64Base;
  uint64_t                  Mmio64Size;
  uint32_t                  Mmio32Gran;
} PCIe_CXL_CONFIG;


///Descriptor header
typedef struct {
  uint32_t                  DescriptorFlags;              ///< Descriptor flags
  uint16_t                  Parent;                       ///< Offset of parent descriptor
  uint16_t                  Peer;                         ///< Offset of the peer descriptor
  uint16_t                  Child;                        ///< Offset of the list of child descriptors
} PCIe_DESCRIPTOR_HEADER;

/// Engine configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor header
  PCIe_ENGINE             EngineData;                   ///< Engine Data
  PCIe_ENGINE_INIT_STATUS InitStatus;                   ///< Initialization Status
  uint8_t                   Scratch;                      ///< Scratch pad
  union {
    PCIe_PORT_CONFIG      Port;                         ///< PCIe port configuration data
    PCIe_CXL_CONFIG       Cxl;                          ///< CXL Configuration data
  } Type;
} PCIe_ENGINE_CONFIG;

/// Wrapper configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  uint8_t                   WrapId;                       ///< Wrapper ID
  uint8_t                   CxlCoreConfig;                ///< Cxl CORE Configuration
  uint8_t                   StartPhyLane;                 ///< Start PHY Lane
  uint8_t                   EndPhyLane;                   ///< End PHY Lane
  uint8_t                   StartDxioLane;                ///< Start Dxio Lane (Translated)
  uint8_t                   EndDxioLane;                  ///< End Dxio Lane (Translated)
  struct {
    uint8_t                 PowerOffUnusedLanes:1;        ///< Power Off unused lanes
    uint8_t                 PowerOffUnusedPlls:1;         ///< Power Off unused Plls
    uint8_t                 ClkGating:1;                  ///< TXCLK gating
    uint8_t                 LclkGating:1;                 ///< LCLK gating
    uint8_t                 TxclkGatingPllPowerDown:1;    ///< TXCLK clock gating PLL power down
    uint8_t                 PllOffInL1:1;                 ///< PLL off in L1
    uint8_t                 AccessEncoding:1;             ///< Reg access encoding
    uint8_t                 CoreReversed:1;               ///< Indicates lanes are reversed in package connection
  } Features;
  uint8_t                   MasterPll;                    ///< Bitmap of master PLL
  uint32_t                   AcsSupport:1;                    ///< Acs Support
  uint32_t                   LtrSupport:1;                    ///< LTR Support
  uint32_t                   AriForwarding:1;                 ///< ARI Forwarding
  uint32_t                   LaneMargining:1;                 ///< Lane Margining
  uint32_t                   NativePCIeEnclosureManagement:1; ///< NPEM
  uint32_t                   DownstreamPortContainment:1;     ///< Downstream port containment
  uint32_t                   AdvancedErrorReporting:1;        ///< Advacned Error Reporting
  uint32_t                   ECRCSupport:2;                   ///< ECRC Capability
  uint32_t                   Reserved:23;                     ///< Reserved bits
} PCIe_WRAPPER_CONFIG;


/// Silicon configuration data
typedef struct  {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  uint8_t                   SocketId;                     ///< Socket ID
  uint8_t                   DieNumber;                    ///< Module ID
  uint8_t                   RBIndex;                      ///< Physical Root Bridge
  uint8_t                   InstanceId;                   ///< Logical Instance Identifier
  PCI_ADDR                Address;                      ///< PCI address of GNB host bridge
  uint16_t                  StartLane;                    ///< Start Lane of this node
  uint16_t                  EndLane;                      ///< End Lane of this node
  uint8_t                   BusNumberLimit;               ///< Last Bus Number assigned to this node
  uint8_t                   SbPresent:1;                  ///< Set to 1 if FCH connected to this NBIO
  uint8_t                   SmuPresent:1;                 ///< Set to 1 if SMU connected to this NBIO
  uint8_t                   MP_Instance:6;                ///< MP Instance
  uint8_t                   LogicalRBIndex;               ///< Logical Root Bridge
  uint8_t                   NumEngineDesc;                ///< Total number of lane bifurcation descriptors
} PCIe_SILICON_CONFIG;

typedef PCIe_SILICON_CONFIG GNB_HANDLE;

/// Complex configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  uint8_t                   NodeId;                       ///< Processor Node ID
  uint8_t                   Reserved;                     ///< For alignment
} PCIe_COMPLEX_CONFIG;

/// PCIe platform configuration info
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  uint64_t                   StdHeader;                    ///< Standard configuration header
  uint32_t                  LinkReceiverDetectionPooling; ///< Receiver pooling detection time in us.
  uint32_t                  PhyConfigData;                ///< Phy Configuration Data
  uint32_t                  LinkGpioResetAssertionTime;   ///< Gpio reset assertion time in us
  uint32_t                  LinkResetToTrainingTime;      ///< Time duration between deassert GPIO reset and release training in us                                                      ///
  uint32_t                  PsppTuningParams;             ///< Tuning parameters for PSPP
  uint32_t                  PsppTuningParams2;            ///< Tuning parameters 2 for PSPP
  uint8_t                   GfxCardWorkaround;            ///< GFX Card Workaround
  uint8_t                   PsppPolicy;                   ///< PSPP policy
  uint8_t                   TrainingExitState;            ///< State at which training should exit (see PCIE_LINK_TRAINING_STATE)
  uint8_t                   TrainingAlgorithm;            ///< Training algorithm (see PCIE_TRAINING_ALGORITHM)
  PCIe_COMPLEX_CONFIG     ComplexList[MAX_NUMBER_OF_COMPLEXES];  ///< Complex
} PCIe_PLATFORM_CONFIG;


/// PCIe Engine Description

typedef struct {
  uint32_t                  Flags;                        /**< Descriptor flags
                                                         * @li @b Bit31 - last descriptor on wrapper
                                                         * @li @b Bit30 - Descriptor allocated for PCIe port
                                                         */
  PCIe_ENGINE             EngineData;                   ///< Engine Data
} PCIe_ENGINE_DESCRIPTOR;

/// PSPP_Policy_For_Each_Port
typedef union {
  struct {                                                             ///<
    uint32_t   DC_MinPreferredSpeed:4;
    uint32_t   Reserved1:1;
    uint32_t   DC_MaxPreferredSpeed:4;
    uint32_t   DC_DirectIndirect:1;
    uint32_t   AC_MinPreferredSpeed:4;
    uint32_t   Reserved2:1;
    uint32_t   AC_MaxPreferredSpeed:4;
    uint32_t   AC_DirectIndirect:1;
    uint32_t   Reserved3:12;
  } Field;                                                             ///<
  uint32_t Value;                                                        ///<
} PSPP_Policy_STRUCT;

#pragma pack (pop)




