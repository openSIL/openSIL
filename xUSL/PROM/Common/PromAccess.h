/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromInit.h
 *  @brief All PROM related defines and structures
 */

#pragma once

#include <SilCommon.h>
#include <PROM/PromClass-api.h>

typedef struct {
  uint32_t GppBus;
  uint32_t UspBus;
  uint32_t XhciDspBus;
  uint32_t SataDspBus;
  uint32_t GppMmio;
  uint32_t UspMmio;
  uint32_t XhciDspMmio;
  uint32_t XhciMmio;
  uint8_t  GppCmd;
  uint8_t  UspCmd;
  uint8_t  XhciDspCmd;
  uint8_t  SataDspCmd;
  uint8_t  XhciCmd;
  uint8_t  SataCmd;
  uint8_t  GppAspm;
  uint8_t  UspAspm;
  uint8_t  XhciDspAspm;
  uint8_t  SataDspAspm;
  uint8_t  XhciAspm;
  uint8_t  SataAspm;
  uint16_t PromBus;
} PROM_PCI_SAVE_RESTORE_TABLE;

uint8_t
Prom21XhciReadByteV2 (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t XhciMmio,
  uint32_t Address
  );

uint8_t
Prom21XhciReadByte (
  uint32_t XhciMmio,
  uint32_t Address
  );

uint16_t
Prom21XhciReadWord (
  uint32_t XhciMmio,
  uint32_t Address
  );

uint32_t
Prom21XhciReadDWord (
  uint32_t XhciMmio,
  uint32_t Address
  );

void 
Prom21XhciWriteByte (
  uint32_t XhciMmio,
  uint32_t Address,
  uint8_t  Data
  );

void 
Prom21XhciWriteWord (
  uint32_t XhciMmio,
  uint32_t Address,
  uint16_t Data
  );

void 
Prom21XhciWriteDWord (
  uint32_t XhciMmio,
  uint32_t Address,
  uint32_t Data
  );

void
SaveInitPromBus (
  PROMCLASS_DATA_BLK            *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE   *PtPciTable,
  uint16_t                      CheckBit
  );

void
RestorePromBus (
  PROMCLASS_DATA_BLK              *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE     *PtPciTable,
  uint16_t                        CheckBit
  );

SIL_STATUS
Prom21RetrainSpeed (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           Bus,
  uint8_t            Dev,
  uint8_t            Func,
  uint32_t           XhciMmio,
  uint8_t            Speed,
  bool               LinkDisable
  );
