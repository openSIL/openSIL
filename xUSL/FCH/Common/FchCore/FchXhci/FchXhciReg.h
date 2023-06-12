/**
 * @file  FchXhciReg.h
 * @brief FCH xHCI controller registers
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
#pragma once

/**********************************************************************************************************************
 * Function prototypes
 *
 */

/**********************************************************************************************************************
 * Module data structure
 *
 */

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define SIL_RESERVED_28 0x16D80000ul
#define FCH_HS_USB0_LANE0_DIG_ANA_RX_ANA_IQ_PHASE_ADJUST \
    (FCH_HS_USB0_USB31_PHY_INTERNAL + 0x42D0) //0x42D0s
#define FCH_HS_USB0_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST \
    (FCH_HS_USB0_USB31_PHY_INTERNAL + 0xC14C) //0xC14C
#define FCH_HS_USB0_RAWLANE0_DIG_PCS_XF_RX_EQ_DELTA_IQ_OVRD_IN \
    (FCH_HS_USB0_USB31_PHY_INTERNAL + 0xC064) //0xC064
#define FCH_HS_USB0_SMN_PCICFG 0x10843000ul
#define FCH_HS_USB0_SUPX_DIG_LVL_OVRD_IN \
    (FCH_HS_USB0_USB31_PHY_INTERNAL + 0x2003C) //0x2003C
#define FCH_HS_USB0_SUP_DIG_LVL_OVRD_IN (FCH_HS_USB0_USB31_PHY_INTERNAL + 0x3C) //0x3C
#define FCH_HS_USB0_USB31_PHY_INTERNAL 0x16D20000ul
#define FCH_HS_USB1_LANE0_DIG_ANA_RX_ANA_IQ_PHASE_ADJUST \
    (FCH_HS_USB1_USB31_PHY_INTERNAL + 0x42D0) //0x42D0
#define FCH_HS_USB1_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST \
    (FCH_HS_USB1_USB31_PHY_INTERNAL + 0xC14C) //0xC14C
#define FCH_HS_USB1_RAWLANE0_DIG_PCS_XF_RX_EQ_DELTA_IQ_OVRD_IN \
    (FCH_HS_USB1_USB31_PHY_INTERNAL + 0xC064) //0xC064
#define FCH_HS_USB1_SMN_PCICFG               0x10743000ul
#define FCH_HS_USB1_SUPX_DIG_LVL_OVRD_IN \
    (FCH_HS_USB1_USB31_PHY_INTERNAL + 0x2003C) //0x2003C

#define FCH_HS_USB1_SUP_DIG_LVL_OVRD_IN (FCH_HS_USB1_USB31_PHY_INTERNAL + 0x3C) //0x3C
#define FCH_HS_USB1_USB31_PHY_INTERNAL 0x16F20000ul
#define FCH_HS_USB_CNTL_STEP 0x00200000ul
#define FCH_HS_USB_GUCTL 0x16C0C12Cul //0x16C0C12C
#define FCH_HS_USB_PORT_CONTROL SIL_RESERVED_28 + 0x130
#define FCH_HS_USB_PORT_DISABLE0 SIL_RESERVED_28 + 0x128

#define FCH_USB_PORT_DISABLE_MASK 0x00030003

#define FCH_XHCI_GUCTL                                               0xC12Cul
#define FCH_XHCI_LINK_REGS0_LLUCTL                                   0xD024ul

#define SIL_RESERVED_35                                              0x180000ul
#define FCH_XHCI_DEVICE_REMOVABLE_CNTR0                              0x114ul
#define FCH_XHCI_INTERRUPT_CONTROL_CNTR0                             0x118ul
#define FCH_XHCI_OVERCURRENT_MAP_0_CNTR0                             0x120ul
#define FCH_XHCI_PORT_CONTROL_OFFSET                                 0x130ul

// USB2.0 Lane Parameter Registers
#define FCH_XHCI_USB_20LANEPARACTL0_CNTR0                            0x11C000ul
#define FCH_XHCI_USB_20LANEPARACTL1_CNTR0                            0x11C004ul

#define SIL_RESERVED_34                                              0x1242D0ul
#define FCH_XHCI_USB31_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST           0x12C14Cul
#define FCH_XHCI_USB31_RAWLANE0_DIG_PCS_XF_RX_EQ_DELTA_IQ_OVRD_IN    0x12C064ul
#define FCH_XHCI_USB31_SUPX_DIG_LVL_OVRD_IN                          0x14003Cul
#define FCH_XHCI_USB31_SUP_DIG_LVL_OVRD_IN                           0x12003Cul
