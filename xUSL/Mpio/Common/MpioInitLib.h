/**
 *  @file MpioInitLib.h
 *  @brief Structures and defines for MPIO firmware initailization
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include "MpioStructs.h"

#define   MAX_INSTANCE_ID           32
#define   MAX_PORT_DEVICE_MAP_SIZE  24
#define   MAX_PORTS_SUPPORTED       128

// The following ancillary data size maximum is defined in MPIO FW, sizeof(ictfw_shared_memory_t)
#define   MPIO_MAX_ANCILLARY_DATA_SIZE  6076

#define MAX_NUMBER_DPCSTATUS      128
/// PCIE_DPC_STATUS
typedef struct {
  uint8_t                           SocketId;                       ///< Socket ID
  uint8_t                           DieID;                          ///< Die ID
  uint8_t                           RBIndex;                        ///< Node to which GNB connected
  uint8_t                           BusNumber;                      ///< PCI Bus Number
  uint8_t                           PCIeCoreID;                     ///< PCIe core ID
  uint8_t                           PCIePortID;                     ///< PCIe port ID
  uint16_t                          DpcStatus;                      ///< PCIe DPC status
} PCIe_DPC_STATUS_RECORD;

/// PCIE_DPC_STATUS
typedef struct {
  uint8_t                           size;                                 ///< number of PCIe DPC status
  PCIe_DPC_STATUS_RECORD            DpcStatusArray[MAX_NUMBER_DPCSTATUS]; ///< PCIe DPC status Array
} PCIe_DPC_STATUS_DATA;


/**--------------------------------------------------------------------
 * @defgroup DXIO_MBOX Values for dxio mailbox
 *
 * @{
 */
/** Dxio mailbox post was defered */
#define DXIO_MBOX_RETURN_POSTED_DEFRD           3
/** @} */

/**--------------------------------------------------------------------
 * @defgroup BIOS_UBM Ubm/Bios return values
 *
 * @{
 */
/** Bios result is ok. */
#define BIOSSMC_Result_OK                    0x1
/** Ubm invalid switch addr return. */
#define UBM_INVALID_SWITCH_ADDR              0x0
/** Ubm Gpi not used return. */
#define UBM_GPIO_NOT_USED                    0xFF
/** Bios post message macro helper. */
#define POSTED_MSG(message) message + (DXIO_MBOX_RETURN_POSTED_DEFRD << 8)
/** The new device connected is an SATA/SAS device (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_SATA_SAS         (0x4)
/** The new device connected is an PCIe device (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_QUAD_PCI         (0x5)
/** The old device is no longer present (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_EMPTY            (0x7)
/** A device that was previously connected has been removed. */
#define BIOS_EVENT_DEVICE_DISCONNECTED      0
/** A device has been connected to a port or was detected during enumeration.*/
#define BIOS_EVENT_DEVICE_CONNECTED         (0x1)
/** No device was connected to a port during enumeration. */
#define BIOS_EVENT_DEVICE_NOT_PRESENT       (0x2)

#define SIL_RESERVED_831                    (0x01)
#define SIL_RESERVED_830                    (0x0F)
/** @} */

#define MPIO_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_MPIO & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)


#pragma pack (push, 1)

