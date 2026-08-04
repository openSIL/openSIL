/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioStructs.h
 *  @brief MPIO data structures
 */

#pragma once

#include <SilPcie.h>
#include <Nbio/NbioIp2Ip.h>
#include <Nbio/Common/PciStructs.h>
#include <Pci.h>
#include <APOB/Common/ApobCmn.h>


#pragma pack(push,1)

#define PCIE_PORT_PARAMETER_COUNT  64

/**
 * @brief Engine descriptor type
 * @details
 */
typedef enum {
  MpioUnusedEngine = 0,                                   ///< Unused descriptor Excluded from configuration
  MpioPcieEngine = 1,                                     ///< PCIe port
  MpioUSB_OVER_PCIE = 4,                                  ///<  USB4 PCIe
  MpioUBMHFCEngine = 5,                                   ///< UBM HFC Connector for auto-discovery
  MpioOCP3Engine = 6,                                     ///< OCP3 Bifurcatable Connector
  MpioUdot3Engine = 7,                                    ///< U.3 Multiprotocol Connector
  MpioDPEngine = 8,                                       ///< Digital Display __For APU display connector list__
  MpioMaxPcieEngine                                       ///< Max engine type for boundary check.
} MPIO_ENGINE_TYPE;


//
// C2PMSG_97  (SMN address 0x3810A84)
//
typedef union {
  struct {
    uint32_t Environment:4;                  ///< Environment
    uint32_t CoreClNum:3;                    ///< Core CL selector: Varies from release to release.  Replaces DF scratch 30[23:0] force
    uint32_t DisablePmuDevinitUseSwmra:1;    ///< Disable DEVINIT and use SWMRA instead. Replaces UmcCfgEco[19] force
    uint32_t OsBootFromDram:1;               ///< OS boot from DRAM.
    uint32_t SkipAllUSBControllerAccess:1;   ///< When set FW will skip all USB Control init communication from the FW
    uint32_t SkipAllUSBPhyAccess:1;          ///< When set FW will skip all USB phy communication from the FW
    uint32_t SkipMpioInitialization:1;       ///< When set FW will skip MPIO Initialization from the FW>
    uint32_t HspDisabled:1;                  ///< HSP disabled in FW
    uint32_t ForceMpmMeventPath:1;           ///< Set to 1 to force MPM M-event path (X86 not released)
    uint32_t SkipUSBCodeInSBIOS:1;           ///< Bit 14 Set to 1 to skip USB code in SBIOS
    uint32_t Port80Redirect:1;               ///< When Bit 15 =1 and Bit 18 =0, Redirect the port80 writes to NBCFG_SCRATCH_0
    uint32_t :1;                             ///< Reserved
    uint32_t SimNowCRBBSD:1;                 ///< Bit 17 Set to 1 to indicate the Environment is SimNow CRB BSD
    uint32_t Port80Redirect2:1;              ///< Bit 18 =1 Redirect the port80 writes to MSR
    uint32_t SkipESPICode:1;                 ///< Bit 19 Set to 1 to skip eSPI code
    uint32_t :12;                            ///< Reserved
  } Field;
  uint32_t  Value;
} MPIO_PRESIL_CTRL0;


typedef union {
  struct {
    uint32_t DisableMemtest:1;               ///< Disable the memory test.  Replaces UmcCfgEco[16] force.  DEPRECATING: Will move to APCB
    uint32_t DisableMemclear:1;              ///< Disable the memory clear.  Replaces DF scratch 30[31] force.  DEPRECATING: Already in APCB
    uint32_t DisableMemSetReg:1;             ///< Controls logging of UMC/PMU register writes.  Replaces UmcCfgEco[17] force.  DEPRECATING: Will create a filter in APCB
    uint32_t DisableMemtestVerbose:1;        ///< Controls logging of memtest.  Replaces UmcCfgEco[17] force.  DEPRECATING: Will create a filter in APCB
    uint32_t DisableExtSpd:1;                ///< Disable SPD read attempts across SMBus.  Replaces UmcCfgEco[20] force.  DEPRECATING: Will move to APCB
    uint32_t DisableRrwMemtest:1;            ///< Disable reliable read-write memtest.  Replaces UmcCfgEco[21] force.  DEPRECATING: Will move to APCB
    uint32_t DisableSettingUnusedChannel:1;  ///< Disable accesses to mem channels other than 0.  Replaces UmcCfgEco[22] force.  DEPRECATING: Will consume harvest fuses
    uint32_t DisableDxioPhyFwLoad:1;         ///< Disable MP0 DXIO phy firmware loading process.  Replaces DF scratch 30[31] force.  DEPRECATING: Will try to move to APCB
    uint32_t DisableMemToUseDvSettings:1;    ///< Design verification settings.  Replaces UmcCfgEco[23] force.  DEPRECATING: Will be a build time switch
    uint32_t EnableAblVerboseMessaging:1;    ///< verbose message control; 0- no override (controlled by build config), 1- enable>
    uint32_t DisableSerialOut:1;             ///< serial out control; 0- no override (controlled by build config), 1- disable>
    uint32_t EnableIoRedirect:1;             ///< Port 80 control. Replaces C2PMSG_82[31] force; 0- no override (controlled by build config), 1- enable>
    uint32_t FASTSIM_GIO:1;                  ///< FASTSIM_GIO
    uint32_t FASTSIM_DXIO:1;                 ///< FASTSIM_DXIO
    uint32_t FASTSIM_DFLT_TBL:1;             ///< FASTSIM_DFLT_TBL
    uint32_t FASTSIM_SMU_MSGS:1;             ///< FASTSIM_SMU_MSGS
    uint32_t FASTSIM_PWR_MSGS:1;             ///< FASTSIM_PWR_MSGS
    uint32_t FASTSIM_PEI_LOG:1;              ///< FASTSIM_PEI_LOG
    uint32_t FASTSIM_DXE_LOG:1;              ///< FASTSIM_DXE_LOG
    uint32_t UseUmcHardcode:1;               ///< Use Hardcoded UMC Settings
    uint32_t UseDdrPhyHardcode:1;            ///< Use Hardcoded DDR PHY Settings
    uint32_t EmulatorType:2;                 ///< Emulator Type. 00 - Palladium. 01 - Veloce.
    uint32_t UseDfHardcode:1;                ///< Use Hardcoded DV DF settings
    uint32_t DisableMp1DxioPhyFwLoad:1;      ///< Disable phy FW loading for MP1
    uint32_t EnableCharPrintSocket1:1;       ///< Enable Socket 1 CharPrint to C2P_MSG_99
    uint32_t ForceFeatureEnForPMMTest:2;     ///< Feature Enablement for PMM Testing
    uint32_t :4;                             ///< Reserved
  } Field;
  uint32_t  Value;
} MPIO_PRESIL_CTRL1;

