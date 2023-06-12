/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSataBlk.c
 * @brief FCH SATA Data Block Interface
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FCH/Common/Fch.h>
#include <FCH/FchSata-api.h>
#include "FchSata.h"
#include "FchSataBlk.h"

static FCHSATA_INPUT_BLK mFchSataDefaults[4] = {
  { .SataEnable = true,
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
    .Reserved = {0}, },

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
      .Reserved = {0}, },
};

/**
 * FchSataGetInputBlk
 *
 * @brief Get Fch Sata Data
 *
 * @param None
 *
 * @retval pointer to FCH_SATA2.
 */
FCHSATA_INPUT_BLK *
FchSataGetInputBlk (
  void
  )
{
  return mFchSataDefaults;
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
FchSataSetInputBlk (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FCHSATA_INPUT_BLK *FchSataInput;

  FchSataInput = (FCHSATA_INPUT_BLK *) SilCreateInfoBlock(SilId_FchSata,
    SATA_CONTROLLER_NUM * sizeof (FCHSATA_INPUT_BLK),
    FCHSATA_INSTANCE,
    FCHSATA_MAJOR_REV,
    FCHSATA_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchSpiSetInputBlk at: 0x%x \n", FchSataInput);
  if (FchSataInput == NULL) {
    return SilAborted;
  }

  /*
   * Fill Fch SPI structure with defaults
   * Is it necessary to have 5 identical default block in flash?
   */
  memcpy((void *)FchSataInput, mFchSataDefaults, sizeof (FCHSATA_INPUT_BLK) * SATA_CONTROLLER_NUM);// 4 sata controllers

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
