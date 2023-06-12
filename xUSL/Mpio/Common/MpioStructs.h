/**
 *  @file MpioStructs.h
 *  @brief MPIO data structures
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilPcie.h>
#include <NBIO/PciStructs.h>
#include <NBIO/NbioIp2Ip.h>

#include <Pci.h>
#include <ApobCmn.h>


#pragma pack(push,1)

// @todo: Warning 4100 'unreferenced formal parameter' shows on underdeveloped functions where input  argument is not
// used in the function. Remove the following #pragma when the code is mature and all arguments in all MPIO-DXIO
// functions are effectively used.
#pragma warning( disable : 4100 )

#define MPIOTEMP_MAX_SIZE_OVERRIDE_TABLE 256
#define PCIE_PORT_PARAMETER_COUNT  64

/**
 * @brief MPIO Ancillary Data Subheap Type Structure
 * @details
 */
typedef struct {
  uint32_t          descriptorType  : 8 ;
  uint32_t          version         : 4 ;
  uint32_t          reserved1       : 4 ;
  uint32_t          numDWORDs       : 8 ;
  uint32_t          reserved2       : 8 ;
} MPIO_ANCILLARYDATA_SUBHEAP;

/**
 * @brief MPIO Ancillary Data Subheap Type Structure
 * @details
 */
typedef struct {
  uint32_t          descriptorType : 8 ;
  uint32_t          version        : 4 ;
  uint32_t          reserved1      : 4 ;
  uint32_t          numDWords      : 8 ;
  uint32_t          reserved2      : 8 ;
} MPIO_ANCILLARYDATA;

/**
 * @brief Engine descriptor type
 * @details
 */
typedef enum {
  MpioUnusedEngine = 0,                                   ///< Unused descriptor Excluded from configuration
  MpioPcieEngine = 1,                                     ///< PCIe port
  MpioUSBEngine = 2,                                      /**< USB port
                                                           *   @li __Deprecated__
                                                           */
  MpioSATAEngine = 3,                                     ///< SATA
  MpioUSB_OVER_PCIE = 4,                                  //   USB4 PCIe (internal use only)
  MpioUBMHFCEngine = 5,                                   ///< New for Genoa UBM HFC Connector for auto-discovery
  MpioOCP3Engine = 6,                                     ///< New for Genoa OCP3 Bifurcatable Connector
  MpioUdot3Engine = 7,                                    ///< New for Genoa U.3 Multiprotocol Connector
  MpioDPEngine = 8,                                       ///< Digital Display __For APU display connector list__
  MpioEthernetEngine = 0x10,                              /**< Ethernet (GBe)
                                                           *   @li __Deprecated__
                                                           */
  MpioGOPEngine = 0x20,                                   /**< GOP
                                                           *   @li __Deprecated__
                                                           */
  MpioNTBDevice = 0x60,                                   //  For NBIF NTB Enable (internal use only)
  MpioHDaudioEngine,                                      //  For NBIF HDaudtio Enable (internal use only)
  MpioACPEngine,                                          //  For NBIF ACP Enable (internal use only)
  MpioMP2Engine,                                          //  For NBIF MP2 Enable (internal use only)
  MpioMaxPcieEngine                                       ///< Max engine type for boundary check.
} MPIO_ENGINE_TYPE;

///MPIO Platform Descriptor
typedef struct {
  uint32_t          PlatformType:16;              ///< PlatformType
  uint32_t             Reserved4:16;              ///< Reserved4
  uint32_t             Reserved3:32;              ///< Reserved3
  uint32_t             Reserved2:32;              ///< Reserved2
  uint32_t         NumEngineDesc:16;              ///< NumEngineDesc
  uint32_t             Reserved1:16;              ///< Reserved1
} MPIO_PLATFORM_DESCRIPTOR;

typedef struct {
  PCI_ADDR                  EarlyTrainAddress;  ///< PCI address of the early train port
  PCIe_ENGINE_DESCRIPTOR    *EarlyTrainDesc;    ///< Pointer to the early train MPIO descriptor
  PCIe_ENGINE_CONFIG        *EarlyTrainEngine;  ///< Pointer to the early train engine
  bool                   EarlyLinkMapped;    ///< Early Train status
} MPIO_EARLY_TRAIN_DATA;

#define MPIO_MSG_INIT_DMAENGINECONIFGTODRAM     0x14

#define  MPIO_ENGINE_DATA_INITIALIZER(mType, mStartLane, mEndLane, mHotplug, mGpioGroupId) \
  { .EngineType = mType, \
    .HotPluggable = mHotplug, \
    .StartLane = mStartLane, \
    .EndLane = mEndLane, \
    .GpioGroupId = mGpioGroupId, \
  }

#define  MPIO_PORT_DATA_INITIALIZER_PCIE(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, \
                                         mMaxLinkCap, mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport) \
  { \
    .PortPresent = mPortPresent, \
    .DeviceNumber = mDevAddress, \
    .FunctionNumber = mDevFunction, \
    .LinkSpeedCapability = mMaxLinkSpeed, \
      .LinkAspm = mAspm, \
      .LinkAspmL1_1 = mAspmL1_1, \
      .LinkAspmL1_2 = mAspmL1_2, \
      .LinkHotplug = mHotplug, \
    .MiscControls = { \
      .LinkSafeMode = mMaxLinkCap, \
      .ClkPmSupport = mClkPmSupport, \
      .TurnOffUnusedLanes = 1, \
    }, \
    /* .EndpointStatus = MpioEndpointDetect, */ \
  }

