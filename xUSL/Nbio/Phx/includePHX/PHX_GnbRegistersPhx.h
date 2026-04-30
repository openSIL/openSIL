/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_GnbRegistersPhx.h
 * @brief Top level header for NBIO Register definitions
 */

#pragma once


#define GNBREGx6C                                       0x6C
#define GNBREGx70                                      0x70

#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS)
#endif

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(HANDLE, WRAPPER, ADDRESS)   (ADDRESS + (WRAPPER->WrapId << 20))
#endif

#ifndef PORT_SPACE
  #define  PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (WRAPPER->WrapId << 20) + (PORTINDEX << 12))
#endif

#ifndef IOHC_BRIDGE_SPACE
  #define  IOHC_BRIDGE_SPACE(HANDLE, ENGINE, ADDRESS)   (ADDRESS + ((ENGINE->Type.Port.LogicalBridgeId < 6) ? \
          (ENGINE->Type.Port.LogicalBridgeId << 10) : ((1 << 13) + ((ENGINE->Type.Port.LogicalBridgeId - 6) << 10))))
#endif

#ifndef IOHC_SPACE
  #define  IOHC_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (PORTINDEX * 0x20))
#endif

#define PCIE_CAP_LIST_ADDRESS                                  0x5A

#define BIT3_OFFSET_AAAABAX      3
#define BITS_3_TO_5_MASK_AAAABAW        0x38
#define RIOMMUx16b0a0f0    0x16b0a0f0UL

#define BIT1_OFFSET_AAAABBJ      1
#define BITS_1_TO_5_MASK_AAAABBI        0x3e
#define BIT6_OFFSET_AAAABBH      6
#define BITS_6_TO_10_MASK_AAAABBG        0x7c0
#define RIOMMUx16b0e200    0x16b0e200UL

#define BIT0_OFFSET_AAAABAZ      0
#define BIT0_MASK_AAAABAY        0x1
#define BIT1_OFFSET_AAAABBB      1
#define BIT1_MASK_AAAABBA        0x2
#define RIOMMUx16b0a120    0x16b0a120UL

#define BIT6_OFFSET_AAAABBD      6
#define BIT6_MASK_AAAABBC        0x40
#define RIOMMUx16b0a0ec    0x16b0a0ecUL

#define BIT0_OFFSET_AAAABBF      0
#define BIT0_MASK_AAAABBE        0x1
#define PCIECOREx1118047c    0x1118047cUL
#define PCIECOREx1128047c    0x1128047cUL

#define GNBREGx5d978                              0x5d978UL

#define GNBREGx6a340      0x6a340UL
#define GNBREGx6a344      0x6a344UL
#define GNBREGx6a348      0x6a348UL
#define GNBREGx6a34c      0x6a34cUL
#define GNBREGx6a350      0x6a350UL
#define GNBREGx6a354      0x6a354UL
#define GNBREGx6a358      0x6a358UL
