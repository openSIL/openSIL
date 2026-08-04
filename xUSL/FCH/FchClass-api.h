/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchClass-api.h
 * @brief openSIL-Host FCH IP interface
 *
 * @details The FCH is a bridge device under which many of the legacy and
 * traditional components of a PC are located.
 *
 *  This file provides the structures details for the Host to configure these
 *  legacy device operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_FCH  FCH api
 *
 *   The FCH is the portion of the SoC that contain most of the IO device and
 *   support IP blocks. The FCH is itself a collection of smaller IP
 *   blocks which are split out separately in openSIL (e.g. XHCI
 *   (USB3) ). This still leaves a 'glue logic' level in the FCH module
 *   that needs configuration.
 *
 *   See the 'Files - @ref FCHClass-api.h' section of this document for
 *   further details.
 *
 *   Blocks comprising this class are:
 *   - @subpage IP_USB3 "USB 3.0"
 *
 * @endcond
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <FchHwAcpi-api.h>
#include <FchAb-api.h>
#include <FchIsa-api.h>
#include <FchUsb-api.h>

#pragma pack (push, 1)

///
/// Hpet structure
///
typedef struct {
  bool               HpetEnable;                        // HPET function switch
  bool               HpetMsiDis;                        // HpetMsiDis - South Bridge HPET MSI Configuration
                                                        // <b>1</b> - disable
                                                        // <b>0</b> - enable
  uint32_t           HpetBase;                          // HPET Base address
} FCH_HPET;

///
/// FCH HID control
///
typedef struct {
  uint8_t            SpiReadMode;                ///< Spi Read Mode 5-Quad 4-Dual 7-Single
  uint8_t            SpiSpeed;                   ///< Spi Speed
                                                 /// 0-66Mhz 1-33Mhz 2-22Mhz 3-17.6Mhz
                                                 /// 4-100Mz 5-800Khz 6-Spi_sp6 7-Spi_sp7
  uint8_t            Spi_spd6;                   ///< Speed = 200Mhz/Spi_spd6
  uint8_t            Spi_spd7;                   ///< Speed = 200Mhz/Spi_spd7
} HID_CONTROL;

///
/// GCPU related parameters
///
typedef struct {
  uint8_t           AcDcMsg;                // Send a message to CPU to indicate the power mode (AC vs battery)
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           TimerTickTrack;         // Send a message to CPU to indicate the latest periodic timer interval
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           ClockInterruptTag;      // Mark the periodic timer interrupt
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           OhciTrafficHanding;     // Cause CPU to break out from C state when USB OHCI has pending traffic
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           EhciTrafficHanding;     // Cause CPU to break out from C state when USB EHCI has pending traffic
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           GcpuMsgCMultiCore;      // Track of CPU C state by monitoring each core's C state message
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
  uint8_t           GcpuMsgCStage;          // Enable the FCH C state coordination logic
                                            //  <b>1</b> - disable
                                            //  <b>0</b> - enable
} FCH_GCPU;

///
/// Timer
///
typedef struct {
  bool               Enable;                         // Whether to register timer SMI in POST
  bool               StartNow;                       // Whether to start the SMI immediately during registration
  uint16_t           CycleDuration;                  // [14:0] - Actual cycle duration = CycleDuration + 1
} TIMER_SMI;

///
/// CS support
///
typedef struct {
  bool               FchCsD3Cold;                    // FCH Cs D3 Cold function
  bool               FchCsHwReduced;                 // FCH Cs hardware reduced ACPI flag
  bool               FchCsPwrBtn;                    // FCH Cs Power Button function
  bool               FchCsAcDc;                      // FCH Cs AcDc function
  bool               AsfNfcEnable;                   // FCH Cs NFC function
  uint8_t            AsfNfcInterruptPin;             // NFC Interrupt pin define
  uint8_t            AsfNfcRegPuPin;                 // NFC RegPu pin define
  uint8_t            AsfNfcWakePin;                  // NFC Wake Pin define
  uint8_t            PowerButtonGpe;                 // GPE# used by Power Button device
  uint8_t            AcDcTimerGpe;                   // GPE# used by Timer device
  bool               FchModernStandby;               // FCH Modern Standby function
} FCH_CS;

///
/// CPPC support
///
typedef struct {
  bool               CppcSupport;                    // CPPC feature support, FCH need configure SCI for CPPC
  uint32_t           SciBit;                         // SCI bit reserved for CPPC
} FCH_CPPC;

