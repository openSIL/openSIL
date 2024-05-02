/**
 * @file  FchHwAcpi.c
 * @brief FCH HW ACPI data and functions
 *
 */
/* Copyright 2021-2024 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include <FCH/Common/FchCommonCfg.h>
#include "FchHwAcpi.h"
#include <FCH/Common/FchCore/FchAb/FchAb.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchHelper.h>
#include <CommonLib/Io.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>


#define RTC_WORKAROUND_SECOND           0x00
#define RTC_VALID_SECOND_VALUE          0x59
#define RTC_SECOND_RESET_VALUE          0x30
#define RTC_SECOND_LOWER_NIBBLE         0x0F
#define RTC_VALID_SECOND_VALUE_LN       0x09

static FCHHWACPI_INPUT_BLK mFchHwAcpiDefaults = {
  .Smbus0BaseAddress      = 0xB00,
  .SioPmeBaseAddress      = 0xE00,
  .WatchDogTimerBase      = 0xFEB00000,
  .AcpiPm1EvtBlkAddr      = 0x400,
  .AcpiPm1CntBlkAddr      = 0x404,
  .AcpiPmTmrBlkAddr       = 0x408,
  .CpuControlBlkAddr      = 0x410,
  .AcpiGpe0BlkAddr        = 0x420,
  .SmiCmdPortAddr         = 0x00B0,
  .AcpiPmaCntBlkAddr      = 0xFE00,
  .SpreadSpectrum         = true,
  .PwrFailShadow          = AlwaysOff,
  .StressResetMode        = 0,
  .OemProgrammingTablePtr = NULL,
  .SpreadSpectrumOptions  = 1,
  .NoClearThermalTripSts  = false,
  .FchAcpiMmioBase        = 0xFED80000,
  .FchAlinkRasSupport     = false,
  .I2c0SdaHold            = 1,
  .I2c1SdaHold            = 1,
  .I2c2SdaHold            = 1,
  .I2c3SdaHold            = 1,
  .I2c4SdaHold            = 1,
  .I2c5SdaHold            = 1,
  .FchAoacProgramEnable   = true,
  .SpdHostCtrlRelease     = true,
  .DimmTelemetry          = false,
  .FchModernStandby       = false,
  .BootTimerEnable        = true,
  .BootTimerResetType     = WarmReset,
  .ToggleAllPwrGoodOnCf9  = false,
};

/**-----------------------------------------------------------------------------
 * Name: GetFchHwAcpiData
 *
 * @brief Get Fch HwAcpi Data
 *
 * @param None
 *
 * @retval pointer to FCHHWACPI_INPUT_BLK.
*/
FCHHWACPI_INPUT_BLK*
GetFchHwAcpiData (void)
{
  return &mFchHwAcpiDefaults;
}

/**
 * FchInitHwAcpiResetPreliminaryMmioTable - Fch ACPI MMIO initial (Preliminary)
 * during the power on stage. No need to re-run in S3.
 *
 *
 *
 *
 */
static const ACPI_REG_WRITE FchInitHwAcpiResetPreliminaryMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  {PMIO_BASE >> 8,  FCH_PMIOA_REG04, 0xFD, BIT_32(1)},          /// Enabled (Mmio_mem_enable)
  {AOAC_BASE >> 8,  FCH_AOAC_REG94 + 1, 0x3F, 0x00},            /// S0I3CTRL_ARBITERDIS + S0I3CTRL_INTERRUPTDIS
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08 + 2, 0xEF, BIT_32(4)},      /// shutdownoption
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC8, 0xEF, 0x00},               /// Clear UseAcpiStraps
  {PMIO_BASE >> 8,  FCH_PMIOA_REGD2, 0xBF, 0x00},               /// Enable CF9
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC0, 0x00, BIT_32(1)},          /// Clear 4s shutdown event status

  {MISC_BASE >> 8,  FCH_MISC_REG50 + 2, 0xEF, BIT_32(4)},       /// MISC 0x50 BIT_32(20)
  {MISC_BASE >> 8,  FCH_MISC_REG50 + 2, 0xEF, 0x00},            /// MISC 0x50 BIT_32(20)

  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchInitResetAcpiMmioTable - Fch ACPI MMIO initial
 * during the power on stage.
 *
 *
 *
 *
 */
static const ACPI_REG_WRITE FchInitResetAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                           /// Signature

  //{PMIO_BASE >> 8,  FCH_PMIOA_REG00 + 2, 0xFB, BIT_32(2)},        // Move ASF Clk switch code to PEI
  //{PMIO_BASE >> 8,  FCH_PMIOA_REGD2, 0xCF, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG00, 0xF7, 0x77},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG08, 0xFE, BIT_32(2) + BIT_32(4)},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG10, 0xFC, 0x00},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54, 0x00, BIT_32(4) + BIT_32(6)},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG74, 0xF6, BIT_32(0)},
  //{PMIO_BASE >> 8,  FCH_PMIOA_REGC4, 0xEE, 0x04},                  /// Release NB_PCIE_RST
  //{PMIO_BASE >> 8,  FCH_PMIOA_REGC0 + 2, 0xBF, 0x40},
  //{PMIO_BASE >> 8,  FCH_PMIOA_REGC4, 0xFB, 0},

  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchInitEnvHwAcpiMmioTable - Fch ACPI MMIO initial
 * during POST.
 *
 */
