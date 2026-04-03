/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobCmn.h
 * @brief OpenSIL APOB functions
 *
 */

#include <ProjSocConst.h>
#include <SilCommon.h>

#pragma once

#define APOB_SIGNATURE        0x424F5041  //!!APOB

#define APOB_HMAC_SIZE                32
#define MAX_APOB_ERROR_ENTRIES        64
#define APOB_BASE_ADDRESS     CONFIG_PLAT_APOB_ADDRESS

// APOB Group Definitions
#define APOB_MEM       1
#define APOB_DF        2
#define APOB_CCX       3
#define APOB_GNB       4
#define APOB_FCH       5
#define APOB_PSP       6
#define APOB_GEN       7
#define APOB_SMBIOS    8
#define APOB_FABRIC    9
#define APOB_APCB     10

// APOB Type Definitions
#define APOB_MEM_GENERAL_ERRORS_TYPE               1
#define APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE   2
#define APOB_GEN_CONFIGURATION_INFO_TYPE           3
#define APOB_GEN_S3_REPLAY_BUFFER_INFO_TYPE        4
#define APOB_MEM_PMU_SMB_TYPE                      5
#define APOB_GEN_EVENT_LOG_TYPE                    6
#define APOB_MEM_DIMM_SMBUS_INFO_TYPE              7
#define APOB_MEM_SMBIOS_TYPE                       8
#define APOB_SYS_MAP_INFO_TYPE                     9
#define APOB_APCB_BOOT_INFO_TYPE                  16
#define APOB_MEM_DIMM_SPD_DATA_TYPE               17
#define APOB_MEM_MBIST_RESULT_INFO_TYPE           18
#define APOB_SYS_NPS_INFO_TYPE                    19
#define APOB_SYS_SLINK_INFO_TYPE                  20
#define APOB_DF_DXIO_PHY_FW_OVERRIDE_INFO_TYPE    21
#define APOB_MEM_PMU_TRAINING_FAILURE_INFO_TYPE   22
#define APOB_ENV_FLAGS_INFO_TYPE                  23
#define APOB_SYS_CXL_INFO_TYPE                    24
#define APOB_MEM_SYSTEM_CONFIGURATION_INFO_TYPE   25
#define APOB_GEN_CONFIG_DATA_TYPE                 26
#define APOB_MEM_SOC_INIT_CONFIG_TYPE             27
#define APOB_MEM_RMP_INFO                         28
#define APOB_MEM_PHY_PIE_IMAGE                    29

#define APOB_MEM_S3_DDR_PHY_REPLAY_PHASE0_BUFFER_INFO_TYPE        30
#define APOB_MEM_S3_DDR_PHY_REPLAY_MAX_ENTRIES                    10

#define APOB_MEM_DDR5_DIMM_HUB_REG_TYPE                           40

#define APOB_MEM_UMC_PHY_CONTEXT_REPLAY_PHASE0_BUFFER_INFO_TYPE   50
#define APOB_MEM_UMC_PHY_CONTEXT_REPLAY_MAX_ENTRIES               16

#define APOB_MEM_PHY_ACSM_BLOCK0_TYPE                             66
#define APOB_MEM_PHY_ACSM_BLOCK1_TYPE                             67

#define APOB_MEM_S3_MOP_ARRAY_REPLAY_CHANNEL0_BUFFER_INFO_TYPE    70

#define APOB_MEM_PMU_SMB_CHANNEL0_TYPE                            90

// APOB CCX Type Definitions
#define APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE                      1
#define APOB_CCX_EDC_THROTTLE_THRESH_TYPE                          2
#define APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE                      3

// APOB APCB Updates
#define APOB_APCB_UPDATES_STRUCT_TYPE                            120

#define APOB_ENTRY_INSTANCE_MAX      0xFF  ///< Apob Entry Instance Max Value

typedef struct {
  uint8_t  ApobHmac[APOB_HMAC_SIZE];  ///< APOB HMAC for
} APOB_HMAC;                          ///< security check of each type