///
/// MISC structure
///
typedef struct {
  bool               NativePcieSupport;              // PCIe NativePcieSupport - Debug function. 1:Enabled, 0:Disabled
  bool               S3Resume;                       // S3Resume - Flag of ACPI S3 Resume.
  bool               RebootRequired;                 // RebootRequired - Flag of Reboot system is required.
  uint8_t            FchVariant;                     // FchVariant - FCH Variant value.
  uint8_t            Cg2Pll;                         // CG2 PLL - 0:disable, 1:enable
  TIMER_SMI          LongTimer;                      // Long Timer SMI
  TIMER_SMI          ShortTimer;                     // Short Timer SMI
  uint32_t           FchCpuId;                       // Saving CpuId for FCH Module.
  bool               NoneSioKbcSupport;              // NoneSioKbcSupport - No KBC/SIO controller
  FCH_CS             FchCsSupport;                   // FCH Cs function structure
  FCH_CPPC           Cppc;                           // FCH CPPC support
  bool               FchiLa1MTraceMemoryEn;          // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Enable
  uint32_t           FchiLa1MTraceMemoryBase;        // FchiLa1MTraceMemoryBase - Fch iLa 1M Trace Memory Base
  uint8_t            FchReadyToBootSmi;              // FchReadyToBootSmi - Fch Ready To Boot Smi
} FCH_MISC;

///
/// SMBus structure
///
typedef struct {
  uint32_t           SmbusSsid;               // SMBUS controller Subsystem ID
} FCH_SMBUS;

///
/// Fch Run Time Parameters
///
typedef struct {
  uint32_t                PcieMmioBase;                     ///< PcieMmioBase
  uint32_t                FchDeviceEnableMap;               ///< FchDeviceEnableMap
                                                            ///< Indicate FCH devices map
                                                            ///< BIT4  - LPC   : LpcEnable
                                                            ///< BIT5  - I2C0  : FchRTDeviceEnableMap[BIT5]
                                                            ///< BIT6  - I2C1  : FchRTDeviceEnableMap[BIT6]
                                                            ///< BIT7  - I2C2  : FchRTDeviceEnableMap[BIT7]
                                                            ///< BIT8  - I2C3  : FchRTDeviceEnableMap[BIT8]
                                                            ///< BIT9  - I2C4  : FchRTDeviceEnableMap[BIT9]
                                                            ///< BIT10 - I2C5  : FchRTDeviceEnableMap[BIT10]
                                                            ///< BIT11 - UART0 : FchRTDeviceEnableMap[BIT11]
                                                            ///< BIT12 - UART1 : FchRTDeviceEnableMap[BIT12]
                                                            ///< BIT13 - I3C1  : FchRTDeviceEnableMap[BIT13]
                                                            ///< BIT14 - I3C2  : FchRTDeviceEnableMap[BIT14]
                                                            ///< BIT15 - I3C3  : FchRTDeviceEnableMap[BIT15]
                                                            ///< BIT16 - UART2 : FchRTDeviceEnableMap[BIT16]
                                                            ///< BIT18 - SD    : EmmcEnable and EmmcType < 5
                                                            ///< BIT20 - UART4 : FchRTDeviceEnableMap[BIT20]
                                                            ///< BIT21 - I3C0  : FchRTDeviceEnableMap[BIT21]
                                                            ///< BIT26 - UART3 : FchRTDeviceEnableMap[BIT26]
                                                            ///< BIT27 - eSPI  : EspiEnable
                                                            ///< BIT28 - eMMC  : EmmcEnable
                                                            ///< BIT29 - HFP Enable
                                                            ///< BIT30 - HID Enable
                                                            ///< BIT31 - HID2 Enable
  uint32_t                FchDeviceD3ColdMap;               ///< FchDeviceD3ColdMap
  uint16_t                Al2AhbLegacyUartIoEnable;         ///< Al2Ahb Legacy Uart Io Enable
  uint8_t                 Uart0Irq;                         ///< Uart 0 Irq
  uint8_t                 Uart1Irq;                         ///< Uart 1 Irq
  uint8_t                 Uart2Irq;                         ///< Uart 2 Irq
  uint8_t                 Uart3Irq;                         ///< Uart 3 Irq
  uint8_t                 Uart4Irq;                         ///< Uart 4 Irq
  uint8_t                 I2c0Irq;                          ///< I2c 0 Irq
  uint8_t                 I2c1Irq;                          ///< I2c 1 Irq
  uint8_t                 I2c2Irq;                          ///< I2c 2 Irq
  uint8_t                 I2c3Irq;                          ///< I2c 3 Irq
  uint8_t                 I2c4Irq;                          ///< I2c 4 Irq
  uint8_t                 I2c5Irq;                          ///< I2c 5 Irq
  HID_CONTROL             HidControl[2];                    ///< Hid Control for HID and HID2
  uint16_t                FchAcpiDeviceInvisibleMap;        ///< FchAcpiDeviceInvisibleMap - Offset (0x2D)
                                                            ///< BIT0 = 1: UART0 ACPI Device is invisible
                                                            ///< BIT1 = 1: UART1 ACPI Device is invisible
                                                            ///< BIT2 = 1: UART2 ACPI Device is invisible
                                                            ///< BIT3 = 1: UART3 ACPI Device is invisible
                                                            ///< BIT4 = 1: UART4 ACPI Device is invisible
                                                            ///< BIT8  = 1: UART0 Legacy IO ACPI Device is invisible
                                                            ///< BIT9  = 1: UART1 Legacy IO ACPI Device is invisible
                                                            ///< BIT10 = 1: UART2 Legacy IO ACPI Device is invisible
                                                            ///< BIT11 = 1: UART3 Legacy IO ACPI Device is invisible
} FCH_RUNTIME;

