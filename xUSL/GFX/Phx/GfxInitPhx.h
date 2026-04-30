/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxInitPhx.h
 *  @brief GFX function prototypes, structs, and defines
 */

#pragma once

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <GFX/Common/GfxV4.h>

#define NUM_DDI_PORTS 5

typedef struct {
  PCIE_CONNECTOR_TYPE  ConnectorType;
  uint8_t              DisplayDeviceEnum;
  uint16_t             ConnectorEnum;
  uint16_t             EncoderEnum;
  uint8_t              ConnectorIndex;
} EXT_CONNECTOR_INFO;

typedef struct {
  uint8_t   DisplayDeviceEnum;
  uint8_t   DeviceIndex;
  uint16_t  DeviceTag;
  uint16_t  DeviceAcpiEnum;
} EXT_DISPLAY_DEVICE_INFO;

typedef struct {
  SIL_STATUS          Status;
  uint8_t             DisplayDeviceEnum;
  uint8_t             RequestedPriorityIndex;
  uint8_t             CurrentPriorityIndex;
  PCIe_ENGINE_CONFIG  *Engine;
} CONNECTOR_ENUM_INFO;

void
GfxIntegratedDebugDumpDisplayPath (
  EXT_DISPLAY_PATH *DisplayPath
  );

SIL_STATUS
GfxIntegratedEnumerateAllConnectorsPhx (
  SIL_CONTEXT                 *SilContext,
  GFXCLASS_INPUT_BLK          *InputBlk,
  EXT_DISPLAY_PATH            *DisplayPathList
  );

void
PopulateSystemInfoTablePhx (
  SIL_CONTEXT                     *SilContext,
  GFXCLASS_INPUT_BLK              *InputBlk,
  ATOM_FUSION_SYSTEM_INFO_V6      *SystemInfoTable
  );

SIL_STATUS
GfxInitPhx (
  SIL_CONTEXT         *SilContext,
  GFXCLASS_INPUT_BLK  *InputBlk
  );
