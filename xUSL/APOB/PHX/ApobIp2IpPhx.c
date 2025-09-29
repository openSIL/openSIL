/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobIp2IpPhx.c
 * @brief OpenSIL APOB functions
 *
 */
#include <xSIM-api.h>
#include <APOB/ApobIp2Ip.h>
#include "ApobIp2IpPhx.h"

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for APOB
 */
APOB_IP2IP_API mApobPhxApi = {
  .Header = {
    .IpId = SilId_ApobClass,
    .IpVersion = 0,
  },
  .ApobGetPhysCcdNumber = ApobGetPhysCcdNumberPhx,
  .ApobGetPhysComplexNumber = ApobGetPhysComplexNumberPhx,
  .ApobGetPhysCoreNumber = ApobGetPhysCoreNumberPhx,
  .ApobGetIsThreadEnabled = ApobGetIsThreadEnabledPhx,
  .ApobGetSubProgram = ApobGetSubProgramPhx,
  .ApobGetDimmSpdData = ApobGetDimmSpdDataPhx,
  .ApobGetCcdLogToPhysMap = ApobGetCcdLogToPhysMapPhx,
  .ApobGetActionOnBistFailure = ApobGetActionOnBistFailurePhx,
  .ApobGetMaxCpus = ApobGetMaxCpusPhx,
  .ApobGetMaxDieInfo = ApobGetMaxDieInfoPhx,
  .ApobAmdGetApobEntryInstance = AmdGetApobEntryInstance,
  .ApobGetApcbUpdate = ApobGetApcbUpdate,
};
