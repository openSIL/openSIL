/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  FchCommonCfg.h
 * @brief Fch Common Data definition
 */

#pragma once

#pragma pack (push, 1)

#include "FchImc.h"
#include "FchPT.h"
#include <FCH/Common/FchCore/FchSpi/FchSpiDef.h>
#include <FCH/Common/FchCore/FchGpp/FchGppDef.h>
#include "FchCommon.h"

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */

/// PCI_ADDRESS - PCI access structure
#define PCI_ADDRESS(bus, dev, func, reg) \
          (uint32_t) ( (((uint32_t)bus) << 24) + (((uint32_t)dev) << 19) + (((uint32_t)func) << 16) + ((uint32_t)reg) )

#define PCIE_CAP_ID      0x10

/**********************************************************************************************************************
 * @brief Declare common variables here
 *
 */

///
/// Fch Run Time Parameters
///
typedef struct {
  uint32_t                PcieMmioBase;                            ///< PcieMmioBase
  uint32_t                FchDeviceEnableMap;                      ///< FchDeviceEnableMap
                                                                   ///< Indicate FCH devices map
                                                                   ///< BIT4 - LPC : LpcEnable
                                                                   ///< BIT5 - I2C0 : FchRTDeviceEnableMap[BIT5]
                                                                   ///< BIT6 - I2C1 : FchRTDeviceEnableMap[BIT6]
                                                                   ///< BIT7 - I2C2 : FchRTDeviceEnableMap[BIT7]
                                                                   ///< BIT8 - I2C3 : FchRTDeviceEnableMap[BIT8]
                                                                   ///< BIT9 - I2C4 : FchRTDeviceEnableMap[BIT9]
                                                                   ///< BIT10 - I2C5 : FchRTDeviceEnableMap[BIT10]
                                                                   ///< BIT11 - UART0 : FchRTDeviceEnableMap[BIT11]
                                                                   ///< BIT12 - UART1 : FchRTDeviceEnableMap[BIT12]
                                                                   ///< BIT16 - UART2 : FchRTDeviceEnableMap[BIT13]
                                                                   ///< BIT18 - SD : EmmcEnable and EmmcType < 5
                                                                   ///< BIT26 - UART3 : FchRTDeviceEnableMap[BIT26]
                                                                   ///< BIT27 - eSPI : EspiEnable
                                                                   ///< BIT28 - eMMC : EmmcEnable
  uint32_t                FchDeviceD3ColdMap;                      ///< FchDeviceD3ColdMap
  uint16_t                Al2AhbLegacyUartIoEnable;                ///< Al2Ahb Legacy Uart Io Enable
  uint8_t                 Uart0Irq;                                ///< Uart 0 Irq
  uint8_t                 Uart1Irq;                                ///< Uart 1 Irq
  uint8_t                 Uart2Irq;                                ///< Uart 2 Irq
  uint8_t                 Uart3Irq;                                ///< Uart 3 Irq
  uint8_t                 I2c0Irq;                                 ///< I2c 0 Irq
  uint8_t                 I2c1Irq;                                 ///< I2c 1 Irq
  uint8_t                 I2c2Irq;                                 ///< I2c 2 Irq
  uint8_t                 I2c3Irq;                                 ///< I2c 3 Irq
  uint8_t                 I2c4Irq;                                 ///< I2c 4 Irq
  uint8_t                 I2c5Irq;                                 ///< I2c 5 Irq
} FCH_RUNTIME;

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
/// SATA main setting structure
///
typedef struct {
  uint8_t            SataModeReg;                       ///< SataModeReg - Sata Controller Mode
  bool               SataEnable;                        ///< SataEnable - Sata Controller Function
                                                        ///   @par
                                                        /// Sata Controller
                                                        ///   @li <b>0</b> - disable
                                                        ///   @li <b>1</b> - enable
                                                        ///
  uint8_t            Sata6AhciCap;                      ///< Sata6AhciCap - Reserved */
  bool               SataSetMaxGen2;                    ///< SataSetMaxGen2 - Set Sata Max Gen2 mode
                                                        ///   @par
                                                        /// Sata Controller Set to Max Gen2 mode
                                                        ///   @li <b>0</b> - disable
                                                        ///   @li <b>1</b> - enable
                                                        ///
  bool               IdeEnable;                         ///< IdeEnable - Reserved
                                                        ///
  uint8_t            SataClkMode;                       /// SataClkMode - Reserved
} SATA_ST;

