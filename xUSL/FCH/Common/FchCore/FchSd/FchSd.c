/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSd.c
 * @brief FCH SD Env init
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <FCH/FchClass-api.h>
#include <string.h>
#include <Pci.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchAoacLib.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/SmnAccess.h>
#include "FchSd.h"

//
// SD controller
//
#define TACOMA_SD_BUS_DEV_FUN     ((0x14 << 3) + 6)
#define TACOMA_SD_PCI_DEV         0x14
#define TACOMA_SD_PCI_FUNC        6

/**
 * @brief FchInitEnvSd - SD Env Initialization before Pci Enumeration
 *
 */
void
FchInitEnvSd (FCH_SD *FchSd)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  PCI_ADDR   PciAddr;

  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = TACOMA_SD_PCI_DEV;
  PciAddr.Address.Function = TACOMA_SD_PCI_FUNC;

  //
  // SD Configuration
  //
  if (FchSd->SdConfig == _SdDisable) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + 0xD3),
      (uint8_t) ~BIT_8(6),
      0
      );
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + 0xE8),
      (uint8_t) ~BIT_8(0),
      0
      );
    FchAoacPowerOnDev(FCH_AOAC_SD, 0);
  } else if (FchSd->SdConfig != _SdDump) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + 0xD3),
      (uint8_t) ~BIT_8(6),
      BIT_8(6)
      );
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + 0xE8),
      (uint8_t) ~BIT_8(0),
      BIT_8(0)
      );
    if (FchSd->SdSsid != 0) {
      PciAddr.Address.Register = 0x2C;
      xUSLPciWrite32(PciAddr.AddressValue, FchSd->SdSsid);
    }

    PciAddr.Address.Register = 0xA4;
    xUSLPciReadModifyWrite32(PciAddr.AddressValue, 0x3FFFFFFF, (uint32_t)(FchSd->SdSlotType) << 30);

    if (FchSd->SdConfig == _SdVer2) {
      PciAddr.Address.Register = 0xA4;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(0xFF << 8),
        (uint32_t)(0x32 << 8)
        );
      PciAddr.Address.Register = 0xA8;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)BIT_32(2),
        (uint32_t)(0x00 << 2)
        );
      PciAddr.Address.Register = 0xA8;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(BIT_32(1)),
        0
        );
      PciAddr.Address.Register = 0xA8;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(BIT_32(0)),
        0
        );
      PciAddr.Address.Register = 0xB0;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(0xFF << 24),
        BIT_32(24)
        );

    } else {
      //  no SD3.0 POR
    }
  
    if (FchSd->SdForce18) {
      PciAddr.Address.Register = 0xD0;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)BIT_32(17),
        BIT_32(17)
        );
    }

    if (FchSd->SdDbgConfig == 1) {
      //DMA clear BIT19,BIT20
      PciAddr.Address.Register = 0xA4;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(BIT_32(19) + BIT_32(20)),
        0
        );
    }

    if (FchSd->SdDbgConfig == 2) {
      //PIO clear BIT19,BIT20,BIT22
      PciAddr.Address.Register = 0xA4;
      xUSLPciReadModifyWrite32(PciAddr.AddressValue,
        ~(uint32_t)(BIT_32(19) + BIT_32(20) + BIT_32(22)),
        0
        );
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
