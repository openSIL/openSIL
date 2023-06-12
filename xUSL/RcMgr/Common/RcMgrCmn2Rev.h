/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RcMgrCmn2Rev.h
 * This file defines the types of all functions contained within Resource Manager's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once
#include <RcMgr/DfX/RcManager4-api.h>
#include <stdint.h>

// Common function type definitions of Cmn2Rev functions

typedef uint8_t (*GET_MAX_SYS_RB_CNT) (void);

typedef uint8_t (*GET_NUM_BUS_REGIONS) (void);

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  uint8_t   Version;
} RCMGR_XFER_TABLE_HEADER;

typedef struct {
  RCMGR_XFER_TABLE_HEADER        Header;
} RCMGR_COMMON_2_REV_XFER_BLOCK;
