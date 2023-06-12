/**
 * @file ApobCmn.h
 * @brief OpenSIL APOB common definitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <Apob.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */

//Common structure across programs
#define APOB_SIGNATURE        0x424F5041
/// AMD APOB_HMAC
#define APOB_HMAC_SIZE 32
#define MAX_APOB_ERROR_ENTRIES  64
#define MAX_HOLE_DESCRIPTOR 18
#define MAX_ADDITIONAL_HOLE_DESCRIPTOR 17

// APOB Group Definitons
#define APOB_MEM      1
#define APOB_DF       2
#define APOB_CCX      3
#define APOB_GNB      4
#define APOB_FCH      5
#define APOB_PSP      6
#define APOB_GEN      7
#define APOB_SMBIOS   8
#define APOB_FABRIC   9

// APOB Type Definitons
#define APOB_MEM_GENERAL_ERRORS_TYPE   1
#define APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE   2
#define APOB_GEN_CONFIGURATION_INFO_TYPE   3
#define APOB_GEN_S3_REPLAY_BUFFER_INFO_TYPE   4
#define APOB_MEM_PMU_SMB_TYPE   5
#define APOB_GEN_EVENT_LOG_TYPE   6
#define APOB_MEM_DIMM_SMBUS_INFO_TYPE   7
#define APOB_MEM_SMBIOS_TYPE  8
#define APOB_SYS_MAP_INFO_TYPE   9
#define APOB_MEM_NVDIMM_INFO_TYPE   15
#define APOB_APCB_BOOT_INFO_TYPE    16
#define APOB_MEM_DIMM_SPD_DATA_TYPE     17
#define APOB_MEM_MBIST_RESULT_INFO_TYPE 18
#define APOB_SYS_NPS_INFO_TYPE   19
#define APOB_SYS_SLINK_INFO_TYPE   20
#define APOB_DF_DXIO_PHY_FW_OVERRIDE_INFO_TYPE 21
#define APOB_MEM_PMU_TRAINING_FAILURE_INFO_TYPE 22
#define APOB_ENV_FLAGS_INFO_TYPE 23
#define APOB_SYS_CXL_INFO_TYPE   24

#define APOB_MEM_S3_DDR_PHY_REPLAY_PHASE0_BUFFER_INFO_TYPE   30
#define APOB_MEM_S3_DDR_PHY_REPLAY_MAX_ENTRIES               10


// APOB CCX Type Definitons
#define APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE  1
#define APOB_CCX_EDC_THROTTLE_THRESH_TYPE      2
#define APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE  3

#define APOB_ENTRY_INSTANCE_MAX 0xFF
#define APOB_HMAC_SIZE 32
#define MAX_APOB_ERROR_ENTRIES  64

#define  EDC_THROTTLE_TYPE_INVALID  0
#define  EDC_THROTTLE_TYPE_L3       1
#define  EDC_THROTTLE_TYPE_CORE     2
#define  EDC_THROTTLE_TYPE_FP       3
#define  MAX_NUMBER_OF_THROTTLERS   3
#define  CCX_NOT_PRESENT (0xFF)

// XUSL_TRACEPOINT wrapper
#define APOB_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_APOB & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)

/**********************************************************************************************************************
 * Module data structure
 *
 */
typedef struct {
  uint8_t  ApobHmac[APOB_HMAC_SIZE];                      ///< APOB HMAC for secutity check of each type
} APOB_HMAC;

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

///
/// APOB SPD Data for each DIMM.
///
typedef struct _APOB_SPD_STRUCT {
  uint8_t  SocketNumber;          ///< Indicates the socket number
  uint8_t  ChannelNumber;         ///< Indicates the channel number
  uint8_t  DimmNumber;            ///< Indicates the channel number
  uint8_t   PageAddress;          ///< Indicates the 256 Byte EE Page the data belongs to
                                    ///<      0 = Lower Page
                                    ///<      1 = Upper Page
  bool DimmPresent;               ///< Indicates if the DIMM is present
  uint8_t  MuxPresent;            ///< SpdMux Present or not. if 1, then yes otherwise no
  uint8_t  MuxI2CAddress;         ///< MuxI2cAddress
  uint8_t  MuxChannel;            ///< MuxChannel no.
  uint32_t Address;               ///< SMBus address of the DRAM
  uint32_t SerialNumber;          ///< DIMM Serial Number
  uint32_t DeviceWidth:3;         ///< Device Width i.e. x4, x8, x16 and x32
  uint32_t DpprSupported:2;       ///< Dppr Support Present
  uint32_t SpprSupported:1;       ///< Sppd Support Present
  uint32_t Reserved:26;           ///< Reserved for Future Use
  uint8_t Data[512];              ///< Buffer for 256 Bytes of SPD data from DIMM
} APOB_SPD_STRUCT;


