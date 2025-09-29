/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Apob-PHX.h
 * @brief OpenSIL APOB functions
 *
 */

#pragma once

#include <APOB/Common/ApobCmn.h>

/***********************************************************/

#define ABL_APOB_MAX_SOCKETS_SUPPORTED    1                               ///< Max number of sockets in system
#define ABL_APOB_MAX_CHANNELS_PER_SOCKET  4                               ///< Max Channels per sockets

// Program dependent
#define MAX_CCX_PER_CCD                   1                               ///< Max complexes per die
#define CCX_MAX_CORES_PER_COMPLEX         8                               ///< Max cores per complex
#define CCX_MAX_THREADS_PER_CORE          2                               ///< Max threads per core
#define CCX_MAX_DIES_PER_SOCKET           1                               ///< Max dies per sockets
#define CCX_MAX_SOCKETS                   ABL_APOB_MAX_SOCKETS_SUPPORTED  ///< Max number of supported sockets
#define MAX_CCDS_PER_DIE                  CCX_MAX_DIES_PER_SOCKET         ///< Max Dies per die?
#define MAX_COMPLEXES_PER_CCD             MAX_CCX_PER_CCD                 ///< MAX_CCX_PER_CCD


/**
 * @brief AMD APOB_GEN_INFO_PARAMETER_INFO_STRUCT
 * @details This structure passes parameter information from ABL cold to ABL Warm/Resume
 *
 */
typedef struct {
  uint32_t  ApobUniqueApcbInstance;   ///< UniqueApcbInstance to ensure
                                      ///< Compatibility for given flashed BIOS lifecycle
  bool      ApobAblRestoreControl;    ///< This field indicates if ABL should perform a restore
  uint8_t   SubProgram;               ///< Subprogram in PSP dir
  uint16_t  BoardMask;                ///< Board Mask
  uint32_t  Reserved1;                ///< Reserved for alignment
} APOB_GEN_INFO_PARAMETER_INFO_STRUCT;

/**
 * @brief AMD APOB_GEN_INFO_ERROR_INFO_STRUCT
 * @details This structure passes Error information from ABL cold to ABL Warm/Resume
 *
 */
typedef struct {
  bool ApobErrorReportPortReportingEnable;  ///< Indicates if ABL will report errors via a port
                                            ///< TRUE - Error logging will be reported via a port
                                            ///< FALSE - Error logging will not be reported via a port

  bool  ApobErrorReportUsingHandshakeEnable; ///< This flag indicates if the ABL will use an handshake for the
                                             ///< TRUE - Error log reported using a handshake with the
                                             ///< "ErrorLogOutputPort"
                                             ///<and "ErrorLogInputPort"
                                             ///< FALSE - Error log reported using "ErrorLogOutputPort"
                                             ///< only with each DWORD in log delayed by
                                             ///<         ErrorLogOutputDwordDelay

  uint16_t Reserved1;                       ///< Reserved for alignment

  uint32_t  ApobErrorReportInputPort;       ///< Input Port to receive ABL Error information
                                            ///< (only valid if ReportErrorLogUsingHandshakeEnable = TRUE)

  uint32_t  ApobErrorReportOutputDwordDelay; ///< Number of "10ns" to wait before sending the next Log Dword
                                             ///<information via "ErrorLogOutputPort"
                                             ///< (only valid if ReportErrorLogUsingHandshakeEnable = FALSE)

  uint32_t  ApobErrorReportOutputPort;      ///< Output Port for ABL Error information

  bool ApobErrorReportStopOnFirstFatalErrorEnable;  ///< Indicates that ABL will stop on the first fatal error
  ///< TRUE - Stop and report the first FATAL error
  ///< FALSE - Report all errors

  uint8_t Reserved2;                      ///< Reserved for alignment

  uint16_t Reserved3;                     ///< Reserved for alignment

  uint32_t  ApobErrorReportInputPortSize; ///< Indicates the size of the input and output port
                                          ///< 1 - 8 bit port
                                          ///< 2 - 16 bit port
                                          ///< 4 - 32 bit port

  uint32_t  ApobErrorReportOutputPortSize; ///< Indicates the size of the input and output port
                                           ///< 1 - 8 bit port
                                           ///< 2 - 16 bit port
                                           ///< 4 - 32 bit port

  uint32_t  ApobErrorReportInputPortType;  ///< Indicates the type of Input Port or location of the port
                                           ///< 0 - PCIE HT0
                                           ///< 2 - PCIE HT1
                                           ///< 5 - PCIE MMIO
                                           ///< 6 - FCH_HT_IO (Default)
                                           ///< 7 - FCH_MMIO

  uint32_t  ApobErrorReportOutputPortType; ///< Indicates the type of Output Port or location of the port
                                           ///< 0 - PCIE HT0
                                           ///< 2 - PCIE HT1
                                           ///< 5 - PCIE MMIO
                                           ///< 6 - FCH_HT_IO (Default)
                                           ///< 7 - FCH_MMIO

  bool  ApobErrorReportClearAcknowledgement; ///< Indicates if the ABL will clear acknowledgements
                                             ///< during protocol
                                             ///< TRUE - Clear acknowledgements
                                             ///< FALSE - Do not clear acknowledgements

  bool ApobErrorLogHeartBeatEnable;          ///< Indicates if ABL will provide periodic status to a port
                                             ///< as a heart beat
                                             ///< TRUE - Heartbeat Error log will be reported via a port
                                             ///< FALSE - Heartbeat Error log will not be reported via a port
                                             ///<
                                             ///< Notes:
                                             ///<   1) This feature is only valid if ErrorLogPortReportingEnable
                                             ///<   2) This is can be mono-directional or bi-directional based on
                                             ///<"ErrorLogReportUsingHandshakeEnable"
                                             ///<        "ErrorLogReportUsingHandshakeEnable" = TRUE - bi-directional
                                             ///<        "ErrorLogReportUsingHandshakeEnable" = FALSE - mono-directional
                                             ///<   3) Requires the following to be defined:
                                             ///<"ErrorLogReportInputPortType", "ErrorLogInputPort,
                                             ///< "ErrorLogReportInputPortSize"
                                             ///< - "ErrorLogReportInputPortType", "ErrorLogInputPort,
                                             ///< "ErrorLogReportInputPortSize"
} APOB_GEN_INFO_ERROR_INFO_STRUCT;