/// MPIO Message List - This enumerator defines the messages supported by MPIO FW3
typedef enum {
  MPIO_GET_FIRMWARE_VERSION  = 0x00,   ///< 0x00 - Get firmware version and status information
  MPIO_GET_STATUS,                     ///< 0x01 - Get the current status of MPIO
  MPIO_SET_GLOBAL_CONFIG,              ///< 0x02 - Set Host Firmware global configuration structure values.
  MPIO_GET_ASK_RESULT,                 ///< 0x03 - Update status for links defined in shared memory
  MPIO_SETUP_LINK,                     ///< 0x04 - MPIO will process links defined in the shared memory
  MPIO_ENABLE_CG,                      ///< 0x05 - MPIO Clock Gating enable after SMU Clock Gating
  MPIO_RECOVER_ASK,                    ///< 0x06 - Recover the BMC link ASK that was requested by ABL
  MPIO_TRANSFER_ASK,                   ///< 0x07 - DMA an array of ASKs using the form MPIO_ASK_STRUCT
  MPIO_TRANSFER_EXTENDED_ATTRIBUTES,   ///< 0x08 - DMA a table of extended attributes formerly ancillary data

  MPIO_MSG_PCIE_SPEED_CHANGE,          ///< 0x09 - PCIE Speed change
  MPIO_MSG_PCIE_INITIALIZE_ESM,        ///< 0x0A - ESM Support
  MPIO_MSG_PCIE_RESET_CONTROLLER,      ///< 0x0B - Reset PCIE Controller

  /*! This interface allows the MPIO to proxy the accesses to the PCIe straps for the Host Firmware.
  // strap number - 0xfffe0000 | index, pcie core upper 16, in aperture order
  // 32 data
  // each controller has index/data port0-8

  @arguments
  - Argument 0 - pcie core [31:16] strap_idx [15:0]
  - Argument 1 - The value to write to the strap
  @response
  - Response Register 0 = @ref SMC_Result_OK on success and @ref SMC_Result_Failed on error.
  - Error Register = @ref MSG_ACCEPTED */
  MPIO_MSG_PCIE_WRITE_STRAP                  = 0x0C,

  /*! Provides the MPIO with the I2C topography information necessary to enumerate the UBM, U.3, and OCP controllers.
  This interface uses the structure defined in @ref ubm_i2c_info_t. The information is provided via a pointer to an
  array of these entries and is saved by the MPIO before responding to this call. This call also starts the MPIO process
  of enumerating the devices which will complete before this command is acknowledged.
  This can take some time allowing the Host Firmware to continue on and check back with the MPIO for completion.
  @note The I2C topography provided must contain at least one I2C switch and expander in order to be considered valid
  and the MPIO responds with an error if this is not the case.
  @arguments
  - Argument 0 - The MPIO accessible location of the I2C data stucture array containing the I2C topology for the system.
  - Argument 1 - The number of entries in the I2C data array provided in the first argument.
  @response
  - Response Register 0 = @ref SMC_Result_OK on success and @ref SMC_Result_Failed on error.
  - Error Register = @ref MSG_ACCEPTED */

  /*! This is to ask for DELI.

  @arguments

  - Argument 0 - Reserved.
  - Argument 1 - DMA source address (high).
  - Argument 2 - DMA source address (low).
  - Argument 3 - Reserved.
  - Argument 4 - Start phy lane of the link.
  - Argument 5 - Reserved.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_GET_DELI_INFO              = 0x0E,

  BIOS_MPIO_MSG_I2C_ENUMERATE              = 0x10,
  /*! Request from the Host Firmware to return the nth DFC connection information for a UBM connected device connector.
  This request provides the connection information for the I2C connector index and device connector index provided.
  If the either is out of range this command returns and error.
  @arguments
  - Argument 0 - The I2C index from the original array provided by the BIOS_MPIO_MSG_I2C_ENUMERATE interface.
  - Argument 1 - The zero based device connector index for this I2C interface.
  @response
  - Response Register 0 = Returns the total number of device connectors associated with the I2C connection.
  - Response Register 1 = @ref bios_dfc_info_t word 0.
  - Response Register 2 = @ref bios_dfc_info_t word 1.
  - Response Register 3 = @ref bios_dfc_info_t word 2.
  - Error Register = @ref MSG_ACCEPTED */
  BIOS_MPIO_MSG_I2C_DEVICE_GET             = 0x11,
  /*! The MPIO signals an event via interrupt to the Host Firmware indicating that it has detected a change has been detected to
  a port that was added via the BIOS_MPIO_MSG_I2C_ENUMERATE command. This command allows the MPIO to return each change
  event to the Host Firmware.
  @arguments
  There are no arguments to this commands.
  @response
  The response provided is based on the type of node that triggered the event. The Host Firmware is expected to know this based
  on the HFC index that is returned via this command.
  - UBM
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 Word 0 of @ref bios_dfc_info_t
      - Response Register 2 Word 1 of @ref bios_dfc_info_t
      - Response Register 3 Word 2 of @ref bios_dfc_info_t
  - OCP
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 is @ref bios_ocp_info_t
  - U.3/U.2:
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 is @ref bios_u3_info_t
  */
  BIOS_MPIO_MSG_I2C_DEVICE_CHANGE_GET      = 0x12,
  /*! This command is used to send the legacy hotplug I2C configuration information to the MPIO.
  @note this was originally a two step process with a call to  fSMC_MSG_SetBiosDramAddr which set this address. It
  was 64-bit address as well which we may still need? Seems like this is part of our shared memory and should be like
  the interface for UBM I2C.
  @arguments
  - Argument 0 - The Address to DMA this data from and into to the MPIO local storage.
  - Argument 1
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET   = 0x13,
  /*! This is the initial call to enable the legacy hotplug interface for the MPIO. The
  BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET command must have been sent before this or this call fails.
  @arguments
  - Argument Register 0 - The values in this argument are encoded in the @ref bios_msg_enable_hotplug_t.args structure.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_ENABLE             = 0x14,
  /*! This is the call is used to disable the legacy hotplug interface for the MPIO. This will shutdown all polling
  and notifications related to any legacy hotplug events.
  @arguments
  None.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_DISABLE            = 0x15,
  /*! This is the call is used to set a given bus switch's address with a single argument passed in the
  first argument.
  @arguments
  - Argument Register 0 - The format of this register is in @ref bios_msg_i2c_switch_addr_t which holds the
  new I2C address in the least significant byte and the bus index in the second byte.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the new address was stored.
  */
  BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET        = 0x16,
  /*! This is the call sets the Express Module blinking period for LEDs.
  @arguments
  - Argument Register 0 - The blink period in ms?
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_BLINK_INTERVAL_SET         = 0x17,
  /*! This is the call sets the polling interval of I2C connected devices. This can be used to slow the polling of
  I2C devices for hotplug related events. This value is ignored if it is less than the total number of I2C devices
  present in the system. The interval given here is the gap between polling the full list of I2C devices.
  @arguments
  - Argument Register 0 - The polling interval between accesses to the I2C devices.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_POLL_INTERVAL_SET  = 0x18,
  /*! This is the call sets some of the debugging features in the hotplug interface. This call is set only and once
  these are set they cannot be cleared.
  @arguments
  - Argument Register 0 - The debug flags to set for the hotplug interface.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_FLAGS_SET          = 0x19,
  /*!
  This command is used to repspond to interrupts by setting the proper values in the argument register formated using
  the GpioIntrCommand_t structure.
  @arguments
  - Argument Register 0 - This is the stucture defined in GpioIntrCommand_t.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_CMD_SET   = 0x1a,
  /*! This command is used to return the current GPIO interrupt status in the firmware.
  @arguments
  - None.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_STATUS_GET        = 0x1b,
  /*! This command is used to issue one of two GPIO related commands. These are based on a field in the
  argument passed to this command in the BIOS_HOTPLUG_COMMAND_FORMAT_t structure.
  These are:
  - Write the hotplug reset pin.
  - Read the hotplug reset pin.
  @arguments
  - None.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_SERVICE       = 0x1c,
  /*! Unblock MSI interrupt for firmware first hotplug on a given NBIO.
  @arguments
  - Argument Register 0 - The zero based NBIO instance number to unblock.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_PORT_UNBLOCK       = 0x1d,
  /*! Cancel hot add for firmware first hotplug on a given NBIO.
  @arguments
  - Argument Register 0
      - byte 0 is the port
      - byte 1 is the pcie instance
      - byte 2 is the zero based NBIO instance
      - byte 3 bit 0 is clear to set bridge disable or 1 to clear bridge disable.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_ADD_CANCEL         = 0x1e,
  BIOS_MPIO_MSG_AUTHENTICATE_CHIPSET       = 0x1f,
  BIOS_MPIO_MSG_NVME_RAID_TRAP             = 0x20,
  BIOS_MPIO_MSG_CONFIG0_NBIF_TRAP          = 0x21,
  /*! Used to update MPIO ask after configuration to provide MPIO with logical bridge ID mapping to ports.
  @arguments
  - None.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_UPDATE_LINK                = 0x22,
  BIOS_MPIO_MSG_MESSAGE_COUNT              = 0x23
} MPIO_MESSAGES;

 /// Transfer Direction - This enumeration defines values for direction of TRANSFER_AS
typedef enum {
    MPIO_TO_DRAM = 0,     ///< Transfer from MPIO into DRAM
    DRAM_TO_MPIO          ///< Transfer from DRAM into MPIO
} TRANSFER_DIRECTION;

 /// Link Selector - This enumeration defines values for selection of link transfers
typedef enum {
    ALL_LINKS = 0,        ///< Transfer ALL links
    SELECTED_LINKS        ///< Transfer SELECTED links
} LINK_SELECTOR;

 /// MPIO Link States - This enumerator defines the link status values for MPIO FW3
typedef enum {
    LINK_NOT_ALLOCATED = 0,   ///< tbd
    LINK_ALLOCATED,           ///< tbd
    LINK_PROVISIONED,         ///< tbd
    LINK_BIFURCATION_FAILURE, ///< tbd
    LINK_RESET,               ///< tbd
    LINK_NOT_TRAINED,         ///< tbd
    LINK_TRAINED,             ///< tbd
    LINK_FAILURE,             ///< tbd
    LINK_TRAINING_FAILURE,    ///< tbd
    LINK_TIMEOUT              ///< tbd
} MPIO_LINK_STATE;

 /// MPIO Control Type - This enumerator defines the link types for the MPIO FW3 ASK structure.
typedef enum {
  ASK_TYPE_PCIe=0, ///< Ask for Pcie data
  ASK_TYPE_SATA=1, ///< Ask for Sata data
  ASK_TYPE_xGMI=2, ///< Ask for Xgmi data
  ASK_TYPE_GMI=3,  ///< Ask for Gmi data
  ASK_TYPE_ETH=4,  ///< Ask for Eth data
  ASK_TYPE_USB=5,  ///< Ask for Usb data
  /*
   * dxio_ctrl_type_WAFL=5, will be type xGMI, sub-type WAFL
   */
} MPIO_ASK_TYPE;

