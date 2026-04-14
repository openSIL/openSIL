/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbCmn2Tc.h
 * @brief FCH USB Common to Tc Transfer function prototypes
 *
 */

#pragma once

#include <SilCommon.h>
#include <FCH/Common/FchCore/FchUsb/FchUsbCmn2Rev.h>

extern FCH_USB_XFER_TABLE mFchUsbXferTc;

void FchCioPcieDisableTc (
  uint32_t         PcieController,
  FCHUSB_INPUT_BLK *FchUsbData
  );

void FchUsbAfterPcieTrainingDoneTc (
  SIL_CONTEXT *SilContext
  );
