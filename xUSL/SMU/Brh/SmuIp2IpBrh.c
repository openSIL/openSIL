/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuIp2IpBrh.c
 * @brief OpenSIL SMU BRH I2I API Table
 *
 */

#include <xSIM-api.h>
#include <SMU/SmuIp2Ip.h>
#include <SMU/Common/SmuCommon.h>
#include "SmuIp2IpBrh.h"

SMU_IP2IP_API mSmuBrhApi = {
  .Header = {
    .IpId = SilId_SmuClass,
    .IpVersion = 14,
  },
  .SmuServiceInitArguments = SmuServiceInitArgumentsCommon,
  .SmuServiceRequest = SmuServiceRequestBrh,
  .SmuServiceRequestOnly = SmuServiceRequestOnlyBrh,
  .SmuFirmwareTest = SmuFirmwareTestCommon,
  .SmuNotifyS3Entry = SmuNotifyS3EntryBrh,
  .SmuGetGnbHandle = SmuGetGnbHandleCommon,
  .SmuReadBrandString = SmuReadBrandStringBrh,
  .SmuLaunchThread = SmuLaunchThreadBrh,
  .SmuReadBistInfo = SmuReadBistInfoBrh,
  .SmuReadCacWeights = SmuReadCacWeightsBrh,
  .SmuRegisterReadDie = SmuRegisterReadDie,
  .SmuRegisterWriteDie = SmuRegisterWriteDie,
  .SmuRegisterRMWDie = SmuRegisterRMWDie,
  .SmuDisableSmt = SmuDisableSmtCommon,
  .SmuGetOpnCorePresence = SmuGetOpnCorePresenceBrh,
  .SmuGetOpnCorePresenceEx = SmuGetOpnCorePresenceExBrh
};