/// Mpio Global Configuration - Contains all vars for Mpio overall config
typedef struct {
    /*
	 * DWORD 0 - General
	 */
    uint32_t    skipVetting                 :1;  ///< tbd
    uint32_t    ntbhotpluginterval          :1;  ///< tbd
    uint32_t    saverestoremode             :2;  ///< tbd
    uint32_t    matchPortSizeExactly        :1;  ///< tbd
    uint32_t    skipLinkDisableOnFailure    :1;  ///< tbd
    uint32_t    usePhySram                  :1;  ///< If optional
    uint32_t    ValidPhyFWFlag              :1;  ///< If optional
    uint32_t    enableLoopbackSupport       :1;  ///< tbd
    uint32_t    stbVerbosityLevel           :2;  ///< tbd
    uint32_t    padDword0                   :21; ///< tbd

    /*
	 * DWORD 1 - Power Management
	 */
    uint32_t    PWRMNGMT_PRFRM_CLK_GATING            :1;  ///< tbd
    uint32_t    PWRMNGMT_PRFRM_STATIC_PWR_GATING     :1;  ///< tbd
    uint32_t    PWRMNGMT_PRFRM_REFCLK_SHUTDOWN       :1;  ///< tbd
    uint32_t    OPTIONS_ENABLEPOWERMANAGEMENT     :1;  ///< tbd
    uint32_t    PWRMNGMT_PRFRM_PMA_POWER_GATING      :1;  ///< tbd
    uint32_t    PWRMNGMT_PRFRM_PMA_CLOCK_GATING      :1;  ///< tbd
    uint32_t    padDword1                            :26; ///< tbd

    /*
	 * DWORD 2 - PCIE Link Timeouts (in msec)
	 */
    uint32_t    LinkReceiverDetectionPolling          :16; ///< tbd
    uint32_t    LinkL0Polling                         :16; ///< tbd

    /*
	 * DWORD 3 - Protocol
	 */
    uint32_t    LinkResetToTrainingTime               :16; ///< tbd
    uint32_t    PCIE_ALLOW_COMPLETION_PASS            :1;  ///< tbd
    uint32_t    OPTIONS_ALLOWPOINTERSLIPINTERVAL   :1;  ///< tbd
    uint32_t    padDword3                             :14; ///< tbd

    /*
	 * DWORD 4 - PCIE PERST
	 */
    uint32_t     disableSbrTrap                       :1;  ///< tbd
    uint32_t     disableLaneMarginingTrap             :1;  ///< tbd
    uint32_t     padDword4                            :30; // From Rembrandt

    /*
	 * DWORD 5 - Future
	 */
    uint32_t     padDword5;    ///< tbd
} MPIO_GLOBAL_CONFIG;

