/**
 * @file  FchSataDef.h
 * @brief FCH SATA controller data structures
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#pragma pack (push, 1)

/// Configuration values for SATA Link Speed
typedef enum {
  Gen1   = 1,                         ///< SATA port GEN1 speed
  Gen2   = 2,                         ///< SATA port GEN2 speed
  Gen3   = 3,                         ///< SATA port GEN3 speed
} SATA_SPEED;

/// Configuration values for SataClass
typedef enum {
  SataNativeIde = 0,                  ///< Native IDE mode
  SataRaid,                           ///< RAID mode
  SataAhci,                           ///< AHCI mode
  SataLegacyIde,                      ///< Legacy IDE mode
  SataIde2Ahci,                       ///< IDE->AHCI mode
  SataAhci7804,                       ///< AHCI mode as 7804 ID (AMD driver)
  SataIde2Ahci7804                    ///< IDE->AHCI mode as 7804 ID (AMD driver)
} SATA_CLASS;

///
/// SATA controller Structure
///
typedef struct {
  bool        SataEnable;                   ///< SATA enable
  bool        SataSetMaxGen2;               ///< SATA enable
  bool        SataMsiEnable;                ///< SATA MSI capability
  uint32_t    SataRaidSsid;                 ///< SATA RAID mode SSID
  uint32_t    SataRaid5Ssid;                ///< SATA RAID 5 mode SSID
  uint32_t    SataAhciSsid;                 ///< SATA AHCI mode SSID
  SATA_CLASS  SataClass;                    ///< SataClass - SATA Controller
                                            ///< mode [2:0]
  uint8_t     SataEspPort;                  ///<  SataEspPort - SATA port is
                                            ///< external accessible on a signal
                                            ///< only connector (eSATA:)
  uint8_t     SataPortPower;                ///<  SataPortPower - Port Power
                                            ///< configuration
  uint16_t    SataPortMd;                   ///<  SataPortMd - Port Mode
  uint8_t     SataAggrLinkPmCap;            /// SataAggrLinkPmCap - 0:OFF   1:ON
  uint8_t     SataPortMultCap;              /// SataPortMultCap - 0:OFF   1:ON
  uint8_t     SataClkAutoOff;               /// SataClkAutoOff - AutoClockOff 0:Disabled, 1:Enabled
  uint8_t     SataPscCap;                   /// SataPscCap 1:Enable PSC, 0:Disable PSC capability
  uint8_t     BiosOsHandOff;                /// BiosOsHandOff - Reserved
  uint8_t     SataFisBasedSwitching;        /// SataFisBasedSwitching - Reserved
  uint8_t     SataCccSupport;               /// SataCccSupport - Reserved
  uint8_t     SataSscCap;                   /// SataSscCap - 1:Enable, 0:Disable SSC capability
  uint8_t     SataTargetSupport8Device;     /// SataTargetSupport8Device - Reserved
  uint8_t     SataDisableGenericMode;       /// SataDisableGenericMode - Reserved
  bool        SataAhciEnclosureManagement;  /// SataAhciEnclosureManagement - Reserved
  uint8_t     SataSgpio0;        /// SataSgpio0 - Reserved
  uint8_t     SataSgpio1;        /// SataSgpio1 - Reserved
  uint8_t     SataPhyPllShutDown;           /// SataPhyPllShutDown - Reserved
  bool        SataOobDetectionEnh;          /// SataOobDetectionEnh - true
  bool        SataRasSupport;               /// SataRasSupport - Support RAS function true: Enable false: Disable
  bool        SataAhciDisPrefetchFunction;  /// SataAhciDisPrefetchFunction - Disable AHCI Prefetch Function Support
  bool        SataDevSlpPort0;              /// SataDevSlpPort0 - Reserved
  bool        SataDevSlpPort1;              /// SataDevSlpPort1 - Reserved
  uint8_t     SataDevSlpPort0Num;           /// SataDevSlpPort0Num - Reserved
  uint8_t     SataDevSlpPort1Num;           /// SataDevSlpPort1Num - Reserved
  uint32_t    TempMmio;                     /// TempMmio - Reserved
  bool        SataControllerAutoShutdown;   /// SataControllerAutoShutdown
  uint32_t    SataDisableMsgParameter;      /// SATA Disable Message parameter
  bool        SataStaggeredSpinupEnable;    ///< Sata Staggered Spin-up feature
  uint8_t     SataBISTLComplianceMode;      ///< SATA BIST-L Compliance Mode
  uint8_t     SataRxPolarity;               ///< SATA RxPolarity
  uint8_t     Reserved[11];
} FCH_SATA2, FCHSATA_INPUT_BLK;

#pragma pack (pop)