///
/// SATA_PORT_ST - SATA PORT structure
///
typedef struct {
  uint8_t                 SataPortReg;                  ///< SATA Port bit map - bits[0:7] for ports 0 ~ 7
                                                        ///  @li <b>0</b> - disable
                                                        ///  @li <b>1</b> - enable
                                                        ///
  bool               Port0;                             ///< PORT0 - 0:disable, 1:enable
  bool               Port1;                             ///< PORT1 - 0:disable, 1:enable
  bool               Port2;                             ///< PORT2 - 0:disable, 1:enable
  bool               Port3;                             ///< PORT3 - 0:disable, 1:enable
  bool               Port4;                             ///< PORT4 - 0:disable, 1:enable
  bool               Port5;                             ///< PORT5 - 0:disable, 1:enable
  bool               Port6;                             ///< PORT6 - 0:disable, 1:enable
  bool               Port7;                             ///< PORT7 - 0:disable, 1:enable
} SATA_PORT_ST;

///
///< _SATA_PORT_MD - Force Each PORT to GEN1/GEN2 mode
///
typedef struct {
  uint16_t                SataPortMode;          ///< SATA Port GEN1/GEN2 mode bit map - bits [0:15] for ports 0 ~ 7
  uint8_t                 Port0;                 ///< PORT0 - set BIT0 to GEN1, BIT1 - PORT0 set to GEN2
  uint8_t                 Port1;                 ///< PORT1 - set BIT2 to GEN1, BIT3 - PORT1 set to GEN2
  uint8_t                 Port2;                 ///< PORT2 - set BIT4 to GEN1, BIT5 - PORT2 set to GEN2
  uint8_t                 Port3;                 ///< PORT3 - set BIT6 to GEN1, BIT7 - PORT3 set to GEN2
  uint8_t                 Port4;                 ///< PORT4 - set BIT8 to GEN1, BIT9 - PORT4 set to GEN2
  uint8_t                 Port5;                 ///< PORT5 - set BIT10 to GEN1, BIT11 - PORT5 set to GEN2
  uint8_t                 Port6;                 ///< PORT6 - set BIT12 to GEN1, BIT13 - PORT6 set to GEN2
  uint8_t                 Port7;                 ///< PORT7 - set BIT14 to GEN1, BIT15 - PORT7 set to GEN2
} SATA_PORT_MD;