///MPIO Link Capabilities
typedef struct {
  uint32_t            portPresent: 1;            ///< portPresent
  uint32_t         earlyTrainLink: 1;            ///< earlyTrainLink
  uint32_t     linkComplianceMode: 1;            ///< linkComplianceMode
  uint32_t           linkReversal: 1;            ///< linkReversal
  uint32_t        maxLinkSpeedCap: 3;            ///< maxLinkSpeedCap
  uint32_t         endpointStatus: 1;            ///< endpointStatus
  uint32_t            linkHotplug: 3;            ///< linkHotplug
  uint32_t               portSize: 5;            ///< portSize
  uint32_t    maxLinkSpeedTrained: 3;            ///< maxLinkSpeedTrained
  uint32_t  enableTurnOffSettings: 1;            ///< enableTurnOffSettings
  uint32_t     turnOffUnusedLanes: 1;            ///< enableTurnOffSettings
  uint32_t             ntbHotPlug: 1;            ///< NTB Hotplug Support
  uint32_t           CurrentSpeed: 2;            ///< PSPP Current Speed
  uint32_t               PSPPMode: 3;            ///< PSPP Mode
  uint32_t      partnerDeviceType: 2;            ///< Partner Device Type
  uint32_t   autoSpeedChangeCntrl: 2;            ///< Auto Speed Change Control
  uint32_t            isMasterPLL: 1;            ///< isMasterPLL
  uint32_t           eqSearchMode: 2;            ///< eqSearchMode
  uint32_t      eqSearchModeOride: 1;            ///< eqSearchModeOride
  uint32_t         invertPolarity: 1;            ///< invertPolarityRx
  uint32_t              txVetting: 1;            ///< txVetting
  uint32_t              rxVetting: 1;            ///< rxVetting
  uint32_t           TXDeEmphasis: 2;            ///< TXDeEmphasis value
  uint32_t      TXDeEmphasisOride: 1;            ///< TXDeEmphasisOride
  uint32_t        invertPolarity2: 1;            ///< invertPolarityTx
  uint32_t        targetLinkSpeed: 3;            ///< targetLinkSpeed
  uint32_t           bypassGen3EQ :1;            ///< bypassGen3EQ
  uint32_t           bypassGen4EQ :1;            ///< bypassGen4EQ
  uint32_t          capabilities1:17;            ///< capabilities1
} MPIO_LINK_CAP;

///MPIO Link Descriptor Base
typedef struct {
  uint32_t          ChannelType: 8;            ///< ChannelType
  uint32_t        ChannelDescID: 8;            ///< ChannelDescID
  uint32_t  AncillaryDataOffset:16;            ///< Reserved3
  uint32_t         DevFunNumber:32;            ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t                MacID: 8;            ///< MacID
  uint32_t            MacPortID: 8;            ///< MacPortID
  uint32_t        StartCoreLane: 8;            ///< StartCoreLane
  uint32_t          EndCoreLane: 8;            ///< EndCoreLane
  uint32_t                PcsID: 8;            ///< PcsID
  uint32_t           Reserved1: 24;            ///< Reserved1
} MPIO_LINK_DESC_BASE;

///MPIO Link Descriptor Eth
typedef struct {
  uint32_t          ChannelType: 8;            ///< ChannelType
  uint32_t            Reserved1: 8;            ///< Reserved1
  uint32_t  AncillaryDataOffset:16;            ///< AncillaryDataOffset
  uint32_t         DevFunNumber:32;            ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t            Reserved2:32;            ///< Reserved2
  uint32_t            Reserved3:32;            ///< Reserved3
} MPIO_LINK_DESC_ETH;

///MPIO Link Descriptor PCIe
typedef struct {
  uint32_t          ChannelType: 8;            ///< ChannelType
  uint32_t        ChannelDescID: 8;            ///< ChannelDescID
  uint32_t  AncillaryDataOffset:16;            ///< Reserved3
  uint32_t         DevFunNumber:32;            ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t                MacID: 8;            ///< MacID
  uint32_t            MacPortID: 8;            ///< MacPortID
  uint32_t        StartCoreLane: 8;            ///< StartCoreLane
  uint32_t          EndCoreLane: 8;            ///< EndCoreLane
  uint32_t                PcsID: 8;            ///< PcsID
  uint32_t    linkTrainingState: 8;            ///< linkTrainingState
  uint32_t           Reserved1: 16;            ///< Reserved1
} MPIO_LINK_DESC_PCIE;

typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_USB;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_DP;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_GOP;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_GMI;

///MPIO Link Descriptor
typedef union {
  MPIO_LINK_DESC_BASE   Base;              ///< MPIO Link Descriptor Base
  MPIO_LINK_DESC_PCIE   Pcie;              ///< MPIO Link Descriptor PCIe
  MPIO_LINK_DESC_USB    Usb;               ///< MPIO Link Descriptor Usb
  MPIO_LINK_DESC_DP     Dp;                ///< MPIO Link Descriptor Dp
  MPIO_LINK_DESC_ETH    Eth;               ///< MPIO Link Descriptor Eth
  MPIO_LINK_DESC_GOP    Gop;               ///< MPIO Link Descriptor Gop
  MPIO_LINK_DESC_GMI    Gmi;               ///< MPIO Link Descriptor Gmi
} MPIO_LINK_DESC;

///MPIO Engine Descriptor
typedef struct {
  uint32_t           EngineType: 8;            ///< EngineType
  uint32_t         HotPluggable: 1;            ///< HotPluggable
  uint32_t            Reserved3: 7;            ///< Reserved3
  uint32_t            StartLane: 8;            ///< StartLane
  uint32_t              EndLane: 8;            ///< EndLane
  uint32_t          GpioGroupID: 8;            ///< GpioGroupID
  uint32_t        engineResetID: 8;            ///< engineResetID
  uint32_t         searchDepth : 1;            ///< searchDepth
  uint32_t      forceKpnpReset : 1;            ///< forceKpnpReset
  uint32_t            Reserved2:14;            ///< Reserved2
  MPIO_LINK_DESC        LinkDesc;              ///< LinkDesc
  uint32_t         MacEnginePtr:16;            ///< MacEnginePtr
  uint32_t          FirstLgdNum: 8;            ///< FirstLgdNum
  uint32_t           LastLgdNum: 8;            ///< LastLgdNum
  uint32_t        TrainingState: 4;            ///< TrainingState
  uint32_t            Reserved1:28;            ///< Reserved1
} MPIO_ENGINE_DESCRIPTOR;

/**
 * @brief High level data structures for passing topology from platform to openSIL
 * @details Engine Configuration
 */
