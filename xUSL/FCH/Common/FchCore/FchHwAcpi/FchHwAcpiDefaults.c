/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiDefaults.c
 * @brief FCH ACPI Default Settings
 *
 */

#include <SilCommon.h>
#include <FchHwAcpi-api.h>

FCHHWACPI_INPUT_BLK mFchHwAcpiDefaults = {
  .WatchDogTimerBase = 0xFEC000F0,                       // WatchDogTimerBase (Check this!)
  .SpreadSpectrum = CONFIG_SPREAD_SPECTRUM,              // SpreadSpectrum
  .PwrFailShadow = CONFIG_POWER_FAIL_RETURN_STATE,       // PwrFailShadow
  .StressResetMode = CONFIG_STRESS_RESET_MODE,           // StressResetMode
  .OemProgrammingTablePtr = NULL,                        // OemProgrammingTablePtr
  .NoClearThermalTripSts = CONFIG_CLEAR_THRMTRIP_STATUS, // NoClearThermalTripSts
  .FchAcpiMmioBase = 0xFED80000,                         // FchAcpiMmioBase
  .FchAlinkRasSupport = false,                           // FchAlinkRasSupport
  .I2c0SdaHold = 1,                                      // I2c0SdaHold
  .I2c1SdaHold = 1,                                      // I2c1SdaHold
  .I2c2SdaHold = 1,                                      // I2c2SdaHold
  .I2c3SdaHold = 1,                                      // I2c3SdaHold
  .I2c4SdaHold = 1,                                      // I2c4SdaHold
  .I2c5SdaHold = 1,                                      // I2c5SdaHold
  .FchAoacProgramEnable = true,                          // FchAoacProgramEnable
  .SpdHostCtrlRelease = false,                           // SpdHostCtrlRelease (Check this!)
  .DimmTelemetry = CONFIG_DIMM_TELEMETRY,                // DimmTelemetry
  .BootTimerEnable = CONFIG_ENABLE_BOOT_TIMER,
  .BootTimerResetType = CONFIG_BOOT_TIMER_RESET,
  .ToggleAllPwrGoodOnCf9 = CONFIG_TOGGLE_ALL_PWRGOOD,
  .FchHfpEnable = false,
  .BpX48M0ClockEnable = true,
  .TurnOffXtalS3S5 = false,
  .TurnOffXtalS0i3 = true,
  .Xtal48MPadPowerSaving = false,
  .AcpiShutdownMessage = true,
};
