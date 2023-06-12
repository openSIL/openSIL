/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobIp2Ip.h
 * This file contains the definition of the APOB Ip 2 Ip API.
 * This API provides other openSIL IPs with any APOB functionality or data
 * they require while also abstracting revision specific differences.
 */
#pragma once

#include <xSIM-api.h>
#include <APOB/Common/ApobCmn.h>

typedef struct {
  SIL_DATA_BLOCK_ID     IpId;       ///< Block Id for this ip
  uint8_t               IpVersion;  ///< Ip version in numerical format
} APOB_IP2IP_API_HEADER;

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef  SIL_STATUS (* APOB_GET_APCB_UPDATE)(
  APOB_APCB_UPDATES_STRUCT  *ApobEntry,
  uint32_t                  Token,
  uint32_t                  *Value
  );

typedef  SIL_STATUS (* APOB_GET_PHYS_CCD_NUMBER)(
  uint32_t       ApobInstanceId,
  uint32_t       CcdIndex,
  uint8_t        *PhysCcdNumber
  );

typedef  SIL_STATUS (* APOB_GET_PHYS_COMPLEX_NUMBER)(
  uint32_t      ApobInstanceId,
  uint32_t      CcdIndex,
  uint32_t      CcxIndex,
  uint8_t       *PhysComplexNumber
  );

typedef  SIL_STATUS (* APOB_GET_PHYS_CORE_NUMBER)(
  uint32_t      ApobInstanceId,
  uint32_t      CcdIndex,
  uint32_t      CcxIndex,
  uint32_t      CoreIndex,
  uint8_t       *PhysCoreNumber
  );

typedef  SIL_STATUS (* APOB_GET_THREAD_ENABLE)(
  uint32_t      ApobInstanceId,
  uint32_t      CcdIndex,
  uint32_t      CcxIndex,
  uint32_t      CoreIndex,
  uint32_t      ThreadIndex,
  bool          *IsThreadEnabled
  );

typedef  SIL_STATUS (* APOB_GET_SUBPROGRAM)(
  uint32_t      *SubProgram
  );

typedef  SIL_STATUS (* APOB_GET_DIMM_SPD_DATA)(
  uint32_t      ApobInstanceId,
  uint8_t       Socket,
  uint8_t       Channel,
  uint8_t       Dimm,
  uint32_t      BufSize,
  uint8_t       *SpdBufPtr
  );

typedef SIL_STATUS (* APOB_GET_CCDLOG_PHYMAP) (
  uint32_t                                      Socket,
  uint32_t                                      Die,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap
  );

typedef SIL_STATUS (*APOB_GET_ACTIONON_BISTFAILURE) (
  uint8_t        *ActionOnBistFailure
  );

typedef uint32_t (* APOB_GET_MAXCPUS)(
  void
  );

typedef void (* APOB_GET_SOC_MAX_DIE_INFO)(
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  );

typedef SIL_STATUS (* APOB_GET_ENTRY_INSTANCE)(
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          InstanceID,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntry
  );

typedef struct {
  APOB_IP2IP_API_HEADER          Header;
  APOB_GET_PHYS_CCD_NUMBER       ApobGetPhysCcdNumber;
  APOB_GET_PHYS_COMPLEX_NUMBER   ApobGetPhysComplexNumber;
  APOB_GET_PHYS_CORE_NUMBER      ApobGetPhysCoreNumber;
  APOB_GET_THREAD_ENABLE         ApobGetIsThreadEnabled;
  APOB_GET_SUBPROGRAM            ApobGetSubProgram;
  APOB_GET_DIMM_SPD_DATA         ApobGetDimmSpdData;
  APOB_GET_CCDLOG_PHYMAP         ApobGetCcdLogToPhysMap;
  APOB_GET_ACTIONON_BISTFAILURE  ApobGetActionOnBistFailure;
  APOB_GET_MAXCPUS               ApobGetMaxCpus;
  APOB_GET_SOC_MAX_DIE_INFO      ApobGetMaxDieInfo;
  APOB_GET_ENTRY_INSTANCE        ApobAmdGetApobEntryInstance;
  APOB_GET_APCB_UPDATE           ApobGetApcbUpdate;
} APOB_IP2IP_API;
