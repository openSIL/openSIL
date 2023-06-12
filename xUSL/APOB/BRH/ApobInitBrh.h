/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobInitBrh.h
 * @brief OpenSIL APOB functions
 *
 */

#pragma once

SIL_STATUS
ApobGetPhysCcdNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint8_t *PhysCcdNumber
  );

SIL_STATUS
ApobGetPhysComplexNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint8_t *PhysComplexNumber
  );

SIL_STATUS
ApobGetPhysCoreNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint8_t *PhysCoreNumber
  );

SIL_STATUS
ApobGetIsThreadEnabledBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint32_t ThreadIndex,
  bool     *IsThreadEnabled
  );

SIL_STATUS
ApobGetSubProgramBrh (
  uint32_t *SubProgram
  );

SIL_STATUS
ApobGetDimmSpdDataBrh (
  uint32_t ApobInstanceId,
  uint8_t  Socket,
  uint8_t  Channel,
  uint8_t  Dimm,
  uint32_t BufSize,
  uint8_t  *SpdBufPtr
  );

uint32_t
ApobGetMaxCpusBrh (
  void
  );

void
ApobGetMaxDieInfoBrh (
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  );

SIL_STATUS
ApobGetCcdLogToPhysMapBrh (
  uint32_t                                      Socket,
  uint32_t                                      Die,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap
  );

SIL_STATUS
ApobGetActionOnBistFailureBrh (
  uint8_t *ActionOnBistFailure
  );