static const ACPI_REG_WRITE FchInitEnvHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  {PMIO_BASE >> 8, FCH_PMIOA_REG74, 0x00, BIT_32(0) + BIT_32(1) + BIT_32(2) + BIT_32(4)},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 2, 0x00, BIT_32(1) + BIT_32(2)},             //en_shutdown_msg, en_sync_flood
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBA, 0xF7, BIT_32(3)},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBC, 0xFD, BIT_32(1)},

  {SMI_BASE >> 8, FCH_SMI_Gevent1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_Gevent3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_Gevent4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_Gevent5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_Gevent6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_Gevent23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_Gevent8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_REG08,  0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_REG0C + 2, 0xF7, BIT_32(3)},
  {SMI_BASE >> 8, FCH_SMI_HS_TWRN, 0, 9},
  {SMI_BASE >> 8, FCH_SMI_HS_FANTHGEVENT, 0, 13},
  {SMI_BASE >> 8, FCH_SMI_HS_NBGPPPME, 0, 16},
  {SMI_BASE >> 8, FCH_SMI_HS_NBGPPHP, 0, 17},
  {SMI_BASE >> 8, FCH_SMI_HS_XHC0WAKE, 0, 0xB},
  {SMI_BASE >> 8, FCH_SMI_HS_XHC1WAKE, 0, 0xB},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchInitHwAcpiPciTable - PCI device registers
 * initial during early POST.
 *
 */
static const REG8_MASK FchInitHwAcpiPciTable[] =
{
  //
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  //
  {0x00, SMBUS_BUS_DEV_FUN, 0},

  {0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE FchInitEnvRasMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {SMI_BASE  >> 8,  FCH_SMI_REGB4 + 1, 0xFC, BIT_32(0)},         // Enable SERR SMI
  {0xFF, 0xFF, 0xFF, 0xFF},
};

///
/// PCI_IRQ_REG_BLOCK- FCH PCI IRQ registers block
///
typedef struct _PCI_IRQ_REG_BLOCK {
  uint8_t                PciIrqIndex;       // PciIrqIndex - selects which PCI interrupt to map
  uint8_t                PciIrqData;        // PciIrqData  - Interrupt #
} PCI_IRQ_REG_BLOCK;

static PCI_IRQ_REG_BLOCK FchInternalDeviceIrqForApicMode[] = {
    { (FCH_HS_IRQ_INTA | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_HS_IRQ_INTB | FCH_IRQ_IOAPIC), 0x11},
    { (FCH_HS_IRQ_INTC | FCH_IRQ_IOAPIC), 0x12},
    { (FCH_HS_IRQ_INTD | FCH_IRQ_IOAPIC), 0x13},
    { (FCH_HS_IRQ_SD | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_HS_IRQ_SDIO | FCH_IRQ_IOAPIC), 0x10},
    { (FCH_HS_IRQ_EMMC | FCH_IRQ_IOAPIC), 0x5},
    { (FCH_HS_IRQ_GPIO | FCH_IRQ_IOAPIC), 0x7},
    { (FCH_HS_IRQ_UART0 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_HS_IRQ_UART1 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_HS_IRQ_UART2 | FCH_IRQ_IOAPIC), 0x3},
    { (FCH_HS_IRQ_UART3 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_HS_IRQ_I2C0 | FCH_IRQ_IOAPIC), 0xa},
    { (FCH_HS_IRQ_I2C1 | FCH_IRQ_IOAPIC), 0xb},
    { (FCH_HS_IRQ_I2C2 | FCH_IRQ_IOAPIC), 0x4},
    { (FCH_HS_IRQ_I2C3 | FCH_IRQ_IOAPIC), 0x6},
    { (FCH_HS_IRQ_I2C4 | FCH_IRQ_IOAPIC), 0xe},
    { (FCH_HS_IRQ_I2C5 | FCH_IRQ_IOAPIC), 0xf},
  };
#define NUM_OF_DEVICE_FOR_APICIRQ  (sizeof (FchInternalDeviceIrqForApicMode) / sizeof (PCI_IRQ_REG_BLOCK))

/*********************************************************************************************************************/
/*                                       FCH 8.0 (Genoa) IP Table                                                    */
/*********************************************************************************************************************/

/**
 * FchGetAcpiMmioBase
 *
 * Returns ACPI MMIO base address.
 *
 * @param[in] AcpiMmioBase  - ACPI MMIO Base
 *
 */
void FchGetAcpiMmioBase (uint32_t *AcpiMmioBase)
{
  *AcpiMmioBase = mFchHwAcpiDefaults.FchAcpiMmioBase;
}

/**
 * ProgramFchAcpiMmioTbl - Program FCH ACPI MMIO register by table (8 bits data)
 *
 *
 *
 * @param[in] AcpiTbl   - Table data pointer
 *
 */
void ProgramFchAcpiMmioTbl (ACPI_REG_WRITE *AcpiTbl)
{
  uint8_t  Or8;
  uint8_t  Mask8;
  uint32_t Var;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (AcpiTbl != NULL) {
    if ((AcpiTbl->MmioReg == 0) && (AcpiTbl->MmioBase == 0) && (AcpiTbl->DataAndMask == 0xB0) && (AcpiTbl->DataOrMask == 0xAC)) {
      // Signature Checking
      AcpiTbl++;
      while (AcpiTbl->MmioBase < 0x1D) {
        Var = ACPI_MMIO_BASE | (AcpiTbl->MmioBase) << 8 | AcpiTbl->MmioReg;
        Or8 = AcpiTbl->DataOrMask;
        Mask8 = AcpiTbl->DataAndMask;
        xUSLMemReadModifyWrite8((void *)(size_t)Var, Mask8, Or8);
        AcpiTbl++;
      }
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * ProgramResetRtcExt - Config RTC External registers
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void ProgramResetRtcExt (void)
{

}

/**
 * ProgramFchSciMapTbl
 * @brief Program FCH SCI Map table (8 bits data)
 *
 *
 *
 * @param[in] SciMapTbl   - Table data pointer
 * @param[in] FchResetDataBlock
 *
 */
static void ProgramFchSciMapTbl (SCI_MAP_CONTROL *SciMapTbl)
{
  uint32_t  ddtempVar;
   FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SciMapTbl != NULL) {
    while (SciMapTbl->InputPin != 0xFF) {
      if ((SciMapTbl->InputPin >= 0x40) && (SciMapTbl->InputPin < 0x80) && (SciMapTbl->GpeMap < 0x20)) {
        ddtempVar = ACPI_MMIO_BASE | SMI_BASE | SciMapTbl->InputPin;
        xUSLMemWrite8 ((void*)(size_t)ddtempVar, (uint8_t)SciMapTbl->GpeMap);
      } else {
        //Assert Warning "SCI map is invalid"
      }
      SciMapTbl++;
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * ProgramCpuRstBTmr - Config RESET_L time
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void ProgramCpuRstBTmr (void)
{
  uint32_t        RegEbx;
  uint8_t         PkgType;

  RegEbx = 0;
  PkgType = (uint8_t) (RegEbx >> 28);
  if (PkgType == 0x4) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG20 + 2), 0, 0xFF);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG20 + 3), 0xF8, 0x07);
  }
}

static void FchInitEnableWdt (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00), 0x7F, FchDataPtr->WdtEnable ? BIT_32(7) : 0);
}

