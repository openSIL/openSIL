/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_IOMMUL2.h
 * @brief This file contains IOMMUL2 Register definitions
 */

#pragma once

#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET              0
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK                0x1
#define IOMMUL2x157000c0                             0x157000c0UL

#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET     0
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK       0x1
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET  1
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK    0x2
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET    2
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK      0x4
#define IOMMUL2x157000cc                            0x157000ccUL

#define L2_TW_CONTROL_TWForceCoherent_OFFSET                   6
#define L2_TW_CONTROL_TWForceCoherent_MASK                     0x40
#define IOMMUL2x13f01150                                     0x13f01150UL

#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET              0
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK                0x1
#define IOMMUL2x13f01200                             0x13f01200UL

#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET     0
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK       0x1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET  1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK    0x2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET     2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK       0x4
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET    3
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK      0x8
#define IOMMUL2x13f01240                            0x13f01240UL

#define BIT1_OFFSET_AAAAASO        1
#define BITS_1_TO_5_MASK_AAAAASN          0x3e
#define BIT6_OFFSET_AAAAASM     6
#define BITS_6_TO_10_MASK_AAAAASL       0x7c0
#define IOMMUL2x13f0130c                               0x13f0130cUL

#define IOMMUL2x13f00078                         0x13f00078UL

#define BIT0_OFFSET_AAAAARS      0
#define BITS_0_TO_15_MASK_AAAAARR        0xffff
#define IOMMUL2x13f0115c                            0x13f0115cUL

#define BIT44_OFFSET_AAAAASR                      0x44

#define SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS                      0x13f00080UL