/// Link Attributes - Specifies link details
typedef struct {
    /*
	 * Byte 0
	 */
    uint32_t    devfuncnumber; ///< tbd

	/*
     * Byte 4
	 */
    uint32_t    portPresent        :1; ///< tbd
    uint32_t    earlyTrainLink     :1; ///< tbd
    uint32_t    linkComplianceMode :1; ///< tbd
    uint32_t    pad                :1; ///< tbd
    uint32_t    linkHotplugType    :4; ///< Move NTB Hotplug to be part of this enum

    /*
	 * Byte5
	 */
    uint32_t    maxLinkSpeedCap    :4; ///< tbd
    uint32_t    targetLinkSpeed    :4; ///< tbd

    /*
	 * Byte 6
	 */
    uint32_t    PSPPMode           :3; ///< tbd
    uint32_t    partnerDeviceType  :2; ///< tbd
    uint32_t    PSPP_pad           :3; ///< tbd

    /*
	 * Byte 7
	 */
    uint32_t    localPerst         :1; ///< tbd
    uint32_t    bifMode            :1; ///< tbd
    uint32_t    isMasterPLL        :1; ///< tbd
    uint32_t    invertPolarityRx   :1; ///< tbd
    uint32_t    invertPolarityTx   :1; ///< tbd
    uint32_t    ctrl_pad           :3; ///< tbd

    /*
	 * Byte 8
	 */
    uint32_t    gen3EqSearchMode   :2; ///< tbd
    uint32_t    gen3EqSearchModeEn :2; ///< tbd
    uint32_t    gen4EqSearchMode   :2; ///< tbd
    uint32_t    gen4EqSearchModeEn :2; ///< tbd

    /*
	 * Byte 9
	 */
    uint32_t    gen5EqSearchMode   :2; ///< tbd
    uint32_t    gen5EqSearchModeEn :2; ///< tbd

    /*
     * Byte 10
	 */
    uint32_t    txDeEmphasis       :2; ///< tbd
    uint32_t    txDeEmphasisEn     :1; ///< tbd
    uint32_t    txVetting          :1; ///< tbd
    uint32_t    rxVetting          :1; ///< tbd
    uint32_t    txrx_padding       :3; ///< tbd

    /*
	 * Byte 11
	 */
    uint32_t    esmSpeed           :6; ///< tbd
    uint32_t    esmMode            :2; ///< tbd

    /*
	 * Byte 12-19
	 */
    uint8_t     hfc_index;              ///< tbd
    uint8_t     dfc_index;              ///< tbd
    uint16_t    LogicalBridgeID    :5;  ///< tbd
    uint16_t    dwPad1             :11; ///< tbd
    uint32_t    dwPad2;                 ///< Reserved for MPIO use
    uint32_t    dwPad3;                 ///< Reserved for MPIO use
} FW3_LINK_ATTR;

// Link Struct For Dxio Lane Info and Identifiers
typedef struct {
    uint32_t   startLaneIndex     :16;  ///< The lowest number DXIO lane assigned to this link
    uint32_t   numberOfLanes      :6;   ///< The number of lanes assigned to this link
    uint32_t   reversed           :1;   ///< Indicates that the link is logically reversed
    uint32_t   linkStatus         :5;   ///< Reserved for ICTFW use only
    uint32_t   ctrlType           :4;   ///< Link type as defined in #MPIO_ASK_TYPE
    uint32_t   gpioHandle         :8;   ///< Platform identifier for PERST# for this link
    uint32_t   channelType        :8;   ///< Channel Type
    uint32_t   ancillaryDataIdx   :16;  ///< Offset of extended attributes for this link in ancillary data
    FW3_LINK_ATTR link_attributes;      ///< #FW3_LINK_ATTR structure
} FW3_LINK_STRUCT;

