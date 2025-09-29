/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobCmn2Rev.h
 * @brief OpenSIL APOB functions
 *
 */
#include <xSIM.h>
#include <stdint.h>
#include <APOB/Common/ApobCmn.h>

#pragma once

typedef void (* APOB_CMN2REV_GET_APOB_GET_MAX_DIE_INFO) (
  SIL_CONTEXT       *SilContext,
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  );

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  APOB_CMN2REV_GET_APOB_GET_MAX_DIE_INFO  ApobGetMaxDieInfo;
} APOB_COMMON_2_REV_XFER_BLOCK;