typedef struct {
  uint32_t   Type;
  uint32_t   OpnValue;
  uint32_t   AblProgrammedValue;
} EDC_THROTTLE_THRESHOLD;

/// Event log entry
typedef struct {
  uint32_t    EventClass;       ///< The severity of the event.
  uint32_t    EventInfo;        ///< Uniquely identifies the event.
  uint32_t    DataA;
  uint32_t    DataB;
} APOB_ERROR_LOG;

typedef struct {
  APOB_ERROR_LOG SilEvent[MAX_APOB_ERROR_ENTRIES];    ///< The entry itself.
} SIL_EVENT_STRUCT;

typedef struct {
  APOB_TYPE_HEADER   ApobTypeHeader;
  uint16_t           Count;             ///< The total number of active entries.
  SIL_EVENT_STRUCT ApobEventStruct;   ///< The entries.
} EVENT_LOG_STRUCT;

typedef struct _APOBLIB_INFO {
  bool       Supported;                ///<  Specify if APOB supported
  uint32_t   ApobSize;
  uint64_t   ApobAddr;                 ///<  The Address of APOB
} APOBLIB_INFO;

typedef struct {
  uint8_t PhysCoreNumber;
  bool    IsThreadEnabled[MAX_THREADS_PER_CORE];
} LOGICAL_CORE_INFO;

typedef struct {
  uint8_t            PhysComplexNumber;
  LOGICAL_CORE_INFO  CoreInfo[MAX_CORES_PER_COMPLEX];
} LOGICAL_COMPLEX_INFO;

/// AMD APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE Header
typedef struct {
  APOB_TYPE_HEADER      ApobTypeHeader;    ///< APOB Type Header
  LOGICAL_COMPLEX_INFO  ComplexMap[CCX_MAX_COMPLEXES_PER_DIE];
} APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT;

typedef struct {
  uint8_t               PhysCcdNumber;
  LOGICAL_COMPLEX_INFO  ComplexMap[MAX_COMPLEXES_PER_CCD];
} LOGICAL_CCD_INFO;

/// AMD APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE Header
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;    ///< APOB Type Header
  LOGICAL_CCD_INFO  CcdMap[MAX_CCDS_PER_DIE];
} APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT;

/// AMD APOB_GEN_INFO_PARAMETER_INFO_STRUCT
/// @brief This structure passes parameter information from ABL cold to ABL Warm/Resume
typedef struct {
  uint32_t ApobUniqueApcbInstance;  ///< UniqueApcbInstance to ensure
                                      ///< (ABL internal use for warm reset data transfer)
                                      ///< compatibitly for given flashed Host Firmware lifecycle
  bool     ApobAblRestoreControl;   ///< This field indicates if ABL should perform a restore
  uint8_t  SubProgram;              ///< Subprogram in PSP dir
  uint16_t BoardMask;
  uint32_t Reserved1;               ///< Reserved for alignment
} APOB_GEN_INFO_PARAMETER_INFO_STRUCT;