/// Link Status Struct
typedef struct {
    uint32_t   state              :4;  ///< State of link represented as ICTFW_LINK_STATES
    uint32_t   speed              :7;  ///< Speed of link represented as ICTFW_LINK_SPEEDS
    uint32_t   width              :5;  ///< Number of lanes in-use by link

    uint32_t   port               :8;  ///< Port associated with this link (0 except for PCIE
    uint32_t   pad                :8;  ///< Reserved
} ICTFW_LINK_STATUS;

 /// MPIO ASK STRUCTURE - ASK is comprised of link description and link status
typedef struct {
    FW3_LINK_STRUCT   desc;          ///< To prevent runtime translation FW3_LINK_STRUCT is used for link description
    ICTFW_LINK_STATUS status;        ///< General link status
    uint32_t            reserved[4]; ///< Reserved fields to keep struct binary compatible after simple changes
} FW_ASK_STRUCT;

/// FW_ASK buffer
typedef struct {
  FW_ASK_STRUCT                    AskArray[MAX_PORT_DEVICE_MAP_SIZE]; ///< MPIO FW_ASK_STRUCT Array
} FW_ASK;

/// DFC_UBM_DATA structure -  event structure used to respond to initial connection information during enumeration and
/// during hotplug events for UBM interfaces.
typedef struct {
  uint8_t     genspeed;          ///< The PCIe gen speed(0-7) A value of 7 is
                                 ///< No limit for PCIe, or this is the SATA
                                 ///< Link Rate(0-3) Value of 3 is No Limit for SATA
  uint8_t     type:3;            ///< Current device type installed This contains
                                 ///< the PRSNT, IFDET, and IFDET2 pin information for UBM.
  uint8_t     rsvd:3;            ///< Reserved for type expansion
  uint8_t     bifurcate_port:1;  ///< If set port is currently bifurcated
  uint8_t     secondary_port:1;  ///< If set then information is for a secondary port
  union {
    struct {
      uint8_t     ref_clk:1;       ///< If set reference clock routing is present.
      uint8_t     power_disable:1; ///< If set power disable feature is present.
      uint8_t     perst:1;         ///< If set PCIe reset feature is present.
      uint8_t     dual_port:1;     ///< If set dual port feature is present.
      uint8_t     rsvd:4;          ///< Reserved.
    } Field;
      uint8_t Value;               ///< Used to directly access the byte.
  } dfcFeats;
  uint8_t slot;
  /*! Align to a 32-bit word. (Word 2 Byte 2-3) */
  uint8_t : 8;
  uint8_t : 8;
} DFC_UBM_DATA;

typedef struct {
    uint8_t host;                ///< The zero based host index for this interface
    uint8_t socket;              ///< The zero based socket index for this interface
    uint8_t                 : 8; ///< Reserved
    uint8_t                 : 8; ///< Reserved
    uint8_t                 : 8; ///< Reserved
    uint8_t                 : 8; ///< Reserved
} DFC_OCP_DATA;

/// Cxl Device Info - Event structure to respond to inital connect info during enumeration and hotplug events
typedef struct
{
	/*
	 * This is the event structure used to respond to initial connection information during enumeration and
	 * during hotplug events for UBM interfaces.  For BIOS_EVENT_DEVICE_CONNECTED and
	 * BIOS_EVENT_DEVICE_NOT_PRESENT events all values are filled with valid data. For
	 * BIOS_EVENT_DEVICE_DISCONNECTED only the hfc_idx and event values are valid, no other values can be
	 * used in this case.
	 */
    uint8_t hfc_idx;    ///< HFC index to match to Host Firmware database.
    uint8_t event;      ///< The event that triggered this message and is one of the BIOS_EVENT_DEVICE_ values.
    uint16_t num_dfcs;  ///< The total number of DFCs for this HFC, this is always returned. (Word 0)
    uint8_t lane_start; ///< The logical lane assignment for this DFC (0-15). (Word 1 Byte 0)
    uint8_t lane_width; ///< The lane width for this DFC (1-16). (Word 1 Byte 1)
    union {
      DFC_UBM_DATA dfcubm; ///< tbd
      DFC_OCP_DATA dfcocp; ///< tbd
    } Device;
} DFC_DESCRIPTOR;

/// Get Status Results - Message Argument Structure
typedef struct {
  uint32_t      CmdStatus;    ///< tbd
  uint32_t      CycleStatus;  ///< Phases already executed
  uint32_t      FwPostCode;   ///< tbd
  uint32_t      FwStatus;     ///< tbd
  uint32_t      Reserved[2];  ///< tbd
} GET_STATUS_RESULTS;

/// Get Ask Result Args - Message Argument Structure
typedef struct {
  uint32_t    DestAddressHi; ///< tbd
  uint32_t    DestAddressLo; ///< tbd
  uint32_t    LinksOnly:1;   ///< Links Only (0 = entire struct, 1 = only selected links)
  uint32_t    Reserved:31;   ///< tbd
  uint32_t    StartLink;     ///< tbd
  uint32_t    LinkCount;     ///< tbd
  uint32_t    Reserved1;     ///< tbd
} GET_ASK_RESULT_ARGS;