//
// C2PMSG_97  (SMN address 0x3810A84)
//
typedef union {
  struct {
    uint32_t Environment:4;                  ///< Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite.  Replaces UserDataPattern0 force
    uint32_t CoreClNum:3;                    ///< Core CL selector: Varies from release to release.  Replaces DF scratch 30[23:0] force
    uint32_t DisablePmuDevinitUseSwmra:1;    ///< Disable DEVINIT and use SWMRA instead. Replaces UmcCfgEco[19] force
    uint32_t Reserved:1;                     ///< Reserved bit
    uint32_t SkipAllUSBControllerAccess:1;   ///< When set FW will skip all USB Control init communication from the FW
    uint32_t SkipAllUSBPhyAccess:1;          ///< When set FW will skip all USB phy communication from the FW
    uint32_t SkipMpioInitializaion:1;        ///<When set FW will skip MPIO Initialization from the FW>
    uint32_t HspDisabled:1;                  ///< HSP disabled in FW
    uint32_t ForceMpmMeventPath:1;           ///< Set to 1 to force MPM M-event path (X86 not released)
    uint32_t :18;                            ///< Reserved
  } Field;
  uint32_t  Value;
} MPIO_PRESIL_CTRL0;


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
  uint32_t          ChannelType: 8;              ///< ChannelType
  uint32_t        ChannelDescID: 8;              ///< ChannelDescID
  uint32_t  AncillaryDataOffset:16;              ///< Reserved3
  uint32_t         DevFunNumber:32;              ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t                MacID: 8;              ///< MacID
  uint32_t            MacPortID: 8;              ///< MacPortID
  uint32_t        StartCoreLane: 8;              ///< StartCoreLane
  uint32_t          EndCoreLane: 8;              ///< EndCoreLane
  uint32_t                PcsID: 8;              ///< PcsID
  uint32_t           Reserved1: 24;              ///< Reserved1
} MPIO_LINK_DESC_BASE;

///MPIO Link Descriptor Eth
typedef struct {
  uint32_t          ChannelType: 8;              ///< ChannelType
  uint32_t            Reserved1: 8;              ///< Reserved1
  uint32_t  AncillaryDataOffset:16;              ///< AncillaryDataOffset
  uint32_t         DevFunNumber:32;              ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t            Reserved2:32;              ///< Reserved2
  uint32_t            Reserved3:32;              ///< Reserved3
} MPIO_LINK_DESC_ETH;

///MPIO Link Descriptor PCIe
typedef struct {
  uint32_t          ChannelType: 8;              ///< ChannelType
  uint32_t        ChannelDescID: 8;              ///< ChannelDescID
  uint32_t  AncillaryDataOffset:16;              ///< Reserved3
  uint32_t         DevFunNumber:32;              ///< DevFunNumber
  MPIO_LINK_CAP     Capabilities;              ///< Capabilities
  uint32_t                MacID: 8;              ///< MacID
  uint32_t            MacPortID: 8;              ///< MacPortID
  uint32_t        StartCoreLane: 8;              ///< StartCoreLane
  uint32_t          EndCoreLane: 8;              ///< EndCoreLane
  uint32_t                PcsID: 8;              ///< PcsID
  uint32_t    linkTrainingState: 8;              ///< linkTrainingState
  uint32_t           Reserved1: 16;              ///< Reserved1
} MPIO_LINK_DESC_PCIE;

typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_SATA;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_USB;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_DP;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_GOP;
typedef MPIO_LINK_DESC_BASE   MPIO_LINK_DESC_GMI;

///MPIO Link Descriptor
typedef union {
  MPIO_LINK_DESC_BASE   Base;              ///< MPIO Link Descriptor Base
  MPIO_LINK_DESC_PCIE   Pcie;              ///< MPIO Link Descriptor PCIe
  MPIO_LINK_DESC_SATA   Sata;              ///< MPIO Link Descriptor Sata
  MPIO_LINK_DESC_USB    Usb;               ///< MPIO Link Descriptor Usb
  MPIO_LINK_DESC_DP     Dp;                ///< MPIO Link Descriptor Dp
  MPIO_LINK_DESC_ETH    Eth;               ///< MPIO Link Descriptor Eth
  MPIO_LINK_DESC_GOP    Gop;               ///< MPIO Link Descriptor Gop
  MPIO_LINK_DESC_GMI    Gmi;               ///< MPIO Link Descriptor Gmi
} MPIO_LINK_DESC;

///MPIO Engine Descriptor
typedef struct {
  uint32_t           EngineType: 8;              ///< EngineType
  uint32_t         HotPluggable: 1;              ///< HotPluggable
  uint32_t            Reserved3: 7;              ///< Reserved3
  uint32_t            StartLane: 8;              ///< StartLane
  uint32_t              EndLane: 8;              ///< EndLane
  uint32_t          GpioGroupID: 8;              ///< GpioGroupID
  uint32_t        engineResetID: 8;              ///< engineResetID
  uint32_t         searchDepth : 1;              ///< searchDepth
  uint32_t      forceKpnpReset : 1;              ///< forceKpnpReset
  uint32_t            Reserved2:14;              ///< Reserved2
  MPIO_LINK_DESC        LinkDesc;              ///< LinkDesc
  uint32_t         MacEnginePtr:16;              ///< MacEnginePtr
  uint32_t          FirstLgdNum: 8;              ///< FirstLgdNum
  uint32_t           LastLgdNum: 8;              ///< LastLgdNum
  uint32_t        TrainingState: 4;              ///< TrainingState
  uint32_t            Reserved1:28;              ///< Reserved1
} MPIO_ENGINE_DESCRIPTOR;

/**
 * @brief High level data structures
 * @details Engine Configuration
 */
