/**
 * @file  PciStructs.h
 * @brief PCI structures and definitions for Genoa
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <Pci.h>
#include <SilSocLogicalId.h>
#include <Apob.h>

/*----------------------------------------------------------------------------
 *   GNB PCIe configuration info
 *----------------------------------------------------------------------------
 */

// Event definitions

#define GNB_EVENT_INVALID_CONFIGURATION               0x20010000ul   // User configuration invalid
#define GNB_EVENT_INVALID_PCIE_TOPOLOGY_CONFIGURATION 0x20010001ul   // Requested lane allocation for PCIe port can not be supported
#define GNB_EVENT_INVALID_PCIE_PORT_CONFIGURATION     0x20010002ul   // Requested incorrect PCIe port device address
#define GNB_EVENT_INVALID_LANES_CONFIGURATION         0x20010005ul   // Lane double subscribe lanes
#define GNB_EVENT_LINK_TRAINING_FAIL                  0x20020000ul   // PCIe Link training fail
#define GNB_EVENT_BROKEN_LANE_RECOVERY                0x20030000ul   // Broken lane workaround applied to recover link training
#define GNB_EVENT_GEN2_SUPPORT_RECOVERY               0x20040000ul   // Scale back to GEN1 to recover link training


#define DESCRIPTOR_TERMINATE_LIST           0x80000000ull
#define DESCRIPTOR_IGNORE                   0x40000000ull

/// PCIe link initialization
typedef enum {
  EndpointDetect = 0,                                     ///< Detect endpoint presence
  EndpointNotPresent                                      ///< Endpoint not present (or connected). Used in case there is alternative way to determine
                                                          ///< if device present on board or in slot. For example GPIO can be used to determine device presence.
} PCIE_ENDPOINT_STATUS;


/// PCIe port misc extended controls
typedef struct  {
        uint8_t                     LinkComplianceMode :1;  ///< Force port into compliance mode (device will not be trained, port output compliance pattern)
        uint8_t                     LinkSafeMode       :2;  /**< Safe mode PCIe capability. (Parameter may limit PCIe speed requested through PCIe_PORT_DATA::LinkSpeedCapability)
                                                             *  @li @b 0 - port can advertize muximum supported capability
                                                             *  @li @b 1 - port limit advertized capability and speed to PCIe Gen1
                                                             */
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
        uint8_t                     Reserved0          :2;  ///< Unused space
} PCIe_PORT_MISC_CONTROL;

/// The IO APIC Interrupt Mapping Info
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
} APIC_DEVICE_INFO;

/// Initial Offset Calibration Control
typedef enum {
  ADAPT_IOC_DISABLED = 0,                                     ///< Initial Offset Calibration Disabled
  ADAPT_IOC_ENABLED                                           ///< Initial Offset Calibration Enabled
} ADAPT_IOC_CONTROL;

/// DFE Control values
typedef enum {
  ADAPT_DFE_CONTROL_DISABLED = 0,                             ///< DFE Disabled
  ADAPD_DFE_CONTROL_1TAP_DFE = 4,                             ///< 1-tap DFE
  ADAPD_DFE_CONTROL_1TAP_DFE_FBF,                             ///< 1-tap DFE with Future Bit Filtering
  ADAPD_DFE_CONTROL_2TAP_DFE,                                ///< 2-tap DFE
  ADAPD_DFE_CONTROL_2TAP_DFE_FBF                              ///< 2-tap DFE with Future Bit Filtering
} ADAPT_DFE_CONTROL;

/// LEQ Control values
typedef enum {
  ADAPT_LEQ_CONTROL_DISABLED = 0,                              ///< LEQ Disabled
  ADAPT_LEQ_CONTROL_DC_GAIN  = 2,                              ///< DC Gain Adaptation
  ADAPT_LEQ_CONTROL_DC_GAIN_POLE                               ///< DC Gain and Pole Adaptation
} ADAPT_LEQ_CONTROL;