/// AMD APOB_GEN_INFO_ERROR_INFO_STRUCT
/// @brief This structure passes Error information from ABL cold to ABL Warm/Resume
typedef struct {
  bool ApobErrorReportPortReportingEnable;            ///< Indicates if ABL will report errors via a port
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< TRUE - Error logging will be reported via a port
                                                        ///< FALSE - Error logging will not be reported via a port
  bool ApobErrorReportUsingHandshakeEnable;           ///< This flag indicates if the ABL will use an handshake for the Error Log
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< TRUE - Error log reported using a handshake with the "ErrorLogOutputPort" and "ErrorLogInputPort"
                                                        ///< FALSE - Error log reported using "ErrorLogOutputPort" only with each DWORD in log delayed by
                                                        ///<         ErrorLogOutputDwordDelay
  uint16_t  Reserved1;                                ///< Reserved for alignment
  uint32_t  ApobErrorReportInputPort;                 ///< Input Port to receive ABL Error information
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< (only valid if ReportErrorLogUsingHandshakeEnable = TRUE)
  uint32_t  ApobErrorReportOutputDwordDelay;          ///< Number of "10ns" to wait before sending the next Log Dword informaiton via "ErrorLogOutputPort"
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< (only valid if ReportErrorLogUsingHandshakeEnable = FALSE)
  uint32_t  ApobErrorReportOutputPort;                ///< Output Port for ABL Error information
                                                        ///< (ABL internal use for resume/warm reset data xfer)
  bool    ApobErrorReportStopOnFirstFatalErrorEnable; ///< Indicates that ABL will stop on the first fatal error
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< TRUE - Stop and report the first FATAL error
                                                        ///< FALSE - Report all errors
  uint8_t   Reserved2;                                ///< Reserved for alignment
  uint16_t  Reserved3;                                ///< Reserved for alignment
  uint32_t  ApobErrorReportInputPortSize;             ///< Indicates the size of the input and outut port
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< 1 - 8 bit port
                                                        ///< 2 - 16 bit port
                                                        ///< 4 - 32 bit port
  uint32_t  ApobErrorReportOutputPortSize;            ///< Indicates the size of the input and outut port
                                                        ///< (ABL internal use for resume/warm reset data xfer)                                              ///< 1 - 8 bit port
                                                        ///< 2 - 16 bit port
                                                        ///< 4 - 32 bit port
  uint32_t  ApobErrorReportInputPortType;             ///< Indicates the type of Input Port or location of the port
                                                        ///< 0 - PCIE HT0
                                                        ///< 2 - PCIE HT1
                                                        ///< 5 - PCIE MMIO
                                                        ///< 6 - FCH_HT_IO (Default)
                                                        ///< 7 - FCH_MMIO
  uint32_t  ApobErrorReportOutputPortType;            ///< Indicates the type of Output Port or location of the port
                                                        ///< 0 - PCIE HT0
                                                        ///< 2 - PCIE HT1
                                                        ///< 5 - PCIE MMIO
                                                        ///< 6 - FCH_HT_IO (Default)
                                                        ///< 7 - FCH_MMIO
  bool      ApobErrorReportClearAcknowledgement;      ///< Indicates if the ABL will clear acknolgements during protocol
                                                        ///< (ABL internal use for resume/warm reset data xfer)
                                                        ///< TRUE - Clear acknowledgemetns
                                                        ///< FALSE - Do not clear acknologements
  bool      ApobErrorLogHeartBeatEnable;              ///< Indicates if ABL will provide periodic status to a port as a heart beat
                                                        ///< TRUE - Heartbeat Error log will be reported via a port
                                                        ///< FALSE - Heartbeat Error log will not be reported via a port
                                                        ///<
                                                        ///< Notes:
                                                        ///<   1) This feature is only valid if ErrorLogPortReportingEnable
                                                        ///<   2) This is can be mono-directional or bi-directional based on "ErrorLogReportUsingHandshakeEnable"
                                                        ///<        "ErrorLogReportUsingHandshakeEnable" = TRUE - bi-directional
                                                        ///<        "ErrorLogReportUsingHandshakeEnable" = FALSE - mono-directional
                                                        ///<   3) Requires the following to be defined:
                                                        ///<    - "ErrorLogReportInputPortType", "ErrorLogInputPort, "ErrorLogReportInputPortSize"
                                                        ///<    - "ErrorLogReportInputPortType", "ErrorLogInputPort, "ErrorLogReportInputPortSize"
} APOB_GEN_INFO_ERROR_INFO_STRUCT;

