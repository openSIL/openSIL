/**
 * @file  FchAb.c
 * @brief FCH AB functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "FchAbReg.h"
#include "FchAb.h"
#include <string.h>
#include <Pci.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchHelper.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <CommonLib/SmnAccess.h>
#include <MultiFchInit.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>

/**********************************************************************************************************************
 * Module data structure
 *
 */

static const FCHAB_INPUT_BLK mFchAbDefaults = {
  .AbMsiEnable =                     false,
  .ALinkClkGateOff =                 1,
  .BLinkClkGateOff =                 1,
  .AbClockGating =                   1,
  .SlowSpeedAbLinkClock =            0,
  .ResetCpuOnSyncFlood =             false,
  .AbDmaMemoryWrtie3264B =           true,
  .AbMemoryPowerSaving =             true,
  .SbgDmaMemoryWrtie3264ByteCount =  false,
  .SbgMemoryPowerSaving =            true,
  .SbgClockGating =                  true,
  .XdmaDmaWrite16ByteMode =          false,
  .XdmaMemoryPowerSaving =           false,
  .XdmaPendingNprThreshold =         0,
  .XdmaDncplOrderDis =               false,
  .SdphostBypassDataPack =           false,
  .SdphostDisNpmwrProtect =          false,
};

/**
 * FchInitEnvAbTable - AB-Link Configuration Table
 *
 *
 */
static const AB_TBL_ENTRY FchInitEnvAbTable[] =
{
  //
  // Enabled SMI ordering enhancement. ABCFG 0x90[21]
  //
  {ABCFG, FCH_ABCFG_REG90, BIT_32(21), BIT_32(21)},

  //
  // Enabling Detection of Upstream Interrupts ABCFG 0x94 [20] = 1
  // ABCFG 0x94 [19:0] = cpu interrupt delivery address [39:20]
  //
  {ABCFG, FCH_ABCFG_REG94, BIT_32(20), BIT_32(20) + 0x00FEE},

  //
  // Programming cycle delay for AB and BIF clock gating
  // Enable the AB and BIF clock-gating logic.
  // Enable the A-Link int_arbiter enhancement to allow the A-Link bandwidth
  // to be used more efficiently
  //
  {ABCFG, FCH_ABCFG_REG10054,  0x00FFFFFF, 0x000007FF},

  //
  // SD ALink prefetch
  //
  {ABCFG, FCH_ABCFG_REG10060, 0xFBFFFFFF, 0x02000000},

  //
  // Enable the IO trap delay logic for the SMI message to ensure that the
  // SMI messages are sent to the CPU in the right order.
  //
  {ABCFG, FCH_ABCFG_REG10090, BIT_32(16), BIT_32(16)},
  {ABCFG, 0, 0, (uint8_t) 0xFF},  /// This dummy entry is to clear ab index
  { (uint8_t)0xFF, (uint8_t)0xFF, (uint8_t)0xFF, (uint8_t)0xFF},
};


/**
 * FchAbSetInputBlk
 * @brief Establish FCH Ab input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchAbSetInputBlk (void) {
  FCHAB_INPUT_BLK *FchAbInput;

  FchAbInput = (FCHAB_INPUT_BLK *) SilCreateInfoBlock (SilId_FchAb,
                                  sizeof(FCHAB_INPUT_BLK),
                                  FCHAB_INSTANCE,
                                  FCHAB_MAJOR_REV,
                                  FCHAB_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchAbSetInputBlk at: 0x%x \n", FchAbInput);
  if (FchAbInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy ((void *)FchAbInput, &mFchAbDefaults, sizeof(FCHAB_INPUT_BLK));

  return SilPass;
}

/**-----------------------------------------------------------------------------
 * @brief FchProgramAbPowerOnReset - Config Ab Bridge during Power-On
 *
 *
 */