typedef struct {
  uint8_t                EngineType;               /**< Engine type
                                                           *  @li @b 0 -  Ignore engine configuration
                                                           *  @li @b 1 -  PCIe port
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
                                                           * type (e.g SATA Express or PCIe)
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
                                                                 *  @li @b 0 - port can advertize muximum supported capability
                                                                 *  @li @b 1 - port limit advertized capability and speed to PCIe Gen1
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
  uint8_t                     TurnOffUnusedLanes     :1;   /**< Turn Off Unused Lanes
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
  uint8_t                   PortPresent         :1 ;   ///< Enable PCIe port for initialization.
  uint8_t                   Reserved1           :2 ;   ///< Reserved
  uint8_t                   DeviceNumber        :5 ;   /**< PCI Device number for port.
                                                               *   @li @b 0 - Native port device number
                                                               *   @li @b N - Port device number (See available configurations in BKDG
                                                               */
  uint8_t                   FunctionNumber      :3 ;   ///< Reserved for future use
  uint8_t                   LinkSpeedCapability :3 ;   /**< PCIe link speed/
                                                               *  @li @b 0 - Maximum supported by silicon
                                                               *  @li @b 1 - Gen1
                                                               *  @li @b 2 - Gen2
                                                               *  @li @b 3 - Gen3
                                                               *  @li @b 4 - Gen4
                                                               *  @li @b 5 - Gen5
                                                               */
  uint8_t                   AutoSpdChng         :2 ;   /**< Upstread Auto Speed Change Allowed/
                                                               *  @li @b 0 - Use default implementation (Disabled for Gen1, Enabled for Gen2/3)
                                                               *  @li @b 1 - Always Disabled
                                                               *  @li @b 2 - Always Enabled
                                                               *  @li @b 3 - Reserved
                                                               */
  uint8_t                   EqPreset            :4 ;   /**< Gen3 Equalization Preset */
  uint8_t                   LinkAspm            :2 ;   /**< ASPM control. */
  uint8_t                   LinkAspmL1_1        :1 ;   /**< ASPM control. */
  uint8_t                   LinkAspmL1_2        :1 ;   /**< ASPM control. */
  uint8_t                   ClkReq              :4 ;   /**< ASPM Reserved Field
                                                               *  @li @b 0 - NONE
                                                               *  @li @b 1 - CLKREQ0 signal
                                                               *  @li @b 2 - CLKREQ1 signal
                                                               *  @li @b 3 - CLKREQ2 signal
                                                               *  @li @b 4 - CLKREQ3 signal
                                                               *  @li @b 5 - CLKREQG signal
                                                               */
  uint8_t                   LinkHotplug         :4 ;   /**< Hotplug control.
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Basic
                                                               *  @li @b 2 - Server
                                                               *  @li @b 3 - Enhanced
                                                               */
  uint8_t                   SlotPowerLimit;           ///< PCIe slot power limit.
  uint8_t                   SlotPowerLimitScale :2 ;  /**< PCIe slot power limit Scale.
                                                               *  @li @b 00b = 1.0x
                                                               *  @li @b 01b = 0.1x
                                                               *  @li @b 10b = 0.01x
                                                               *  @li @b 11b = 0.001x
                                                               */
  uint8_t                   IsMasterPLL         :1 ;  ///< IsMasterPLL
  uint8_t                   Gen4Features        :5 ;  /**< Unused bits
                                                               *< BIT0(DLF_Exchange) 1 - Disable, 0 - Enable
                                                               *< BIT1(DLF_Capability) 1 - Disable, 0 - Enable
                                                               */
  uint16_t                  SlotNum             :13;  ///< PHYSICAL_SLOT_NUM
  uint16_t                  CsLink              :3 ;  ///< Reserved
  MPIO_PORT_MISC_CONTROL  MiscControls;             ///< Misc extended controls
  MPIO_APIC_DEVICE_INFO   ApicDeviceInfo;           ///< IOAPIC device programming info
  //MPIO_ENDPOINT_STATUS    EndpointStatus;           ///< PCIe endpoint (device connected to PCIe port) status
  uint8_t                   EsmSpeedBump;             ///< Speed bump for ESM
  uint8_t                   EsmControl          :1 ;  ///< Enhanced speed mode control
  uint8_t                   TxVetting           :1;   ///< Tx Vetting
  uint8_t                   RxVetting           :1;   ///< Rx Vetting
  uint8_t                   InvertPolarity      :1;   ///< Invert RX Polarity
  uint8_t                   InvertPolarity2     :1;   ///< Invert TX Polarity
  uint8_t                   NtbHotplug          :1;   /**< NTB Hotplug flag
                                                              *  @li @b 0b = Disabled
                                                              *  @li @b 1b = Enabled
                                                              */
  uint8_t                   Reserved2           :1;   ///< Reserved
  uint8_t                   SetGen3FixedPreset  :1;   ///< Gen3 Fixed Preset Set
  uint8_t                   SetGen4FixedPreset  :1;   ///< Gen4 Fixed Preset Set
  uint8_t                   SetGen5FixedPreset  :1;   ///< Gen5 Fixed Preset Set
  uint8_t                   Reserved3           :5;   ///< Reserved
  uint8_t                   Gen3FixedPreset     :4;   ///< Gen3 Fixed Preset
  uint8_t                   Gen4FixedPreset     :4;   ///< Gen4 Fixed Preset
  uint8_t                   Gen5FixedPreset     :4;   ///< Gen5 Fixed Preset
  uint8_t                   Reserved4           :4;   ///< Reserved
  uint16_t                  PsppPolicyDC        ;     ///< Pspp DC control
  uint16_t                  PsppPolicyAC        ;     ///< PSPP AC control
  uint8_t                   PsppDeviceType      ;     ///< Pspp Device Type
  // uint8_t                   NtbHotplug          :1;   /**< NTB Hotplug flag
  //                                                             *  @li @b 0b = Disabled
  //                                                             *  @li @b 1b = Enabled
  //                                                             */
  uint8_t                   DisGen3EQPhase      :1;   ///< Gen3 Bypass phase2/3 EQ
  uint8_t                   DisGen4EQPhase      :1;   ///< Gen4 Bypass phase2/3 EQ
  uint8_t                   TXDeEmphasisOride   :1;   ///< Override Gen2 MPIO deemphasis default
  uint8_t                   TXDeEmphasis        :2;   ///< Gen2 MPIO deemphasis setting
  uint8_t                   SetGen3LaneEq       :1;   ///< Gen3 Lane Equalization Set
  uint8_t                   SetGen4LaneEq       :1;   ///< Gen4 Lane Equalization Set
  uint8_t                   SetGen5LaneEq       :1;   ///< Gen5 Lane Equalization Set
  struct {
    uint16_t                  DsTxPreset          :4;   ///< Gen3 Downstream Tx Preset
    uint16_t                  DsRxPresetHint      :3;   ///< Gen3 Downstream Rx Preset Hint
    uint16_t                  UsTxPreset          :4;   ///< Gen3 Upstream Tx Preset
    uint16_t                  UsRxPresetHint      :3;   ///< Gen3 Upstream Rx Preset Hint
    uint16_t                  Reserved1           :2;   ///< Unused bits
  } LaneEqualizationCntl;                                    ///< Lane equalization control structure used for Gen3 values
  struct {
    uint8_t                   DsTxPreset          :4;   ///< Gen4 Downstream Tx Preset
    uint8_t                   UsTxPreset          :4;   ///< Gen4 Upstream Tx Preset
  } Gen4LaneEqualizationCntl;                                ///< Lane equalization control structure used for Gen4 values
  struct {
    uint8_t                   DsTxPreset          :4;   ///< Gen5 Downstream Tx Preset
    uint8_t                   UsTxPreset          :4;   ///< Gen5 Upstream Tx Preset
  } Gen5LaneEqualizationCntl;                           ///< Lane equalization control structure used for Gen5 values
   struct {
    uint32_t                  PresetMask8Gt       :10;  ///< Preset Mask 8GT.
    uint32_t                  PresetMask16Gt      :10;  ///< Preset Mask 16GT.
    uint32_t                  PresetMask32Gt      :10;  ///< Preset Mask 32GT.
    uint32_t                  Reserved1           :2;   ///< Unused bits
    uint8_t                   SetPresetMask8Gt    :1;   ///< Preset Mask 8GT Set
    uint8_t                   SetPresetMask16Gt   :1;   ///< Preset Mask 16GT Set
    uint8_t                   SetPresetMask32Gt   :1;   ///< Preset Mask 32GT Set
    uint8_t                   Reserved2           :5;   ///< Unused bits
  } PresetMaskCntl;
  uint8_t                   TargetLinkSpeed     :3;   ///< Target Link Speed
  uint8_t                   BypassGen3EQ        :1;   ///< Bypass Gen3 equalization
  uint8_t                   BypassGen4EQ        :1;   ///< Bypass Gen4 equalization
  uint8_t                   AlwaysExpose        :1;   ///< Always expose unused PCIE port
  uint8_t                   SrisSkipInterval    :2;   ///< Controls SRIS SKP generation interval
  uint8_t                   SrisEnableMode      :4;   ///< 0:Disable 1:Enable 0xF:Auto
  uint8_t                   SrisAutoDetectMode  :4;   ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  uint8_t                   LowerSkpOsGenSup    ;     ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  uint8_t                   LowerSkpOsRcvSup    ;     ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  uint8_t                   SrisSkpIntervalSel  :2;   ///< Controls SRIS SKIP Interval Selection Mode
  uint8_t                   SrisAutodetectFactor:2;   ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  uint8_t                   IsBmcLocation       :1;   ///< IsBmcLocation
  uint8_t                   Reserved5           :3;   ///< Unused bits
  uint8_t                   I2CMuxInfo          :6;   ///< Legacy I2c switch
  uint8_t                   Reserved6           :2;   ///< Unused bits
  uint16_t                  NpemEnable          :12;  ///< Controls NPEM Enable
  uint16_t                  Reserved7           :4;   ///< Unused bits
  uint16_t                  NpemCapability      :12;  ///< Controls NPEM Capability
  uint16_t                  Reserved8           :4;   ///< Unused bits
  uint16_t                  MpioAncDataIdx      ;     ///< Reserved for internal use only
  uint8_t                   Reserved9           ;     ///< Reserved bits
} MPIO_PORT_DATA;


