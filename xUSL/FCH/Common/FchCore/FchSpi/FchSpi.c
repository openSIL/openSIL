/**
 * @file  FchSpi.c
 * @brief FCH SPI controller functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include "FchSpi.h"
#include "FchSpiReg.h"
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchHelper.h>
#include <FCH/Common/FchCommonCfg.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>

#define SPI_BASE 0xFEC10000ul


static const FCHSPI_INPUT_BLK mFchSpiDefaults = {
  // This is where you declare all input block variables with their values you want to share with the Host.
  // This becomes part of the IP API for the Host.
  .LpcEnable = true,
  .LpcMsiEnable = false,
  .LpcSsid = 0x00000000,
  .RomBaseAddress = 0,
  .SpiSpeed = 0x2,
  .SpiFastSpeed = 0,
  .WriteSpeed = 0,
  .SpiTpmSpeed = 0,
  .SpiBurstWrite = 0,
  .LpcClk0 = true,
  .LpcClk1 = true,
};

/**
 * FchSpiSetInputBlk
 * @brief Establish FCH Spi input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchSpiSetInputBlk (void)
{
  FCHSPI_INPUT_BLK *FchSpiInput;

  FchSpiInput = (FCHSPI_INPUT_BLK *) SilCreateInfoBlock (SilId_FchSpi,
                                  sizeof(FCHSPI_INPUT_BLK),
                                  FCHSPI_INSTANCE,
                                  FCHSPI_MAJOR_REV,
                                  FCHSPI_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchSpiSetInputBlk at: 0x%x \n", FchSpiInput);
  if (FchSpiInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy ((void *)FchSpiInput, &mFchSpiDefaults, sizeof(FCHSPI_INPUT_BLK));

  return SilPass;
}

/**-----------------------------------------------------------------------------
 * @brief FchInitResetSpi - Config Spi controller during Power-On
 *
 * @param[in] FchSpiData FCH_SPI configuration structure pointer.
 *
 */
static void FchInitResetSpi (FCH_SPI *FchSpiData) {

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Set Spi ROM Base Address
  //

  xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG00), 0xFFFFFFFF,
                              BIT_32(19) + BIT_32(24) + BIT_32(25) + BIT_32(26));
  xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG0C), 0xFFC0FFFF, 0 );

  //Set SPI100 Enable
  xUSLMemReadModifyWrite8 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG20), 0xFE, 0x01);

  //
  //  Spi Mode Initial
  //
  if (FchSpiData->SpiSpeed) {
    xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG20),
            ~((uint32_t) (0xF << FCH_SPI_NORMAL_SPEED_BIT_OFFSET)),
            ((FchSpiData->SpiSpeed - 1 ) << FCH_SPI_NORMAL_SPEED_BIT_OFFSET));
  }

  if (FchSpiData->WriteSpeed) {
    xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG20),
            ~((uint32_t) (0xF << FCH_SPI_WRITE_SPEED_BIT_OFFSET)),
            ((FchSpiData->WriteSpeed - 1 ) << FCH_SPI_WRITE_SPEED_BIT_OFFSET));
  }

  if (FchSpiData->SpiTpmSpeed) {
    xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG20),
            ~((uint32_t) (0xF << FCH_SPI_TPM_SPEED_BIT_OFFSET)),
            ((FchSpiData->SpiTpmSpeed - 1 ) << FCH_SPI_TPM_SPEED_BIT_OFFSET));
  }

  xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG1C),
            ~BIT_32(10), ((FchSpiData->SpiBurstWrite) << 10));

  xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG2C),
            ~BIT_32(14), BIT_32(14));

  xUSLMemReadModifyWrite32 ((void*)(size_t)(SPI_BASE + FCH_SPI_MMIO_REG2C),
            ~BIT_32(15), 0);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * InitializeFchSpiTp1
 *
 * @brief Config Spi controller during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSpiTp1 (void)
{
  FCHSPI_INPUT_BLK   *LclInpSpiBlk; //pointer to input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpSpiBlk = (FCHSPI_INPUT_BLK *) SilFindStructure (SilId_FchSpi, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Spi found blk at: 0x%x \n", LclInpSpiBlk);
  if (LclInpSpiBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchSpiPrePcieInit(LclInpSpiBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchSpiTp2
 *
 * @brief Config Spi controller during timepoint 2 (Post-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSpiTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchSpiTp3
 *
 * @brief Config Spi controller during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchSpiTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchSpiPrePcieInit
 * @brief Config Spi controller during Pre-Pcie phase
 *
 * @param[in] FchSpiData FCH_SPI configuration structure pointer.
 *
 */
void
FchSpiPrePcieInit (
  FCH_SPI *FchSpi
  )
{
  FchInitResetSpi(FchSpi);
}
