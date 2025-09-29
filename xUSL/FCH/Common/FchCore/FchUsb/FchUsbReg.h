/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbReg.h
 * @brief FCH USB registers
 *
 */

#pragma once

#define   FCHSEL12                                    12

#define FCHOFSTx00120000                        0x00120000ul
#define FCHOFSTx00180000                             0x00180000ul

// USB2.0 Lane Parameter Registers
#define FCH_XHCI_USB_20LANEPARACTL0_CNTR0                            0x11C000ul
#define FCH_XHCI_USB_20LANEPARACTL1_CNTR0                            0x11C004ul

#define FCHUSBx168028                                  0x168028ul

//
// USB4
//

#define FCHUSBxD8264   0xD8264ul
#define FCHUSBxD8270                     0xD8270ul

#define FCHUSBxE7B04 0xE7B04ul
#define FCHUSBxE7B08         0xE7B08ul
#define FCHUSBxE7B0C   0xE7B0Cul