typedef struct {
  uint8_t                EngineType;               /**< Engine type
                                                    *  @li @b 0 -  Ignore engine configuration
                                                    *  @li @b 1 -  PCIe port
                                                    *  @li @b 2 -  DDI
                                                    */
  uint8_t                HotPluggable  :1;         /**<HotPluggable
                                                    *  @li @b 0 - Link is NOT Hot-Switchable
                                                    *  @li @b 1 - Link IS Hot-Switchable
                                                    */
  uint8_t                Reserved1     :7;         /**< Unused field, leave as 0
                                                    */
  uint8_t                StartLane;                /**< Start Lane ID (in reversed configuration StartLane > EndLane)
                                                    * Refer to lane descriptions and supported configurations in BKDG
                                                    */
  uint8_t                EndLane;                  /**< End lane ID (in reversed configuration StartLane > EndLane)
                                                    * Refer to lane descriptions and supported configurations in BKDG
                                                    */
  uint8_t                GpioGroupId;              /**< Unique identifier for the GPIO or GPIO group associated with
                                                    * this engine.  GPIOs are used for hotplug notification and link
                                                    * type (e.g PCIe)
                                                    */
  uint8_t                MpioStartLane;            /**< Internal coding of start lane
                                                    */
  uint8_t                MpioEndLane;              /**< Internal coding of end lane
                                                    */
  uint8_t                SearchDepth;              /**< SearchDepth only uses 1 bit - always initialize to 0 will be updated dynamically
                                                    */
} MPIO_ENGINE_DATA;

/**
 * @brief PCIe specific data structures
 * @details PCIe port misc extended controls
 */
typedef struct  {
  uint8_t                     LinkComplianceMode     :1;  ///< Force port into compliance mode (device will not be trained, port output compliance pattern)
  uint8_t                     LinkSafeMode           :1;  /**< Safe mode PCIe capability. (Parameter may limit PCIe speed requested through MPIO_PORT_DATA::LinkSpeedCapability)
                                                           *  @li @b 0 - port can advertise maximum supported capability
                                                           *  @li @b 1 - port limit advertised capability and speed to PCIe Gen1
                                                           */
  uint8_t                     SbLink                 :1;  /**< PCIe link type
                                                           *  @li @b 0 - General purpose port
                                                           *  @li @b 1 - Port connected to SB
                                                           */
  uint8_t                     ClkPmSupport           :1;  /**< Clock Power Management Support
                                                           *  @li @b 0 - Clock Power Management not configured
                                                           *  @li @b 1 - Clock Power Management configured according to PCIe device capability
                                                           */
  uint8_t                     ChannelType            :3;  /**< Channel Type
                                                           *  @li @b 0 - Channel Type Not Specified
                                                           *  @li @b 1 - Channel Type Short Trace
                                                           *  @li @b 2 - Channel Type Long Trace
                                                           */
  uint8_t                     TurnOffUnusedLanes     :1;  /**< Turn Off Unused Lanes
                                                           *  @li @b 0 -
                                                           *  @li @b 1 -
                                                           */
} MPIO_PORT_MISC_CONTROL;

/**
 * @brief The IO APIC Interrupt Mapping Info
 * @details
 */
typedef struct {
  uint8_t                     GroupMap;               /**< Group mapping for slot or endpoint device (connected to PCIE port) interrupts .
                                                       *  @li <b>0</b> - IGNORE THIS STRUCTURE AND USE RECOMMENDED SETTINGS
                                                       *  @li <b>1</b> - mapped to Grp 0 (Interrupts 0..3   of IO APIC redirection table)
                                                       *  @li <b>2</b> - mapped to Grp 1 (Interrupts 4..7   of IO APIC redirection table)
                                                       *  @li ...
                                                       *  @li <b>8</b> - mapped to Grp 7 (Interrupts 28..31 of IO APIC redirection table)
                                                       */
  uint8_t                     Swizzle;                /**< Swizzle interrupt in the Group.
                                                       *  @li <b>0</b> - ABCD
                                                       *  @li <b>1</b> - BCDA
                                                       *  @li <b>2</b> - CDAB
                                                       *  @li <b>3</b> - DABC
                                                       */
  uint8_t                     BridgeInt;              /**< IOAPIC redirection table entry for PCIE bridge interrupt
                                                       *  @li <b>0</b>  - Entry 0  of IO APIC redirection table
                                                       *  @li <b>1</b>  - Entry 1  of IO APIC redirection table
                                                       *  @li ...
                                                       *  @li <b>31</b> - Entry 31 of IO APIC redirection table
                                                       */
} MPIO_APIC_DEVICE_INFO;

/**
 * @brief PCIe port configuration data
 * @details
 */
