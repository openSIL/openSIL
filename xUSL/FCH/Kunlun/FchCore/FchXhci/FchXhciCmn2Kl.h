/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciCmn2Kl.h
 * @brief Kunlun FCH XHCI Common to Kunlun Transfer function prototypes
 */

#pragma once
#include <FCH/FchUsb-api.h>

/**
 * FchInitPrePcieXhciKl
 *
 * @brief Config FCH XHCI Module before PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPrePcieXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  );

/**
 * FchInitPostPcieXhciKl
 *
 * @brief Config FCH XHCI Module after PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPostPcieXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  );

/**
 * FchInitPreOsXhciKl
 *
 * @brief Config FCH XHCI Module before OS boot.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPreOsXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  );