/// AMD APOB_HEADER Header
typedef struct {
  uint32_t   Signature;                 ///< APOB signature
  uint32_t   Version;                   ///< Version
  uint32_t   Size;                      ///< APOB Size
  uint32_t   OffsetOfFirstEntry;        ///< APOB Header Size
} APOB_BASE_HEADER;

/// AMD APOB_HEADER Header
typedef struct {
  uint32_t  GroupID;                    ///< Group ID
  uint32_t  DataTypeID;                 ///< Data Type ID
  uint32_t  InstanceID;                 ///< Instance ID
                                        ///< - Bit 15:0 - SocketID
                                        ///< - Bit  8:0 - DieID
  uint32_t  TypeSize;                   ///< Type Size (including header)
  APOB_HMAC ApobTypeHmac;               ///< Hmac location for type
} APOB_TYPE_HEADER;

typedef struct {
  uint32_t  PreSilCtrl0;
  uint32_t  PreSilCtrl1;
} ENV_FLAGS_STRUCT;

typedef struct {
  APOB_TYPE_HEADER           ApobTypeHeader;
  ENV_FLAGS_STRUCT           Flags;
} APOB_ENV_FLAGS_STRUCT;

///
/// APOB SPD Data for each DIMM.
///
typedef struct _APOB_SPD_STRUCT {
  uint8_t  SocketNumber;          ///< Indicates the socket number
  uint8_t  ChannelNumber;         ///< Indicates the channel number
  uint8_t  DimmNumber;            ///< Indicates the channel number
  uint8_t  PageAddress;           ///< Indicates the 256 Byte EE Page the data belongs to
                                  ///<      0 = Lower Page
                                  ///<      1 = Upper Page
  bool     DimmPresent;           ///< Indicates if the DIMM is present
  uint8_t  MuxPresent;            ///< SpdMux Present or not. if 1, then yes otherwise no
  uint8_t  MuxI2CAddress;         ///< MuxI2cAddress
  uint8_t  MuxChannel;            ///< MuxChannel no.
  uint32_t Address;               ///< SMBus address of the DRAM
  uint32_t SerialNumber;          ///< DIMM Serial Number
  uint32_t DeviceWidth:3;         ///< Device Width i.e. x4, x8, x16 and x32
  uint32_t DpprSupported:2;       ///< Dppr Support Present
  uint32_t SpprSupported:1;       ///< Sppd Support Present
  uint32_t Reserved:26;           ///< Reserved for Future Use
  uint8_t  Data[512];             ///< Buffer for 256 Bytes of SPD data from DIMM
} APOB_SPD_STRUCT;

typedef struct _APOB_D5_SPD_STRUCT {
  bool                 DramDownSpdValid;  ///< Indicates that the DramDown SPD is valid
  bool                 DimmPresent;       ///< Indicates that the DIMM is present and Data is valid
  uint32_t             Address;           ///< SMBus address of the DRAM
  uint8_t              SocketNumber;      ///< Indicates the socket number
  uint8_t              ChannelNumber;     ///< Indicates the channel number
  uint8_t              DimmNumber;        ///< Indicates the channel number
  bool                 ShadowSpdValid;    ///< Indicates whether the DIMM is present in the last boot
  uint8_t              Data[1024];        ///< Buffer for 1024 Bytes of SPD data from DIMM
} APOB_D5_SPD_STRUCT;

typedef struct _APOB_LP5_SPD_STRUCT {
  bool                 DramDownSpdValid;  ///< Indicates that the DramDown SPD is valid
  bool                 DimmPresent;       ///< Indicates that the DIMM is present and Data is valid
  uint32_t             Address;           ///< SMBus address of the DRAM
  uint8_t              SocketNumber;      ///< Indicates the socket number
  uint8_t              ChannelNumber;     ///< Indicates the channel number
  uint8_t              DimmNumber;        ///< Indicates the channel number
  bool                 ShadowSpdValid;    ///< Indicates whether the DIMM is present in the last boot
  uint8_t              Data[512];         ///< Buffer for 512 Bytes of SPD data from DIMM
} APOB_LP5_SPD_STRUCT;

#define  CCX_NOT_PRESENT           (0xFF)

