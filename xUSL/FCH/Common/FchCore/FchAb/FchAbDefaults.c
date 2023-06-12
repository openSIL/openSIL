/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAbDefaults.c
 * @brief FCH AB Default Settings
 *
 */

#include <SilCommon.h>
#include <FchAb-api.h>

const FCHAB_INPUT_BLK mFchAbDefaults = {
  .AbMsiEnable = false,                                  // AbMsiEnable
  .ALinkClkGateOff = CONFIG_FCH_ALINK_CLK_GATING,        // ALinkClkGateOff
  .BLinkClkGateOff = CONFIG_FCH_BLINK_CLK_GATING,        // BLinkClkGateOff
  .AbClockGating = CONFIG_FCH_AB_CLK_GATING,             // AbClockGating
  .SlowSpeedAbLinkClock = CONFIG_FCH_SLOW_BCLOCK,        // SlowSpeedAbLinkClock
  .ResetCpuOnSyncFlood = CONFIG_FCH_RESET_SYNC_FLOOD,    // ResetCpuOnSyncFlood
  .AbDmaMemoryWrtie3264B = CONFIG_FCH_DMA_SIZE,          // AbDmaMemoryWrtie3264B
  .AbMemoryPowerSaving = CONFIG_FCH_MEM_PWR_SAVING,      // AbMemoryPowerSaving
  .SbgDmaMemoryWrtie3264ByteCount = false,               // SbgDmaMemoryWrtie3264ByteCount
  .SbgMemoryPowerSaving = CONFIG_FCH_SDP_PWR_SAVING,     // SbgMemoryPowerSaving
  .SbgClockGating = CONFIG_FCH_SDP_CLK_GATING,           // SbgClockGating
  .XdmaDmaWrite16ByteMode = CONFIG_FCH_XDMA_LIMIT_SIZE,  // XDMA DMA Write 16 byte mode
  .XdmaMemoryPowerSaving = CONFIG_FCH_XDMA_PWR_SAVING,   // XDMA memory power saving
  .XdmaPendingNprThreshold = CONFIG_FCH_XDMA_NPR_COUNT,  // XDMA PENDING NPR THRESHOLD
  .XdmaDncplOrderDis = CONFIG_FCH_XDMA_ORDER_DIS,        // XDMA DNCPL ORDER DIS
  .SdphostBypassDataPack = CONFIG_FCH_SDP_BYPASS_PACK,   // SdphostBypassDataPack
  .SdphostDisNpmwrProtect = CONFIG_FCH_SDP_INTERLEAVING, // SdphostDisNpmwrProtect
};