/**
 * @brief AMD APOB_MEM_DIMM_D5_SPD_DATA_STRUCT
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;    ///< APOB Type Header
  uint8_t MaxDimmsPerChannel;          ///< Max DIMMs per channel
  uint8_t MaxChannelsPerSocket;        ///< Max channels per socket
  APOB_D5_SPD_STRUCT   DimmSmbusInfo[ABL_APOB_MAX_CHANNELS_PER_DIE * ABL_APOB_MAX_DIMMS_PER_CHANNEL];
  ///< DIMM SMBUS Info for max number of DIMMS per die
} APOB_MEM_DIMM_D5_SPD_DATA_STRUCT;


#define MAX_APOB_MEM_NB_BLOCK_SIZE       1528
#define MAX_APOB_CHANNELS_PER_DIE          12  ///< Max channels per die
#define MAX_APOB_MEM_DIE_BLOCK_SIZE        72
#define MAX_APOB_MEM_DCT_BLOCK_SIZE      1116  ///< Memory DCT Block Buffer Size

/// AMD APOB_GEN_CONFIGURATION_INFO Header
typedef struct {
  APOB_TYPE_HEADER ApobTypeHeader;  ///< APOB Type Header
  uint32_t  Bootmode;
  ///< Bootmode detected by ABL.
  bool      Reserved0;
  bool      Reserved1;
  uint16_t                            Reserved2;
  ///< Reserved for alignment
  APOB_GEN_INFO_ERROR_INFO_STRUCT     ApobErrorInfo;
  ///< APOB error information
  APOB_GEN_INFO_PARAMETER_INFO_STRUCT ApobParamInfo;
  ///< APOB parameter information
  EVENT_LOG_STRUCT                    ApobEventLog;
  ///< APOB event log
  uint8_t   MemNbBlock[MAX_APOB_MEM_NB_BLOCK_SIZE];
  ///< Memory NB block buffer
  uint8_t   MemDctBlock[MAX_APOB_CHANNELS_PER_DIE][MAX_APOB_MEM_DCT_BLOCK_SIZE];
  ///< Memory DCT block buffer
  uint8_t   MemDieBlock[MAX_APOB_MEM_DIE_BLOCK_SIZE];
  ///< Memory Die block buffer
} APOB_GEN_INFO_TYPE_STRUCT;

/**
 * @brief Definition for APOB Boot Info to pass RecoveryFlag and ApcbInstance
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader; ///< APOB Type Header
  uint32_t    ApcbActiveInstance;   ///< APCB Active Instance of the boot
  DIMM_APOB_OPT_ID CurrentBootDimmIdInfo[ABL_APOB_MAX_SOCKETS_SUPPORTED]
  [ABL_APOB_MAX_CHANNELS_PER_SOCKET][ABL_APOB_MAX_DIMMS_PER_CHANNEL];
  ///< Current boot DIMM info
  bool DimmConfigurationUpdated;   ///< FALSE - DIMM Configuration not updated,
                                   ///< TRUE - DIMM Configuration updated
  uint8_t     ApcbRecoveryFlag;    ///< APCB Recovery Flag
  uint8_t     ActionOnBistFailure; ///< Action On BIST failure
  uint8_t     Reserved[1];         ///< Reserved for alignment
  uint32_t    LastPmuTrainTime;    ///< Last time memory get trained
  uint32_t    LastPartSerialNum0;  ///< Last part serial number 0
  uint32_t    LastPartSerialNum1;  ///< Last part serial number 1
  uint32_t    Reserved1;
} APOB_APCB_BOOT_INFO_STRUCT;

/**
 * @brief AMD LOGICAL_CORE_INFO
 *
 */
