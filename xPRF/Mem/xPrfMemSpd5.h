/* Copyright 2008-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 *
 * xPrfMemSpd5.h
 *
 * DDR5 SPD Structures
 *
 **/

#ifndef _XPRFMEMSPD5_H_
#define _XPRFMEMSPD5_H_

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
  SpdBlock_BaseConfig_0          = 0x00,
  SpdBlock_BaseConfig_1          = 0x01,
  SpdBlock_Reserved_2            = 0x02,
  SpdBlock_ModuleParms_0         = 0x03,
  SpdBlock_ModuleParms_1         = 0x04,
  SpdBlock_ModuleParms_2         = 0x05,
  SpdBlock_ModuleParms_3         = 0x06,
  SpdBlock_Reserved_7            = 0x07,
  SpdBlock_MfgInfo0              = 0x08,
  SpdBlock_MfgInfo1              = 0x09,
  SpdBlock_EndUserProgrammable0  = 0x0A,
  SpdBlock_EndUserProgrammable1  = 0x0B,
  SpdBlock_EndUserProgrammable2  = 0x0C,
  SpdBlock_EndUserProgrammable3  = 0x0D,
  SpdBlock_EndUserProgrammable4  = 0x0E,
  SpdBlock_EndUserProgrammable5  = 0x0F
} DDR5_SPD_BLOCK_NAMES;

typedef struct {
  uint8_t Data[SPD_BLOCK_LEN];
} SPD_BLOCK_S;

#define SPD_NUM_BYTES       0

typedef union {
   struct {
    uint8_t Reserved_30      : 4;
    uint8_t SpdBytesTotal    : 3;
    uint8_t Reserved_7       : 1;
  } Field;
  uint8_t Value;
} SPD_NUM_BYTES_S;

#define SPD_DECODE_BYTES_TOTAL(val)  (128 << val)

#define SPD_REVISION        1

typedef union {
   struct {
    uint8_t SpdRevisionAdditions         : 4;
    uint8_t SpdRevisionEncodings         : 4;
  } Field;
  uint8_t Value;
} SPD_REVISION_S;

#define SPD_KEY_BYTE        2

typedef union {
   struct {
    uint8_t ModuleType      : 8;
  } Field;
  uint8_t Value;
} SPD_KEY_BYTE_S;

typedef enum {
  ModTypeReserved0,
  ModTypeFastPageMode,
  ModTypeEDO,
  ModTypePipelinedNibble,
  ModTypeSdram,
  ModTypeRom,
  ModTypeDdrSgram,
  ModTypeDdrSdram,
  ModTypeDdr2Sdram,
  ModTypeDdr2SdramFBDimm,
  ModTypeDdr2SdramFBDimmProbe,
  ModTypeDdr3Sdram,
  ModTypeDdr4Sdram,
  ModTypeReserved13,
  ModTypeDdr4ESdram,
  ModTypeLpddr3Sdram,
  ModTypeLpddr4Sram,
  ModTypeLpddr4xSdram,
  ModTypeDdr5Sdram,
  ModTypeLpddr5Sdram,
  ModTypeDdr5NvdimmP,
  ModTypeModuleTypeMax
} MODULE_TYPE;

#define SPD_KEY_BYTE2       3

