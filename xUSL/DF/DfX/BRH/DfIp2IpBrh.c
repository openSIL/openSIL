/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfIp2IpBrh.c
 * @brief Declaration of data fabric BRH Ip2Ip API table
 */

#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXFabricWdt.h>
#include "BrhAbstractRegAcc.h"
#include "DfBrh.h"
#include "DfInitBrh.h"

DF_IP2IP_API DfIp2IpApiBrh = {
  .DfGetNumberOfProcessorsPresent = BrhGetNumberOfProcessorsPresent,
  .DfGetNumberOfSystemDies = DfXGetNumberOfSystemDies,
  .DfGetNumberOfSystemRootBridges = DfXGetNumberOfSystemRootBridges,
  .DfGetNumberOfRootBridgesOnSocket = DfXGetNumberOfRootBridgesOnSocket,
  .DfGetNumberOfRootBridgesOnDie = BrhGetNumberOfRootBridgesOnDie,
  .DfGetDieSystemOffset = BrhGetDieSystemOffset,
  .DfGetDeviceMapOnDie = BrhGetDeviceMapOnDie,
  .DfGetHostBridgeBusBase = BrhGetHostBridgeBusBase,
  .DfGetNumberOfBusRegions = BrhGetNumberOfBusRegions,
  .DfGetHostBridgeBusLimit = BrhGetHostBridgeBusLimit,
  .DfGetPhysRootBridgeNumber = BrhGetPhysRootBridgeNumber,
  .DfGetNumberOfDiesOnSocket = DfGetNumberOfDiesOnSocket,
  .DfGetHostBridgeSystemFabricID = DfGetHostBridgeSystemFabricID,
  .DfGetSystemComponentRootBridgeLocation = BrhGetSystemComponentRootBridgeLocation,
  .DfHasFch = BrhHasFch,
  .DfHasSmu = BrhHasSmu,
  .DfFindDeviceTypeEntryInMap = BrhFindDeviceTypeEntryInMap,
  .DfFabricRegisterAccRead = DfXFabricRegisterAccRead,
  .DfFabricRegisterAccWrite = DfXFabricRegisterAccWrite,
  .DfGetWdtInfo = DfXGetWdtInfo,
  .DfGetRootBridgeInfo = DfGetRootBridgeInfo,
  .DfGetDieInfo = DfGetDieInfo,
  .DfGetProcessorInfo = DfGetProcessorInfo,
  .DfGetSystemInfo = DfGetSystemInfo,
  .DfDomainXlat = DfXDomainXlat,
  .DfAbstractRegAcc = BrhAbstractRegAcc
};
