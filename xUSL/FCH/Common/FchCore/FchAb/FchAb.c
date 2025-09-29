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
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>

extern FCHAB_INPUT_BLK mFchAbDefaults;

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
  {ABx0CDC_xC000_0090, BIT_32(21), BIT_32(21)},

  //
  // Enabling Detection of Upstream Interrupts
  //
  {ABx0CDC_xC000_0094, BIT_32(20), BIT_32(20) + 0x00FEE},

  //
  // Programming cycle delay for AB and BIF clock gating
  // Enable the AB and BIF clock-gating logic.
  // Enable the A-Link int_arbiter enhancement to allow the A-Link bandwidth
  // to be used more efficiently
  //
  {ABx0CDC_xC001_0054, 0x00FFFFFF, 0x000007FF},

  //
  // SD ALink prefetch
  //
  {ABx0CDC_xC001_0060, 0xFBFFFFFF, 0x02000000},

  //
  // Enable the IO trap delay logic for the SMI message to ensure that the
  // SMI messages are sent to the CPU in the right order.
  //
  {ABx0CDC_xC001_0090, BIT_32(16), BIT_32(16)},
};

/**
 * FchAbSetInputBlk
 * @brief Establish FCH Ab input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval SIL_STATUS
 */
SIL_STATUS FchAbSetInputBlk (
  SIL_CONTEXT  *SilContext
  )
{
  FCHAB_INPUT_BLK *FchAbInputBlk;

  FchAbInputBlk = (FCHAB_INPUT_BLK *) SilCreateInfoBlock(SilContext,
    SilId_FchAb,
    sizeof (FCHAB_INPUT_BLK),
    FCHAB_INPUT_BLK_INSTANCE,
    FCHAB_MAJOR_REV,
    FCHAB_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchAbSetInputBlk at: 0x%x \n", FchAbInputBlk);
  if (FchAbInputBlk == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy((void *)FchAbInputBlk, &mFchAbDefaults, sizeof (FCHAB_INPUT_BLK));

  return SilPass;
}

/**
 * @brief AbCfgTbl - Program ABCFG by input table.
 *
 * @param[in] ABTbl  ABCFG config table.
 * @param[in] Size Number of entries in the table.
 *
 */
static void
AbCfgTbl (
  const AB_TBL_ENTRY *ABTbl,
  uint32_t Size
  )
{
  uint32_t AbValue;
  uint32_t i;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  for (i = 0; i < Size; i++) {
    AbValue = ABTbl[i].RegIndex | (ABCFG << 29);
    WriteAlink(AbValue, ((ReadAlink(AbValue)) & (0xFFFFFFFF ^ (ABTbl[i].RegMask))) | ABTbl[i].RegData);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchAbLinkInitCommon
 *
 * @brief Code shared between FchInitEnvAb and FchSecondaryFchInitAB
 *
 * @param[in] IohcBusNumber    FCH Iohc bus number
 * @param[in] AcpiMmioBaseAddr FCH ACPI MMIO base address
 * @param[in] FchAbInputBlk       FCH AB input data block
 *
 */
static
void
FchAbLinkInitCommon (
  uint32_t IohcBusNumber,
  uint64_t AcpiMmioBaseAddr,
  FCHAB_INPUT_BLK *FchAbInputBlk
  )
{
  uint32_t AcpiMmioBaseAddr32;
  AcpiMmioBaseAddr32 = (uint32_t)(AcpiMmioBaseAddr & 0xFFFFFFFF);

  if (FchAbInputBlk->AbClockGating) {
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(24), BIT_32(24));
    RwAlink(ABx0CDC_xC001_0054 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), (uint32_t) (0x3 << 24));
  } else {
    RwAlink(ABx0CDC_xC001_0054 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x3 << 24), 0);
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(24), 0);
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
  }

  //
  // A/B Clock Gate-OFF
  //
  xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
    0xFE,
    FchAbInputBlk->ALinkClkGateOff ? BIT_8(0) : 0
    );

  if (FchAbInputBlk->BLinkClkGateOff) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFD,
      BIT_8(1)
      );
    xUSLSmnReadModifyWrite(0,
      IohcBusNumber,
      FCH_SMN_RSMU_BASE + FCHRSMUREGx0000001C,
      ~BIT_32(31),
      BIT_32(31)
      );
    RwAlink(FCHSDPx0CDC_xC000_0228 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(1) + BIT_32(3)),
      (BIT_32(1) + BIT_32(3))
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_CLKGATEDCNTL + 2),
      0xFD,
      0
      );
    xUSLSmnReadModifyWrite(0,
      IohcBusNumber,
      FCH_SMN_RSMU_BASE + FCHRSMUREGx0000001C,
      ~BIT_32(31),
      0
      );
    RwAlink(FCHSDPx0CDC_xC000_0228 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(1) + BIT_32(3)),
      0
      );
  }
  if (FchAbInputBlk->ALinkClkGateOff | FchAbInputBlk->BLinkClkGateOff) {
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

  if (FchAbInputBlk->SbgMemoryPowerSaving) {
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0);
    RwAlink(FCHSDPx0CDC_xC000_0208 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), (uint32_t) (0x5 << 7));
    RwAlink(FCHSDPx0CDC_xC000_0238 | (uint32_t) (ABCFG << 29), ~BIT_32(10), BIT_32(10));
  } else {
    RwAlink(FCHSDPx0CDC_xC000_0208 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 7), 0);
    RwAlink(FCHSDPx0CDC_xC000_0238 | (uint32_t) (ABCFG << 29), ~BIT_32(10), 0);
    xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFD, 0x02);
  }

  //
  // SDP Internal Clock Gating
  if (FchAbInputBlk->SbgClockGating) {
    RwAlink(FCHSDPx0CDC_xC000_0208 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(22)),
      (BIT_32(15) + BIT_32(22))
      );
  } else {
    RwAlink(FCHSDPx0CDC_xC000_0208 | (uint32_t) (ABCFG << 29),
      ~(BIT_32(15) + BIT_32(22)),
      0
      );
  }

  //
  // XDMA DMA Write 16 byte Mode
  //
  RwAlink(ABx0CDC_xC000_0180 | (uint32_t) (ABCFG << 29),
    ~BIT_32(0),
    FchAbInputBlk->XdmaDmaWrite16ByteMode ? BIT_32(0) : 0
    );

  //
  // XDMA Memory Power Saving
  //
  RwAlink(ABx0CDC_xC000_0184 | (uint32_t) (ABCFG << 29),
    ~BIT_32(2),
    FchAbInputBlk->XdmaMemoryPowerSaving ? BIT_32(2) : 0
    );

  //
  // XDMA Pending NPR Threshold
  //
  RwAlink(ABx0CDC_xC000_0180 | (uint32_t) (ABCFG << 29),
    ~(uint32_t) (0x1F << 8),
    (uint32_t) (
      FchAbInputBlk->XdmaPendingNprThreshold ?
      (FchAbInputBlk->XdmaPendingNprThreshold << 8) : 0
      )
    );

  //
  // XDMA DNCPL Order Dis
  //
  RwAlink(ABx0CDC_xC000_0180 | (uint32_t) (ABCFG << 29),
    ~BIT_32(5),
    FchAbInputBlk->XdmaDncplOrderDis ? BIT_32(5) : 0
    );

  //
  // SDPHOST_BYPASS_DATA_PAC
  //
  RwAlink(FCHSDPx0CDC_xC000_022C | (uint32_t) (ABCFG << 29),
    ~BIT_32(11),
    FchAbInputBlk->SdphostBypassDataPack ? BIT_32(11) : 0
    );

  //
  // SDPHOST_DIS_NPMWR_PROTECT
  //
  RwAlink(FCHSDPx0CDC_xC000_022C | (uint32_t) (ABCFG << 29),
    ~BIT_32(14),
    FchAbInputBlk->SdphostDisNpmwrProtect ? BIT_32(14) : 0
    );

}