///
/// EC structure
///
typedef struct _FCH_EC {
  uint8_t                 MsgFun81Zone0MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun81Zone0MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun81Zone0MsgReg2;           ///< Thermal zone control byte 1
  uint8_t                 MsgFun81Zone0MsgReg3;           ///< Thermal zone control byte 2
  uint8_t                 MsgFun81Zone0MsgReg4;           ///< Bit[3:0] - Thermal diode offset adjustment in degrees
                                                          ///< Celsius.
  uint8_t                 MsgFun81Zone0MsgReg5;           ///< Hysteresis information
  uint8_t                 MsgFun81Zone0MsgReg6;           ///< SMBUS Address for SMBUS based temperature sensor such as
                                                          ///< SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone0MsgReg7;           ///< Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based
                                                          ///< temperature sensor is located.
  uint8_t                 MsgFun81Zone0MsgReg8;           ///< Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone0MsgReg9;           ///< Fan PWM ramping rate in 5ms unit
//
// EC LDN9 function 81 zone 1
//
  uint8_t                 MsgFun81Zone1MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun81Zone1MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun81Zone1MsgReg2;           ///< Thermal zone control byte 1
  uint8_t                 MsgFun81Zone1MsgReg3;           ///< Thermal zone control byte 2
  uint8_t                 MsgFun81Zone1MsgReg4;           ///< Bit[3:0] - Thermal diode offset adjustment in degrees
                                                          ///< Celsius.
  uint8_t                 MsgFun81Zone1MsgReg5;           ///< Hysteresis information
  uint8_t                 MsgFun81Zone1MsgReg6;           ///< SMBUS Address for SMBUS based temperature sensor such as
                                                          ///< SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone1MsgReg7;           ///< Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based
                                                          ///< temperature sensor is located.
  uint8_t                 MsgFun81Zone1MsgReg8;           ///< Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone1MsgReg9;           ///< Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 81 zone 2
//
  uint8_t                 MsgFun81Zone2MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun81Zone2MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun81Zone2MsgReg2;           ///< Thermal zone control byte 1
  uint8_t                 MsgFun81Zone2MsgReg3;           ///< Thermal zone control byte 2
  uint8_t                 MsgFun81Zone2MsgReg4;           ///< Bit[3:0] - Thermal diode offset adjustment in degrees
                                                          ///< Celsius.
  uint8_t                 MsgFun81Zone2MsgReg5;           ///< Hysteresis information
  uint8_t                 MsgFun81Zone2MsgReg6;           ///< SMBUS Address for SMBUS based temperature sensor such as
                                                          ///< SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone2MsgReg7;           ///< Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based
                                                          ///< temperature sensor is located.
  uint8_t                 MsgFun81Zone2MsgReg8;           ///< Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone2MsgReg9;           ///< Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 81 zone 3
//
  uint8_t                 MsgFun81Zone3MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun81Zone3MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun81Zone3MsgReg2;           ///< Thermal zone control byte 1
  uint8_t                 MsgFun81Zone3MsgReg3;           ///< Thermal zone control byte 2
  uint8_t                 MsgFun81Zone3MsgReg4;           ///< Bit[3:0] - Thermal diode offset adjustment in degrees
                                                          ///< Celsius.
  uint8_t                 MsgFun81Zone3MsgReg5;           ///< Hysteresis information
  uint8_t                 MsgFun81Zone3MsgReg6;           ///< SMBUS Address for SMBUS based temperature sensor such as
                                                          ///< SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone3MsgReg7;           ///< Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based
                                                          ///< temperature sensor is located.
  uint8_t                 MsgFun81Zone3MsgReg8;           ///< Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone3MsgReg9;           ///< Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 83 zone 0
//
  uint8_t                 MsgFun83Zone0MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun83Zone0MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun83Zone0MsgReg2;           ///< _AC0
  uint8_t                 MsgFun83Zone0MsgReg3;           ///< _AC1
  uint8_t                 MsgFun83Zone0MsgReg4;           ///< _AC2
  uint8_t                 MsgFun83Zone0MsgReg5;           ///< _AC3
  uint8_t                 MsgFun83Zone0MsgReg6;           ///< _AC4
  uint8_t                 MsgFun83Zone0MsgReg7;           ///< _AC5
  uint8_t                 MsgFun83Zone0MsgReg8;           ///< _AC6
  uint8_t                 MsgFun83Zone0MsgReg9;           ///< _AC7
  uint8_t                 MsgFun83Zone0MsgRegA;           ///< _CRT
  uint8_t                 MsgFun83Zone0MsgRegB;           ///< _PSV
//
//EC LDN9 function 83 zone 1
//
  uint8_t                 MsgFun83Zone1MsgReg0;           ///< Thermal zone
  uint8_t                 MsgFun83Zone1MsgReg1;           ///< Thermal zone
  uint8_t                 MsgFun83Zone1MsgReg2;           ///< _AC0
  uint8_t                 MsgFun83Zone1MsgReg3;           ///< _AC1
  uint8_t                 MsgFun83Zone1MsgReg4;           ///< _AC2
  uint8_t                 MsgFun83Zone1MsgReg5;           ///< _AC3
  uint8_t                 MsgFun83Zone1MsgReg6;           ///< _AC4
  uint8_t                 MsgFun83Zone1MsgReg7;           ///< _AC5
  uint8_t                 MsgFun83Zone1MsgReg8;           ///< _AC6
  uint8_t                 MsgFun83Zone1MsgReg9;           ///< _AC7
  uint8_t                 MsgFun83Zone1MsgRegA;           ///< _CRT
  uint8_t                 MsgFun83Zone1MsgRegB;           ///< _PSV
//
//EC LDN9 function 83 zone 2
//
  uint8_t                 MsgFun83Zone2MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun83Zone2MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun83Zone2MsgReg2;           ///<_AC0
  uint8_t                 MsgFun83Zone2MsgReg3;           ///<_AC1
  uint8_t                 MsgFun83Zone2MsgReg4;           ///<_AC2
  uint8_t                 MsgFun83Zone2MsgReg5;           ///<_AC3
  uint8_t                 MsgFun83Zone2MsgReg6;           ///<_AC4
  uint8_t                 MsgFun83Zone2MsgReg7;           ///<_AC5
  uint8_t                 MsgFun83Zone2MsgReg8;           ///<_AC6
  uint8_t                 MsgFun83Zone2MsgReg9;           ///<_AC7
  uint8_t                 MsgFun83Zone2MsgRegA;           ///<_CRT
  uint8_t                 MsgFun83Zone2MsgRegB;           ///<_PSV
//
//EC LDN9 function 83 zone 3
//
  uint8_t                 MsgFun83Zone3MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun83Zone3MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun83Zone3MsgReg2;           ///<_AC0
  uint8_t                 MsgFun83Zone3MsgReg3;           ///<_AC1
  uint8_t                 MsgFun83Zone3MsgReg4;           ///<_AC2
  uint8_t                 MsgFun83Zone3MsgReg5;           ///<_AC3
  uint8_t                 MsgFun83Zone3MsgReg6;           ///<_AC4
  uint8_t                 MsgFun83Zone3MsgReg7;           ///<_AC5
  uint8_t                 MsgFun83Zone3MsgReg8;           ///<_AC6
  uint8_t                 MsgFun83Zone3MsgReg9;           ///<_AC7
  uint8_t                 MsgFun83Zone3MsgRegA;           ///<_CRT
  uint8_t                 MsgFun83Zone3MsgRegB;           ///<_PSV
//
//EC LDN9 function 85 zone 0
//
  uint8_t                 MsgFun85Zone0MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun85Zone0MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun85Zone0MsgReg2;           ///<AL0 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg3;           ///<AL1 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg4;           ///<AL2 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg5;           ///<AL3 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg6;           ///<AL4 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg7;           ///<AL5 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg8;           ///<AL6 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone0MsgReg9;           ///<AL7 PWM level in percentage (0 - 100%)
//
//EC LDN9 function 85 zone 1
//
  uint8_t                 MsgFun85Zone1MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun85Zone1MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun85Zone1MsgReg2;           ///<AL0 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg3;           ///<AL1 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg4;           ///<AL2 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg5;           ///<AL3 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg6;           ///<AL4 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg7;           ///<AL5 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg8;           ///<AL6 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone1MsgReg9;           ///<AL7 PWM level in percentage (0 - 100%)
//
//EC LDN9 function 85 zone 2
//
  uint8_t                 MsgFun85Zone2MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun85Zone2MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun85Zone2MsgReg2;           ///<AL0 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg3;           ///<AL1 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg4;           ///<AL2 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg5;           ///<AL3 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg6;           ///<AL4 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg7;           ///<AL5 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg8;           ///<AL6 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone2MsgReg9;           ///<AL7 PWM level in percentage (0 - 100%)
//uint8_t
//EC LDN9 function 85 zone 3
//
  uint8_t                 MsgFun85Zone3MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun85Zone3MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun85Zone3MsgReg2;           ///<AL0 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg3;           ///<AL1 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg4;           ///<AL2 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg5;           ///<AL3 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg6;           ///<AL4 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg7;           ///<AL5 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg8;           ///<AL6 PWM level in percentage (0 - 100%)
  uint8_t                 MsgFun85Zone3MsgReg9;           ///<AL7 PWM level in percentage (0 - 100%)
//uint8_t
//EC LDN9 function 89 TEMPIN channel 0
//
  uint8_t                 MsgFun89Zone0MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun89Zone0MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun89Zone0MsgReg2;           ///<At DWORD bit 0-7
  uint8_t                 MsgFun89Zone0MsgReg3;           ///<At DWORD bit 15-8
  uint8_t                 MsgFun89Zone0MsgReg4;           ///<At DWORD bit 23-16
  uint8_t                 MsgFun89Zone0MsgReg5;           ///<At DWORD bit 31-24
  uint8_t                 MsgFun89Zone0MsgReg6;           ///<Ct DWORD bit 0-7
  uint8_t                 MsgFun89Zone0MsgReg7;           ///<Ct DWORD bit 15-8
  uint8_t                 MsgFun89Zone0MsgReg8;           ///<Ct DWORD bit 23-16
  uint8_t                 MsgFun89Zone0MsgReg9;           ///<Ct DWORD bit 31-24
  uint8_t                 MsgFun89Zone0MsgRegA;           ///<Mode bit 0-7
//
//EC LDN9 function 89 TEMPIN channel 1
//
  uint8_t                 MsgFun89Zone1MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun89Zone1MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun89Zone1MsgReg2;           ///<At DWORD bit 0-7
  uint8_t                 MsgFun89Zone1MsgReg3;           ///<At DWORD bit 15-8
  uint8_t                 MsgFun89Zone1MsgReg4;           ///<At DWORD bit 23-16
  uint8_t                 MsgFun89Zone1MsgReg5;           ///<At DWORD bit 31-24
  uint8_t                 MsgFun89Zone1MsgReg6;           ///<Ct DWORD bit 0-7
  uint8_t                 MsgFun89Zone1MsgReg7;           ///<Ct DWORD bit 15-8
  uint8_t                 MsgFun89Zone1MsgReg8;           ///<Ct DWORD bit 23-16
  uint8_t                 MsgFun89Zone1MsgReg9;           ///<Ct DWORD bit 31-24
  uint8_t                 MsgFun89Zone1MsgRegA;           ///<Mode bit 0-7
//
//EC LDN9 function 89 TEMPIN channel 2
//
  uint8_t                 MsgFun89Zone2MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun89Zone2MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun89Zone2MsgReg2;           ///<At DWORD bit 0-7
  uint8_t                 MsgFun89Zone2MsgReg3;           ///<At DWORD bit 15-8
  uint8_t                 MsgFun89Zone2MsgReg4;           ///<At DWORD bit 23-16
  uint8_t                 MsgFun89Zone2MsgReg5;           ///<At DWORD bit 31-24
  uint8_t                 MsgFun89Zone2MsgReg6;           ///<Ct DWORD bit 0-7
  uint8_t                 MsgFun89Zone2MsgReg7;           ///<Ct DWORD bit 15-8
  uint8_t                 MsgFun89Zone2MsgReg8;           ///<Ct DWORD bit 23-16
  uint8_t                 MsgFun89Zone2MsgReg9;           ///<Ct DWORD bit 31-24
  uint8_t                 MsgFun89Zone2MsgRegA;           ///<Mode bit 0-7
//
//EC LDN9 function 89 TEMPIN channel 3
//
  uint8_t                 MsgFun89Zone3MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun89Zone3MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun89Zone3MsgReg2;           ///<At DWORD bit 0-7
  uint8_t                 MsgFun89Zone3MsgReg3;           ///<At DWORD bit 15-8
  uint8_t                 MsgFun89Zone3MsgReg4;           ///<At DWORD bit 23-16
  uint8_t                 MsgFun89Zone3MsgReg5;           ///<At DWORD bit 31-24
  uint8_t                 MsgFun89Zone3MsgReg6;           ///<Ct DWORD bit 0-7
  uint8_t                 MsgFun89Zone3MsgReg7;           ///<Ct DWORD bit 15-8
  uint8_t                 MsgFun89Zone3MsgReg8;           ///<Ct DWORD bit 23-16
  uint8_t                 MsgFun89Zone3MsgReg9;           ///<Ct DWORD bit 31-24
  uint8_t                 MsgFun89Zone3MsgRegA;           ///<Mode bit 0-7
//
//EC LDN9 function 8C Startup PWM channel 0
//
  uint8_t                 MsgFun8CZone0MsgReg0;           ///<Return 0xFA stands for success
  uint8_t                 MsgFun8CZone0MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone0MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone,
                                                          ///< bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone0MsgReg3;           ///<Startup PWM (effective range 1~100)
//
//EC LDN9 function 8C Startup PWM channel 1
//
  uint8_t                 MsgFun8CZone1MsgReg0;           ///<Return 0xFA stands for success
  uint8_t                 MsgFun8CZone1MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone1MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone,
                                                          ///< bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone1MsgReg3;           ///<Startup PWM (effective range 1~100)
//uint8_t
//EC LDN9 function 8C Startup PWM channel 2
//
  uint8_t                 MsgFun8CZone2MsgReg0;           ///<Return 0xFA stands for success
  uint8_t                 MsgFun8CZone2MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone2MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone,
                                                          ///< bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone2MsgReg3;           ///<Startup PWM (effective range 1~100)
//
//EC LDN9 function 8C Startup PWM channel 3
//
  uint8_t                 MsgFun8CZone3MsgReg0;           ///<Return 0xFA stands for success
  uint8_t                 MsgFun8CZone3MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone3MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone,
                                                          ///< bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone3MsgReg3;           ///<Startup PWM (effective range 1~100)
//
// FLAG for Fun83/85/89/8C support
//
  uint32_t                IMCFUNSupportBitMap;            ///< Bit0=81FunZone0 support(1=On;0=Off);
                                                          ///< bit1-3=81FunZone1-Zone3;
                                                          ///< Bit4-7=83FunZone0-Zone3;
                                                          ///< Bit8-11=85FunZone0-Zone3;
                                                          ///< Bit11-15=89FunZone0-Zone3;
} FCH_EC;

