/**
 * @file  FchSata.c
 * @brief FCH SATA controller functions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include "FchSata.h"
#include "FchSataCmn2Rev.h"
#include "FchSataReg.h"
#include <FCH/Common/FchHelper.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchCommonCfg.h>
#include <CommonLib/SmnAccess.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Fch.h>

#define MAX_RETRY_NUM 200

static FCH_SATA2 mFchSataDefaults[4] = {
    {   .SataEnable = true,
        .SataSetMaxGen2 = true,
        .SataMsiEnable = false,
        .SataRaidSsid = 0x00000000,
        .SataRaid5Ssid = 0x00000000,
        .SataAhciSsid = 0x00000000,
        .SataClass = SataAhci,
        .SataEspPort = 0,
        .SataPortPower = 0,
        .SataPortMd = 0,
        .SataAggrLinkPmCap = 1,
        .SataPortMultCap = 1,
        .SataClkAutoOff = 1,
        .SataPscCap = 1,
        .BiosOsHandOff = 0,
        .SataFisBasedSwitching = 1,
        .SataCccSupport = 0,
        .SataSscCap = 1,
        .SataTargetSupport8Device = 0,
        .SataDisableGenericMode = 0,
        .SataAhciEnclosureManagement = false,
        .SataSgpio0 = 0,
        .SataSgpio1 = 0,
        .SataPhyPllShutDown = 0,
        .SataOobDetectionEnh = false,
        .SataRasSupport = false,
        .SataAhciDisPrefetchFunction = true,
        .SataDevSlpPort0 = false,
        .SataDevSlpPort1 = false,
        .SataDevSlpPort0Num = 0,
        .SataDevSlpPort1Num = 0,
        .TempMmio = 0,
        .SataControllerAutoShutdown = false,
        .SataDisableMsgParameter = 0,
        .Reserved = {0},
    },

    {
        .SataEnable = true,
        .SataSetMaxGen2 = true,
        .SataMsiEnable = false,
        .SataRaidSsid = 0x00000000,
        .SataRaid5Ssid = 0x00000000,
        .SataAhciSsid = 0x00000000,
        .SataClass = SataAhci,
        .SataEspPort = 0,
        .SataPortPower = 0,
        .SataPortMd = 0,
        .SataAggrLinkPmCap = 1,
        .SataPortMultCap = 1,
        .SataClkAutoOff = 1,
        .SataPscCap = 1,
        .BiosOsHandOff = 0,
        .SataFisBasedSwitching = 1,
        .SataCccSupport = 0,
        .SataSscCap = 1,
        .SataTargetSupport8Device = 0,
        .SataDisableGenericMode = 0,
        .SataAhciEnclosureManagement = false,
        .SataSgpio0 = 0,
        .SataSgpio1 = 0,
        .SataPhyPllShutDown = 0,
        .SataOobDetectionEnh = false,
        .SataRasSupport = false,
        .SataAhciDisPrefetchFunction = true,
        .SataDevSlpPort0 = false,
        .SataDevSlpPort1 = false,
        .SataDevSlpPort0Num = 0,
        .SataDevSlpPort1Num = 0,
        .TempMmio = 0,
        .SataControllerAutoShutdown = false,
        .SataDisableMsgParameter = 0,
        .Reserved = {0},
    },

    {
        .SataEnable = true,
        .SataSetMaxGen2 = true,
        .SataMsiEnable = false,
        .SataRaidSsid = 0x00000000,
        .SataRaid5Ssid = 0x00000000,
        .SataAhciSsid = 0x00000000,
        .SataClass = SataAhci,
        .SataEspPort = 0,
        .SataPortPower = 0,
        .SataPortMd = 0,
        .SataAggrLinkPmCap = 1,
        .SataPortMultCap = 1,
        .SataClkAutoOff = 1,
        .SataPscCap = 1,
        .BiosOsHandOff = 0,
        .SataFisBasedSwitching = 1,
        .SataCccSupport = 0,
        .SataSscCap = 1,
        .SataTargetSupport8Device = 0,
        .SataDisableGenericMode = 0,
        .SataAhciEnclosureManagement = false,
        .SataSgpio0 = 0,
        .SataSgpio1 = 0,
        .SataPhyPllShutDown = 0,
        .SataOobDetectionEnh = false,
        .SataRasSupport = false,
        .SataAhciDisPrefetchFunction = true,
        .SataDevSlpPort0 = false,
        .SataDevSlpPort1 = false,
        .SataDevSlpPort0Num = 0,
        .SataDevSlpPort1Num = 0,
        .TempMmio = 0,
        .SataControllerAutoShutdown = false,
        .SataDisableMsgParameter = 0,
        .Reserved = {0},
    },

    {   .SataEnable = true,
        .SataSetMaxGen2 = true,
        .SataMsiEnable = false,
        .SataRaidSsid = 0x00000000,
        .SataRaid5Ssid = 0x00000000,
        .SataAhciSsid = 0x00000000,
        .SataClass = SataAhci,
        .SataEspPort = 0,
        .SataPortPower = 0,
        .SataPortMd = 0,
        .SataAggrLinkPmCap = 1,
        .SataPortMultCap = 1,
        .SataClkAutoOff = 1,
        .SataPscCap = 1,
        .BiosOsHandOff = 0,
        .SataFisBasedSwitching = 1,
        .SataCccSupport = 0,
        .SataSscCap = 1,
        .SataTargetSupport8Device = 0,
        .SataDisableGenericMode = 0,
        .SataAhciEnclosureManagement = false,
        .SataSgpio0 = 0,
        .SataSgpio1 = 0,
        .SataPhyPllShutDown = 0,
        .SataOobDetectionEnh = false,
        .SataRasSupport = false,
        .SataAhciDisPrefetchFunction = true,
        .SataDevSlpPort0 = false,
        .SataDevSlpPort1 = false,
        .SataDevSlpPort0Num = 0,
        .SataDevSlpPort1Num = 0,
        .TempMmio = 0,
        .SataControllerAutoShutdown = false,
        .SataDisableMsgParameter = 0,
        .Reserved = {0},
    },
};


/**
 * GetFchSataData
 *
 * @brief Get Fch Sata Data
 *
 * @param None
 *
 * @retval pointer to FCH_SATA2.
*/
FCH_SATA2* GetFchSataData (void)
{
  return mFchSataDefaults;
}