typedef struct  {
  uint8_t                   PortPresent         :1;  ///< Enable PCIe port for initialization.
  uint8_t                   Reserved1           :2;  ///< Reserved
  uint8_t                   DeviceNumber        :5;  /**< PCI Device number for port.
                                                      *   @li @b 0 - Native port device number
                                                      *   @li @b N - Port device number (See available configurations in BKDG
                                                      */
  uint8_t                   FunctionNumber      :3;  ///< Reserved for future use
  uint8_t                   LinkSpeedCapability :3;  /**< PCIe link speed/
                                                      *  @li @b 0 - Maximum supported by silicon
                                                      *  @li @b 1 - Gen1
                                                      *  @li @b 2 - Gen2
                                                      *  @li @b 3 - Gen3
                                                      *  @li @b 4 - Gen4
                                                      *  @li @b 5 - Gen5
                                                      */
  uint8_t                   AutoSpdChng         :2;  /**< Upstream Auto Speed Change Allowed/
                                                      *  @li @b 0 - Use default implementation (Disabled for Gen1, Enabled for Gen2/3)
                                                      *  @li @b 1 - Always Disabled
                                                      *  @li @b 2 - Always Enabled
                                                      *  @li @b 3 - Reserved
                                                      */
  uint8_t                   EqPreset            :4;  /**< Gen3 Equalization Preset */
  uint8_t                   LinkAspm            :2;  /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                      *  @li @b 0 - Disabled
                                                      *  @li @b 1 - L0s only
                                                      *  @li @b 2 - L1 only
                                                      *  @li @b 3 - L0s and L1
                                                      */
  uint8_t                   LinkAspmL1_1        :1;  /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                      *  @li @b 0 - Disabled
                                                      *  @li @b 1 - Enabled
                                                      */
  uint8_t                   LinkAspmL1_2        :1;  /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                      *  @li @b 0 - Disabled
                                                      *  @li @b 1 - Enabled
                                                      */
  uint8_t                   ClkReq              :4;  /**< ASPM Reserved Field
                                                      *  @li @b 0 - NONE
                                                      *  @li @b 1 - CLKREQ0 signal
                                                      *  @li @b 2 - CLKREQ1 signal
                                                      *  @li @b 3 - CLKREQ2 signal
                                                      *  @li @b 4 - CLKREQ3 signal
                                                      *  @li @b 5 - CLKREQG signal
                                                      */
  uint8_t                   LinkHotplug         :4;  /**< Hotplug control.
                                                      *  @li @b 0 - Disabled
                                                      *  @li @b 1 - Basic
                                                      *  @li @b 2 - Server
                                                      *  @li @b 3 - Enhanced
                                                      */
  uint8_t                   SlotPowerLimit;          ///< PCIe slot power limit.
  uint8_t                   SlotPowerLimitScale :2;  /**< PCIe slot power limit Scale.
                                                      *  @li @b 00b = 1.0x
                                                      *  @li @b 01b = 0.1x
                                                      *  @li @b 10b = 0.01x
                                                      *  @li @b 11b = 0.001x
                                                      */
  uint8_t                   IsMasterPLL         :1;  ///< IsMasterPLL
  uint8_t                   Gen4Features        :5;  /**< Unused bits
                                                      *< BIT0(DLF_Exchange) 1 - Disable, 0 - Enable
                                                      *< BIT1(DLF_Capability) 1 - Disable, 0 - Enable
                                                      */
  uint16_t                  SlotNum             :13; ///< PHYSICAL_SLOT_NUM
  uint16_t                  CsLink              :3;  ///< Reserved
  MPIO_PORT_MISC_CONTROL    MiscControls;            ///< Misc extended controls
  MPIO_APIC_DEVICE_INFO     ApicDeviceInfo;          ///< IOAPIC device programming info
  PCIE_ENDPOINT_STATUS      EndpointStatus;          ///< PCIe endpoint (device connected to PCIe port) status
  uint8_t                   EsmSpeedBump;            ///< Speed bump for ESM
  uint8_t                   EsmControl          :1;  ///< Enhanced speed mode control
  uint8_t                   CxlControl          :1;  ///< CXL control
  uint8_t                   TxVetting           :1;  ///< Tx Vetting
  uint8_t                   RxVetting           :1;  ///< Rx Vetting
  uint8_t                   InvertPolarity      :1;  ///< Invert RX Polarity
  uint8_t                   InvertPolarity2     :1;  ///< Invert TX Polarity
  uint8_t                   NtbHotplug          :1;  /**< NTB Hotplug flag
                                                      *  @li @b 0b = Disabled
                                                      *  @li @b 1b = Enabled
                                                      */
  uint8_t                   Reserved2           :1;  ///< Reserved
  uint8_t                   SetGen3FixedPreset  :1;  ///< Gen3 Fixed Preset Set
  uint8_t                   SetGen4FixedPreset  :1;  ///< Gen4 Fixed Preset Set
  uint8_t                   SetGen5FixedPreset  :1;  ///< Gen5 Fixed Preset Set
  uint8_t                   Reserved3           :5;  ///< Reserved
  uint8_t                   Gen3FixedPreset     :4;  ///< Gen3 Fixed Preset
  uint8_t                   Gen4FixedPreset     :4;  ///< Gen4 Fixed Preset
  uint8_t                   Gen5FixedPreset     :4;  ///< Gen5 Fixed Preset
  uint8_t                   Reserved4           :4;  ///< Reserved
  uint16_t                  PsppPolicyDC;            ///< Pspp DC control
  uint16_t                  PsppPolicyAC;            ///< PSPP AC control
  uint8_t                   PsppDeviceType;          ///< Pspp Device Type
  uint8_t                   DisGen3EQPhase      :1;  ///< Gen3 Bypass phase2/3 EQ
  uint8_t                   DisGen4EQPhase      :1;  ///< Gen4 Bypass phase2/3 EQ
  uint8_t                   TXDeEmphasisOride   :1;  ///< Override Gen2 MPIO deemphasis default
  uint8_t                   TXDeEmphasis        :2;  ///< Gen2 MPIO deemphasis setting
  uint8_t                   Reserved5           :3;  ///< Reserved
  struct {
    uint16_t                  DsTxPreset          :4;   ///< Gen3 Downstream Tx Preset
    uint16_t                  DsRxPresetHint      :3;   ///< Gen3 Downstream Rx Preset Hint
    uint16_t                  UsTxPreset          :4;   ///< Gen3 Upstream Tx Preset
    uint16_t                  UsRxPresetHint      :3;   ///< Gen3 Upstream Rx Preset Hint
    uint16_t                  Reserved1           :2;   ///< Unused bits
    uint8_t                   SetDsTxPreset       :1;   ///< Gen3 Set Downstream Tx Preset
    uint8_t                   SetDsRxPresetHint   :1;   ///< Gen3 Set Downstream Rx Preset Hint
    uint8_t                   SetUsTxPreset       :1;   ///< Gen3 Set Upstream Tx Preset
    uint8_t                   SetUsRxPresetHint   :1;   ///< Gen3 Set Upstream Rx Preset Hint
    uint8_t                   Reserved2           :4;   ///< Unused bits
  } LaneEqualizationCntl;                               ///< Lane equalization control structure used for Gen3 values
  struct {
    uint8_t                   DsTxPreset          :4;   ///< Gen4 Downstream Tx Preset
    uint8_t                   UsTxPreset          :4;   ///< Gen4 Upstream Tx Preset
    uint8_t                   SetDsTxPreset       :1;   ///< Gen4 Set Downstream Tx Preset
    uint8_t                   SetUsTxPreset       :1;   ///< Gen4 Set Upstream Tx Preset
    uint8_t                   Reserved1           :6;   ///< Unused bits
  } Gen4LaneEqualizationCntl;                           ///< Lane equalization control structure used for Gen4 values
  struct {
    uint8_t                   DsTxPreset          :4;   ///< Gen5 Downstream Tx Preset
    uint8_t                   UsTxPreset          :4;   ///< Gen5 Upstream Tx Preset
    uint8_t                   SetDsTxPreset       :1;   ///< Gen5 Set Downstream Tx Preset
    uint8_t                   SetUsTxPreset       :1;   ///< Gen5 Set Upstream Tx Preset
    uint8_t                   Reserved1           :6;   ///< Unused bits
  } Gen5LaneEqualizationCntl;                           ///< Lane equalization control structure used for Gen5 values
  struct {
    uint32_t                  PresetMask8Gt       :10; ///< Preset Mask 8GT.
    uint32_t                  PresetMask16Gt      :10; ///< Preset Mask 16GT.
    uint32_t                  PresetMask32Gt      :10; ///< Preset Mask 32GT.
    uint32_t                  Reserved1           :2;  ///< Unused bits
    uint8_t                   SetPresetMask8Gt    :1;  ///< Preset Mask 8GT Set
    uint8_t                   SetPresetMask16Gt   :1;  ///< Preset Mask 16GT Set
    uint8_t                   SetPresetMask32Gt   :1;  ///< Preset Mask 32GT Set
    uint8_t                   Reserved2           :5;  ///< Unused bits
  } PresetMaskCntl;                                    ///< Preset Mask control structure used for Gen3/Gen4/Gen5 values
  uint8_t                   TargetLinkSpeed     :3;    ///< Target Link Speed
  uint8_t                   BypassGen3EQ        :1;    ///< Bypass Gen3 equalization
  uint8_t                   BypassGen4EQ        :1;    ///< Bypass Gen4 equalization
  uint8_t                   SrisSkipInterval    :3;    ///< Controls SRIS SKP generation interval
  uint8_t                   SrisEnableMode      :4;    ///< 0:Disable 1:Enable 0xF:Auto
  uint8_t                   SrisAutoDetectMode  :4;    ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  uint8_t                   LowerSkpOsGenSup;          ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  uint8_t                   LowerSkpOsRcvSup;          ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  uint8_t                   SrisSkpIntervalSel  :2;    ///< Controls SRIS SKIP Interval Selection Mode
  uint8_t                   SrisAutodetectFactor:2;    ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  uint8_t                   IsBmcLocation       :1;    ///< IsBmcLocation
  uint8_t                   SetEsmControl       :1;    ///< Set ESM Control
  uint8_t                   SetEsmSpeedBump     :1;    ///< Set Speed bump for ESM
  uint8_t                   Reserved6           :1;    ///< Unused bits
  uint8_t                   I2CMuxInfo          :6;    ///< Legacy I2c switch
  uint8_t                   AlwaysExpose        :1;    ///< Always expose unused PCIE port
  uint8_t                   Reserved7           :1;    ///< Unused bits
  uint16_t                  NpemEnable          :12;   ///< Controls NPEM Enable
  uint16_t                  Reserved8           :4;    ///< Unused bits
  uint16_t                  NpemCapability      :12;   ///< Controls NPEM Capability
  uint16_t                  SwingMode           :3;    ///< PCIe Swing Mode
  uint16_t                  Reserved9           :1;    ///< Unused bits
  uint16_t                  MpioAncDataIdx;            ///< Reserved
  uint8_t                   Reserved10;                ///< Reserved bits
} MPIO_PORT_DATA;

