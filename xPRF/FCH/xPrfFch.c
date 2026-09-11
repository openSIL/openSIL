/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
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
#include <xSIM.h>
#include <FCH/FchClass-api.h>
#include <FchHwAcpi-api.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>

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
 */
void
xPrfFchEnableSpi (
  void
  )
{
  xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_SPI_BASE_ADDR),
    0x001F,
    SPI_BASE
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI100ENABLE_REGISTER), 0xFE, 0x01);
}

/*
 * These FCH operations are host-triggered rather than tied to an openSIL time
 * point, so they are exposed as on-demand xPRF services.
 */

/**
 * xPrfFchReleaseSpdBus
 *
 * @brief Relinquish control over the FCH SPD bus.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchReleaseSpdBus (
  void
  )
{
  FCHCLASS_INPUT_BLK  *FchData;
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchData = (FCHCLASS_INPUT_BLK *)SilFindStructure(SilId_FchClass, 0);
  if (FchData == NULL) {
    return SilNotFound;
  }

  FchI2cReleaseControl(FchData, FchHwAcpi);

  return SilPass;
}

/**
 * xPrfFchServicePowerButton
 *
 * @brief Handle a power button depress.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchServicePowerButton (
  void
  )
{
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchHwAcpiServicePowerButton(FchHwAcpi);

  return SilPass;
}

/**
 * xPrfFchServiceAcpiOn
 *
 * @brief Hand ACPI/SCI over to the OS.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchServiceAcpiOn (
  void
  )
{
  FCHCLASS_INPUT_BLK  *FchData;
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchData = (FCHCLASS_INPUT_BLK *)SilFindStructure(SilId_FchClass, 0);
  if (FchData == NULL) {
    return SilNotFound;
  }

  FchHwAcpiServiceAcpiOn(FchData, FchHwAcpi);

  return SilPass;
}

/**
 * xPrfFchServiceAcpiOff
 *
 * @brief Take SCI back from the OS.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchServiceAcpiOff (
  void
  )
{
  FCHCLASS_INPUT_BLK  *FchData;
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchData = (FCHCLASS_INPUT_BLK *)SilFindStructure(SilId_FchClass, 0);
  if (FchData == NULL) {
    return SilNotFound;
  }

  FchHwAcpiServiceAcpiOff(FchData, FchHwAcpi);

  return SilPass;
}

/**
 * xPrfFchServiceSmiTimerStart
 *
 * @brief Start the SMI timer.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchServiceSmiTimerStart (
  void
  )
{
  FCHCLASS_INPUT_BLK  *FchData;
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchData = (FCHCLASS_INPUT_BLK *)SilFindStructure(SilId_FchClass, 0);
  if (FchData == NULL) {
    return SilNotFound;
  }

  FchHwAcpiServiceSmiTimerStart(FchData, FchHwAcpi);

  return SilPass;
}

/**
 * xPrfFchServiceSmiTimerStop
 *
 * @brief Stop the SMI timer.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfFchServiceSmiTimerStop (
  void
  )
{
  FCHCLASS_INPUT_BLK  *FchData;
  FCHHWACPI_INPUT_BLK *FchHwAcpi;

  FchHwAcpi = (FCHHWACPI_INPUT_BLK *)SilFindStructure(SilId_FchHwAcpiP, 0);
  if (FchHwAcpi == NULL) {
    return SilNotFound;
  }

  FchData = (FCHCLASS_INPUT_BLK *)SilFindStructure(SilId_FchClass, 0);
  if (FchData == NULL) {
    return SilNotFound;
  }

  FchHwAcpiServiceSmiTimerStop(FchData, FchHwAcpi);

  return SilPass;
}
