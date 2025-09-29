/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSpi.c
 * @brief FCH SPI controller functions
 *
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>
#include "FchIsaCmn2Rev.h"
#include "FchSpi.h"

/**
 * FchIsaInitSpi
 * @brief Initializes SPI controller during Power-On
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchDataPtr            Pointer to Fch input data block
 * @param FchSpiData            FCH_SPI configuration structure pointer.
 *
 */
void
FchIsaInitSpi (
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_SPI            *FchSpiData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FCH_ISA_XFER_TABLE *FchIsaXfer;

  if (SilGetCommon2RevXferTable(SilContext, SilId_FchIsa, (void **)(&FchIsaXfer)) != SilPass) {
    return;
  }

  // Set time unit 15ns (15ns * WaitCount)
  xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI_CNTRL1_REGISTER),
    0xFFC0FFFF,
    0
    );

  // Set SPI Normal Speed
  // Note: If the command is not using TMP Speed or Fast Speed, it uses Normal Speed
  if (FchSpiData->SpiSpeed) {
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
      ~(uint32_t)(0xF << 28),
      ((FchSpiData->SpiSpeed - 1) << 28)
      );
  }

  // Set SPI Speed for AltOpCode mode
  if (FchSpiData->WriteSpeed) {
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
      ~(uint32_t)(0xF << 20),
      (FchSpiData->WriteSpeed - 1) << 20
      );
  }

  // Set SPI TMP Read/Write Speed
  if (FchSpiData->SpiTpmSpeed) {
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
      ~(uint32_t)(0xF << 16),
      (FchSpiData->SpiTpmSpeed - 1) << 16
      );
  }

  FchIsaXfer->FchIsaRelocateRom3(SilContext);
  FchIsaXfer->FchIsaTpmInit(FchSpiData);

  // Enable host buffer burst data out
  xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100_HOST_PREFETCH_CONFIG_REGISTER),
    (uint16_t) ~BIT_16(14),
    BIT_16(14)
    );

  // Disable host burst to 4 DWORD
  xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100_HOST_PREFETCH_CONFIG_REGISTER),
    (uint16_t) ~BIT_16(15),
    0
    );

  // Enable flash prefetch on behalf of the host
  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCHD14F3x0BB),
    (uint8_t) ~BIT_8(0),
    BIT_8(0)
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