/// Setup Link Result Args - Message Argument Structure
typedef struct {
  uint32_t    Map:1;            ///< Reconcile (Map)
  uint32_t    Configure:1;      ///< PCS/PHY Programing  (Configure)
  uint32_t    Reconfigure:1;    ///< Controller programming (Reconfigure)
  uint32_t    PerstReq:1;       ///< Perst Callout
  uint32_t    Training:1;       ///< Training
  uint32_t    Enumerate:1;      ///< Enumerate
  uint32_t    Reserved:26;      ///< tbd
  uint32_t    Reserved1[5];     ///< tbd
} SETUP_LINK_ARGS;

/// Transfer Ask Args - Message Argument Structure
typedef struct {
  uint32_t    DramAddressHi; ///< tbd
  uint32_t    DramAddressLo; ///< tbd
  uint32_t    Links:1;       ///< tbd
  uint32_t    Direction:1;   ///< tbd
  uint32_t    Reserved:30;   ///< tbd
  uint32_t    StartLink;     ///< tbd
  uint32_t    LinkCount;     ///< tbd
  uint32_t    Reserved1;     ///< tbd
} TRANSFER_ASK_ARGS;

/// Transfer Ask Attribute Args - Message Argument Structure
typedef struct {
  uint32_t    DramAddressHi;  ///< tbd
  uint32_t    DramAddressLo;  ///< tbd
  uint32_t    NumberOfDWords; ///< tbd
  uint32_t    Reserved[3];    ///< tbd
} TRANSFER_EXT_ATTR_ARGS;

/*=============================================
 * UBM Structures
 *=============================================
 */

#define MAX_I2C_TOPOGRAPHY_DEPTH  2
#pragma pack(1)
typedef enum {
    MPIO_I2C_NODE_TYPE_UBM  = 0, ///< This is a UBM connected node (ubm_i2c_info_t.node_type).
    MPIO_I2C_NODE_TYPE_OCP  = 1, ///< This is a OCP connected node (ubm_i2c_info_t.node_type).
    MPIO_I2C_NODE_TYPE_U2   = 2, ///< This is a U.2 connected node (ubm_i2c_info_t.node_type).
    MPIO_I2C_NODE_TYPE_U3   = 3, ///< This is a U.3 connected node (ubm_i2c_info_t.node_type).
} i2c_node_type;

/// I2c Expander
typedef struct {
  uint8_t Bp_Type;        ///< This is the bit offset for the bp_type pin in a GPIO expander.
  uint8_t I2c_Reset;      ///< This is the bit offset for the i2c_reset pin in a GPIO expander.
  uint8_t Reserved0 : 8;  ///< This is the bit offset for the perst pin in a GPIO expander.
  uint8_t SlotNum;        ///< Reserved
} UBM_DATA;

typedef struct {
  uint8_t OcpPresentStart;        ///< tbd
  uint8_t OcpNumHosts      : 4;   ///< tbd
  uint8_t OcpNumSockets    : 4;   ///< tbd
  union {
    struct {
      uint8_t OcpBifPrim   : 3;   ///< tbd
      uint8_t OcpBifSec    : 3;   ///< tbd
      uint8_t OcpFormFactor: 1;   ///< tbd
      uint8_t Reserved     : 1;   ///< tbd
    } Field;
    uint8_t Identifier;           ///< tbd
  } OcpForm;
  uint8_t SlotNum;                ///< tbd
} OCP_DATA;

/// I2C Switch - The information describing the path to this HFC.
typedef struct {
  uint8_t Addr;     ///< The I2C address of this I2C Switch. A value of UBM_INVALID_SWITCH_ADDR indicates
                    ///< the entry must not be used.
  uint8_t Select:4; ///< The switch selection port for this HFC which has valid values from 0-3.
  uint8_t Type:4;   ///< Reserved for future use, must be zero.
} I2C_SWITCH;

/// MPIO Hfc Port
typedef struct {
  uint8_t NodeType :8;    ///< tbd
  struct I2C_EXPANDER {
    uint8_t Addr;         ///< The address of the gpio Expander for this HFC.
    uint8_t Type;         ///< The type of this GPIO expander and is one of the following values:
                          ///<  - UBM_GPIO_EXPANDER_TYPE_9539
                          ///<  - UBM_GPIO_EXPANDER_TYPE_9535
                          ///<  - UBM_GPIO_EXPANDER_TYPE_9506
  } Expander;

  uint8_t Start_Lane;     ///< Start_Lane

  union {
    UBM_DATA Ubm;         ///< tbd
    OCP_DATA Ocp;         ///< tbd
  } Device;

  struct I2C_SWITCH{
  uint8_t Addr;           ///< The I2C address of this I2C Switch. A value of
                          ///< UBM_INVALID_SWITCH_ADDR indicates the entry must not be used.
  uint8_t Select:4;       ///< The switch selection port for this HFC which has valid values from 0-3.
  uint8_t Type:4;         ///< Reserved for future use, must be zero.
  } Switch[MAX_I2C_TOPOGRAPHY_DEPTH];

} MPIO_HFC_PORT;

