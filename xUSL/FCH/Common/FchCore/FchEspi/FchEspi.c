/**
 * @file  FchEspi.c
 * @brief FCH ESPI functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <FCH/Common/Fch.h>
#include <FchEspi.h>
#include <FchEspiReg.h>
#include <FCH/Common/FchCommonCfg.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <Pci.h>

/**
 * GetEspiBase
 *
 * Get Espi MMIO base address.
 *
 */
static uint32_t GetEspiBase (void)
{
  uint32_t Base;
  uint32_t MasterCap;
  Base = xUSLPciRead32(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, 0xA0));
  Base &= 0xFFFFFFC0;
  Base += 0x00010000;

  MasterCap = xUSLMemRead32 ((void *)(size_t)(Base + ESPI_MASTER_CAP));
  // eSPI Master Capability
  if (MasterCap == 0 || MasterCap == 0xFFFFFFFF) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, " ESPI master capablity not configured!! \n");
  }

  return Base;
}

/**
 * FchinitHSEspiEnableKbc6064 - Config ESPI controller
 *
 * @param[in] Enable Enable/disable ESPI 60h/64h decoding.
 */
static void FchinitHSEspiEnableKbc6064 (bool Enable)
{
  uint32_t EspiBase;
  EspiBase = GetEspiBase ();
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  if (Enable) {
      xUSLPciReadModifyWrite32 (PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_29),
                                ~BIT_32(29), 0x00);    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), ~BIT_32(1), BIT_32(1)); // 60h/64h
  } else {
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), ~BIT_32(1), 0x00); // 60h/64h
  }
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

static void FchinitHSEspiEnableIo80 (bool Enable)
{
  uint32_t EspiBase;
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  EspiBase = GetEspiBase ();
  if (Enable) {
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), ~BIT_32(2), BIT_32(2));                            // Enable 80 IO decode
  } else {
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), ~BIT_32(2), 0);                            // Disable 80 IO decode
  }
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**
 * CheckDnCommandStatus
 *
 * Wait for DNCMD_STATUS(BIT_32(3)) bit in ESPI_DN_TXHDR_0
 * to get cleared
 */
static void
CheckDnCommandStatus (
    uint32_t     EspiBase
  )
{
  uint32_t Retry;

  for (Retry = 0; Retry < MAX_ESPI_RETRY; Retry++) {
    if ((xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0)) & DNCMD_STATUS) == 0) {
      break;
    }
    xUSLIoWrite16 (0x80, 0xE291);
  }
}

/**
 * EspiGetConfiguration
 *
 * Read RegAddr configuration.
 *
 * @param EspiBase : Espi MMIO base address.
 * @param RegAddr  : DNCMD_HDATA1 Field
 *
 * @Return  : ESPIx04_DN_TXHDR1 register value.
 */
static
uint32_t
EspiGetConfiguration  (
  uint32_t     EspiBase,
  uint32_t     RegAddr
  )
{
  ESPIx00_DN_TXHDR0  DxHdr0;

  CheckDnCommandStatus (EspiBase);
  DxHdr0.Value = xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0));

  DxHdr0.Field.SecondaryNSelect = 0;
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

  DxHdr0.Field.SWCommandType = GET_CONFIGURATION;
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

  DxHdr0.Field.AddrByte0PCycTypeVWCntOOBCycType = RegAddr & 0xFF;
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

  DxHdr0.Field.AddrByte0VWIdxOOBPktByte1 = (RegAddr & 0xFF00) >> 8;
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_1), 0x00);
  DxHdr0.Field.CommandStatus = 1;
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

  CheckDnCommandStatus (EspiBase);
  return xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_1));
}

/**
 * EspiSetConfiguration
 *
 * Set RegAddr configuration.
 *
 * @param EspiBase : Espi MMIO base address.
 * @param RegAddr  : DNCMD_HDATA1 Field
 * @param Value    : Value to write in ESPI_DN_TXHDR_1 reg.
 */
static void
EspiSetConfiguration  (
    uint32_t     EspiBase,
    uint32_t     RegAddr,
    uint32_t     Value
  )
{
    ESPIx00_DN_TXHDR0  DxHdr0;

    CheckDnCommandStatus (EspiBase);
    DxHdr0.Value = xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0));

    DxHdr0.Field.SecondaryNSelect = 0;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    DxHdr0.Field.SWCommandType = SET_CONFIGURATION;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    DxHdr0.Field.AddrByte0PCycTypeVWCntOOBCycType = RegAddr & 0xFF;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    DxHdr0.Field.AddrByte0VWIdxOOBPktByte1 = (RegAddr & 0xFF00) >> 8;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_1), Value);
    DxHdr0.Field.CommandStatus = 1;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    CheckDnCommandStatus (EspiBase);
}