///
/// IMC structure
///
typedef struct _FCH_IMC {
  uint8_t                 ImcEnable;                      ///< ImcEnable - IMC Enable
  uint8_t                 ImcEnabled;                     ///< ImcEnabled - IMC Enable
  uint8_t                 ImcSureBootTimer;               ///< ImcSureBootTimer - IMc SureBootTimer function
  FCH_EC                  EcStruct;                       ///< EC structure
  uint8_t                 ImcEnableOverWrite;             ///< OverWrite IMC with the EC structure
                                                          ///   @li <b>00</b> - by default strapping
                                                          ///   @li <b>01</b> - enable
                                                          ///   @li <b>10</b> - disable
                                                          ///
} FCH_IMC;

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

///
/// SD structure
///
typedef struct {
  SD_MODE               SdConfig;                       ///< SD Mode configuration
                                                        ///   @li <b>00</b> - Disabled
                                                        ///   @li <b>00</b> - AMDA
                                                        ///   @li <b>01</b> - DMA
                                                        ///   @li <b>10</b> - PIO
                                                        ///
  uint8_t                 SdSpeed;                      ///< SD Speed
                                                        ///   @li <b>0</b> - Low speed
                                                        ///   @li <b>1</b> - High speed
                                                        ///
  uint8_t                 SdBitWidth;                   ///< SD Bit Width
                                                        ///   @li <b>0</b> - 32BIT clear 23
                                                        ///   @li <b>1</b> - 64BIT, set 23,default
                                                        ///
  uint32_t                SdSsid;                       ///< SD Subsystem ID
  SD_CLOCK_CONTROL        SdClockControl;               ///< SD Clock Control
  bool                    SdClockMultiplier;            ///< SD Clock Multiplier enable/disable
  uint8_t                 SdReTuningMode;               ///< SD Re-tuning modes select
                                                        ///    @li <b>0</b> - mode 1
                                                        ///    @li <b>1</b> - mode 2
                                                        ///    @li <b>2</b> - mode 3
  uint8_t                 SdHostControllerVersion;      ///< SD controller Version
                                                        ///    @li <b>1</b> - SD 2.0
                                                        ///    @li <b>2</b> - SD 3.0
  uint8_t                 SdrCapabilities;              ///< SDR Capability mode select
                                                        ///    @li <b>00</b> - SDR25/15
                                                        ///    @li <b>01</b> - SDR50
                                                        ///    @li <b>11</b> - SDR104
  uint8_t                 SdSlotType;                   ///< SDR Slot Type select
                                                        ///    @li <b>00</b> - Removable Card Slot
                                                        ///    @li <b>01</b> - Embedded Slot for One Device
                                                        ///    @li <b>10</b> - Shared Bus Slot
  bool                    SdForce18;                    ///< SD Force18
  uint8_t                 SdDbgConfig;                  ///< SD Mode configuration
                                                        ///   @li <b>00</b> - Disabled
                                                        ///   @li <b>00</b> - AMDA
                                                        ///   @li <b>01</b> - DMA
                                                        ///   @li <b>10</b> - PIO
                                                        ///
} FCH_SD;