void FchInitEnableBootTimer (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  xUSLMemReadModifyWrite8(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG44 + 3),
    0x7F,
    FchHwAcpi->BootTimerEnable ? 0x88 : 0x80
    );
}


/************************************************************ ENV INIT P *************************************/

/**
 * IsExternalClockMode - Is External Clock Mode?
 *
 *
 * @retval  true or false
 *
 */
bool IsExternalClockMode (void)
{
  uint8_t    MISC80;
  MISC80 = xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80 + 2));
  return ( (bool) ((MISC80 & BIT_32(1)) == 0) );
}

/**
 * ProgramFchEnvSpreadSpectrum
 * @brief Config SpreadSpectrum before PCI emulation
 *
 * @param[in] FchHwAcpi Fch Acpi configuration structure pointer.
 *
 */
void ProgramFchEnvSpreadSpectrum (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  uint8_t FchSpreadSpectrum;

  FchSpreadSpectrum = FchHwAcpi->SpreadSpectrum;

  if (IsExternalClockMode ()) {
    FchSpreadSpectrum = 0;
  }

  if (FchSpreadSpectrum) {
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x10), ~BIT_32(29), BIT_32(29));
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x08), ~BIT_32(0), BIT_32(0));
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x40), ~BIT_32(30), BIT_32(30));
  } else {
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x10), ~BIT_32(29), BIT_32(29));
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x08), ~BIT_32(0), 0x00);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x40), ~BIT_32(30), BIT_32(30));
  }
}

static void ClearThermalTripSts (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  if (FchHwAcpi->NoClearThermalTripSts == false) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC0), 0, BIT_32(0));
  }
}

/************************************************************ ENV INIT *************************************/