/**
 * EspiInBandRst
 *
 *
 * @param EspiBase : Espi MMIO base address.
 */
static void
EspiInBandRst  (
  uint32_t     EspiBase
  )
{

    ESPIx00_DN_TXHDR0  DxHdr0;
    ESPIx68_SECONDARY0_CONFIG Secondary0Config;

    Secondary0Config.Value = xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG));
    DxHdr0.Value = xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0));

    DxHdr0.Field.CommandStatus = 0;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    CheckDnCommandStatus (EspiBase);
    DxHdr0.Field.SecondaryNSelect = 0;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    Secondary0Config.Field.OperatingFreq = 0;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    DxHdr0.Field.SWCommandType = IN_BAND_RESET;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), Secondary0Config.Value);

    DxHdr0.Field.CommandStatus = 1;
    xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_DN_TXHDR_0), DxHdr0.Value);

    CheckDnCommandStatus (EspiBase);
}

/**
 * FchinitHSEspiSecondary0Device
 *
 * Configure ESPI controller and device.
 *
 * @param EspiBase ESPI MMIO base.
 *
 */
static void FchinitHSEspiSecondary0Device (uint32_t EspiBase)
{

  ESPIx2C_MASTER_CAP              MasterCapibility;
  ESPIx68_SECONDARY0_CONFIG           Secondary0Config;
  ESPI_SL08_SECONDARY_GENERAL_CAPCFG  DeviceCapCfg;
  ESPI_SL10_SECONDARY_PC_CAPCFG       SecondaryPCCapCfg;
  ESPI_SL20_SECONDARY_VW_CAPCFG       SecondaryVWCapCfg;
  ESPI_SL30_SECONDARY_OOB_CAPCFG      SecondaryOOBCapCfg;
  ESPI_SL40_SECONDARY_FA_CAPCFG       SecondaryFACapCfg;

  //
  // 1, In-Band Reset
  //
  EspiInBandRst (EspiBase);

  //
  // 2 - 4, check and update Secondary Device Capability and Secondary0 configuration
  //
  MasterCapibility.Value = xUSLMemRead32 ((void *)(size_t)(EspiBase + ESPI_MASTER_CAP));
  DeviceCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_GENERAL_CAPCFG);
  Secondary0Config.Value = 0;
  //
  // Check support for each channel
  //
  if ( MasterCapibility.Field.FlashAccessChannelSupport && DeviceCapCfg.Field.RO_FASupported ) {
    Secondary0Config.Field.FlashAccessChannelEnable = 1;
  }

  if ( MasterCapibility.Field.OOBMessageChannelSupport && DeviceCapCfg.Field.RO_OOBMsgSupported ) {
    Secondary0Config.Field.OOBMessageChannelEnable = 1;
  }

  if ( MasterCapibility.Field.VWChannelSupport && DeviceCapCfg.Field.RO_VWSupported ) {
    Secondary0Config.Field.VWChannelEnable = 1;
  }

  if ( MasterCapibility.Field.PChannelSupport && DeviceCapCfg.Field.RO_PCSupported ) {
    Secondary0Config.Field.PChannelEnable = 1;
  }

  //
  // Operating Frequency - fix to 16.7MHz (000) for now
  //
  Secondary0Config.Field.OperatingFreq = 0;
  DeviceCapCfg.Field.OperatingFreq = 0;

  //
  // I/O Mode Select:
  //
  if ( (MasterCapibility.Field.IOMode == 2) && (DeviceCapCfg.Field.RO_IOModeSupported & BIT_32(1)) ) {
    // Quad Mode
    Secondary0Config.Field.IOModeSelect = 2;
    DeviceCapCfg.Field.IOModeSelect = 2;
  } else if ( (MasterCapibility.Field.IOMode == 1) && (DeviceCapCfg.Field.RO_IOModeSupported & BIT_32(0)) ) {
    // Dual Mode
    Secondary0Config.Field.IOModeSelect = 1;
    DeviceCapCfg.Field.IOModeSelect = 1;
  } else {
    // single Mode
    Secondary0Config.Field.IOModeSelect = 0;
    DeviceCapCfg.Field.IOModeSelect = 0;
  }

  //
  // Alert Mode
  //
  Secondary0Config.Field.AlertMode = MasterCapibility.Field.AlertMode;
  DeviceCapCfg.Field.AlertMode = MasterCapibility.Field.AlertMode;

  //
  // CRC Checking
  //
  Secondary0Config.Field.CRCCheckingEnable = MasterCapibility.Field.CRCCheck;
  DeviceCapCfg.Field.CRCCheckingEn = MasterCapibility.Field.CRCCheck;

  //
  // update Device configuration and Secondary0 configuration
  //
  EspiSetConfiguration (EspiBase, ESPI_SECONDARY_GENERAL_CAPCFG, DeviceCapCfg.Value);
  xUSLMemWrite32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG), (Secondary0Config.Value & 0xFFFFFFF0));

  //
  // 5 - 8 Configure and enable channels
  //
  // FA Channel
  //
  if ( Secondary0Config.Field.FlashAccessChannelEnable ) {
    SecondaryFACapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_FA_CAPCFG);
    SecondaryFACapCfg.Field.FAEn = 1;
    EspiSetConfiguration (EspiBase, ESPI_SECONDARY_FA_CAPCFG, SecondaryFACapCfg.Value);
    do {
      SecondaryFACapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_FA_CAPCFG);
      xUSLIoWrite16 (0x80, 0xE292);
    } while (!SecondaryFACapCfg.Field.RO_FAReady);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG), ~(uint32_t)0, SECONDARY_FA_ENABLE);
  }

  //
  // OOB Channel
  //
  if ( Secondary0Config.Field.OOBMessageChannelEnable ) {
    SecondaryOOBCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_OOB_CAPCFG);
    SecondaryOOBCapCfg.Field.OOBEn = 1;
    EspiSetConfiguration (EspiBase, ESPI_SECONDARY_OOB_CAPCFG, SecondaryOOBCapCfg.Value);
    do {
      SecondaryOOBCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_OOB_CAPCFG);
      xUSLIoWrite16 (0x80, 0xE292);
    } while (!SecondaryOOBCapCfg.Field.RO_OOBReady);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG), ~(uint32_t)0, SECONDARY_FA_ENABLE);
  }

  //
  // VW Channel
  //
  if ( Secondary0Config.Field.VWChannelEnable ) {
    SecondaryVWCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_VW_CAPCFG);
    SecondaryVWCapCfg.Field.VWEn = 1;
    EspiSetConfiguration (EspiBase, ESPI_SECONDARY_VW_CAPCFG, SecondaryVWCapCfg.Value);
    do {
      SecondaryVWCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_VW_CAPCFG);
      xUSLIoWrite16 (0x80, 0xE292);
    } while (!SecondaryVWCapCfg.Field.RO_VWReady);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG), ~(uint32_t)0, SECONDARY_VW_ENABLE);
  }

  //
  // PC Channel
  //
  if ( Secondary0Config.Field.PChannelEnable ) {
    SecondaryPCCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_PC_CAPCFG);
    SecondaryPCCapCfg.Field.PCEn = 1;
    EspiSetConfiguration (EspiBase, ESPI_SECONDARY_PC_CAPCFG, SecondaryPCCapCfg.Value);
    do {
      SecondaryPCCapCfg.Value = EspiGetConfiguration (EspiBase, ESPI_SECONDARY_PC_CAPCFG);
      xUSLIoWrite16 (0x80, 0xE292);
    } while (!SecondaryPCCapCfg.Field.RO_PCReady);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + ESPI_SECONDARY0_CONFIG), ~(uint32_t)0, SECONDARY_PC_ENABLE);
  }
}

