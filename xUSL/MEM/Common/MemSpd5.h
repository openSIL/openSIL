/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemSpd5.h
 * @brief This file contains DDR5 SPD Structures
 */

#pragma once

#include <SilCommon.h>

#pragma pack(push, 1)

/// Version of the SPD Data Supported by this Header File
#define SPD_DOCUMENT_REVISION 0x09

/// 16 Bit SPD Values
typedef union {
  struct {
    uint8_t Lsb;
    uint8_t Msb;
  } byte;
  uint16_t Value;
} SPD_VALUE16;

/// General timing structure
typedef union {
  struct {
    uint8_t Lsb       : 8;
    uint8_t Msb       : 8;
  } Field;
  uint16_t Value;
} SPD_TIMING_S;

/// JEP106 Manufacturer's ID Structure
typedef union {
  struct {
    uint8_t NumContCodes  : 7;
    uint8_t OddParity     : 1;
    uint8_t LastNonZero;
  } Field;
  uint16_t Value;
} SPD_MANUFACTURER_ID_S;

/// SPD Blocks
#define SPD_NUM_BLOCKS        16
#define SPD_BLOCK_LEN         64

typedef enum {
  SpdBlock_BaseConfig_0 = 0x00,
  SpdBlock_BaseConfig_1 = 0x01,
  SpdBlock_Reserved_2 = 0x02,
  SpdBlock_ModuleParms_0 = 0x03,
  SpdBlock_ModuleParms_1 = 0x04,
  SpdBlock_ModuleParms_2 = 0x05,
  SpdBlock_ModuleParms_3 = 0x06,
  SpdBlock_Reserved_7 = 0x07,
  SpdBlock_MfgInfo0 = 0x08,
  SpdBlock_MfgInfo1 = 0x09,
  SpdBlock_EndUserProgrammable0 = 0x0A,
  SpdBlock_EndUserProgrammable1 = 0x0B,
  SpdBlock_EndUserProgrammable2 = 0x0C,
  SpdBlock_EndUserProgrammable3 = 0x0D,
  SpdBlock_EndUserProgrammable4 = 0x0E,
  SpdBlock_EndUserProgrammable5 = 0x0F
} DDR5_SPD_BLOCK_NAMES;

typedef struct {
  uint8_t Data[SPD_BLOCK_LEN];
} SPD_BLOCK_S;

#define SPD_NUM_BYTES       0

typedef union {
  struct {
    uint8_t Reserved_30      : 4;
    uint8_t SpdBytesTotal    : 3;   ////< SPD Bytes Total
    uint8_t Reserved_7       : 1;
  } Field;
  uint8_t Value;
} SPD_NUM_BYTES_S;

#define SPD_DECODE_BYTES_TOTAL(val)  (128 << val)

/// SPD Revision for Base Configuration Parameters
#define SPD_REVISION        1

typedef union {
  struct {
    uint8_t SpdRevisionAdditions         : 4;
    uint8_t SpdRevisionEncodings         : 4;
  } Field;
  uint8_t Value;
} SPD_REVISION_S;

/// Key Byte / Host Bus Command Protocol Type
#define SPD_KEY_BYTE        2

typedef union {
  struct {
    uint8_t ModuleType      : 8;  /// SDRAM / Module Type Corresponding to Key Byte
  } Field;
  uint8_t Value;
} SPD_KEY_BYTE_S;

/// Key Byte / Module Type
#define SPD_KEY_BYTE2       3

/// be documented with those bytes in the base section.
typedef union {
  struct {
    uint8_t BaseModuleType  : 4;  /// Base Module Type
    uint8_t HybridMedia     : 3;  /// Hybrid Media
    uint8_t Hybrid          : 1;  /// Hybrid
  } Field;
  uint8_t Value;
} SPD_KEY_BYTE2_S;

#define SPD_BASEMODULE_RDIMM            1
#define SPD_BASEMODULE_UDIMM            2
#define SPD_BASEMODULE_SODIMM           3
#define SPD_BASEMODULE_LRDIMM           4
#define SPD_BASEMODULE_SORDIMM          8
#define SPD_BASEMODULE_DDIMM            10
#define SPD_BASEMODULE_SOLDEREDDOWN     11

typedef enum {
  BaseModReserved0,
  BaseModRdimm,
  BaseModUdimm,
  BaseModSodimm,
  BaseModLrdimm,
  BaseModReserved5,
  BaseModReserved6,
  BaseModReserved7,
  BaseModSoRdimm,
  BaseModReserved9,
  BaseModDDimm,
  BaseModSolderDown,
  BaseModBaseModuleTypeMax
} BASE_MODULE_TYPE;

typedef enum {
  SpdHybridMediaNotHybrid,
  SpdHybridMediaNvdimmN,
  SpdHybridMediaNvdimmP,
  SpdHybridMediaMax
} HYBRID_MEDIA;

typedef enum {
  SpdNotHybrid,
  SpdHybrid
} HYBRID;

/// First SDRAM Density and Package
#define SPD_FIRST_DENSITY_PACKAGE    4

