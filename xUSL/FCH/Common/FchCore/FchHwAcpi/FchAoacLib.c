/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAoacLib.c
 * @brief FCH AOAC functions
 *
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiReg.h>
#include <FCH/Common/FchReg.h>
#include <Fch.h>
#include <FchCommon.h>
#include "FchAoacLib.h"

/**
 * FchAoacPowerOnDev
 * @brief Powers On/Off Fch Device
 *
 * @param[in] Device     - Device to be turn on or off
 * @param[in] On         - 1 to turn on, 0 to turn off
 *
 */
void FchAoacPowerOnDev (uint8_t Device, uint8_t On)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  uint8_t  Retry = 0;

  xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVSTS_0 + (Device << 1)));
  if (On == 1) {
    while (((xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVSTS_0 + (Device << 1))) & 7) != 7)
      && (Retry < FCH_AOAC_RETRY)) {
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)),
        (uint8_t) ~(BIT_8(0) + BIT_8(1) + BIT_8(7)),
        BIT_8(3)
        );
      SilFchStall(10);
      Retry++;
    }

    if ((xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVSTS_0 + (Device << 1))) & 7) != 7) {
      // Log timeout failure
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "[FCH]FchAoacPowerOnDev Power ON Device 0x%x failed!\n", Device);
      // Power On one more time
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)),
        0xFF,
        BIT_32(3)
        );
    }
  } else {
    while ((xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVSTS_0 + (Device << 1))) != 0) &&
      (Retry < FCH_AOAC_RETRY)) {
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)),
        (uint8_t) ~(BIT_8(3) + BIT_8(7)),
        0
        );
      SilFchStall(10);
      Retry++;
    }

    if ((xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVSTS_0 + (Device << 1))) & 7) != 0) {
      // Log timeout fail
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "[FCH]FchAoacPowerOnDev Power OFF Device 0x%x failed!\n", Device);
      // Power Off with SW control
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)),
        0,
        0x80
        );
      xUSLMemWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)), 0x80);
    }

    if (Device != FCH_AOAC_ESPI) {  //PLAT-26858
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + FCH_AOAC_DEVCTRL_0 + (Device << 1)),
        0xFF,
        BIT_32(0) + BIT_32(1)
        );
    }
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