///
/// ASF structure
///
typedef struct {
  bool               DisableMaster;                 // Disable ASF Master
  bool               DisableSecondary;              // Disable ASF Secondary
} FCH_ASF;

/// Configuration values for GPIO function
typedef enum {
  Function0 = 0,                   ///< GPIO Function 1
  Function1 = 1,                   ///< GPIO Function 1
  Function2 = 2,                   ///< GPIO Function 2
  Function3 = 3,                   ///< GPIO Function 3
} GPIO_FUN;

/// Configuration values for GPIO_CFG
typedef enum {
  OwnedByEc = 1 << 0,           ///< This bit can only be written by EC
  OwnedByHost = 1 << 1,         ///< This bit can only be written by host (BIOS)
  Sticky = 1 << 2,              ///< If set, [6:3] are sticky
  PullUpB = 1 << 3,             ///< 0: Pullup enable; 1: Pullup disabled
  PullDown = 1 << 4,            ///< 0: Pulldown disabled; 1: Pulldown enable
  GpioOutEnB = 1 << 5,          ///< 0: Output enable; 1: Output disable
  GpioOut = 1 << 6,             ///< Output state when GpioOutEnB is 0
  GpioIn = 1 << 7,              ///< This bit is read only - current pin state
} CFG_BYTE;