///
/// SATA structure
///
typedef struct {
  bool                    SataMsiEnable;                ///< SATA MSI capability
  uint32_t                SataIdeSsid;                  ///< SATA IDE mode SSID
  uint32_t                SataRaidSsid;                 ///< SATA RAID mode SSID
  uint32_t                SataRaid5Ssid;                ///< SATA RAID 5 mode SSID
  uint32_t                SataAhciSsid;                 ///< SATA AHCI mode SSID

  SATA_ST                 SataMode;                     /// SataMode - Reserved
  SATA_CLASS              SataClass;                    ///< SataClass - SATA Controller mode [2:0]
  uint8_t                 SataIdeMode;                  ///< SataIdeMode - Sata IDE Controller mode
                                                        ///  @par
                                                        ///   @li <b>0</b> - Legacy IDE mode
                                                        ///   @li <b>1</b> - Native IDE mode
                                                        ///
  uint8_t                 SataDisUnusedIdePChannel;     ///< Reserved
  uint8_t                 SataDisUnusedIdeSChannel;     ///< Reserved
  uint8_t                 IdeDisUnusedIdePChannel;      ///< Reserved
  uint8_t                 IdeDisUnusedIdeSChannel;      ///< Reserved
  uint8_t                 SataOptionReserved;           /// SataOptionReserved - Reserved

  SATA_PORT_ST            SataEspPort;                  ///<  SataEspPort - SATA port is external accessible on a
                                                        ///                 signal only connector (eSATA:)

  SATA_PORT_ST            SataPortPower;                ///<  SataPortPower - Port Power configuration

  SATA_PORT_MD            SataPortMd;                   ///<  SataPortMd - Port Mode

  uint8_t                 SataAggrLinkPmCap;            /// SataAggrLinkPmCap - 0:OFF   1:ON
  uint8_t                 SataPortMultCap;              /// SataPortMultCap - 0:OFF   1:ON
  uint8_t                 SataClkAutoOff;               /// SataClkAutoOff - AutoClockOff 0:Disabled, 1:Enabled
  uint8_t                 SataPscCap;                   /// SataPscCap 1:Enable PSC, 0:Disable PSC capability
  uint8_t                 BiosOsHandOff;                /// BiosOsHandOff - Reserved
  uint8_t                 SataFisBasedSwitching;        /// SataFisBasedSwitching - Reserved
  uint8_t                 SataCccSupport;               /// SataCccSupport - Reserved
  uint8_t                 SataSscCap;                   /// SataSscCap - 1:Enable, 0:Disable SSC capability
  uint8_t                 SataMsiCapability;            /// SataMsiCapability 0:Hidden 1:Visible
  uint8_t                 SataForceRaid;                /// SataForceRaid   0:No function 1:Force RAID
  uint8_t                 SataInternal100Spread;        /// SataInternal100Spread - Reserved
  uint8_t                 SataDebugDummy;               /// SataDebugDummy - Reserved
  uint8_t                 SataTargetSupport8Device;     /// SataTargetSupport8Device - Reserved
  uint8_t                 SataDisableGenericMode;       /// SataDisableGenericMode - Reserved
  bool                    SataAhciEnclosureManagement;  /// SataAhciEnclosureManagement - Reserved
  uint8_t                 SataSgpio0;                   /// SataSgpio0 - Reserved
  uint8_t                 SataSgpio1;                   /// SataSgpio1 - Reserved
  uint8_t                 SataPhyPllShutDown;           /// SataPhyPllShutDown - Reserved
  bool                    SataHotRemovalEnh;            /// SataHotRemovalEnh - Reserved

  SATA_PORT_ST            SataHotRemovalEnhPort;        ///<  SataHotRemovalEnhPort - Hot Remove

  bool                    SataOobDetectionEnh;          /// SataOobDetectionEnh - true
  bool                    SataPowerSavingEnh;           /// SataPowerSavingEnh - true
  uint8_t                 SataMemoryPowerSaving;        /// SataMemoryPowerSaving - 0-3 Default [3]
  bool                    SataRasSupport;               /// SataRasSupport - Support RAS function
                                                        ///  @par
                                                        ///   @li true  - Enable
                                                        ///   @li false - Disable
  bool                    SataAhciDisPrefetchFunction;  /// SataAhciDisPrefetchFunction - Disable AHCI Prefetch
  bool                    SataDevSlpPort0;              /// SataDevSlpPort0 - Reserved
  bool                    SataDevSlpPort1;              /// SataDevSlpPort1 - Reserved
  uint32_t                TempMmio;                     /// TempMmio - Reserved
  uint8_t                 SataDevSlpPort0Num;           /// SataDevSlpPort0Num - Reserved
  uint8_t                 SataDevSlpPort1Num;           /// SataDevSlpPort1Num - Reserved
} FCH_SATA;

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
                                            //<b>1</b> - disable
                                            //<b>0</b> - enable
  uint8_t           GcpuMsgCMultiCore;      // Track of CPU C state by monitoring each core's C state message
                                            // <b>1</b> - disable
                                            // <b>0</b> - enable
  uint8_t           GcpuMsgCStage;          // Enable the FCH C state coordination logic
                                            // <b>1</b> - disable
                                            // <b>0</b> - enable
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
  bool               NoneSioKbcSupport;              // NoneSioKbcSupport - No KBC/SIO controller ( Turn on Inchip
                                                     //                     KBC emulation function )
  FCH_CS             FchCsSupport;                   // FCH Cs function structure
  FCH_CPPC           Cppc;                           // FCH CPPC support
  bool               FchiLa1MTraceMemoryEn;          // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Enable
  uint32_t           FchiLa1MTraceMemoryBase;        // FchiLa1MTraceMemoryBase - Fch iLa 1M Trace Memory Base
} FCH_MISC;

///
/// SMBus structure
///
typedef struct {
  uint32_t           SmbusSsid;               // SMBUS controller Subsystem ID
} FCH_SMBUS;


///
/// Reset Type
///
typedef enum {
  WarmReset = 0,                             ///< Warm Reset
  ColdReset = 1,                             ///< Cold Reset
} RESET_TYPE;

