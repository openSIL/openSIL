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
  SIL_RESERVED_STRUCT_0010  *Usb2Phy
  );

bool
FchUsbCheckOemTableValidTc (
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable
  );