/**
 * @brief FchInitEnvAb - Ab Env Initialization before Pci Enumeration
 *
 */
void
FchInitEnvAb (FCHAB_INPUT_BLK *FchAbInputBlk)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // AB CFG programming
  //
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MISCCLKCNTRL0),
    (uint8_t) ~BIT_8(1),
    FchAbInputBlk->SlowSpeedAbLinkClock ? BIT_8(1) : 0
    );

  AbCfgTbl(FchInitEnvAbTable, sizeof (FchInitEnvAbTable) / sizeof (FchInitEnvAbTable[0]));


  //
  // Reset CPU on sync flood
  //
  if (FchAbInputBlk->ResetCpuOnSyncFlood) {
    RwAlink(ABx0CDC_xC001_0050 | (uint32_t) (ABCFG << 29),
      ~BIT_32(2),
      BIT_32(2)
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(FCH_ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG),
      ~BIT_32(18),
      BIT_32(18)
      );
  } else {
    RwAlink(ABx0CDC_xC001_0050 | (uint32_t) (ABCFG << 29),
      ~BIT_32(2),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(FCH_ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG),
      ~BIT_32(18),
      0
      );
  }

  if (FchAbInputBlk->AbDmaMemoryWrite3264B ) {
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));
  } else {
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(0), BIT_32(0));
    RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);
  }
  if (FchAbInputBlk->AbMemoryPowerSaving ) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x00);
    RwAlink(ABx0CDC_xC000_00BC | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
    RwAlink(ABx0CDC_xC000_0058 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
    RwAlink(ABx0CDC_xC000_0058 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));
  } else {
    RwAlink(ABx0CDC_xC000_0058 | (uint32_t) (ABCFG << 29), ~(uint32_t) (0x5 << 29), 0);
    RwAlink(ABx0CDC_xC000_00BC | (uint32_t) (ABCFG << 29), ~BIT_32(4), 0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x04);
  }

  FchAbLinkInitCommon(0, ACPI_MMIO_BASE, FchAbInputBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSecondaryFchInitAB
 * @brief Secondary Fch AB Link Initialization
 *
 * @param[in] IohcBusNumber    FCH Iohc bus number
 * @param[in] AcpiMmioBaseAddr FCH ACPI MMIO base address
 * @param[in] FchAbInputBlk       FCH AB input data block
 *
 */
void
FchSecondaryFchInitAB (
  uint32_t           IohcBusNumber,
  uint64_t           AcpiMmioBaseAddr,
  FCHAB_INPUT_BLK    *FchAbInputBlk
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  uint32_t AcpiMmioBaseAddr32;
  AcpiMmioBaseAddr32 = (uint32_t)(AcpiMmioBaseAddr & 0xFFFFFFFF);
  xUSLSmnWrite(0, IohcBusNumber, FCHREGx02D01300 + FCH_PM_ABREGBAR, ALINK_ACCESS_INDEX);

  RwAlink(ABx0CDC_xC000_0090 | (uint32_t) (ABCFG << 29), ~BIT_32(21), BIT_32(21));
  RwAlink(ABx0CDC_xC001_0054 | (uint32_t) (ABCFG << 29), 0xFF000000, 0x7FF);
  RwAlink(ABx0CDC_xC001_0060 | (uint32_t) (ABCFG << 29), 0xFBFFFFFF, 0x02000000);
  RwAlink(ABx0CDC_xC001_0090 | (uint32_t) (ABCFG << 29), ~BIT_32(16), BIT_32(16));

  //AbDmaMemoryWrite3264B
  RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(0), 0);
  RwAlink(ABx0CDC_xC000_0054 | (uint32_t) (ABCFG << 29), ~BIT_32(2), BIT_32(2));

  //AbMemoryPowerSaving
  xUSLMemReadModifyWrite8((void *)(size_t)(AcpiMmioBaseAddr32 + MISC_BASE + FCH_MISC_MEMPWRSAVCNTRL), 0xFB, 0x00);
  RwAlink(ABx0CDC_xC000_00BC | (uint32_t) (ABCFG << 29), ~BIT_32(4), BIT_32(4));
  RwAlink(ABx0CDC_xC000_0058 | (uint32_t) (ABCFG << 29), ~BIT_32(29), BIT_32(29));
  RwAlink(ABx0CDC_xC000_0058 | (uint32_t) (ABCFG << 29), ~BIT_32(31), BIT_32(31));

  FchAbLinkInitCommon(IohcBusNumber, AcpiMmioBaseAddr, FchAbInputBlk);

  // Disable Syncflood
  RwAlink(ABx0CDC_xC001_0050 | (uint32_t) (ABCFG << 29), ~BIT_32(2), 0);

  xUSLSmnWrite(0, IohcBusNumber, FCHREGx02D01300 + FCH_PM_ABREGBAR, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 *  FchInitResetAb
 *  @brief Initializes FCH AB module in pre-pcie phase
 *
 */
void
FchInitResetAb (
  void
  )
{
  //
  // Set A-Link bridge access address.
  // This is an I/O address. The I/O address must be on 16-byte boundary.
  //
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  xUSLMemReadModifyWrite32((void *)(size_t)(FCH_ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ABREGBAR),
    0,
    ALINK_ACCESS_INDEX
    );
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
