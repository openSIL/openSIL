/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAb.c
 * @brief FCH AB functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include "FchAbReg.h"
#include <FchAb-api.h>
#include <FchAlinkLib.h>
#include <FchAb.h>
#include <string.h>
#include <Pci.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchReg.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>

#define FCHAB_MAJOR_REV 0
#define FCHAB_MINOR_REV 1
#define FCHAB_INSTANCE  0

extern FCHAB_INPUT_BLK mFchAbDefaults;

/**
 * FchInitEnvAbTable - AB-Link Configuration Table
 *
 *
 */
static const AB_TBL_ENTRY FchInitEnvAbTable[] =
{
  //
  // Setting B-Link Prefetch Mode (ABCFG 0x80 [18:17] = 11)
  // BlPrefEn [7:0] = 1
  //
  {ABCFG, 0x80, BIT_32(17) + BIT_32(18), BIT_32(17) + BIT_32(18) + 0x01},

  {ABCFG, SIL_RESERVED_0306, BIT_32(21), BIT_32(21)},

  {ABCFG, SIL_RESERVED_0312, BIT_32(20), BIT_32(20) + 0x00FEE},

  {ABCFG, SIL_RESERVED_0302, 0x00FFFFFF, 0x000007FF},

  {ABCFG, SIL_RESERVED_0303, 0xFBFFFFFF, 0x02000000},

  {ABCFG, SIL_RESERVED_0311, BIT_32(16), BIT_32(16)},
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

  FchAbInput = (FCHAB_INPUT_BLK *) SilCreateInfoBlock(SilId_FchAb,
    sizeof (FCHAB_INPUT_BLK),
    FCHAB_INSTANCE,
    FCHAB_MAJOR_REV,
    FCHAB_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchAbSetInputBlk at: 0x%x \n", FchAbInput);
  if (FchAbInput == NULL) {
    return SilAborted;
  }

  //Fill Fch SPI structure with defaults
  memcpy((void *)FchAbInput, &mFchAbDefaults, sizeof (FCHAB_INPUT_BLK));

  return SilPass;
}

/**
 *  FchInitResetAb
 *  @brief Initializes FCH AB module in pre-PCIe phase
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
  xUSLMemReadModifyWrite32((void *)(size_t)(FCH_ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ABREGBAR), 0, ALINK_ACCESS_INDEX);
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

  while ((ABTbl->RegType) != 0xFF ) {
    if ( ABTbl->RegType == AXINDC ) {
      AbValue = 0x30 | (ABTbl->RegType << 29);
      WriteAlink(AbValue, (ABTbl->RegIndex & 0x00FFFFFF));
      AbValue = 0x34 | (ABTbl->RegType << 29);
      WriteAlink(AbValue, ((ReadAlink(AbValue)) & (0xFFFFFFFF ^ (ABTbl->RegMask))) | ABTbl->RegData);
    } else if ( ABTbl->RegType == AXINDP ) {
      AbValue = 0x38 | (ABTbl->RegType << 29);
      WriteAlink(AbValue, (ABTbl->RegIndex & 0x00FFFFFF));
      AbValue = 0x3C | (ABTbl->RegType << 29);
      WriteAlink(AbValue, ((ReadAlink(AbValue)) & (0xFFFFFFFF ^ (ABTbl->RegMask))) | ABTbl->RegData);
    } else {
      AbValue = ABTbl->RegIndex | (ABTbl->RegType << 29);
      WriteAlink(AbValue, ((ReadAlink(AbValue)) & (0xFFFFFFFF ^ (ABTbl->RegMask))) | ABTbl->RegData);
    }

    ++ABTbl;
  }

  //
  //Clear ALink Access Index
  //
  AbValue = 0;
  xUSLIoWrite32(ALINK_ACCESS_INDEX, AbValue);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchInitEnvAbLinkInit - Ab Link Env Initialization before Pci Enumeration.
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
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MISCCLKCNTRL0),
      (uint8_t) ~BIT_8(1),
      BIT_8(1)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MISCCLKCNTRL0),
      (uint8_t) ~BIT_8(1),
      0
      );
  }

  AbTblPtr = (AB_TBL_ENTRY *) (&FchInitEnvAbTable[0]);
  AbCfgTbl(AbTblPtr);

  if (LclInpFchAbBlk->ResetCpuOnSyncFlood ) {
    RwAlink(SIL_RESERVED_0310 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink(SIL_RESERVED_0310 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }

  if (LclInpFchAbBlk->AbClockGating) {
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(24), BIT_32(24));
    RwAlink(SIL_RESERVED_0302 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), (uint32_t) (0x3 << 24));
  } else {
    RwAlink(SIL_RESERVED_0302 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), 0);
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(24), 0);
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
  }

  if (LclInpFchAbBlk->AbDmaMemoryWrtie3264B ) {
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }
  if (LclInpFchAbBlk->AbMemoryPowerSaving ) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x00);
    RwAlink(SIL_RESERVED_0308 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink(SIL_RESERVED_0307 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
    RwAlink(SIL_RESERVED_0307 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));
  } else {
    RwAlink(SIL_RESERVED_0307 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 29), 0);
    RwAlink(SIL_RESERVED_0308 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x04);
  }

  //
  // A/B Clock Gate-OFF
  //
  if (LclInpFchAbBlk->ALinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2), 0xFE, BIT_32(0));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2), 0xFE, 0);
  }
  if (LclInpFchAbBlk->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2), 0xFD, BIT_32(1));
    xUSLSmnReadModifyWrite(0, 0, FCH_SMN_RSMU_BASE + SIL_RESERVED_0332, ~BIT_32(31), BIT_32(31));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2), 0xFD, 0);
    xUSLSmnReadModifyWrite(0, 0, FCH_SMN_RSMU_BASE + SIL_RESERVED_0332, ~BIT_32(31), 0);
  }
  if (LclInpFchAbBlk->ALinkClkGateOff | LclInpFchAbBlk->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ISACONTROL + 2), 0xFE, BIT_32(0));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ISACONTROL + 2), 0xFE, 0);
  }

  if (LclInpFchAbBlk->SbgMemoryPowerSaving) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0x00);
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), (uint32_t) (0x5 << 7));
    RwAlink(SIL_RESERVED_0336 | (uint32_t) (ABCFG << 29), ~BIT_32(10), BIT_32(10)); //Huashan add
  } else {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), (uint32_t) 0x0);
    RwAlink(SIL_RESERVED_0336 | (uint32_t) (ABCFG << 29), ~BIT_32(10), 0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0x02);
  }

  if (LclInpFchAbBlk->SbgClockGating) {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)),
      (BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22))
      );
    RwAlink(SIL_RESERVED_0333 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), (BIT_32(1) + BIT_32(3)));
  } else {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29), ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)), 0);
    RwAlink(SIL_RESERVED_0333 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), 0);
  }
  //
  // XDMA DMA Write 16 byte Mode
  //
  if (LclInpFchAbBlk->XdmaDmaWrite16ByteMode) {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  }
  //
  // XDMA Memory Power Saving
  //
  if (LclInpFchAbBlk->XdmaMemoryPowerSaving) {
    RwAlink(SIL_RESERVED_0304 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink(SIL_RESERVED_0304 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }
  //
  // XDMA Pending NPR Threshold
  //
  if (LclInpFchAbBlk->XdmaPendingNprThreshold) {
    AbValue8 = LclInpFchAbBlk->XdmaPendingNprThreshold;
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x1F << 8), (uint32_t) (AbValue8 << 8));
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x1F << 8), 0);
  }
  //
  // XDMA DNCPL Order Dis
  //
  if (LclInpFchAbBlk->XdmaDncplOrderDis) {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(5), BIT_32(5));
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(5), 0);
  }
  //
  // SDPHOST_BYPASS_DATA_PAC
  //
  if (LclInpFchAbBlk->SdphostBypassDataPack) {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(11), BIT_32(11));
  } else {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(11), 0);
  }
  //
  // SDPHOST_DIS_NPMWR_PROTECT
  //
  if (LclInpFchAbBlk->SdphostDisNpmwrProtect) {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(14), BIT_32(14));
  } else {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(14), 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchInitEnvAb - Config Ab Bridge before PCI
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
 * @brief Secondary Fch AB Link Initialization
 *
 * @param[in] IohcBusNumber    FCH Iohc bus number
 * @param[in] AcpiMmioBaseAddr FCH ACPI MMIO base address
 * @param[in] FchAbBlock       FCH AB input data block
 *
 */
void
FchSecondaryFchInitAB (
  uint32_t           IohcBusNumber,
  uint64_t           AcpiMmioBaseAddr,
  FCHAB_INPUT_BLK    *FchAbBlock
  )
{
  uint32_t AcpiMmioBaseAddr32;
  AcpiMmioBaseAddr32 = (uint32_t)(AcpiMmioBaseAddr & 0xFFFFFFFF);
  xUSLSmnWrite(0, IohcBusNumber, SIL_RSVD_ADDR_02D01300 + FCH_PM_ABREGBAR, ALINK_ACCESS_INDEX);

  // AB register init
  RwAlink(0x80 | (uint32_t) (ABCFG << 29),
    ~(BIT_32(17) + BIT_32(18) + BIT_32(0)),
    BIT_32(17) + BIT_32(18) + BIT_32(0)
    );
  RwAlink(SIL_RESERVED_0306 | (uint32_t) (ABCFG << 29), ~BIT_32(21), BIT_32(21));
  RwAlink(SIL_RESERVED_0302 | (uint32_t) (ABCFG << 29), 0xFF000000, 0x7FF);
  RwAlink(SIL_RESERVED_0303 | (uint32_t) (ABCFG << 29), 0xFBFFFFFF, 0x02000000);
  RwAlink(SIL_RESERVED_0311 | (uint32_t) (ABCFG << 29), ~BIT_32(16), BIT_32(16));

  if (FchAbBlock->AbClockGating) {
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(24), BIT_32(24));
    RwAlink(SIL_RESERVED_0302 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), (uint32_t) (0x3 << 24));
  } else {
    RwAlink(SIL_RESERVED_0302 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), 0);
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(24), 0);
    RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
  }

  //AbDmaMemoryWrtie3264B
  RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  RwAlink(SIL_RESERVED_0309 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));

  //AbMemoryPowerSaving
  xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x00);
  RwAlink(SIL_RESERVED_0308 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
  RwAlink(SIL_RESERVED_0307 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
  RwAlink(SIL_RESERVED_0307 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));

  //
  // A/B Clock Gate-OFF
  //
  if (FchAbBlock->ALinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFE,
      BIT_32(0)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFE,
      0
      );
  }

  if (FchAbBlock->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFD,
      BIT_32(1)
      );
    xUSLSmnReadModifyWrite(0,
      IohcBusNumber,
      FCH_SMN_RSMU_BASE + SIL_RESERVED_0332,
      ~BIT_32(31),
      BIT_32(31)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFD,
      0
      );
    xUSLSmnReadModifyWrite(0,
      IohcBusNumber,
      FCH_SMN_RSMU_BASE + SIL_RESERVED_0332,
      ~BIT_32(31),
      0
      );
  }

  if (FchAbBlock->ALinkClkGateOff | FchAbBlock->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + PMIO_BASE + FCH_PM_ISACONTROL + 2),
      0xFE,
      BIT_32(0)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + PMIO_BASE + FCH_PM_ISACONTROL + 2),
      0xFE,
      0
      );
  }

  if (FchAbBlock->SbgMemoryPowerSaving) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0x00);
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), (uint32_t) (0x5 << 7));
    RwAlink(SIL_RESERVED_0336 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x1 << 10), (uint32_t) (0x1 << 10));
  } else {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), (uint32_t) 0x0);
    RwAlink(SIL_RESERVED_0336 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x1 << 10), (uint32_t) 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0x02);
  }

  if (FchAbBlock->SbgClockGating) {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)),
      (BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22))
      );
    RwAlink(SIL_RESERVED_0333 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), (BIT_32(1) + BIT_32(3)));
  } else {
    RwAlink(SIL_RESERVED_0335 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(16) + BIT_32(17) + BIT_32(22)),
      0
      );
    RwAlink(SIL_RESERVED_0333 | (uint32_t) (ABCFG << 29), ~(BIT_32(1) + BIT_32(3)), 0);
  }

  if (FchAbBlock->XdmaDmaWrite16ByteMode) {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  }

  if (FchAbBlock->XdmaMemoryPowerSaving) {
    RwAlink(SIL_RESERVED_0304 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink(SIL_RESERVED_0304 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }

  if (FchAbBlock->XdmaPendingNprThreshold) {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29),
      ~(uint32_t) (0x1F << 8),
      (uint32_t) (FchAbBlock->XdmaPendingNprThreshold << 8)
      );
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29),
      ~(uint32_t) (0x1F << 8),
      0
      );
  }

  if (FchAbBlock->XdmaDncplOrderDis) {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(5), BIT_32(5));
  } else {
    RwAlink(SIL_RESERVED_0305 | (uint32_t) (ABCFG << 29), ~BIT_32(5), 0);
  }

  if (FchAbBlock->SdphostBypassDataPack) {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(11), BIT_32(11));
  } else {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(11), 0);
  }

  if (FchAbBlock->SdphostDisNpmwrProtect) {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(14), BIT_32(14));
  } else {
    RwAlink(SIL_RESERVED_0334 | (uint32_t) (ABCFG << 29), ~BIT_32(14), 0);
  }

  // Disable Syncflood
  RwAlink(SIL_RESERVED_0310 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);

  xUSLSmnWrite(0, IohcBusNumber, SIL_RSVD_ADDR_02D01300 + FCH_PM_ABREGBAR, 0);
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

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpFchAbBlk = (FCHAB_INPUT_BLK *) xUslFindStructure(SilId_FchAb, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH AB found blk at: 0x%x \n", LclInpFchAbBlk);
  if (LclInpFchAbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchInitResetAb ();
  FchInitEnvAb(LclInpFchAbBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