/// AMD APOB_GEN_CONFIGURATION_INFO Header
typedef struct {
  APOB_TYPE_HEADER ApobTypeHeader;    ///< APOB Type Header
  uint32_t Bootmode; ///< Bootmode detected by ABL.
  bool EmulationEnv; ///< Emulation Environment detected by ABL.
  bool SimulationEnv; ///< Simulation Environment detected by ABL.
  uint16_t Reserved; ///< Reserved for alignment
  APOB_GEN_INFO_ERROR_INFO_STRUCT ApobErrorInfo; ///< APOB error information
  APOB_GEN_INFO_PARAMETER_INFO_STRUCT ApobParamInfo; ///< APOB parameter information
  EVENT_LOG_STRUCT ApobEventLog;     ///< APOB event log
  uint8_t MemNbBlock[MAX_APOB_MEM_NB_BLOCK_SIZE];    ///> Memory NB block buffer
  uint8_t MemDctBlock[MAX_APOB_CHANNELS_PER_DIE][MAX_APOB_MEM_DCT_BLOCK_SIZE];              ///> Memory DCT block buffer
  uint8_t MemDieBlock[MAX_APOB_MEM_DIE_BLOCK_SIZE];    ///> Memory Die block buffer
} APOB_GEN_INFO_TYPE_STRUCT;

/// DDR4 DIMM ID information
typedef struct {
  uint16_t DimmManId;             ///< DIMM manufacturer ID
  uint16_t DimmModId;             ///< DIMM Module ID
  uint16_t Page1SpdChecksumLow;   ///< DIMM checksum for lower 128 bytes
  uint16_t Page1SpdChecksumHigh;  ///< DIMM checksum for upper 128 bytes
  bool     DimmPresentInConfig;   ///< Indicates that the DIMM config is present
  uint8_t  Reserved[3];
} DIMM_APOB_OPT_ID;

/// Defintion for APOB Boot Info to pass RecoveryFlag and ApcbInstance
typedef struct {
  APOB_TYPE_HEADER ApobTypeHeader;
  uint32_t    ApcbActiveInstance;          ///< APCB Active Insatnce of the boot
  DIMM_APOB_OPT_ID CurrentBootDimmIdInfo[MAX_SOCKETS_SUPPORTED][MAX_CHANNELS_PER_SOCKET][MAX_DIMMS_PER_CHANNEL];
  bool DimmConfigurationUpdated;       //< FALSE - DIMM Configuration not updated,
                                         //< TRUE - DIMM Configuration updated
  uint8_t     ApcbRecoveryFlag;            ///< APCB Recovery Flag
  uint8_t     ActionOnBistFailure;         ///< Action On BIST failure
  uint8_t     WorkloadProfile;             ///< Workload Profile
  uint32_t    LastPmuTrainTime;            ///< Last time memory get trained
  uint32_t    LastPartSerialNum0;          ///< Last part serial number 0
  uint32_t    LastPartSerialNum1;          ///< Last part serial number 1
  uint32_t    Reserved1;
} APOB_APCB_BOOT_INFO_STRUCT;

