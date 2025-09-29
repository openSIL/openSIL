/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCio.h
 * @brief FCH USB4 controller function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchUsb-api.h>

void
FchCioTunnelDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioAdapterHide (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioTbt3Disable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioTxFFEConfigure (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioGen3Support (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioPhyLoad (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         PhySelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioClxEnable (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioP4PgEnable (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioLinkSpeed (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void
FchCioDebugSCDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         RouterSelect,
  bool             Disable
  );
