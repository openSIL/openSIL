/**
 * @file  FchSataCmn2Kl.c
 * @brief Kunlun FCH SATA controller  revision specific transfer functions
 *
 */
/* Copyright 2025 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/SmnAccess.h>
#include <Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchCore/FchSata/FchSataCmn2Rev.h>
#include <FCH/Common/FchCore/FchSata/FchSataReg.h>
#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include <FCH/FchHwAcpi-api.h>
#include "FchSataKl.h"
#include "FchSataCmn2Kl.h"

FCH_SATA_XFER_TABLE mFchSataXferKl = {
  .Header = {
    .Version    = 1   // Version 1
  },
  .FchSataInitHideSataPci         = FchSataInitHideSataPciKl,
  .FchSataInitMsi                 = FchSataInitMsiKl,
  .FchSataSetBistLComplianceMode  = FchSataSetBISTLComplianceModeKl,
  .FchSataInitDevSlp              = FchSataInitDevSlpKl
};

/**
 * FchSataInitHideSataPciKl
 *
 * @brief Disable Sata Controller PCI configuration space.
 *
 * @param DieBusNum  IOCH bus number on current Die.
 * @param Controller Sata controller number.
 *
 */
void
FchSataInitHideSataPciKl (
  uint32_t  DieBusNum,
  uint32_t  Controller
  )
{
  uint32_t  SataBridgeAddress;

  SataBridgeAddress = 0;

  if ((Controller != 0) && (Controller != 2)) {
    SataBridgeAddress = SIL_RESERVED_44 + FCH_SMN_SATA_STEP *
      (Controller - 1);

    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Hide SATA PCI agent. DieBusNum 0x%x, nBIF SATA %d, SMN 0x%x set BIT28\n",
      DieBusNum,
      Controller,
      SataBridgeAddress
      );

    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SataBridgeAddress,
      ~(BIT_32(28)),
      0
      );
  }
}

/**
 * FchSataInitMsiKl
 *
 * @brief Enable/Disable Sata Msi capabilities
 *
 * @param DieBusNum   Bus Number of current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitMsiKl (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  bool FchSataMsiEnable;
  FchSataMsiEnable = FchSata[Controller].SataMsiEnable;

  if (FchSataMsiEnable) {
    switch (Controller) {
    case 0:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_45,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 1:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_42,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 2:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_39,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 3:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_36,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    default:
      break;
    }
  } else {
    switch (Controller) {
    case 0:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_45,
        ~BIT_32(18),
        0x00
        );
      break;
    case 1:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_42,
        ~BIT_32(18),
        0x00
        );
      break;
    case 2:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_39,
        ~BIT_32(18),
        0x00
        );
      break;
    case 3:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_36,
        ~BIT_32(18),
        0x00
        );
      break;
    default:
      break;
    }
  }
}

/**
 * FchKLSataSetBISTLComplianceMode - Set Sata port BIST-L Compliance mode
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void
FchSataSetBISTLComplianceModeKl (
  uint32_t   DieBusNum,
  uint32_t   Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t                  PortNumByte;
  uint32_t                  BISTLPattern;

  PortNumByte   = 0;
  BISTLPattern  = FchSata[Controller].SataBISTLComplianceMode;

  if (BISTLPattern != 0) {
    for (PortNumByte=0; PortNumByte < SATA_CONTROLLER_NUM; PortNumByte++) {
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNumByte * 0x20 + 0x94,
        ~BIT_32(11),
        BIT_32(11)
        );
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNumByte * 0x20 + 0x94,
        ~(BIT_32(12) | BIT_32(13)),
        (BISTLPattern & 3) << 12
        );
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNumByte * 0x20 + 0x94,
        (uint32_t)~0x3C,
        0x18
        );
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNumByte * 0x20 + 0x94,
        ~BIT_32(0),
        BIT_32(0)
        );
    }
  }
}

/**
 * FchSataInitDevSlpKl - init DevSlp configuration
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata Controller Number.
 * @param[in] FchSata    Fch configuration structure pointer.
 *
 */
void
FchSataInitDevSlpKl (
  uint32_t  DieBusNum,
  uint32_t  Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint8_t PortNumByte;
  FCHHWACPI_INPUT_BLK *HwAcpiBlk;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Start\n");

  HwAcpiBlk = (FCHHWACPI_INPUT_BLK *) xUslFindStructure(SilId_FchHwAcpiP, 0);
  if (HwAcpiBlk == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH HW ACPI input block not found\n");
    return;
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "BusNum = 0x%x Controller = 0x%x MMIO = 0x%x\n", DieBusNum, Controller, HwAcpiBlk->FchAcpiMmioBase);

  if (FchSata[Controller].SataDevSlpPort0) {
    PortNumByte = FchSata[Controller].SataDevSlpPort0Num;

    xUSLMemReadModifyWrite8((void *)(size_t)(HwAcpiBlk->FchAcpiMmioBase + IOMUX_BASE + 0x05), 0, 1);
    xUSLMemReadModifyWrite32(
      (void *)(size_t)(HwAcpiBlk->FchAcpiMmioBase + GPIO_BANK0_BASE + 0x14),
      ~(BIT_32(20) + BIT_32(21) + BIT_32(22) + BIT_32(23)),
      BIT_32(23) | BIT_32(20)
      );
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0xF4,
      0xFFFFFFFF,
      BIT_32(4) | (BIT_32(8) << PortNumByte)
      );
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x8C,
      0xFFFF8FFF,
      (uint32_t) (PortNumByte << 12)
      );
  }

  if (FchSata[Controller].SataDevSlpPort1) {
    if (FchSata[Controller].SataDevSlpPort0 && 
        FchSata[Controller].SataDevSlpPort0Num == FchSata[Controller].SataDevSlpPort1Num)
    {
      // Port1 is pointing to the same port as port 0. We should skip Port1 configuration.
      FCH_TRACEPOINT(SIL_TRACE_INFO, "WARNING: DevSlp 1 is pointing to the same controller/port as DevSlp 0's. We should skip DevSlp 1 configuration.\n");
      FCH_TRACEPOINT(SIL_TRACE_EXIT, "End\n");
      return;
    }

    PortNumByte = FchSata[Controller].SataDevSlpPort1Num;

    xUSLMemReadModifyWrite8((void *)(size_t)(HwAcpiBlk->FchAcpiMmioBase + IOMUX_BASE + 0x06), 0 , 1);
    xUSLMemReadModifyWrite32(
      (void *)(size_t)(HwAcpiBlk->FchAcpiMmioBase + GPIO_BANK0_BASE + 0x18),
      ~(BIT_32(20) + BIT_32(21) + BIT_32(22) + BIT_32(23)),
      BIT_32(23) | BIT_32(20)
      );

    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0xF4,
      0xFFFFFFFF,
      BIT_32(4) | (BIT_32(8) << PortNumByte)
      );
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0xAC,
      0xFFFF8FFF,
      (uint32_t) (PortNumByte << 12)
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "End\n");
}