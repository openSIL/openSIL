/**
 * @file
 * @brief OpenSIL APOB struct definitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define MAX_SOCKETS_SUPPORTED   2   ///< Max number of sockets in system
#define MAX_DIES_PER_SOCKET     4   ///< Max number of dies per socket
#define MAX_CHANNELS_PER_DIE    2   ///< Max Channels per die
#define MAX_CHANNELS_PER_SOCKET 12  ///< Max Channels per sockets
#define MAX_DIMMS_PER_CHANNEL   2   ///< Max DIMMs on a memory channel (independent of platform)
#define MAX_RBS_PER_SOCKET      12  ///< Max number of root bridges per socket

#define MAX_THREADS_PER_CORE    2
#define MAX_COMPLEXES_PER_CCD   2   ///< Program dependent

#define ABL_APOB_MAX_SOCKETS_SUPPORTED   2   ///< Max number of sockets in system
#define ABL_APOB_MAX_DIES_PER_SOCKET     1   ///< Max dies per socket
#define ABL_APOB_MAX_CHANNELS_PER_SOCKET 12  ///< Max Channels per sockets
#define ABL_APOB_MAX_CHANNELS_PER_DIE    12  ///< Max channels per die
#define ABL_APOB_MAX_DIMMS_PER_CHANNEL   2   ///< Max dimms per die
#define ABL_APOB_MAX_CS_PER_CHANNEL      4   ///< Max CS per channel
#define MAX_PMU_SMB_SIZE  0x400   ///< Max PMU SMB size

#define APOB_BASE_ADDRESS          CONFIG_PLAT_APOB_ADDRESS

#define CCX_MAX_SOCKETS            ABL_APOB_MAX_SOCKETS_SUPPORTED
#define CCX_MAX_DIES_PER_SOCKET    1   // Program dependent
#define CCX_MAX_COMPLEXES_PER_DIE  2   // Program dependent
#define CCX_MAX_CORES_PER_COMPLEX  8
#define CCX_MAX_THREADS_PER_CORE   2
#define CCX_NOT_PRESENT (0xFF)


#define MAX_CORES_PER_COMPLEX      8
#define MAX_CCDS_PER_DIE           12  ///< Program dependent
#define MAX_APOB_CHANNELS_PER_DIE  12  ///< Max channels per die

#define MAX_APOB_MEM_NB_BLOCK_SIZE       1528
#define MAX_APOB_MEM_DCT_BLOCK_SIZE      976
#define MAX_APOB_MEM_TIMINGS_BLOCK_SIZE  360
#define MAX_APOB_MEM_CHANNEL_BLOCK_SIZE  450
#define MAX_APOB_MEM_DIE_BLOCK_SIZE      108

/**
 * @brief Memory DMI Type 17 - for memory use
 *
 */
typedef struct {
  uint8_t                     Socket:2;               ///< Socket ID
  uint8_t                     Channel:4;              ///< Channel ID
  uint8_t                     Dimm:1;                 ///< DIMM ID
  uint8_t                     DimmPresent:1;          ///< Dimm Present
  uint8_t                     SpdAddr;                ///< SPD Address
  uint16_t                    Handle;                 ///< The temporary handle, or instance number, associated
                                                      ///< with the structure
  uint16_t                    ConfigSpeed;            ///< Configured memory clock speed
  uint16_t                    ConfigVoltage;          ///< Configured voltage for this device, in millivolt
} APOB_MEM_DMI_PHYSICAL_DIMM;

/**
 * @brief Memory DMI Type 20 - for memory use
 *
 */
typedef struct {
  uint8_t                     Socket:2;               ///< Socket ID
  uint8_t                     Channel:4;              ///< Channel ID
  uint8_t                     Dimm:1;                 ///< DIMM ID
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
  /**
   * @brief Ext Starting Address
   * @details Union for 64 bit address or struct of upper and lower 32 bits
   *
   */
  union {
    uint64_t                  ExtStartingAddr;        ///< The physical address, in bytes, of a range of
                                                        ///< memory mapped to the referenced Memory Device.
    /**
     * @brief Struct for the address upper and lower 32 bits
     *
     */
    struct {
      uint32_t                ExtStartingAddrLow;   ///< Lower 32 bits of the address
      uint32_t                ExtStartingAddrHigh;  ///< Upper 32 bits of the address
    } ExtStartingAddrForPsp;
  } UnifiedExtStartingAddr;

  /**
   * @brief Ext Ending Address
   * @details Union for 64 bit address or struct of upper and lower 32 bits
   *
   */
  union {
    uint64_t                  ExtEndingAddr;          ///< The physical ending address, in bytes, of the last of
                                                      ///< a range of addresses mapped to the referenced Memory Device
    /**
     * @brief Struct for the address upper and lower 32 bits
     *
     */
    struct {
      uint32_t                ExtEndingAddrLow;   ///< Lower 32 bits of the address
      uint32_t                ExtEndingAddrHigh;  ///< Upper 32 bits of the address
    } ExtEndingAddrForPsp;
  } UnifiedExtEndingAddr;
} APOB_MEM_DMI_LOGICAL_DIMM;