typedef struct {
  uint16_t               ParamType;      ///< This identifies a specific PHY parameter
  uint16_t               ParamValue;     ///< This specifies the value to be assigned to indicated PHY parameter
} MPIO_PHY_PARAM;

typedef struct {
  MPIO_PHY_PARAM       PhyParam[44];     ///< physical parameter
} MPIO_PHY_DATA;

typedef struct {
  uint16_t               ParamType;                ///< This identifies a specific port parameter to set.
  uint16_t               ParamValue;               ///< Specifies the value to be assigned to port parameter.
} MPIO_PORT_PARAM;

typedef struct {
  MPIO_PORT_PARAM           PhyParam[PCIE_PORT_PARAMETER_COUNT];      ///< PHY port parameter
} MPIO_PORT_PARAMS;

/**
 *  @brief Port ancillary data override structure
 *  @details Ancillary data override data support type
 */
typedef struct {
  uint8_t                Type;                /**< Ancillary Data Type.
                                               *  @li <b>0</b> - Not override (NOT_OVRD)
                                               *  @li <b>3</b> - Speed override (SPD_OVRD)
                                               */
  uint32_t               Value;               /**< Ancillary Data Value.
                                               *  Speed override (SPD_OVRD):
                                               *  Continue define SPD_OVRD type for each gen speed override. First for Gen1, Second for Gen2, Third for Gen3, etc.
                                               */

} MPIO_ANC_DATA_PARAM;

/**
 *  @brief Ancillary data struct with table size and address
 */
typedef struct {
  uint32_t               Count;               ///< Total count in this Ancillary data table
  uint32_t               Ovrd;                ///< Ancillary data table address point to ANC_DATA_PARAM[]
} MPIO_ANC_DATA;

typedef struct {
  uint32_t               Flags;                /**< Descriptor flags
                                                * @li @b Bit31 - last descriptor in complex
                                                */
  MPIO_ENGINE_DATA     EngineData;             ///< Engine data
  MPIO_PORT_DATA       Port;                   ///< PCIe port specific configuration info
  uint32_t             Reserved[8];            ///< Reserved data
  MPIO_PHY_DATA        Phy;                    ///< Ancillary data for PHY programming customization
  MPIO_PORT_PARAMS     PortParams;             ///< Extensible port parameter list for simplified topology structure
  MPIO_ANC_DATA        AncData;                ///< Ancillary data override
} MPIO_PORT_DESCRIPTOR;

///Port information needed for lane translation callback
typedef struct {
  uint8_t      StartLane;                    ///< Start Lane from User Config
  uint8_t      EndLane;                      ///< End Lane from User Config
  uint8_t      MpioStartLane;                ///< Translated Start Lane
  uint8_t      MpioEndLane;                  ///< Translated End Lane
  bool         LinkReversal;                 ///< Link Reversal should be set
  bool         LanesFound;                   ///< Flag for lanes found
  uint8_t                   SocketNumber;    ///< SocketNumber
  uint8_t                   EngineID;        ///< Index of EngineID
  uint8_t                   MacPortID;       ///< MacPortID from MPIO firmware
  MPIO_PORT_DESCRIPTOR    *UserCfgEntry;     ///< Pointer into UserCfgPortList
  MPIO_ENGINE_DESCRIPTOR  *MpioEntry;        ///< Pointer into MPIO Engine Config
  uint8_t                   *PortDevMap;     ///< Pointer to array of port device mapping
  PCIe_ENGINE_CONFIG      *EarlyTrainEngine; ///< Pointer to early train engine
} MPIO_PORT_INFO_CALLBACK;

/**
 *  @brief Port parameter override enumerator.
 *  @details This enumerator provides a parameter type for platform topology override values.
 */