///
/// Acpi structure
///
typedef struct {
  uint16_t           Smbus0BaseAddress;       // Smbus BASE Address
  uint16_t           SioPmeBaseAddress;       // SIO PME BASE Address
  uint32_t           WatchDogTimerBase;       // Watch Dog Timer Address
  uint16_t           AcpiPm1EvtBlkAddr;       // ACPI PM1 event block Address
  uint16_t           AcpiPm1CntBlkAddr;       // ACPI PM1 Control block Address
  uint16_t           AcpiPmTmrBlkAddr;        // ACPI PM timer block Address
  uint16_t           CpuControlBlkAddr;       // ACPI CPU control block Address
  uint16_t           AcpiGpe0BlkAddr;         // ACPI GPE0 block Address
  uint16_t           SmiCmdPortAddr;          // SMI command port Address
  uint16_t           AcpiPmaCntBlkAddr;       // ACPI PMA Control block Address
  bool               SpreadSpectrum;          // Spread Spectrum function; 0-disable; 1-enable
  uint8_t            SpreadSpectrumOptions;   // Spread Spectrum Option
  POWER_FAIL         PwrFailShadow;           // PwrFailShadow = PM_Reg: 5Bh [3:0]
                                                 // <b>00</b> - Always off
                                                 // <b>01</b> - Always on
                                                 // <b>11</b> - Use previous
  uint8_t            StressResetMode;         // StressResetMode 01-10
                                                 // <b>00</b> - Disabed
                                                 // <b>01</b> - Io Write 0x64 with 0xfe
                                                 // <b>10</b> - Io Write 0xcf9 with 0x06
                                                 // <b>11</b> - Io Write 0xcf9 with 0x0e
  bool               NoClearThermalTripSts;   // Skip clearing ThermalTrip status
  uint32_t           FchAcpiMmioBase;         // FCH ACPI MMIO Base
  bool               FchAlinkRasSupport;      // FCH A-Link parity error support
  uint32_t           I2c0SdaHold;             // I2C0 SDA_HOLD
  uint32_t           I2c1SdaHold;             // I2C1 SDA_HOLD
  uint32_t           I2c2SdaHold;             // I2C2 SDA_HOLD
  uint32_t           I2c3SdaHold;             // I2C3 SDA_HOLD
  uint32_t           I2c4SdaHold;             // I2C4 SDA_HOLD
  uint32_t           I2c5SdaHold;             // I2C5 SDA_HOLD
  bool               FchAoacProgramEnable;    // Enable/disable AOAC init programming
  bool               SpdHostCtrlRelease;      // Release SPD Host Ctrl
  bool               DimmTelemetry;           // Send message to PMFW to start DIMM telemetry
  bool               FchModernStandby;        // Enable Modern Standby feature
  bool               BootTimerEnable;         // Enable FCH Boot Timer
  bool               BootTimerResetType;      // Boot Timer Reset Type
  bool               ToggleAllPwrGoodOnCf9;   // Enable Toggle all PowerGood on Cf9 feature
  uint32_t           Reserverd[16];           // Reserved
  void*              OemProgrammingTablePtr;  // Pointer of ACPI OEM table
} FCHHWACPI_INPUT_BLK;


///
/// _ABTblEntry - AB link register table R/W structure
///
typedef struct {
  uint8_t  RegType;  // RegType  : AB Register Type (ABCFG, AXCFG and so on)
  uint32_t RegIndex; // RegIndex : AB Register Index
  uint32_t RegMask;  // RegMask  : AB Register Mask
  uint32_t RegData;  // RegData  : AB Register Data
} AB_TBL_ENTRY;