/**
 * @brief Post Package Repair List Entry struct
 *
 */
typedef union DPPRCL_REPAIR_REPORT_ENTRY_V2 {
  struct {
    uint32_t Valid: 1 ;                     ///< [0:0] Valid entry
    uint32_t Status: 8 ;                    ///< [8:1] Status
    uint32_t RepairType: 3 ;                ///< [11:9] Repair Type 0 - Soft,
                                            ///< 1 - Hard, 2 - MBIST  SoftPPR
    uint32_t ErrorCause: 3 ;                ///< [14:12] Cause of error which initiated this repair
    uint32_t DeviceTypeRepaired : 1;        ///< [15:15] Device Type which was repaird. 0  - DDR, 1 - HBM
    uint32_t DeviceWidth: 5 ;               ///< [20:16] Device WIdth
    uint32_t TargetDevice: 5 ;              ///< [25:21] Target Device
    uint32_t BankAddress: 5 ;               ///< [30:26] Bank Address
    uint32_t Reserved_0: 1 ;                ///< [31:31] Reserved

    uint32_t Socket: 3 ;                    ///< [2:0] Socket Number
    uint32_t Channel: 4 ;                   ///< [6:3] DDR Channel
    uint32_t SubChannel: 1 ;                ///< [7:7] Sub Channel 0 - Subchannel A, 1 - Subchannel B
    uint32_t ChipSelect: 2 ;                ///< [9:8] ChipSelect 0 -3
    uint32_t RankMultiplier: 3 ;            ///< [12:10] Rank Multiplier
    uint32_t RowAddress: 18 ;               ///< [30:13] RowAddress
    uint32_t Reserved_1: 1 ;                ///< [31:31] Reserved
} ddr;
  struct {
    uint32_t Valid: 1 ;                     ///< [0:0] Valid entry
    uint32_t Status: 8 ;                    ///< [8:1] Status
    uint32_t RepairType: 3 ;                ///< [11:9] Repair Type 0 - Soft,
                                            ///< 1 - Hard, 2 - MBIST  SoftPPR
    uint32_t ErrorCause: 3 ;                ///< [14:12] Cause of error which initiated this repair
    uint32_t DeviceTypeRepaired : 1;        ///< [15:15] Device Type which was repaird. 0  - DDR, 1 - HBM
    uint32_t Socket : 3;                    ///< [18:16] Socket
    uint32_t Channel : 4;                   ///< [22:19] Channel
    uint32_t SubChannel : 1;                ///< [23:23] SubChannel (DDR5)
    uint32_t MfrId : 4;                     ///< [27:24] Manufacturer Id
    uint32_t Reserved_0 : 4;                ///< [31:28] MBZ

    uint32_t BankAddress: 5 ;               ///< [4:0] Bank Address
    uint32_t Column : 5;                    ///< [9:5] Column Address
    uint32_t Row : 15;                      ///< [24:10] Row Address
    uint32_t Reserved_1:7;                  ///< [31:25] Reserved for hbm PPR repair update
  } hbm;
} DPPRCL_REPAIR_REPORT_ENTRY_V2;

#define APOB_MAX_DPPRCL_ENTRY 64                        ///< Max post package repair list entires
#define APOB_MAX_DPPRCL_SOCKETS 1                       ///< Max post package repair list sockets
#define APOB_MAX_DPPRCL_CHANNELS 1                      ///< Max post package repair list channels

/// Max post package repair list entries based on max sockets and channels
#define APOB_MAX_DPPRCL_ENTRIES APOB_MAX_DPPRCL_SOCKETS*APOB_MAX_DPPRCL_CHANNELS

/**
 * @brief Post Package Repair List
 * @details Contains all Post Package Repair List Entries
 *
 */
typedef struct _APOB_DPPRCL_STRUCT_V2 {
  DPPRCL_REPAIR_REPORT_ENTRY_V2 DppRclReportEntry [APOB_MAX_DPPRCL_ENTRY];
} APOB_DPPRCL_STRUCT_V2;

/**
 * @brief DDR Post Package Repair Struct
 *
 */