typedef struct {
  uint16_t NpemEnable;          ///< tbd
  uint16_t NpemCap;             ///< tbd
  MPIO_ANC_DATA AncData;        ///< Ancillary data override
  uint8_t OcpDefValid;          ///< tbd
  uint8_t OcpDefPrimPrsntb;     ///< tbd
  uint8_t OcpDefSecPrsntb;      ///< tbd
} HFC_TO_DFC_DATA;

/// UBM HFC Descriptor
typedef struct {
  uint32_t        Size;             ///< Socket Id
  MPIO_HFC_PORT   HfcPortList[32];  ///< Pointer to array of PCIe port
                                    ///< descriptors or NULL (Last element of array
                                    ///< must be terminated with DESCRIPTOR_TERMINATE_LIST).
  HFC_TO_DFC_DATA HfcToDfcData[32]; ///< Data to be pulled from HFC descriptors
                                    ///< in topology table that needs to be
                                    ///< populated in generated DFC ports.
} MPIO_UBM_HFC_DESCRIPTOR;
#pragma pack()

/// Ancillary data format
typedef enum
{
  ANCILLARY =1,           ///< tbd
  ANC_LIST = 2,           ///< tbd
  ANC_PHY_OVRD = 3,       ///< tbd
  ANC_PERPORT_STRAP = 4,  ///< tbd
  ANC_ALLPORT_STRAP = 5,  ///< tbd
  ANC_CTRLLER_STRAP = 6,  ///< tbd
  ANC_SPEED_OVRD = 7      ///< tbd
} ANC_TYPE;

/// Strap Type
typedef enum
{
  SHORT_DATA = 0, ///< tbd
  LONG_DATA = 1   ///< tbd
} STRAP_TYPE;

/// Strap Entry Short Per Port
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  data     : 19; ///< tbd
} MPIO_STRAPENTRY_SHORT_PERPORT;

/// Strap Entry Long Per Port
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  reserved : 19; ///< tbd
  uint32_t  data     : 32; ///< tbd
} MPIO_STRAPENTRY_LONG_PERPORT;

/// MPIO Strap Entry Short All Ports
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  data     : 19; ///< tbd
} MPIO_STRAPENTRY_SHORT_ALLPORT;

/// MPIO Strap Entry Long All Ports
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  reserved : 19; ///< tbd
  uint32_t  data     : 32; ///< tbd
} MPIO_STRAPENTRY_LONG_ALLPORT;

/// MPIO Strap Entry Short Controller
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  data     : 19; ///< tbd
} MPIO_STRAPENTRY_SHORT_CTRLLER;

/// MPIO Strap Entry Long Controller
typedef struct
{
  uint32_t  strapType: 3;  ///< tbd
  uint32_t  strapIdx : 10; ///< tbd
  uint32_t  reserved : 19; ///< tbd
  uint32_t  data     : 32; ///< tbd
} MPIO_STRAPENTRY_LONG_CTRLLER;

/// Generic Ancillary Data Header
typedef struct
{
  uint8_t  type;     ///< tbd
  uint8_t  id;       ///< tbd
  uint8_t  length;   ///< tbd
  uint8_t  reserved; ///< tbd
} GENERIC_ANC_HEADER;

typedef enum {
  E32,                      ///< 0x00 - E32
  GMI3 = 0x02,              ///< 0x02 - GMI3
} PHY_TYPE;

typedef enum {
  PCIE_PCS,                 ///< PCIE PCS
  SATA_PCS,                 ///< SATA PCS
  XGMI_PCS,                 ///< XGMI PCS
  GMI_PCS,                  ///< GMI PCS
  XPCS_PCS,                 ///< XPCS PCS
} SPD_OVRD_TYPE;

#define ANC_LIST_TYPE            GENERIC_ANC_HEADER

typedef struct {
  uint16_t ElementOfs0;     ///< tbd
  uint16_t ElementOfs1;     ///< tbd
} ANC_DATA_LIST;

typedef struct {
  uint8_t  type;          ///< tbd
  uint8_t  id;            ///< tbd
  uint8_t  length;        ///< tbd
  uint8_t  phytype;       ///< tbd
} ANC_PHY_OVRD_HEADER;    ///< tbd

typedef struct {
  uint8_t  type;        ///< tbd
  uint8_t  ovrdtype;    ///< tbd
  uint8_t  length;      ///< tbd
  uint8_t  lane;        ///< tbd
} ANC_SPD_OVRD_HEADER;

typedef struct {
  uint32_t PhyOverride;   ///< tbd
} ANC_PHY_OVRD_DATA;

typedef union {
  struct {
    uint32_t  EnMask:8;   ///< tbd
    uint32_t  SpdC0:8;    ///< tbd
    uint32_t  SpdCn:8;    ///< tbd
    uint32_t  SpdCp:8;    ///< tbd
  } Field;
  uint32_t Value;         ///< tbd
} ANC_SPD_OVRD_DATA;

#pragma pack (pop)

 /**--------------------------------------------------------------------
 * @defgroup Hot plug States
 *
 * @{
 */
 /** tbd. */
