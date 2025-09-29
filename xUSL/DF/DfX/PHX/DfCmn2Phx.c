/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfCmn2Phx.c
 * @brief Declaration of data fabric PHX Cmn2Rev xfer table
 */

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <DF/Df.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/PHX/DfAcpiDomainInfoPhx.h>
#include "DfPhx.h"
#include "DfCmn2Phx.h"
#include "DfAbstractRegAccPhx.h"

DF_COMMON_2_REV_XFER_BLOCK DfCmn2RevPhxXfer = {
  .DfFabricRegisterAccRead = DfXFabricRegisterAccRead,
  .DfFabricRegisterAccWrite = DfXFabricRegisterAccWrite,
  .DfGetNumberOfProcessorsPresent = PhxGetNumberOfProcessorsPresent,
  .DfGetNumberOfSystemDies = DfXGetNumberOfSystemDies,
  .DfGetNumberOfSystemRootBridges = DfXGetNumberOfSystemRootBridges,
  .DfGetNumberOfRootBridgesOnSocket = DfXGetNumberOfRootBridgesOnSocket,
  .DfGetNumberOfRootBridgesOnDie = DfXGetNumberOfRootBridgesOnDie,
  .DfGetDieSystemOffset = PhxGetDieSystemOffset,
  .DfGetDeviceMapOnDie = PhxGetDeviceMapOnDie,
  .DfGetNumberOfBusRegions = PhxGetNumberOfBusRegions,
  .DfGetHostBridgeBusBase = PhxGetHostBridgeBusBase,
  .DfGetHostBridgeBusLimit = PhxGetHostBridgeBusLimit,
  .DfGetPhysRootBridgeNumber = PhxGetPhysRootBridgeNumber,
  .DfFindDeviceTypeEntryInMap = PhxFindDeviceTypeEntryInMap,
  .DfFindComponentLocationMap = PhxFindComponentLocationMap,
  .DfAbstractRegAcc = PhxAbstractRegAcc,
  .DfGetGlblCtrlInstanceIds = PhxGetGlblCtrlInstanceIds,
  .DfGetNpsMapData = PhxGetNpsMapData,
  .DfGetPhysDomainOfCcd = PhxGetPhysDomainOfCcd,
  .DfBuildCcdInfo = PhxBuildCcdInfo,
  .DfGetSystemComponentRootBridgeLocation = PhxGetSystemComponentRootBridgeLocation,
  .DfHasFch = PhxHasFch,
  .DfHasSmu = PhxHasSmu
};