//
// C2PMSG_98  (SMN address 0x3810A88)
//
typedef union {
  struct {
    uint32_t DisableMemtest:1;              ///< Disable the memory test.
                                            ///< Replaces UmcCfgEco[16] force.
                                            ///< DEPRECATING: Will move to APCB
    uint32_t DisableMemclear:1;             ///< Disable the memory clear.
                                            ///< Replaces DF scratch 30[31]
                                            ///< force.
                                            ///< DEPRECATING: Already in APCB
    uint32_t DisableMemSetReg:1;            ///< Controls logging of UMC/PMU
                                            ///< register writes.  Replaces
                                            ///< UmcCfgEco[17] force.
                                            ///< DEPRECATING: Will create a
                                            ///< filter in APCB
    uint32_t DisableMemtestVerbose:1;       ///< Controls logging of memtest.
                                            ///< Replaces UmcCfgEco[17] force.
                                            ///< DEPRECATING: Will create a
                                            ///< filter in APCB
    uint32_t DisableExtSpd:1;               ///< Disable SPD read attempts
                                            ///< across SMBus.  Replaces
                                            ///< UmcCfgEco[20] force.
                                            ///< DEPRECATING: Will move to
                                            ///< APCB
    uint32_t DisableRrwMemtest:1;           ///< Disable reliable read-write
                                            ///< memtest.  Replaces
                                            ///< UmcCfgEco[21] force.
                                            ///< DEPRECATING: Will move to APCB
    uint32_t DisableSettingUnusedChannel:1; ///< Disable accesses to mem
                                            ///< channels other than 0.
                                            ///< Replaces UmcCfgEco[22] force.
                                            ///< DEPRECATING: Will consume
                                            ///< harvest fuses
    uint32_t DisableDxioPhyFwLoad:1;        ///< Disable MP0 DXIO phy firmware
                                            ///< loading process.  Replaces DF
                                            ///< scratch 30[31] force.
                                            ///< DEPRECATING: Will try to move
                                            ///< to APCB
    uint32_t DisableMemToUseDvSettings:1;   ///< Design verification settings.
                                            ///< Replaces UmcCfgEco[23] force.
                                            ///< DEPRECATING: Will be a build
                                            ///< time switch
    uint32_t EnableAblVerboseMessaging:1;   ///< verbose message control; 0- no
                                            ///< override (controlled by build
                                            ///< config),
                                            ///< 1- enable>
    uint32_t DisableSerialOut:1;            ///< serial out control; 0- no
                                            ///< override (controlled by build
                                            ///< config),
                                            ///< 1- disable>
    uint32_t EnableIoRedirect:1;            ///< Port 80 control. Replaces
                                            ///< C2PMSG_82[31] force; 0- no
                                            ///< override
                                            ///< (controlled by build config),
                                            ///< 1- enable>
    uint32_t FASTSIM_GIO:1;                 ///< FASTSIM_GIO
    uint32_t FASTSIM_DXIO:1;                ///< FASTSIM_DXIO
    uint32_t FASTSIM_DFLT_TBL:1;            ///< FASTSIM_DFLT_TBL
    uint32_t FASTSIM_SMU_MSGS:1;            ///< FASTSIM_SMU_MSGS
    uint32_t FASTSIM_PWR_MSGS:1;            ///< FASTSIM_PWR_MSGS
    uint32_t FASTSIM_PEI_LOG:1;             ///< FASTSIM_PEI_LOG
    uint32_t FASTSIM_DXE_LOG:1;             ///< FASTSIM_DXE_LOG
    uint32_t UseUmcHardcode:1;              ///< Use Hardcoded UMC Settings
    uint32_t UseDdrPhyHardcode:1;           ///< Use Hardcoded DDR PHY Settings
    uint32_t EmulatorType:2;                ///< Emulator Type. 00 - Palladium.
                                            ///< 01 - Veloce.
    uint32_t UseDfHardcode:1;               ///< Use Hardcoded DV DF settings
    uint32_t DisableMp1DxioPhyFwLoad:1;     ///< Disable phy FW loading for MP1
    uint32_t EnableCharPrintSocket1:1;      ///< Enable Socket 1 CharPrint to
                                            ///< C2P_MSG_99
    uint32_t :6;                            ///< Reserved
  } Field;
  uint32_t  Value;
} PRESIL_CTRL1;

/**
 * APOB Environment flags structures.
 *
 * openSIL does not use PreSilCtrl0, so it is not defined.
 *
 */
typedef struct {
  uint32_t               PreSilCtrl0;
  PRESIL_CTRL1           PreSilCtrl1;
} ENV_FLAGS_STRUCT;

/**
 * AMD APOB_ENV_FLAGS_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER           ApobTypeHeader;  ///< APOB Type Header
  ENV_FLAGS_STRUCT           Flags;           ///< Environment Flags
} APOB_ENV_FLAGS_STRUCT;

/**
 * @brief AMD SYSTEM MEMORY Types
 *
 */