/// Event log entry
typedef struct {
  uint32_t    EventClass;  ///< The severity of the event, its associated status.
  uint32_t    EventInfo;   ///< Uniquely identifies the event.
  uint32_t    DataA;
  uint32_t    DataB;
} APOB_ERROR_LOG;

typedef struct {
  APOB_ERROR_LOG AgesaEvent[MAX_APOB_ERROR_ENTRIES];    ///< The entry itself.
} AGESA_EVENT_STRUCT;

typedef struct {
  APOB_TYPE_HEADER   ApobTypeHeader;  ///< APOB Type Header
  uint16_t           Count;           ///< The total number of active entries.
  AGESA_EVENT_STRUCT ApobEventStruct; ///< The entries.
} EVENT_LOG_STRUCT;

//
// APOB MEM RMP Information
//

typedef struct _APOB_RMP_PROFILE_DATA {
  bool        ProfileEn;
  uint16_t    MemClk;
  uint16_t    Vdd;
  uint16_t    Tcl;
  uint8_t     Reserved[4];
} APOB_RMP_PROFILE_DATA;

typedef struct {
  uint16_t DimmManId;            ///< DIMM manufacturer ID
  uint16_t DimmModId;            ///< DIMM Module ID
  uint16_t Page1SpdChecksumLow;  ///< DIMM checksum for lower 128 bytes
  uint16_t Page1SpdChecksumHigh; ///< DIMM checksum for upper 128 bytes
  bool     DimmPresentInConfig;  ///< Indicates that the DIMM config is present
  uint8_t  Reserved[3];
} DIMM_APOB_OPT_ID;

/**
 * @brief AMD APCB_ITEM32 struct
 *
 */
typedef struct {
  uint32_t  Token;
  uint32_t  Value;
} APCB_ITEM32;

#define MAX_APCB_UPDATES           32

typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;      ///< APOB Type Header
  uint32_t          NumItems;
  uint8_t           Reserved[4];
  APCB_ITEM32       Item[MAX_APCB_UPDATES];
} APOB_APCB_UPDATES_STRUCT;

#define ABL_APOB_MAX_CHANNELS_PER_DIE       12  ///< Max channels per die
#define ABL_APOB_MAX_DIMMS_PER_CHANNEL       2  ///< Max dimms per die

/**
 * @brief Normalized Address for Low and High Address
 */
typedef struct _DPPRCL_REPAIR_NORMALIZED_ADDRESS_V3 {
  uint32_t NormalizedAddressLo : 32;  ///< [31:0] Lower 32 bit of Normalized Address
  uint32_t NormalizedAddressHi : 32;  ///< [31:0] Upper 32 bit of Normalized Address
} DPPRCL_REPAIR_NORMALIZED_ADDRESS_V3;

/**
 * @brief Module Serial Number
 * @details DDR will utilize the LoWord only while Hob Memory uses all 64 bits
 */
typedef struct _DPPRCL_REPAIR_SERIAL_NUMBER_V3 {
  uint32_t SerialNumberLoWord : 32;   ///< [31:0] Serial Number LoWord
  uint32_t SerialNumberHiWord : 32;   ///< [31:0] Serial Number HiWord
} DPPRCL_REPAIR_SERIAL_NUMBER_V3;

/**
 * @brief DDR Post Package Repair Entry extend field union (v3)
 * @details This union supports creating low and high values
 *          for normalized address or serial number
 */
typedef union _DPPRCL_REPAIR_EXTEND_FIELD_V3  {
  DPPRCL_REPAIR_NORMALIZED_ADDRESS_V3 rt;
  DPPRCL_REPAIR_SERIAL_NUMBER_V3      bt;
} DPPRCL_REPAIR_EXTEND_FIELD_V3;

/**
 * @brief Hob Memory Post Package Repair Entry structure
 * @details Contains details for performing a repair
 */