/**
 * FchI2cUartInit
 * @brief Config Fch AMBA I2C Uart init
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static void FchI2cUartInit (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  uint32_t FchDeviceEnMap;
  FchDeviceEnMap = FchDataPtr->FchRunTime.FchDeviceEnableMap;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // I2C0
  //
  if ( FchDeviceEnMap & BIT_32(5) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C0, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x91), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x92), 0, 0x0);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C0_BASE + 0x7C), 0, FchHwAcpi->I2c0SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C0, 0);
  }

  //
  // I2C1
  //
  if ( FchDeviceEnMap & BIT_32(6) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C1, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x93), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x94), 0, 0x0);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C1_BASE + 0x7C), 0, FchHwAcpi->I2c1SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C1, 0);
  }

  //
  // I2C2
  //
  if ( FchDeviceEnMap & BIT_32(7) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C2, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x71), 0, 0x1);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x72), 0, 0x1);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C2_BASE + 0x7C), 0, FchHwAcpi->I2c2SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C2, 0);
  }
  //
  // I2C3
  //
  if ( FchDeviceEnMap & BIT_32(8) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C3, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x13), 0, 0x1);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x14), 0, 0x1);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C3_BASE + 0x7C), 0, FchHwAcpi->I2c3SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C3, 0);
  }

  //
  // I2C4
  //
  if ( FchDeviceEnMap & BIT_32(9) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C4, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x95), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x96), 0, 0x0);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C4_BASE + 0x7C), 0, FchHwAcpi->I2c4SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C4, 0);
  }

  //
  // I2C5
  //
  if ( FchDeviceEnMap & BIT_32(10) ) {
    FchAoacPowerOnDev (FCH_AOAC_I2C5, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x97), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x98), 0, 0x0);
    // SDA_HOLD
    xUSLMemReadModifyWrite32 ((void *)(size_t)(FCH_I2C5_BASE + 0x7C), 0, FchHwAcpi->I2c5SdaHold);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_I2C5, 0);
  }

  //
  // UART0
  //
  if ( FchDeviceEnMap & BIT_32(11) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART0, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x87), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x88), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x89), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8A), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8B), 0, 0x0);

    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_21C_UART0_CTS_L_UART2_RXD_EGPIO135),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16), 0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_220_UART0_RXD_EGPIO136),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16), 0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_224_UART0_RTS_L_UART2_TXD_EGPIO137),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16), 0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_228_UART0_TXD_EGPIO138),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16), 0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_22C_UART0_INTR_AGPIO139),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16), 0);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART0, 0);
  }

  //
  // UART1
  //
  if ( FchDeviceEnMap & BIT_32(12) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART1, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8C), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8D), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8E), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8F), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x90), 0, 0x0);

    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_234_UART1_RXD_EGPIO141),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_238_UART1_RTS_L_UART3_RXD_EGPIO142),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),0);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART1, 0);
  }

  //
  // UART2
  //
  if ( FchDeviceEnMap & BIT_32(16) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART2, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x87), 0, 0x1);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x89), 0, 0x1);

    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_21C_UART0_CTS_L_UART2_RXD_EGPIO135),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),0);
    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_224_UART0_RTS_L_UART2_TXD_EGPIO137),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),0);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART2, 0);
  }
  //
  // UART3
  //
  if ( FchDeviceEnMap & BIT_32(26) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART3, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8C), 0, 0x1);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8E), 0, 0x1);

    xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_238_UART1_RTS_L_UART3_RXD_EGPIO142),
                              (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),0);
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART3, 0);
  }

  //
  // PSP_INTR0
  //
  // IOMUX
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x59), 0, 0x1);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchAlinkRasEnable
 * @brief Enable FCH A-Link parity error
 *
 *
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static void FchAlinkRasEnable (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (FchHwAcpi->FchAlinkRasSupport) {
    RwAlink (FCH_ABCFG_REG10050 | (uint32_t) (ABCFG << 29), ~BIT_32(3), BIT_32(3));
    ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitEnvRasMmioTable[0]));
  } else {
    RwAlink (FCH_ABCFG_REG10050 | (uint32_t) (ABCFG << 29), ~BIT_32(3), 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * HpetInit - Program Fch HPET function
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void HpetInit (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  bool               FchHpetTimer;
  bool               FchHpetMsiDis;

  FchHpetTimer = FchDataPtr->Hpet.HpetEnable;
  FchHpetMsiDis = FchDataPtr->Hpet.HpetMsiDis;

  if (FchHpetTimer == true) {
    //
    //Enabling decoding of HPET MMIOProgram the HPET BAR address
    //
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00), 0xFF, BIT_32(6));

    //
    //Enable HPET MSI support
    //
    if ( FchHpetMsiDis == false ) {
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00), ~ BIT_32(29), BIT_32(29));
#ifdef FCH_TIMER_TICK_INTERVAL_WA
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00), ~ BIT_32(29), 0);
#endif
    } else {
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00), ~ BIT_32(29), 0);
    }

  }
}

/** AcLossControl - Reserved **/
static void
AcLossControl (uint8_t AcLossControlValue)
{
  AcLossControlValue &= 0x03;
  AcLossControlValue |= BIT_32(2);
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B), 0xF0, AcLossControlValue);
}

/**
 * ProgramPcieNativeMode - Config Pcie Native Mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void ProgramPcieNativeMode (void *FchDataPtr)
{
  uint8_t        FchNativepciesupport;
  FCHCLASS_INPUT_BLK         *LocalCfgPtr;

  LocalCfgPtr = (FCHCLASS_INPUT_BLK *) FchDataPtr;
  FchNativepciesupport = (uint8_t) LocalCfgPtr->Misc.NativePcieSupport;

  //
  // PCIE Native setting
  //
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBA + 1), (uint8_t)~BIT_8(6), 0);
  if ( FchNativepciesupport == 1) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3),
            (uint8_t)~(BIT_8(3) + BIT_8(1) + BIT_8(0)), (BIT_8(3) + BIT_8(0)));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG74 + 3),
            (uint8_t)~(BIT_8(3) + BIT_8(1) + BIT_8(0)), BIT_8(3));
  }
}

/**
 * ProgramFchEnvHwAcpiPciReg - Config HwAcpi PCI controller
 * before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void ProgramFchEnvHwAcpiPciReg(void *FchDataPtr)
{
  FCHCLASS_INPUT_BLK         *LocalCfgPtr;

  LocalCfgPtr = (FCHCLASS_INPUT_BLK *) FchDataPtr;

  //
  //Early post initialization of pci config space
  //
  ProgramPciByteTable ((REG8_MASK*) (&FchInitHwAcpiPciTable[0]),
                          sizeof (FchInitHwAcpiPciTable) / sizeof (REG8_MASK));

  if ( LocalCfgPtr->Smbus.SmbusSsid != 0 ) {
    xUSLPciReadModifyWrite32 (PCI_LIB_ADDRESS(FCH_ISA_BUS, FCH_ISA_DEV, FCH_ISA_FUNC, FCH_CFG_REG2C),
                              0, LocalCfgPtr->Smbus.SmbusSsid);
  }
  if ( LocalCfgPtr->Misc.NoneSioKbcSupport ) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED),
                              (uint8_t)~BIT_8(1), BIT_8(1));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED),
                            (uint8_t)~BIT_8(1), 0);
  }
  ProgramPcieNativeMode (FchDataPtr);
}

/*************************** LATE INIT ****************************/

