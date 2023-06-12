/**
 * @file  FabricRcInitDfX.h
 * @brief Fabric MMIO/IO init prototypes for DFX
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include "RcManager4-api.h"
#include <RcMgr/FabricResourceManager.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

/// MMIO ABOVE 4G
typedef struct _FABRIC_MMIO_ABOVE_4G_QUEUE {
  uint8_t  MmioType;
  uint64_t AlignBit;
} FABRIC_MMIO_ABOVE_4G_QUEUE;

void
SilSetMmioReg4 (
  uint8_t   TotalSocket,
  uint8_t   MmioPairIndex,
  uint32_t  SktNum,
  uint32_t  RbNum,
  uint64_t  BaseAddress,
  uint64_t  Length
  );

void
SilSetIoReg4 (
  uint8_t  TotalSocket,
  uint8_t  RegIndex,
  uint32_t SktNum,
  uint32_t RbNum,
  uint32_t IoBase,
  uint32_t IoSize
  );

void
SilAdditionalMmioSetting4 (
  DFX_RCMGR_INPUT_BLK *SilData,
  uint64_t            BottomOfCompat,
  bool                ReservedRegionAlreadySet
  );

void
DfXSilGetPhySktRbNum (
  uint32_t  LogSktNum,
  uint32_t  LogRbNum,
  uint32_t  *PhySktNum,
  uint32_t  *PhyRbNum,
  bool *LogToPhyMapInit,
  FABRIC_RB_LOG_TO_PHY_MAP *pLogToPhyMap
  );

void
SilSetCfgAddrMapReg4 (
  DFX_RCMGR_INPUT_BLK *SilData,
  uint8_t             RbPerDie,
  uint8_t             RegIndex,
  uint32_t            SktNum,
  uint32_t            RbNum,
  uint16_t            Base,
  uint16_t            Limit
  );


SIL_STATUS
SilInitPciBusBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK *SilData
  );

SIL_STATUS
SilInitMmioEqually4 (
  DFX_RCMGR_INPUT_BLK *SilData
  );

SIL_STATUS
SilInitIoEqually4 (
  DFX_RCMGR_INPUT_BLK *SilData
  );

SIL_STATUS
SilInitIoBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  bool                    SetDfRegisters
  );

SIL_STATUS
SilInitMmioBasedOnNvVariable4 (
  DFX_RCMGR_INPUT_BLK     *SilData,
  FABRIC_ADDR_SPACE_SIZE  *SpaceStatus,
  bool                    SetDfRegisters
  );

SIL_STATUS
InitializeResourceManagerDfXTp1 (void);

SIL_STATUS
InitializeResourceManagerDfXTp2 (void);

SIL_STATUS
InitializeResourceManagerDfXTp3 (void);