typedef enum {
  MPIO_PP_DEVICE = 1,               /**< Request a specific device number for the root port
                                     *  This should be a valid PCI device number within the default
                                     *  range of the PCIe root ports for this program, typically 1-4.
                                     */
  MPIO_PP_FUNCTION,                 /**< Request a specific function number for the root port
                                     *  This should be a valid PCI device number within the default
                                     *  range of the PCIe root ports for this program, typically 1-7.
                                     */
  MPIO_PP_PORT_PRESENT,             ///< Define whether training should be attempted.
  MPIO_PP_LINK_SPEED_CAP,           /**< Limit the maximum link speed supported
                                     *  @li @b 0 - Maximum supported by silicon
                                     *  @li @b 1 - Gen1
                                     *  @li @b 2 - Gen2
                                     *  @li @b 3 - Gen3
                                     *  @li @b 4 - Gen4
                                     *  @li @b 5 - Gen5
                                     */
  MPIO_PP_LINK_ASPM,                /**< Define the ASPM capability.
                                     *  @li @b 0 - MpioAspmDisabled - Disabled
                                     *  @li @b 1 - MpioAspmL0s - PCIe L0s link state
                                     *  @li @b 2 - MpioAspmL1 - PCIe L1 link state
                                     *  @li @b 3 - MpioAspmL0sL1 - PCIe L0s & L1 link state
                                     */
  MPIO_PP_HOTPLUG_TYPE,             /**< Assign a hotplug type.
                                     *   NOTE: UBM and OCP hotplug types are identified by auto-discovery
                                     *  @li @b 0 - PcieHotplugDisabled,
                                     *  @li @b 1 - PcieHotplugBasic,
                                     *  @li @b 2 - PcieHotplugServerExpress
                                     *  @li @b 3 - PcieHotplugEnhanced,
                                     *  @li @b 4 - PcieHotplugInboard,
                                     *  @li @b 5 - PcieHotplugServerEntSSD,
                                     *  @li @b 6 - PcieHotplugUBM,
                                     *  @li @b 7 - PcieHotplugOCP
                                     */
  MPIO_PP_CLKREQ,                   /**< Specify a clock request line.
                                     *  @li MpioClkReqNone - Default None
                                     *  @li MpioClkReq0 - CLKREQ0 signal
                                     *  @li MpioClkReq1 - CLKREQ1 signal
                                     *  @li MpioClkReq2 - CLKREQ2 signal
                                     *  @li MpioClkReq3 - CLKREQ3 signal
                                     *  @li MpioClkReqG - CLKREQG/CLKREQ4 signal
                                     *  @li MpioClkReq5 - CLKREQ5 signal
                                     *  @li MpioClkReq6 - CLKREQ6 signal
                                     *  @li MpioClkReqInvalid = 0x8,  Reserved
                                     *  @li MpioGpp0ClkReq - gpp0_clkreq signal - MISC2
                                     *  @li MpioGpp1ClkReq - gpp1_clkreq signal - MISC2
                                     *  @li MpioGpp2ClkReq - gpp2_clkreq signal - MISC2
                                     *  @li MpioGpp3ClkReq - gpp3_clkreq signal - MISC2
                                     *  @li MpioGpp4ClkReq - gpp4_clkreq signal - MISC2
                                     */
  MPIO_PP_ASPM_L1_1,                ///< (__BOOLEAN__) Define the L1.1 substate capability.
  MPIO_PP_ASPM_L1_2,                ///< (__BOOLEAN__) Define the L1.2 substate capability.
  MPIO_PP_COMPLIANCE,               ///< (__BOOLEAN__) Request compliance mode.
  MPIO_PP_SAFE_MODE,                ///< (__BOOLEAN__) Request safe mode.
  MPIO_PP_CHIPSET_LINK,             ///< (__BOOLEAN__) Identify this as the link connected to the chipset.
  MPIO_PP_CLOCK_PM,                 ///< (__BOOLEAN__) Enable clock power management.
  MPIO_PP_CHANNELTYPE,              ///< Reserved
  MPIO_PP_TURN_OFF_UNUSED_LANES,    ///< (__BOOLEAN__) Enable powerdown of unused lanes.
  MPIO_PP_MASTER_PLL,               ///< (__BOOLEAN__) Associate this port as the master PLL.
  MPIO_PP_SLOT_NUM,                 ///< Specify a SLOT NUMBER value.
  MPIO_PP_PHY_PARAM,                ///< Specifies a PHY sub-parameter. __SEE MPIO_PHY_PARAM_TYPE__
  MPIO_PP_ESM,                      ///< Enable support for Enhanced Speed Mode.
  MPIO_PP_CXL,                      ///< Enable the port as CXL capable.
  MPIO_PP_GEN3_DS_TX_PRESET,        ///< Gen3 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_DS_RX_PRESET_HINT,   ///< Gen3 Downstream Rx Preset Hint. __3 bit value__
  MPIO_PP_GEN3_US_TX_PRESET,        ///< (__BOOLEAN__) Gen3 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_US_RX_PRESET_HINT,   ///< Gen3 Upstream Rx Preset Hint. __3 bit value__
  MPIO_PP_GEN4_DS_TX_PRESET,        ///< Gen4 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN4_US_TX_PRESET,        ///< Gen4 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN5_DS_TX_PRESET,        ///< Gen5 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN5_US_TX_PRESET,        ///< Gen5 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_FIXED_PRESET,        ///< (__BOOLEAN__) Gen3 Fixed Preset.
  MPIO_PP_GEN4_FIXED_PRESET,        ///< (__BOOLEAN__) Gen4 Fixed Preset.
  MPIO_PP_GEN5_FIXED_PRESET,        ///< (__BOOLEAN__) Gen5 Fixed Preset.
  MPIO_PP_GEN3_PRESET_MASK,         ///< Preset Mask 8GT. __10 bit value__
  MPIO_PP_GEN4_PRESET_MASK,         ///< Preset Mask 16GT. __10 bit value__
  MPIO_PP_GEN5_PRESET_MASK,         ///< Preset Mask 32GT. __10 bit value__
  MPIO_PP_PSPP_DC,                  /**< Pspp DC control.
                                     *   @li __Applicable for client programs__
                                     */
  MPIO_PP_PSPP_AC,                  /**< PSPP AC control.
                                     *   @li __Applicable for client programs__
                                     */
  MPIO_PP_GEN2_DEEMPHASIS,          /**< Specify the deemphasis value for Gen2.
                                     *  @li @b TX_DEEMPHASIS_CSR = 0 - Use de-emphasis from CSR
                                     *  @li @b TX_DEEMPHASIS_US = 0x1 - Use de-emphasis requested by upstream component
                                     *  @li @b TX_DEEMPHASIS_60 = 0x2 - Use -6.0 dB
                                     *  @li @b TX_DEEMPHASIS_35 = 0x3 - Use -3.5 dB
                                     *  @li @b TX_DEEMPHASIS_DIS = 0xFF - Skip override setting
                                     */
  MPIO_PP_INVERT_POLARITY,          ///< (__BOOLEAN__) Request inverted polarity.
  MPIO_PP_TARGET_LINK_SPEED,        /**< Set the desired operating link speed.
                                     *  @li @b 0 - Maximum supported by silicon
                                     *  @li @b 1 - Gen1
                                     *  @li @b 2 - Gen2
                                     *  @li @b 3 - Gen3
                                     *  @li @b 4 - Gen4
                                     *  @li @b 5 - Gen5
                                     */
  MPIO_PP_GEN4_DLF_CAP_DISABLE,     ///< (__BOOLEAN__) Disable GEN4 DLF Capability.
  MPIO_PP_GEN4_DLF_EXCHG_DISABLE,   ///< (__BOOLEAN__) Disable GEN4 DLF Exchange.
  MPIO_PP_I2C_EXPANDER_ADDRESS,     /**< (uint8_t) 7-bit I2C Expander Address for server features
                                     */
  MPIO_PP_I2C_EXPANDER_TYPE,        /**< (uint8_t) I2C Expander Type for server features
                                     * @li @b 0 - UBM_GPIO_EXPANDER_TYPE_9539,
                                     * @li @b 1 - UBM_GPIO_EXPANDER_TYPE_9535,
                                     * @li @b 2 - UBM_GPIO_EXPANDER_TYPE_9506
                                     */
  MPIO_PP_I2C_CLEAR_ALL_INTS,       /**< (__BOOLEAN__) Clear all interrupts on I2c expanders
                                       < Used to force MPIO to clear all interrupts on an expander in cases where additional
                                       < non-spec'd lines are connected to interrupt generating pins.
                                     */
  MPIO_PP_UBM_SWITCH0_ADDR,         /**< (uint8_t) 7-bit I2C Address of UBM Switch __(required)__
                                     */
  MPIO_PP_UBM_SWITCH0_SELECT,       /**< (uint8_t) I2C Select of UBM Switch __(required)__
                                     *   Valid values are 0-3 for 9545, 9546
                                     *                    0-7 for 9548
                                     */
  MPIO_PP_UBM_SWITCH0_TYPE,         /**< (uint8_t) I2C Type of UBM Switch __(required)__
                                     * @li @b 0 - UBM_GPIO_SWITCH_TYPE_9545,
                                     * @li @b 1 - UBM_GPIO_SWITCH_TYPE_9546_48
                                     */
  MPIO_PP_UBM_SWITCH1_ADDR,         /**< (uint8_t) 7-bit I2C Address of second UBM Switch __(optional)__
                                     */
  MPIO_PP_UBM_SWITCH1_SELECT,       /**< (uint8_t) I2C Select of second UBM Switch __(optional)__
                                     *   Valid values are 0-3 for 9545, 9546
                                     *                    0-7 for 9548
                                     */
  MPIO_PP_UBM_SWITCH1_TYPE,         /**< (uint8_t) I2C Type of UBM Switch __(required)__
                                     * @li @b 0 - UBM_GPIO_SWITCH_TYPE_9545,
                                     * @li @b 1 - UBM_GPIO_SWITCH_TYPE_9546_48
                                     */
  MPIO_PP_UBM_HFC_INDEX,            /**< (uint8_t) HFC Index for this port
                                     *    NOTE: This value is set in UBM auto-discovery
                                     *   Valid values are 0-0x31
                                     */
  MPIO_PP_UBM_DFC_INDEX,            /**< (uint8_t) DFC Index for this port
                                     *    NOTE: This value is set in UBM auto-discovery
                                     *   Valid values are 0-0xFF
                                     */
  MPIO_PP_DFC_EVENT,                /**< (UINT8) DFC Event for this port
                                     *   Valid values are 0 = BIOS_EVENT_DEVICE_DISCONNECTED , 1 = BIOS_EVENT_DEVICE_CONNECTED, 2 = BIOS_EVENT_DEVICE_NOT_PRESENT
                                     */
  MPIO_PP_GPIOx_I2C_RESET,          /**< (uint8_t) Pin number on I2C Expander of UBM reset
                                     */
  MPIO_PP_GPIOx_BP_TYPE,            /**< (uint8_t) Pin number of U.3 IFDET2 pin on I2C Expander
                                     */
  MPIO_PP_START_LANE,               /**< (uint8_t) Starting lane for MPIO discovered ports
                                     */
  MPIO_PP_OCP_PRESENT_START,        /**< (uint8_t) Pin number on I2C Expander of first OCP PRESENT pin
                                     */
  MPIO_PP_OCP_DEF_VALID,            /**< (UINT8) Default valid configuration for PRSNT pins
                                     *   Valid values are 0-1
                                     */
  MPIO_PP_OCP_DEF_PRSNTB_PRIMARY,   /**< (UINT8) Default Primary Connector PRSNTB[3:0]#
                                     *   Valid values are 0-0xF
                                     */
  MPIO_PP_OCP_DEF_PRSNTB_SECONDARY, /**< (UINT8) Default Secondary Connector PRSNTB[3:0]#
                                     *   Valid values are 0-0xF
                                     */
  MPIO_PP_OCP_BIF_PRIMARY,          ///Primary Connector BIF[2:0]# (SFF,LFF)
  MPIO_PP_OCP_BIF_SECONDARY,        ///Secondary Connector BIF[2:0]# (LFF only)
  MPIO_PP_OCP_NUM_HOSTS,            /**< (UINT8) Number of Hosts, single, dual , quad
                                     */
  MPIO_PP_OCP_NUM_SOCKETS,          /**< (UINT8) Number of AMD sockets supporting OCP NIC 3.0 PCIe links, 1 or 2
                                     */
  MPIO_PP_OCP_FORM_FACTOR,          /**< (UINT8) OCP form factor SFF or LFF
                                     */
  MPIO_PP_ALWAYS_EXPOSE,            /**< (__BOOLEAN__) Always Expose Unused PCIE port
                                     *  @li @b Disable = 0 - Not exposed
                                     *  @li @b Enable = 1 - Always exposed
                                     */
  MPIO_PP_SRIS_ENABLED,             /**< (uint8_t) SRIS Capability Enabled
                                     *  @li @b Disable = 0
                                     *  @li @b Enable = 1
                                     */
  MPIO_PP_SRIS_SKIP_INTERVAL,       /**< (uint8_t) Controls SRIS SKP generation interval
                                     *  @li @b 0 - 1506; 144; 6050; 640
                                     *  @li @b 1 - 1538; 154; 6068; 656
                                     *  @li @b 2 - 1358; 128; 6032; 624
                                     *  @li @b 3 - 1180; 112; 5996; 608
                                     */
  MPIO_PP_SRIS_LOWER_OS_GEN_SUP,    /**< (uint8_t) Controls LOWER_SKP_OS_GEN_SUPPORT
                                     *  @li @b 0x0 - Disable
                                     *  @li @b 0x1 - Gen1
                                     *  @li @b 0x3 - Gen2
                                     *  @li @b 0x7 - Gen3
                                     *  @li @b 0xF - Gen4
                                     */
  MPIO_PP_SRIS_LOWER_OS_RCV_SUP,    /**< (uint8_t) Controls LOWER_SKP_OS_RCV_SUPPORT
                                     *  @li @b 0x0 - Disable
                                     *  @li @b 0x1 - Gen1
                                     *  @li @b 0x3 - Gen2
                                     *  @li @b 0x7 - Gen3
                                     *  @li @b 0xF - Gen4
                                     */
  MPIO_PP_SRIS_AUTODETECT_MODE,     /**< (uint8_t) Controls SRIS Autodetect mode
                                     *  @li @b Disable = 0
                                     *  @li @b Enable = 1
                                     */
  MPIO_PP_SRIS_SKP_INTERVAL_SEL,    /**< (uint8_t) Controls SRIS SKP Interval Selection Mode
                                     *  @li @b 0x0 - SKP ordered set Interval Lock Mode
                                     *  @li @b 0x1 - Dynamic SKP ordered set Interval Mode
                                     *  @li @b 0x2 - Far End Nominal Empty Mode
                                     */
  MPIO_PP_SRIS_AUTODETECT_FACTOR,   /**< (uint8_t) Controls Autodetect Factor
                                     *  @li @b 0x0 - 1x
                                     *  @li @b 0x1 - 0.95x
                                     *  @li @b 0x2 - 0.9x
                                     *  @li @b 0x3 - 0.85x
                                     */
  MPIO_PP_LEGACY_SWITCH0_ADDR,      /**< (uint8_t) 7-bit I2C Address of Legacy I2c Switch if __(optional)__
                                     *   Defines an 9545 or 9546 I2c switch on the hotplug I2c bus
                                     *   *BEFORE* the switch defined in the legacy hotplug descriptor
                                     *   tables.  This switch will be accessed before the switch in
                                     *   the descriptor tables.
                                     *   Valid values are 0x70-0x77
                                     */
  MPIO_PP_LEGACY_SWITCH0_SELECT,    /**< (uint8_t) I2C Select of Legacy I2c Switch __(optional)__
                                     *   Valid values are 0-3
                                     */
  MPIO_PP_NPEM_ENABLE,              /**< (uint8_t) Controls NPEM features
                                     *  The bit[1] to bit[11] of the NPEM Control register
                                       are set depending on NPEM Capability register
                                     *  Valid bits are bit[0] to bit[11]
                                     */
  MPIO_PP_NPEM_CAPABILITIES,        /**< (UINT16) Controls NPEM Capability
                                     *  NPEM Capability register is set to this value
                                       when PP_NPEM_ENABLE = 1.
                                     *  Valid bits are bit[0] to bit[11]
                                     */
  MPIO_PP_BMC_LOCATION,             /**< (BOOLEAN) States the port location of the BMC
                                     *  Defines the location of the BMC if TRUE
                                     */
  MPIO_PP_SWING_MODE                /**< (__UINT8__) Configure PCIe Swing Mode.
                                     *  When set on a single port this will apply to all ports on the PCIe Controller.
                                     *  Values for PP_SWING_MODE cannot be mixed on the same PCIe controller.
                                     *  @li Valid Values: See __PCIE_SWING_MODE_TYPE__
                                     */
} MPIO_PCIe_PORT_PARAM_TYPE;

