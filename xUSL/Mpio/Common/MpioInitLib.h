/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioInitLib.h
 *  @brief Structures and defines for MPIO firmware initialization
 */

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

typedef struct {
  PCIe_DPC_STATUS_DATA   *DpcStatusData;
  bool                   IsPortActive;
} AFTER_RECONFIG_BUFFER;

/**--------------------------------------------------------------------
 * @defgroup DXIO_MBOX Values for dxio mailbox
 *
 * @{
 */
/** Dxio mailbox post was deferred */
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

/** The OCP default validation mask bit. */
#define OCP_DEFAULT_VALID_MASK              (0x01)
/** The OCP default PRSNTB[3:0] of primary/secondary mask bit. */
#define OCP_DEFAULT_PRSNTB_MASK             (0x0F)
/** @} */

#define MPIO_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_MPIO & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)


#pragma pack (push, 1)

/// MPIO Message List - This enumerator defines the messages supported by MPIO FW3
typedef enum {
  MPIO_GET_FIRMWARE_VERSION = 0x00,  ///< 0x00 - Get firmware version and status information
  MPIO_GET_STATUS,                   ///< 0x01 - Get the current status of MPIO
  MPIO_SET_GLOBAL_CONFIG,            ///< 0x02 - Set BIOS global configuration structure values.
  MPIO_GET_ASK_RESULT,               ///< 0x03 - Update status for links defined in shared memory
  MPIO_SETUP_LINK,                   ///< 0x04 - MPIO will process links defined in the shared memory
  MPIO_ENABLE_CG,                    ///< 0x05 - MPIO Clock Gating enable after SMU Clock Gating
  MPIO_RECOVER_ASK,                  ///< 0x06 - Recover the BMC link ASK that was requested by ABL
  MPIO_TRANSFER_ASK,                 ///< 0x07 - DMA an array of ASKs using the form MPIO_ASK_STRUCT
  MPIO_TRANSFER_EXTENDED_ATTRIBUTES, ///< 0x08 - DMA a table of extended attributes formerly ancillary data
  MPIO_MSG_PCIE_SPEED_CHANGE,        ///< 0x09 - PCIE Speed change
  MPIO_MSG_PCIE_INITIALIZE_ESM,      ///< 0x0A - ESM Support
  MPIO_MSG_PCIE_RESET_CONTROLLER,    ///< 0x0B - Reset PCIE Controller
  MPIO_MSG_PCIE_WRITE_STRAP = 0x0C,
  BIOS_MPIO_MSG_GET_DELI_INFO = 0x0E,
  BIOS_MPIO_MSG_I2C_ENUMERATE = 0x10,
  BIOS_MPIO_MSG_I2C_DEVICE_GET = 0x11,
  BIOS_MPIO_MSG_I2C_DEVICE_CHANGE_GET = 0x12,
  BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET = 0x13,
  BIOS_MPIO_MSG_HOTPLUG_ENABLE = 0x14,
  BIOS_MPIO_MSG_HOTPLUG_DISABLE = 0x15,
  BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET = 0x16,
  BIOS_MPIO_MSG_HOTPLUG_BLINK_INTERVAL_SET = 0x17,
  BIOS_MPIO_MSG_HOTPLUG_POLL_INTERVAL_SET = 0x18,
  BIOS_MPIO_MSG_HOTPLUG_FLAGS_SET = 0x19,
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_CMD_SET = 0x1a,
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_STATUS_GET = 0x1b,
  BIOS_MPIO_MSG_HOTPLUG_GPIO_SERVICE = 0x1c,
  BIOS_MPIO_MSG_HOTPLUG_PORT_UNBLOCK = 0x1d,
  BIOS_MPIO_MSG_HOTPLUG_ADD_CANCEL = 0x1e,
  BIOS_MPIO_MSG_AUTHENTICATE_CHIPSET = 0x1f,
  BIOS_MPIO_MSG_NVME_RAID_TRAP = 0x20,
  BIOS_MPIO_MSG_CONFIG0_NBIF_TRAP = 0x21,
  BIOS_MPIO_MSG_UPDATE_LINK = 0x22,
  BIOS_MPIO_MSG_PCIE_GPIO_RESET = 0x23,
  BIOS_MPIO_MSG_PCIE_PORT_TRAINING = 0x24,
  BIOS_MPIO_MSG_SET_EXT_PCIE_BUSES = 0x25,
  BIOS_MPIO_MSG_PCIE_PROXY_WRITE = 0x26,
  BIOS_MPIO_MSG_PCIE_PSPP_SETTINGS = 0x28,
  BIOS_CMD_FRAME_BUF_TRAP_INIT = 0x29,
  BIOS_MPIO_MSG_UBM_PERST_RELEASE = 0x2A,
  BIOS_MPIO_MSG_PCIE_DLWM_SETTINGS = 0x2B,
  BIOS_MPIO_MSG_CNLI_INITIALIZE = 0x2C,
  BIOS_CMD_DEASSERT_PERST = 0x2D,
  BIOS_MPIO_MSG_MESSAGE_COUNT = 0x2E
} MPIO_MESSAGES;