typedef struct _DPPRCL_HOBMEM_REPAIR_REPORT_ENTRY_V3 {
  uint32_t DeviceTypeToRepair : 3;        ///< [2:0] Device Type to be repaired
                                          ///< (NOTE: This bit-field is fixed at this location)
                                          ///< 0 = DDR, 1 = Hob Memory, 2 and 3 Reserved for future use
  uint32_t Bank : 4;                      ///< [6:3] Bank {BG 3:2, BA 1:0}
  uint32_t Reserved1 : 1;                 ///< [7] MBZ
  uint32_t Column : 5;                    ///< [12:8] Column Address
  uint32_t Reserved2 : 1;                 ///< [13] MBZ
  uint32_t StackId : 2;                   ///< [15:14] Stack Id
  uint32_t Row : 15;                      ///< [30:16] Row Address
  uint32_t Valid : 1;                     ///< [31] Valid PPR entry (NOTE: This bit-field
                                          ///< is fixed at this location)
  uint32_t RepairType : 3;                ///< [2:0] Repair type (0=sPPR, 1=hPPR, 2=mPPR, 3=MBIST)
  uint32_t Reserved3 : 1;                 ///< [3] MBZ
  uint32_t MfrId : 4;                     ///< [7:4] Manufacturer Id
  uint32_t RepairResult : 8;              ///< [15:8] Result code of the repair
  uint32_t Channel : 7;                   ///< [22:16] Channel
  uint32_t Reserved4 : 2;                 ///< [24:23] MBZ
  uint32_t SubChannel : 1;                ///< [25] SubChannel (DDR5)
  uint32_t HardPPRDone : 1;               ///< [26] Set once after ABL performs a successful Hard repair
  uint32_t Reserved5 : 2;                 ///< [28:27] MBZ
  uint32_t Socket : 3;                    ///< [31:29] Socket
  uint32_t Reserved6: 32;                 ///< [31:0] MBZ
  DPPRCL_REPAIR_EXTEND_FIELD_V3 extfield; ///< Extend Field
} DPPRCL_HOBMEM_REPAIR_REPORT_ENTRY_V3;

/**
 * @brief DDR Post Package Repair Entry structure
 * @details Contains details for performing a repair
 */
typedef struct _DPPRCL_DDR_REPAIR_REPORT_ENTRY_V3 {
  uint32_t DeviceTypeToRepair : 3;        ///< [2:0] Device Type to be repaired
                                          ///< (NOTE: This bit-field is fixed at this location)
                                          ///< 0 = DDR, 1 = Hob Memory, 2 and 3 Reserved for future use
  uint32_t Bank : 5;                      ///< [7:3] Bank {BG 4:2,BA 1:0}
  uint32_t Device : 5;                    ///< [12:8] Device width
                                          ///<        Device = 4, Repair DIMMs with device width x4 only
                                          ///<        Device = 8, Repair DIMMs with device width x8 only
                                          ///<        Device = 16, Repair DIMMs with device width x16 only
                                          ///<        Device = 0x1F - Ignore device width and repair
                                          ///< specific devices based on TargetDevice
  uint32_t ChipSelect : 2;                ///< [14:13] ChipSelect
  uint32_t Column : 11;                   ///< [25:15] Column Address
  uint32_t TargetDevice : 5;              ///< [30:26] Target Device (only valid if Device = 0x1F)
                                          ///<         x4 (0-17 (with ECC)), x8 (0-8 (with ECC))
  uint32_t Valid : 1;                     ///< [31] Valid entry (NOTE: NOTE: This bit-field is
                                          ///< fixed at this location)
  uint32_t Row : 18;                      ///< [17:0] Row Address
  uint32_t RankMultiplier : 3;            ///< [20:18] Rank Multiplier
  uint32_t Channel : 4;                   ///< [24:21] Channel
  uint32_t SubChannel : 1;                ///< [25] Sub Channel. 0 - SubChannel A, 1 - Sub Channel B
  uint32_t HardPPRDone : 1;               ///< [26] Set Once after ABL performs a successful hard repair.
  uint32_t PPRUndo : 1;                   ///< [27] Set to request a PPR undo on DDR5
  uint32_t PPRLock : 1;                   ///< [28] Set to request a PPR Lock on DDR5
  uint32_t Socket : 3;                    ///< [31:29] Socket
  uint32_t RepairType : 3;                ///< [2:0] Repair type (0=sPPR, 1=hPPR, 2=mPPR, 3=MBIST)
  uint32_t ErrorCause : 3;                ///< [5:3] Cause of the error initiating post package repair
  uint32_t Reserved1 : 2;                 ///< [7:6] MBZ
  uint32_t RepairResult : 8;              ///< [15:8] Result code of the repair
  uint32_t Reserved2 : 16;                ///< [31:16] MBZ
  DPPRCL_REPAIR_EXTEND_FIELD_V3 extfield; ///< Extend Field

} DPPRCL_DDR_REPAIR_REPORT_ENTRY_V3;