static void
FchInitResetAb (
  void
  )
{
  //
  // Set A-Link bridge access address.
  // This is an I/O address. The I/O address must be on 16-byte boundary.
  //
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  uint32_t AcpiMmioBase = 0;
  FchGetAcpiMmioBase(&AcpiMmioBase);
  xUSLMemReadModifyWrite32((void *)(size_t)(AcpiMmioBase + PMIO_BASE + FCH_PMIOA_REGE0), 0, ALINK_ACCESS_INDEX);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief AbCfgTbl - Program ABCFG by input table.
 *
 *
 * @param[in] ABTbl  ABCFG config table.
 *
 */
static void
AbCfgTbl (
  AB_TBL_ENTRY *ABTbl
  )
{
  uint32_t AbValue;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  while ( (ABTbl->RegType) != 0xFF ) {
    if ( ABTbl->RegType == AXINDC ) {
      AbValue = 0x30 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, (ABTbl->RegIndex & 0x00FFFFFF));
      AbValue = 0x34 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData);
    } else if ( ABTbl->RegType == AXINDP ) {
      AbValue = 0x38 | (ABTbl->RegType << 29);
      WriteAlink (AbValue, (ABTbl->RegIndex & 0x00FFFFFF));
      AbValue = 0x3C | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData);
    } else {
      AbValue = ABTbl->RegIndex | (ABTbl->RegType << 29);
      WriteAlink (AbValue, ((ReadAlink (AbValue)) & (0xFFFFFFFF^ (ABTbl->RegMask))) | ABTbl->RegData);
    }

    ++ABTbl;
  }

  //
  //Clear ALink Access Index
  //
  AbValue = 0;
  xUSLIoWrite32 (ALINK_ACCESS_INDEX, AbValue);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchInitEnvAbLinkInit - Ab Link Env Initialization before Pci Enumeration
 * emulation.
 *
 *
 *
 */