#define MAX_HOTPLUG_PCIE_PORTS 160
/** @} */

typedef struct {
  uint32_t HotPlugFormat          : 3; ///< tbd
  uint32_t ResetDescriptorValid   : 1; ///< tbd
  uint32_t PortActive             : 1; ///< tbd
  uint32_t MasterSecondaryAPU         : 1; ///< tbd
  uint32_t DieID                  : 1; ///< tbd
  uint32_t PciePortInstance       : 4; ///< tbd
  uint32_t PcieTileInstance       : 4; ///< tbd
  uint32_t BridgeOffset           : 5; ///< tbd
  uint32_t                        : 4; ///< tbd
  uint32_t AltSlotNum             : 6; ///< tbd
  uint32_t SecondaryLink          : 1; ///< tbd
  uint32_t                        : 1; ///< tbd
} PcieHotPlugMappingDescriptorV3_t;

typedef struct {
  uint32_t i2c_bit_or_nibble_select   :   3; ///< [2] Nibble Select for Enterprise SSD, [2:0] Bit Select for Simple
                                             ///< Presence Detect
  uint32_t i2c_byte_select            :   3; ///< tbd
  uint32_t i2c_device_address         :   5; ///< tbd
  uint32_t i2c_device_type            :   2; ///< tbd
  uint32_t i2c_bus_segment            :   5; ///< tbd
  uint32_t function_mask              :   8; ///< tbd
  uint32_t i2c_bus_segment2           :   6; ///< tbd
} PcieHotPlugFunctionDescriptor_t;

typedef struct {
  uint32_t                            :   3; ///< tbd
  uint32_t i2c_byte_select            :   3; ///< tbd
  uint32_t i2c_device_address         :   5; ///< tbd
  uint32_t i2c_device_type            :   2; ///< tbd
  uint32_t i2c_bus_segment            :   5; ///< tbd
  uint32_t reset_select               :   8; ///< tbd
  uint32_t                            :   6; ///< tbd
} PcieHotPlugResetDescriptor_t;

typedef struct {
  PcieHotPlugMappingDescriptorV3_t  MappingDescriptor   [MAX_HOTPLUG_PCIE_PORTS]; ///< tbd
  PcieHotPlugFunctionDescriptor_t FunctionDescriptor  [MAX_HOTPLUG_PCIE_PORTS]; ///< tbd
  PcieHotPlugResetDescriptor_t    ResetDescriptor     [MAX_HOTPLUG_PCIE_PORTS]; ///< tbd
} PcieHotPlugConfigTable_t;

typedef struct {
  uint32_t       AssertionSeq;     ///< 0:Disable, 1:DeAssert (Compatible with legacy ), 2:First Assert then Deassert
  uint32_t       Address;          ///< tbd
  uint32_t       DeassertionValue; ///< tbd
  uint32_t       AssertionValue;   ///< tbd
  uint32_t       MillisecodsDelay; ///< tbd
} MPIO_S0I3_GPIO_RST_EP;

void
HfcDescriptorDebugDump (
  MPIO_UBM_HFC_DESCRIPTOR  *HfcDescriptor
);

void
MpioUbmDiscovery (
  MPIO_COMPLEX_DESCRIPTOR     **ComplexDescriptor,
  MPIO_UBM_HFC_DESCRIPTOR     *HfcDescriptor,
  uint8_t                     *HfcStartLanes,
  uint8_t                     HfcListSize,
  size_t                      Socket1Index,
  PCIe_PLATFORM_CONFIG        *Pcie
  );

void
AddHfcDescriptor (
  MPIO_PORT_DESCRIPTOR         *EngineDescriptor,
  MPIO_UBM_HFC_DESCRIPTOR      *HfcDescriptor,
  uint8_t                      *HfcStartLanes,
  uint8_t                      *HfcListSize
  );

void
MpioCfgAfterDxioInit (
  PCIe_PLATFORM_CONFIG                 *Pcie
  );

void
InitializeTopologyEntry (
  MPIO_COMPLEX_DESCRIPTOR      **InTablePointer,
  uint8_t                      EngineType,
  uint8_t                      StartLane,
  uint8_t                      EndLane,
  uint8_t                      ResetId,
  uint8_t                      Device,
  uint8_t                      Function,
  bool                         *InitializeSuccess,
  uint64_t                     *EntryHandle
  );

void
AddParameter (
  MPIO_COMPLEX_DESCRIPTOR      **TablePointer,
  uint64_t                     EntryHandle,
  MPIO_PORT_PARAM              Parameter,
  bool                         *AddParamSuccess
  );

void
RemoveEntry (
  MPIO_COMPLEX_DESCRIPTOR      **TablePointer,
  uint64_t                     EntryHandle,
  bool                         *RemoveEntrySuccess
  );

void
IncreaseTableSize (
  MPIO_COMPLEX_DESCRIPTOR      **TablePointer,
  size_t                       NewSize,
  bool                         *IncreaseSuccess
  );

SIL_STATUS InitializeMpioTp1 (void);
SIL_STATUS InitializeMpioTp2 (void);
SIL_STATUS InitializeMpioTp3 (void);
SIL_STATUS MpioClassSetInputBlock (void);
