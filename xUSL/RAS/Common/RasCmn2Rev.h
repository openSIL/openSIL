/**
 * @file    RasCmn2Rev.h
 * @details This file defines the types of all functions contained within RAS's Cmn2Rev
 *          xfer table, as well as the table itself as a struct containing pointers to
 *          these functions
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <stdint.h>

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
  RAS_XFER_TABLE_HEADER Header;
} RAS_XFER_TABLE;
