/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchInitTc.c
 * @brief TC Fch Class Silicon initialization functions
 *
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Include/FchGPIORegs.h>
#include <FCH/Common/Fch.h>
#include "FchIp2IpTc.h"

#define ACPI_MMIO_BASE_ADDRESS    0xFED80000
#define GPIO_BANK_BASE            0x1500

/**
 * SilFchGpioSlotResetControl
 *
 * @brief   Gpio reset control.
 *
 * @details This function will try to deassert slot reset for each die in the system. If multi-die
 *          is not supported for the SoC, this function will utilize the AcpiMmioBase from the FCH data
 *          block to deassert the primary die.
 *
 * @retval  SilPass        Function returns successfully
 *
 */
SIL_STATUS
SilFchGpioSlotResetControl (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_INFO, "Deassert PCIe Slot Reset, ACPI MMIO Base 0x%llx.\n", ACPI_MMIO_BASE_ADDRESS);

  /* Deassert both PCIe Resets */
  xUSLMemWrite32((void *)(size_t)(ACPI_MMIO_BASE_ADDRESS + GPIO_BANK_BASE + FCH_GPIO_PCIE_RST1_L_AGPIO26), 0xc40000);
  xUSLMemWrite32((void *)(size_t)(ACPI_MMIO_BASE_ADDRESS + GPIO_BANK_BASE + GPIOx0000006C), 0xc40000);

  return SilPass;
}
