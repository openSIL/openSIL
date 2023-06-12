/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchEnvDef.c
 * @brief FCH HW data block defaults
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>

FCHCLASS_INPUT_BLK InitEnvCfgDefault =
{
  // FCH_RUNTIME
  .FchRunTime = {
    .PcieMmioBase = 0xF8000000,
    .FchDeviceEnableMap = 0,
    .FchDeviceD3ColdMap = 0,
    .Al2AhbLegacyUartIoEnable = 0xE400,
    .Uart0Irq = 0x03,
    .Uart1Irq = 0x04,
    .Uart2Irq = 0x03,
    .Uart3Irq = 0x04,
    .I2c0Irq = 0x0A,
    .I2c1Irq = 0x0B,
    .I2c2Irq = 0x04,
    .I2c3Irq = 0x06,
    .I2c4Irq = 0x0E,
    .I2c5Irq = 0x0F,
  },

  // FCH_SMBUS
  .Smbus = {
    .SmbusSsid = 0x00000000
  },

  // FCH_SD
  .Sd = {
    .SdConfig = _SdDisable,
    .SdSpeed = 0,
    .SdBitWidth = 0,
    .SdSsid = 0x00000000,
    .SdClockControl = _Sd50MhzTraceCableLengthWithinSixInches,
    .SdClockMultiplier = false,
    .SdReTuningMode = 0,
    .SdHostControllerVersion = 1,
    .SdrCapabilities = 3,
    .SdSlotType = 0,
    .SdForce18 = false,
    .SdDbgConfig = 0,
  },

  // FCH_HPET
  .Hpet = {
    .HpetEnable = true,
    .HpetMsiDis = true,
    .HpetBase = 0xFED00000
  },

  // FCH_GCPU
  .Gcpu = {
    .AcDcMsg = 0,
    .TimerTickTrack = 0
//    .ClockInterruptTag = 0
//    .OhciTrafficHanding = 0,
//    .EhciTrafficHanding = 0,
//    .GcpuMsgCMultiCore = 0,
//    .GcpuMsgCStage = 0,
  },

  // FCH_IMC
  .Imc = {
    .ImcEnable = 0,
    .ImcEnabled = 0,
    .ImcSureBootTimer = 0,
    .EcStruct = {0, },
    .ImcEnableOverWrite = 0,
  },

  // FCH_MISC
  .Misc = {
    .NativePcieSupport = false,
    .S3Resume = false,
    .RebootRequired = false,
    .FchVariant = 0,
    .Cg2Pll = 0,
    .LongTimer = { // TIMER_SMI-LongTimer
      .Enable = false,
      .StartNow = false,
      .CycleDuration = 1000
    },
    .ShortTimer = { // TIMER_SMI-ShortTimer
      .Enable = false,
      .StartNow = false,
      .CycleDuration = 0x7FFF
    },
    .FchCpuId = 0,
    .NoneSioKbcSupport = false,
    .FchCsSupport = { 0 },
    .Cppc = {
      false,
      BIT_32(24)
    },
    .FchiLa1MTraceMemoryEn = true, // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Enable
    .FchiLa1MTraceMemoryBase = 0,  // FchiLa1MTraceMemoryEn - Fch iLa 1M Trace Memory Base
  },

  .FchReset = { .UmiGen2 = true,
                .SataEnable = true,
                .IdeEnable = true,
                .Xhci0Enable = true,
                .Xhci1Enable = true, }, // FchReset
  .EcKbd = false,
  .LegacyFree = false,
  .SataSetMaxGen2 = false,
  .SataClkMode = 1,
  .SataModeReg = 0,
  .SataInternal100Spread = false,
  .SataActLWkaEnable = false,
  .FchOscout1ClkContinous = false,
  .LpcClockDriveStrength = 0,
  .LpcClockDriveStrengthRiseTime = 8,
  .LpcClockDriveStrengthFallTime = 8,
  .EspiEnable = false,
  .EspiIo80Enable = false,
  .EspiKbc6064Enable = false,
  .EspiEc0Enable = false,
  .WdtEnable = true,
  .FchAcpiMmioBase = 0xFED80000,
  .FchBldCfg = {
    .CfgSmbus0BaseAddress = 0xB00,
    .CfgSioPmeBaseAddress = 0xE00,
    .CfgAcpiPm1EvtBlkAddr = 0x400,
    .CfgAcpiPm1CntBlkAddr = 0x404,
    .CfgAcpiPmTmrBlkAddr = 0x408,
    .CfgCpuControlBlkAddr = 0x410,
    .CfgAcpiGpe0BlkAddr = 0x420,
  },
  .SerialIrqEnable = true,
  .FchAsfCfg = {
    .DisableMaster = false,
    .DisableSlave = false
  },
  .ToggleAllPwrGoodOnCf9 = false,
  .CfgIoApicIdPreDefEnable = false,
  .FchIoApicId = 0x00,
};