typedef struct {
  uint8_t    PhysCoreNumber;                             ///< Physical Core Number
  bool       IsThreadEnabled[CCX_MAX_THREADS_PER_CORE];  ///< Thread enabled flag per thread
} LOGICAL_CORE_INFO_PHX;

/**
 * @brief AMD LOGICAL_COMPLEX_INFO
 *
 */
typedef struct {
  uint8_t                  PhysComplexNumber;                   ///< Physical Complex Number
  LOGICAL_CORE_INFO_PHX    CoreInfo[CCX_MAX_CORES_PER_COMPLEX]; ///< Core info per core
} LOGICAL_COMPLEX_INFO_PHX;

typedef struct {
  uint8_t                     field0;
  LOGICAL_COMPLEX_INFO_PHX    field1[MAX_COMPLEXES_PER_CCD];
} SIL_RESERVED_STRUCT_0013;

/**
 * @brief AMD APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE Header
 *
 */
typedef struct {
  APOB_TYPE_HEADER      ApobTypeHeader;                        ///< APOB Type Header
  SIL_RESERVED_STRUCT_0013  CcdMap[CCX_MAX_CORES_PER_COMPLEX]; ///< CCD Maps
} APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT_PHX;

/**
 * @brief Memory DMI Type 20 - for memory use
 *
 */
typedef struct {
  uint8_t                     Socket:2;               ///< Socket ID
  uint8_t                     Channel:3;              ///< Channel ID
  uint8_t                     Dimm:2;                 ///< DIMM ID
  uint8_t                     DimmPresent:1;          ///< Dimm Present
  uint8_t                     Interleaved:1;          ///< Interleaving enabled
  uint8_t                     Reserved:7;             ///< Reserved
  uint16_t                    MemoryDeviceHandle;     ///< The handle, or instance number, associated with
                                                      ///< the Memory Device structure to which this address
                                                      ///< range is mapped.
  uint32_t                    StartingAddr;           ///< The physical address, in kilobytes, of a range
                                                      ///< of memory mapped to the referenced Memory Device.
  uint32_t                    EndingAddr;             ///< The handle, or instance number, associated with
                                                      ///< the Memory Device structure to which this address
                                                      ///< range is mapped.
  union {
    uint64_t    ExtStartingAddr;        ///< The physical address, in bytes, of a range of
                                        ///< memory mapped to the referenced Memory Device.
    struct {
      uint32_t                ExtStartingAddrLow;   ///< Lower 32 bits of the address
      uint32_t                ExtStartingAddrHigh;  ///< Upper 32 bits of the address
    } ExtStartingAddrForPsp;
  } UnifiedExtStartingAddr;
  union {
    uint64_t                  ExtEndingAddr;        ///< The physical ending
                                                    ///< address, in bytes, of the last of
                                                    ///< a range of addresses mapped
                                                    ///< to the referenced Memory Device
    struct {
      uint32_t                ExtEndingAddrLow;   ///< Lower 32 bits of the address
      uint32_t                ExtEndingAddrHigh;  ///< Upper 32 bits of the address
    } ExtEndingAddrForPsp;
  } UnifiedExtEndingAddr;
} APOB_MEM_DMI_LOGICAL_DIMM_PHX;

/**
 * @brief Memory DMI Type 17 - for memory use
 *
 */
typedef struct {
  uint8_t                     Socket:2;               ///< Socket ID
  uint8_t                     Channel:3;              ///< Channel ID
  uint8_t                     Dimm:2;                 ///< DIMM ID
  uint8_t                     DimmPresent:1;          ///< Dimm Present
  uint8_t                     SpdAddr;                ///< SPD Address
  uint16_t                    Handle;                 ///< The temporary handle, or instance
                                                      ///< number, associated with the structure
  uint16_t                    ConfigSpeed;            ///< Configured memory clock speed
  uint16_t                    ConfigVoltage;          ///< Configured voltage for this device, in millivolts
} APOB_MEM_DMI_PHYSICAL_DIMM_PHX;