///
/// AB structure
///
typedef struct {
  bool    AbMsiEnable;                    // ABlink MSI capability
  uint8_t ALinkClkGateOff;                // Alink Clock Gate-Off function - 0:disable, 1:enable *KR
  uint8_t BLinkClkGateOff;                // Blink Clock Gate-Off function - 0:disable, 1:enable *KR
  uint8_t AbClockGating;                  // AB Clock Gating - 0:disable, 1:enable *KR *CZ
  uint8_t SlowSpeedAbLinkClock;           // Slow Speed AB Link Clock - 0:disable, 1:enable *KR
  bool    ResetCpuOnSyncFlood;            // Reset Cpu On Sync Flood - 0:disable, 1:enable *KR
  bool    AbDmaMemoryWrtie3264B;          // AB DMA Memory Write 32/64 BYTE Support *KR only
  bool    AbMemoryPowerSaving;            // AB Memory Power Saving *KR *CZ
  bool    SbgDmaMemoryWrtie3264ByteCount; // SBG DMA Memory Write 32/64 BYTE Count Support *KR only
  bool    SbgMemoryPowerSaving;           // SBG Memory Power Saving *KR *CZ
  bool    SbgClockGating;                 // SBG Clock Gate *CZ
  bool    XdmaDmaWrite16ByteMode;         // XDMA DMA Write 16 byte mode *CZ
  bool    XdmaMemoryPowerSaving;          // XDMA memory power saving *CZ
  uint8_t XdmaPendingNprThreshold;        // XDMA PENDING NPR THRESHOLD *CZ
  bool    XdmaDncplOrderDis;              // XDMA DNCPL ORDER DIS *CZ
  bool    SdphostBypassDataPack;          // SdphostBypassDataPack
  bool    SdphostDisNpmwrProtect;         // Disable NPMWR interleaving protection
} FCHAB_INPUT_BLK;

typedef struct {
  uint32_t Usb20OcPin; // XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
  uint16_t Usb31OcPin; // XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
} XHCI_OC;

typedef struct {
  uint8_t COMPDSTUNE;
  uint8_t SQRXTUNE;
  uint8_t TXFSLSTUNE;
  uint8_t TXPREEMPAMPTUNE;
  uint8_t TXPREEMPPULSETUNE;
  uint8_t TXRISETUNE;
  uint8_t TXVREFTUNE;
  uint8_t TXHSXVTUNE;
  uint8_t TXRESTUNE;
} FCH_USB20_PHY;

typedef struct {
  uint8_t RX_ANA_IQ_PHASE_ADJUST;  //RX_ANA_IQ_PHASE_ADJUST
  uint8_t RX_EQ_DELTA_IQ_OVRD_EN;  //RX_EQ_DELTA_IQ_OVRD_EN
  uint8_t RX_EQ_DELTA_IQ_OVRD_VAL; //RX_EQ_DELTA_IQ_OVRD_VAL
  uint8_t RX_IQ_PHASE_ADJUST;      //RX_IQ_PHASE_ADJUST
  uint8_t TX_VBOOST_LVL_EN;        //SUP_DIG_LVL_OVRD_IN::TX_VBOOST_LVL_EN
  uint8_t TX_VBOOST_LVL;           //SUP_DIG_LVL_OVRD_IN::TX_VBOOST_LVL
  uint8_t RX_VREF_CTRL_EN;         //SUP_DIG_LVL_OVRD_IN::RX_VREF_CTRL_EN
  uint8_t RX_VREF_CTRL;            //SUP_DIG_LVL_OVRD_IN::RX_VREF_CTRL
  uint8_t TX_VBOOST_LVL_EN_X;      //SUPX_DIG_LVL_OVRD_IN::TX_VBOOST_LVL_EN
  uint8_t TX_VBOOST_LVL_X;         //SUPX_DIG_LVL_OVRD_IN::TX_VBOOST_LVL
  uint8_t RX_VREF_CTRL_EN_X;       //SUPX_DIG_LVL_OVRD_IN::RX_VREF_CTRL_EN
  uint8_t RX_VREF_CTRL_X;          //SUPX_DIG_LVL_OVRD_IN::RX_VREF_CTRL
} FCH_USB31_PHY;

typedef struct {
  uint8_t       Version_Major;     // USB IP version
  uint8_t       Version_Minor;     // USB IP version
  uint8_t       TableLength;       // TableLength
  uint8_t       Reserved0;
  uint8_t       Usb20PhyEnable;    // Enable USB 2.0 PHY setting
  FCH_USB20_PHY Usb20PhyPort[16];   // USB 2.0 Driving Strength
  uint8_t       Reserved1;         // Reserved
  uint8_t       S1Usb20PhyEnable;  // Enable USB 2.0 PHY setting
  FCH_USB20_PHY S1Usb20PhyPort[16]; // USB 2.0 Driving Strength
  uint8_t       Usb31PhyEnable;    // Enable USB 3.1 PHY setting
  FCH_USB31_PHY Usb31PhyPort[8];   // USB 3.1 Driving Strength
  uint8_t       S1Usb31PhyEnable;  // Enable USB 3.1 PHY setting
  FCH_USB31_PHY S1Usb31PhyPort[8]; // USB 3.1 Driving Strength
  uint8_t       Reserved[256];     // Reserved for future
} FCH_USB_OEM_PLATFORM_TABLE;