/**
 * FchinitHSEspiTimer - Config ESPI controller
 *
 *
 *
 * @param[in] EspiBase ESPI MMIO base.
 *
 */
static void FchinitHSEspiTimer (uint32_t EspiBase)
{
  // eSPIx30 [29:24, WaitStateCounter]     = 00h,
  //         [23:8,  WatchDogCounter]      = 0FFFh,
  //         [6:4,   Glb_Alink_Idle_Timer] = 111b - 2048 clocks
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x30), 0xFF00008F, 0x000FFF70);
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x30), ~BIT_32(0), BIT_32(0)); // WDG_EN
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x30), ~BIT_32(1), BIT_32(1)); // WAIT_CHKEN.
}

/**
 * FchinitHSEspiEc0 - Config ESPI controller
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchinitHSEspiEc0 (void)
{
  uint32_t     EspiBase;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  EspiBase = GetEspiBase ();

  //6.1 eSPI Control Register Initialization
  //  1  Enable PCI Bus Watch Dog Timer
  //  2  Enable eSPI Bus Wait State Control Timer
  //  3  Enable eSPI clock gating function if neeed.
  //  4  Set each SecondaryN Error Interrupt Enable.
  //  5  Set eSPI Controller ERROR interrupt Mapping, default is SMI.
  //  6  Set eSPI error protection bit if needed.
  //  7  Set each SecondaryN Register command interrupt Enable.
  //  8  Set eSPI Register Command interrupt Mapping, default is SMI.
  //  9  Set each SecondaryN IO Range, MMIO Range and decoding enable.
  //  10 Enable eSPI subtractive decoding if needed
  //  11 Assign IRQ0~IRQ23 to each SecondaryN by setting each SecondaryN IRQ Mask bit, IRQ plority.
  //  12 Enable Configurable Virtual Wire Index/Data Received Register.
  //  13 Set eSPI Bus Master Enable.

  //  eSPIx00[5:4, SecondaryNSelect] = 00b - Secondary 0 selected.
  //  Make sure the following operation is for secondary 0
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x00), ~(BIT_32(4) + BIT_32(5)), 0);                // Select Secondary0

  FchinitHSEspiTimer (EspiBase);                                                            // 1 - 2

  //FchinitEspiClockGating (0, EspiBase);                                                 // 3
  //FchinitEspiErrCmdInterrupt (0, EspiBase);                                             // 4 - 8
  //FchinitEspiDecoding (0, EspiBase);                                                    // 9 - 10
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x44), 0x00, 0x004E004F);                               // IO range
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x48), 0x00, 0x004C0600);                               // IO range
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x4C), 0x00, 0x00FF0000);                               // IO range size
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), 0xFFFFF0FF, 0x00000F00);                         // Enable IO range
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x40), ~BIT_32(1), BIT_32(1));                            // 60h/64h                                                      // 11 - 12
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0xA4), 0x00, 0x00040506);
  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0xA8), 0x00, 0x00000007);

  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x34), ~BIT_32(1), BIT_32(1));                            // BUS_MASTER_EN

  xUSLMemReadModifyWrite32 ((void *)(size_t)(EspiBase + 0x6C), 0x00, 0xFFFFFFFF);                               // Enable interrupt at last - CZ

  FchinitHSEspiSecondary0Device (EspiBase);
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**
 * FchinitEspiIoMux - Config ESPI controller
 *
 *
 *
 */