/**
 * @brief DDR Post Package Repair Entry union (v3)
 * @details This union supports creating a PPR entry for DDR or Hob Memory
 */
typedef union _DPPRCL_REPAIR_REPORT_ENTRY_V3{
  DPPRCL_DDR_REPAIR_REPORT_ENTRY_V3    ddr;    ///< For DDR systems
  DPPRCL_HOBMEM_REPAIR_REPORT_ENTRY_V3 hobmem; ///< For Hob Memory systems
} DPPRCL_REPAIR_REPORT_ENTRY_V3;

#define APOB_MAX_DPPRCL_ENTRY 64          ///< Max post package repair list entries

/**
 * @brief Post Package Repair List
 * @details Contains all Post Package Repair List Entries
 *
 */
typedef struct _APOB_DPPRCL_STRUCT_V3{
  DPPRCL_REPAIR_REPORT_ENTRY_V3 DppRclReportEntry[APOB_MAX_DPPRCL_ENTRY];
} APOB_DPPRCL_STRUCT_V3;

#define APOB_MAX_DPPRCL_SOCKETS 1             ///< Max post package repair list sockets
#define APOB_MAX_DPPRCL_CHANNELS 1            ///< Max post package repair list channels
#define APOB_MAX_DPPRCL_ENTRIES \
        APOB_MAX_DPPRCL_SOCKETS *APOB_MAX_DPPRCL_CHANNELS  ///< Max post package
///< repair list entries based on
///< max sockets and channels

/**
 * @brief DDR Post Package Repair Struct
 *
 */
typedef struct _APOB_DPPR_STRUCT_V3 {
  bool       PprResultsValid;          /// Indicates that the PPR results are valid
  uint8_t    Reserved;
  uint16_t   Reserved1;
  APOB_DPPRCL_STRUCT_V3 Channel[APOB_MAX_DPPRCL_ENTRIES];
} APOB_DPPR_STRUCT_V3;

/**
 * @brief Interleaving settings
 *
 */
typedef union {
  /**
   * @brief Bitfields of Interleaving Settings
   *
   */
  struct {
    uint32_t ChannelIntlv:8;  ///< Bit map for channel interleave setting
    uint32_t DieIntlv:2;      ///< Bit map for die interleave setting
    uint32_t SocketIntlv:1;   ///< Socket interleave setting
    uint32_t :21;             ///< Reserved
  } Field;
  uint32_t  Value;            ///< Interleaving Settings as 32 bit value
} INTLV_SETTING;

/**
 * @brief APOB Memory Config Info
 *
 */
typedef struct _APOB_MEM_CFG_INFO {
  /**
   * @brief Union for status enabled flag or configured status value
   *
   */
  union {
    bool     Enabled;  ///< Status.Enabled - TRUE: Enabled.
    uint16_t Value;    ///< Status.Value - Configured value.
  } Status;
  uint16_t StatusCode; ///< Status Code.
} APOB_MEM_CFG_INFO;

/**
 * @brief AMD SYSTEM MEMORY Types
 *
 */
