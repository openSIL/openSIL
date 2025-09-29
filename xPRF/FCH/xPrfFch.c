/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFch.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        FCH
 *
 */

#include <SilCommon.h>
#include <xPRF-api.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>

/**
 * xPrfFchEnableSpi
 *
 * @brief This Function is responsible for enabling FCH SPI interface
 *
 * @details SPI interface is required by Data Fabric IP
 * for accessing the SPIROM early during the system boot.
 * xPrfFchEnableSpi enables MMIO access to SPI configuration
 * registers by programming SPI_BASE address to FCH D14F3x0A0
 * register (i.e., FCH::LPCPCICFG::SPI_BASE_ADDR). It also sets
 * FCH SPIx0020 register (i.e, FCH::LPCHOSTSPIREG::SPI100ENABLE_REGISTER)
 * LSB (i.e., usespi100) to 1, enabling the SPI interface.
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 */
void
xPrfFchEnableSpi (
  SIL_CONTEXT     *SilContext
  )
{
  xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCHD14F3x0A0),
    0x001F,
    SPI_BASE
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER), 0xFE, 0x01);
}