/// Dynamic Offset Calibration Control
typedef enum {
  ADAPT_DOC_DISABLED = 0,                                      ///< Dynamic Offset Calibration Disabled
  ADAPT_DOC_ENABLED                                            ///< Dynamic Offset Calibration Enabled
} ADAPT_DOC_CONTROL;

/// FOM Calculation Control
typedef enum {
  ADAPT_FOMC_DISABLED = 0,                                    ///< FOM Calculation Disabled
  ADAPT_FOMC_ENABLED                                          ///< FOM Calculation Enabled
} ADAPT_FOMC_CONTROL;

/// PI Offset Calibration Control
typedef enum {
  ADAPT_PIOC_DISABLED = 0,                                    ///< PI Offset Calibration Disabled
  ADAPT_PIOC_ENABLED                                          ///< PI Offset Calibration Enabled
} ADAPT_PIOC_CONTROL;

/// Bank Group Swap Mode
typedef enum {
  BANK_GROUP_SWAP_DISABLED = 0,                              ///< Bank Group Swap disabled
  BANK_GROUP_SWAP_CPU = 1,                                   ///< Bank Group Swap CPU Mode
  BANK_GROUP_SWAP_APU = 2                                    ///< Bank Group Swap APU Mode
} BANK_GROUP_SWAP_MODE;

/// GEN3 RxAdaptMode Configuration Structure
typedef struct {
        bool                InitOffsetCancellation;     ///< Initial Offset Cancellation Enable
        uint8_t                  DFEControl;                 ///< DFE Control
        uint8_t                  LEQControl;                 ///< LEQ Control
        bool                DynamicOffsetCalibration;   ///< Dynamic Offset Calibration Enable
        bool                FOMCalculation;             ///< FOM Calculation Enable
        bool                PIOffsetCalibration;        ///< PI Offset Calibratino Enable
} RX_ADAPT_MODE;

/// PCIe port configuration data
typedef struct  {
         uint8_t                   PortPresent;              ///< Enable PCIe port for initialization.
         uint8_t                   ChannelType;              /**< Channel type.
                                                               *  @li @b 0 - "lowLoss",
                                                               *  @li @b 1 - "highLoss",
                                                               *  @li @b 2 - "mob0db",
                                                               *  @li @b 3 - "mob3db",
                                                               *  @li @b 4 - "extnd6db"
                                                               *  @li @b 5 - "extnd8db"
                                                               */
         uint8_t                   DeviceNumber;             /**< PCI Device number for port.
                                                               *   @li @b 0 - Native port device number
                                                               *   @li @b N - Port device number (See available configurations in BKDG
                                                               */
         uint8_t                   FunctionNumber;           ///< Reserved for future use
         uint8_t                   LinkSpeedCapability;      /**< PCIe link speed/
                                                               *  @li @b 0 - Maximum supported by silicon
                                                               *  @li @b 1 - Gen1
                                                               *  @li @b 2 - Gen2
                                                               *  @li @b 3 - Gen3
                                                               */
         uint8_t                   LinkAspm;                 /**< ASPM control. (see SilPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - L0s only
                                                               *  @li @b 2 - L1 only
                                                               *  @li @b 3 - L0s and L1
                                                               */
         uint8_t                   LinkHotplug;              /**< Hotplug control.
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Basic
                                                               *  @li @b 2 - Server
                                                               *  @li @b 3 - Enhanced
                                                               */
         uint8_t                   ResetId;                  /**< Arbitrary number greater than 0 assigned by platform firmware for GPIO
                                                               *   identification which control reset for given port.
                                                               *   Each port with unique GPIO should have unique ResetId assigned.
                                                               *   All ports use same GPIO to control reset should have same ResetId assigned.
                                                               *   see SilPcieSlotResetContol.
                                                               */
         uint16_t                  SlotNum;                  ///< Physical Slot Number
         PCIe_PORT_MISC_CONTROL  MiscControls;             ///< Misc extended controls
         APIC_DEVICE_INFO        ApicDeviceInfo;           ///< IOAPIC device programming info
         PCIE_ENDPOINT_STATUS    EndpointStatus;           ///< PCIe endpoint (device connected to PCIe port) status
         RX_ADAPT_MODE           RxAdaptMode;              ///< Gen3 RxAdaptMode configuration
} PCIe_PORT_DATA;
#pragma pack(push,1)

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

} PCIe_ENGINE_DATA;