/// Transfer Direction - This enumeration defines values for direction of TRANSFER_ASK
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
  LINK_NOT_ALLOCATED = 0,
  LINK_ALLOCATED,
  LINK_PROVISIONED,
  LINK_BIFURCATION_FAILURE,
  LINK_RESET,
  LINK_NOT_TRAINED,
  LINK_TRAINED,
  LINK_FAILURE,
  LINK_TRAINING_FAILURE,
  LINK_TIMEOUT
} MPIO_LINK_STATE;

/// MPIO Control Type - This enumerator defines the link types for the MPIO FW3 ASK structure.
typedef enum {
  ASK_TYPE_PCIe = 0, ///< Ask for Pcie data
  ASK_TYPE_xGMI = 2, ///< Ask for Xgmi data
  ASK_TYPE_GMI = 3,  ///< Ask for Gmi data
  ASK_TYPE_ETH = 4,  ///< Ask for Eth data
  ASK_TYPE_USB = 5,  ///< Ask for Usb data
} MPIO_ASK_TYPE;

/// Mpio Global Configuration - Contains all vars for Mpio overall config
typedef struct {
  /*
   * DWORD 0 - General
   */
  uint32_t    skipVetting                 :1;
  uint32_t    ntbhotpluginterval          :1;
  uint32_t    saverestoremode             :2;
  uint32_t    matchPortSizeExactly        :1;
  uint32_t    skipLinkDisableOnFailure    :1;
  uint32_t    usePhySram                  :1;  ///< If optional
  uint32_t    ValidPhyFWFlag              :1;  ///< If optional
  uint32_t    enableLoopbackSupport       :1;
  uint32_t    stbVerbosityLevel           :2;
  uint32_t    EnablePcieNonCompliantWa    :1;
  uint32_t    ActiveSltMode               :1;
  uint32_t    LegacyDevBootfailureWa      :1;
  uint32_t    DeferredMessageSupport      :1;
  uint32_t    CxlGpfPhase2Timeout         :4;
  uint32_t    enable_dlwm                          :1;
  uint32_t    combine_training_early_and_normal    :1;
  uint32_t    Reserved0                            :1;
  uint32_t    RunXgmiSafeRecoveryOdt               :1;
  uint32_t    RunZcal                              :1;
  uint32_t    XgmiAsyncFifoModeEnable              :1;
  uint32_t    padDword0                            :7;

  /*
   * DWORD 1 - Power Management
   */
  uint32_t    PWRMNGMT_PRFRM_CLK_GATING             :1;
  uint32_t    PWRMNGMT_PRFRM_STATIC_PWR_GATING      :1;
  uint32_t    PWRMNGMT_PRFRM_REFCLK_SHUTDOWN        :1;
  uint32_t    CBSOPTIONS_ENABLEPOWERMANAGEMENT      :1;
  uint32_t    PWRMNGMT_PRFRM_PMA_POWER_GATING       :1;
  uint32_t    PWRMNGMT_PRFRM_PMA_CLOCK_GATING       :1;
  uint32_t    CBSOPTIONS_DYNAMIC_PWR_GATING         :1;
  uint32_t    enable_kpx_shallow_pstate             :4;
  uint32_t    PWRMNGMT_PRFRM_USB4_STATIC_PWR_GATING :1;
  uint32_t    padDword1                             :20;

  /*
   * DWORD 2 - PCIE Link Timeouts (in msec)
   */
  uint32_t    LinkReceiverDetectionPolling          :16;
  uint32_t    LinkL0Polling                         :16;

  /*
   * DWORD 3 - Protocol
   */
  uint32_t    LinkResetToTrainingTime               :16;
  uint32_t    PCIE_ALLOW_COMPLETION_PASS            :1;
  uint32_t    CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL   :1;
  uint32_t    LinkDisableAtPowerOffDelay            :4;
  uint32_t    Enable2SpcGen4                        :1;
  uint32_t    padDword3                             :1;
  uint32_t    TxFifoRdPtrOffset                     :8;

  /*
   * DWORD 4 - Trap Control
   */
  uint32_t     disableSbrTrap                       :1;
  uint32_t     disableLaneMarginingTrap             :1;
  uint32_t     padDword4                            :30;

  /*
   * DWORD 5 - Future
   */
  uint32_t     Enable2SpcGen5                       :1;
  uint32_t     padDword5                            :31;
} MPIO_GLOBAL_CONFIG;

