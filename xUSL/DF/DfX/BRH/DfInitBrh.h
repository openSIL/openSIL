/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfInitBrh.h
 * @brief Data fabric silicon init definitions for BRH.
 */

#pragma once

#include <Sil-api.h>
#include <DF/DfClass-api.h>
#include <stdint.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

// Base Fabric Topology functions for the I2I Api

uint32_t
BrhGetNumberOfRootBridgesOnDie (
  uint32_t Socket,
  uint32_t Die
  );

uint32_t
BrhGetNumberOfProcessorsPresent (void);

uint32_t
BrhGetDieSystemOffset (
  uint32_t Socket,
  uint32_t Die
  );

const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
BrhGetDeviceMapOnDie (
  uint32_t Socket,
  uint32_t Die
  );

uint8_t
BrhGetNumberOfBusRegions (void);

uint32_t
BrhGetHostBridgeBusBase (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

uint32_t
BrhGetHostBridgeBusLimit (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

uint32_t
BrhGetPhysRootBridgeNumber (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
BrhFindDeviceTypeEntryInMap (
  uint32_t            Socket,
  uint32_t            Die,
  FABRIC_DEVICE_TYPE  Type
  );

// Functions from DfInitBrh.c
void
FabricSocInitBrh (
  DFCLASS_INPUT_BLK *DfInputBlock
  );

SIL_STATUS
SilPrepareTopologyMap (
  SIL_RESERVED_STRUCT_0009  *FabricTopologyMap
  );

const uint32_t *
BrhGetGlblCtrlInstanceIds (void);

void
BrhGetNpsMapData (
  uint32_t *Nps0,
  uint32_t *Nps1,
  uint32_t *Nps2,
  uint32_t *Nps4
  );

uint32_t
BrhGetPhysDomainOfCcd (
  uint32_t            Socket,
  uint32_t            PhysCcd,
  uint32_t            NumberOfPhysicalDomains,
  void *DomainInfo
  );

void
BrhBuildCcdInfo (
  uint32_t            SocketCount,
  uint32_t            DieCount,
  void *DomainInfo
  );

bool
BrhHasFch (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

bool
BrhHasSmu (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  );

bool
BrhGetSystemComponentRootBridgeLocation (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION *Location
  );