typedef struct  {
  uint32_t               PortNum           :8;     ///< Port Number
  uint32_t               PlatConf          :4;     /**< Platform Config
                                                            *  0 = Reserved
                                                            *  1 = 10G/1G BackPlane
                                                            *  2 = 2.5G BackPlane
                                                            *  3= Soldered down 1000Base-T
                                                            *  4 = Soldered down 1000Base-X
                                                            *  5 = Soldered down NBase-T
                                                            *  6 = Soldered down 10GBase-T
                                                            *  7 = Soldered down 10GBase-r
                                                            *  8 = SFP+ Connector
                                                            */
  uint32_t               Reserved1         :4;     ///< Unused 12-15
  uint32_t               MdioId            :5;     ///< MDIO ID when MDIO Side band is used
  uint32_t               Reserved2         :2;     ///< Unused 21-22
  uint32_t               SuppSpeed         :4;     /**< Supported Speeds by Platform
                                                            *  1 = 100M Supported
                                                            *  2 = 1G Supported
                                                            *  4 = 2.5G Supported
                                                            *  8 = 10G Supported
                                                            */
  uint32_t               Reserved3         :1;     ///< Unused 27
  uint32_t               ConnType          :3;     /**< Supported Speeds by Platform
                                                            *  0 = Port not Used
                                                            *  1 = SFP+ Connection I2C interface
                                                            *  2 = MDIO PHY
                                                            *  4 = Backplane Connection
                                                            */
  uint32_t               Reserved4         :1;     ///< Unused 31
} MPIO_ETH_PORT_PROPERTY0;