typedef union {
  struct {
    uint8_t Density         : 5;  /// SDRAM Density Per Die
    uint8_t DiePerPkg       : 3;  /// Die Per Package
  } Field;
  uint8_t Value;
} SPD_DENSITY_PACKAGE_S;

typedef enum {
  Density_NoMemory,
  Density_4Gb,
  Density_8Gb,
  Density_12Gb,
  Density_16Gb,
  Density_24Gb,
  Density_32Gb,
  Density_48Gb,
  Density_64Gb,
  DensityInvalid
} DENSITY_PER_DIE;

typedef enum {
  DiePerPkg_1,
  DiePerPkg_Reserved1,
  DiePerPkg_2,
  DiePerPkg_4,
  DiePerPkg_8,
  DiePerPkg_16,
  DiePerPkgInvalid
} DIE_PER_PKG;

#define SPD_DENSITY_TABLE       {0, 4, 8, 12, 16, 24, 32, 48, 64}
#define SPD_DIE_PER_PKG_TABLE   {1, 0, 2, 4, 8, 16}

/// First SDRAM Addressing
#define SPD_FIRST_ADDRESSING     5

/// This byte describes the number SDRAM column and row address bits. These values comes from
/// the DDR5 SDRAM data sheet, JESD79-5. This byte applies to the even rank SDRAMs for
/// asymmetrical memory configurations, or for all SDRAMs for symmetrical configurations.
typedef union {
  struct {
    uint8_t NumRows         : 5;  /// SDRAM Row Address Bits
    uint8_t NumColumns      : 3;  /// SDRAM Column Address Bits
  } Field;
  uint8_t Value;
} SPD_ADDRESSING_S;

typedef enum {
  TenColumns,
  ElevenColumns,
  NumColumnsInvalid
} NUM_COLUMNS;

#define SPD_DECODE_COL_BITS(Val)    (Val + 10)

typedef enum {
  SixteenRows,
  SeventeenRows,
  EighteenRows,
  NumRowsInvalid
} NUM_ROWS;

#define SPD_DECODE_ROW_BITS(Val)    (Val + 16)

/// First SDRAM I/O Width
#define SPD_FIRST_IO_WIDTH    6

typedef union {
  struct {
    uint8_t Reserved40      : 5;
    uint8_t IoWidth         : 3;  /// SDRAM I/O Width
  } Field;
  uint8_t Value;
} SPD_IO_WIDTH_S;

typedef enum {
  x4,
  x8,
  x16,
  x32,
  IoWidthMax
} IO_WIDTH;

#define SPD_DECODE_IO_WIDTH(Val)  (4 << Val)
#define SPD_DECODE_PAGESIZE(Val)  ((Val > x8) ? 1 : 0 )

/// First SDRAM Bank Groups & Banks Per Bank Group
#define SPD_FIRST_BANKS    7

typedef union {
  struct {
    uint8_t BanksPerBG      : 3;  /// SDRAM Banks Per Bank Group
    uint8_t Reserved43      : 2;
    uint8_t BankGroups      : 3;  /// SDRAM Bank Groups
  } Field;
  uint8_t Value;
} SPD_BANKS_S;

typedef enum {
  OneBank,
  TwoBanks,
  FourBanks,
  BanksPerBGMax
} BANKS_PER_BG;

#define SPD_DECODE_SPD_BANKS_PER_BG(Val)    (1 << Val)

typedef enum {
  OneBg,
  TwoBgs,
  FourBgs,
  EightBgs,
  BgMax
} BANK_GROUPS;

#define SPD_DECODE_BANK_GROUPS(Val)    (1 << Val)

/// Second SDRAM Density and Package
#define SPD_SECOND_DENSITY_PKG    8
#define SPD_SECOND_ADDRESSING     9
#define SPD_SECOND_IO_WIDTH       10
#define SPD_SECOND_BANKS          11
#define SPD_OPTIONAL_FEATURES    12

typedef union {
  struct {
    uint8_t Reserved30      : 4;
    uint8_t BL32            : 1;  /// BL32
    uint8_t SoftPPR         : 1;  /// Soft PPR
    uint8_t PPR             : 2;  /// Post Package Repair (PPR)
  } Field;
  uint8_t Value;
} SPD_OPT_FEATURES_S;

/// Reserved Bytes 13 - 15
#define SPD_RESERVED_13          13
#define SPD_RESERVED_14          14
#define SPD_RESERVED_15          15

/// SDRAM Nominal Voltage, VDD
#define SPD_NOMINAL_VDD     16

