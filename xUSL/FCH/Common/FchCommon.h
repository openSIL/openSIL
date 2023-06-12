/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  FchCommon.h
 * @brief OpenSIL FCH common functions header.
 */

#pragma once
#include <FCH/Common/FchCore/FchSata/FchSataDef.h>


//-----------------------------------------------------------------------------
//                     FCH DEFINITIONS AND MACROS
//
//-----------------------------------------------------------------------------

//
// I2C
//
#define FCH_I2C0_BASE                     0xFEDC2000ul
#define FCH_I2C1_BASE                     0xFEDC3000ul
#define FCH_I2C2_BASE                     0xFEDC4000ul
#define FCH_I2C3_BASE                     0xFEDC5000ul
#define FCH_I2C4_BASE                     0xFEDC6000ul
#define FCH_I2C5_BASE                     0xFEDCB000ul

//
// Uart
//
#define FCH_UART0_BASE                    0xFEDC9000ul
#define FCH_UART1_BASE                    0xFEDCA000ul
#define FCH_UART2_BASE                    0xFEDCE000ul
#define FCH_UART3_BASE                    0xFEDCF000ul


#define FCH_GPIO_21C_UART0_CTS_L_UART2_RXD_EGPIO135             0x21C
#define FCH_GPIO_220_UART0_RXD_EGPIO136                         0x220
#define FCH_GPIO_224_UART0_RTS_L_UART2_TXD_EGPIO137             0x224
#define FCH_GPIO_228_UART0_TXD_EGPIO138                         0x228
#define FCH_GPIO_22C_UART0_INTR_AGPIO139                        0x22C
#define FCH_GPIO_234_UART1_RXD_EGPIO141                         0x234
#define FCH_GPIO_238_UART1_RTS_L_UART3_RXD_EGPIO142             0x238

/// Modes for eMMC controller
typedef enum {
  eMMC_Disable = 0,///< Disabled
  SdNormalSpeed,   ///< Normal Speed 12.5MB/s    25MHz   0x0 (v1.01)
  SdHighSpeed,     ///< High Speed   25MB/s      50MHz   0x01 (v2)
  SdUhsiSdr50,     ///< UHSI-SDR50   50MB/s      100MHz  0x2 (v3.01)
  SdUhsiDdr50,     ///< UHSI-DDR50   50MB/s      50MHz   0x2 (v3.01)
  SdUhsiSdr104,    ///< UHSI-SDR104  100MB/s     200MHz  0x2 (v3.01)
  EmmcBwComp,      ///< Backward Comp   25MB/s  25MHz   0x0 (v4.5)
  EmmcHsSdr50,     ///< High Speed SDR  50MB/s  50MHz   0x1 (v4.5)
  EmmcHsDdr100,    ///< High Speed DDR  100MB/s 50MHz   0x2 (v4.5)
  EmmcHs200,       ///< HS200   200MB/s 200MHz  0x2 (v4.5)
  EmmcHs400,       ///< HS400   400MB/s 200MHz  0x2 (v5)
  EmmcHs300        ///< HS300   300MB/s 150MHz  0x2 (v5)
} EMMC_MODE;

/// Configuration values for SdConfig
typedef enum {
  _SdDisable = 0,                      ///< Disabled
  _SdVer2,                             ///< Version 2.0
  _SdVer3,                             ///< Version 3.0
  _SdV3SDR50,                          ///< V3 SdSDR50
  _SdV3SDR104,                         ///< V3 SdSDR104
  _SdV3DDR50,                          ///< V3 SdDDR50
  _SdDump                              ///< SD DUMP, don't touch SD
} SD_MODE;

/// Configuration values for SdClockControl
typedef enum {
  _Sd50MhzTraceCableLengthWithinSixInches = 4,           ///< 50Mhz, default
  _Sd40MhzTraceCableLengthSix2ElevenInches = 6,          ///< 40Mhz
  _Sd25MhzTraceCableLengthEleven2TwentyfourInches = 7,   ///< 25Mhz
} SD_CLOCK_CONTROL;

/// Configuration values for AzaliaController
typedef enum {
  _AzAuto = 0,                         ///< Auto - Detect Azalia controller automatically
  _AzDisable,                          ///< Diable - Disable Azalia controller
  _AzEnable                            ///< Enable - Enable Azalia controller
} HDA_CONFIG;

/// Configuration values for IrConfig
typedef enum {
  _IrDisable  = 0,                     ///< Disable
  _IrRxTx0    = 1,                     ///< Rx and Tx0
  _IrRxTx1    = 2,                     ///< Rx and Tx1
  _IrRxTx0Tx1 = 3                      ///< Rx and both Tx0,Tx1
} IR_CONFIG;

/// Configuration values for FchPowerFail
typedef enum {
  AlwaysOff   = 0,                    ///< Always power off after power resumes
  AlwaysOn    = 1,                    ///< Always power on after power resumes
  UsePrevious = 3,                    ///< Resume to same setting when power fails
} POWER_FAIL;


/// Configuration values for GPIO function
typedef enum {
  Function0   = 0,                    ///< GPIO Function 1
  Function1   = 1,                    ///< GPIO Function 1
  Function2   = 2,                    ///< GPIO Function 2
  Function3   = 3,                    ///< GPIO Function 3
} GPIO_FUN;

/// Configuration values for memory phy voltage (VDDR)
#define  VOLT0_95   0                 ///< VDDR 0.95V
#define  VOLT1_05   1                 ///< VDDR 1.05V
#define  MAX_VDDR   2                 ///< Maxmum value for this enum definition