///
/// FCH USB sturcture
///
typedef struct {
  uint32_t           DieBusNum;                      ///< IOHC Die bus number of USB controllers
  bool               Xhci0Enable;                    ///< XHCI0 controller enable
  bool               Xhci1Enable;                    ///< XHCI1 controller enable
  bool               Xhci2Enable;                    ///< XHCI2 controller enable
  uint32_t           XhciSsid;                       ///< XHCI SSID
  bool               XhciECCDedErrRptEn;
  uint32_t           Xhci0DevRemovable;
  bool               DisableXhciPortLate;
  uint32_t           XhciUsb3PortDisable;
  uint32_t           XhciUsb2PortDisable;
  XHCI_OC            XhciOCpinSelect[2];             ///< XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
  bool               XhciOcPolarityCfgLow;
  uint8_t            Usb3PortForceGen1;
  FCH_USB_OEM_PLATFORM_TABLE OemUsbConfigurationTable;
  bool               UsbSparseModeEnable;
} FCH_USB, FCHUSB_INPUT_BLK;

/// Emmc Config bit maps
typedef union {
  struct {                                                           ///<
    uint64_t                  Spec_Ver :8;                           ///< [0:7]Spec version
    uint64_t                  Sdr50_Sup :1;                          ///< [8] SDR50 Sup
    uint64_t                  Sdr100_Sup :1;                         ///< [9] SDR100 Sup
    uint64_t                  Ddr50_Sup :1;                          ///< [10] DDR50 Sup
    uint64_t                  Force18 :1;                            ///< [11]Force 1.8V
    uint64_t                  Force33 :1;                            ///< [12]Force 3.3V
    uint64_t                  ReTuningMode :2;                       ///< [13:14] AdvisoryNonfatalErrMask
    uint64_t                  Sdr50Tune :1;                          ///< [15]Sdr50 Tune
    uint64_t                  WpCdTie :2;                            ///< [16:17] WP_TIE, CD_TIE
    uint64_t                  Sdr50DriverStrength :2;                ///< [18:19] WP_TIE, CD_TIE
    uint64_t                  Ddr50DriverStrength :2;                ///< [20:21] WP_TIE, CD_TIE
    uint64_t                  SlotType :2;                           ///< [22:23] WP_TIE, CD_TIE
    uint64_t                  BaseFre :8;                            ///< [24:31] Base Frequence
    uint64_t                  DownSpeed :2;                          ///< [32:33] Down Speed
    uint64_t                  Reserved_63_34 :30;                    ///< Unused bits
  } Field;                                                           ///<
//  uint64_t Value;                                                  ///<
} EMMC_CONFIG_BIT_MAP;


///
/// FCH EMMC sturcture
///
typedef struct {
  uint8_t            EmmcEnable;         ///< eMMC, 0:Disabled, 1:Enabled
  uint8_t            EmmcType;            ///  @li   <b>0001</b> - Normal Speed 12.5MB/s 25MHz (v1.01) pll clock 63MHz
                                          ///  @li   <b>0010</b> - High Speed 25MB/s 50MHz (v2) pll clock 400MHz
                                          ///  @li   <b>0011</b> - UHSI-SDR50 50MB/s 100MHz (v3.01) pll clock 400MHz
                                          ///  @li   <b>0100</b> - UHSI-DDR50 50MB/s 50MHz (v3.01)
                                          ///  @li   <b>0101</b> - UHSI-SDR104 100MB/s 200MHz (v3.01)
                                          ///  @li   <b>0110</b> - Backward Comp 25MB/s 25MHz (v4.5)
                                          ///  @li   <b>0111</b> - High Speed SDR 50MB/s 50MHz (v4.5)
                                          ///  @li   <b>1000</b> - High Speed DDR 100MB/s 50MHz (v4.5)
                                          ///  @li   <b>1001</b> - HS200 200MB/s 200MHz (v4.5)
                                          ///  @li   <b>1010</b> - HS400 400MB/s 200MHz (v5)
                                          ///  @li   <b>1011</b> - HS300 300MB/s 150MHz (v5)
  bool               EmmcDriverType;     ///< Enable eMMC as PnP0D40, False:AMDI0040, True:PnP0D40
  bool               EmmcBoot;           ///< eMMC boot, 0:Disabled, 1:Enabled
  bool               EmmcAdma2Support;   ///< Emmc Adma2 Support, 0:Disabled, 1:Enabled
  bool               EmmcAdmaSupport;    ///< Emmc Adma  Support, 0:Disabled, 1:Enabled
  bool               EmmcSdmaSupport;    ///< Emmc Sdma  Support, 0:Disabled, 1:Enabled
  bool               EmmcA64bSupport;    ///< Emmc A64b  Support, 0:Disabled, 1:Enabled
  bool               EmmcD3Support;      ///< Emmc D3  Support, 0:Disabled, 1:Enabled
} FCHEMMC_INPUT_BLK;

