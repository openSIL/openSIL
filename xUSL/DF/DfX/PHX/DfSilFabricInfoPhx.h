/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfSilFabricInfoPhx.h
 * @brief Phx specific data fabric defines
 *
 */

#pragma once

#define MAX_CCX_PER_CCM            1
#define  PHX_CS0_FABRIC_ID         0
#define  PHX_CS1_FABRIC_ID         1
#define  PHX_CS2_FABRIC_ID         2
#define  PHX_CS3_FABRIC_ID         3
#define  PHX_GCM0_FABRIC_ID       8
#define  PHX_GCM1_FABRIC_ID       9
#define  PHX_GCM2_FABRIC_ID       0xA
#define  PHX_GCM3_FABRIC_ID       0xB
#define  PHX_CCM0_FABRIC_ID       0xC
#define  PHX_NCM0_FABRIC_ID       0xE
#define  PHX_NCM1_FABRIC_ID       0xF
#define  PHX_NCS0_FABRIC_ID       0x10
#define  PHX_NCS1_FABRIC_ID       0x11
#define  PHX_IOM0_FABRIC_ID       0x12
#define  PHX_IOS0_FABRIC_ID       0x13
#define  PHX_PIE_FABRIC_ID        0x14
#define  PHX_IOM0_INSTANCE_ID     0xE
#define  PHX_GCM0_INSTANCE_ID     4
#define  PHX_GCM1_INSTANCE_ID     5
#define  PHX_GCM2_INSTANCE_ID     6
#define  PHX_GCM3_INSTANCE_ID     7
#define  PHX_IPU0_INSTANCE_ID     9
#define  PHX_NCS0_INSTANCE_ID     0xC
#define  PHX_NCS1_INSTANCE_ID     0xD

#define  PHX_CS0_INSTANCE_ID       0
#define  PHX_CS1_INSTANCE_ID       1
#define  PHX_CS2_INSTANCE_ID       2
#define  PHX_CS3_INSTANCE_ID       3
#define  PHX_CCM0_INSTANCE_ID     8
#define  PHX_NCM0_INSTANCE_ID     0xA
#define  PHX_NCM1_INSTANCE_ID     0xB
#define  PHX_MMHUB_INSTANCE_ID    PHX_NCM0_INSTANCE_ID
#define  PHX_DCE_INSTANCE_ID      PHX_NCM1_INSTANCE_ID
#define  PHX_IOS0_INSTANCE_ID     0xF
#define  PHX_PIE_INSTANCE_ID      0x10
#define  PHX_SPF0_INSTANCE_ID    0x11
#define  PHX_SPF1_INSTANCE_ID    0x12
#define  PHX_SPF2_INSTANCE_ID    0x13
#define  PHX_SPF3_INSTANCE_ID    0x14
#define  PHX_TCDX0_INSTANCE_ID    0x15
#define  PHX_TCDX1_INSTANCE_ID    0x16
#define  PHX_TCDX2_INSTANCE_ID    0x17
#define  PHX_TCDX3_INSTANCE_ID    0x18
#define  PHX_TCDX4_INSTANCE_ID    0x19
#define  PHX_TCDX5_INSTANCE_ID    0x1A

#define  PHX_NUM_CS_BLOCKS        4
#define  PHX_NUM_CS_UMC_BLOCKS    4
#define  PHX_NUM_CCM_BLOCKS        1

#define PHX_NUMBER_OF_BUS_REGIONS         0x8
#define PHX_BUS_REGION_REGISTER_OFFSET    0x8

#define PHX_NUMBER_OF_DRAM_REGIONS        0x2
#define PHX_MAX_SOCKETS                   1   ///< Max number of sockets in system

#define PHX_FABRIC_ID_SOCKET_SHIFT        0