static void FchInitEnvAbLinkInit (FCHAB_INPUT_BLK *LclInpFchAbBlk)
{
  uint8_t AbValue8;
  AB_TBL_ENTRY *AbTblPtr;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // AB CFG programming
  //
  if (LclInpFchAbBlk->SlowSpeedAbLinkClock ) {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG40),
      (uint8_t)~BIT_8(1),
      BIT_8(1)
      );
  } else {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG40),
      (uint8_t)~BIT_8(1),
      0
      );
  }

  AbTblPtr = (AB_TBL_ENTRY *) (&FchInitEnvAbTable[0]);
  AbCfgTbl (AbTblPtr);

  if (LclInpFchAbBlk->ResetCpuOnSyncFlood ) {
    RwAlink (FCH_ABCFG_REG10050 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink (FCH_ABCFG_REG10050 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }

  if (LclInpFchAbBlk->AbClockGating) {
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(24), BIT_32(24));
    RwAlink (FCH_ABCFG_REG10054 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x3 << 24), (uint32_t) (0x3 << 24));
  } else {
    RwAlink (FCH_ABCFG_REG10054 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x3 << 24), 0);
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(24), 0);
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
  }

  if (LclInpFchAbBlk->AbDmaMemoryWrtie3264B ) {
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }
  if (LclInpFchAbBlk->AbMemoryPowerSaving ) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x68),0xFB, 0x00);
    RwAlink (SIL_RESERVED_27 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink (FCH_ABCFG_REG58 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
    RwAlink (FCH_ABCFG_REG58 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));
  } else {
    RwAlink (FCH_ABCFG_REG58 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 29), 0);
    RwAlink (SIL_RESERVED_27 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x68),0xFB, 0x04);
  }

  //
  // A/B Clock Gate-OFF
  //
  if (LclInpFchAbBlk->ALinkClkGateOff) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2), 0xFE, BIT_32(0));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2), 0xFE, 0);
  }
  if (LclInpFchAbBlk->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2), 0xFD, BIT_32(1));
    xUSLSmnReadModifyWrite (0, 0, FCH_SMN_RSMU_BASE + 0x1C, ~BIT_32(31), BIT_32(31));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG2C + 2), 0xFD, 0);
    xUSLSmnReadModifyWrite (0, 0, FCH_SMN_RSMU_BASE + 0x1C, ~BIT_32(31), 0);
  }
  if (LclInpFchAbBlk->ALinkClkGateOff | LclInpFchAbBlk->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04 + 2), 0xFE, BIT_32(0));
  } else {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04 + 2), 0xFE, 0);
  }

  if (LclInpFchAbBlk->SbgMemoryPowerSaving) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x68), 0xFD, 0x00);
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x5 << 7), (uint32_t) (0x5 << 7));
    RwAlink (SIL_RESERVED_26  | (uint32_t) (ABCFG << 29), ~BIT_32(10), BIT_32(10));
  } else {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x5 << 7), (uint32_t) 0x0);
    RwAlink (SIL_RESERVED_26  | (uint32_t) (ABCFG << 29), ~BIT_32(10), 0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x68), 0xFD, 0x02);
  }

  if (LclInpFchAbBlk->SbgClockGating) {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)), (BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)));
    RwAlink (FCH_ABCFG_REG228 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), (BIT_32(1) + BIT_32(3)));
  } else {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29), ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)), 0);
    RwAlink (FCH_ABCFG_REG228 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), 0);
  }
  if (LclInpFchAbBlk->XdmaDmaWrite16ByteMode) {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
  } else {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  }
  if (LclInpFchAbBlk->XdmaMemoryPowerSaving) {
    RwAlink (SIL_RESERVED_24  | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink (SIL_RESERVED_24  | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }
  if (LclInpFchAbBlk->XdmaPendingNprThreshold) {
    AbValue8 = LclInpFchAbBlk->XdmaPendingNprThreshold;
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x1F << 8), (uint32_t) (AbValue8  << 8));
  } else {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x1F << 8), 0);
  }
  if (LclInpFchAbBlk->XdmaDncplOrderDis) {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(5), BIT_32(5));
  } else {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(5), 0);
  }
  if (LclInpFchAbBlk->SdphostBypassDataPack) {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(11), BIT_32(11));
  } else {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(11), 0);
  }
  if (LclInpFchAbBlk->SdphostDisNpmwrProtect) {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(14), BIT_32(14));
  } else {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(14), 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchInitEnvAb - Config Ab Bridge before PCI emulation
 *
 *
 *
 */
static void FchInitEnvAb (FCHAB_INPUT_BLK *LclInpFchAbBlk)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchInitEnvAbLinkInit(LclInpFchAbBlk);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSecondaryFchInitAB
 * @brief Secondary Fch AB Link Initialization.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FCHCLASS_INPUT_BLK configuration structure pointer
 * @param[in] FCH_AB configuration structure pointer
 *
 * @retval None
 *
 */
void
FchSecondaryFchInitAB (
  uint8_t DieBusNum,
  FCHCLASS_INPUT_BLK *FchDataBlock,
  FCHAB_INPUT_BLK *FchAbBlock
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  uint16_t Value;

  FCH_TRACEPOINT (SIL_TRACE_INFO, "Program Fch AB registers...Start.\n");

  Value = ALINK_ACCESS_INDEX;
  SilFchWritePmio (FCH_PMIOA_REGE0, AccessWidth16, (uint8_t *)&Value);

  // AB register init
  RwAlink (FCH_ABCFG_REG90 | (uint32_t) (ABCFG << 29), ~BIT_32(21), BIT_32(21));
  RwAlink (FCH_ABCFG_REG10054 | (uint32_t) (ABCFG << 29), 0xFF000000, 0x7FF);
  RwAlink (FCH_ABCFG_REG10060 | (uint32_t) (ABCFG << 29), 0xFBFFFFFF, 0x02000000);
  RwAlink (FCH_ABCFG_REG10090 | (uint32_t) (ABCFG << 29), ~BIT_32(16), BIT_32(16));

  if (FchAbBlock->AbClockGating) {
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(24), BIT_32(24));
    RwAlink (FCH_ABCFG_REG10054 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x3 << 24), (uint32_t) (0x3 << 24));
  } else {
    RwAlink (FCH_ABCFG_REG10054 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x3 << 24), 0);
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(24), 0);
    RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
  }

  //AbDmaMemoryWrtie3264B
  RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  RwAlink (FCH_ABCFG_REG54 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));

  xUSLMemReadModifyWrite8 ((void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + 0x68), 0xFB, 0x00);
  RwAlink (SIL_RESERVED_27 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
  RwAlink (FCH_ABCFG_REG58 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
  RwAlink (FCH_ABCFG_REG58 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));

  //
  // A/B Clock Gate-OFF
  //
  if (FchAbBlock->ALinkClkGateOff) {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + FCH_MISC_REG2C + 2),
      0xFE,
      BIT_32(0)
      );
  } else {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + FCH_MISC_REG2C + 2),
      0xFE,
      0
      );
  }

  if (FchAbBlock->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + FCH_MISC_REG2C + 2),
      0xFD,
      BIT_32(1)
      );
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SMN_RSMU_BASE + 0x1C, ~BIT_32(31), BIT_32(31));
  } else {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + FCH_MISC_REG2C + 2),
      0xFD,
      0
      );
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SMN_RSMU_BASE + 0x1C, ~BIT_32(31), 0);
  }

  if (FchAbBlock->ALinkClkGateOff | FchAbBlock->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + PMIO_BASE + FCH_PMIOA_REG04 + 2),
      0xFE,
      BIT_32(0)
      );
  } else {
    xUSLMemReadModifyWrite8 (
      (void *)(size_t)(FchDataBlock->FchAcpiMmioBase + PMIO_BASE + FCH_PMIOA_REG04 + 2),
      0xFE,
      0
      );
  }

  if (FchAbBlock->SbgMemoryPowerSaving) {
    xUSLMemReadModifyWrite8 ((void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + 0x68), 0xFD, 0x00);
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x5 << 7), (uint32_t) (0x5 << 7));
    RwAlink (SIL_RESERVED_26  | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x1 << 10), (uint32_t) (0x1  << 10));
  } else {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x5 << 7), (uint32_t) 0x0);
    RwAlink (SIL_RESERVED_26  | (uint32_t) (ABCFG << 29), ~ (uint32_t) (0x1 << 10), (uint32_t) 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(FchDataBlock->FchAcpiMmioBase + MISC_BASE + 0x68), 0xFD, 0x02);
  }

  if (FchAbBlock->SbgClockGating) {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)), (BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)));
    RwAlink (FCH_ABCFG_REG228 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), (BIT_32(1) + BIT_32(3)));
  } else {
    RwAlink (FCH_ABCFG_REG208 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)), 0);
    RwAlink (FCH_ABCFG_REG228 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), 0);
  }

  if (FchAbBlock->XdmaDmaWrite16ByteMode) {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(1), BIT_32(1));
  } else {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  }

  if (FchAbBlock->XdmaMemoryPowerSaving) {
    RwAlink (SIL_RESERVED_24  | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink (SIL_RESERVED_24  | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }

  if (FchAbBlock->XdmaPendingNprThreshold) {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29),
      ~(uint32_t) (0x1F << 8), (uint32_t) (FchAbBlock->XdmaPendingNprThreshold  << 8));
  } else {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29),
      ~(uint32_t) (0x1F << 8), 0);
  }

  if (FchAbBlock->XdmaDncplOrderDis) {
    RwAlink (SIL_RESERVED_23  | (uint32_t) (ABCFG << 29), ~BIT_32(5), BIT_32(5));
  } else {
    RwAlink (SIL_RESERVED_23 | (uint32_t) (ABCFG << 29), ~BIT_32(5), 0);
  }

  if (FchAbBlock->SdphostBypassDataPack) {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(11), BIT_32(11));
  } else {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(11), 0);
  }

  if (FchAbBlock->SdphostDisNpmwrProtect) {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(14), BIT_32(14));
  } else {
    RwAlink (SIL_RESERVED_25  | (uint32_t) (ABCFG << 29), ~BIT_32(14), 0);
  }

  RwAlink (FCH_ABCFG_REG10050 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);

  Value = 0;
  SilFchWritePmio (FCH_PMIOA_REGE0, AccessWidth16, (uint8_t *)&Value);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "Program Fch AB registers...Complete.\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * InitializeFchAbTp1
 *
 * @brief Config Ab controller during Pre-Pcie phase
 *
 * @retval SIL_STATUS
 */
SIL_STATUS InitializeFchAbTp1 (void)
{
  FCHAB_INPUT_BLK *LclInpFchAbBlk; //pointer to input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpFchAbBlk = (FCHAB_INPUT_BLK *) SilFindStructure (SilId_FchAb, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH AB found blk at: 0x%x \n", LclInpFchAbBlk);
  if (LclInpFchAbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchAbPrePcieInit(LclInpFchAbBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchAbTp2
 *
 * @brief Config Ab controller during timepoint 2
 *
 * @retval SIL_STATUS
 */
SIL_STATUS InitializeFchAbTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchAbTp3
 *
 * @brief Config Ab controller during timepoint 2
 *
 * @retval SIL_STATUS
 */
SIL_STATUS InitializeFchAbTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchAbPrePcieInit
 * @brief Config Ab Bridge on power-on and before PCI enumeration
 *
 *
 */
void FchAbPrePcieInit(FCHAB_INPUT_BLK *LclInpFchAbBlk) {
  FchInitResetAb();
  FchInitEnvAb(LclInpFchAbBlk);
}