typedef enum {
  UMA,                        ///< UC DRAM cycles.
  MMIO,                       ///< Cycles are sent out to IO.  Only expect the 1 below 4GB
  PrivilegedDRAM,             ///< Read-only 0xFFs.  No special cache considerations are needed.  Map out of E820
  Reserved1TbRemap,           ///< Read-only 0xFFs.  This region is set to WB DRAM and cannot be used as MMIO.
  ReservedSLink,              ///< SLink connected memory is not available until later in POST
  ReservedSLinkAlignment,     ///< Read-only 0xFFs.  This region is set to WB DRAM and cannot be used as MMIO.
  ReservedDrtm,               ///< DRAM reserved for PSP use.  Access is determined by the PSP.
  ReservedCvip,               ///< DRAM reserved for CVIP use.
  ReservedSmuFeatures,        ///< DRAM reserved for SMU features
  ReservedFwtpm,              ///< DRAM reserved for HSP
  ReservedMpioC20,            ///< DRAM reserved for C20 PHY
  ReservedNbif,               ///< DRAM reserved for NBIF
  ReservedCxl,                ///< CXL connected memory is not available until later in POST
  ReservedCxlAlignment,       ///< Read-only 0xFFs.  This region is set to WB DRAM and cannot be used as MMIO.
  ReservedCpuTmr,             ///< DRAM reserved for CPU TMR
  ReservedRasEinj,            ///< DRAM reserved for RAS EINJ
  MaxMemoryHoleTypes,         ///< Not a valid type.  Used for validating the others.
} MEMORY_HOLE_TYPES;

/**
 * @brief AMD MEMORY_HOLE_DESCRIPTOR struct
 *
 */
typedef struct {
  uint64_t             Base;    ///< Full 64 bit base address of the hole
  uint64_t             Size;    ///< Size in bytes of the hole
  MEMORY_HOLE_TYPES  Type;    ///< Hole type
  uint32_t                  _unused;          /// Don't rely on compiler aligning the next entry to 64bit
} MEMORY_HOLE_DESCRIPTOR;

/**
 * @brief AMD SYSTEM_MEMORY_MAP struct
 *
 */
typedef struct {
  uint64_t                  TopOfSystemMemory;  ///< Final DRAM byte address in the system + 1
  uint32_t                  NumberOfHoles;      ///< SoC15 systems will always have at least one
  uint32_t                  _unused;          /// Don't rely on compiler aligning the next entry to 64bit
  MEMORY_HOLE_DESCRIPTOR    HoleInfo [MAX_HOLE_DESCRIPTOR];  ///Descriptors for holes in the memory
} SYSTEM_MEMORY_MAP;

/**
 * @brief AMD APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT Header
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;                ///< APOB Type Header
  SYSTEM_MEMORY_MAP   ApobSystemMap;               ///< APOB event log
  MEMORY_HOLE_DESCRIPTOR  AdditionalHoleInfo [MAX_ADDITIONAL_HOLE_DESCRIPTOR];  ///Descriptors for holes in the memory
} APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT;

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
  uint8_t     IntlvSize;       ///< Resulting region interleave size.  See DF_MEM_INTLV_SIZE_VALIDVAL.
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

typedef struct {
  bool       Capable;
  bool       Enabled;
  uint32_t   CumulativeSize;
} FABRIC_INTERLEAVING_INFO;

typedef struct {
  FABRIC_INTERLEAVING_INFO  Socket;
  FABRIC_INTERLEAVING_INFO  EightWay[2];
  FABRIC_INTERLEAVING_INFO  SixWay[2];
  FABRIC_INTERLEAVING_INFO  FourWaySpecialCase[2];
  FABRIC_INTERLEAVING_INFO  FourWaySymmetric[2][2];
  FABRIC_INTERLEAVING_INFO  TwoWay[2][4];
  FABRIC_INTERLEAVING_INFO  None[2][8];
} FABRIC_SYSTEM_INTERLEAVING;
/**
 * @brief AMD APOB_SYSTEM_NPS_INFO_TYPE_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER              ApobTypeHeader;      ///< APOB Type Header
  uint8_t                       RequestedNps;        ///< Requested NPS
  uint8_t                       ActualNps;           ///< Actual NPS
  uint8_t                       MaxAllowableNps;     ///< Max allowable NPS
  FABRIC_SYSTEM_INTERLEAVING    SystemInterleaving;  ///< System Interleaving Info
} APOB_SYSTEM_NPS_INFO_TYPE_STRUCT;

/**********************************************************************************************************************
 * Declare Function prototypes
 *
 */