typedef struct  {
  uint32_t               MdioReset         :2;     /**< MDIO Reset Type
                                                            *  0 = None
                                                            *  1 = I2C GPIO
                                                            *  2 = Integrated GPIO
                                                            *  3 = Reserved
                                                            */
  uint32_t               Reserved1         :2;     ///< Unused 2-3
  uint32_t               MdioGpioResetNum  :2;     ///< Integrated GPIO number for reset
  uint32_t               Reserved2         :2;     ///< Unused 6-7
  uint32_t               SfpGpioAdd        :3;     ///< Lower I2C address of GPIO Expander PCA9535
  uint32_t               Reserved3         :1;     ///< Unused 11
  uint32_t               TxFault           :4;     ///< TX FAULT
  uint32_t               Rs                :4;     ///< RS Signal
  uint32_t               ModAbs            :4;     ///< MOD_ABS signal
  uint32_t               RxLoss            :4;     ///< Rx_LOS signal
  uint32_t               SfpGpioMask       :4;     ///< SFP+ sideband signals that are not supported by platform
} MPIO_ETH_PORT_PROPERTY3;

typedef struct  {
  uint32_t               SfpMux            :3;     ///< Lower address of Mux PCA 9545
  uint32_t               Reserved1         :1;     ///< Unused 3
  uint32_t               SfpBusSeg         :3;     ///< SFP BUS Segment. Downstream channels of PCA9545
  uint32_t               Reserved2         :1;     ///< Unused 7
  uint32_t               SfpMuxUpAdd       :5;     ///< Upper address of Mux PCA 9545
  uint32_t               Reserved3         :3;     ///< Unused 13-15
  uint32_t               RedriverAddress   :7;     ///< Address of ReDriver
  uint32_t               RedriverInterface :1;     ///< ReDriver Interface Descriptor
  uint32_t               RedriverLane      :3;     ///< ReDriver Lane number
  uint32_t               Reserved4         :1;     ///< Unused 27
  uint32_t               RedriverModel     :3;     ///< ReDriver Model
  uint32_t               RedriverPresent   :1;     ///< Redriver Present
} MPIO_ETH_PORT_PROPERTY4;

typedef struct  {
  uint32_t               TxEqPre           :6;     ///< TX EQ PRE
  uint32_t               Reserved1         :2;     ///< Unused 7-6
  uint32_t               TxEqMain          :6;     ///< TX EQ MAIN
  uint32_t               Reserved2         :2;     ///< Unused 15-14
  uint32_t               TxEqPost          :6;     ///< TX EQ POST
  uint32_t               Reserved3         :10;    ///< Unused 31-23
} MPIO_ETH_PORT_TXEQ;

/**
 * @brief Ethernet PCIe port configuration data
 * @details
 */
typedef struct  {
  MPIO_ETH_PORT_PROPERTY0   EthPortProp0;
  MPIO_ETH_PORT_PROPERTY3   EthPortProp3;
  MPIO_ETH_PORT_PROPERTY4   EthPortProp4;
  uint32_t               PadMux0;                  ///< PadMux0 Setting (8 bits)
  uint32_t               PadMux1;                  ///< PadMux1 Setting (8 bits)
  uint32_t               MacAddressLo;             ///< Lower 32 bits of MAC Address
  uint32_t               MacAddressHi;             ///< Upper 32 bits of MAC Address
  MPIO_ETH_PORT_TXEQ        EthPortTxEq;              ///< TX EQ Settings
} MPIO_ETHERNET_PORT_DATA;

typedef struct {
  uint16_t               ParamType;      ///< This identifies a specific PHY parameter
  uint16_t               ParamValue;     ///< This specifies the value to be assigned to indicated PHY parameter
} MPIO_PHY_PARAM;

typedef struct {
  MPIO_PHY_PARAM       PhyParam [44];      ///< physical parameter
} MPIO_PHY_DATA;

typedef struct {
  uint32_t         size;
  uint8_t          data[MPIOTEMP_MAX_SIZE_OVERRIDE_TABLE];
} MPIO_PHYOVR_TABLE;

typedef struct {
  uint16_t               ParamType;                ///< This identifies a specific port parameter to set.
  uint16_t               ParamValue;               ///< Specifies the value to be assigned to port parameter.
} MPIO_PORT_PARAM;

typedef struct {
  MPIO_PORT_PARAM           PhyParam [PCIE_PORT_PARAMETER_COUNT];       ///< PHY port parameter
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
                                              *  Bits [ 0: 2] Enable Mask: This mask allows the user to selectively override only some attributes.
                                              *               Bit0: Enable C0 override. Bit1: Enable Cn Override. Bit2: Enable Cp override.
                                              *  Bits [ 8:15] C0 override value.
                                              *  Bits [16:23] Cn override value.
                                              *  Bits [24:31] Cp override value.
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
  uint32_t               Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in complex
                                                           */
  MPIO_ENGINE_DATA     EngineData;               ///< Engine data
  MPIO_PORT_DATA       Port;                     ///< PCIe port specific configuration info
  MPIO_ETHERNET_PORT_DATA   EtherNet;            ///< Ancillary data for EtherNet
  MPIO_PHY_DATA        Phy;                      ///< Ancillary data for PHY programming customization
  MPIO_PORT_PARAMS     PortParams;               ///< Extensible port parameter list for simplified topology structure
  MPIO_ANC_DATA        AncData;                  ///< Ancillary data override
} MPIO_PORT_DESCRIPTOR;

