/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RcMgrCmn2Rev.h
 * This file defines the types of all functions contained within Resource Managers's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once
#include <RcMgr/DfX/RcManager-api.h>
#include <stdint.h>

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  uint8_t   Version;
} RCMGR_XFER_TABLE_HEADER;

typedef struct {
  RCMGR_XFER_TABLE_HEADER        Header;
} RCMGR_COMMON_2_REV_XFER_BLOCK;
