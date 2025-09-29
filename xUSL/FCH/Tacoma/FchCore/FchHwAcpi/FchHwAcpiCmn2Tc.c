/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiCmn2Tc.c
 * @brief FCH HwAcpi Common to TC Transfer functions
 *
 */


#include <SilCommon.h>
#include <FchHwAcpi-api.h>
#include <FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>
#include "FchHwAcpiCmn2Tc.h"

FCH_HWACPI_XFER_TABLE mFchHwAcpiXferTc = {
  .FchHwAcpiPrePcieInit = FchHwAcpiPrePcieInitTc,
  .FchHwAcpiUartInit = FchHwAcpiUartInitTc,
  .FchHwAcpiI2cI3cInit = FchHwAcpiI2cI3cInitTc,
  .FchHwAcpiAoacInit = FchHwAcpiAoacInitTc,
  .FchHwAcpiSpreadSpectrum = FchHwAcpiSpreadSpectrumTc,
  .FchHwAcpiZstateTiming = FchHwAcpiZstateTimingTc,
};
