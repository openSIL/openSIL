/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobIp2IpPhx.h
 * @brief OpenSIL APOB functions
 *
 */
#pragma once

#include <xSIM.h>
#include <stdint.h>
#include <Apob-PHX.h>

SIL_STATUS
ApobGetPhysCcdNumberPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     ApobInstanceId,
  uint32_t     CcdIndex,
  uint8_t      *PhysCcdNumber
  );

SIL_STATUS
ApobGetPhysComplexNumberPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     ApobInstanceId,
  uint32_t     CcdIndex,
  uint32_t     CcxIndex,
  uint8_t      *PhysComplexNumber
  );

SIL_STATUS
ApobGetPhysCoreNumberPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     ApobInstanceId,
  uint32_t     CcdIndex,
  uint32_t     CcxIndex,
  uint32_t     CoreIndex,
  uint8_t      *PhysCoreNumber
  );

SIL_STATUS
ApobGetIsThreadEnabledPhx (
  SIL_CONTEXT   *SilContext,
  uint32_t      ApobInstanceId,
  uint32_t      CcdIndex,
  uint32_t      CcxIndex,
  uint32_t      CoreIndex,
  uint32_t      ThreadIndex,
  bool          *IsThreadEnabled
  );

SIL_STATUS
ApobGetSubProgramPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     *SubProgram
  );

SIL_STATUS
ApobGetDimmSpdDataPhx (
  SIL_CONTEXT   *SilContext,
  uint32_t      ApobInstanceId,
  uint8_t       Socket,
  uint8_t       Channel,
  uint8_t       Dimm,
  uint32_t      BufSize,
  uint8_t       *SpdBufPtr
  );

SIL_STATUS
ApobGetActionOnBistFailurePhx (
  SIL_CONTEXT    *SilContext,
  uint8_t        *ActionOnBistFailure
  );

SIL_STATUS
ApobGetCcdLogToPhysMapPhx (
  SIL_CONTEXT                                   *SilContext,
  uint32_t                                      Socket,
  uint32_t                                      Die,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap
  );

uint32_t
ApobGetMaxCpusPhx (
  SIL_CONTEXT       *SilContext
  );

void
ApobGetMaxDieInfoPhx (
  SIL_CONTEXT       *SilContext,
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  );

SIL_STATUS
ApobGetApcbUpdate (
  APOB_APCB_UPDATES_STRUCT  *ApobEntry,
  uint32_t                  Token,
  uint32_t                  *Value
  );

SIL_STATUS
AmdGetApobEntryInstance (
  SIL_CONTEXT       *SilContext,
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          InstanceID,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntry
  );