/// Configuration values for GPIO_CFG
typedef enum {
  OwnedByEc   = 1 << 0,               ///< This bit can only be written by EC
  OwnedByHost = 1 << 1,               ///< This bit can only be written by host (Host Firmware)
  Sticky      = 1 << 2,               ///< If set, [6:3] are sticky
  PullUpB     = 1 << 3,               ///< 0: Pullup enable; 1: Pullup disabled
  PullDown    = 1 << 4,               ///< 0: Pulldown disabled; 1: Pulldown enable
  GpioOutEnB  = 1 << 5,               ///< 0: Output enable; 1: Output disable
  GpioOut     = 1 << 6,               ///< Output state when GpioOutEnB is 0
  GpioIn      = 1 << 7,               ///< This bit is read only - current pin state
} CFG_BYTE;

/// Configuration values for GPIO_CFG2
typedef enum {
  DrvStrengthSel_4mA = 0 << 1,               ///< 18:17 DrvStrengthSel.
  DrvStrengthSel_8mA = 1 << 1,               ///< 18:17 DrvStrengthSel.
  DrvStrengthSel_12mA = 2 << 1,              ///< 18:17 DrvStrengthSel.
  DrvStrengthSel_16mA = 3 << 1,              ///< 18:17 DrvStrengthSel.
  PullUpSel_8K     = 1 << 3,                 ///< 19 PullUpSel. Read-write. 0=4 K pull-up is selected. 1=8 K pull-up is selected.
  PullUpEnable    = 1 << 4,                  ///< 20 PullUpEnable. Read-write. 0=Pull-up is disabled on the pin. 1=Pull-up is enabled on the pin.
  PullDownEnable  = 1 << 5,                  ///< 21 PullDownEnable. Read-write. 0=Pull-down is disabled on the pin. 1=Pull-down is enabled on thepin.
  OutputValue     = 1 << 6,                  ///< 22 OutputValue. Read-write. 0=low. 1=high.
  OutputEnable      = 1 << 7,                ///< 23 OutputEnable. Read-write. 0=Output is disabled on the pin. 1=Output is enabled on the pin.
} CFG2_BYTE;

/// FCH GPIO CONTROL
typedef struct {
  uint8_t        GpioPin;               ///< Gpio Pin, valid range: 0-67, 128-150, 160-228
  GPIO_FUN     PinFunction;           ///< Multi-function selection
  CFG_BYTE     CfgByte;               ///< GPIO Register value
} GPIO_CONTROL;

///
/// FCH SCI MAP CONTROL
///
typedef struct {
  uint8_t        InputPin;              ///< Input Pin, valid range 0-63
  uint8_t        GpeMap;                ///< Gpe Map, valid range 0-31
} SCI_MAP_CONTROL;

///
/// FCH SATA PHY CONTROL
///
typedef struct {
  bool       CommonPhy;             ///< Common PHY or not
                                      ///<   @li <b>false</b> - Only applied to specified port
                                      ///<   @li <b>true</b>  - Apply to all SATA ports
  SATA_SPEED Gen;                   ///< SATA speed
  uint8_t    Port;                  ///< Port number, valid range: 0-7
  uint32_t   PhyData;               ///< SATA PHY data, valid range: 0-0xFFFFFFFF
} SATA_PHY_CONTROL;

///
/// FCH Component Data Structure in InitReset stage
///
typedef struct {
  bool      UmiGen2;             ///< Enable Gen2 data rate of UMI
                                 ///<   @li <b>false</b> - Disable Gen2
                                 ///<   @li <b>true</b>  - Enable Gen2

  bool      SataEnable;          ///< SATA controller function
                                 ///<   @li <b>false</b> - SATA controller is disabled
                                 ///<   @li <b>true</b> - SATA controller is enabled

  bool      IdeEnable;           ///< SATA IDE controller mode enabled/disabled
                                 ///<   @li <b>false</b> - IDE controller is disabled
                                 ///<   @li <b>true</b> - IDE controller is enabled

  bool      GppEnable;           ///< Master switch of GPP function
                                 ///<   @li <b>false</b> - GPP disabled
                                 ///<   @li <b>true</b> - GPP enabled

  bool      Xhci0Enable;         ///< XHCI0 controller function
                                 ///<   @li <b>false</b> - XHCI0 controller disabled
                                 ///<   @li <b>true</b> - XHCI0 controller enabled

  bool      Xhci1Enable;         ///< XHCI1 controller function
                                 ///<   @li <b>false</b> - XHCI1 controller disabled
                                 ///<   @li <b>true</b> - XHCI1 controller enabled
} FCH_RESET_INTERFACE;

///
///  - Byte Register R/W structure
///
typedef struct _REG8_MASK {
  uint8_t                 RegIndex;                       /// RegIndex - Reserved
  uint8_t                 AndMask;                        /// AndMask - Reserved
  uint8_t                 OrMask;                         /// OrMask - Reserved
} REG8_MASK;

/********** FCH Common Ip-2-Ip Function Prototypes **************/
void
SilFchStall (
  uint32_t MicroSeconds
  );

/********** Other Function Prototypes **************/
void
FchAoacPowerOnDev (
  uint8_t Device,
  uint8_t On
  );

void
ProgramPciByteTable (
  REG8_MASK *pPciByteTable,
  uint16_t dwTableSize
  );

SIL_STATUS
InitializeFchApi (void);