/**
 * @brief  PCIe link initialization
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */
typedef enum {
  MpioPortDisabled,                                           ///< Disable
  MpioPortEnabled                                             ///< Enable
} MPIO_PORT_ENABLE;

/**
 * @brief  PCIe ClkPmSupport initialization
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */
typedef enum {
  MpioClkPmSupportDisabled,                                   ///< Disable
  MpioClkPmSupportEnabled                                     ///< Enable
} MPIO_CLKPM_SUPPORT_ENABLE;

typedef struct {                                    ///< Structure that describe lane mapping
  uint8_t              Lane0   :2;                  /**< Lane 0 mapping
                                                     *  @li @b 0 - Map to lane 0
                                                     *  @li @b 1 - Map to lane 1
                                                     *  @li @b 2 - Map to lane 2
                                                     *  @li @b 2 - Map to lane 3
                                                     */
  uint8_t              Lane1   :2;                  ///< Lane 1 mapping (see "Lane 0 mapping")
  uint8_t              Lane2   :2;                  ///< Lane 2 mapping (see "Lane 0 mapping")
  uint8_t              Lane3   :2;                  ///< Lane 3 mapping (see "Lane 0 mapping")
} MPIO_CHANNEL_MAPPING;

/**
 * @brief Common channel mapping
 * @details
 */