/// FCH GPIO CONTROL
typedef struct {
  uint8_t        GpioPin;             ///< Gpio Pin, valid range: 0-67, 128-150, 160-228
  GPIO_FUN     PinFunction;           ///< Multi-function selection
  CFG_BYTE     CfgByte;               ///< GPIO Register value
} GPIO_CONTROL;


/// FCH Platform Configuration Policy
typedef struct {
  uint16_t     CfgSmbus0BaseAddress;        ///< SMBUS0 Controller Base Address
  uint16_t     CfgSioPmeBaseAddress;        ///< I/O base address for LPC I/O target range
  uint16_t     CfgAcpiPm1EvtBlkAddr;        ///< I/O base address of ACPI power management Event Block
  uint16_t     CfgAcpiPm1CntBlkAddr;        ///< I/O base address of ACPI power management Control Block
  uint16_t     CfgAcpiPmTmrBlkAddr;         ///< I/O base address of ACPI power management Timer Block
  uint16_t     CfgCpuControlBlkAddr;        ///< I/O base address of ACPI power management CPU Control Block
  uint16_t     CfgAcpiGpe0BlkAddr;          ///< I/O base address of ACPI power management General Purpose Event Block
  uint16_t     CfgSmiCmdPortAddr;           ///< I/O base address of ACPI SMI Command Block
  uint32_t     CfgGecShadowRomBase;         ///< 32-bit base address to the GEC shadow ROM
  uint32_t     CfgWatchDogTimerBase;        ///< Watchdog Timer base address
  uint32_t     CfgHpetBaseAddress;          ///< HPET MMIO base address
  uint32_t     CfgAzaliaSsid;               ///< Subsystem ID of HD Audio controller
  uint32_t     CfgSmbusSsid;                ///< Subsystem ID of SMBUS controller
  uint32_t     CfgEhciSsid;                 ///< Subsystem ID of EHCI
  uint32_t     CfgOhciSsid;                 ///< Subsystem ID of OHCI
  uint32_t     CfgSdSsid;                   ///< Subsystem ID of SecureDigital controller
  uint32_t     CfgXhciSsid;                 ///< Subsystem ID of XHCI
  bool         CfgFchPort80BehindPcib;      ///< Is port80 cycle going to the PCI bridge
  bool         CfgFchEnableAcpiSleepTrap;   ///< ACPI sleep SMI enable/disable
  uint8_t      CfgFchIrPinControl;          ///< Register bitfield describing Infrared Pin Control:
                                            ///<   [0] - IR Enable 0
                                            ///<   [1] - IR Enable 1
                                            ///<   [2] - IR Tx0
                                            ///<   [3] - IR Tx1
                                            ///<   [4] - IR Open Drain
                                            ///<   [5] - IR Enable LED
  SD_CLOCK_CONTROL  CfgFchSdClockControl;   ///< FCH SD Clock Control

  GPIO_CONTROL     *CfgFchGpioControl;          ///< FCH GPIO Control
  bool              CfgFchUsbPortDisWorkAround; ///< FCH USB Workaround
} FCH_PLATFORM_POLICY;

