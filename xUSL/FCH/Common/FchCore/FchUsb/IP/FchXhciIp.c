/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciIp.c
 * @brief FCH xHCI functions
 *
 */

#include <IP/FchXhciIp.h>
#include <SilCommon.h>
#include <Pci.h>
#include <SmnAccess.h>

const FCH_BIOSSMC_MSG_INPUT_BLK FchBiosSmcMsgIp = {
  .UsbInit = 0x2B,
  .UsbConfigUpdate = 0x2C,
  .UsbSxEntry = 0x29,
  .UsbSxExit = 0x2A,
  .DisableUsbDbgClk = 0x67,
};

bool
FchXhciCheckUsbControllerSkip (uint32_t DieBusNum) {
  uint32_t PresilCtrl0;

  PresilCtrl0 = xUSLSmnRead(0, DieBusNum, 0x3810A84);
  return (bool)(PresilCtrl0 & BIT_32(9));
}

bool
FchXhciCheckUsbPhySkip (uint32_t DieBusNum) {
  uint32_t PresilCtrl0;

  PresilCtrl0 = xUSLSmnRead(0, DieBusNum, 0x3810A84);
  return (bool)(PresilCtrl0 & BIT_32(10));
}

/**
 * FchXhciIohcPmeDisable
 *
 * @brief Enable or disable IOHC PME for USB function
 *
 * @param DieBusNum Bus Number on Current Die.
 * @param PMEDis    true to disable, false to enable
 *
 */
void
FchXhciIohcPmeDisable (
  uint32_t DieBusNum,
  bool PMEDis
  )
{
  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(DieBusNum, 0, 0, 0x4C),
    (uint8_t) ~BIT_8(4),
    PMEDis ? BIT_8(4) : 0
    );
}