/// This byte describes the voltage levels for the SDRAM VDD supply only.
typedef union {
  struct {
    uint8_t Endurant        : 2;  ///<'Endurant' is defined as the VDD voltage at which the module may
                                  ///<  be powered without adversely affecting the life expectancy
                                  ///< or reliability. Operation is not supported at this voltage
    uint8_t Operable        : 2;  ///< 'Operable' is defined as the VDD voltage at which module operation is allowed
                                  ///<  using the performance values programmed in the SPD
    uint8_t Nominal         : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDD_S;

/// SDRAM Nominal Voltage, VDDQ
#define SPD_NOMINAL_VDQ     17

/// This byte describes the voltage levels for the SDRAM VDDQ supply only.
typedef union {
  struct {
    uint8_t Endurant : 2; ///<'Endurant' is the VDD voltage at which the module may be powered without adversely
                          ///< affecting the life expectancy or reliability. Operation not supported at this voltage
    uint8_t Operable : 2; ///< 'Operable' is defined as the VDD voltage at which module operation is allowed using the
                          ///< performance values programmed in the SPD
    uint8_t Nominal  : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDQ_S;

/// SDRAM Nominal Voltage, VPP
#define SPD_NOMINAL_VDP     18
/// This byte describes the voltage levels for the SDRAM VPP supply only.
typedef union {
  struct {
    uint8_t Endurant        : 2; ///<'Endurant' is defined as the VDD voltage at which the module may be powered
                                 ///< without adversely affecting the life expectancy or
                                 ///< reliability. Operation is not supported at this voltage
    uint8_t Operable        : 2; ///< 'Operable' is defined as the VDD voltage at which module operation is allowed
                                 ///< using the performance values programmed in the SPD
    uint8_t Nominal         : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDP_S;

/// Reserved Byte 19
#define SPD_RESERVED_19        19

/// SDRAM Minimum Cycle Time (tCKAVGmin)
#define SPD_TCKAVG_MIN_LSB     20
#define SPD_TCKAVG_MIN_MSB     21

typedef union {
  SPD_TIMING_S       Field;
  uint16_t Value;
} SPD_TCKAVG_MIN;

#define SPD_TCKAVG_MAX_LSB     22
#define SPD_TCKAVG_MAX_MSB     23

/// SDRAM CAS Latencies Supported
#define SPD_CAS_LATENCIES_SUPPORTED          24
#define SPD_CAS_LATENCIES_SUPPORTED_1        24   ///< CAS Latencies Supported, First Byte
#define SPD_CAS_LATENCIES_SUPPORTED_2        25   ///< CAS Latencies Supported, Second Byte
#define SPD_CAS_LATENCIES_SUPPORTED_3        26   ///< CAS Latencies Supported, Third Byte
#define SPD_CAS_LATENCIES_SUPPORTED_4        27   ///< CAS Latencies Supported, Fourth Byte
#define SPD_CAS_LATENCIES_SUPPORTED_5        28   ///< CAS Latencies Supported, Fifth Byte
#define SPD_TCL_BYTES                         5

typedef struct {
  uint8_t Byte[SPD_TCL_BYTES];
} SPD_SUPPORTED_CAS_LAT_S;

#define SPD_CHECK_CASLAT_SUPPORT(Offset, Val)                                                \
        ((Offset[SPD_CAS_LATENCIES_SUPPORTED + (((Val - 20) / 2) / 8)] \
        >> (((Val - 20) / 2) % 8) & 0x01) == 0x01)

/// Reserved Byte 29
#define SPD_RESERVED_29     29

#define SPD_TAA_MIN_LSB     30
#define SPD_TAA_MIN_MSB     31

#define SPD_TRCD_MIN_LSB    32
#define SPD_TRCD_MIN_MSB    33

#define SPD_TRP_MIN_LSB     34
#define SPD_TRP_MIN_MSB     35

#define SPD_TRAS_MIN_LSB    36
#define SPD_TRAS_MIN_MSB    37

#define SPD_TRC_MIN_LSB     38
#define SPD_TRC_MIN_MSB     39

#define SPD_TWR_MIN_LSB     40
#define SPD_TWR_MIN_MSB     41

#define SPD_TRFC1_MIN_LSB   42
#define SPD_TRFC1_MIN_MSB   43

#define SPD_TRFC2_MIN_LSB   44
#define SPD_TRFC2_MIN_MSB   45

#define SPD_TRFC_SB_MIN_LSB 46
#define SPD_TRFC_SB_MIN_MSB 47

#define SPD_TRFC1_DLR_MIN_LSB 48
#define SPD_TRFC1_DLR_MIN_MSB 49

#define SPD_TRFC2_DLR_MIN_LSB 50
#define SPD_TRFC2_DLR_MIN_MSB 51

#define SPD_TRFC_SB_DLR_MIN_LSB 52
#define SPD_TRFC_SB_DLR_MIN_MSB 53

#define SPD_PRIMARY_REFRESH_MANAGEMENT_FIRST     54

typedef union {
  struct {
    uint8_t Rfm             : 1;  ///< RFM Required
    uint8_t Raaimt          : 4;  ///< Rolling Accumulated ACT Initial Management Threshold (RAAIMT)
    uint8_t Raammt          : 3;  ///< Rolling Accumulated ACT Maximum Management Threshold (RAAMMT)
  } Field;
  uint8_t Value;
} SPD_REF_MANAGEMENT_FIRST_S;

/// Primary SDRAM Refresh Management Second Byte
#define SPD_PRIMARY_REFRESH_MANAGEMENT_SECOND     55

typedef union {
  struct {
    uint8_t Reserved_50     : 6;
    uint8_t Decrement       : 2;  ///< RFM RAA Counter Decrement per REF command
  } Field;
  uint8_t Value;
} SPD_REF_MANAGEMENT_SECOND_S;

/// Secondary SDRAM Refresh Management First Byte
#define SPD_SECONDARY_REFRESH_MANAGEMENT_FIRST    56

/// Secondary SDRAM Refresh Management Second Byte
#define SPD_SECONDARY_REFRESH_MANAGEMENT_SECOND   57

#define SPD_MODULE_REVISION 192

/// This byte defines the SPD revision for bytes 192~447. See SPD byte 1 for details/// Manufacture ID SPD
#define SPD_MANUFACTURER_ID_SPD             194
#define SPD_MANUFACTURER_ID_SPD_1ST_BYTE    194
#define SPD_MANUFACTURER_ID_SPD_2ND_BYTE    195

/// Device Type SPD
#define SPD_DEVICE_TYPE_SPD                 196

typedef union {
  struct {
    uint8_t DevType        : 4;
    uint8_t Reserved_54    : 2;
    uint8_t Reserved_6     : 1;
    uint8_t Installed      : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_DEVICE_TYPE_SPD_S;

typedef enum {
  Spd5118,
  Spd5108,
  MaxSpdType
} SpdDevTypes;

#define SPD_DEVICE_REVISION_SPD 197

typedef union {
  struct {
    uint8_t MinorRev        : 4;
    uint8_t MajorRev        : 4
    ;
  } Field;
  uint8_t Value;
} SPD_MODULE_REVISION_S;

/// PMIC0
#define SPD_MANUFACTURER_ID_PMIC0                 198
#define SPD_MANUFACTURER_ID_PMIC0_1ST_BYTE        198
#define SPD_MANUFACTURER_ID_PMIC0_2ND_BYTE        199
#define SPD_DEVICE_TYPE_PMIC0                     200
#define SPD_DEVICE_REVISION_PMIC0                 201

/// PMIC1
#define SPD_MANUFACTURER_ID_PMIC1                 202
#define SPD_MANUFACTURER_ID_PMIC1_1ST_BYTE        202
#define SPD_MANUFACTURER_ID_PMIC1_2ND_BYTE        203
#define SPD_DEVICE_TYPE_PMIC1                     204
#define SPD_DEVICE_REVISION_PMIC1                 205

/// PMIC2
#define SPD_MANUFACTURER_ID_PMIC2                 206
#define SPD_MANUFACTURER_ID_PMIC2_1ST_BYTE        206
#define SPD_MANUFACTURER_ID_PMIC2_2ND_BYTE        207
#define SPD_DEVICE_TYPE_PMIC2                     208
#define SPD_DEVICE_REVISION_PMIC2                 209

typedef union {
  struct {
    uint8_t DevType        : 4;
    uint8_t Reserved_54    : 2;
    uint8_t Reserved_6     : 1;
    uint8_t Installed      : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_DEVICE_TYPE_PMIC_S;

typedef enum {
  Pmic5000,
  Pmic5010,
  Pmic5100,
  MaxPmicDevTypes
} PmicDevTypes;

/// Thermal Sensors
#define SPD_MANUFACTURER_ID_TS                    210
#define SPD_MANUFACTURER_ID_TS_FIRST_BYTE         210
#define SPD_MANUFACTURER_ID_TS_SECOND_BYTE        211
#define SPD_DEVICE_TYPE_TS                        212
#define SPD_DEVICE_REVISION_TS                    213

typedef union {
  struct {
    uint8_t DeviceType        : 4;
    uint8_t Reserved54        : 2;
    uint8_t TS1               : 1;
    uint8_t TS0               : 1;
  } Field;
  uint8_t Value;
} SPD_DEVICE_TYPE_TS_S;

typedef enum {
  Ts5111,
  Ts5110,
  MaxTsDevTyped
} TsDevTypes;

/// Module Nominal Height
#define SPD_MODULE_HEIGHT     230

typedef union {
  struct {
    uint8_t Height            : 4;  /// Module Nominal Height max, in mm (baseline height = 15 mm)
    uint8_t Reserved54        : 4;
  } Field;
  uint8_t Value;
} SPD_MODULE_HEIGHT_S;

#define SPD_MODULE_HEIGHT_DECODE(Val)   (Val + 15)

/// Module Maximum Thickness
#define SPD_MODULE_THICKNESS  231

typedef union {
  struct {
    uint8_t Front             : 4;  /// Module Maximum Thickness max, Front, in mm (baseline thickness = 1 mm)
    uint8_t Back              : 4;  /// Module Maximum Thickness max, Back, in mm (baseline thickness = 1 mm)
  } Field;
  uint8_t Value;
} SPD_MODULE_THICKNESS_S;

/// Reference Raw Card Used
#define SPD_RAW_CARD          232

typedef union {
  struct {
    uint8_t RefDesign         : 5;  /// Reference Design
    uint8_t DesignRev         : 3;  /// Design Revision
  } Field;
  uint8_t Value;
} SPD_RAW_CARD_S;

typedef enum {
  RawCard_A,
  RawCard_B,
  RawCard_C,
  RawCard_D,
  RawCard_E,
  RawCard_F,
  RawCard_G,
  RawCard_H,
  RawCard_J,
  RawCard_K,
  RawCard_L,
  RawCard_M,
  RawCard_N,
  RawCard_P,
  RawCard_R,
  RawCard_T,
  RawCard_U,
  RawCard_V,
  RawCard_W,
  RawCard_Y,
  RawCard_AA,
  RawCard_AB,
  RawCard_AC,
  RawCard_AD,
  RawCard_AE,
  RawCard_AF,
  RawCard_AG,
  RawCard_AH,
  RawCard_AJ,
  RawCard_AK,
  RawCard_Reserved,
  RawCard_ZZ
} SPD_RAW_CARD_E;

/// DIMM Attributes
#define SPD_ATTRIBUTES        233

typedef union {
  struct {
    uint8_t DramRows          : 2;  /// # of rows of DRAMs on Module
    uint8_t HeatSpreader      : 1;  /// Heat Spreader
    uint8_t Reserved_3        : 1;
    uint8_t TempGrade         : 4;  /// Temperature Grade
  } Field;
  uint8_t Value;
} SPD_ATTRIBUTES_S;

typedef enum {
  TempGradeA1T,
  TempGradeA2T,
  TempGradeA3T,
  TempGradeIT,
  TempGradeST,
  TempGradeET,
  TempGradeRT,
  TempGradeNT,
  TempGradeXT,
  TempGradeMax
} SPD_TEMP_GRADE;

/// Module Organization
#define SPD_MODULE_ORG        234

typedef union {
  struct {
    uint8_t Reserved_20       : 3;
    uint8_t RanksPerChannel   : 3;  /// Number of Package Ranks per Channel
    uint8_t RankMix           : 1;  /// Rank Mix
    uint8_t Reserved_7        : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_ORG_S;

#define SPD_PACKAGE_RANKS_DECODE(Val)   (Val + 1)

typedef enum {
  RankMixSymmetrical,
  RankMixAsymmetrical
} SPD_RANKMIX_E;

/// Memory Channel Bus Width
#define SPD_CHANNEL_BUS_WIDTH 235

typedef union {
  struct {
    uint8_t Width             : 3;  /// Primary bus width per Channel, in bits
    uint8_t WidthExt          : 2;  /// Bus width extension per Channel, in bits
    uint8_t NumChannels       : 2;  /// Number of Channels per DIMM
    uint8_t Reserved_7        : 1;
  } Field;
  uint8_t Value;
} SPD_CHANNEL_BUS_WIDTH_S;

#define SPD_CHANNEL_BUS_WIDTH_DECODE(Val)       (8 << Val)
#define SPD_CHANNEL_BUS_WIDTH_EXT_DECODE(Val)   (4 * Val)
#define SPD_CHANNELS_PER_DIMM_DECODE(Val)       (Val + 1)

#define SPD_MANUFACTURER_ID_RCD                   240
#define SPD_MANUFACTURER_ID_RCD_1ST_BYTE          240
#define SPD_MANUFACTURER_ID_RCD_2ND_BYTE          241

#define SPD_DEVICE_TYPE_RCD                       242

typedef union {
  struct {
    uint8_t DevType        : 4;
    uint8_t Reserved_54    : 2;
    uint8_t Reserved_6     : 1;
    uint8_t Installed      : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_DEVICE_TYPE_RCD_S;

#define SPD_DEVICE_REVISION_RCD                  243

#define SPD_MANUFACTURER_ID_DB                   244
#define SPD_MANUFACTURER_ID_DB_1ST_BYTE          244
#define SPD_MANUFACTURER_ID_DB_2ND_BYTE          245

#define SPD_DEVICE_TYPE_DB                       246

typedef union {
  struct {
    uint8_t DevType        : 4;
    uint8_t Reserved_54    : 2;
    uint8_t Reserved_6     : 1;
    uint8_t Installed      : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_DEVICE_TYPE_DB_S;

/// Data Buffer Revision
#define SPD_DEVICE_REVISION_DB                   247

/// Annex A.5: Module Specific Bytes for Differential Memory Module Type[a-z]
/// (Bytes 192~447, 0x0C0~0x1BF)
/// Differential Memory Buffer Manufacturer ID Code
#define SPD_MANUFACTURER_ID_DMB                  244
#define SPD_MANUFACTURER_ID_DMB_1ST_BYTE         244
#define SPD_MANUFACTURER_ID_DMB_2ND_BYTE         245

/// Manufacturer ID Code: Per JEP106.
/// Differential Memory Buffer Device Type
#define SPD_DEVICE_TYPE_DMB                      246

typedef union {
  struct {
    uint8_t DevType        : 4;
    uint8_t Reserved_54    : 2;
    uint8_t Reserved_6     : 1;
    uint8_t Installed      : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_DEVICE_TYPE_DMB_S;

/// Data Buffer Revision
#define SPD_DEVICE_REVISION_DB                   247

#define SPD_CRC_16                510
#define SPD_CRC_LSB               510
#define SPD_CRC_MSB               511

typedef SPD_VALUE16 SPD_CRC_S;

#define SPD_CRC_COUNT             510

#define SPD_MODULE_MANUFACTURER_ID_CODE_1ST_BYTE     512
#define SPD_MODULE_MANUFACTURER_ID_CODE_2nd_BYTE     513

#define SPD_MODULE_MANUFACTURING_LOCATION            514

#define SPD_MODULE_MANUFACTURING_DATE                515 /// Year, Week, Coded in BCD
#define SPD_MODULE_MANUFACTURING_YEAR                515 /// Year, Coded in BCD
#define SPD_MODULE_MANUFACTURING_WEEK                516 /// Week, Coded in BCD

typedef union {
  struct {
    uint8_t MfgYear;
    uint8_t MfgWeek;
  } Fields;
  uint16_t Value;
} SPD_MFG_DATE_S;

#define SPD_MODULE_SERIAL_NUMBER                     517
#define SPD_MODULE_SERIAL_NUMBER_LEN                 (520 - 517 + 1) /// 4 Bytes

typedef struct {
  uint8_t String[SPD_MODULE_SERIAL_NUMBER_LEN];
} SPD_MODULE_SERIAL_NUMBER_S;

#define SPD_MODULE_PART_NUMBER                       521
#define SPD_MODULE_PART_NUMBER_LEN                   (550 - 521 + 1) /// 20 Bytes

typedef struct {
  uint8_t String[SPD_MODULE_PART_NUMBER_LEN];
} SPD_MODULE_PART_NUMBER_S;

#define SPD_DRAM_MANUFACTURERS_ID_CODE_1ST_BYTE      552
#define SPD_DRAM_MANUFACTURERS_ID_CODE_2nd_BYTE      553


#define SPD_DRAM_STEPPING                            554


typedef struct {
  SPD_NUM_BYTES_S                   NumBytes;                       /// 0
  SPD_REVISION_S                    Revision;                       /// 1
  SPD_KEY_BYTE_S                    KeyByte1;                       /// 2
  SPD_KEY_BYTE2_S                   KeyByte2;                       /// 3
  SPD_DENSITY_PACKAGE_S             FirstDensity;                   /// 4
  SPD_ADDRESSING_S                  FirstAddressing;                /// 5
  SPD_IO_WIDTH_S                    FirstIoWidth;                   /// 6
  SPD_BANKS_S                       FirstBanks;                     /// 7
  SPD_DENSITY_PACKAGE_S             SecondDensity;                  /// 8
  SPD_ADDRESSING_S                  SecondAddressing;               /// 9
  SPD_IO_WIDTH_S                    SecondIoWidth;                  /// 10
  SPD_BANKS_S                       SecondBanks;                    /// 11
  SPD_OPT_FEATURES_S                OptionalFeatures;               /// 12
  uint8_t                           Reserved_13;                    /// 13
  uint8_t                           Reserved_14;                    /// 14
  uint8_t                           Reserved_15;                    /// 15
  SPD_NOMINAL_VDD_S                 Vdd;                            /// 16
  SPD_NOMINAL_VDQ_S                 Vdq;                            /// 17
  SPD_NOMINAL_VDP_S                 Vdp;                            /// 18
  uint8_t                           Reserved_19;                    /// 19
  SPD_TIMING_S                      TckAvgMin;                      /// 20
  SPD_TIMING_S                      TckAvgMax;                      /// 22
  uint8_t                           CasLatSupported[SPD_TCL_BYTES]; /// 24 - 28
  uint8_t                           Reserved_29;                    /// 29
  SPD_TIMING_S                      TaaMin;                         /// 30
  SPD_TIMING_S                      TrcdMin;                        /// 32
  SPD_TIMING_S                      TrpMin;                         /// 34
  SPD_TIMING_S                      TrasMin;                        /// 36
  SPD_TIMING_S                      TrcMin;                         /// 48
  SPD_TIMING_S                      TwrMin;                         /// 40
  SPD_TIMING_S                      Trfc1Min;                       /// 42
  SPD_TIMING_S                      Trfc2Min;                       /// 44
  SPD_TIMING_S                      TrfcSbMin;                      /// 46
  SPD_TIMING_S                      Trfc1DlrMin;                    /// 48
  SPD_TIMING_S                      Trfc2DlrMin;                    /// 50
  SPD_TIMING_S                      Trfc2SbDlrMin;                  /// 52
  SPD_REF_MANAGEMENT_FIRST_S        PriRefManagement1st;            /// 54
  SPD_REF_MANAGEMENT_SECOND_S       PriRefManagement2nd;            /// 55
  SPD_REF_MANAGEMENT_FIRST_S        SecRefManagement1st;            /// 56
  SPD_REF_MANAGEMENT_SECOND_S       SecRefManagement2nd;            /// 57
  uint8_t                           Reserved_58_63[63 - 57];        /// 58 - 63
} SPD_BASE_CONFIG_0_S;


typedef struct {
  SPD_NUM_BYTES_S                   NumBytes;                       /// 0
  SPD_REVISION_S                    Revision;                       /// 1
  SPD_KEY_BYTE_S                    KeyByte1;                       /// 2
  SPD_KEY_BYTE2_S                   KeyByte2;                       /// 3
  SPD_DENSITY_PACKAGE_S             FirstDensity;                   /// 4
  SPD_ADDRESSING_S                  FirstAddressing;                /// 5
  SPD_IO_WIDTH_S                    FirstIoWidth;                   ///< 6
  SPD_BANKS_S                       FirstBanks;                     ///< 7
  SPD_DENSITY_PACKAGE_S             SecondDensity;                  ///< 8
  SPD_ADDRESSING_S                  SecondAddressing;               ///< 9
  SPD_IO_WIDTH_S                    SecondIoWidth;                  ///< 10
  SPD_BANKS_S                       SecondBanks;                    ///< 11
  SPD_OPT_FEATURES_S                OptionalFeatures;               ///< 12
  uint8_t                           Reserved_13;                    ///< 13
  uint8_t                           Reserved_14;                    ///< 14
  uint8_t                           Reserved_15;                    ///< 15
  SPD_NOMINAL_VDD_S                 Vdd;                            ///< 16
  SPD_NOMINAL_VDQ_S                 Vdq;                            ///< 17
  SPD_NOMINAL_VDP_S                 Vdp;                            ///< 18
  uint8_t                           Reserved_19;                    ///< 19
  SPD_TIMING_S                      TckAvgMin;                      ///< 20
  SPD_TIMING_S                      TckAvgMax;                      ///< 22
  uint8_t                           CasLatSupported[SPD_TCL_BYTES]; ///< 24 - 28
  uint8_t                           Reserved_29;                    ///< 29
  SPD_TIMING_S                      TaaMin;                         ///< 30
  SPD_TIMING_S                      TrcdMin;                        ///< 32
  SPD_TIMING_S                      TrpMin;                         ///< 34
  SPD_TIMING_S                      TrasMin;                        ///< 36
  SPD_TIMING_S                      TrcMin;                         ///< 48
  SPD_TIMING_S                      TwrMin;                         ///< 40
  SPD_TIMING_S                      Trfc1Min;                       ///< 42
  SPD_TIMING_S                      Trfc2Min;                       ///< 44
  SPD_TIMING_S                      TrfcSbMin;                      ///< 46
  SPD_TIMING_S                      Trfc1DlrMin;                    ///< 48
  SPD_TIMING_S                      Trfc2DlrMin;                    ///< 50
  SPD_TIMING_S                      Trfc2SbDlrMin;                  ///< 52
  SPD_REF_MANAGEMENT_FIRST_S        PriRefManagement1st;            ///< 54
  SPD_REF_MANAGEMENT_SECOND_S       PriRefManagement2nd;            ///< 55
  SPD_REF_MANAGEMENT_FIRST_S        SecRefManagement1st;            ///< 56
  SPD_REF_MANAGEMENT_SECOND_S       SecRefManagement2nd;            ///< 57
  uint8_t                             Reserved_58_127[127 - 57];    ///< 58 - 127
} SPD_BASE_CONFIG_S;


typedef struct {
  SPD_REVISION_S                     ModuleRevision;       ///< 192
  uint8_t                            Reserved_193;         ///< 193
  SPD_MANUFACTURER_ID_S              SpdMfgID;             ///< 194
  SPD_MODULE_DEVICE_TYPE_SPD_S       SpdDevType;           ///< 196
  SPD_MODULE_REVISION_S              SpdRevision;          ///< 197
  SPD_MANUFACTURER_ID_S              Pmic0MfgId;           ///< 198
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic0DevType;         ///< 200
  SPD_MODULE_REVISION_S              Pmic0Revision;        ///< 201
  SPD_MANUFACTURER_ID_S              Pmic1MfgId;           ///< 202
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic1DevType;         ///< 204
  SPD_MODULE_REVISION_S              Pmic1Revision;        ///< 205
  SPD_MANUFACTURER_ID_S              Pmic2MfgId;           ///< 206
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic2DevType;         ///< 208
  SPD_MODULE_REVISION_S              Pmic2Revision;        ///< 209
  SPD_MANUFACTURER_ID_S              TsMfgId;              ///< 210
  SPD_DEVICE_TYPE_TS_S               TsDevType;            ///< 212
  SPD_MODULE_REVISION_S              TsRevision;           ///< 213
  uint8_t                            Reserved_214_229[16]; ///< 214 - 229
  SPD_MODULE_HEIGHT_S                Height;               ///< 230
  SPD_MODULE_THICKNESS_S             Thickness;            ///< 231
  SPD_RAW_CARD_S                     RawCard;              ///< 232
  SPD_ATTRIBUTES_S                   Attributes;           ///< 233
  SPD_MODULE_ORG_S                   ModuleOrg;            ///< 234
  SPD_CHANNEL_BUS_WIDTH_S            ChBusWidth;           ///< 235
  uint8_t                            Reserved_236_239[4];  ///< 236 - 239
} SPD_ANNEX_COMMON_S;


typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  uint8_t                            Reserved_240_447[208]; ///< 240 - 447
} SPD_SODOWN_ANNEX_S;


typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  uint8_t                            Reserved_240_447[208]; ///< 240 -447
} SPD_UDIMM_ANNEX_S;

typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                // 192
  SPD_MANUFACTURER_ID_S              RcdManfId;             ///< 240
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;            ///< 242
  SPD_MODULE_REVISION_S              RcdRevision;           ///< 243
  uint8_t                            Reserved_244_447[204]; ///< 244 - 447
} SPD_RDIMM_ANNEX_S;

typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  SPD_MANUFACTURER_ID_S              RcdManfId;             ///< 240
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;            ///< 242
  SPD_MODULE_REVISION_S              RcdRevision;           ///< 243
  SPD_MANUFACTURER_ID_S              DbMfgId;               ///< 244
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;             ///< 246
  SPD_MODULE_REVISION_S              DbRevision;            ///< 247
  uint8_t                            Reserved_248_447[200]; ///< 248 - 447
} SPD_LRDIMM_ANNEX_S;

typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  SPD_MANUFACTURER_ID_S              DmbManfId;             ///< 240
  SPD_MODULE_DEVICE_TYPE_DMB_S       DmbDevType;            ///< 242
  SPD_MODULE_REVISION_S              DmbRevision;           ///< 243
  uint8_t                            Reserved_244_447[204]; ///< 244 - 447
} SPD_DDIMM_ANNEX_S;

typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  SPD_MANUFACTURER_ID_S              RcdManfId;             ///< 240
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;            ///< 242
  SPD_MODULE_REVISION_S              RcdRevision;           ///< 243
  SPD_MANUFACTURER_ID_S              DbMfgId;               ///< 244
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;             ///< 246
  SPD_MODULE_REVISION_S              DbRevision;            ///< 247
  uint8_t                            Reserved_248_447[200]; ///< 248 - 447
} SPD_NVDIMM_N_ANNEX_S;

typedef struct {
  SPD_ANNEX_COMMON_S                 Common;                ///< 192
  SPD_MANUFACTURER_ID_S              RcdManfId;             ///< 240
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;            ///< 242
  SPD_MODULE_REVISION_S              RcdRevision;           ///< 243
  SPD_MANUFACTURER_ID_S              DbMfgId;               ///< 244
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;             ///< 246
  SPD_MODULE_REVISION_S              DbRevision;            ///< 247
  uint8_t                            Reserved_248_447[200]; ///< 248 - 447
} SPD_NVDIMM_P_ANNEX_S;

/// Standard Module Parameters
typedef union {
  SPD_ANNEX_COMMON_S                 Common;                 ///< Annex A0 Common
  SPD_SODOWN_ANNEX_S                 Sodown;                 ///< Annex A1 Soldered Down
  SPD_UDIMM_ANNEX_S                  Udimm;                  ///< Annex A2 UDIMM
  SPD_RDIMM_ANNEX_S                  Rdimm;                  ///< Annex A3 RDIMM
  SPD_LRDIMM_ANNEX_S                 Lrdimm;                 ///< Annex A4 LRDIMM
  SPD_DDIMM_ANNEX_S                  Ddimm;                  ///< Annex A5 DDIMM
  SPD_NVDIMM_N_ANNEX_S               Nvdimmn;                ///< Annex A6 NVDIMM-N
  SPD_NVDIMM_P_ANNEX_S               Nvdimmp;                ///< Annex A7 NVDIMM-P
} SPD_MODULE_SPECIFIC_S;

typedef struct {
  SPD_MANUFACTURER_ID_S               ModuleMfgId;                    /// 512
  uint8_t                             ModuleMfgLocation;              ///< 514
  SPD_MFG_DATE_S                      ModuleMfgDate;                  ///< 515
  SPD_MODULE_SERIAL_NUMBER_S          ModuleSerialNumber;             ///< 517 - 520
  SPD_MODULE_PART_NUMBER_S            ModulePartNumber;               ///< 521 - 550
  SPD_REVISION_S                      ModuleRevisionCode;             ///< 551
  SPD_MANUFACTURER_ID_S               DramMfgId;                      ///< 552
  uint8_t                             DramStepping;                   ///< 554
  uint8_t                             Reserved_555_639[639 - 554];    ///< 555 - 639
} SPD_MANUFACTURING_INFO_S;

typedef struct {
  SPD_BLOCK_S                         Block[SPD_NUM_BLOCKS];          ///< SpdBlocks 0 - 15
} DDR5_SPD_BLOCKS_S;

typedef struct {
  SPD_BASE_CONFIG_S                   Base;                            ///< 0~127
  uint8_t                             Reserved_128_191[191 - 127];     ///< 128~191
  SPD_MODULE_SPECIFIC_S               ModuleSpec;                      ///< 192~447
  uint8_t                             Reserved_448_509[509 - 447];     ///< 448~509
  SPD_CRC_S                           Crc;                             ///< 510~511
  SPD_MANUFACTURING_INFO_S            MfgInfo;                         ///< 512~639
  SPD_BLOCK_S                         End_User_Programmable_640_703;   ///< 640~703 End User Programmable
  SPD_BLOCK_S                         End_User_Programmable_704_767;   ///< 704~767 End User Programmable
  SPD_BLOCK_S                         End_User_Programmable_760_831;   ///< 760~831 End User Programmable
  SPD_BLOCK_S                         End_User_Programmable_832_895;   ///< 832~895 End User Programmable
  SPD_BLOCK_S                         End_User_Programmable_896_959;   ///< 896~959 End User Programmable
  SPD_BLOCK_S                         End_User_Programmable_960_1023;  ///< 960~1023 End User Programmable
} DDR5_SPD_S;

#pragma pack(pop)
