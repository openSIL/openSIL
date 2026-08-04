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

#define FCHUSBx12478C      0x12478Cul
#define FCHUSBx124790      0x124790ul
#define FCHUSBx124B8C      0x124B8Cul
#define FCHUSBx124B90      0x124B90ul
#define FCHUSBx124788      0x124788ul
#define FCHUSBx124B88      0x124B88ul

#define FCHUSBx168000                                  0x168000ul
#define FCHUSBx168028                                  0x168028ul
#define FCHUSBx168040                                  0x168040ul

#define FCHUSBxC12C        0xC12Cul
#define FCHUSBxC1B0        0xC1B0ul
#define FCHUSBxC604        0xC604ul
#define FCHUSBxC608        0xC608ul

//
// USB4
//

#define FCHUSBx00088       0x00088ul
#define FCHUSBx08044       0x08044ul
#define FCHUSBx1825C       0x1825Cul

#define FCHUSBxD8264   0xD8264ul
#define FCHUSBxD8270                     0xD8270ul

#define FCHUSBxE7B04 0xE7B04ul
#define FCHUSBxE7B08         0xE7B08ul
#define FCHUSBxE7B0C   0xE7B0Cul

#define FCH_TC_USB0_SMN_BASE   0x16C00000ul
#define FCH_TC_USB1_SMN_BASE   0x16E00000ul
#define FCH_TC_USB3_SMN_BASE   0x17200000ul
#define FCH_TC_USB4_SMN_BASE   0x19500000ul
