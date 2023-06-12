/**
 * @file  DfX.c
 * @brief OpenSIL DataFabric ver 4 specific initialization
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <DfIp2Ip.h>
#include <DF/Common/DfCmn2Rev.h>
#include "DfX.h"
#include "DfXBaseFabricTopology.h"
#include "DfXFabricRegisterAcc.h"
#include "DfXFabricWdt.h"
#include <DF/Common/BaseFabricTopologyCmn.h>
#include "FabricDfClkGatingInit/FabricDfClkGatingInit.h"
#include "FabricPieRasInit/FabricPieRasInit.h"
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>

static const DF_IP2IP_API DfIp2IpApiRev4 = {
  .DfGetNumberOfProcessorsPresent         =         DfXGetNumberOfProcessorsPresent,
  .DfGetNumberOfSystemDies                =         DfXGetNumberOfSystemDies,
  .DfGetNumberOfSystemRootBridges         =         DfXGetNumberOfSystemRootBridges,
  .DfGetNumberOfRootBridgesOnSocket       =         DfXGetNumberOfRootBridgesOnSocket,
  .DfGetNumberOfRootBridgesOnDie          =         DfXGetNumberOfRootBridgesOnDie,
  .DfGetDieSystemOffset                   =         DfXGetDieSystemOffset,
  .DfGetDeviceMapOnDie                    =         DfXGetDeviceMapOnDie,
  .DfGetHostBridgeBusBase                 =         DfXGetHostBridgeBusBase,
  .DfGetHostBridgeBusLimit                =         DfXGetHostBridgeBusLimit,
  .DfGetPhysRootBridgeNumber              =         DfXGetPhysRootBridgeNumber,
  .DfGetNumberOfDiesOnSocket              =         DfGetNumberOfDiesOnSocket,
  .DfGetHostBridgeSystemFabricID          =         DfGetHostBridgeSystemFabricID,
  .DfGetSystemComponentRootBridgeLocation =         DfGetSystemComponentRootBridgeLocation,
  .DfHasFch                               =         DfHasFch,
  .DfHasSmu                               =         DfHasSmu,
  .DfFindDeviceTypeEntryInMap             =         DfFindDeviceTypeEntryInMap,
  .DfFabricRegisterAccRead                =         DfXFabricRegisterAccRead,
  .DfFabricRegisterAccWrite               =         DfXFabricRegisterAccWrite,
  .DfGetWdtInfo                           =         DfXGetWdtInfo,
  .DfGetRootBridgeInfo                    =         DfGetRootBridgeInfo,
  .DfGetDieInfo                           =         DfGetDieInfo,
  .DfGetProcessorInfo                     =         DfGetProcessorInfo,
  .DfGetSystemInfo                        =         DfGetSystemInfo,
  .DfDomainXlat                           =         DfDomainXlat
};

extern DF_COMMON_2_REV_XFER_BLOCK DfCmn2Rev4Xfer;

/**
 *  FabricSocInit
 *
 * @brief Makes DfX specific initialization calls
 * @details This function makes DF init calls specific to DfX initialization.
 *          It is called after the DF common initialization is complete.
 */
void FabricSocInit(DFCLASS_INPUT_BLK* DfInputBlock)
{
  FabricClockGatingInit(DfInputBlock);
  FabricPieRasInit(DfInputBlock);
}

/**
 * DfInitApiRev4
 *
 * @brief Initialize Ip2Ip API and Cmn2Rev xfer table for DfX
 *
 */
SIL_STATUS
DfInitApiRev4 (void)
{
  SIL_STATUS Status;

  // Set Cmn2Rev table for DF
  Status = SilInitCommon2RevXferTable (SilId_DfClass, &DfCmn2Rev4Xfer);
  if (Status != SilPass) {
    return Status;
  }

  // Set Ip2Ip API for DF
  return SilInitIp2IpApi(SilId_DfClass, (void*) &DfIp2IpApiRev4);
}
