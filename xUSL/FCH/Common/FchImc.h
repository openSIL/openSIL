/**
 * @file  FchImc.h
 * @brief FCH IMC Data definition
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>

#pragma pack (push, 1)
//
// IMC Message Register Software Interface
//
#define CPU_MISC_BUS_DEV_FUN          ((0x18 << 3) + 3)

#define MSG_SYS_TO_IMC                0x80
#define Fun_80                        0x80
#define Fun_81                        0x81
#define Fun_82                        0x82
#define Fun_83                        0x83
#define Fun_84                        0x84
#define Fun_85                        0x85
#define Fun_86                        0x86
#define Fun_87                        0x87
#define Fun_88                        0x88
#define Fun_89                        0x89
#define Fun_8B                        0x8B
#define Fun_8C                        0x8C
#define Fun_90                        0x90
#define MSG_IMC_TO_SYS                0x81
#define MSG_REG0                      0x82
#define MSG_REG1                      0x83
#define MSG_REG2                      0x84
#define MSG_REG3                      0x85
#define MSG_REG4                      0x86
#define MSG_REG5                      0x87
#define MSG_REG6                      0x88
#define MSG_REG7                      0x89
#define MSG_REG8                      0x8A
#define MSG_REG9                      0x8B
#define MSG_REGA                      0x8C
#define MSG_REGB                      0x8D
#define MSG_REGC                      0x8E
#define MSG_REGD                      0x8F

#define DISABLED                      0
#define ENABLED                       1

///
/// EC structure
///
typedef struct _FCH_EC {
  uint8_t                 MsgFun81Zone0MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun81Zone0MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun81Zone0MsgReg2;           ///<Thermal zone control byte 1
  uint8_t                 MsgFun81Zone0MsgReg3;           ///<Thermal zone control byte 2
  uint8_t                 MsgFun81Zone0MsgReg4;           ///<Bit[3:0] - Thermal diode offset adjustment in degrees Celsius.
  uint8_t                 MsgFun81Zone0MsgReg5;           ///<Hysteresis information
  uint8_t                 MsgFun81Zone0MsgReg6;           ///<SMBUS Address for SMBUS based temperature sensor such as SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone0MsgReg7;           ///<Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based temperature sensor is located.
  uint8_t                 MsgFun81Zone0MsgReg8;           ///<Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone0MsgReg9;           ///<Fan PWM ramping rate in 5ms unit
//
// EC LDN9 function 81 zone 1
//
  uint8_t                 MsgFun81Zone1MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun81Zone1MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun81Zone1MsgReg2;           ///<Thermal zone control byte 1
  uint8_t                 MsgFun81Zone1MsgReg3;           ///<Thermal zone control byte 2
  uint8_t                 MsgFun81Zone1MsgReg4;           ///<Bit[3:0] - Thermal diode offset adjustment in degrees Celsius.
  uint8_t                 MsgFun81Zone1MsgReg5;           ///<Hysteresis information
  uint8_t                 MsgFun81Zone1MsgReg6;           ///<SMBUS Address for SMBUS based temperature sensor such as SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone1MsgReg7;           ///<Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based temperature sensor is located.
  uint8_t                 MsgFun81Zone1MsgReg8;           ///<Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone1MsgReg9;           ///<Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 81 zone 2
//
  uint8_t                 MsgFun81Zone2MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun81Zone2MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun81Zone2MsgReg2;           ///<Thermal zone control byte 1
  uint8_t                 MsgFun81Zone2MsgReg3;           ///<Thermal zone control byte 2
  uint8_t                 MsgFun81Zone2MsgReg4;           ///<Bit[3:0] - Thermal diode offset adjustment in degrees Celsius.
  uint8_t                 MsgFun81Zone2MsgReg5;           ///<Hysteresis information
  uint8_t                 MsgFun81Zone2MsgReg6;           ///<SMBUS Address for SMBUS based temperature sensor such as SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone2MsgReg7;           ///<Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based temperature sensor is located.
  uint8_t                 MsgFun81Zone2MsgReg8;           ///<Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone2MsgReg9;           ///<Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 81 zone 3
//
  uint8_t                 MsgFun81Zone3MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun81Zone3MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun81Zone3MsgReg2;           ///<Thermal zone control byte 1
  uint8_t                 MsgFun81Zone3MsgReg3;           ///<Thermal zone control byte 2
  uint8_t                 MsgFun81Zone3MsgReg4;           ///<Bit[3:0] - Thermal diode offset adjustment in degrees Celsius.
  uint8_t                 MsgFun81Zone3MsgReg5;           ///<Hysteresis information
  uint8_t                 MsgFun81Zone3MsgReg6;           ///<SMBUS Address for SMBUS based temperature sensor such as SB-TSI and ADM1032
  uint8_t                 MsgFun81Zone3MsgReg7;           ///<Bit[1:0]: 0 - 2,  SMBUS bus number where the SMBUS based temperature sensor is located.
  uint8_t                 MsgFun81Zone3MsgReg8;           ///<Fan PWM stepping rate in unit of PWM level percentage
  uint8_t                 MsgFun81Zone3MsgReg9;           ///<Fan PWM ramping rate in 5ms unit
//
//EC LDN9 function 83 zone 0
//
  uint8_t                 MsgFun83Zone0MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun83Zone0MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun83Zone0MsgReg2;           ///<_AC0
  uint8_t                 MsgFun83Zone0MsgReg3;           ///<_AC1
  uint8_t                 MsgFun83Zone0MsgReg4;           ///<_AC2
  uint8_t                 MsgFun83Zone0MsgReg5;           ///<_AC3
  uint8_t                 MsgFun83Zone0MsgReg6;           ///<_AC4
  uint8_t                 MsgFun83Zone0MsgReg7;           ///<_AC5
  uint8_t                 MsgFun83Zone0MsgReg8;           ///<_AC6
  uint8_t                 MsgFun83Zone0MsgReg9;           ///<_AC7
  uint8_t                 MsgFun83Zone0MsgRegA;           ///<_CRT
  uint8_t                 MsgFun83Zone0MsgRegB;           ///<_PSV
//
//EC LDN9 function 83 zone 1
//
  uint8_t                 MsgFun83Zone1MsgReg0;           ///<Thermal zone
  uint8_t                 MsgFun83Zone1MsgReg1;           ///<Thermal zone
  uint8_t                 MsgFun83Zone1MsgReg2;           ///<_AC0
  uint8_t                 MsgFun83Zone1MsgReg3;           ///<_AC1
  uint8_t                 MsgFun83Zone1MsgReg4;           ///<_AC2
  uint8_t                 MsgFun83Zone1MsgReg5;           ///<_AC3
  uint8_t                 MsgFun83Zone1MsgReg6;           ///<_AC4
  uint8_t                 MsgFun83Zone1MsgReg7;           ///<_AC5
  uint8_t                 MsgFun83Zone1MsgReg8;           ///<_AC6
  uint8_t                 MsgFun83Zone1MsgReg9;           ///<_AC7
  uint8_t                 MsgFun83Zone1MsgRegA;           ///<_CRT
  uint8_t                 MsgFun83Zone1MsgRegB;           ///<_PSV
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
  uint8_t                 MsgFun8CZone0MsgReg0;           ///<Reture 0xFA stands for success
  uint8_t                 MsgFun8CZone0MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone0MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone, bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone0MsgReg3;           ///<Startup PWM (effective range 1~100)
//
//EC LDN9 function 8C Startup PWM channel 1
//
  uint8_t                 MsgFun8CZone1MsgReg0;           ///<Reture 0xFA stands for success
  uint8_t                 MsgFun8CZone1MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone1MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone, bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone1MsgReg3;           ///<Startup PWM (effective range 1~100)
//uint8_t
//EC LDN9 function 8C Startup PWM channel 2
//
  uint8_t                 MsgFun8CZone2MsgReg0;           ///<Reture 0xFA stands for success
  uint8_t                 MsgFun8CZone2MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone2MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone, bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone2MsgReg3;           ///<Startup PWM (effective range 1~100)
//
//EC LDN9 function 8C Startup PWM channel 3
//
  uint8_t                 MsgFun8CZone3MsgReg0;           ///<Reture 0xFA stands for success
  uint8_t                 MsgFun8CZone3MsgReg1;           ///<Bit 2-0 Thermal zone number
  uint8_t                 MsgFun8CZone3MsgReg2;           ///<Startup PWM flags; bit0: enable/disable current zone, bit1/2/3: 1 if values in reg3/4/5 are valid.
  uint8_t                 MsgFun8CZone3MsgReg3;           ///<Startup PWM (effective range 1~100)
//
// FLAG for Fun83/85/89/8C support
//
  uint32_t                IMCFUNSupportBitMap;            ///< Bit0=81FunZone0 support(1=On;0=Off); bit1-3=81FunZone1-Zone3;Bit4-7=83FunZone0-Zone3;Bit8-11=85FunZone0-Zone3;Bit11-15=89FunZone0-Zone3;
} FCH_EC;

///
/// IMC structure
///
typedef struct _FCH_IMC {
  uint8_t                 ImcEnable;                      ///< ImcEnable - IMC Enable
  uint8_t                 ImcEnabled;                     ///< ImcEnabled - IMC Enable
  uint8_t                 ImcSureBootTimer;               ///< ImcSureBootTimer - IMc SureBootTimer function
  FCH_EC                EcStruct;                       ///< EC structure
  uint8_t                 ImcEnableOverWrite;             ///< OverWrite IMC with the EC structure
                                                        ///   @li <b>00</b> - by default strapping
                                                        ///   @li <b>01</b> - enable
                                                        ///   @li <b>10</b> - disable
                                                        ///
} FCH_IMC;

#pragma pack (pop)