/**
 * FchCppcSciInit
 * @brief Init SCI for CPPC feature
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchCppcSciInit (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  uint32_t SciTrig;
  uint32_t SciLevl;
  uint32_t SwSciEn;
  uint32_t EventEn;

  if (FchDataPtr->Misc.Cppc.CppcSupport) {
    if (FchDataPtr->Misc.Cppc.SciBit){
      SciLevl  = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C));
      SciLevl |= FchDataPtr->Misc.Cppc.SciBit;   //Level trigger
      xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C), SciLevl);

      SciTrig  = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG08));
      SciTrig |= FchDataPtr->Misc.Cppc.SciBit;   //Active high
      xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG08), SciTrig);

      SwSciEn  = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG18));
      SwSciEn |= FchDataPtr->Misc.Cppc.SciBit;   //Enable SCI
      xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG18), SwSciEn);

      EventEn  = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04));
      EventEn |= FchDataPtr->Misc.Cppc.SciBit;   //Event Enable
      xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04), EventEn);
    }
  }
}

/**
 * FchEventInitUsbGpe - Init Gevent that mapped to Usb Pme
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
static void
FchEventInitUsbGpe(void)
{
  uint8_t                  UsbGpe0Num = 0, UsbGpe1Num = 0;
  uint32_t                 SciLevl32;

  SciLevl32  = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C));

  if (UsbGpe0Num != 0) {
    /* coverity[dead_error_begin] */
    UsbGpe0Num &= 0x1F;
    SciLevl32 |= (uint32_t) (1 << UsbGpe0Num);
  }

  if (UsbGpe1Num != 0) {
    /* coverity[dead_error_begin] */
    UsbGpe1Num &= 0x1F;
    SciLevl32 |= (uint32_t) (1 << UsbGpe1Num);
  }

  xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG0C), SciLevl32);
}

static void
FchModifyDeviceIrq (
      void     *FchDataPtr
    )
{
  FCHCLASS_INPUT_BLK         *LocalCfgPtr;
  uint8_t                  i;

  LocalCfgPtr = (FCHCLASS_INPUT_BLK *) FchDataPtr;
  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    switch (FchInternalDeviceIrqForApicMode[i].PciIrqIndex){
      case (FCH_HS_IRQ_UART0 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart0Irq;
        break;
      case (FCH_HS_IRQ_UART1 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart1Irq;
        break;
      case (FCH_HS_IRQ_UART2 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart2Irq;
        break;
      case (FCH_HS_IRQ_UART3 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.Uart3Irq;
        break;
      case (FCH_HS_IRQ_I2C0 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c0Irq;
        break;
      case (FCH_HS_IRQ_I2C1 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c1Irq;
        break;
      case (FCH_HS_IRQ_I2C2 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c2Irq;
        break;
      case (FCH_HS_IRQ_I2C3 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c3Irq;
        break;
      case (FCH_HS_IRQ_I2C4 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c4Irq;
        break;
      case (FCH_HS_IRQ_I2C5 | FCH_IRQ_IOAPIC):
        FchInternalDeviceIrqForApicMode[i].PciIrqData = LocalCfgPtr->FchRunTime.I2c5Irq;
        break;
      default:break;
    }
  }
}

/**
 * FchInternalDeviceIrqInit - Config Fch internal Device Interrupt routing
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
FchInternalDeviceIrqInit (
    void     *FchDataPtr
  )
{
  uint8_t                  i;

  FchModifyDeviceIrq(FchDataPtr);

  for (i = 0; i < NUM_OF_DEVICE_FOR_APICIRQ; i++) {
    xUSLIoWrite8 ( FCH_IOMAP_REGC00, FchInternalDeviceIrqForApicMode[i].PciIrqIndex);
    xUSLIoWrite8 ( FCH_IOMAP_REGC01, FchInternalDeviceIrqForApicMode[i].PciIrqData);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * SbSleepTrapControl - SB Sleep Trap Control
 *
 *
 *
 * @param[in] SleepTrap    - Whether sleep trap is enabled
 *
 */
static void
SbSleepTrapControl (
          bool          SleepTrap
  )
{
  if (SleepTrap) {
    xUSLMemReadModifyWrite32 ((void*)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0),
                              ~(BIT_32(2) + BIT_32(3)), BIT_32(2));
    xUSLMemReadModifyWrite8 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE),
                              (uint8_t)~BIT_8(5), 0x00);
    xUSLMemReadModifyWrite8 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG08 + 3),
                              (uint8_t)~(BIT_8(0) + BIT_8(1)), BIT_8(1));
  } else {
    xUSLMemReadModifyWrite8 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE),
                              (uint8_t)~BIT_8(5), BIT_8(5));
    xUSLMemReadModifyWrite8 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG08 + 3),
                            (uint8_t)~(BIT_8(0) + BIT_8(1)), BIT_8(0));
    xUSLMemReadModifyWrite32 ((void*)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0),
                            ~(BIT_32(2) + BIT_32(3)), 0);
  }
}

