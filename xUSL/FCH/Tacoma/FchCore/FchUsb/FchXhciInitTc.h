/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciInitTc.h
 * @brief Tacoma FCH XHCI Module silicon init function prototypes
 */

#pragma once

#include <SilCommon.h>
#include <FchUsb-api.h>
#include <FchUsbOemTc.h>

void
FchInitPrePcieXhciTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  );

void
FchUsbOemUsb20PhyConfigurePerPortTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  uint32_t          Controller,
  uint8_t           Port,
  FCH_USB20_PHY     *Usb2Phy
  );

bool
FchUsbCheckOemTableValidTc (
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable
  );
