/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAb-api.h
 * @brief FCH AB Host data (input block) definitions
 *
 * @details The FCH AB component is the main bridge between the system ring-bus
 * and the FCH sub-components.
 */

#pragma once

#pragma pack (push, 1)  // match the packing of it's origin file

///
/// AB structure
///
typedef struct {
  bool    AbMsiEnable;                    // ABlink MSI capability
  uint8_t ALinkClkGateOff;                // Alink Clock Gate-Off function - 0:disable, 1:enable *KR
  uint8_t BLinkClkGateOff;                // Blink Clock Gate-Off function - 0:disable, 1:enable *KR
  uint8_t AbClockGating;                  // AB Clock Gating - 0:disable, 1:enable *KR *CZ
  uint8_t SlowSpeedAbLinkClock;           // Slow Speed AB Link Clock - 0:disable, 1:enable *KR
  bool    ResetCpuOnSyncFlood;            // Reset Cpu On Sync Flood - 0:disable, 1:enable *KR
  bool    AbDmaMemoryWrtie3264B;          // AB DMA Memory Write 32/64 BYTE Support *KR only
  bool    AbMemoryPowerSaving;            // AB Memory Power Saving *KR *CZ
  bool    SbgDmaMemoryWrtie3264ByteCount; // SBG DMA Memory Write 32/64 BYTE Count Support *KR only
  bool    SbgMemoryPowerSaving;           // SBG Memory Power Saving *KR *CZ
  bool    SbgClockGating;                 // SBG Clock Gate *CZ
  bool    XdmaDmaWrite16ByteMode;         // XDMA DMA Write 16 byte mode *CZ
  bool    XdmaMemoryPowerSaving;          // XDMA memory power saving *CZ
  uint8_t XdmaPendingNprThreshold;        // XDMA PENDING NPR THRESHOLD *CZ
  bool    XdmaDncplOrderDis;              // XDMA DNCPL ORDER DIS *CZ
  bool    SdphostBypassDataPack;          // SdphostBypassDataPack
  bool    SdphostDisNpmwrProtect;         // Disable NPMWR interleaving protection
} FCHAB_INPUT_BLK;

#pragma pack (pop)
