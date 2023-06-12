/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file    RasCmn2Rev.h
 * @details This file defines the types of all functions contained within RAS's Cmn2Rev
 *          xfer table, as well as the table itself as a struct containing pointers to
 *          these functions
 *
 */

#pragma once
#include <stdint.h>

typedef uint8_t (*GET_CORE_MCA_SMN_ADDR_BYTE) (
  uint16_t    McaType
  );

/**
 * RAS IP Transfer (xfer) Table
 *
 * This structure contains an table of IP specific functions that need to run from RAS common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  uint8_t   Version;
} RAS_XFER_TABLE_HEADER;

typedef struct {
  RAS_XFER_TABLE_HEADER       Header;
  GET_CORE_MCA_SMN_ADDR_BYTE  GetCoreMcaSmnAddrByte;
} RAS_XFER_TABLE;
