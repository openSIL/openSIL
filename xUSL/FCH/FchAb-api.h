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

#define FCHAB_MAJOR_REV 0
#define FCHAB_MINOR_REV 1
#define FCHAB_INPUT_BLK_INSTANCE  0

#pragma pack (push, 1)  // match the packing of its origin file

///
/// AB structure
///
typedef struct {
  bool    AbMsiEnable;                    // ABlink MSI capability
  uint8_t ALinkClkGateOff;                // Alink Clock Gate-Off function - 0:disable, 1:enable
  uint8_t BLinkClkGateOff;                // Blink Clock Gate-Off function - 0:disable, 1:enable
  uint8_t AbClockGating;                  // AB Clock Gating - 0:disable, 1:enable
  uint8_t SlowSpeedAbLinkClock;           // Slow Speed AB Link Clock - 0:disable, 1:enable
  bool    ResetCpuOnSyncFlood;            // Reset Cpu On Sync Flood - 0:disable, 1:enable
  bool    AbDmaMemoryWrite3264B;          // AB DMA Memory Write 32/64 BYTE Support
  bool    AbMemoryPowerSaving;            // AB Memory Power Saving
  bool    SbgDmaMemoryWrite3264ByteCount; // SBG DMA Memory Write 32/64 BYTE Count Support
  bool    SbgMemoryPowerSaving;           // SBG Memory Power Saving
  bool    SbgClockGating;                 // SBG Clock Gate
  bool    XdmaDmaWrite16ByteMode;         // XDMA DMA Write 16 byte mode
  bool    XdmaMemoryPowerSaving;          // XDMA memory power saving
  uint8_t XdmaPendingNprThreshold;        // XDMA PENDING NPR THRESHOLD
  bool    XdmaDncplOrderDis;              // XDMA DNCPL ORDER DIS
  bool    SdphostBypassDataPack;          // SdphostBypassDataPack
  bool    SdphostDisNpmwrProtect;         // Disable NPMWR interleaving protection
} FCHAB_INPUT_BLK;

#pragma pack (pop)