typedef enum {
  UMA,                        ///< UC DRAM cycles.
  MMIO,                       ///< Cycles are sent out to IO.
                              ///< Only expect the 1 below 4GB
  PrivilegedDRAM,             ///< Read-only 0xFFs.  No special
                              ///< cache considerations are needed.
                              ///< Map out of E820
  Reserved1TbRemap,           ///< Read-only 0xFFs.  This region
                              ///< is set to WB DRAM and
                              ///< cannot be used as MMIO.
  ReservedSLink,              ///< SLink connected memory is not
                              ///< available until later in POST
  ReservedSLinkAlignment,     ///< Read-only 0xFFs.  This region is set
                              ///< to WB DRAM and cannot be used as MMIO.
  ReservedDrtm,               ///< DRAM reserved for PSP use.
                              ///< Access is determined by the PSP.
  ReservedCvip,               ///< DRAM reserved for CVIP use.
  ReservedSmuFeatures,        ///< DRAM reserved for SMU features
  ReservedFwtpm,              ///< DRAM reserved for HSP
  ReservedMpioC20,            ///< DRAM reserved for C20 PHY
  ReservedNbif,               ///< DRAM reserved for NBIF
  ReservedCxl,                ///< CXL connected memory is not
                              ///< available until later in POST
  ReservedCxlAlignment,       ///< Read-only 0xFFs.  This region is
                              ///< set to WB DRAM and cannot be used as MMIO.
  ReservedCpuTmr,             ///< DRAM reserved for CPU TMR
  ReservedRasEinj,            ///< DRAM reserved for RAS EINJ
  ReservedABLOutput,          ///< DRAM reserved for ABL Output
  MaxMemoryHoleTypes,         ///< Not a valid type.  Used for validating the others.
} MEMORY_HOLE_TYPES;

/**
 * @brief AMD MEMORY_HOLE_DESCRIPTOR struct
 *
 */
typedef struct {
  uint64_t             Base;    ///< Full 64 bit base address of the hole
  uint64_t             Size;    ///< Size in bytes of the hole
  MEMORY_HOLE_TYPES    Type;    ///< Hole type
  uint32_t             _unused; ///< Don't rely on compiler aligning the next entry to 64bit
} MEMORY_HOLE_DESCRIPTOR;

/**
 * @brief AMD SYSTEM_MEMORY_MAP struct
 *
 */
typedef struct {
  uint64_t                  TopOfSystemMemory;  ///< Final DRAM byte address in the system + 1
  uint32_t                  NumberOfHoles;      ///< SoC15 systems will always have at least one
  uint32_t                  _unused;            ///< Don't rely on compiler aligning the next entry to 64bit
  MEMORY_HOLE_DESCRIPTOR    HoleInfo[1];        ///< Open ended array of descriptors
} SYSTEM_MEMORY_MAP;


/**
 * @brief AMD APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT Header
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;                ///< APOB Type Header

  SYSTEM_MEMORY_MAP   ApobSystemMap;               ///< APOB event log
  MEMORY_HOLE_DESCRIPTOR  AdditionalHoleInfo[17];  ///< Descriptors for memory holes in the memory map
} APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT;

/**
 * @brief APOB Memory DMI entry header
 *
 */
typedef struct {
  APOB_TYPE_HEADER ApobTypeHeader;            ///< APOB Type header
  uint8_t            MemoryType:7;            ///< Memory Type
  uint8_t            EccCapable:1;            ///< ECC Capable
  uint8_t            MaxPhysicalDimms;        ///< Maximum physical DIMMs
  uint8_t            MaxLogicalDimms;         ///< Maximum logical DIMMs
  uint8_t            Reserved;                ///< Reserved
} APOB_MEM_DMI_HEADER;

// XUSL_TRACEPOINT wrapper
#define APOB_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_APOB & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

/**
 * @brief AMD CXL_ADDR_MAP_INFO struct
 *
 */