///
/// FCH GBE sturcture
///
typedef struct {
  bool                    Gbe0Enable;                     ///< Gbe controller0 enable/disable
  bool                    Gbe1Enable;                     ///< Gbe controller1 enable/disable
  uint32_t                Gbe0MacAddress0Low;             ///< Gbe0 MAC Address0 Low
  uint32_t                Gbe0MacAddress0High;            ///< Gbe0 MAC Address0 High
  uint32_t                Gbe0MacAddress1Low;             ///< Gbe0 MAC Address1 Low
  uint32_t                Gbe0MacAddress1High;            ///< Gbe0 MAC Address1 High
  uint32_t                Gbe1MacAddress0Low;             ///< Gbe1 MAC Address0 Low
  uint32_t                Gbe1MacAddress0High;            ///< Gbe1 MAC Address0 High
  uint32_t                Gbe1MacAddress1Low;             ///< Gbe1 MAC Address1 Low
  uint32_t                Gbe1MacAddress1High;            ///< Gbe1 MAC Address1 High
} FCH_GBE;

///
/// ASF structure
///
typedef struct {
  bool               DisableMaster;                 // Disable ASF Master
  bool               DisableSecondary;                  // Disable ASF Secondary
} FCH_ASF;

/// FCH Platform Configuration Policy
typedef struct {
  uint16_t     CfgSmbus0BaseAddress;        ///< SMBUS0 Controller Base Address
  uint16_t     CfgSmbus1BaseAddress;        ///< SMBUS1 Controller Base Address
  uint16_t     CfgSioPmeBaseAddress;        ///< I/O base address for LPC I/O target range
  uint16_t     CfgAcpiPm1EvtBlkAddr;        ///< I/O base address of ACPI power management Event Block
  uint16_t     CfgAcpiPm1CntBlkAddr;        ///< I/O base address of ACPI power management Control Block
  uint16_t     CfgAcpiPmTmrBlkAddr;         ///< I/O base address of ACPI power management Timer Block
  uint16_t     CfgCpuControlBlkAddr;        ///< I/O base address of ACPI power management CPU Control Block
  uint16_t     CfgAcpiGpe0BlkAddr;          ///< I/O base address of ACPI power management General Purpose Event Block
  uint16_t     CfgSmiCmdPortAddr;           ///< I/O base address of ACPI SMI Command Block
  uint16_t     CfgAcpiPmaCntBlkAddr;        ///< I/O base address of ACPI power management additional control block
  uint32_t     CfgGecShadowRomBase;         ///< 32-bit base address to the GEC shadow ROM
  uint32_t     CfgWatchDogTimerBase;        ///< Watchdog Timer base address
  uint32_t     CfgSpiRomBaseAddress;        ///< Base address for the SPI ROM controller
  uint32_t     CfgHpetBaseAddress;          ///< HPET MMIO base address
  uint32_t     CfgAzaliaSsid;               ///< Subsystem ID of HD Audio controller
  uint32_t     CfgSmbusSsid;                ///< Subsystem ID of SMBUS controller
  uint32_t     CfgIdeSsid;                  ///< Subsystem ID of IDE controller
  uint32_t     CfgSataAhciSsid;             ///< Subsystem ID of SATA controller in AHCI mode
  uint32_t     CfgSataIdeSsid;              ///< Subsystem ID of SATA controller in IDE mode
  uint32_t     CfgSataRaid5Ssid;            ///< Subsystem ID of SATA controller in RAID5 mode
  uint32_t     CfgSataRaidSsid;             ///< Subsystem ID of SATA controller in RAID mode
  uint32_t     CfgEhciSsid;                 ///< Subsystem ID of EHCI
  uint32_t     CfgOhciSsid;                 ///< Subsystem ID of OHCI
  uint32_t     CfgLpcSsid;                  ///< Subsystem ID of LPC ISA Bridge
  uint32_t     CfgSdSsid;                   ///< Subsystem ID of SecureDigital controller
  uint32_t     CfgXhciSsid;                 ///< Subsystem ID of XHCI
  bool         CfgFchPort80BehindPcib;      ///< Is port80 cycle going to the PCI bridge
  bool         CfgFchEnableAcpiSleepTrap;   ///< ACPI sleep SMI enable/disable
  GPP_LINKMODE CfgFchGppLinkConfig;         ///< GPP link configuration
  bool         CfgFchGppPort0Present;       ///< Is FCH GPP port 0 present
  bool         CfgFchGppPort1Present;       ///< Is FCH GPP port 1 present
  bool         CfgFchGppPort2Present;       ///< Is FCH GPP port 2 present
  bool         CfgFchGppPort3Present;       ///< Is FCH GPP port 3 present
  bool         CfgFchGppPort0HotPlug;       ///< Is FCH GPP port 0 hotplug capable
  bool         CfgFchGppPort1HotPlug;       ///< Is FCH GPP port 1 hotplug capable
  bool         CfgFchGppPort2HotPlug;       ///< Is FCH GPP port 2 hotplug capable
  bool         CfgFchGppPort3HotPlug;       ///< Is FCH GPP port 3 hotplug capable
  uint8_t      CfgFchEsataPortBitMap;       ///< ESATA Port definition, eg: [0]=1, means port 0 is ESATA capable
  uint8_t      CfgFchIrPinControl;          ///< Register bitfield describing Infrared Pin Control:
                                            ///<   [0] - IR Enable 0
                                            ///<   [1] - IR Enable 1
                                            ///<   [2] - IR Tx0
                                            ///<   [3] - IR Tx1
                                            ///<   [4] - IR Open Drain
                                            ///<   [5] - IR Enable LED
  SD_CLOCK_CONTROL  CfgFchSdClockControl;   ///< FCH SD Clock Control
  SCI_MAP_CONTROL  *CfgFchSciMapControl;    ///< FCH SCI Mapping Control
  SATA_PHY_CONTROL *CfgFchSataPhyControl;   ///< FCH SATA PHY Control
  GPIO_CONTROL     *CfgFchGpioControl;      ///< FCH GPIO Control
  bool              CfgFchRtcWorkAround;    ///< FCH RTC Workaround
  bool              CfgFchUsbPortDisWorkAround;  ///< FCH USB Workaround
} FCH_PLATFORM_POLICY;

/// Private: FCHCLASS_INPUT_BLK
typedef struct {
  FCH_RUNTIME           FchRunTime;                     ///< FCH Run Time Parameters
  FCH_SMBUS             Smbus;                          ///< SMBus structure
  FCH_SD                Sd;                             ///< SD structure
  FCH_HPET              Hpet;                           ///< HPET structure
  FCH_GCPU              Gcpu;                           ///< GCPU structure
  FCH_IMC               Imc;                            ///< IMC structure
  FCH_MISC              Misc;                           ///< MISC structure
  FCH_RESET_INTERFACE   FchReset;                       ///< Reset interface
  bool                  EcKbd;                          ///< EC KBD - 0:disable, 1:enable
  bool                  LegacyFree;                     ///< Legacy Free - 0:disable, 1:enable
  bool                  SataSetMaxGen2;                 ///< SATA enable maximum GEN2
  uint8_t               SataClkMode;                    ///< SATA reference clock selector and divider
  uint8_t               SataModeReg;                    ///< Output: SATAConfig PMIO:0xDA
  bool                  SataInternal100Spread;          ///< SATA internal 100MHz spread ON/OFF
  bool                  FchOscout1ClkContinous;         ///< FCH OSCOUT1_CLK Continous
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
