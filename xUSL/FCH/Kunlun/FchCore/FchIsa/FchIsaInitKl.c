/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaInitKl.c
 * @brief Kunlun FCH ISA module silicon init functions
 *
 */

#include <SilCommon.h>
#include <FchIsa-api.h>
#include <FchCore/FchIsa/FchIsaCmn2Rev.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <Pci.h>
#include "FchIsaCmn2Kl.h"

/**
 * FchIsaDriveSpiClockEarlyKl
 * @brief Enables driving the SPI clock early
 *
 */
void
FchIsaDriveSpiClockEarlyKl (
  void
  )
{
  // Enable drv_spiclk_earlier to fix TPM CS error issue
  xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI_CNTRL0_REGISTER),
    ~BIT_32(13),
    BIT_32(13)
    );
}

/**
 * FchIsaEnableTpmBufferFlushKl
 * @brief Enables flushing of TPM receive buffer
 *
 */
void
FchIsaEnableTpmBufferFlushKl (
  void
  )
{
  // Enable TMP flush prefetch buffer
  xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_0330),
    ~BIT_32(0),
    BIT_32(0)
    );
}
