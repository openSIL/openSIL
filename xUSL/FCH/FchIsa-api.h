/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsa-api.h
 * @brief FCH ISA Host data (input block) definitions
 *
 * @details The FCH ISA sub-component is the bridge to legacy buses, such as:
 *   - Serial-Peripheral-Interface (SPI) bus,
 *   - Low-Pin-Count (LPC) bus.
 * Devices connected to these buses are typically the Flash ROM(s), or TPM.
 */

#pragma once

#pragma pack (push, 1)  // match the packing of it's origin file

///
/// ISA structures
///
typedef  struct  {

  bool      LpcEnable;        ///< LPC enable
  bool      LpcClk0;          ///< Lclk0En - LPCCLK0
                              ///< LPC Clock 0 mode
                              ///< @li <b>0</b> - forced to stop
                              ///< @li <b>1</b> - functioning with CLKRUN protocol
  bool      LpcClk1;          ///< Lclk1En - LPCCLK1
                              ///< LPC Clock 1 mode
                              ///< @li <b>0</b> - forced to stop
                              ///< @li <b>1</b> - functioning with CLKRUN protocol
  bool      LpcMsiEnable;     ///< LPC MSI capability
  uint32_t  LpcSsid;          ///< LPC Subsystem ID
} FCH_LPC;


typedef  struct  {
  uint8_t   SpiSpeed;         ///< SpiSpeed - Spi Bus normal speed
                              ///< SPI Speed - the clock speed for non-fast read command
                              ///<  @li <b>0000</b> - keep HW default
                              ///<  @li <b>0001</b> - 66Mhz
                              ///<  @li <b>0010</b> - 33Mhz
                              ///<  @li <b>0011</b> - 22Mhz
                              ///<  @li <b>0100</b> - 16.5Mhz
                              ///<  @li <b>0101</b> - 100Khz
                              ///<  @li <b>0110</b> - 800Khz
                              ///<  @li <b>0111</b> - 50Mhz
                              ///<  @li <b>1000</b> - 4Mhz
  //
  uint8_t   WriteSpeed;       ///< WriteSpeed - Spi Write Speed
                              ///< WriteSpeed - the clock speed for Spi write command
                              ///<  @li <b>0000</b> - keep HW default
                              ///<  @li <b>0001</b> - 66Mhz
                              ///<  @li <b>0010</b> - 33Mhz
                              ///<  @li <b>0011</b> - 22Mhz
                              ///<  @li <b>0100</b> - 16.5Mhz
                              ///<  @li <b>0101</b> - 100Khz
                              ///<  @li <b>0110</b> - 800Khz
                              ///<  @li <b>0111</b> - 50Mhz
                              ///<  @li <b>1000</b> - 4Mhz
  //
  uint8_t   SpiTpmSpeed;      ///< SpiTpmSpeed - Spi Bus Speed for TPM read and write
                              ///< SpiTpmSpeed - the clock speed for Spi TPM command
                              ///<  @li <b>0000</b> - keep HW default
                              ///<  @li <b>0001</b> - 66Mhz
                              ///<  @li <b>0010</b> - 33Mhz
                              ///<  @li <b>0011</b> - 22Mhz
                              ///<  @li <b>0100</b> - 16.5Mhz
                              ///<  @li <b>0101</b> - 100Khz
                              ///<  @li <b>0110</b> - 800Khz
                              ///<  @li <b>0111</b> - 50Mhz
                              ///<  @li <b>1000</b> - 4Mhz
  //
  uint8_t   FastSpeed;        ///< FastSpeed - Spi Bus fast speed
                              ///< SPI Speed - the clock speed for fast read command
                              ///<  @li <b>0000</b> - keep HW default
                              ///<  @li <b>0001</b> - 66Mhz
                              ///<  @li <b>0010</b> - 33Mhz
                              ///<  @li <b>0011</b> - 22Mhz
                              ///<  @li <b>0100</b> - 16.5Mhz
                              ///<  @li <b>0101</b> - 100Mhz
                              ///<  @li <b>0110</b> - 800Khz
  //
  bool    Spi100Enable;
  bool    BurstWrite;
} FCH_SPI;


typedef struct {
  FCH_LPC   LpcConfig;
  FCH_SPI   SpiConfig;
} FCHISA_INPUT_BLK;

#pragma pack (pop)