/**-----------------------------------------------------------------------------
 * FchInitResetSata - Config Sata controller during Power-On
 *
 *
 *
 * @param[in] FchSata Fch SATA configuration structure pointer.
 *
 */
static void
FchInitResetSata (
  FCH_SATA2 *FchSata
  )
{
  FCH_SATA_XFER_TABLE  *FchSataXfer;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable (SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }

  FchSataXfer->FchInitResetSataProgram (0, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitDevSlp
 * @brief Init DevSlp configuration
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
static void
FchSataInitDevSlp (
  uint32_t DieBusNum,
  FCH_SATA2 *FchSata
  )
{
  uint16_t PortNumByte;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  PortNumByte = FchSata[0].SataDevSlpPort0Num;
  if (FchSata[0].SataDevSlpPort0) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG05), 0x00, 0x1);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_014_BP_AGPIO5), ~(uint32_t) (BIT_32(20) + BIT_32(21) + BIT_32(22) + BIT_32(23)), BIT_32(23));
    xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + SIL_RESERVED_51, 0xFFFFFFEF, BIT_32(4) + (BIT_32(8) << PortNumByte));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG05), 0x00, 0x00);
    xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + SIL_RESERVED_51, ~(uint32_t) (BIT_32(8) << PortNumByte), 0x00);
    if (!FchSata[0].SataDevSlpPort1) {
      xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + SIL_RESERVED_51, 0xFFFFFFEF, 0x00);
    }
  }

  PortNumByte = FchSata[0].SataDevSlpPort1Num;
  if (FchSata[0].SataDevSlpPort1) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG06), 0x00, 0x1);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_018_BP_AGPIO6), ~(uint32_t) (BIT_32(20) + BIT_32(21) + BIT_32(22) + BIT_32(23)), BIT_32(23));
    xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + SIL_RESERVED_51, 0xFFFFFFEF, BIT_32(4) + (BIT_32(8) << PortNumByte));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG06), 0x00, 0x00);
    xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + SIL_RESERVED_51, ~(uint32_t) (BIT_32(8) << PortNumByte), 0x00);
    if (!FchSata[0].SataDevSlpPort0) {
      xUSLSmnReadModifyWrite (0, DieBusNum,
        SIL_RESERVED_48 + SIL_RESERVED_51,
        0xFFFFFFEF,
        0x00);
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataGpioSetPad - Set SGPIO PAD
 *
 *
 * @param[in] Controller Sata controller number.
 *
 */
static void
FchSataGpioSetPad (
    uint32_t     Controller
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  switch ( Controller ) {
  case 0:    //Sata0
    //GPIOx00024 0000_0000h 9 AGPIO9_SGPIO_DATAOUT_MDIO1_SCL
    //GPIOx00028 0000_0000h 10 S0A3_GPIO_AGPIO10_SGPIO_CLK_MDIO0_SCL
    //GPIOx0005C 0000_0000h 23 AGPIO23_SGPIO_LOAD_MDIO1_SDA
    //GPIOx000A0 0000_0000h 40 AGPIO40_SGPIO_DATAIN_MDIO0_SDA
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG09),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG0A),
        0x00,
        0x02);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG17),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG28),
        0x00,
        0x01);
    //Make SPIO pins pull up enable
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x24),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x28),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x5C),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    break;

  case 1:    //Sata1 PAD on remote tile
    //REMOTE_GPIOx00010 0000_0000h C4 AGPIO260_SGPIO_DATAOUT_1
    //REMOTE_GPIOx00014 0000_0000h C5 AGPIO261_SGPIO_LOAD_1
    //REMOTE_GPIOx00018 0000_0000h C6 AGPIO262_SGPIO_CLK_1
    //REMOTE_GPIOx0001C 0000_0000h C7 AGPIO263_SGPIO_DATAIN_1
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC4),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC5),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC6),
        0x00,
        0x01);
    //Make SPIO pins pull up enable
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x10),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x14),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x18),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    break;

  case 2:    //Sata2
    //REMOTE_GPIOx00000 0000_0000h C0 AGPIO256_SGPIO_DATAOUT_2
    //REMOTE_GPIOx00004 0000_0000h C1 AGPIO257_SGPIO_LOAD_2
    //REMOTE_GPIOx00008 0000_0000h C2 AGPIO258_SGPIO_CLK_2
    //REMOTE_GPIOx0000C 0000_0000h C3 AGPIO259_SGPIO_DATAIN_2
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC0),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC1),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC2),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_REGC3),
        0x00,
        0x01);
    //Make SPIO pins pull up enable
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x00),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x04),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + 0x08),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    break;

  case 3:    //Sata3
    //GPIOx00074 0000_0000h 29 AGPIO29_SGPIO_DATAOUT_3
    //GPIOx00078 0000_0000h 30 AGPIO30_SGPIO_LOAD_3
    //GPIOx0007C 0000_0000h 31 AGPIO31_SGPIO_CLK_3
    //GPIOx00080 0000_0000h 32 AGPIO32_SGPIO_DATAIN_3
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG1D),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG1E),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG1F),
        0x00,
        0x01);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG20),
        0x00,
        0x01);
    //Make SPIO pins pull up enable
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x74),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x78),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x7C),
      (uint32_t)~FCH_GPIO_BANK0_PULLDOWNENABLE,
      FCH_GPIO_BANK0_PULLUPENABLE);
    break;

  default:
    break;
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataGpioInitial - Sata GPIO function Procedure
 *
 *   - Private function
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
FchSataGpioInitial (
    uint32_t     DieBusNum,
    uint32_t     Controller
  )
{
  uint32_t                 FchSataBarRegDword;

  FchSataGpioSetPad (Controller);

  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_49, ~BIT_32(27), BIT_32(27));

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0xC0);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x01);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00, 0x00000020);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
   FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0x03);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x01);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00, 0xA0A0A0A0);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
    FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x02);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00000000, BIT_32(23));
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x510, 0x00000000, 0x0F0F3700);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
    FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0xC0);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x01);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00, 0x00000021);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
    FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0x03);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x01);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00, 0xA0A0A0A0);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
    FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);

  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x506, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x507, 0x00, 0x00);
  xUSLSmnReadModifyWrite8 (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x508, 0x00, 0x02);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x50C, 0x00000000, BIT_32(23));
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + 0x510, 0x00000000, 0x0F0F3700);
  xUSLSmnReadModifyWrite (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20, ~(uint32_t) (BIT_32(8)), BIT_32(8));
  do {
    FchSataBarRegDword = xUSLSmnRead (0, DieBusNum, SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + FCH_SATA_BAR5_REG20);
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall (5000);
}

