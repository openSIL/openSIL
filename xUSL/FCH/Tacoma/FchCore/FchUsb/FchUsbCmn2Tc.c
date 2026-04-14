/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbCmn2Tc.c
 * @brief FCH USB Common to TC Transfer functions
 *
 */


#include <SilCommon.h>
#include <FchUsb-api.h>
#include <FchCore/FchUsb/FchUsbCmn2Rev.h>
#include "FchUsbCmn2Tc.h"

FCH_USB_XFER_TABLE mFchUsbXferTc = {
  .FchCioPcieDisable = FchCioPcieDisableTc,
  .FchUsbAfterPcieTrainingDone = FchUsbAfterPcieTrainingDoneTc
};
