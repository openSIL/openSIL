/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuPhxIp2Ip.c
 * @brief PHX SMU specific transfer table functions
 *
 */
#include <SmuIp2Ip.h>
#include "SmuPhxIp2Ip.h"

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for SMU
 */
SMU_IP2IP_API mSmuPhxApi = {
  .Header = {
    .IpId = SilId_SmuClass,
    .IpVersion = 13,
  },
  .SmuServiceInitArguments = SmuServiceInitArgumentsCommon,
  .SmuServiceRequest = SmuServiceRequestPhx,
  .SmuServiceRequestOnly = SmuServiceRequestOnlyPhx,
  .SmuFirmwareTest = SmuFirmwareTestCommon,
  .SmuNotifyS3Entry = SmuNotifyS3EntryPhx,
  .SmuGetGnbHandle = SmuGetGnbHandleCommon,
  .SmuReadBrandString = SmuReadBrandStringPhx,
  .SmuReadBistInfo = SmuReadBistInfoPhx,
  .SmuReadCacWeights = SmuReadCacWeightsPhx,
  .SmuRegisterReadDie = SmuRegisterReadDie,
  .SmuRegisterWriteDie = SmuRegisterWriteDie,
  .SmuRegisterRMWDie = SmuRegisterRMWDie,
  .SmuDisableSmt = SmuDisableSmtCommon,
  .SmuGetOpnCorePresence = SmuGetOpnCorePresencePhx,
  .SmuGetOpnCorePresenceEx = SmuGetOpnCorePresenceExPhx
};
