/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhci.h
 * @brief FCH xHCI controller function prototypes
 *
 */

#pragma once

#include <FCH/FchClass-api.h>
#include "FchXhciReg.h"
#include <SMU/SmuDefs.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

#define GET_BITMASK(n)                      ((1 << n) - 1)
#define GET_USB_OP_GROUP(usbhost, group)    ((1 << 24) | ((usbhost & 0xF) << 20) | group)

#define SIL_RESERVED_0294                 0xA
#define SIL_RESERVED_0293         0x13
#define SIL_RESERVED_0296              0x14
#define SIL_RESERVED_0295               0x15

void
FchXhciPrePcieInit (
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchInitLateUsbXhci (
  FCHUSB_INPUT_BLK *FchUsbData
  );

SIL_STATUS
FchUsbSetInputBlk (
  void
  );

SMC_RESULT FchXhciSmuService (
  uint32_t DieBusNum,
  uint32_t RequestId
  );

SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  uint32_t DieBusNum,
  uint32_t SmnRegister,
  uint32_t SmnMask,
  uint32_t SmnData,
  uint32_t SmnGroup
  );

void
FchUsb3PortForceGen1 (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t UsbPortForceGen1
  );

void
FchXhciDbgSC (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData,
  uint32_t         EnableValue,
  uint32_t         DisableValue
  );

void
FchXhciSparseMode (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchXhciOCPolarity (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchXhciRasFeature (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchXhciDeviceRemovable (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DeviceRemovable
  );

void
FchXhciOverCurrent (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t OverCurrentMap
  );

void
FchUsb31PhySwitchPort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port
  );
