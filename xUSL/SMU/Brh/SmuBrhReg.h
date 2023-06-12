/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuBrhReg.h
 * @brief openSIL SMU BRH register definitions
 *
 */

#pragma once

#include <stdint.h>
#include <SilSocLogicalId.h>
#define BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS 0x3B109C4ul

#define BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS 0x3B109C8ul

#define BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS 0x3B109CCUL

#define BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS 0x3B109D0UL

#define BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS 0x3B109D4ul

#define BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS 0x3B109D8ul

#define BRH_MP1_C2PMSG_MESSAGE_ADDRESS    0x3B10930ul

#define BRH_MP1_C2PMSG_RESPONSE_ADDRESS   0x3B1097Cul


typedef union {
  struct {
    uint32_t :13; // Reserved
    uint32_t field_bits_13_to_24 :12;
    uint32_t :7; // Reserved
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0044;


#define SIL_RSVD_ADDR_03820000    0x03820000  //Non Secure SMU FUSE SMN Public Base Address

#define SIL_RSVD_ADDR_000A02BC      0x000A02BC  //FUSE_NON_SECURE replace start address for S2 region

#define SIL_RSVD_ADDR_000A081C    0x000A081C  //UMC Harvest Fuse

#define SMUFUSE_UMCHARVEST_PUBLICADDR (SIL_RSVD_ADDR_03820000 + SIL_RSVD_ADDR_000A081C - \
        SIL_RSVD_ADDR_000A02BC)    //UMC Harvest Fuse Public SMN Address

#define SIL_RSVD_ADDR_203C0020       0x203C0020UL