uint32_t
ApobGetApcbInstance (
  uint8_t  *ApcbInstance
  );

uint32_t
ApobGetApcbRecoveryFlag (
  bool  *ApcbRecoveryFlag
  );

uint32_t
ApobGetDimmCfgUpdatedFlag (
  bool  *DimmCfgUpdatedFlag
  );

//CcdIndex will be ignored in Pre Family 17h Model 30h
SIL_STATUS
ApobGetPhysCcdNumber (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint8_t *PhysCcdNumber
);

//CcdIndex will be ignored in Pre Family 17h Model 30h
SIL_STATUS
ApobGetPhysComplexNumber (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint8_t *PhysComplexNumber
);

//CcdIndex will be ignored in Pre Family 17h Model 30h
SIL_STATUS
ApobGetPhysCoreNumber (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint8_t  *PhysCoreNumber
);

//CcdIndex will be ignored in Pre Family 17h Model 30h
uint32_t
ApobGetIsThreadEnabled (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint32_t ThreadIndex,
  bool *IsThreadEnabled
);

uint32_t
ApobGetEccEnable (
  uint32_t ApobInstanceId,
  uint32_t ChannelIndex,
  bool *EccEnable
);

uint32_t
ApobGetNvdimmPresentInSystem (
  uint32_t ApobInstanceId,
  bool *NvdimmPresentInSystem
);

uint32_t
ApobGetNvdimmInfo (
  uint32_t ApobInstanceId,
  uint32_t **NvdimmInfo,
  uint32_t *NvdimmInfoSize
);

uint32_t
ApobGetMaxDimmsPerChannel (
  uint32_t ApobInstanceId,
  uint8_t *MaxDimmsPerChannel
);

uint32_t
ApobGetMaxChannelsPerDie (
  uint32_t ApobInstanceId,
  uint8_t *MaxChannelsPerDie
);

uint32_t
ApobGetMaxChannelsPerSocket (
  uint32_t ApobInstanceId,
  uint8_t *MaxChannelsPerSocket
);

uint32_t
ApobGetDimmSmbusInfo (
  uint32_t ApobInstanceId,
  uint32_t Index,
  APOB_SPD_STRUCT **DimmSmbusInfo
);

uint32_t
ApobGetMemClkFreq (
  uint32_t ApobInstanceId,
  uint16_t *MemClkFreq
);

uint32_t
ApobGetDdrMaxRate (
  uint32_t ApobInstanceId,
  uint16_t *DdrMaxRate
);

uint32_t
ApobGetMemGenInfoElement (
  uint32_t ApobInstanceId,
  uint32_t ElementId,
  uint16_t *ElementSizeInByte,
  uint64_t *ElementValue
  );

uint32_t
ApobGetNumberOfValidThresholds (
  uint32_t ApobInstanceId,
  uint32_t *NumberOfValidThresholds
  );

uint32_t
ApobGetThresholds (
  uint32_t ApobInstanceId,
  uint32_t Index,
  EDC_THROTTLE_THRESHOLD **Thresholds
  );

uint32_t
ApobGetBoardMask (
  uint32_t ApobInstanceId,
  uint16_t *BoardMask
  );

uint32_t
ApobGetSubProgram (
  uint32_t *SubProgram
  );

SIL_STATUS
ApobGetCcdLogToPhysMap (
  uint32_t                                      Socket,
  uint32_t                                      Die,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap
  );

SIL_STATUS
ApobGetCoreTopologyOnDie (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfCcds,
  uint32_t *NumberOfComplexes,
  uint32_t *NumberOfCores,
  uint32_t *NumberOfThreads
  );