/// PCIe port descriptor
typedef struct {
         uint32_t               Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in complex
                                                           */
         PCIe_ENGINE_DATA     EngineData;               ///< Engine data
         PCIe_PORT_DATA       Port;                     ///< PCIe port specific configuration info
} PCIe_PORT_DESCRIPTOR;

#pragma pack(pop)
/// PCIe Complex descriptor
typedef struct {
         uint32_t               Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in topology
                                                           */
         uint32_t               SocketId;                 ///< Socket Id
         PCIe_PORT_DESCRIPTOR *PciePortList;            ///< Pointer to array of PCIe port descriptors or NULL (Last element of array must be terminated with DESCRIPTOR_TERMINATE_LIST).
         void                 *Reserved;                ///< Reserved for future use
} PCIe_COMPLEX_DESCRIPTOR;

/// Action to control PCIe slot reset
typedef enum {
  AssertSlotReset,                                        ///< Assert slot reset
  DeassertSlotReset                                       ///< Deassert slot reset
} PCIE_RESET_CONTROL;

#define GFX_VBIOS_IMAGE_FLAG_SPECIAL_POST  0x1

/// Engine descriptor type
typedef enum {
  PcieUnusedEngine = 0,                                   ///< Unused descriptor
  PciePortEngine = 1,                                     ///< PCIe port
  MaxPcieEngine                                           ///< Max engine type for boundary check.
} PCIE_ENGINE_TYPE;

/// PCIe link capability/speed
typedef enum  {
  PcieGenMaxSupported,                                    ///< Maximum supported
  PcieGen1 = 1,                                           ///< Gen1
  PcieGen2,                                               ///< Gen2
  PcieGen3,                                               ///< Gen3
  PcieGen4,                                               ///< Gen4
  PcieGen5,                                               ///< Gen5
  MaxPcieGen                                              ///< Max Gen for boundary check
} PCIE_LINK_SPEED_CAP;

/// PCIe link channel type
typedef enum {
  ChannelTypeLowLoss,                                     ///< Low Loss
  ChannelTypeHighLoss,                                    ///< High Loss
  ChannelTypeMob0db,                                      ///< Mobile 0dB
  ChannelTypeMob3db,                                      ///< Mobile 3dB
  ChannelTypeExt6db,                                      ///< Extended 6dB
  ChannelTypeExt8db,                                      ///< Extended 8dB
  MaxChannelType                                          ///< Not valid value, used to verify input
} PCIE_CHANNEL_TYPE;

/// PCIe link ASPM
typedef enum {
  AspmDisabled,                                           ///< Disabled
  AspmL0s,                                                ///< PCIe L0s link state
  AspmL1,                                                 ///< PCIe L1 link state
  AspmL0sL1,                                              ///< PCIe L0s & L1 link state
  MaxAspm                                                 ///< Not valid value, used to verify input
} PCIE_ASPM_TYPE;

/**
 * @brief  PCIe link hotplug support
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */

typedef enum {
  PcieHotplugDisabled,                                    ///< Hotplug disable
  PcieHotplugBasic,                                       ///< Basic Hotplug
  PcieHotplugServerExpress,                               ///< Server Hotplug Express Module
  PcieHotplugEnhanced,                                    ///< Enhanced
  PcieHotplugInboard,                                     ///< Inboard
  PcieHotplugServerEntSSD,                                ///< Server Hotplug Enterprise SSD
  PcieHotplugUBM,                                         ///< UBM Backplane
  PcieHotplugOCP,                                         ///< OCP NIC 3.0
  PcieMaxHotplug                                          ///< Not valid value, used to verify input

} PCIE_HOTPLUG_TYPE;

/// PCIe link initialization
typedef enum {
  PortDisabled,                                           ///< Disable
  PortEnabled                                             ///< Enable
} PCIE_PORT_ENABLE;