static void FchinitHSEspiIoMux (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x75), 0x00, 0x00); //ESPI_CLK
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x77), 0x00, 0x01); //ESPI_CS
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x78), 0x00, 0x00); //ESPI_D1
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x79), 0x00, 0x00); //ESPI_D0
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x7A), 0x00, 0x00); //ESPI_D2
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x85), 0x00, 0x00); //ESPI_D3

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**
 * FchEspiPrePcieInit
 * @brief Config ESPI controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
FchEspiPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  if (FchDataPtr->EspiEnable) {
    FchAoacPowerOnDev (FCH_AOAC_ESPI, 1);
    FchinitHSEspiIoMux ();
  } else {
    FchAoacPowerOnDev (FCH_AOAC_ESPI, 0);
    return;
  }

  if (FchDataPtr->EspiEc0Enable) {
    FchinitHSEspiEc0 ();
  }

  if (FchDataPtr->EspiIo80Enable) {
    FchinitHSEspiEnableIo80 (true);
  }

  if (FchDataPtr->EspiKbc6064Enable) {
    FchinitHSEspiEnableKbc6064 (true);
  } else {
    FchinitHSEspiEnableKbc6064 (false);
  }

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**
 * InitializeFchEspi
 * @brief Config Espi controller during Pre-Pcie phase
 *
 * @return SIL_STATUS
 */
SIL_STATUS InitializeFchEspi (void) {
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to Fch input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) SilFindStructure (SilId_FchClass, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH found blk at: 0x%x \n", LclInpFchBlk);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchEspiPrePcieInit(LclInpFchBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