/**
 * @brief Get specific SPD data by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 * @param ApobInstanceId
 * @param Socket
 * @param Channel
 * @param Dimm
 * @param BufSize
 * @param SpdBufPtr Point to buffer to hold SPD data
 *
 * @retval       SilPass           Function succeed
 * @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetDimmSpdData (
  uint32_t  ApobInstanceId,
  uint8_t   Socket,
  uint8_t   Channel,
  uint8_t   Dimm,
  uint32_t  BufSize,
  uint8_t   *SpdBufPtr
  );

SIL_STATUS ApobGetActionOnBistFailure (uint8_t *ActionOnBistFailure);

/**
 * AmdGetApobEntryInstance
 * @brief   Return Entry instance as specified GroupId, DataTypeId, InstanceId
 *
 * @details  Passing the APOB base address as a parameter is optional.
 *           ApobBaseAddress will not set the address beyond this Time Point.
 *           If the APOB address has been initialized in this time point then
 *           this argument will not be used.
 *
 * @param[in]  GroupID          GroupId of Apob entry
 * @param[in]  DataTypeID       DataTypeID of Apob entry
 * @param[in]  InstanceID       InstanceID of Apob entry
 * @param[in]  ApobBaseAddress  Location the the APOB optional
 * @param[out] ApobEntry        Point to the array which used to hold the ApobEntries
 *                              The array need be allocated outside of the routine
 *
 * @retval       SilPass        Function succeed
 * @retval       NON-ZERO       Error occurs
 */
SIL_STATUS
AmdGetApobEntryInstance (
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          InstanceID,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntry
  );

/// Domain type
typedef enum {
  NumaDram,
  NumaSLink,
  NumaCxl,
  MaxNumaDomainType2,
} FABRIC_DOMAIN_TYPE2;

/// Reported Domain Info
typedef struct {
  FABRIC_DOMAIN_TYPE2  Type;            ///< Type
  uint32_t      SocketMap;       ///< Bitmap indicating physical socket location
  uint32_t      PhysicalDomain;  ///< Physical domain number
} FABRIC_DOMAIN_INFO2;

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
  CXL_ADDR_INVALID_SIZE = 5,       ///< The requested region was too large for the current configuration.
} FABRIC_CXL_REGION_INIT_STATUS;

  /// Physical Dram Info
typedef struct {
  uint32_t   NormalizedCsMap;       ///< Bitmap of CSs comprising this physical domain
  uint32_t   SharingEntityCount;    ///< Number of virtual domains sharing this physical domain
  uint32_t   SharingEntityMap;      ///< Bitmap of reported domains that share this physical domain
  uint32_t   Reserved;              ///< Reserved
} FABRIC_PHYS_DOMAIN_INFO;

/**
 * @brief APOB Memory DMI entry header
 *
 */
typedef struct {
  APOB_TYPE_HEADER ApobTypeHeader;                       ///< APOB Type header
  uint8_t            MemoryType:7;                       ///< Memory Type
  uint8_t            EccCapable:1;                       ///< ECC Capable
  uint8_t            MaxPhysicalDimms;                   ///< Maximum physical DIMMs
  uint8_t            MaxLogicalDimms;                    ///< Maximum logical DIMMs
  uint8_t            Reserved;                           ///< Reserved
} APOB_MEM_DMI_HEADER;

typedef struct _APOB_D5_SPD_STRUCT {
  uint8_t                 DramDownSpdValid;               ///< Indicates that the DramDown SPD is valid
  uint8_t                 DimmPresent;                    ///< Indicates that the DIMM is present and Data is valid
  uint32_t                  Address;                      ///< SMBus address of the DRAM
  uint8_t                   SocketNumber;                 ///< Indicates the socket number
  uint8_t                   ChannelNumber;                ///< Indicates the channel number
  uint8_t                   DimmNumber;                   ///< Indicates the channel number
  uint8_t                 ShadowSpdValid;                 ///< Indicates whether the DIMM is present in the last boot
  uint8_t                   Data[1024];                   ///< Buffer for 1024 Bytes of SPD data from DIMM
} APOB_D5_SPD_STRUCT;

/**
 * @brief AMD APOB_MEM_DIMM_D5_SPD_DATA_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;  ///< APOB Type Header
  uint8_t MaxDimmsPerChannel;          ///< Max DIMMs per channel
  uint8_t MaxChannelsPerSocket;        ///< Max channels per socket
  // DIMM SMBUS Info for max number of DIMMS per die
  APOB_D5_SPD_STRUCT   DimmSmbusInfo[ABL_APOB_MAX_CHANNELS_PER_DIE * ABL_APOB_MAX_DIMMS_PER_CHANNEL];
} APOB_MEM_DIMM_D5_SPD_DATA_STRUCT;
