/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchHwAcpi-api.h
 * @brief openSIL-Host FCH-ACPI IP interface
 *
 * @details The FCH is a bridge device under which many of the legacy and
 * traditional components of a PC are located.
 *
 *  This file provides the structures details for the Host to configure the
 *  ACPI and other miscellaneous devices' operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_FCH_ACPI  FCH (FCH) ACPI api
 *
 *   The FCH-ACPI sub-component is the portion of the FCH that contains
 *   most of the legacy devices, including the ACPI OS interface
 *   registers.
 *
 *   See the 'Files - @ref FchAcpi-api.h' section of this document for
 *   further details.
 *
 *   Blocks comprising this module are:
 *   - @ subpage IP_LPC "LPC bus"
 *   - @ subpage IP_SMBUS "SMbus"
 *   - @ subpage IP_I2C "I^2 C bus"
 *
 * @endcond
 */

#pragma once

#pragma pack (push, 1)  // match the packing of it's origin file

/** @brief Configuration values for FchPowerFail
 *  @details This is a configuration option to select to what state
 *  should the system be set/returned when a power failure is detected
 */
typedef enum {
  AlwaysOff = 0,      ///< Always power off after power resumes
  AlwaysOn = 1,       ///< Always power on after power resumes
  UsePrevious = 3,    ///< Resume to previous state when power fails
} POWER_FAIL;

///
/// Acpi structure
///
typedef struct {
  uint16_t           Smbus0BaseAddress;     // Smbus BASE Address
  uint16_t           SioPmeBaseAddress;     // SIO PME BASE Address
  uint32_t           WatchDogTimerBase;     // Watch Dog Timer Address
  uint16_t           AcpiPm1EvtBlkAddr;     // ACPI PM1 event block Address
  uint16_t           AcpiPm1CntBlkAddr;     // ACPI PM1 Control block Address
  uint16_t           AcpiPmTmrBlkAddr;      // ACPI PM timer block Address
  uint16_t           CpuControlBlkAddr;     // ACPI CPU control block Address
  uint16_t           AcpiGpe0BlkAddr;       // ACPI GPE0 block Address
  uint16_t           SmiCmdPortAddr;        // SMI command port Address
  uint16_t           AcpiPmaCntBlkAddr;     // ACPI PMA Control block Address
  bool               SpreadSpectrum;        // Spread Spectrum function; 0-disable; 1-enable
  uint8_t            SpreadSpectrumOptions; // Spread Spectrum Option
  POWER_FAIL         PwrFailShadow;         // PwrFailShadow = PM_Reg: 5Bh [3:0]
                                            // <b>00</b> - Always off
                                            // <b>01</b> - Always on
                                            // <b>11</b> - Use previous
  uint8_t            StressResetMode;       // StressResetMode 01-10
                                            // <b>00</b> - Disabled
                                            // <b>01</b> - Io Write 0x64 with 0xfe
                                            // <b>10</b> - Io Write 0xcf9 with 0x06
                                            // <b>11</b> - Io Write 0xcf9 with 0x0e
  bool               NoClearThermalTripSts; // Skip clearing ThermalTrip status
  uint32_t           FchAcpiMmioBase;       // FCH ACPI MMIO Base
  bool               FchAlinkRasSupport;    // FCH A-Link parity error support
  uint32_t           I2c0SdaHold;           // I2C0 SDA_HOLD
  uint32_t           I2c1SdaHold;           // I2C1 SDA_HOLD
  uint32_t           I2c2SdaHold;           // I2C2 SDA_HOLD
  uint32_t           I2c3SdaHold;           // I2C3 SDA_HOLD
  uint32_t           I2c4SdaHold;           // I2C4 SDA_HOLD
  uint32_t           I2c5SdaHold;           // I2C5 SDA_HOLD
  bool               FchAoacProgramEnable;  // Enable/disable AOAC init programming
  bool               SpdHostCtrlRelease;    // Release SPD Host Ctrl
  bool               DimmTelemetry;         // Send message to PMFW to start DIMM telemetry
  bool               FchModernStandby;      // Enable Modern Standby feature
  bool               BootTimerEnable;       // Enable FCH Boot Timer
  bool               BootTimerResetType;    // Boot Timer Reset Type
  bool               ToggleAllPwrGoodOnCf9; // Enable Toggle all PowerGood on Cf9 feature
  uint32_t           Reserved[16];          // Reserved
  void *OemProgrammingTablePtr;             // Pointer of ACPI OEM table
} FCHHWACPI_INPUT_BLK;

#pragma pack (pop)
