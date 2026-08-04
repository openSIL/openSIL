/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuClass-api.h
 * @brief openSIL-Host SMU IP interface
 *
 * @details The SMU API provides some controls for SMU initialization.
 *
 *  This file provides the structures details for the Host to configure the
 *  SMU operations.
 */

/*
 * Note: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SMUCLASS_MAJOR_REV   0
#define SMUCLASS_MINOR_REV   1
#define SMUCLASS_INSTANCE    0

/**
 * @cond FullCode_Doc
 * @ingroup SMU_group
 * @endcond
 *
 * Definition for the SMU module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the openSIL SMU IP module.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP.
 *
 */
typedef struct {
  uint32_t  SmuFeatureControl;
  uint32_t  SmuFeatureControlExt;
  uint32_t  SmuFeatureControl64;
  uint8_t    CfgSystemConfiguration;
  // PP table settings
  uint32_t  AmdcTDP;
  uint32_t  CfgPPT;
  uint32_t  CfgPlatformTDP;
  uint32_t  CfgPlatformPPT;
  uint32_t  CfgPlatformTDC;
  uint32_t  CfgPlatformEDC;
  uint8_t   CfgApbDis;
  uint32_t  SustainedPowerLimit;
  uint32_t  VrmSocCurrentLimit;
  uint32_t  VrmMaximumCurrentLimit;
  uint32_t  VrmSocMaximumCurrentLimit;
  uint32_t  ProchotlDeassertionRampTime;
  uint8_t   DldoPsmMargin;
  uint32_t  VddcrCpuVoltageMargin;
  uint32_t  VddcrSocVoltageMargin;
  uint32_t  Telemetry_VddcrVddfull_Scale_Current;
  uint32_t  Telemetry_VddcrVddOffset;
  uint32_t  Telemetry_VddcrSocfull_Scale_Current;
  uint32_t  Telemetry_VddcrSocOffset;
  uint32_t  Telemetry_VddcrSrfull_Scale_Current;
  uint32_t  Telemetry_VddcrSrOffset;
  uint8_t   OcDisable;
  uint16_t  OcVoltageMax;
  uint16_t  OcFrequencyMax;
  uint16_t  ForceGfxclkFrequency;
  uint16_t  ForceVddcrSocVoltage;
  uint16_t  ForceVddcrCpuVoltage;
  uint16_t  GfxclkFmaxOverride;
  uint16_t  CclkFminOverride;
  uint8_t   SttEnable;
  uint8_t   SttPcbSensorCount;
  uint16_t  SttMinPowerLimit;
  uint16_t  SttM1;
  uint16_t  SttM2;
  uint16_t  SttM3;
  uint16_t  SttM4;
  uint16_t  SttM5;
  uint16_t  SttM6;
  uint16_t  SttCApu;
  uint16_t  SttCGpu;
  uint16_t  SttAlphaApu;
  uint16_t  SttAlphaGpu;
  uint16_t  SttSkinTemperatureLimitApu;
  uint16_t  SttSkinTemperatureLimitGpu;
  uint16_t  SttErrorCoeff;
  uint16_t  SttErrorRateCoeff;
  uint8_t   StapmBoost;
  uint32_t  StapmTimeConstant;
  uint8_t   CppcConstraintsEnabled;
  uint8_t   CppcPerfLimitMaxRange;
  uint8_t   CppcPerfLimitMinRange;
  uint8_t   CppcEppMaxRange;
  uint8_t   CppcEppMinRange;
  uint8_t   ForceFanPwmEn;
  uint8_t   ForceFanPwm;
  uint32_t  FastPptLimit;
  uint32_t  SlowPptLimit;
  uint32_t  SlowPptTimeConstant;
  uint32_t  CfgThermCtlValue;
  uint32_t  VrmCurrentLimit;
  bool      GfxOcDisable;
  uint8_t   OcGFXMinVID;
  uint16_t  OcGFXFreqMax;
  uint16_t  GfxclkFminOverride;
  uint8_t   StbSmuVerbosityControl;
  uint8_t   SwSciGpeID;
  uint8_t   CoreCountControlEnable;
  uint8_t   P3TLimitCtrl;
  uint32_t  P3TLimit;
  uint32_t  CfgLclkFrequencyRange;
  uint16_t  VddMiscVoltage;
  uint8_t   AmdSmuVddcrVddSlewRateIndex;
  uint8_t   AmdSmuVddcrVddSlewRateDownCntl;
  uint8_t   CfgPeApmEnable;
  uint8_t   AmdSmuPsiDecayConditionOverride;
  uint8_t   AmdSmuPsiDecayConditionVddRail;
  uint8_t   AmdSmuPsiDecayConditionSocRail;
  uint8_t   AmdSmuPsiDecayConditionSrRail;
  uint8_t   AmdSmuVddcrSocSlewRateOverride;
  uint8_t   AmdSmuVddcrSocSlewRateIndex;
  uint8_t   AmdSmuVddcrSocSlewRateDownCntl;
  uint8_t   AmdSmuZ10SlewRateOverride;
  uint8_t   AmdSmuZ10UpSlewRateVdd;
  uint8_t   AmdSmuZ10UpSlewRateSoc;
  uint8_t   AmdSmuZ10UpSlewRateSr;
  uint8_t   AmdSmuZ10DownSlewRateVdd;
  uint8_t   AmdSmuZ10DownSlewRateSoc;
  uint8_t   AmdSmuZ10DownSlewRateSr;
  bool      DisplayIdleOptimizationEn;
  uint64_t  CurveShaperPsmArray;
  uint8_t   AmdSmuSbTsiSlaveAddrOverride;
  uint8_t   AmdSmuSbTsiSlaveAddrSelect;
  uint8_t   VRHOTEnable;
  // Early init params
  uint8_t   AmdPowerSupplyIdleControl;
  uint32_t  Svi3TableAddress;
  uint8_t   AmdFabricCstate;
  bool      DcBtc;
  uint8_t   DcBtcVid;
  uint8_t   DcBtcErrorOfsetVoltageMargin;
  bool      SocDcBtcEnable;
  bool      FllBtcEnable;
  bool      UlvVidCtrl;
  uint32_t  UlvVidOffset;
  bool      VddOffVidCtrl;
  uint32_t  VddOffVid;
  uint32_t  SlowPPTLimitApuOnly;
  uint32_t  MsgSetSustainedPowerLimit;
  uint32_t  MsgSetFastPPTLimit;
  uint32_t  MsgSetSlowPPTLimit;
  uint32_t  MsgStapmTimeConstant;
  uint32_t  MocPPTLimit;
  uint32_t  MocTDCLimit;
  uint32_t  MocSocTDCLimit;
  uint32_t  MocEDCLimit;
  uint32_t  MocSocEDCLimit;
  uint32_t  MocPBOLimitScalar;
  uint32_t  MocTjMax;
  uint64_t  CfgCoreDldoPsmArray;
  uint32_t  SetS0i3PmeTurnOffDelay;
  bool      SmuGfxPsmMarginControl;
  uint16_t  SmuGfxPsmMarginValue;
  uint8_t   MocGfxCoreVid;
  uint16_t  MocGfxClockFrequency;
  uint32_t  AmdSmuGpioConfigTable;
  bool      AmdSmuZStateControlEnable;
  bool      S0i3Enable;
  bool      AmdSmuHspClkDsEn;
  bool      AmdSmuFclkDpmEn;
  bool      EnableStapm;
  uint8_t   CpuBoostClockOverride;
  uint32_t  FMaxFrequency;
  uint8_t   GpuBoostClockOverride;
  uint32_t  GFXFMaxFrequency;
  bool      AmdSmuGfxDpmEn;
  bool      PublicFanControlEnable;
  bool      AmdZstatesMaskEnable;
  bool      AmdZstatesWake0Timer;
  bool      AmdZstatesWake0S2nsTimer;
  bool      AmdZstatesWake0Fch;
  bool      AmdZstatesWake0Acp;
  bool      AmdZstatesWake0Mp2;
  bool      AmdZstatesWake0Dbreq;
  bool      AmdZstatesWake0DcnS;
  bool      AmdZstatesWake0DcnNs;
  bool      AmdZstatesWake0Df;
} SMUCLASS_INPUT_BLK;
