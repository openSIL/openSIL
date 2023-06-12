/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciIp2IpKl.h
 * @brief Kunlun FCH XHCI Common to Kunlun Transfer function prototypes
 */

#pragma once
#include <FCH/FchUsb-api.h>

/**
 * FchSecondaryFchInitUsbKl
 *
 * @brief  Secondary Fch Usb Initialization.
 *
 * @param DieBusNum   Bus Number for Current Die
 * @param FchUsbData  FCH USB configuration structure pointer
 *
 * @retval None
 *
 */
void
FchSecondaryFchInitUsbKl (
  uint8_t          DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  );
