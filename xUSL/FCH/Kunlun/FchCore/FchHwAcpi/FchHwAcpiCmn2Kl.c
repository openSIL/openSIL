/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiCmn2Kl.c
 * @brief Kunlun FCH ACPI Common to Kunlun Transfer functions
 *
 */

#include <SilCommon.h>
#include <FchHwAcpi-api.h>
#include <FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>
#include "FchHwAcpiCmn2Kl.h"

FCH_HWACPI_XFER_TABLE mFchHwAcpiXferKl = {
  .Header = {
    .Version = 2
  },
  .FchInitPreliminaryPrePcieHwAcpi = FchInitPreliminaryPrePcieHwAcpiKl,
  .FchInitPrePcieHwAcpi = FchInitPrePcieHwAcpiKl,
  .FchInitPreliminaryPostPcieHwAcpi = FchInitPreliminaryPostPcieHwAcpiKl,
  .FchInitPostPcieHwAcpi = FchInitPostPcieHwAcpiKl,
  .FchInitPreliminaryPreOsHwAcpi = FchInitPreliminaryPreOsHwAcpiKl,
  .FchInitPreOsHwAcpi = FchInitPreOsHwAcpiKl,
  .FchHwAcpiEnableIxC = FchHwAcpiEnableIxCKl,
  .FchHwAcpiResetRtcExt = FchHwAcpiResetRtcExtKl,
  .FchHwAcpiUartLegacyIoInit = FchHwAcpiUartLegacyIoInitKl,
  .FchHwAcpiAoacInit = FchHwAcpiAoacInitKl,
};
