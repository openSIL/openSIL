/**
 * @file  FchEspiReg.h
 * @brief FCH ESPI registers definition
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

/**********************************************************************************************************************
 * @brief Module data structure
 *
 */

/// eSPIx00 eSPI Software Specific Register 0
typedef union {
  struct {
    uint32_t      SWCommandType:3;
    uint32_t      CommandStatus:1;
    uint32_t      SecondaryNSelect:2;
    uint32_t      Reserved_7_6:2;
    uint32_t      AddrByte0PCycTypeVWCntOOBCycType:8;
    uint32_t      AddrByte0VWIdxOOBPktByte1:8;
    uint32_t      VWDataOOBPktByte2:8;
  } Field;
  uint32_t Value;
} ESPIx00_DN_TXHDR0;

/// eSPIx04 eSPI Software Specific Register 1
typedef union {
  struct {
    uint32_t      VWIdxOOBPktbyte3:8;
    uint32_t      VWDataOOBByteCnt:8;
    uint32_t      VWIdxOOBPktByte5:8;
    uint32_t      VWDataOOBPktByte6:8;
  } Field;
  uint32_t Value;
} ESPIx04_DN_TXHDR1;

/// Offset 08h: General Capabilities and Configurations
typedef union {
  struct {
    uint32_t      RO_PCSupported:1;
    uint32_t      RO_VWSupported:1;
    uint32_t      RO_OOBMsgSupported:1;
    uint32_t      RO_FASupported:1;
    uint32_t      Reserved_7_3:4;
    uint32_t      Reserved_11_8:4;
    uint32_t      RO_MaxWaitStateAllowed:4;
    uint32_t      RO_MaxFreqSupported:3;
    uint32_t      RO_OpenDrainAlertSupported:1;
    uint32_t      OperatingFreq:3;
    uint32_t      OpenDrainAlertSelect:1;
    uint32_t      RO_IOModeSupported:2;
    uint32_t      IOModeSelect:2;
    uint32_t      AlertMode:1;
    uint32_t      Reserved_29:1;
    uint32_t      ResponseModifierEn:1;
    uint32_t      CRCCheckingEn:1;
  } Field;
  uint32_t Value;
} ESPI_SL08_SECONDARY_GENERAL_CAPCFG;

/// Offset 10h: Channel 0 Capabilities and Configurations
typedef union {
  struct {
    uint32_t      PCEn:1;
    uint32_t      RO_PCReady:1;
    uint32_t      BusMasterEn:1;
    uint32_t      Reserved_3:1;
    uint32_t      RO_PCMaxPayloadSizeSupported:3;
    uint32_t      Reserved_7:1;
    uint32_t      PCMaxPayloadSizeSelected:3;
    uint32_t      Reserved_11:1;
    uint32_t      PCMaxReadRequestSize:3;
    uint32_t      Reserved_31_15:17;
  } Field;
  uint32_t Value;
} ESPI_SL10_SECONDARY_PC_CAPCFG;

/// Offset 20h: Channel 1 Capabilities and Configurations
typedef union {
  struct {
    uint32_t      VWEn:1;
    uint32_t      RO_VWReady:1;
    uint32_t      Reserved_7_2:6;
    uint32_t      RO_MaxVWCntSupported:6;
    uint32_t      Reserved_15_14:2;
    uint32_t      OpMaxVWCnt:6;
    uint32_t      Reserved_31_22:10;
  } Field;
  uint32_t Value;
} ESPI_SL20_SECONDARY_VW_CAPCFG;


