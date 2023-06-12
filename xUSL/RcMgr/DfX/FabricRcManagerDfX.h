/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcManagerDfX.h
 * @brief Fabric MMIO/IO type definitions for RS
 *
 */
#pragma once

#include <xSIM.h>
#include <RcMgr/DfX/RcManager4-api.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include <DF/DfIp2Ip.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <ProjSocConst.h>

/// MMIO ABOVE 4G
typedef struct _FABRIC_MMIO_ABOVE_4G_QUEUE {
  uint8_t  MmioType;
  uint64_t AlignBit;
} FABRIC_MMIO_ABOVE_4G_QUEUE;

SIL_STATUS
SilSetMmioReg4 (
  DF_IP2IP_API  *DfIp2IpApi,
  uint8_t       TotalSocket,
  uint8_t       MmioPairIndex,
  uint32_t      SktNum,
  uint32_t      RbNum,
  uint64_t      BaseAddress,
  uint64_t      Length
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
  DF_IP2IP_API *DfIp2IpApi,
  uint32_t      LogSktNum,
  uint32_t      LogRbNum,
  uint32_t      *PhySktNum,
  uint32_t      *PhyRbNum,
  bool          *LogToPhyMapInit,
  FABRIC_RB_LOG_TO_PHY_MAP *pLogToPhyMap
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
