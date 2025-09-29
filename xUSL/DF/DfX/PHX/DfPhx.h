/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfPhx.h
 * @brief Phx entrypoint and API init function declarations
 */

#pragma once

#include <xSIM.h>
#include <DF/DfClass-api.h>
#include <DF/DfX/PHX/DfAcpiDomainInfoPhx.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

#define DF_DATA_SIZE_PHX sizeof (DFCLASS_INPUT_BLK) + sizeof (DF_PHX_DOMAIN_INFO_BLK)

SIL_STATUS
DfSetInputBlkPhx (
  void
  );
SIL_STATUS
InitializeApiDfXPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeDfXPhxTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeDfXPhxTp2 (
  void
  );

SIL_STATUS
InitializeDfXPhxTp3 (
  void
  );

uint32_t
PhxGetNumberOfProcessorsPresent (
  SIL_CONTEXT          *SilContext
  );

uint32_t
PhxGetDieSystemOffset (
  uint32_t Socket,
  uint32_t Die
  );

const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
PhxGetDeviceMapOnDie (
  uint32_t Socket,
  uint32_t Die
  );

uint8_t
PhxGetNumberOfBusRegions (
  void
  );

uint32_t
PhxGetHostBridgeBusBase (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       Index
  );

uint32_t
PhxGetHostBridgeBusLimit (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       Index
  );

uint32_t
PhxGetPhysRootBridgeNumber (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
PhxFindDeviceTypeEntryInMap (
  uint32_t            Socket,
  uint32_t            Die,
  FABRIC_DEVICE_TYPE  Type
  );

const
COMPONENT_LOCATION *
PhxFindComponentLocationMap (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  );

// Functions from DfInitPhx.c
const
uint32_t *
PhxGetGlblCtrlInstanceIds (
  void
  );

void
PhxGetNpsMapData (
  uint32_t *Nps0,
  uint32_t *Nps1,
  uint32_t *Nps2,
  uint32_t *Nps4
  );

uint32_t
PhxGetPhysDomainOfCcd (
  uint32_t             Socket,
  uint32_t             PhysCcd,
  uint32_t             NumberOfPhysicalDomains,
  void *DomainInfo
  );

SIL_STATUS
DfXSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
DfDomainXlatPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Ccx,
  uint32_t *Domain
  );

void
DfPhxCsInit (
  SIL_CONTEXT *SilContext
  );

void
DfPhxRasInit (
  SIL_CONTEXT        *SilContext,
  DFCLASS_INPUT_BLK *DfData
  );

SIL_STATUS
DfBuildDomainInfoPhx (
  SIL_CONTEXT  *SilContext
  );

void
FabricSocInitPhx (
  SIL_CONTEXT        *SilContext,
  DFCLASS_INPUT_BLK *DfInputBlock
  );

void
PhxBuildCcdInfo (
  uint32_t             SocketCount,
  uint32_t             DieCount,
  void                 *DomainInfo
  );