/// eSPIx2C eSPI Master Capability
typedef union {
  struct {
    uint32_t      FlashAccessChannelSupport:1;
    uint32_t      OOBMessageChannelSupport:1;
    uint32_t      VWChannelSupport:1;
    uint32_t      PChannelSupport:1;
    uint32_t      MasterVersion:3;
    uint32_t      FlashAccessChannelMaxPayload:3;
    uint32_t      OOBMessageChannelMaxPayload:3;
    uint32_t      OperatingMaxVWCount:6;
    uint32_t      PChannelMaxPayloadSize:3;
    uint32_t      NumberOfSecondary:3;
    uint32_t      OperatingSupportFreq:3;
    uint32_t      IOMode:2;
    uint32_t      AlertMode:1;
    uint32_t      CRCCheck:1;
  } Field;
  uint32_t Value;
} ESPIx2C_MASTER_CAP;

/// Offset 30h: Channel 2 Capabilities and Configurations
typedef union {
  struct {
    uint32_t      OOBEn:1;
    uint32_t      RO_OOBReady:1;
    uint32_t      Reserved_3_2:2;
    uint32_t      RO_MsgChMaxPayloadSizeSupported:3;
    uint32_t      Reserved_7:1;
    uint32_t      MsgChMaxPayloadSizeSelected:3;
    uint32_t      Reserved_31_11:21;
  } Field;
  uint32_t Value;
} ESPI_SL30_SECONDARY_OOB_CAPCFG;

/// Offset 40h: Channel 2 Capabilities and Configurations
typedef union {
  struct {
    uint32_t      FAEn:1;
    uint32_t      RO_FAReady:1;
    uint32_t      FlashBlockEraseSize:3;
    uint32_t      RO_ChMaxPayloadSizeSupported:3;
    uint32_t      ChMaxPayloadSizeSelected:3;
    uint32_t      RO_FlashSharingMode:1;
    uint32_t      ChMaxReadReqSize:3;
    uint32_t      Reserved_31_15:17;
  } Field;
  uint32_t Value;
} ESPI_SL40_SECONDARY_FA_CAPCFG;

/// eSPIx68 eSPI Secondary N Configuration
typedef union {
  struct {
    uint32_t      FlashAccessChannelEnable:1;
    uint32_t      OOBMessageChannelEnable:1;
    uint32_t      VWChannelEnable:1;
    uint32_t      PChannelEnable:1;
    uint32_t      OOBValidBitEnable:1;
    uint32_t      Reserved_24_5:20;
    uint32_t      OperatingFreq:3;
    uint32_t      IOModeSelect:2;
    uint32_t      AlertMode:1;
    uint32_t      CRCCheckingEnable:1;
  } Field;
  uint32_t Value;
} ESPIx68_SECONDARY0_CONFIG;

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define ACPI_MMIO_BASE            0xFED80000ul
#define FCH_LPC_BUS               0
#define FCH_LPC_DEV               20
#define FCH_LPC_FUNC              3

#define SIL_RESERVED_29           0x44
#define SIL_RESERVED_30           0x78
#define IOMUX_BASE                0xD00 // BYTE
#define FCH_AOAC_RETRY            100
#define FCH_AOAC_ESPI             0x1B
#define MAX_ESPI_RETRY            100000
#define ESPI_DN_TXHDR_0           0x0
#define DNCMD_STATUS              BIT_32(3)
#define ESPI_DN_TXHDR_1           0x04
#define ESPI_SECONDARY_GENERAL_CAPCFG 0x08
#define ESPI_MASTER_CAP           0x2C
#define ESPI_SECONDARY0_CONFIG        0x68
#define ESPI_SECONDARY_PC_CAPCFG      0x10
#define ESPI_SECONDARY_VW_CAPCFG      0x20
#define ESPI_SECONDARY_OOB_CAPCFG     0x30
#define ESPI_SECONDARY_FA_CAPCFG      0x40
#define SECONDARY_FA_ENABLE           BIT_32(0)
#define SECONDARY_OOB_ENABLE          BIT_32(1)
#define SECONDARY_VW_ENABLE           BIT_32(2)
#define SECONDARY_PC_ENABLE           BIT_32(3)
#define SET_CONFIGURATION         0
#define GET_CONFIGURATION         1
#define IN_BAND_RESET             2
