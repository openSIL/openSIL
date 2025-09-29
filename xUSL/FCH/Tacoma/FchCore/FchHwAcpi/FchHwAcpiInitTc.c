/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiInitTc.c
 * @brief TC FCH HwAcpi Module silicon init functions
 *
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <FchHwAcpi-api.h>
#include <Fch.h>
#include <FchCommon.h>
#include <FchReg.h>
#include <Tacoma/Include/FchGPIORegsTc.h>
#include <FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>
#include <FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FchCore/FchHwAcpi/FchAoacLib.h>
#include "FchHwAcpiInitTc.h"
#include "FchHwAcpiCmn2Tc.h"

#define FCH_VERSION_TC                  0x0803

/**
 * FchTcInitEnvHwAcpiPciTable - PCI device registers
 * initial during early POST.
 *
 */
static const REG8_MASK FchTcInitEnvHwAcpiPciTable[] =
{
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {0x10, 0X00, (FCH_VERSION_TC & 0xFF)},                ///Program the version information
  {0x11, 0X00, (FCH_VERSION_TC >> 8)},
  {0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE MmioEnvInitTableTc[] = {
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG + 2, BIT_8(1) + BIT_8(2), 0},
  {SMI_BASE >> 8, FCHSMI_56, 0, 11},
  {SMI_BASE >> 8, FCHSMI_57, 0, 11},
  {SMI_BASE >> 8, FCHSMI_37, 0, 11},
  {SMI_BASE >> 8, FCHSMI_38, 0, 11},
  {SMI_BASE >> 8, FCHSMI_61, 0, 11},
  {SMI_BASE >> 8, FCHSMI_62, 0, 11},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchInitPreliminaryPrePcieHwAcpiTc
 *
 * @brief Configures FCH ACPI module before PCI enumeration
 *
 * @note This init flow is needed for the FCH in addition to the common init flow.
 * This function should be called in the TP1 entry point.
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchHwAcpiPreliminaryPrePcieInitTc (
  void
  )
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + PMx00000070 + 1),
    (uint8_t) ~BIT_8(7),
    0x00
    );
}

/**
 * FchInitPrePcieHwAcpiTc
 *
 * @brief Configures FCH ACPI module before PCI enumeration
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchHwAcpiPrePcieInitTc (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  UNUSED(FchDataPtr);
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  ProgramPciByteTable(FchTcInitEnvHwAcpiPciTable,
    sizeof (FchTcInitEnvHwAcpiPciTable) / sizeof (FchTcInitEnvHwAcpiPciTable[0])
    );

  FchHwAcpiWriteMmioTable(MmioEnvInitTableTc);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiAoacInitTc
 * @brief Initializes FCH AOAC module
 *
 * @param FchHwAcpi Pointer to FCH ACPI data block
 *
 */
void
FchHwAcpiAoacInitTc (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  if (FchHwAcpi->FchAoacProgramEnable) {
    // AB
    xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_ADDR),
      0,
      0x0001
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_DATA),
      0,
      0x39260080
      );

    // ACPISMBUS
    xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_ADDR),
      0,
      0x0002
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_DATA),
      0,
      0x88440100
      );

    // LPC
    xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_ADDR),
      0,
      0x0004
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_DATA),
      0,
      0x82570200
      );

    // ESPI
    xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_ADDR),
      0,
      0x001B
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_SRAM_DATA),
      0,
      0x211E0380
      );

    // Enable HW initiated store/restore
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_HW_INIT_EN),
      0,
      0x00000016
      );

    // Check if eSPI is on
    if (
      xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (FCH_AOAC_ESPI << 1)))
      & BIT_8(3)
      ) {
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_REG_HW_INIT_EN),
        ~BIT_32(27),
        BIT_32(27)
        );
    }
    // Set these two bits to enable store/restore of HPET and ACPI PM timer automatically.
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_SHADOW_TIMER_CTRL),
      0,
      BIT_32(0) + BIT_32(1)
      );

    // Start Shadow Timer
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + FCHREGx1100 + SSCx00000010),
      0,
      BIT_32(0) + BIT_32(3)
      );
  }
}

void
FchHwAcpiI2cI3cInitTc (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  //
  // I2C0
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(5)) {
    FchAoacPowerOnDev(FCH_AOAC_I2C0, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000091), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000092), 0, 0x0);
  } else {
    FchAoacPowerOnDev(FCH_AOAC_I2C0, 0);
  }

  //
  // I2C1
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(6)) {
    FchAoacPowerOnDev(FCH_AOAC_I2C1, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000093), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000094), 0, 0x0);
  } else {
    FchAoacPowerOnDev(FCH_AOAC_I2C1, 0);
  }

  //
  // I2C2
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(7)) {
    FchAoacPowerOnDev(FCH_AOAC_I2C2, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000071), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000072), 0, 0x1);
  } else {
    FchAoacPowerOnDev(FCH_AOAC_I2C2, 0);
  }
  //
  // I2C3
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(8)) {
    FchAoacPowerOnDev(FCH_AOAC_I2C3, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000013), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000014), 0, 0x1);
  } else {
    FchAoacPowerOnDev(FCH_AOAC_I2C3, 0);
  }

  //
  // I3C0
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(21)) {
    FchAoacPowerOnDev(AOACDEVx15, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000091), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000092), 0, 0x1);
  } else {
    FchAoacPowerOnDev(AOACDEVx15, 0);
  }

  //
  // I3C1
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(13)) {
    FchAoacPowerOnDev(AOACDEVx0D, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000093), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000094), 0, 0x1);
  } else {
    FchAoacPowerOnDev(AOACDEVx0D, 0);
  }
  //
  // I3C2
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(14)) {
    FchAoacPowerOnDev(AOACDEVx0E, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000071), 0, 0x2);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000072), 0, 0x2);
  } else {
    FchAoacPowerOnDev(AOACDEVx0E, 0);
  }
  //
  // I3C3
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(15)) {
    FchAoacPowerOnDev(AOACDEVx0F, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000013), 0, 0x2);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000014), 0, 0x2);
  } else {
    FchAoacPowerOnDev(AOACDEVx0F, 0);
  }
}