/**
 * StressResetModeLate - Stress Reset Mode
 *
 *
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static void StressResetModeLate (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  uint8_t ResetValue;

  switch (FchHwAcpi->StressResetMode) {
  case 0:
    return;
  case 1:
    ResetValue = FCH_KBC_RESET_COMMAND;
    xUSLIoWrite8 (FCH_KBDRST_BASE_IO, ResetValue);
    break;
  case 2:
    ResetValue = FCH_PCI_RESET_COMMAND06;
    xUSLIoWrite8 (FCH_PCIRST_BASE_IO, ResetValue);
    break;
  case 3:
    ResetValue = FCH_PCI_RESET_COMMAND0E;
    xUSLIoWrite8 (FCH_PCIRST_BASE_IO, ResetValue);
    break;
  case 4:
    FchHwAcpi->StressResetMode = 3;
    return;
  default:
    //ASSERT (false);
    return;
  }
  while (FchHwAcpi->StressResetMode) {
  }
}

/**
 * GcpuRelatedSetting - Program Gcpu C related function
 *
 *
 *
 * @param[in] FchDataPtr   Fch configuration structure pointer.
 *
 */
static void GcpuRelatedSetting (void *FchDataPtr) {
  uint8_t  FchAcDcMsg;
  uint8_t  FchTimerTickTrack;
  uint8_t  FchClockInterruptTag;
  uint8_t  FchOhciTrafficHanding;
  uint8_t  FchEhciTrafficHanding;
  uint8_t  FchGcpuMsgCMultiCore;
  uint8_t  FchGcpuMsgCStage;
  uint32_t Value;
  FCHCLASS_INPUT_BLK *LocalCfgPtr;

  LocalCfgPtr = (FCHCLASS_INPUT_BLK *) FchDataPtr;

  FchAcDcMsg = (uint8_t) LocalCfgPtr->Gcpu.AcDcMsg;
  FchTimerTickTrack = (uint8_t) LocalCfgPtr->Gcpu.TimerTickTrack;
  FchClockInterruptTag = (uint8_t) LocalCfgPtr->Gcpu.ClockInterruptTag;
  FchOhciTrafficHanding = (uint8_t) LocalCfgPtr->Gcpu.OhciTrafficHanding;
  FchEhciTrafficHanding = (uint8_t) LocalCfgPtr->Gcpu.EhciTrafficHanding;
  FchGcpuMsgCMultiCore = (uint8_t) LocalCfgPtr->Gcpu.GcpuMsgCMultiCore;
  FchGcpuMsgCStage = (uint8_t) LocalCfgPtr->Gcpu.GcpuMsgCStage;

  Value = xUSLMemRead32 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA0));
  Value = Value & 0xC07F00A0;

  if ( FchAcDcMsg ) {
    Value = Value | BIT_32(0);
  }

  if ( FchTimerTickTrack ) {
    Value = Value | BIT_32(1);
  }

  if ( FchClockInterruptTag ) {
    Value = Value | BIT_32(10);
  }

  if ( FchOhciTrafficHanding ) {
    Value = Value | BIT_32(13);
  }

  if ( FchEhciTrafficHanding ) {
    Value = Value | BIT_32(15);
  }

  if ( FchGcpuMsgCMultiCore ) {
    Value = Value | BIT_32(23);
  }

  if ( FchGcpuMsgCStage ) {
    Value = (Value | (BIT_32(6) + BIT_32(4) + BIT_32(3) + BIT_32(2)));
  }

  xUSLMemWrite32 ((void*)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGA0), Value);
}