/// PCIe ACS capability - Access Control Services
typedef enum  {
  PcieAcsDisabled,                                        ///< Disabled
  PcieAcsEnabled,                                         ///< Enabled
} PCIE_ACS_CAP;

/// PCIe ClkPmSupport initialization
typedef enum {
  ClkPmSupportDisabled,                                   ///< Disable
  ClkPmSupportEnabled                                     ///< Enable
} CLKPM_SUPPORT_ENABLE;

#define EXT_DISPLAY_PATH_CAPS_DP_FIXED_VS_EN             0x02         ///< BIT[1] VBIOS will always output fixed voltage swing during DP link training
#define EXT_DISPLAY_PATH_CAPS_HDMI20_PI3EQX1204          0x04         ///< BIT[2] HDMI 2.0 connector
#define EXT_DISPLAY_PATH_CAPS_HDMI20_TISN65DP159RSBT     0x08         ///< BIT[3] HDMI 2.0 connector
#define EXT_DISPLAY_PATH_CAPS_HDMI20_PARADE_PS175        0x0C         ///< BIT[3:2] DP -> HDMI recoverter chip

/// DP receiver definitions with fixed voltage swing
typedef enum {
  DP_VS_0_4V_0DB,                                         ///< 0x00
  DP_VS_0_6V_0DB,                                         ///< 0x01
  DP_VS_0_8V_0DB,                                         ///< 0x02
  DP_VS_1_2V_0DB,                                         ///< 0x03
  DP_VS_0_4V_3_5DB = 0x8,                                 ///< 0x08
  DP_VS_0_6V_3_5DB,                                       ///< 0x09
  DP_VS_0_8V_3_5DB,                                       ///< 0x0a
  DP_VS_0_4V_6DB = 0x10,                                  ///< 0x10
  DP_VS_0_6V_6DB,                                         ///< 0x11
  DP_VS_0_4V_9_5DB = 0x18                                 ///< 0x18
} DP_FIXED_VOLT_SWING_TYPE;

// definition for eDP 1.4 VSMode
#define EDP_VS_LEGACY_MODE                  0             ///< Legacy Mode
#define EDP_VS_LOW_VDIFF_MODE               1             ///< Low Vdiff Training Mode
#define EDP_VS_HIGH_VDIFF_MODE              2             ///< High Vdiff Training Mode
#define EDP_VS_STRETCH_MODE                 3             ///< Stretched DP training mode
#define EDP_VS_SINGLE_VDIFF_MODE            4             ///< Single Vdiff Training Mode
#define EDP_VS_VARIABLE_PREM_MODE           5             ///< Single Vdiff Training with Variable Transition Vdiff

///  HDMI re-driver register/value
typedef struct _HDMI_RE_DRIVER_I2C_REG_INFO {
    uint8_t       RegIndex;                               ///< HDMI re-driver Register Index
    uint8_t       RegVal;                                 ///< HDMI re-driver Register Value
} HDMI_RE_DRIVER_I2C_REG_INFO;

/// AZ I2SBUS select
typedef enum {
  GnbAcpI2sBus,                                            ///< I2sBus
  GnbAcpAzalia,                                            ///< Azalia
  MaxAcp                                                   ///< Not valid value, used to verify input
} GNB_ACP_TYPE;

/// AZ I2SBUS pin configuration
typedef enum {
  GnbAcp4Tx4RxBluetooth,                                   ///< 4Tx4Rx and Bluetooth
  GnbAcp2Tx4RxBluetooth,                                   ///< 2Tx4Rx and Bluetooth
  GnbAcp6Tx4RxBluetooth,                                   ///< 6Tx4Rx and Bluetooth
  GnbAcpPinNotConfigured                                   ///< Not valid value, used to verify input
} GNB_ACP_AZ_I2SBUS_PIN;

