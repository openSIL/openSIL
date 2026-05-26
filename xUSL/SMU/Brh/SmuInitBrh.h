/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuInitBrh.h
 * @brief OpenSIL SMU services specific to BRH
 *
 */

#pragma once

SIL_STATUS InitializeSmuBrh (void);

#define SIL_RESERVED_1761             0xD
#define SIL_RESERVED_1762           0xF
#define SIL_RESERVED_1760                 0x50

#define SIL_RESERVED_ADDR_0x3810A84    0x3810A84
#define SIL_RESERVED_ADDR_0x3810A88    0x3810A88

#define SIL_SMU_RESERVED_0x3           0x3
#define SIL_SMU_RESERVED_0x5           0x5
#define SIL_SMU_RESERVED_0x6           0x6
#define SIL_SMU_RESERVED_0x8           0x8
#define SIL_SMU_RESERVED_0xC           0xC
#define SIL_SMU_RESERVED_0x10          0x10
#define SIL_SMU_RESERVED_0x2B          0x2B
#define SIL_SMU_RESERVED_0x2F          0x2F
#define SIL_SMU_RESERVED_0x34          0x34
#define SIL_SMU_RESERVED_0x37          0x37
#define SIL_SMU_RESERVED_0x41          0x41
#define SIL_SMU_RESERVED_0x42          0x42
#define SIL_SMU_RESERVED_0x4E          0x4E
#define SIL_SMU_RESERVED_0x54          0x54

typedef struct {
  //DEFAULT INFRASTRUCTURE LIMITS
  uint32_t TDP;   //Power [W]
  uint32_t PPT;   //Power [W]
  uint32_t TDC;   //Current [A]
  uint32_t EDC;   //Current [A]
  uint32_t TjMax; //Degrees [C]

  //PLATFORM INFRASTRUCTURE LIMITS
  uint32_t TDP_PlatformLimit; //Power [W]
  uint32_t PPT_PlatformLimit; //Power [W]
  uint32_t TDC_PlatformLimit; //Current [A]
  uint32_t EDC_PlatformLimit; //Current [A]

  //CBS debug options
  int32_t CoreDldoPsmMargin;     //[PSM count] [signed]
  int32_t VddcrCpuVoltageMargin; //[mV] [signed]
  int32_t VddcrSocVoltageMargin; //[mV] [signed]
  int32_t VddioVoltageMargin;    //[mV] [signed]
  uint8_t CC1Dis;                //[0 = CC1 enabled; 1 = CC1 disabled]
  uint8_t DeterminismControl;    //[0 = use fused default; 1 = disable performance determinism; 2 = enable performance determinism]
  uint8_t CCX_VdciAsync;         //[0 = predictive mode; 1 = async mode]
  uint8_t APBDIS;                //[0 = not APBDIS; 1 = APBDIS]
  uint8_t Policy;                //[0 = High Performance Mode (DEFAULT); 1 = Efficiency Mode; 2 = Maximum IO Performance Mode]
  uint8_t PcieSpeedControl;      //[0 = Enable PCIe speed controller; 1 = Limit to GEN4; 2 = Limit to GEN5]
  uint8_t ThrottlerMode;         //[0 = link per plane throttlers; 1 = unlink per plane throttlers]
  uint8_t spare;

  //DF CSTATE CONFIG
  uint8_t DfCstateConfigOverride;
  uint8_t DfCstateClkPwrDnEn;
  uint8_t DfCstateSelfRefrEn;
  uint8_t DfCstateGmiPwrDnEn;
  uint8_t DfCstateGopPwrDnEn;
  uint8_t DfCstateSpare[3];

  //xGMI CONFIGURATION
  uint8_t xGMIMaxLinkWidthEn;
  uint8_t xGMIMaxLinkWidth;
  uint8_t xGMIForceLinkWidthEn;
  uint8_t xGMIForceLinkWidth;

  //TELEMETRY
  uint8_t  TelemetryCurrentGuardband; //[0.125% encoding] [unsigned] [EX: 30 --> 3.75%]
  uint8_t  Svi3SvcSpeed; //[0=50MHz, 1=40MHz, 2=26.67MHz, 3=20MHz, 4=16MHz, 5=13.33MHz, 6=10MHz, 7=8MHz, 8=5MHz]
  uint16_t PccLimit;      //[Amps] [unsigned] [0=use default platform PCC limit]
  uint8_t  I3cPpHcnt;     // I3C SCL push-pull high count
  uint8_t  I3cSpeed;      // I3C SDR Speed
  uint8_t  I3cSdaHold[4]; // I3C bus data hold time

  //DRAM PPR setting
  uint8_t  PprConfigInitiator; // DRAM Post Package Repair Configuration Initiator [ 0 = In-Band, 1 = Out Of Band]
  uint8_t  spare5; 

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  uint8_t  OC_DISABLE; //1=disable overclocking; 0=allow overclocking
  uint8_t  OC_MAXVID;  //[VID] SVI3 VID encoding
  uint16_t OC_FREQMAX; //MHz

  //CCLK FREQUENCY FORCE
  uint16_t ForceCclkFrequency;   //[MHz; 0 means no force]
  uint16_t FmaxOverride;         //[MHz; 0 means no override]
  uint8_t  APBDIS_DfPstate;      //[DfPstate index to set when APBDIS=1]
  uint8_t  DFFO_Disable;         //[1 means disable the DFPstate CCLK effective frequency optimizer]
  uint16_t ForceVddcrCpuVoltage; //[mV; 0 means no force]
  uint16_t ForceVddcrSocVoltage; //[mV; 0 means no force]
  uint16_t ForceVddioVoltage;    //[mV; 0 means no force]

  uint8_t  DfPstateRangeSupportEn;
  uint8_t  DfPstateRangeMin;
  uint8_t  DfPstateRangeMax;
  uint8_t  DfPstateRangeSpare;
  uint8_t  XgmiPstateRangeSupportEn;
  uint8_t  XgmiPstateRangeMin;
  uint8_t  XgmiPstateRangeMax;
  uint8_t  XgmiPstateRangeSpare;

  uint8_t  xGMIMinLinkWidth;
  uint8_t  xGMILinkWidthSpare[3];

  uint8_t  CclkMode;
  uint8_t  AdjustGB;
  uint8_t  OneCppcMax;

  uint8_t  spare4[29];
} PPTable_t;

void
SmuFixupPlatformConfig (
  uint32_t                   PackageType,
  PPTable_t                  *PPTable,
  SMUCLASS_INPUT_BLK         *SmuInputBlock
  );
