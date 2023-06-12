/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfCmn2Brh.c
 * @brief Declaration of data fabric BRH Cmn2Rev xfer table
 */

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <DF/Df.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include "DfBrh.h"
#include "BrhAbstractRegAcc.h"
#include "DfInitBrh.h"

DF_COMMON_2_REV_XFER_BLOCK DfCmn2RevBrhXfer = {
  .DfFabricRegisterAccRead = DfXFabricRegisterAccRead,
  .DfFabricRegisterAccWrite = DfXFabricRegisterAccWrite,
  .DfGetNumberOfProcessorsPresent = BrhGetNumberOfProcessorsPresent,
  .DfGetNumberOfSystemDies = DfXGetNumberOfSystemDies,
  .DfGetNumberOfSystemRootBridges = DfXGetNumberOfSystemRootBridges,
  .DfGetNumberOfRootBridgesOnSocket = DfXGetNumberOfRootBridgesOnSocket,
  .DfGetNumberOfRootBridgesOnDie = BrhGetNumberOfRootBridgesOnDie,
  .DfGetDieSystemOffset = BrhGetDieSystemOffset,
  .DfGetDeviceMapOnDie = BrhGetDeviceMapOnDie,
  .DfGetNumberOfBusRegions = BrhGetNumberOfBusRegions,
  .DfGetHostBridgeBusBase = BrhGetHostBridgeBusBase,
  .DfGetHostBridgeBusLimit = BrhGetHostBridgeBusLimit,
  .DfGetPhysRootBridgeNumber = BrhGetPhysRootBridgeNumber,
  .DfFindDeviceTypeEntryInMap = BrhFindDeviceTypeEntryInMap,
  .DfAbstractRegAcc = BrhAbstractRegAcc,
  .DfGetGlblCtrlInstanceIds = BrhGetGlblCtrlInstanceIds,
  .DfGetNpsMapData = BrhGetNpsMapData,
  .DfGetPhysDomainOfCcd = BrhGetPhysDomainOfCcd,
  .DfBuildCcdInfo = BrhBuildCcdInfo,
  .DfGetSystemComponentRootBridgeLocation = BrhGetSystemComponentRootBridgeLocation,
  .DfHasFch = BrhHasFch,
  .DfHasSmu = BrhHasSmu
};