typedef struct {
  uint64_t    Base;            ///< Base address of assigned region or 0xFFFFFFFF_FFFFFFFF
                               ///< if a problem occurred or if the input Size was zero.
  uint64_t    Size;            ///<  Raw size routed in DF (zero if a problem occurred)
  uint32_t    Status;          ///< see DF_SLINK_REGION_INIT_STATUS
  uint8_t     Socket;          ///< Zero means socket 0, One means socket 1
  uint8_t     PhysNbioMap;     ///< Bit n set means NBIO n on the given socket should be included in this region
  uint8_t     Alignment;       ///< Bit position to align to (e.g. 32 will result in the region being 4GB aligned)
  uint8_t     IntlvSize;       ///< Resulting region interleave size.
  uint8_t     SubIntlvMap[4];  ///< sub-link interleave bitfield
  uint8_t     SubIntlvSize;    ///< sub-link interleave size
  uint8_t     Type;            ///< 0 - Volatile; 1 - Persistent
} CXL_ADDR_MAP_INFO;
/**
 * @brief AMD APOB_SYSTEM_CXL_INFO_TYPE_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER     ApobTypeHeader;      ///< APOB Type Header
  CXL_ADDR_MAP_INFO    CxlInfo[8];          ///< CXL address map information
} APOB_SYSTEM_CXL_INFO_TYPE_STRUCT;

/**
 * @brief AMD APOB_SYSTEM_NPS_INFO_TYPE_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER     ApobTypeHeader;    ///< APOB Type Header
  uint8_t              RequestedNps;      ///< Requested NPS
  uint8_t              ActualNps;         ///< Actual NPS
  uint8_t              MinAllowableNps;   ///< Min allowable NPS
  uint8_t              MaxAllowableNps;   ///< Max allowable NPS
} APOB_SYSTEM_NPS_INFO_TYPE_STRUCT;

/**
 * @brief AMD LOGICAL_CORE_INFO
 *
 */
typedef struct {
  uint8_t    PhysCoreNumber;                                 ///< Physical Core Number
  bool       IsThreadEnabled[PROJ_MAX_CCX_THREADS_PER_CORE]; ///< Thread enabled flag per thread
} LOGICAL_CORE_INFO;

/**
 * @brief AMD LOGICAL_COMPLEX_INFO
 *
 */
typedef struct {
  uint8_t              PhysComplexNumber;                        ///< Physical Complex Number
  LOGICAL_CORE_INFO    CoreInfo[PROJ_MAX_CCX_CORES_PER_COMPLEX]; ///< Core info per core
} LOGICAL_COMPLEX_INFO;

/**
 * @brief AMD LOGICAL_CCD_INFO
 *
 */
typedef struct {
  uint8_t                 PhysCcdNumber;                          ///< Physical CCD Number
  LOGICAL_COMPLEX_INFO    ComplexMap[PROJ_MAX_COMPLEXES_PER_CCD]; ///< Complex Info per CCD
} LOGICAL_CCD_INFO;

/**
 * @brief AMD APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE Header
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;                        ///< APOB Type Header
  LOGICAL_CCD_INFO  CcdMap[PROJ_MAX_CCD_DIES_PER_SOCKET];  ///< CCD Maps
} APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT;

/**
 * @brief AMD APOB_SOC_DIE_INFO
 *
 */
typedef struct {
  uint8_t     MaxSocCcdsPerDieValue;
  uint8_t     MaxSocCcxPerCcdValue;
  uint8_t     MaxSocCoresPerComplexValue;
  uint8_t     MaxSocDiesPerSocketValue;
  uint8_t     MaxSocSocketsSupportedValue;
  uint8_t     MaxSocThreadPerCore;
  uint8_t     MaxSocCcdPerSocket;
} APOB_SOC_DIE_INFO;

SIL_STATUS
AmdGetApobEntryInstance (
  SIL_CONTEXT       *SilContext,
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          InstanceID,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntry
  );

SIL_STATUS
ApobGetApcbUpdate (
  APOB_APCB_UPDATES_STRUCT  *ApobEntry,
  uint32_t                  Token,
  uint32_t                  *Value
  );

/**
 * @brief DF CXL Region Init Status
 *
 */
typedef enum {
  CXL_ADDR_SUCCESS = 0,            ///< The requested CXL mapping was successfully created
  CXL_ADDR_OUT_OF_MAPS = 1,        ///< This region was not routed due to a shortage of DRAM map registers
  CXL_ADDR_INVALID_SOCKET = 2,     ///< The APCB passed in a 3rd and/or 4th region descriptor
                                   ///< attached to the same socket
  CXL_ADDR_INVALID_NBIO_MAP = 3,   ///< The APCB passed in 2 even or 2 odd numbered NBIOs for the same or2
                                   ///< socket, whether it is in the same descriptor different descriptors
  CXL_ADDR_INVALID_ALIGNMENT = 4,  ///< The requested alignment was not achievable.
  CXL_ADDR_INVALID_SIZE = 5        ///< The requested region was too large for the current configuration.
} FABRIC_CXL_REGION_INIT_STATUS;
