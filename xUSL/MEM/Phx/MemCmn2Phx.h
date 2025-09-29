/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemCmn2Phx.h
 * @brief OpenSIL MEM C2R API Function declaration.
 *
 */

#pragma once

#include <SilCommon.h>
#include <MEM/Common/MemTableTypes.h>
#include <MEM/MemClass-api.h>
#include <APOB/Common/ApobCmn.h>

HOST_TO_APCB_CHANNEL_XLAT *
GetChannelXlatTablePhx (void);

SIL_STATUS
ConfigureMemInfoPhx (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummary
  );

SIL_STATUS
PopulateSmbiosMemInfoPhx (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  );

void
ConfigureTable17DimmPresentPhx (
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  );
