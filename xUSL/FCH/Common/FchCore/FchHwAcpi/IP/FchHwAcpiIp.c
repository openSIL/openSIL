/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiIp.c
 * @brief FCH HwAcpi functions
 *
 */

#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <Mmio.h>
#include <FchHwAcpiReg.h>
#include <FchHwAcpi.h>
#include "FchHwAcpiIp.h"

static const ACPI_REG_WRITE MmioPrePcieResetTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {MISC_BASE >> 8, MISCx0000004C + 1, 0xFB, BIT_8(2)},
  {MISC_BASE >> 8, MISCx00000030, (uint8_t) ~BIT_8(1), BIT_8(1)},
  {MISC_BASE >> 8, MISCx00000030 + 1, (uint8_t) ~0x07, 0x07},
  {0xFF, 0xFF, 0xFF, 0xFF},
};


/**
 * FchHwAcpiHfpInit
 *
 * @brief Host Finger Print configuration
 *
 * @param[in] FchDataPtr FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchHwAcpi  Fch HwAcpi configuration structure pointer.
 *
 */
static
void FchHwAcpiHfpInit (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  if (FchHwAcpi->FchHfpEnable) {
    FchDataPtr->FchRunTime.FchDeviceEnableMap |= BIT_32(29);
  }

  xUSLMemReadModifyWrite8((void *)(size_t)(FCHREGxFEC11000 + HFPx0000001D),
    (uint8_t) ~(BIT_8(0) + BIT_8(1)),
    BIT_8(1)
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(FCHREGxFEC11000 + HFPx00000000 + 2),
    (uint8_t) ~(BIT_8(6) + BIT_8(7)),
    0
    );
}

/**
 * FchHwAcpiHidInit
 *
 * @brief Host Hid configuration
 *
 * @param[in] FchDataPtr FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchHwAcpi  Fch HwAcpi configuration structure pointer.
 *
 */
static
void FchHwAcpiHidInit (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  // HID1
  if (FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(30)) {
    xUSLMemReadModifyWrite32((void *)(size_t)(FCHREGxFEC13000 + HIDx00000000),
      ~(BIT_32(18) + BIT_32(29) + BIT_32(30)),
      ((FchDataPtr->FchRunTime.HidControl[0].SpiReadMode & 1) << 18)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiReadMode & 6) << 28)
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(FCHREGxFEC13000 + HIDx00000020),
      0x000FFFFF,
      ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 20)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 24)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 28)
      );
    xUSLMemReadModifyWrite16((void *)(size_t)(FCHREGxFEC13000 + HIDx0000006C),
      0xC0C0,
      (FchDataPtr->FchRunTime.HidControl[0].Spi_spd6 & 0x3F)
      + ((FchDataPtr->FchRunTime.HidControl[0].Spi_spd7 & 0x3f) << 8)
      );
    xUSLMemReadModifyWrite16((void *)(size_t)(FCHREGxFEC13000 + HIDx00000150),
      (uint16_t) ~BIT_16(15),
      BIT_16(15)
      );
  }
  // HID2
  if (FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(31)) {
    xUSLMemReadModifyWrite32((void *)(size_t)(FCHREGxFEC12000 + HID2x00000000),
      ~(BIT_32(18) + BIT_32(29) + BIT_32(30)),
      ((FchDataPtr->FchRunTime.HidControl[0].SpiReadMode & 1) << 18)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiReadMode & 6) << 28)
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(FCHREGxFEC12000 + HID2x00000020),
      0x000FFFFF,
      ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 20)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 24)
      + ((FchDataPtr->FchRunTime.HidControl[0].SpiSpeed & 0xF ) << 28)
      );
    xUSLMemReadModifyWrite16((void *)(size_t)(FCHREGxFEC12000 + HID2x0000006C),
      0xC0C0,
      (FchDataPtr->FchRunTime.HidControl[0].Spi_spd6 & 0x3F)
      + ((FchDataPtr->FchRunTime.HidControl[0].Spi_spd7 & 0x3f) << 8)
      );
    xUSLMemReadModifyWrite16((void *)(size_t)(FCHREGxFEC12000 + HID2x00000150),
      (uint16_t) ~BIT_16(15),
      BIT_16(15)
      );
  }
}

/**
 * FchHwAcpiPrePcieInitIp
 * @brief Config HwAcpi controller during power-on, additional init flow
 *
 * @note If this init flow is needed, this function should be called in the TP1
 * entry point.
 *
 * @param[in] FchDataPtr FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchHwAcpi  Fch HwAcpi configuration structure pointer.
 *
 */
void FchHwAcpiPrePcieInitIp (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  uint32_t Value32 = 0;

  FchHwAcpiWriteMmioTable(MmioPrePcieResetTable);

  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MISCCLKCNTRL0),
    (uint8_t) ~BIT_8(2),
    FchHwAcpi->BpX48M0ClockEnable ? BIT_8(2) : 0
    );

  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL),
    ~(BIT_32(20) + BIT_32(21)),
    FchHwAcpi->TurnOffXtalS3S5 ? BIT_32(20) + BIT_32(21) : 0
    );

  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL),
    ~BIT_32(19),
    FchHwAcpi->TurnOffXtalS0i3 ? BIT_32(19) : 0
    );

  if ( FchHwAcpi->Xtal48MPadPowerSaving ) {
    Value32 = xUSLMemRead32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx0000005C));
    if (!(Value32 & BIT_32(22))) {
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx0000005C),
        ~BIT_32(22),
        BIT_32(22)
        );
      SilFchStall(1);
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx00000000),
        ~(BIT_32(18) + BIT_32(17) + BIT_32(16)),
        BIT_32(17)
        );
      SilFchStall(1);
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx00000000),
        ~(BIT_32(21) + BIT_32(20) + BIT_32(19)),
        BIT_32(20)
        );
      SilFchStall(1);
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx00000000),
        ~(BIT_32(18) + BIT_32(17) + BIT_32(16)),
        BIT_32(17) + BIT_32(16)
        );
      SilFchStall(1);
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx0000005C),
        ~(BIT_32(21) + BIT_32(20) + BIT_32(19)),
        BIT_32(21)
        );
      SilFchStall(1);
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + MISCx00000000),
        ~BIT_32(22),
        BIT_32(22)
        );
    }
  }

  FchHwAcpiHfpInit(FchDataPtr, FchHwAcpi);
  FchHwAcpiHidInit(FchDataPtr);
}
