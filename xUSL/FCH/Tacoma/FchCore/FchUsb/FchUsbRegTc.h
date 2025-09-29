/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbRegTc.h
 * @brief Tacoma FCH USB register definitions
 */

#pragma once

#define FCH_XHCI_NAME_TC                                      "PHX (F.1)"
#define FCH_XHCI_VERSION_MAJOR_TC                             0xF
#define FCH_XHCI_VERSION_MINOR_TC                             0x1

#define FCH_USB0_NBIF_STRAP0_TC              0x10134600ul
#define FCH_USB1_NBIF_STRAP0_TC              0x10134800ul

//
// USB4
//

#define FCH_USB4_RT0_NBIF_STRAP0_TC          0x10136A00ul
#define FCH_USB4_RT1_NBIF_STRAP0_TC          0x10136C00ul

#define FCH_IOHC_BRIDGE_CNTL_PCIE2_TC        0x13B35004ul
#define FCH_IOHC_BRIDGE_CTRL_PCIE3_TC        0x13B37004ul

#define FCH_USB_HOST_SELECT_USB4RT0_TC       0x5
#define FCH_USB_HOST_SELECT_USB4PHY0_TC      0x6
#define FCH_USB_HOST_SELECT_USB4RT1_TC       0x7
#define FCH_USB_HOST_SELECT_USB4PHY1_TC      0x8