typedef struct _APOB_DPPR_STRUCT_V2 {
  bool       PprResultsValid;          /// Indicates that the PPR results are valid
  uint8_t    Reserved;
  uint16_t   Reserved1;
  APOB_DPPRCL_STRUCT_V2 Channel [APOB_MAX_DPPRCL_ENTRIES];
} APOB_DPPR_STRUCT_V2;

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
  uint32_t   Value;           ///< Interleaving Settings as 32 bit value
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
    bool     Enabled;       ///< Status.Enabled - TRUE: Enabled.
    uint16_t Value;         ///< Status.Value - Configured value.
  } Status;
  uint16_t StatusCode;      ///< Status Code.
} APOB_MEM_CFG_INFO;

/**
 * @brief APOB Memory General Config Info Struct
 *
 */
typedef struct {
  uint8_t ApobTypeHeader [48];                     ///< APOB Type Header
  uint32_t BootMediaTypeInfo;                      ///< BootMediaTypeInfo
  uint16_t MemClkFreq;                             ///< Data from MemClkFreq: 667/800/933/1067/1200/1333/1467/1600 Mhz
  uint16_t DdrMaxRate;                             ///< UMC DdrMaxRate
  bool EccEnable [ABL_APOB_MAX_CHANNELS_PER_DIE];  ///< ECC enabled or Disabled per channel. ie. Channel 0 or Channel 1
  bool NvDimmInstalled[ABL_APOB_MAX_CHANNELS_PER_DIE];  ///< NVDIMM presence per channel. ie. Channel 0 or Channel 1
  bool ChannelIntlvEn;                                  ///< DIMM channel interleave status
  uint8_t  Reserved1[7];                          ///< Reserved
  APOB_DPPR_STRUCT_V2 DdrPostPackageRepair;       ///< DDR Post package repaair
  INTLV_SETTING InterLeaveCurrentMode;            ///< Current interleave mode
  INTLV_SETTING InterLeaveCapability;             ///< Capability of interleave
  uint32_t InterLeaveSize;                        ///< Interleaving size
  uint32_t DimmSize[ABL_APOB_MAX_CHANNELS_PER_DIE][ABL_APOB_MAX_DIMMS_PER_CHANNEL]; ///< 2d array of DIMM sizes per
                                                                                    ///< channel per die
  APOB_MEM_CFG_INFO ChipselIntlv[ABL_APOB_MAX_CHANNELS_PER_DIE];                    ///< Chipselect interleaving per
                                                                                    ///< channel
  APOB_MEM_CFG_INFO DramEcc;                      ///< DRAM ECC Configuration
  APOB_MEM_CFG_INFO DramParity;                   ///< DRAM Parity Configuration
  APOB_MEM_CFG_INFO AutoRefFineGranMode;          ///< AutoRefresh Fine Granularity Mode Configuration
  APOB_MEM_CFG_INFO MbistTestEnable;              ///< MBIST Test Enable Configuration
  APOB_MEM_CFG_INFO MbistAggressorEnable;         ///< MBIST Aggressor Enable Configuration
  APOB_MEM_CFG_INFO MbistPerBitSecondaryDieReport;    ///< MBIST Per Bit Secondary Die Report Configuration
  APOB_MEM_CFG_INFO DramTempControlledRefreshEn;  ///< DRAM Temperature Controlled Refresh Configuration
  APOB_MEM_CFG_INFO UserTimingMode;               ///< User Timing Mode Configuration
  APOB_MEM_CFG_INFO UserTimingValue;              ///< User Timing Value Configuration
  APOB_MEM_CFG_INFO MemBusFreqLimit;              ///< Memory Bus Frequency Limit Configuration
  APOB_MEM_CFG_INFO EnablePowerDown;              ///< Enable Power Down Configuration
  APOB_MEM_CFG_INFO DramDoubleRefreshRate;        ///< DRAM Double Refresh Rate Configuration
  APOB_MEM_CFG_INFO PmuTrainMode;                 ///< PMU Training Mode Configuration
  APOB_MEM_CFG_INFO EccSymbolSize;                ///< ECC Symbol Size Configuration
  APOB_MEM_CFG_INFO UEccRetry;                    ///< Uncorrectable ECC Retry Configuration
  APOB_MEM_CFG_INFO IgnoreSpdChecksum;            ///< Ignore SPD Checksum Configuration
  APOB_MEM_CFG_INFO EnableBankGroupSwapAlt;       ///< Bank Group Swap Alt Control Configuration
  APOB_MEM_CFG_INFO EnableBankGroupSwap;          ///< Bank Group Swap Control Configuration
  APOB_MEM_CFG_INFO DdrRouteBalancedTee;          ///< Motherboard DDR Routing Configuration
  APOB_MEM_CFG_INFO NvdimmPowerSource;            ///< NVDIMM Power Source Configuration 1: device, 2: host
  APOB_MEM_CFG_INFO OdtsCmdThrotEn;               ///< ODTS Command Throttle Configuration
  APOB_MEM_CFG_INFO OdtsCmdThrotCyc;              ///< ODTS Command Throttle Cycle Configuration
} APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT;
