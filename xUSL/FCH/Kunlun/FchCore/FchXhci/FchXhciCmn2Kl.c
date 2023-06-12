/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciCmn2Kl.c
 * @brief Kunlun FCH XHCI Common to Kunlun Transfer functions
 *
 */

#include <SilCommon.h>
#include <FCH/Common/FchCore/FchXhci/FchUsbCmn2Rev.h>
#include "FchXhciCmn2Kl.h"

FCH_XHCI_XFER_TABLE mFchXhciXferKl = {
  .Header = {
    .Version = 2  // Version 2 = Kunlun
  },
  .FchInitPrePcieXhci = FchInitPrePcieXhciKl,
  .FchInitPostPcieXhci = FchInitPostPcieXhciKl,
  .FchInitPreOsXhci = FchInitPreOsXhciKl
};
