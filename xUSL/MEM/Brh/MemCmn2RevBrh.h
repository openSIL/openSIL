/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemInitBrh.h
 * @brief OpenSIL MEM C2R API Function declaration.
 *
 */

#pragma once

#include <SilCommon.h>
#include <xPRF-api.h>
#include <MEM/Common/MemTableTypes.h>
#include <MEM/MemClass-api.h>
#include <ApobCmn.h>

HOST_TO_APCB_CHANNEL_XLAT *
GetChannelXlatTableBrh (void);

SIL_STATUS
ConfigureMemInfoBrh (
  AMD_MEMORY_SUMMARY *MemSummary
  );

SIL_STATUS
PopulateSmbiosMemInfoBrh (
  SIL_DMI_INFO *DmiInfoTable
  );

void
ConfigureTable17DimmPresentBrh (
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  );