void
FchHwAcpiUartInitTc (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // UART0
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(11)) {
    FchAoacPowerOnDev(FCH_AOAC_UART0, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008C), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008D), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008E), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008F), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000090), 0, 0x2);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000230),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx00000234),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000238),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000240),
      ~BIT_32(23),
      0
      );
  } else {
    FchAoacPowerOnDev(FCH_AOAC_UART0, 0);
  }

  //
  // UART1
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(12)) {
    FchAoacPowerOnDev(FCH_AOAC_UART1, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008C), 0, 0x2);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008E), 0, 0x2);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000238),
      ~BIT_32(23),
      0
      );
  } else {
    FchAoacPowerOnDev(FCH_AOAC_UART1, 0);
  }

  //
  // UART2
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(16)) {
    FchAoacPowerOnDev(FCH_AOAC_UART2, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000087), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000088), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000089), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008A), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000008B), 0, 0x1);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx0000021C),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx00000220),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000224),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx0000022C),
      ~BIT_32(23),
      0
      );
  } else {
    FchAoacPowerOnDev(FCH_AOAC_UART2, 0);
  }

  //
  // UART3
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(26)) {
    FchAoacPowerOnDev(FCH_AOAC_UART3, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000087), 0, 0x2);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000089), 0, 0x2);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      GPIOx00000224),
      ~BIT_32(23),
      0
      );
  } else {
    FchAoacPowerOnDev(FCH_AOAC_UART3, 0);
  }

  //
  // UART4
  //
  if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(20)) {
    FchAoacPowerOnDev(FCH_AOAC_UART4, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx00000099), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000009A), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000009B), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000009C), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + IOMUXx0000009D), 0, 0x1);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx00000264),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx00000268),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx0000026C),
      ~BIT_32(23),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + GPIOx00000274),
      ~BIT_32(23),
      0
      );
  } else {
    FchAoacPowerOnDev(FCH_AOAC_UART4, 0);
  }

  // UART Legacy IO Enable Support
  if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable ) {
    // Check to disable eSPI 3F8 decode to avoid hard hang when UART 3F8 is enabled.
    if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & BIT_16(3)) {
      if ( xUSLMemRead16((void *)(size_t)(FCHREGxFEC20000 + FCH_ITF_ESPI_SECONDARY0_IO_BASE_REG0)) == 0x3F8) {
        //Disable eSPI 3F8 decoding at Range0
        xUSLMemReadModifyWrite16((void *)(size_t)(FCHREGxFEC20000 + ESPIx00000040),
          (uint16_t) ~BIT_16(8),
          0
          );
      }
    }

    xUSLMemWrite16((void *)(size_t)AL2AHBJTAGxFFEDC0020,
      FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable
      );

    xUSLMemReadModifyWrite32((void *)(size_t)(FCHREGxFED80000 + SMBUSPCIx000000FC),
      ~(BIT_32(31) + BIT_32(30) + BIT_32(29) + BIT_32(28)),
      BIT_32(31) + BIT_32(30) + BIT_32(29) + BIT_32(28)
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void
FchHwAcpiSpreadSpectrumTc (
  bool Enable
  )
{
  // In PHX, SSC is handled in ABL
  UNUSED(Enable);
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void
FchHwAcpiZstateTimingTc (
  void
  )
{
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PGPWRGOODTOALLENTMR),
    0x00,
    0x48084810
    );
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PLLRSTBTMR),
    0x00,
    0x18040801
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PCIRSTBTMR),
    0x00,
    0x3820
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_NBPWRGOODTMR),
    0x00,
    0x08A0
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PGPWRENDLY + 3),
    0x00,
    0x59
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + PMx0000004C),
    0x00,
    0x09
    );
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_VIRTUALWIRE),
    0x00,
    0x08230885
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PWRRSTCFG),
    0xDF,
    0x00
    );
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_TPRESET2),
    ~BIT_32(21),
    BIT_32(21)
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RESETCONTROL2),
    (uint8_t) ~BIT_8(1),
    0x00
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PWRRSTCNFG + 1),
    0x00,
    0x06
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_BTTNSCICONTROL + 2),
    0x3F,
    0x00
    );
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_APUPLLCTRL),
    0x0000FFFE,
    0x01000001
    );

  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + AOACx00000094),
    0xF00FFFFF,
    0x03B00000
    );
}
