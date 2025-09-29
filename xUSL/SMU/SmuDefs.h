/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuDefs.h
 * This file contains enums for SMU response types.
 */

#pragma once

#define SMU_ARGUMENT_SIZE              24
#define MAX_CAC_WEIGHT_NUM             23

/// SMU Response Codes
typedef enum {
  SMC_Result_Fatal = -4,
  SMC_Result_OK = 0x1,
  SMC_Result_CmdRejectedBusy = 0xFC,
  SMC_Result_CmdRejectedPrereq = 0xFD,
  SMC_Result_UnknownCmd = 0xFE,
  SMC_Result_Failed = 0xFF,
} SMC_RESULT;