///Port information needed for lane translation callback
typedef struct {
  uint8_t      StartLane;          ///< Start Lane from User Config
  uint8_t      EndLane;            ///< End Lane from User Config
  uint8_t      MpioStartLane;      ///< Translated Start Lane
  uint8_t      MpioEndLane;        ///< Translated End Lane
  bool         LinkReversal;       ///< Link Reversal should be set
  bool         LanesFound;         ///< Flag for lanes found
  uint8_t                   SocketNumber;       ///< SocketNumber
  uint8_t                   EngineID;           ///< Index of EngineID
  uint8_t                   MacPortID;          ///< MacPortID from MPIO firmware
  MPIO_PORT_DESCRIPTOR    *UserCfgEntry;      ///< Pointer into UserCfgPortList
  MPIO_ENGINE_DESCRIPTOR  *MpioEntry;         ///< Pointer into MPIO Engine Config
  uint8_t                   *PortDevMap;        ///< Pointer to array of port device mapping
  PCIe_ENGINE_CONFIG      *EarlyTrainEngine;  ///< Pointer to early train engine
} MPIO_PORT_INFO_CALLBACK;

/**
 *  @brief Port parameter override enumerator.
 *  @details This enumerator provides a parmeter type for platform topology override values.
 */
typedef enum {
  MPIO_PP_DEVICE = 1             ,  /**< Request a specific device number for the root port
                                *  This should be a valid PCI device number within the default
                                *  range of the PCIe root ports for this program, typically 1-4.
                                */
  MPIO_PP_FUNCTION               ,  /**< Request a specific function number for the root port
                                *  This should be a valid PCI device number within the default
                                *  range of the PCIe root ports for this program, typically 1-7.
                                */
  MPIO_PP_PORT_PRESENT           ,  ///< Define whether training should be attempted.
  MPIO_PP_LINK_SPEED_CAP         ,  /**< Limit the maximum link speed supported
                                *  @li @b 0 - Maximum supported by silicon
                                *  @li @b 1 - Gen1
                                *  @li @b 2 - Gen2
                                *  @li @b 3 - Gen3
                                *  @li @b 4 - Gen4
                                *  @li @b 5 - Gen5
                                */
  MPIO_PP_LINK_ASPM              ,  /**< Define the ASPM capability.
                                *  @li @b 0 - MpioAspmDisabled - Disabled
                                *  @li @b 1 - MpioAspmL0s - PCIe L0s link state
                                *  @li @b 2 - MpioAspmL1 - PCIe L1 link state
                                *  @li @b 3 - MpioAspmL0sL1 - PCIe L0s & L1 link state
                                */
  MPIO_PP_HOTPLUG_TYPE           ,  /**< Assign a hotplug type.
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
  MPIO_PP_CLKREQ                 ,  /**< Specify a clock request line.
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
  MPIO_PP_ASPM_L1_1              ,  ///< (__BOOLEAN__) Define the L1.1 substate capability.
  MPIO_PP_ASPM_L1_2              ,  ///< (__BOOLEAN__) Define the L1.2 substate capability.
  MPIO_PP_COMPLIANCE             ,  ///< (__BOOLEAN__) Request compliance mode.
  MPIO_PP_SAFE_MODE              ,  ///< (__BOOLEAN__) Request safe mode.
  MPIO_PP_CHIPSET_LINK           ,  ///< (__BOOLEAN__) Identify this as the link connected to the chipset.
  MPIO_PP_CLOCK_PM               ,  ///< (__BOOLEAN__) Enable clock power management.
  MPIO_PP_CHANNELTYPE            ,  /**< Specify a SATA channel type.
                                *  @li __SATA ONLY__
                                */
  MPIO_PP_TURN_OFF_UNUSED_LANES  ,  ///< (__BOOLEAN__) Enable powerdown of unused lanes.
  MPIO_PP_APIC_GROUPMAP          ,  ///< Override the IOAPIC group map assignment.
  MPIO_PP_APIC_SWIZZLE           ,  ///< Override the IOAPIC swizzling.
  MPIO_PP_APIC_BRIDGEINT         ,  ///< Override the IOAPIC bridge interrupt.
  MPIO_PP_MASTER_PLL             ,  ///< (__BOOLEAN__) Associate this port as the master PLL.
  MPIO_PP_SLOT_NUM               ,  ///< Specify a SLOT NUMBER value.
  MPIO_PP_PHY_PARAM              ,  ///< Specifies a PHY sub-parameter. __SEE MPIO_PHY_PARAM_TYPE__
  MPIO_PP_ESM                    ,  ///< Enable support for Enhanced Speed Mode.
  MPIO_PP_CXL                    ,  ///< Enable the port as CXL capable.
  MPIO_PP_GEN3_DS_TX_PRESET      ,  ///< Gen3 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_DS_RX_PRESET_HINT ,  ///< Gen3 Downstream Rx Preset Hint. __3 bit value__
  MPIO_PP_GEN3_US_TX_PRESET      ,  ///< (__BOOLEAN__) Gen3 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_US_RX_PRESET_HINT ,  ///< Gen3 Upstream Rx Preset Hint. __3 bit value__
  MPIO_PP_GEN4_DS_TX_PRESET      ,  ///< Gen4 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN4_US_TX_PRESET      ,  ///< Gen4 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN5_DS_TX_PRESET      ,  ///< Gen5 Downstream Tx Preset. __4 bit value__
  MPIO_PP_GEN5_US_TX_PRESET      ,  ///< Gen5 Upstream Tx Preset. __4 bit value__
  MPIO_PP_GEN3_FIXED_PRESET      ,  ///< (__BOOLEAN__) Gen3 Fixed Preset.
  MPIO_PP_GEN4_FIXED_PRESET      ,  ///< (__BOOLEAN__) Gen4 Fixed Preset.
  MPIO_PP_GEN5_FIXED_PRESET      ,  ///< (__BOOLEAN__) Gen5 Fixed Preset.
  MPIO_PP_GEN3_PRESET_MASK       ,  ///< Preset Mask 8GT. __10 bit value__
  MPIO_PP_GEN4_PRESET_MASK       ,  ///< Preset Mask 16GT. __10 bit value__
  MPIO_PP_GEN5_PRESET_MASK       ,  ///< Preset Mask 32GT. __10 bit value__
  MPIO_PP_PSPP_DC                ,  /**< Pspp DC control.
                                *   @li __Applicable for client programs__
                                */
  MPIO_PP_PSPP_AC                ,  /**< PSPP AC control.
                                *   @li __Applicable for client programs__
                                */
  MPIO_PP_GEN2_DEEMPHASIS        ,  /**< Specify the deemphasis value for Gen2.
                                *  @li @b TX_DEEMPHASIS_CSR = 0 - Use de-emphasis from CSR
                                *  @li @b TX_DEEMPHASIS_US = 0x1 - Use de-emphasis requested by upstream component
                                *  @li @b TX_DEEMPHASIS_60 = 0x2 - Use -6.0 dB
                                *  @li @b TX_DEEMPHASIS_35 = 0x3 - Use -3.5 dB
                                *  @li @b TX_DEEMPHASIS_DIS = 0xFF - Skip override setting
                                */
  MPIO_PP_INVERT_POLARITY        ,  ///< (__BOOLEAN__) Request inverted polarity.
  MPIO_PP_TARGET_LINK_SPEED      ,  /**< Set the desired operating link speed.
                                *  @li @b 0 - Maximum supported by silicon
                                *  @li @b 1 - Gen1
                                *  @li @b 2 - Gen2
                                *  @li @b 3 - Gen3
                                *  @li @b 4 - Gen4
                                *  @li @b 5 - Gen5
                                */
  MPIO_PP_GEN4_DLF_CAP_DISABLE   ,  ///< (__BOOLEAN__) Disable GEN4 DLF Capability.
  MPIO_PP_GEN4_DLF_EXCHG_DISABLE ,  ///< (__BOOLEAN__) Disable GEN4 DLF Exchange.
  MPIO_PP_I2C_EXPANDER_ADDRESS   ,  /**< (uint8_t) 7-bit I2C Expander Address for server features
                                */
  MPIO_PP_I2C_EXPANDER_TYPE      ,  /**< (uint8_t) I2C Expander Type for server features
                                * @li @b 0 - UBM_GPIO_EXPANDER_TYPE_9539,
                                * @li @b 1 - UBM_GPIO_EXPANDER_TYPE_9535,
                                * @li @b 2 - UBM_GPIO_EXPANDER_TYPE_9506
                                */
  MPIO_PP_I2C_CLEAR_ALL_INTS     ,  /**< (__BOOLEAN__) Clear all interrupts on I2c expanders
                                  < Used to force MPIO to clear all interrupts on an expander in cases where additional
                                  < non-spec'd lines are connected to interrupt generating pins.
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_SWITCH0_ADDR       ,  /**< (uint8_t) 7-bit I2C Address of UBM Switch __(required)__
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_SWITCH0_SELECT     ,  /**< (uint8_t) I2C Select of UBM Switch __(required)__
                                *   Valid values are 0-3 for 9545, 9546
                                *                    0-7 for 9548
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_SWITCH0_TYPE      ,  /**< (uint8_t) I2C Type of UBM Switch __(required)__
                                * @li @b 0 - UBM_GPIO_SWITCH_TYPE_9545,
                                * @li @b 1 - UBM_GPIO_SWITCH_TYPE_9546_48
                                */
  MPIO_PP_UBM_SWITCH1_ADDR       ,  /**< (uint8_t) 7-bit I2C Address of second UBM Switch __(optional)__
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_SWITCH1_SELECT     ,  /**< (uint8_t) I2C Select of second UBM Switch __(optional)__
                                *   Valid values are 0-3 for 9545, 9546
                                *                    0-7 for 9548
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_SWITCH1_TYPE      ,  /**< (uint8_t) I2C Type of UBM Switch __(required)__
                                * @li @b 0 - UBM_GPIO_SWITCH_TYPE_9545,
                                * @li @b 1 - UBM_GPIO_SWITCH_TYPE_9546_48
                                */
  MPIO_PP_UBM_HFC_INDEX          ,  /**< (uint8_t) HFC Index for this port
                                *    NOTE: This value is set in UBM auto-discovery
                                *   Valid values are 0-0x31
                                * @li __New for Genoa__
                                */
  MPIO_PP_UBM_DFC_INDEX          ,  /**< (uint8_t) DFC Index for this port
                                *    NOTE: This value is set in UBM auto-discovery
                                *   Valid values are 0-0xFF
                                * @li __New for Genoa__
                                */
  MPIO_PP_DFC_EVENT              ,  /**< (UINT8) DFC Event for this port
                                *   Valid values are 0 = BIOS_EVENT_DEVICE_DISCONNECTED , 1 = BIOS_EVENT_DEVICE_CONNECTED, 2 = BIOS_EVENT_DEVICE_NOT_PRESENT
                                * @li __New for Genoa__
                                */
  MPIO_PP_GPIOx_I2C_RESET        ,  /**< (uint8_t) Pin number on I2C Expander of UBM reset
                                * @li __New for Genoa__
                                */
  MPIO_PP_GPIOx_BP_TYPE          ,  /**< (uint8_t) Pin number of U.3 IFDET2 pin on I2C Expander
                                * @li __New for Genoa__
                                */
  MPIO_PP_START_LANE             ,  /**< (uint8_t) Starting lane for MPIO discovered ports
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_PRESENT_START      ,  /**< (uint8_t) Pin number on I2C Expander of first OCP PRESENT pin
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_PRESENT_COUNT      ,  /**< (uint8_t) Number of OCP PRESENT pins on I2C Expander
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_DEF_VALID          ,  /**< (UINT8) Default valid configuration for PRSNT pins
                                *   Valid values are 0-1
                                */
  MPIO_PP_OCP_DEF_PRSNTB_PRIMARY ,  /**< (UINT8) Default Primary Connector PRSNTB[3:0]#
                                *   Valid values are 0-0xF
                                */
  MPIO_PP_OCP_DEF_PRSNTB_SECONDARY, /**< (UINT8) Default Secondary Connector PRSNTB[3:0]#
                                *   Valid values are 0-0xF
                                */
  MPIO_PP_OCP_BIF_PRIMARY        ,  ///Primary Connector BIF[2:0]# (SFF,LFF)
  MPIO_PP_OCP_BIF_SECONDARY      ,  ///Secondary Connector BIF[2:0]# (LFF only)
  MPIO_PP_OCP_NUM_HOSTS          ,  /**< (UINT8) Number of Hosts, single, dual , quad
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_NUM_SOCKETS        ,  /**< (UINT8) Number of AMD sockets supporting OCP NIC 3.0 PCIe links, 1 or 2
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_FORM_FACTOR        ,  /**< (UINT8) OCP form factor SFF or LFF
                                * @li __New for Genoa__
                                */
  MPIO_PP_OCP_SLOT_NUM           ,  ///< Specify a SLOT NUMBER value for OCP.
  MPIO_PP_U3_PRESENT_PIN         ,  /**< (uint8_t) Pin number of U.3 PRESENT pin on I2C Expander
                                * @li __New for Genoa__
                                */
  MPIO_PP_U3_IFDET_PIN           ,  /**< (uint8_t) Pin number of U.3 IFDET pin on I2C Expander
                                * @li __New for Genoa__
                                */
  MPIO_PP_U3_IFDET2_PIN          ,  /**< (uint8_t) Pin number of U.3 IFDET2 pin on I2C Expander
                                * @li __New for Genoa__
                                */
  MPIO_PP_ALWAYS_EXPOSE          ,  /**< (__BOOLEAN__) Always Expose Unused PCIE port
                                *  @li @b Disable = 0 - Not exposed
                                *  @li @b Enable = 1 - Always exposed
                                */
  MPIO_PP_SRIS_ENABLED           ,  /**< (uint8_t) SRIS Capability Enabled
                                *  @li @b Disable = 0
                                *  @li @b Enable = 1
                                */
  MPIO_PP_SRIS_SKIP_INTERVAL     ,  /**< (uint8_t) Controls SRIS SKP generation interval
                                *  @li @b 0 - 1506; 144; 6050; 640
                                *  @li @b 1 - 1538; 154; 6068; 656
                                *  @li @b 2 - 1358; 128; 6032; 624
                                *  @li @b 3 - 1180; 112; 5996; 608
                                */
  MPIO_PP_SRIS_LOWER_OS_GEN_SUP  ,  /**< (uint8_t) Controls LOWER_SKP_OS_GEN_SUPPORT
                                *  @li @b 0x0 - Disable
                                *  @li @b 0x1 - Gen1
                                *  @li @b 0x3 - Gen2
                                *  @li @b 0x7 - Gen3
                                *  @li @b 0xF - Gen4
                                */
  MPIO_PP_SRIS_LOWER_OS_RCV_SUP  ,  /**< (uint8_t) Controls LOWER_SKP_OS_RCV_SUPPORT
                                *  @li @b 0x0 - Disable
                                *  @li @b 0x1 - Gen1
                                *  @li @b 0x3 - Gen2
                                *  @li @b 0x7 - Gen3
                                *  @li @b 0xF - Gen4
                                */
  MPIO_PP_SRIS_AUTODETECT_MODE  ,  /**< (uint8_t) Controls SRIS Autodetect mode
                                *  @li @b Disable = 0
                                *  @li @b Enable = 1
                                */
  MPIO_PP_SRIS_SKP_INTERVAL_SEL  ,  /**< (uint8_t) Controls SRIS SKP Interval Selection Mode
                                *  @li @b 0x0 - SKP ordered set Interval Lock Mode
                                *  @li @b 0x1 - Dynamic SKP ordered set Interval Mode
                                *  @li @b 0x2 - Far End Nominal Empty Mode
                                */
  MPIO_PP_SRIS_AUTODETECT_FACTOR,  /**< (uint8_t) Controls Autodetect Factor
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
                                * @li __New for Genoa__
                                */
  MPIO_PP_LEGACY_SWITCH0_SELECT,     /**< (uint8_t) I2C Select of Legacy I2c Switch __(optional)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
  MPIO_PP_NPEM_ENABLE,               /**< (uint8_t) Controls NPEM features
                                *  The bit[1] to bit[11] of the NPEM Control register
                                   are set depending on NPEM Capability register
                                *  Valid bits are bit[0] to bit[11]
                                */
  MPIO_PP_NPEM_CAPABILITES,           /**< (UINT16) Controls NPEM Capability
                                *  NPEM Capability register is set to this value
                                   when PP_NPEM_ENABLE = 1.
                                *  Valid bits are bit[0] to bit[11]
                                */
  MPIO_PP_BMC_LOCATION               /**< (BOOLEAN) States the port location of the BMC
                                *  Defines the location of the BMC if TRUE
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

typedef struct {                                          ///< Structure that discribe lane mapping
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
  MPIO_CHANNEL_MAPPING      ChannelMapping;            ///< Channel mapping
} MPIO_CONN_CHANNEL_MAPPING;

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
  void                   *Reserved2;               ///< Reserved for future use
  uint8_t                BmcLinkLocation;          ///< Identifies the socket/die location of a BMC link
  uint8_t                BmcLinkLaneNum;           ///< Identifies the socket/die location of a BMC Lane number
  uint8_t                Reserved3[2];             ///< Reserved for future
} MPIO_COMPLEX_DESCRIPTOR;

#pragma pack(pop)