/**
 * FchInitLateHwAcp
 * @brief Prepare HwAcpi controller to boot to OS.
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void FchInitLateHwAcpi (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  GcpuRelatedSetting (FchDataPtr);

  StressResetModeLate (FchHwAcpi);
  SbSleepTrapControl (true);
  FchInternalDeviceIrqInit (FchDataPtr);
  FchEventInitUsbGpe ();
  FchCppcSciInit(FchDataPtr);
}

/**
 * FchInitHwAcpiEnableBase
 * @brief Enable Base Address in ACPI/SmBus module during Power-On
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchInitHwAcpiEnableBase (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  uint16_t       SmbusBase;

  //
  // Enabled Base Address
  //
  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00),
    0x00FF,
    SmbusBase + BIT_32(4)
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPm1EvtBlkAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPm1CntBlkAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPmTmrBlkAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG66),
    0x0000,
    FchDataPtr->FchBldCfg.CfgCpuControlBlkAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG68),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiGpe0BlkAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A),
    0x0000,
    FchDataPtr->FchBldCfg.CfgSmiCmdPortAddr
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6E),
    0x0000,
    FchDataPtr->FchBldCfg.CfgSmiCmdPortAddr + 8
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6C),
    0x0000,
    0xFFFF
    );

}

/**
 * FchInitEnableSmBus
 * @brief Enable SmBus during Power-On
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchInitEnableSmBus (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  uint16_t       SmbusBase;
  uint8_t        Value;

  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  //
  // SmBus init
  //
  xUSLMemReadModifyWrite32(
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00),
    ~(BIT_32(19) + BIT_32(20)),
    0
    );
  Value = 0x00;
  xUSLIoWrite8(SmbusBase + 0x14, Value);
}

/**
 * FchInitHwAcpiWaFeature
 * @brief common configuration and Workaround on ACPI module
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchInitHwAcpiWaFeature (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  uint16_t       SmbusBase;
  uint8_t        Value;
  uint16_t       AsfPort;
  uint32_t       GeventEnableBits;
  uint32_t       GeventValue;
  uint32_t       ApicIdValue;
  uint32_t       ApicReg;
  uint32_t       ApicIndexBackup;

  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  //
  // RTC Workaround for Daylight saving time enable bit
  //
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E), 0, 0);
  xUSLMemReadModifyWrite8 (
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F),
    0xFE,
    BIT_32(0)
    );   // Enable DltSavEnable
  Value = 0x0B;
  xUSLIoWrite8 (FCH_IOMAP_REG70, Value);
  Value = xUSLIoRead8(FCH_IOMAP_REG71);
  Value &= 0xFE;
  xUSLIoWrite8 (FCH_IOMAP_REG71, Value);
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5E), 0, 0);
  xUSLMemReadModifyWrite8 (
    (void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5F),
    0xFE,
    0
    );   // Enable DltSavEnable
  //
  // Prevent RTC error
  //
  Value = 0x0A;
  xUSLIoWrite8 (FCH_IOMAP_REG70, Value);
  Value = xUSLIoRead8(FCH_IOMAP_REG71);
  Value &= 0xEF;
  xUSLIoWrite8 (FCH_IOMAP_REG71, Value);

  if (FchDataPtr->FchBldCfg.CfgFchRtcWorkAround) {
    Value = RTC_WORKAROUND_SECOND;
    xUSLIoWrite8 (FCH_IOMAP_REG70, Value);
    Value = xUSLIoRead8(FCH_IOMAP_REG71);
    if ( Value > RTC_VALID_SECOND_VALUE ) {
      Value = RTC_SECOND_RESET_VALUE;
      xUSLIoWrite8 (FCH_IOMAP_REG71, Value);
    }
    Value = xUSLIoRead8(FCH_IOMAP_REG71);
    Value &= RTC_SECOND_LOWER_NIBBLE;
    if ( Value > RTC_VALID_SECOND_VALUE_LN ) {
      Value = xUSLIoRead8(FCH_IOMAP_REG71);
      Value = RTC_SECOND_RESET_VALUE;
      xUSLIoWrite8 (FCH_IOMAP_REG71, Value);
    }
  }

  //
  // Clear RTCD Date Alarm
  //
  Value = 0x0D;
  xUSLIoWrite8 (FCH_IOMAP_REG70, Value);
  Value = xUSLIoRead8(FCH_IOMAP_REG71);
  if ((Value & 0x3F) == 0x3F) {
    Value &= 0x80;
    xUSLIoWrite8 (FCH_IOMAP_REG71, Value);
  }

  AsfPort = SmbusBase + 0x20;
  if ( AsfPort != 0 ) {
    uint8_t  dbValue;
    dbValue = 0x2F;
    xUSLIoWrite8 (AsfPort + 0x0A, dbValue);
  }

  //
  // PciExpWakeStatus workaround
  //
  AsfPort = xUSLMemRead16 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60));
  AsfPort++;
  GeventEnableBits = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG04));
  GeventValue = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG00));
  if ( (GeventValue & GeventEnableBits) != 0 ) {
    Value = 0x40;
    xUSLIoWrite8 (AsfPort, Value);
  }
  Value = xUSLIoRead8(AsfPort);
  if ((Value & (BIT_32(2) + BIT_32(0))) != 0) {
    Value = 0x40;
    xUSLIoWrite8 (AsfPort, Value);
  }

  Value = 0x08;
  xUSLIoWrite8 (FCH_IOMAP_REGC00, Value);
  Value = xUSLIoRead8(FCH_IOMAP_REGC01);

  if (!FchDataPtr->EcKbd) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value | 0x0A;
  }
  xUSLIoWrite8 (FCH_IOMAP_REGC01, Value);

  Value = 0x09;
  xUSLIoWrite8 (FCH_IOMAP_REGC00, Value);
  Value = xUSLIoRead8(FCH_IOMAP_REGC01);
  if (!FchDataPtr->EcKbd) {
    //
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    //
    Value = Value & 0xF9;
  }

  Value = Value & 0x9F;
  Value = Value | BIT_32(4);
  xUSLIoWrite8 (FCH_IOMAP_REGC01, Value);

  if (FchDataPtr->FchOscout1ClkContinous) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG54), 0xBF, 0);
  }
  if (FchDataPtr->SerialIrqEnable) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG54), 0x7F, BIT_32(7));
  }

  //
  // Disable secondary ASF
  //
  if (FchDataPtr->FchAsfCfg.DisableSecondary) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + ASF_BASE + 0x15), 0xEF, BIT_32(4));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + ASF_BASE + 0x15), 0xEF, 0);
  }
  // FCH IOAPIC ID configuration

  if (FchDataPtr->CfgIoApicIdPreDefEnable) {

    ApicIdValue = (FchDataPtr->FchIoApicId << 24);
    ApicReg = FCH_IOAPIC_ID_REG;
    //Backup IOAPIC Index
    ApicIndexBackup = xUSLMemRead32 ((void *)(size_t)(FCH_IOAPIC_INDEX));
    //Program IOAPIC Index to IOAPIC ID REG
    xUSLMemWrite32 ((void *)(size_t)(FCH_IOAPIC_INDEX), ApicReg);
    xUSLMemWrite32 ((void *)(size_t)(FCH_IOAPIC_DATA), ApicIdValue);
    xUSLMemWrite32 ((void *)(size_t)(FCH_IOAPIC_INDEX), ApicIndexBackup);

    FCH_TRACEPOINT(SIL_TRACE_INFO, "FchIoapicValue Value 0x%x\n", ApicIdValue);
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  }
}

/**
 * FchInitHwAcpiPreliminaryPrePciCmn  - Init ACPI module (Preliminary) before Pcie enumeration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchInitHwAcpiPreliminaryPrePciCmn(
    FCHCLASS_INPUT_BLK *FchDataPtr,
    FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  UNUSED(FchDataPtr);
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitHwAcpiResetPreliminaryMmioTable[0]));

  ClearThermalTripSts (FchHwAcpi);
}

/**
 * FchInitHwAcpiPrePciCmn  - Init ACPI module before Pcie enumeration
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchInitHwAcpiPrePciCmn(FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  FchInitHwAcpiEnableBase (FchDataPtr);
  FchInitEnableSmBus (FchDataPtr);
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitResetAcpiMmioTable[0]));
  FchInitEnableWdt (FchDataPtr);
  ProgramResetRtcExt ();
  if (FchDataPtr->FchBldCfg.CfgFchSciMapControl != NULL) {
    ProgramFchSciMapTbl (FchDataPtr->FchBldCfg.CfgFchSciMapControl);
  }
  FchInitHwAcpiWaFeature (FchDataPtr);

  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchInitEnvHwAcpiMmioTable[0]));

  ProgramFchEnvHwAcpiPciReg (FchDataPtr);
  AcLossControl ((uint8_t) FchHwAcpi->PwrFailShadow);
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE *) (FchHwAcpi->OemProgrammingTablePtr));
  HpetInit (FchDataPtr);
  FchI2cUartInit (FchDataPtr, FchHwAcpi);
  FchAlinkRasEnable (FchHwAcpi);
}

/*********************************************************************************************************************/
/**************************************FCH HWACPI IP interface******************************************************/
/*********************************************************************************************************************/