/// Alternative DRAM MAC
typedef enum {
  MAC_UNTESTEDMAC,                               ///< Assign 0 to Untested MAC
  MAC_700k,                                      ///< Assign 1 to 700k
  MAC_600k,                                      ///< Assign 2 to 600k
  MAC_500k,                                      ///< Assign 3 to 500k
  MAC_400k,                                      ///< Assign 4 to 400k
  MAC_300k,                                      ///< Assign 5 to 300k
  MAC_200k,                                      ///< Assign 6 to 200k
  MAC_UNRESTRICTEDMAC = 8,                       ///< Assign 8 to Unrestricted MAC
} DRAM_MAXIMUM_ACTIVATE_COUNT;

// Macro for statically initializing various structures
#define  PCIE_ENGINE_DATA_INITIALIZER(mType, mStartLane, mEndLane) {mType, mStartLane, mEndLane}
#define  PCIE_PORT_DATA_INITIALIZER(mPortPresent, mChannelType, mDevAddress, mHotplug, mMaxLinkSpeed, mMaxLinkCap, mAspm, mResetId) \
{mPortPresent, mChannelType, mDevAddress, 0, mMaxLinkSpeed, mAspm, mHotplug, mResetId, 0, {0, mMaxLinkCap, 0, 0}, {0, 0, 0}, EndpointDetect, \
{ADAPT_IOC_ENABLED, ADAPT_DFE_CONTROL_DISABLED, ADAPT_LEQ_CONTROL_DC_GAIN_POLE, ADAPT_DOC_DISABLED, ADAPT_FOMC_ENABLED, ADAPT_PIOC_DISABLED}}
#define  PCIE_PORT_DATA_INITIALIZER_V2(mPortPresent, mChannelType, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, mAspm, mResetId, mClkPmSupport) \
{mPortPresent, mChannelType, mDevAddress, mDevFunction, mMaxLinkSpeed, mAspm, mHotplug, mResetId, 0, {0, mMaxLinkCap, 0, mClkPmSupport}, {0, 0, 0}, EndpointDetect, \
{ADAPT_IOC_ENABLED, ADAPT_DFE_CONTROL_DISABLED, ADAPT_LEQ_CONTROL_DC_GAIN_POLE, ADAPT_DOC_DISABLED, ADAPT_FOMC_ENABLED, ADAPT_PIOC_DISABLED}}
#define  PCIE_PORT_DATA_INITIALIZER_GEN3(mPortPresent, mChannelType, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, mAspm, mResetId, mClkPmSupport, \
mInitOffsetCancellation, mDFEControl, mLEQControl, mDynamicOffsetCalibration, mFOMCalculation, mPIOffsetCalibration) \
{mPortPresent, mChannelType, mDevAddress, mDevFunction, mMaxLinkSpeed, mAspm, mHotplug, mResetId, 0, {0, mMaxLinkCap, 0, mClkPmSupport}, {0, 0, 0}, EndpointDetect, \
{mInitOffsetCancellation, mDFEControl, mLEQControl, mDynamicOffsetCalibration, mFOMCalculation, mPIOffsetCalibration}}

///IOMMU requestor ID
typedef struct {
         uint16_t     Bus       :8;                                ///< Bus
         uint16_t     Device    :5;                                ///< Device
         uint16_t     Function  :3;                                ///< Function
} IOMMU_REQUESTOR_ID;

/// IVMD exclusion range descriptor
typedef struct {
         uint32_t               Flags;                    /**< Descriptor flags
                                                           * @li @b Flags[31] - Terminate descriptor array.
                                                           * @li @b Flags[30] - Ignore descriptor.
                                                           */
         IOMMU_REQUESTOR_ID   RequestorIdStart;         ///< Requestor ID start
         IOMMU_REQUESTOR_ID   RequestorIdEnd;           ///< Requestor ID end (use same as start for single ID)
         uint64_t               RangeBaseAddress;         ///< Phisical base address of exclusion range
         uint64_t               RangeLength;              ///< Length of exclusion range in bytes
} IOMMU_EXCLUSION_RANGE_DESCRIPTOR;

// Gnb PCIe Master PLL
#define GNB_PCIE_MASTERPLL_A   0xA
#define GNB_PCIE_MASTERPLL_B   0xB
#define GNB_PCIE_MASTERPLL_C   0xC
#define GNB_PCIE_MASTERPLL_D   0xD
