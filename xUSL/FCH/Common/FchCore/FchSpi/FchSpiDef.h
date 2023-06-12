/**
 * @file  FchSpiDef.h
 * @brief FCH SPI controller difinitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#pragma pack (push, 1)

///
/// _SPI_DEVICE_PROFILE Spi Device Profile structure
///
typedef struct _SPI_DEVICE_PROFILE {
  uint32_t                JEDEC_ID;                       /// JEDEC ID
  uint32_t                RomSize;                        /// ROM Size
  uint32_t                SectorSize;                     /// Sector Size
  uint16_t                MaxNormalSpeed;                 /// Max Normal Speed
  uint16_t                MaxFastSpeed;                   /// Max Fast Speed
  uint16_t                MaxDualSpeed;                   /// Max Dual Speed
  uint16_t                MaxQuadSpeed;                   /// Max Quad Speed
  uint8_t                 QeReadRegister;                 /// QE Read Register
  uint8_t                 QeWriteRegister;                /// QE Write Register
  uint8_t                 QeOperateSize;                  /// QE Operate Size 1byte/2bytes
  uint16_t                QeLocation;                     // QE Location in the register
} SPI_DEVICE_PROFILE;

///
/// SPI structure
///
typedef struct {
  bool                    LpcEnable;                      ///< LPC enable
  bool                    LpcMsiEnable;                   ///< LPC MSI capability
  uint32_t                LpcSsid;                        ///< LPC Subsystem ID
  uint32_t                RomBaseAddress;                 ///< SpiRomBaseAddress
                                                        ///   @par
                                                        ///   SPI ROM BASE Address
                                                        ///
  uint8_t                 SpiSpeed;                       ///< SpiSpeed - Spi Bus normal speed
                                                        ///  @par
                                                        ///  SPI Speed - the clock speed for non-fast read command
                                                        ///   @li <b>0000</b> - keep HW default
                                                        ///   @li <b>0001</b> - 66Mhz
                                                        ///   @li <b>0010</b> - 33Mhz
                                                        ///   @li <b>0011</b> - 22Mhz
                                                        ///   @li <b>0100</b> - 16.5Mhz
                                                        ///   @li <b>0101</b> - 100khz
                                                        ///   @li <b>0110</b> - 800khz
                                                        ///   @li <b>0111</b> - 50Mhz
                                                        ///   @li <b>1000</b> - 4Mhz
                                                        ///
  uint8_t                 SpiFastSpeed;                 ///< FastSpeed  - Obsolete!!! Spi fast speed is set by PSP
  uint8_t                 WriteSpeed;                   ///< WriteSpeed - Spi Write Speed
                                                        /// @par
                                                        ///  WriteSpeed - the clock speed for Spi write command
                                                        ///   @li <b>0000</b> - keep HW default
                                                        ///   @li <b>0001</b> - 66Mhz
                                                        ///   @li <b>0010</b> - 33Mhz
                                                        ///   @li <b>0011</b> - 22Mhz
                                                        ///   @li <b>0100</b> - 16.5Mhz
                                                        ///   @li <b>0101</b> - 100khz
                                                        ///   @li <b>0110</b> - 800khz
                                                        ///   @li <b>0111</b> - 50Mhz
                                                        ///   @li <b>1000</b> - 4Mhz
                                                        ///
  uint8_t                 SpiTpmSpeed;                  ///< SpiTpmSpeed - Spi Bus Speed for TPM read and write
                                                        /// @par
                                                        ///  SpiTpmSpeed - the clock speed for Spi TPM command
                                                        ///   @li <b>0000</b> - keep HW default
                                                        ///   @li <b>0001</b> - 66Mhz
                                                        ///   @li <b>0010</b> - 33Mhz
                                                        ///   @li <b>0011</b> - 22Mhz
                                                        ///   @li <b>0100</b> - 16.5Mhz
                                                        ///   @li <b>0101</b> - 100khz
                                                        ///   @li <b>0110</b> - 800khz
                                                        ///   @li <b>0111</b> - 50Mhz
                                                        ///   @li <b>1000</b> - 4Mhz
                                                        ///
  uint8_t                 SpiBurstWrite;                  ///< SpiBurstWrite - Spi Burst Write Mode
                                                        /// @par
                                                        ///  SPI Burst Write
                                                        ///  @li <b>0</b> - Disabled
                                                        ///  @li <b>1</b> - Enabled
  bool                    LpcClk0;                        ///< Lclk0En - LPCCLK0
                                                        /// @par
                                                        ///  LPC Clock 0 mode
                                                        ///  @li <b>0</b> - forced to stop
                                                        ///  @li <b>1</b> - functioning with CLKRUN protocol
  bool                    LpcClk1;                        ///< Lclk1En - LPCCLK1
                                                        /// @par
                                                        ///  LPC Clock 1 mode
                                                        ///  @li <b>0</b> - forced to stop
                                                        ///  @li <b>1</b> - functioning with CLKRUN protocol
} FCH_SPI, FCHSPI_INPUT_BLK;

#pragma pack (pop)


