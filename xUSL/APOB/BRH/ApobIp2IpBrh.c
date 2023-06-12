/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobIp2IpBrh.c
 * @brief OpenSIL APOB functions
 *
 */
#include <xSIM-api.h>
#include <APOB/ApobIp2Ip.h>
#include "ApobIp2IpBrh.h"

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for APOB
 */
APOB_IP2IP_API mApobBrhApi = {
  .Header = {
    .IpId = SilId_ApobClass,
    .IpVersion = 0,
  },
  .ApobGetPhysCcdNumber = ApobGetPhysCcdNumberBrh,
  .ApobGetPhysComplexNumber = ApobGetPhysComplexNumberBrh,
  .ApobGetPhysCoreNumber = ApobGetPhysCoreNumberBrh,
  .ApobGetIsThreadEnabled = ApobGetIsThreadEnabledBrh,
  .ApobGetSubProgram = ApobGetSubProgramBrh,
  .ApobGetDimmSpdData = ApobGetDimmSpdDataBrh,
  .ApobGetCcdLogToPhysMap = ApobGetCcdLogToPhysMapBrh,
  .ApobGetActionOnBistFailure = ApobGetActionOnBistFailureBrh,
  .ApobGetMaxCpus = ApobGetMaxCpusBrh,
  .ApobGetMaxDieInfo = ApobGetMaxDieInfoBrh,
  .ApobAmdGetApobEntryInstance = AmdGetApobEntryInstance,
  .ApobGetApcbUpdate = ApobGetApcbUpdate
};