/**
 * FchSataInitEsata
 * @brief Enable eSATA port
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
static void
FchSataInitEsata (
  uint32_t DieBusNum,
  uint32_t Controller,
  FCH_SATA2 *FchSata
  )
{
  uint32_t SataRegAddr;

  if (FchSata[Controller].SataEspPort != 0) {
    SataRegAddr = SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_50;
    xUSLSmnReadModifyWrite (0, DieBusNum, SataRegAddr, ~(FchSata[Controller].SataEspPort), 0);
    xUSLSmnReadModifyWrite (0, DieBusNum, SataRegAddr, 0xFF00FFFF, (FchSata[Controller].SataEspPort << 16));
    SataRegAddr = SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_49;
    xUSLSmnReadModifyWrite (0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(20)), BIT_32(20));
  } else {
    SataRegAddr = SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_50;
    xUSLSmnReadModifyWrite (0, DieBusNum, SataRegAddr, 0xFF00FF00, 0x00);
    SataRegAddr = SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_49;
    xUSLSmnReadModifyWrite (0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(20)), 0x00);
  }
}

/**
 * FchInitEnvProgramSata - Sata Init before PCI scan
 *
 *
 * @param[in] DieBusNum - Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata - Fch Sata configuration structure pointer
 *
 */
void
FchInitEnvProgramSata (
    uint32_t   DieBusNum,
    uint32_t   Controller,
    FCH_SATA2 *FchSata
  )
{
  FCH_SATA_XFER_TABLE *FchSataXfer;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable (SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }

  // Do Sata init

  FchSataInitEsata (DieBusNum, Controller, FchSata);

  if (FchSata[Controller].SataSgpio0) {
    FchSataGpioInitial (DieBusNum, Controller);
  } else {
    FchSataXfer->FchSgpioToMpio (DieBusNum, Controller);
  }

  if (Controller == 0) {
    FchSataInitDevSlp (DieBusNum, FchSata);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*************************** LATE INIT ***************************************/

/**
 * FchInitLateProgramSataRegs - Sata Control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchInitLateProgramSataRegs (
  uint32_t   DieBusNum,
  uint32_t   Controller,
  FCH_SATA2 *FchSata
  )
{
  FCH_SATA_XFER_TABLE *FchSataXfer;

  if (SilGetCommon2RevXferTable (SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }
  FchSataXfer->FchSataInitMmc (DieBusNum, Controller, FchSata);
}

/**
 * FchInitLateSata
 * @brief Prepare SATA controller to boot to OS.
 *
 * @param[in] FchData Fch Sata configuration structure pointer.
 *
 */
void FchInitLateSata (FCH_SATA2 *FchSata)
{
  uint32_t SataController;

  for (SataController = 0; SataController < SATA_CONTROLLER_NUM; SataController++) {
    if (FchSata[SataController].SataEnable) {
      FchInitLateProgramSataRegs (0, SataController, FchSata);
    } else {
      continue;
    }
  }
}

/**
 * FchSataSetInputBlk
 * @brief Establish FCH Sata input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS
FchSataSetInputBlk (void)
{
  FCHSATA_INPUT_BLK *FchSataInput;

  FchSataInput = (FCHSATA_INPUT_BLK *) SilCreateInfoBlock (SilId_FchSata,
                                  SATA_CONTROLLER_NUM * sizeof(FCHSATA_INPUT_BLK),
                                  FCHSATA_INSTANCE,
                                  FCHSATA_MAJOR_REV,
                                  FCHSATA_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchSpiSetInputBlk at: 0x%x \n", FchSataInput);
  if (FchSataInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy ((void *)FchSataInput, mFchSataDefaults, sizeof(FCHSATA_INPUT_BLK) * SATA_CONTROLLER_NUM);// 4 sata controllers

  return SilPass;
}

/**
 * FchSataPrePcieInit
 *
 * @brief Config Sata controller during power-on
 *
 * @param FchSata FCH Sata configuration structure pointer.
 *
 */
void
FchSataPrePcieInit (
  FCH_SATA2 *FchSata
  )
{
  FCH_SATA_XFER_TABLE *FchSataXfer;

  if (SilGetCommon2RevXferTable (SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }
  FchInitResetSata(FchSata);
  FchSataXfer->FchInitEnvSata(FchSata);
}
