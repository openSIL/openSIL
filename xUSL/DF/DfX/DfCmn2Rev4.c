/**
 * @file  DfCmn2Rev4.c
 * @brief Declaration of DfX Cmn2Rev transfer block
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <DF/Common/DfCmn2Rev.h>
#include "DfXBaseFabricTopology.h"
#include "DfXFabricRegisterAcc.h"
#include "FabricAcpiDomain/FabricAcpiDomainInfo.h"

DF_COMMON_2_REV_XFER_BLOCK DfCmn2Rev4Xfer = {
    .DfFabricRegisterAccRead                =         DfXFabricRegisterAccRead,
    .DfFabricRegisterAccWrite               =         DfXFabricRegisterAccWrite,
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
    .DfFindComponentLocationMap             =         DfXFindComponentLocationMap,
    .DfBaseFabricTopologyConstructor        =         DfXBaseFabricTopologyConstructor,
    .DfBuildDomainInfo                      =         DfXBuildDomainInfo
};
