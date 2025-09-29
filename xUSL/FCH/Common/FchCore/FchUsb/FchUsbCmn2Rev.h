/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

#pragma once

#include <SilCommon.h>
#include <FchUsb-api.h>

typedef void (*FCH_CIO_PCIE_DISABLE) (
  uint32_t         PcieController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

typedef struct {
  FCH_CIO_PCIE_DISABLE FchCioPcieDisable;
} FCH_USB_XFER_TABLE;