/**
 * @brief APOB Memory General Config Info Struct
 *
 */
typedef struct {
  APOB_TYPE_HEADER  ApobTypeHeader;                  ///< APOB Type Header
  uint32_t BootMediaTypeInfo;                        ///< BootMediaTypeInfo
  uint16_t MemClkFreq;                               ///< Data from MemClkFreq:
                                                     ///< 667/800/933/1067/1200/1333/1467/1600 Mhz
  uint16_t MemPORMaxSpeed;                           ///< DDR POR speed in Mhz on
                                                     ///< current DIMM configuration
  uint16_t DdrMaxRate;                               ///< UMC DdrMaxRate
  bool     EccEnable[ABL_APOB_MAX_CHANNELS_PER_DIE]; ///< ECC enabled or Disabled per
                                                     ///< channel. ie. Channel 0 or Channel 1
  bool     Reserved0[ABL_APOB_MAX_CHANNELS_PER_DIE]; ///< Reserved
  bool     ChannelIntlvEn;                           ///< DIMM channel interleave status
  uint8_t  Reserved1[7];                             ///< Reserved
  APOB_DPPR_STRUCT_V3 DdrPostPackageRepair;          ///< DDR Post package repair

  INTLV_SETTING InterLeaveCurrentMode;                ///< Current interleave mode

  INTLV_SETTING InterLeaveCapability;                 ///< Capability of interleave

  uint32_t InterLeaveSize;                            ///< Interleaving size
  uint32_t DimmSize[ABL_APOB_MAX_CHANNELS_PER_DIE][ABL_APOB_MAX_DIMMS_PER_CHANNEL];
  ///< 2d array of DIMM sizes per channel per die
  APOB_MEM_CFG_INFO ChipselIntlv[ABL_APOB_MAX_CHANNELS_PER_DIE]; /// Chipselect
                                                                 ///< interleaving per channel
  APOB_MEM_CFG_INFO DramEcc;                                     ///< DRAM ECC Configuration
  APOB_MEM_CFG_INFO DramParity;                                  ///< DRAM Parity Configuration
  APOB_MEM_CFG_INFO AutoRefFineGranMode;                         ///< AutoRefresh Fine Granularity
                                                                 ///< Mode Configuration
  APOB_MEM_CFG_INFO MbistTestEnable;                             ///< MBIST Test Enable Configuration
  APOB_MEM_CFG_INFO MbistAggressorEnable;                        ///< MBIST Aggressor Enable Configuration
  APOB_MEM_CFG_INFO MbistPerBitSecondaryDieReport;               ///< MBIST Per Bit Secondary Die Report Configuration
  APOB_MEM_CFG_INFO DramTempControlledRefreshEn;                 ///< DRAM Temperature Controlled
                                                                 ///< Refresh Configuration
  APOB_MEM_CFG_INFO UserTimingMode;                              ///< User Timing Mode Configuration
  APOB_MEM_CFG_INFO UserTimingValue;                             ///< User Timing Value Configuration
  APOB_MEM_CFG_INFO MemBusFreqLimit;                             ///< Memory Bus Frequency Limit Configuration
  APOB_MEM_CFG_INFO EnablePowerDown;                             ///< Enable Power Down Configuration
  APOB_MEM_CFG_INFO DramDoubleRefreshRate;                       ///< DRAM Double Refresh Rate Configuration
  APOB_MEM_CFG_INFO PmuTrainMode;                                ///< PMU Training Mode Configuration
  APOB_MEM_CFG_INFO EccSymbolSize;                               ///< ECC Symbol Size Configuration
  APOB_MEM_CFG_INFO UEccRetry;                                   ///< Uncorrectable ECC Retry Configuration
  APOB_MEM_CFG_INFO IgnoreSpdChecksum;                           ///< Ignore SPD Checksum Configuration
  APOB_MEM_CFG_INFO EnableBankGroupSwapAlt;                      ///< Bank Group Swap Alt Control Configuration
  APOB_MEM_CFG_INFO EnableBankGroupSwap;                         ///< Bank Group Swap Control Configuration
  APOB_MEM_CFG_INFO DdrRouteBalancedTee;                         ///< Motherboard DDR Routing Configuration
  APOB_MEM_CFG_INFO DimmPowerSource;                             ///< DIMM Power Source Configuration
                                                                 ///< 1: device, 2: host
  APOB_MEM_CFG_INFO OdtsCmdThrotEn;                              ///< ODTS Command Throttle Configuration
  APOB_MEM_CFG_INFO OdtsCmdThrotCyc;                             ///< ODTS Command Throttle Cycle Configuration
} APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_PHX;