/// Link Attributes - Specifies link details
typedef struct {
  /*
   * Byte 0
   */
  uint32_t    devfuncnumber;

  /*
   * Byte 4
   */
  uint32_t    portPresent        :1;
  uint32_t    earlyTrainLink     :1;
  uint32_t    linkComplianceMode :1;
  uint32_t    pad                :1;
  uint32_t    linkHotplugType    :4; ///< Move NTB Hotplug to be part of this enum

  /*
   * Byte5
   */
  uint32_t    maxLinkSpeedCap    :4;
  uint32_t    targetLinkSpeed    :4;

  /*
   * Byte 6
   */
  uint32_t    PSPPMode           :3;
  uint32_t    partnerDeviceType  :2;
  uint32_t    PSPP_Enable        :1;
  uint32_t    recovery           :1;
  uint32_t    PSPP_pad           :1;

  /*
   * Byte 7
   */
  uint32_t    localPerst         :1;
  uint32_t    bifMode            :1;
  uint32_t    isMasterPLL        :1;
  uint32_t    invertPolarityRx   :1;
  uint32_t    invertPolarityTx   :1;
  uint32_t    ctrl_pad           :3;

  /*
   * Byte 8
   */
  uint32_t    gen3EqSearchMode   :2;
  uint32_t    gen3EqSearchModeEn :2;
  uint32_t    gen4EqSearchMode   :2;
  uint32_t    gen4EqSearchModeEn :2;

  /*
   * Byte 9
   */
  uint32_t    gen5EqSearchMode   :2;
  uint32_t    gen5EqSearchModeEn :2;
  uint32_t    gen5EqSearchReserve:4;

  /*
   * Byte 10
   */
  uint32_t    txDeEmphasis       :2;
  uint32_t    txDeEmphasisEn     :1;
  uint32_t    txVetting          :1;
  uint32_t    rxVetting          :1;
  uint32_t    txrx_padding       :3;

  /*
   * Byte 11
   */
  uint32_t    esmSpeed           :6;
  uint32_t    esmMode            :2;

  /*
   * Byte 12-19
   */
  uint8_t     hfc_index;
  uint8_t     dfc_index;
  uint16_t    LogicalBridgeID    :5;
  uint16_t    SwingMode          :3;
  uint16_t    SrisSkipInterval   :3;
  uint16_t    dwPad1             :5;
  uint32_t    l1ss_en            :1;
  uint32_t    EnabledLTR         :1;
  uint32_t    DC_MinPreferredSpeed : 4;
  uint32_t    DC_MaxPreferredSpeed : 4;
  uint32_t    DC_direct            : 1;
  uint32_t    AC_MinPreferredSpeed : 4;
  uint32_t    AC_MaxPreferredSpeed : 4;
  uint32_t    AC_direct            : 1;
  uint32_t    dwPad2               :12;
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

/// Get Status Results - Message Argument Structure
typedef struct {
  uint32_t      CmdStatus;
  uint32_t      CycleStatus;  ///< Phases already executed
  uint32_t      FwPostCode;
  uint32_t      FwStatus;
  uint32_t      Reserved[2];
} GET_STATUS_RESULTS;

/// Get Ask Result Args - Message Argument Structure
typedef struct {
  uint32_t    DestAddressHi;
  uint32_t    DestAddressLo;
  uint32_t    LinksOnly:1;   ///< Links Only (0 = entire struct, 1 = only selected links)
  uint32_t    Reserved:31;
  uint32_t    StartLink;
  uint32_t    LinkCount;
  uint32_t    Reserved1;
} GET_ASK_RESULT_ARGS;

/// Setup Link Result Args - Message Argument Structure
typedef struct {
  uint32_t    Map:1;            ///< Reconcile (Map)
  uint32_t    Configure:1;      ///< PCS/PHY Programming  (Configure)
  uint32_t    Reconfigure:1;    ///< Controller programming (Reconfigure)
  uint32_t    PerstReq:1;       ///< Perst Callout
  uint32_t    Training:1;       ///< Training
  uint32_t    Enumerate:1;      ///< Enumerate
  uint32_t    Reserved:26;
  uint32_t    Reserved1[5];
} SETUP_LINK_ARGS;

/// Transfer Ask Args - Message Argument Structure
typedef struct {
  uint32_t    DramAddressHi;
  uint32_t    DramAddressLo;
  uint32_t    Links:1;
  uint32_t    Direction:1;
  uint32_t    Reserved:30;
  uint32_t    StartLink;
  uint32_t    LinkCount;
  uint32_t    Reserved1;
} TRANSFER_ASK_ARGS;

/// Transfer Ask Attribute Args - Message Argument Structure
typedef struct {
  uint32_t    DramAddressHi;
  uint32_t    DramAddressLo;
  uint32_t    NumberOfDWords;
  uint32_t    Reserved[3];
} TRANSFER_EXT_ATTR_ARGS;

/*=============================================
 * UBM Structures
 *=============================================
 */

#pragma pack(1)
typedef enum {
  MPIO_I2C_NODE_TYPE_UBM = 0, ///< This is a UBM connected node (ubm_i2c_info_t.node_type).
  MPIO_I2C_NODE_TYPE_OCP = 1, ///< This is a OCP connected node (ubm_i2c_info_t.node_type).
  MPIO_I2C_NODE_TYPE_U2 = 2,  ///< This is a U.2 connected node (ubm_i2c_info_t.node_type).
  MPIO_I2C_NODE_TYPE_U3 = 3,  ///< This is a U.3 connected node (ubm_i2c_info_t.node_type).
} i2c_node_type;

/// I2C Switch - The information describing the path to this HFC.
typedef struct {
  uint8_t Addr;     ///< The I2C address of this I2C Switch. A value of UBM_INVALID_SWITCH_ADDR indicates
                    ///< the entry must not be used.
  uint8_t Select:4; ///< The switch selection port for this HFC which has valid values from 0-3.
  uint8_t Type:4;   ///< Reserved for future use, must be zero.
} I2C_SWITCH;

#pragma pack()

/// Ancillary data format
typedef enum
{
  ANCILLARY = 1,
  ANC_LIST = 2,
  ANC_PHY_OVRD = 3,
  ANC_PERPORT_STRAP = 4,
  ANC_ALLPORT_STRAP = 5,
  ANC_CTRLLER_STRAP = 6,
  ANC_SPEED_OVRD = 7
} ANC_TYPE;

/// Strap Type
#define MPIO_ANC_PERPORT_STRAP_HEADER {ANC_PERPORT_STRAP, 0, 0, 0}
#define MPIO_ANC_ALLPORT_STRAP_HEADER {ANC_ALLPORT_STRAP, 0, 0, 0}
#define MPIO_ANC_CTRLLER_STRAP_HEADER {ANC_CTRLLER_STRAP, 0, 0, 0}

typedef enum
{
  SHORT_DATA = 0,
  LONG_DATA = 1
} STRAP_TYPE;

/// Strap Entry Short Per Port
typedef struct
{
  uint32_t  strapType: 3;
  uint32_t  strapIdx : 10;
  uint32_t  data     : 19;
} MPIO_STRAPENTRY_SHORT_PERPORT;

/// MPIO Strap Entry Short All Ports
#define MPIO_SHORT_ALLPORT_STRAP(INDEX, DATA)   {SHORT_DATA, INDEX, DATA}

typedef struct
{
  uint32_t  strapType: 3;
  uint32_t  strapIdx : 10;
  uint32_t  data     : 19;
} MPIO_STRAPENTRY_SHORT_ALLPORT;

#define MPIO_SHORT_CTRLLER_STRAP(INDEX, DATA)    {SHORT_DATA, INDEX, DATA}
#define MPIO_LONG_CTRLLER_STRAP(INDEX, DATA)     {LONG_DATA, INDEX, 0, DATA}

/// MPIO Strap Entry Short Controller
typedef struct
{
  uint32_t  strapType: 3;
  uint32_t  strapIdx : 10;
  uint32_t  data     : 19;
} MPIO_STRAPENTRY_SHORT_CTRLLER;

/// MPIO Strap Entry Long Controller
typedef struct
{
  uint32_t  strapType: 3;
  uint32_t  strapIdx : 10;
  uint32_t  reserved : 19;
  uint32_t  data     : 32;
} MPIO_STRAPENTRY_LONG_CTRLLER;

/// Generic Ancillary Data Header
typedef struct
{
  uint8_t  type;
  uint8_t  id;
  uint8_t  length;
  uint8_t  reserved;
} GENERIC_ANC_HEADER;

#define ANC_DATA_HEADER_SIZE 1

typedef enum {
  E32,                      ///< 0x00 - E32
  GMI3 = 0x02,              ///< 0x02 - GMI3
} PHY_TYPE;

typedef enum {
  PCIE_PCS,                 ///< PCIE PCS
  XGMI_PCS,                 ///< XGMI PCS
  GMI_PCS,                  ///< GMI PCS
  XPCS_PCS,                 ///< XPCS PCS
} SPD_OVRD_TYPE;

#define ANC_LIST_TYPE            GENERIC_ANC_HEADER

typedef struct {
  uint16_t ElementOfs0;
  uint16_t ElementOfs1;
} ANC_DATA_LIST;

typedef struct {
  uint8_t  type;
  uint8_t  id;
  uint8_t  length;
  uint8_t  phytype;
} ANC_PHY_OVRD_HEADER;

typedef struct {
  uint8_t  type;
  uint8_t  ovrdtype;
  uint8_t  length;
  uint8_t  lane;
} ANC_SPD_OVRD_HEADER;

typedef struct {
  uint32_t PhyOverride;
} ANC_PHY_OVRD_DATA;

typedef union {
  struct {
    uint32_t  EnMask:8;
    uint32_t  SpdC0:8;
    uint32_t  SpdCn:8;
    uint32_t  SpdCp:8;
  } Field;
  uint32_t Value;
} ANC_SPD_OVRD_DATA;

#pragma pack (pop)

/**--------------------------------------------------------------------
 * @defgroup Hot plug States
 *
 */
 #define MAX_HOTPLUG_PCIE_PORTS 160

typedef struct {
  uint32_t HotPlugFormat          : 3;
  uint32_t ResetDescriptorValid   : 1;
  uint32_t PortActive             : 1;
  uint32_t MasterSecondaryAPU     : 1;
  uint32_t DieID                  : 1;
  uint32_t PciePortInstance       : 4;
  uint32_t PcieTileInstance       : 4;
  uint32_t BridgeOffset           : 5;
  uint32_t                        : 4;
  uint32_t AltSlotNum             : 6;
  uint32_t SecondaryLink          : 1;
  uint32_t                        : 1;
} PcieHotPlugMappingDescriptorV3_t;

typedef struct {
  uint32_t i2c_bit_or_nibble_select   :   3; ///< [2] Nibble Select for Enterprise SSD, [2:0] Bit Select for Simple
                                             ///< Presence Detect
  uint32_t i2c_byte_select            :   3;
  uint32_t i2c_device_address         :   5;
  uint32_t i2c_device_type            :   2;
  uint32_t i2c_bus_segment            :   5;
  uint32_t function_mask              :   8;
  uint32_t i2c_bus_segment2           :   6;
} PcieHotPlugFunctionDescriptor_t;

typedef struct {
  uint32_t                            :   3;
  uint32_t i2c_byte_select            :   3;
  uint32_t i2c_device_address         :   5;
  uint32_t i2c_device_type            :   2;
  uint32_t i2c_bus_segment            :   5;
  uint32_t reset_select               :   8;
  uint32_t                            :   6;
} PcieHotPlugResetDescriptor_t;

typedef struct {
  PcieHotPlugMappingDescriptorV3_t  MappingDescriptor[MAX_HOTPLUG_PCIE_PORTS];
  PcieHotPlugFunctionDescriptor_t FunctionDescriptor[MAX_HOTPLUG_PCIE_PORTS];
  PcieHotPlugResetDescriptor_t    ResetDescriptor[MAX_HOTPLUG_PCIE_PORTS];
} PcieHotPlugConfigTable_t;

typedef struct {
  uint32_t       AssertionSeq;     ///< 0:Disable, 1:DeAssert (Compatible with legacy ), 2:First Assert then Deassert
  uint32_t       Address;
  uint32_t       DeassertionValue;
  uint32_t       AssertionValue;
  uint32_t       MillisecodsDelay;
} MPIO_S0I3_GPIO_RST_EP;

SIL_STATUS MpioClassSetInputBlockCommon (
  SIL_CONTEXT  *SilContext
  );
