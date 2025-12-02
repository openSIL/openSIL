/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
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
 * @param[in] FchDataPtr Pointer to Fch input data block
 * @param[in] FchSpiData FCH_SPI configuration structure pointer.
 *
 */
void
FchIsaInitSpi (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_SPI            *FchSpiData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FCH_ISA_XFER_TABLE *FchIsaXfer;

  if (SilGetCommon2RevXferTable(SilId_FchIsa, (void **)(&FchIsaXfer)) != SilPass) {
    return;
  }

  // Set time unit 15ns (15ns * WaitCount)
  xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI_CNTRL1_REGISTER),
    0xFFC0FFFF,
    0
    );

  xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI_CNTRL0_REGISTER),
    0xFFFFFFFF,
    (BIT_32(19) + BIT_32(24) + BIT_32(25) + BIT_32(26))
    );

  xUSLMemReadModifyWrite8((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
    ~BIT_8(0),
    FchSpiData->Spi100Enable
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

  if (FchSpiData->FastSpeed) {
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
      ~(uint32_t)(0xF << 24),
      (FchSpiData->FastSpeed - 1) << 24
      );
  }

  // Set SPI TPM Read/Write Speed
  if (FchSpiData->SpiTpmSpeed) {
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER),
      ~(uint32_t)(0xF << 16),
      (FchSpiData->SpiTpmSpeed - 1) << 16
      );
  }

  FchIsaXfer->FchIsaDriveSpiClockEarly ();

  xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100_HOST_PREFETCH_CONFIG_REGISTER),
    0xBFFF,
    0x4000
    );


  xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + 0x1C),
    ~BIT_32(10),
    FchSpiData->BurstWrite ? BIT_32(10) : 0
    );

  // Enable host buffer burst data out
  xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100_HOST_PREFETCH_CONFIG_REGISTER),
    0xBFFF,
    0x4000
    );

  // Disable host burst to 4 DWORD
  xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100_HOST_PREFETCH_CONFIG_REGISTER),
    0x7FFF,
    0x0000
    );

  FchIsaXfer->FchIsaEnableTpmBufferFlush ();

  // Enable flash prefetch on behalf of the host
  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_HOSTCONTROL),
    0xFF,
    BIT_8(0)
    );

  // Enable SPI prefetch for USB
  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, 0xBA),
    0xFF,
    BIT_8(7)
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