/**
 * FchHwAcpiPreliminarySetInputBlk
 * @brief Establish FCH Spi input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchHwAcpiPreliminarySetInputBlk (void)
{
  FCHHWACPI_INPUT_BLK *FchHwAcpiPreliminaryInput;

  FchHwAcpiPreliminaryInput = (FCHHWACPI_INPUT_BLK *) SilCreateInfoBlock (SilId_FchHwAcpiP,
                                  sizeof(FCHHWACPI_INPUT_BLK),
                                  FCHHWACPI_INSTANCE,
                                  FCHHWACPI_MAJOR_REV,
                                  FCHHWACPI_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchHwAcpiPreliminarySetInputBlk at: 0x%x \n", FchHwAcpiPreliminaryInput);
  if (FchHwAcpiPreliminaryInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy ((void *)FchHwAcpiPreliminaryInput, &mFchHwAcpiDefaults, sizeof(FCHHWACPI_INPUT_BLK));

  return SilPass;
}

/**
 * FchHwAcpiPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param[in] FchDataBlock FCHCLASS_INPUT_BLK configuration structure pointer.
 *
 */
static void FchHwAcpiPrePcieInit (FCHCLASS_INPUT_BLK *FchDataBlock, FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;

  if (SilGetCommon2RevXferTable (SilId_FchHwAcpiP, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }
  FchInitHwAcpiPrePciCmn (FchDataBlock, FchHwAcpi);
  FchHwAcpiXfer->FchInitPrePcieHwAcpi(FchDataBlock, FchHwAcpi);
}

/**
 * FchHwAcpiPreliminaryPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param[in] FchDataBlock FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchEmmc FCHEMMC_INPUT_BLK configuration structure pointer.
 *
 */
static void FchHwAcpiPreliminaryPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;

  if (SilGetCommon2RevXferTable (SilId_FchHwAcpiP, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }
  FchInitHwAcpiPreliminaryPrePciCmn (FchDataBlock, FchHwAcpi);
  FchHwAcpiXfer->FchInitPrePcieHwAcpiP(FchDataBlock, FchHwAcpi);
}

/**
 * InitializeFchHwAcpiPreliminaryTp1
 * @brief Config Spi controller during Pre-Pcie phase
 *
 *
 */
SIL_STATUS InitializeFchHwAcpiPreliminaryTp1 (void)
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to Fch input blk
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk; //pointer to HwAcpi  input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) SilFindStructure (SilId_FchHwAcpiP, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);
  if (LclInpHwAcpiBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) SilFindStructure (SilId_FchClass, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH found blk at: 0x%x \n", LclInpFchBlk);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchHwAcpiPreliminaryPrePcieInit(LclInpFchBlk, LclInpHwAcpiBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiPreliminaryTp2
 *
 * @brief Timepoint 2
 *
 */
SIL_STATUS InitializeFchHwAcpiPreliminaryTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiPreliminaryTp3
 *
 * @brief Timepoint 2
 *
 */
SIL_STATUS InitializeFchHwAcpiPreliminaryTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiTp1
 *
 * @brief Config Spi controller during Pre-Pcie phase
 *
 */
SIL_STATUS
InitializeFchHwAcpiTp1 (void)
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to Fch input blk
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk; //pointer to HwAcpi  input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) SilFindStructure (SilId_FchHwAcpiP, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);
  if (LclInpHwAcpiBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) SilFindStructure (SilId_FchClass, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH HwAcpi found blk at: 0x%x \n", LclInpFchBlk);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchHwAcpiPrePcieInit(LclInpFchBlk, LclInpHwAcpiBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiTp2
 *
 * @brief Config FCH ACPI during timepoint 2
 *
 */
SIL_STATUS
InitializeFchHwAcpiTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiTp3
 *
 * @brief Config FCH ACPI during timepoint 3
 *
 */
SIL_STATUS
InitializeFchHwAcpiTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
