/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfIp2IpPhx.c
 * @brief Declaration of data fabric PHX Ip2Ip API table
 */

#include "DfPhx.h"
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXFabricWdt.h>
#include "DfIp2IpPhx.h"
#include "DfAbstractRegAccPhx.h"

DF_IP2IP_API DfIp2IpApiPhx = {
  .DfGetNumberOfProcessorsPresent = PhxGetNumberOfProcessorsPresent,
  .DfGetNumberOfSystemDies = DfXGetNumberOfSystemDies,
  .DfGetNumberOfSystemRootBridges = DfXGetNumberOfSystemRootBridges,
  .DfGetNumberOfRootBridgesOnSocket = DfXGetNumberOfRootBridgesOnSocket,
  .DfGetNumberOfRootBridgesOnDie = DfXGetNumberOfRootBridgesOnDie,
  .DfGetDieSystemOffset = PhxGetDieSystemOffset,
  .DfGetDeviceMapOnDie = PhxGetDeviceMapOnDie,
  .DfGetHostBridgeBusBase = PhxGetHostBridgeBusBase,
  .DfGetNumberOfBusRegions = PhxGetNumberOfBusRegions,
  .DfGetHostBridgeBusLimit = PhxGetHostBridgeBusLimit,
  .DfGetPhysRootBridgeNumber = PhxGetPhysRootBridgeNumber,
  .DfGetNumberOfDiesOnSocket = DfGetNumberOfDiesOnSocket,
  .DfGetHostBridgeSystemFabricID = DfGetHostBridgeSystemFabricID,
  .DfGetSystemComponentRootBridgeLocation = PhxGetSystemComponentRootBridgeLocation,
  .DfHasFch = PhxHasFch,
  .DfHasSmu = PhxHasSmu,
  .DfFindDeviceTypeEntryInMap = PhxFindDeviceTypeEntryInMap,
  .DfFabricRegisterAccRead = DfXFabricRegisterAccRead,
  .DfFabricRegisterAccWrite = DfXFabricRegisterAccWrite,
  .DfGetWdtInfo = DfXGetWdtInfo,
  .DfGetRootBridgeInfo = DfGetRootBridgeInfo,
  .DfGetDieInfo = DfGetDieInfo,
  .DfGetProcessorInfo = DfGetProcessorInfo,
  .DfGetSystemInfo = DfGetSystemInfo,
  .DfDomainXlat = DfDomainXlatPhx,
  .DfAbstractRegAcc = PhxAbstractRegAcc
};