typedef union {
  uint8_t                ChannelMappingValue;       ///< Raw lane mapping
  MPIO_CHANNEL_MAPPING      ChannelMapping;         ///< Channel mapping
} MPIO_CONN_CHANNEL_MAPPING;

/**
 * @brief DDI configuration data
 * @details
 */
typedef struct  {
  uint8_t                ConnectorType;             /**< Display Connector Type
                                                     *  @li @b 0 - DP
                                                     *  @li @b 1 - eDP
                                                     *  @li @b 2 - Single Link DVI-D
                                                     *  @li @b 3 - Dual  Link DVI-D (see @ref DualLinkDviDescription "Example Dual Link DVI connector description")
                                                     *  @li @b 4 - HDMI
                                                     *  @li @b 5 - DP-to-VGA
                                                     *  @li @b 8 - Single Link DVI-I
                                                     *  @li @b 11 - Auto detect LCD panel connector type.
                                                     *              The auto detection method only supports panels with EDID.
                                                     */
  uint8_t                AuxIndex;                  /**< Indicates which AUX or DDC Line is used
                                                     *  @li @b 0 - AUX1
                                                     *  @li @b 1 - AUX2
                                                     *  @li @b 2 - AUX3
                                                     *  @li @b 3 - AUX4
                                                     *  @li @b 4 - AUX5
                                                     *  @li @b 5 - AUX6
                                                     */
  uint8_t                HdpIndex;                  /**< Indicates which HDP pin is used
                                                     *  @li @b 0 - HDP1
                                                     *  @li @b 1 - HDP2
                                                     *  @li @b 2 - HDP3
                                                     *  @li @b 3 - HDP4
                                                     *  @li @b 4 - HDP5
                                                     *  @li @b 5 - HDP6
                                                     */
  MPIO_CONN_CHANNEL_MAPPING Mapping[2];             /**< Set specific mapping of lanes to connector pins
                                                     *  @li Mapping[0] define mapping for group of 4 lanes starting at PCIe_ENGINE_DATA.StartLane
                                                     *  @li Mapping[1] define mapping for group of 4 lanes ending at PCIe_ENGINE_DATA.EndLane (only applicable for Dual DDI link)
                                                     *  if Mapping[x] set to 0 than default mapping assumed
                                                     */
  uint8_t                LanePnInversionMask;       /**< Specifies whether to invert the state of P and N for each lane. Each bit represents a PCIe lane on the DDI port.
                                                     *  @li 0 - Do not invert (default)
                                                     *  @li 1 - Invert P and N on this lane
                                                     */
  uint8_t                Flags;                     /**< Capabilities flags
                                                     *  @li Flags bit[0] DDI_DATA_FLAGS_DP1_1_ONLY Selects downgrade PHY link to DP1.1
                                                     *  @li Flags bit[7:1] Reserved
                                                     */
} MPIO_DDI_DATA;

typedef struct {
  uint32_t          Flags;                    /**< Descriptor flags
                                               * @li @b Bit31 - last descriptor in complex
                                               */
  MPIO_DDI_DATA     Ddi;                      ///< DDI port specific configuration info
} MPIO_DDI_DESCRIPTOR;

/**
 * @cond INT
 * @brief PCIe Complex descriptor
 * @details
 */
typedef struct {
  uint32_t               Flags;                    /**< Descriptor flags
                                                    * @li @b Bit31 - last descriptor in topology
                                                    */
  uint32_t               SocketId;                 ///< Socket Id
  MPIO_PORT_DESCRIPTOR   *PciePortList;            ///< Pointer to array of PCIe port descriptors or NULL (Last element of array must be terminated with DESCRIPTOR_TERMINATE_LIST).
  MPIO_DDI_DESCRIPTOR    *DdiLinkList;             ///< Reserved for compatibility with FM15
  void                   *Reserved2;               ///< Reserved for future use
  uint8_t                BmcLinkLocation;          ///< Identifies the socket/die location of a BMC link (Used by openSIL, input not required)
  uint8_t                BmcLinkLaneNum;           ///< Identifies the socket/die location of a BMC Lane number
  uint8_t                Reserved3[2];             ///< Reserved for future
} MPIO_COMPLEX_DESCRIPTOR;

#pragma pack(pop)
