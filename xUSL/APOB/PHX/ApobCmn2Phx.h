/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobCmn2Phx.h
 * @brief OpenSIL APOB functions
 *
 */
#pragma once

#include <xSIM.h>
#include <stdint.h>
#include <APOB/Common/ApobCmn.h>

void
ApobGetMaxDieInfoPhx (
  SIL_CONTEXT       *SilContext,
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  );
