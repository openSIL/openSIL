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
  .Smbus0BaseAddress = 0xB00,                            // Smbus0BaseAddress
  .SioPmeBaseAddress = 0xE00,                            // SioPmeBaseAddress
  .WatchDogTimerBase = 0xFEC000F0,                       // WatchDogTimerBase (Check this!)
  .AcpiPm1EvtBlkAddr = 0x400,                            // AcpiPm1EvtBlkAddr
  .AcpiPm1CntBlkAddr = 0x404,                            // AcpiPm1CntBlkAddr
  .AcpiPmTmrBlkAddr = 0x408,                             // AcpiPmTmrBlkAddr
  .CpuControlBlkAddr = 0x410,                            // CpuControlBlkAddr
  .AcpiGpe0BlkAddr = 0x420,                              // AcpiGpe0BlkAddr
  .SmiCmdPortAddr = 0x00B0,                              // SmiCmdPortAddr
  .AcpiPmaCntBlkAddr = 0xFE00,                           // AcpiPmaCntBlkAddr
  .SpreadSpectrum = CONFIG_SPREAD_SPECTRUM,              // SpreadSpectrum
  .PwrFailShadow = 0x03,                                 // PwrFailShadow (Check this!)
  .StressResetMode = CONFIG_STRESS_RESET_MODE,           // StressResetMode
  .OemProgrammingTablePtr = NULL,                        // OemProgrammingTablePtr
  .SpreadSpectrumOptions = 1,                            // SpreadSpectrumOptions
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
  .FchModernStandby = false,
  .BootTimerEnable = CONFIG_ENABLE_BOOT_TIMER,
  .BootTimerResetType = CONFIG_BOOT_TIMER_RESET,
  .ToggleAllPwrGoodOnCf9 = CONFIG_TOGGLE_ALL_PWRGOOD,
};
