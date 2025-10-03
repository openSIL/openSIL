/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsa.c
 * @brief FCH ISA sub-controller functions
 *
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FchClass-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <Pci.h>
#include "FchSpi.h"
#include "FchIsa.h"

static const FCHISA_INPUT_BLK FchIsaDefaults = {
  .LpcConfig = {
    .LpcEnable = CONFIG_LPC_BUS_ENABLE,
    .LpcClk0 = true,
    .LpcClk1 = true,
    .LpcMsiEnable = false,
    .LpcSsid = 0
  },
  .SpiConfig = {
    .SpiSpeed = CONFIG_SPI_STD_SPEED,
    .WriteSpeed = CONFIG_SPI_WRT_SPEED,
    .SpiTpmSpeed = CONFIG_SPI_TPM_SPEED,
    .FastSpeed = CONFIG_SPI_FAST_SPEED,
    .Spi100Enable = true,
    .BurstWrite = false
  }
};

/**
 * FchIsaSetInputBlk
 * @brief Establish FCH ISA input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchIsaSetInputBlk (void)
{
  FCHISA_INPUT_BLK *FchIsaInput;

  FchIsaInput = (FCHISA_INPUT_BLK *) SilCreateInfoBlock(SilId_FchIsa,
    sizeof (FCHISA_INPUT_BLK),
    SIL_RESERVED_0283,
    FCHISA_MAJOR_REV,
    FCHISA_MINOR_REV
    );

  if (FchIsaInput == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL Fch ISA input block was not found.\n");
    return SilAborted;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA input block is at: 0x%x \n", FchIsaInput);

  // Fill Fch ISA structure with defaults
  memcpy((void *)FchIsaInput, &FchIsaDefaults, sizeof (FCHISA_INPUT_BLK));

  return SilPass;
}

/**
 * FchInitResetLpc - Config Lpc controller during Power-On
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
FchInitResetLpc (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_LPC            *FchLpc
  )
{
  xUSLPciReadModifyWrite32(
    PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_ROMADDRESSRANGE_2_START_ADDRESS),
    0xFFFFFF00,
    0
    );

  if (FchLpc->LpcEnable ) {
    //PLAT-33566
    xUSLPciReadModifyWrite8(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_IO_MEM_PORT_DECODE_ENABLE),
      BIT_8(2),
      BIT_8(0) + BIT_8(1)
      );

    // Force EC_PortActive to 1 to fix possible IR non function issue when NO_EC_SUPPORT is defined
    xUSLPciReadModifyWrite8(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_0329),
      ~BIT_8(0),
      BIT_8(0)
      );

    xUSLPciReadModifyWrite8(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_0328 + 1),
      ~BIT_8(5),
      FchLpc->LpcClk0 ? BIT_8(5) : 0
      );

    xUSLPciReadModifyWrite8(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_0328 + 1),
      ~BIT_8(6),
      FchLpc->LpcClk1 ? BIT_8(6) : 0
      );

    xUSLPciReadModifyWrite32(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_IO_PORT_DECODE_ENABLE),
      0,
      FchDataPtr->LegacyFree ? 0x0003C000 : 0xFF03FFD5
      );

  } else {
    // Disable LPC decode when LPC disabled
    xUSLPciReadModifyWrite32(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_IO_PORT_DECODE_ENABLE),
      0,
      0
      );
    xUSLPciReadModifyWrite32(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_IO_MEM_PORT_DECODE_ENABLE),
      0xFF00,
      0
      );
    // Disable LPC clock when LPC disabled
    // xUSLPciReadModifyWrite8(
    //   PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, SIL_RESERVED_0328 + 1),
    //   ~(BIT_8(5) | BIT_8(6)),
    //   0
    //   );
  }
}

/**
 * FchIsaInitSpi
 * @brief Initializes SPI controller during Power-On
 *
 * @param[in] FchDataPtr Pointer to Fch input data block
 * @param[in] FchSpiData FCH_SPI configuration structure pointer.
 *
 */
static void
FchIsaInitLpc (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_LPC            *FchLpc
  )
{

  FchInitResetLpc(FchDataPtr, FchLpc);

  xUSLPciReadModifyWrite8(
    PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_PCI_CONTROL),
    0xFB,
    BIT_8(2)
    );
  xUSLPciReadModifyWrite8(
    PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_MISCELLANEOUS_CONTROL_BITS),
    0xFC,
    0
    );
  xUSLPciReadModifyWrite8(
    PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_HOSTCONTROL),
    0xF3,
    BIT_8(2) | BIT_8(3)
    );

  if (FchLpc->LpcMsiEnable) {
    xUSLPciReadModifyWrite32(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPCPCICFG_MISCELLANEOUS_CONTROL_BITS),
      ~BIT_32(1),
      BIT_32(1)
      );
  }

  if (FchLpc->LpcSsid != 0) {
    xUSLPciReadModifyWrite32(
      PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, 0x2c),
      0x00,
      FchLpc->LpcSsid
    );
  }
}

/**
 * InitializeFchIsaTp1
 *
 * @brief Config ISA controller during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchIsaTp1 (void)
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to Fch input blk
  FCHISA_INPUT_BLK   *LclInpIsaBlk; //pointer to input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH Class was not found\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", LclInpFchBlk);

  LclInpIsaBlk = (FCHISA_INPUT_BLK *) xUslFindStructure(SilId_FchIsa, 0);
  if (LclInpIsaBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH ISA was not found.\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA found blk at: 0x%x \n", LclInpIsaBlk);

  FchIsaInitLpc(LclInpFchBlk, &(LclInpIsaBlk->LpcConfig));
  FchIsaInitSpi(LclInpFchBlk, &(LclInpIsaBlk->SpiConfig));

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaTp2
 *
 * @brief Config ISA controller during timepoint 2 (Post-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchIsaTp2 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaTp3
 *
 * @brief Config ISA controller during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchIsaTp3 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