typedef union {
   struct {
    uint8_t BaseModuleType  : 4;
    uint8_t HybridMedia     : 3;
    uint8_t Hybrid          : 1;
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

#define SPD_FIRST_DENSITY_PACKAGE    4

typedef union {
   struct {
    uint8_t Density         : 5;
    uint8_t DiePerPkg       : 3;
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

#define SPD_FIRST_ADDRESSING     5

typedef union {
   struct {
    uint8_t NumRows         : 5;
    uint8_t NumColumns      : 3;
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

#define SPD_FIRST_IO_WIDTH    6

typedef union {
   struct {
    uint8_t Reserved40      : 5;
    uint8_t IoWidth         : 3;
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

#define SPD_FIRST_BANKS    7

typedef union {
   struct {
    uint8_t BanksPerBG      : 3;
    uint8_t Reserved43      : 2;
    uint8_t BankGroups      : 3;
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

#define SPD_SECOND_DENSITY_PKG    8

#define SPD_SECOND_ADDRESSING     9

#define SPD_SECOND_IO_WIDTH       10

#define SPD_SECOND_BANKS          11

#define SPD_OPTIONAL_FEATURES    12

typedef union {
   struct {
    uint8_t Reserved30      : 4;
    uint8_t BL32            : 1;
    uint8_t SoftPPR         : 1;
    uint8_t PPR             : 2;
  } Field;
  uint8_t Value;
} SPD_OPT_FEATURES_S;

#define SPD_RESERVED_13          13
#define SPD_RESERVED_14          14
#define SPD_RESERVED_15          15

#define SPD_NOMINAL_VDD     16
typedef union {
   struct {
    uint8_t Endurant        : 2;
    uint8_t Operable        : 2;
    uint8_t Nominal         : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDD_S;

#define SPD_NOMINAL_VDQ     17
typedef union {
   struct {
    uint8_t Endurant : 2;
    uint8_t Operable : 2;
    uint8_t Nominal  : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDQ_S;

#define SPD_NOMINAL_VDP     18
typedef union {
   struct {
    uint8_t Endurant        : 2;
    uint8_t Operable        : 2;
    uint8_t Nominal         : 4;
  } Field;
  uint8_t Value;
} SPD_NOMINAL_VDP_S;

#define SPD_RESERVED_19        19

#define SPD_TCKAVG_MIN_LSB     20
#define SPD_TCKAVG_MIN_MSB     21

typedef union {
   SPD_TIMING_S       Field;
   uint16_t Value;
} SPD_TCKAVG_MIN;

#define SPD_TCKAVG_MAX_LSB     22
#define SPD_TCKAVG_MAX_MSB     23

#define SPD_CAS_LATENCIES_SUPPORTED          24
#define SPD_CAS_LATENCIES_SUPPORTED_1        24
#define SPD_CAS_LATENCIES_SUPPORTED_2        25
#define SPD_CAS_LATENCIES_SUPPORTED_3        26
#define SPD_CAS_LATENCIES_SUPPORTED_4        27
#define SPD_CAS_LATENCIES_SUPPORTED_5        28
#define SPD_TCL_BYTES                         5

typedef struct {
  uint8_t Byte[SPD_TCL_BYTES];
} SPD_SUPPORTED_CAS_LAT_S;

#define SPD_CHECK_CASLAT_SUPPORT(Offset, Val)                                                \
                                ((Offset[SPD_CAS_LATENCIES_SUPPORTED + (((Val - 20)/2) / 8)] \
                                >> (((Val - 20) / 2) % 8) & 0x01) == 0x01)

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
    uint8_t Rfm             : 1;
    uint8_t Raaimt          : 4;
    uint8_t Raammt          : 3;
  } Field;
  uint8_t Value;
} SPD_REF_MANAGEMENT_FIRST_S;

#define SPD_PRIMARY_REFRESH_MANAGEMENT_SECOND     55

typedef union {
   struct {
    uint8_t Reserved_50     : 6;
    uint8_t Decrement       : 2;
  } Field;
  uint8_t Value;
} SPD_REF_MANAGEMENT_SECOND_S;

#define SPD_SECONDARY_REFRESH_MANAGEMENT_FIRST    56

#define SPD_SECONDARY_REFRESH_MANAGEMENT_SECOND   57

#define SPD_MODULE_REVISION 192

#define SPD_MANUFACTURER_ID_SPD             194
#define SPD_MANUFACTURER_ID_SPD_1ST_BYTE    194
#define SPD_MANUFACTURER_ID_SPD_2ND_BYTE    195

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
    uint8_t MajorRev        : 4;
  } Field;
  uint8_t Value;
} SPD_MODULE_REVISION_S;

#define SPD_MANUFACTURER_ID_PMIC0                 198
#define SPD_MANUFACTURER_ID_PMIC0_1ST_BYTE        198
#define SPD_MANUFACTURER_ID_PMIC0_2ND_BYTE        199
#define SPD_DEVICE_TYPE_PMIC0                     200
#define SPD_DEVICE_REVISION_PMIC0                 201

#define SPD_MANUFACTURER_ID_PMIC1                 202
#define SPD_MANUFACTURER_ID_PMIC1_1ST_BYTE        202
#define SPD_MANUFACTURER_ID_PMIC1_2ND_BYTE        203
#define SPD_DEVICE_TYPE_PMIC1                     204
#define SPD_DEVICE_REVISION_PMIC1                 205

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

#define SPD_MODULE_HEIGHT     230

typedef union {
   struct{
    uint8_t Height            : 4;
    uint8_t Reserved54        : 4;
  } Field;
  uint8_t Value;
} SPD_MODULE_HEIGHT_S;

#define SPD_MODULE_HEIGHT_DECODE(Val)   (Val + 15)

#define SPD_MODULE_THICKNESS  231

typedef union {
   struct {
    uint8_t Front             : 4;
    uint8_t Back              : 4;
  } Field;
  uint8_t Value;
} SPD_MODULE_THICKNESS_S;

#define SPD_RAW_CARD          232

typedef union {
   struct {
    uint8_t RefDesign         : 5;
    uint8_t DesignRev         : 3;
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

#define SPD_ATTRIBUTES        233

typedef union {
   struct {
    uint8_t DramRows          : 2;
    uint8_t HeatSpreader      : 1;
    uint8_t Reserved_3        : 1;
    uint8_t TempGrade         : 4;
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

#define SPD_MODULE_ORG        234

typedef union {
   struct {
    uint8_t Reserved_20       : 3;
    uint8_t RanksPerChannel   : 3;
    uint8_t RankMix           : 1;
    uint8_t Reserved_7        : 1;
  } Field;
  uint8_t Value;
} SPD_MODULE_ORG_S;

#define SPD_PACKAGE_RANKS_DECODE(Val)   (Val + 1)

typedef enum {
  RankMixSymmetrical,
  RankMixAsymmetrical
} SPD_RANKMIX_E;

#define SPD_CHANNEL_BUS_WIDTH 235

typedef union {
   struct {
    uint8_t Width             : 3;
    uint8_t WidthExt          : 2;
    uint8_t NumChannels       : 2;
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

#define SPD_DEVICE_REVISION_DB                   247

#define SPD_MANUFACTURER_ID_DMB                  244
#define SPD_MANUFACTURER_ID_DMB_1ST_BYTE         244
#define SPD_MANUFACTURER_ID_DMB_2ND_BYTE         245

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

#define SPD_DEVICE_REVISION_DB                   247

#define SPD_CRC_16                510
#define SPD_CRC_LSB               510
#define SPD_CRC_MSB               511

typedef SPD_VALUE16 SPD_CRC_S;

#define SPD_CRC_COUNT             510

#define SPD_MODULE_MANUFACTURER_ID_CODE_1ST_BYTE     512
#define SPD_MODULE_MANUFACTURER_ID_CODE_2nd_BYTE     513

#define SPD_MODULE_MANUFACTURING_LOCATION            514

#define SPD_MODULE_MANUFACTURING_DATE                515
#define SPD_MODULE_MANUFACTURING_YEAR                515
#define SPD_MODULE_MANUFACTURING_WEEK                516

typedef union {
   struct {
    uint8_t MfgYear;
    uint8_t MfgWeek;
  } Fields;
  uint16_t Value;
} SPD_MFG_DATE_S;

#define SPD_MODULE_SERIAL_NUMBER                     517
#define SPD_MODULE_SERIAL_NUMBER_LEN                 (520 - 517 + 1)

typedef struct {
  uint8_t String[SPD_MODULE_SERIAL_NUMBER_LEN];
} SPD_MODULE_SERIAL_NUMBER_S;

#define SPD_MODULE_PART_NUMBER                       521
#define SPD_MODULE_PART_NUMBER_LEN                   (550 - 521 + 1)

typedef struct {
  uint8_t String[SPD_MODULE_PART_NUMBER_LEN];
} SPD_MODULE_PART_NUMBER_S;

#define SPD_DRAM_MANUFACTURERS_ID_CODE_1ST_BYTE      552
#define SPD_DRAM_MANUFACTURERS_ID_CODE_2nd_BYTE      553

#define SPD_DRAM_STEPPING                            554

typedef struct {
  SPD_NUM_BYTES_S                   NumBytes;
  SPD_REVISION_S                    Revision;
  SPD_KEY_BYTE_S                    KeyByte1;
  SPD_KEY_BYTE2_S                   KeyByte2;
  SPD_DENSITY_PACKAGE_S             FirstDensity;
  SPD_ADDRESSING_S                  FirstAddressing;
  SPD_IO_WIDTH_S                    FirstIoWidth;
  SPD_BANKS_S                       FirstBanks;
  SPD_DENSITY_PACKAGE_S             SecondDensity;
  SPD_ADDRESSING_S                  SecondAddressing;
  SPD_IO_WIDTH_S                    SecondIoWidth;
  SPD_BANKS_S                       SecondBanks;
  SPD_OPT_FEATURES_S                OptionalFeatures;
  uint8_t                           Reserved_13;
  uint8_t                           Reserved_14;
  uint8_t                           Reserved_15;
  SPD_NOMINAL_VDD_S                 Vdd;
  SPD_NOMINAL_VDQ_S                 Vdq;
  SPD_NOMINAL_VDP_S                 Vdp;
  uint8_t                           Reserved_19;
  SPD_TIMING_S                      TckAvgMin;
  SPD_TIMING_S                      TckAvgMax;
  uint8_t                           CasLatSupported[SPD_TCL_BYTES];
  uint8_t                           Reserved_29;
  SPD_TIMING_S                      TaaMin;
  SPD_TIMING_S                      TrcdMin;
  SPD_TIMING_S                      TrpMin;
  SPD_TIMING_S                      TrasMin;
  SPD_TIMING_S                      TrcMin;
  SPD_TIMING_S                      TwrMin;
  SPD_TIMING_S                      Trfc1Min;
  SPD_TIMING_S                      Trfc2Min;
  SPD_TIMING_S                      TrfcSbMin;
  SPD_TIMING_S                      Trfc1DlrMin;
  SPD_TIMING_S                      Trfc2DlrMin;
  SPD_TIMING_S                      Trfc2SbDlrMin;
  SPD_REF_MANAGEMENT_FIRST_S        PriRefManagement1st;
  SPD_REF_MANAGEMENT_SECOND_S       PriRefManagement2nd;
  SPD_REF_MANAGEMENT_FIRST_S        SecRefManagement1st;
  SPD_REF_MANAGEMENT_SECOND_S       SecRefManagement2nd;
  uint8_t                           Reserved_58_63[63-57];
} SPD_BASE_CONFIG_0_S;

/// Bytes 0 - 127
typedef struct {
  SPD_NUM_BYTES_S                   NumBytes;
  SPD_REVISION_S                    Revision;
  SPD_KEY_BYTE_S                    KeyByte1;
  SPD_KEY_BYTE2_S                   KeyByte2;
  SPD_DENSITY_PACKAGE_S             FirstDensity;
  SPD_ADDRESSING_S                  FirstAddressing;
  SPD_IO_WIDTH_S                    FirstIoWidth;
  SPD_BANKS_S                       FirstBanks;
  SPD_DENSITY_PACKAGE_S             SecondDensity;
  SPD_ADDRESSING_S                  SecondAddressing;
  SPD_IO_WIDTH_S                    SecondIoWidth;
  SPD_BANKS_S                       SecondBanks;
  SPD_OPT_FEATURES_S                OptionalFeatures;
  uint8_t                           Reserved_13;
  uint8_t                           Reserved_14;
  uint8_t                           Reserved_15;
  SPD_NOMINAL_VDD_S                 Vdd;
  SPD_NOMINAL_VDQ_S                 Vdq;
  SPD_NOMINAL_VDP_S                 Vdp;
  uint8_t                           Reserved_19;
  SPD_TIMING_S                      TckAvgMin;
  SPD_TIMING_S                      TckAvgMax;
  uint8_t                           CasLatSupported[SPD_TCL_BYTES];
  uint8_t                           Reserved_29;
  SPD_TIMING_S                      TaaMin;
  SPD_TIMING_S                      TrcdMin;
  SPD_TIMING_S                      TrpMin;
  SPD_TIMING_S                      TrasMin;
  SPD_TIMING_S                      TrcMin;
  SPD_TIMING_S                      TwrMin;
  SPD_TIMING_S                      Trfc1Min;
  SPD_TIMING_S                      Trfc2Min;
  SPD_TIMING_S                      TrfcSbMin;
  SPD_TIMING_S                      Trfc1DlrMin;
  SPD_TIMING_S                      Trfc2DlrMin;
  SPD_TIMING_S                      Trfc2SbDlrMin;
  SPD_REF_MANAGEMENT_FIRST_S        PriRefManagement1st;
  SPD_REF_MANAGEMENT_SECOND_S       PriRefManagement2nd;
  SPD_REF_MANAGEMENT_FIRST_S        SecRefManagement1st;
  SPD_REF_MANAGEMENT_SECOND_S       SecRefManagement2nd;
  uint8_t                             Reserved_58_127[127-57];
} SPD_BASE_CONFIG_S;

typedef struct{
  SPD_REVISION_S                     ModuleRevision;
  uint8_t                            Reserved_193;
  SPD_MANUFACTURER_ID_S              SpdMfgID;
  SPD_MODULE_DEVICE_TYPE_SPD_S       SpdDevType;
  SPD_MODULE_REVISION_S              SpdRevision;
  SPD_MANUFACTURER_ID_S              Pmic0MfgId;
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic0DevType;
  SPD_MODULE_REVISION_S              Pmic0Revision;
  SPD_MANUFACTURER_ID_S              Pmic1MfgId;
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic1DevType;
  SPD_MODULE_REVISION_S              Pmic1Revision;
  SPD_MANUFACTURER_ID_S              Pmic2MfgId;
  SPD_MODULE_DEVICE_TYPE_PMIC_S      Pmic2DevType;
  SPD_MODULE_REVISION_S              Pmic2Revision;
  SPD_MANUFACTURER_ID_S              TsMfgId;
  SPD_DEVICE_TYPE_TS_S               TsDevType;
  SPD_MODULE_REVISION_S              TsRevision;
  uint8_t                            Reserved_214_229[16];
  SPD_MODULE_HEIGHT_S                Height;
  SPD_MODULE_THICKNESS_S             Thickness;
  SPD_RAW_CARD_S                     RawCard;
  SPD_ATTRIBUTES_S                   Attributes;
  SPD_MODULE_ORG_S                   ModuleOrg;
  SPD_CHANNEL_BUS_WIDTH_S            ChBusWidth;
  uint8_t                            Reserved_236_239[4];
} SPD_ANNEX_COMMON_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  uint8_t                            Reserved_240_447[208];
} SPD_SODOWN_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  uint8_t                            Reserved_240_447[208];
} SPD_UDIMM_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  SPD_MANUFACTURER_ID_S              RcdManfId;
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;
  SPD_MODULE_REVISION_S              RcdRevision;
  uint8_t                            Reserved_244_447[204];
} SPD_RDIMM_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  SPD_MANUFACTURER_ID_S              RcdManfId;
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;
  SPD_MODULE_REVISION_S              RcdRevision;
  SPD_MANUFACTURER_ID_S              DbMfgId;
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;
  SPD_MODULE_REVISION_S              DbRevision;
  uint8_t                            Reserved_248_447[200];
} SPD_LRDIMM_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  SPD_MANUFACTURER_ID_S              DmbManfId;
  SPD_MODULE_DEVICE_TYPE_DMB_S       DmbDevType;
  SPD_MODULE_REVISION_S              DmbRevision;
  uint8_t                            Reserved_244_447[204];
} SPD_DDIMM_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  SPD_MANUFACTURER_ID_S              RcdManfId;
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;
  SPD_MODULE_REVISION_S              RcdRevision;
  SPD_MANUFACTURER_ID_S              DbMfgId;
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;
  SPD_MODULE_REVISION_S              DbRevision;
  uint8_t                            Reserved_248_447[200];
} SPD_NVDIMM_N_ANNEX_S;

typedef struct{
  SPD_ANNEX_COMMON_S                 Common;
  SPD_MANUFACTURER_ID_S              RcdManfId;
  SPD_MODULE_DEVICE_TYPE_RCD_S       RcdDevType;
  SPD_MODULE_REVISION_S              RcdRevision;
  SPD_MANUFACTURER_ID_S              DbMfgId;
  SPD_MODULE_DEVICE_TYPE_DB_S        DbDevType;
  SPD_MODULE_REVISION_S              DbRevision;
  uint8_t                            Reserved_248_447[200];
} SPD_NVDIMM_P_ANNEX_S;

typedef union {
  SPD_ANNEX_COMMON_S                 Common;
  SPD_SODOWN_ANNEX_S                 Sodown;
  SPD_UDIMM_ANNEX_S                  Udimm;
  SPD_RDIMM_ANNEX_S                  Rdimm;
  SPD_LRDIMM_ANNEX_S                 Lrdimm;
  SPD_DDIMM_ANNEX_S                  Ddimm;
  SPD_NVDIMM_N_ANNEX_S               Nvdimmn;
  SPD_NVDIMM_P_ANNEX_S               Nvdimmp;
} SPD_MODULE_SPECIFIC_S;

typedef struct {
  SPD_MANUFACTURER_ID_S               ModuleMfgId;
  uint8_t                             ModuleMfgLocation;
  SPD_MFG_DATE_S                      ModuleMfgDate;
  SPD_MODULE_SERIAL_NUMBER_S          ModuleSerialNumber;
  SPD_MODULE_PART_NUMBER_S            ModulePartNumber;
  SPD_REVISION_S                      ModuleRevisionCode;
  SPD_MANUFACTURER_ID_S               DramMfgId;
  uint8_t                             DramStepping;
  uint8_t                             Reserved_555_639[639-554];
} SPD_MANUFACTURING_INFO_S;

typedef struct {
  SPD_BLOCK_S                         Block[SPD_NUM_BLOCKS];
} DDR5_SPD_BLOCKS_S;

typedef struct {
  SPD_BASE_CONFIG_S                   Base;
  uint8_t                             Reserved_128_191[191-127];
  SPD_MODULE_SPECIFIC_S               ModuleSpec;
  uint8_t                             Reserved_448_509[509-447];
  SPD_CRC_S                           Crc;
  SPD_MANUFACTURING_INFO_S            MfgInfo;
  SPD_BLOCK_S                         End_User_Programmable_640_703;
  SPD_BLOCK_S                         End_User_Programmable_704_767;
  SPD_BLOCK_S                         End_User_Programmable_760_831;
  SPD_BLOCK_S                         End_User_Programmable_832_895;
  SPD_BLOCK_S                         End_User_Programmable_896_959;
  SPD_BLOCK_S                         End_User_Programmable_960_1023;
} DDR5_SPD_S;

#pragma pack(pop)

#endif