/*
 *  Define the FCH class module's Input Block
 */
typedef struct {
  FCH_RUNTIME           FchRunTime;                     ///< FCH Run Time Parameters
  FCH_SMBUS             Smbus;                          ///< SMBus structure
  FCH_SD                Sd;                             ///< SD structure
  FCH_HPET              Hpet;                           ///< HPET structure
  FCH_GCPU              Gcpu;                           ///< GCPU structure
  FCH_IMC               Imc;                            ///< IMC structure
  FCH_MISC              Misc;                           ///< MISC structure
  bool                  EcKbd;                          ///< EC KBD - 0:disable, 1:enable
  bool                  LegacyFree;                     ///< Legacy Free - 0:disable, 1:enable
  bool                  FchOscout1ClkContinuous;        ///< FCH OSCOUT1_CLK Continuous
  uint8_t               LpcClockDriveStrength;          ///< Lpc Clock Drive Strength
  uint8_t               LpcClockDriveStrengthRiseTime;  ///< Lpc Clock Drive Strength Rise Time
  uint8_t               LpcClockDriveStrengthFallTime;  ///< Lpc Clock Drive Strength Fall Time
  bool                  EspiEnable;                     ///< ESPI controller - 0:disable, 1:enable
  bool                  EspiIo80Enable;                 ///< ESPI 80h Port - 0:disable, 1:enable
  bool                  EspiKbc6064Enable;              ///< ESPI 60h/65h  - 0:disable, 1:enable
  bool                  EspiEc0Enable;                  ///< ESPI EC  - 0:disable, 1:enable
  bool                  WdtEnable;                      ///< enable FCH WatchDog Timer (WDT)
  uint32_t              FchAcpiMmioBase;                ///< FCH ACPI MMIO Base
  FCH_PLATFORM_POLICY   FchBldCfg;                      /// Build Options
  bool                  SerialIrqEnable;                ///< Enable the serial IRQ function
  FCH_ASF               FchAsfCfg;
  bool                  CfgIoApicIdPreDefEnable;        ///Configure IOAPIC ID pre define enable.
  uint8_t               FchIoApicId;                    ///IoApicID
} FCHCLASS_INPUT_BLK;

#pragma pack (pop)
