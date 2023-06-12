/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciReg.h
 * @brief FCH xHCI controller registers
 *
 */

#pragma once

#define   SIL_RESERVED_0297                                    12

// USB2.0 Lane Parameter Registers
#define FCH_XHCI_USB_20LANEPARACTL0_CNTR0                            0x11C000ul
#define FCH_XHCI_USB_20LANEPARACTL1_CNTR0                            0x11C004ul

// USB3 PHY Registers
#define FCH_XHCI_USB31_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST           0x13004Cul
#define SIL_RESERVED_0298                                4

typedef enum {
  FchUsbConfigRegGroup1 = 1,
  FchUsbConfigRegGroup2,
  FchUsbConfigRegGroup3,
} FCH_USB_CONFIG_REG_GROUP;
