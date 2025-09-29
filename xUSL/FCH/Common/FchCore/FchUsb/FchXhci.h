/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhci.h
 * @brief FCH xHCI controller function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include "FchUsbReg.h"
#include <SMU/SmuDefs.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

#define GET_BITMASK(n)                      ((1 << n) - 1)
#define GET_USB_OP_GROUP(usbhost, group)    ((1 << 24) | ((usbhost & 0xF) << 20) | group)

typedef enum {
  FchUsbConfigRegGroup1 = 1,
  FchUsbConfigRegGroup2 = 2,
  FchUsbConfigRegGroup3 = 3,
  FchUsbConfigRegGroup6 = 6,
} FCH_USB_CONFIG_REG_GROUP;

SIL_STATUS
FchUsbSetInputBlk (
  SIL_CONTEXT  *SilContext,
  const FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsgDefs
  );

SMC_RESULT FchXhciSmuService (
  SIL_CONTEXT  *SilContext,
  uint32_t     DieBusNum,
  uint32_t     RequestId
  );

SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      SmnRegister,
  uint32_t      SmnMask,
  uint32_t      SmnData,
  uint32_t      SmnGroup
  );

void
FchUsb3PortForceGen1 (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      UsbController,
  uint32_t      PortNum,
  uint32_t      UsbPortForceGen1
  );

void
FchXhciDbgSC (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData,
  uint32_t         EnableValue,
  uint32_t         DisableValue
  );

void
FchXhciSparseMode (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchXhciOCPolarity (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchXhciDeviceRemovable (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      UsbController,
  uint32_t      DeviceRemovable
  );

void
FchXhciOverCurrent (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      UsbController,
  uint32_t      PortNum,
  uint32_t      OverCurrentMap
  );

void
FchXhciDdiModeEnable (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DdiMode
  );

void
FchXhciPdInterruptMode (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  uint32_t UsbController
  );

SMC_RESULT
FchUsbDbgClkDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchUsb31PhySwitchPort (
  SIL_CONTEXT    *SilContext,
  uint32_t       DieBusNum,
  uint32_t       UsbController,
  uint32_t       Port
  );
