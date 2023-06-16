/**
 * @file  FchSataSn.c
 * @brief 9004 FCH SATA controller functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <FCH/Common/FchHelper.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchCommonCfg.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include "FchHwAcpiSn.h"
#include <CommonLib/SmnAccess.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Fch.h>
#include <Utils.h>

static FCH_HWACPI_XFER_TABLE mFchHwAcpiXferSn = {
  .Header = {
    .Version    = 1   // Version 1 = 9004 FCH
  },
  .FchInitPrePcieHwAcpiP    = FchInitPrePcieHwAcpiPSn,
  .FchInitPrePcieHwAcpi     = FchInitPrePcieHwAcpiSn,
  .FchInitPostPcieHwAcpiP   = FchInitPostPcieHwAcpiPSn,
  .FchInitPostPcieHwAcpi    = FchInitPostPcieHwAcpiSn,
  .FchInitPreOsHwAcpiP      = FchInitPreOsHwAcpiPSn,
  .FchInitPreOsHwAcpi       = FchInitPreOsHwAcpiSn
};

static const ACPI_REG_WRITE Fch9004InitResetAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                                /// Signature

  {MISC_BASE >> 8,  FCH_MISC_REG6C + 2, 0xDF, BIT_32(5)},              // MISC 0x6C BIT_32(21)
  {MISC_BASE >> 8,  FCH_MISC_REG6C + 3, 0xF7, BIT_32(3)},              // MISC 0x6C BIT_32(27)
  {PMIO_BASE >> 8,  FCH_PMIOA_REGDC, 0x00, 0},                         /// Clear XDC SATA configuration
  {PMIO_BASE >> 8,  FCH_PMIOA_REG3C + 2, 0xFB, BIT_32(2)},             // Set PMIO_3E SMERR_L pad to output mode
  {PMIO_BASE >> 8,  FCH_PMIOA_REGC0 + 3, 0x00, BIT_32(5)},             // Clear PMIO_C0 mp1_wdtout

  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * Fch9004InitEnvHwAcpiPciTable - PCI device registers
 * initial during early POST.
 *
 */
static const REG8_MASK Fch9004InitEnvHwAcpiPciTable[] =
{
  //
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  //
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {FCH_CFG_REG11, 0X00, (FCH_VERSION_9004 >> 8)},
  {0xFF, 0xFF, 0xFF},
};


/**
 * FchInitPrePcieHwAcpiPSn
 *
 * @brief Config FCH ACPI Module (Preliminary) before PCI enumeration. It's no need to re-run in S3 resume.
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPrePcieHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPrePcieHwAcpiSn
 *
 * @brief Config FCH ACPI Module before PCI enumeration
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPrePcieHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{


  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //
  FchInitEnableBootTimerV80 (FchHwAcpi);
  ProgramCpuRstBTmr ();

  //
  // Program specific Init
  //
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&Fch9004InitResetAcpiMmioTable[0]));
  ProgramPciByteTable (
    (REG8_MASK*) (&Fch9004InitEnvHwAcpiPciTable[0]),
    sizeof (Fch9004InitEnvHwAcpiPciTable) / sizeof (REG8_MASK)
    );
  ProgramFchEnvSpreadSpectrumV80 (FchHwAcpi);

  //DESSPIODRTL-353 (GPIO120:PD=0/PU=1)
  xUSLMemReadModifyWrite32 (
    (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK1_BASE + FCH_REGE0),
    ~(BIT_32(21) + BIT_32(20)), BIT_32(20)
    );


  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPostPcieHwAcpiPSn
 *
 * @brief Config FCH ACPI Module (Preliminary) after PCI enumeration. It's no need to re-run in S3 resume.
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPostPcieHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{


  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPostPcieHwAcpiSn
 *
 * @brief Config FCH ACPI Module before PCI enumeration
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPostPcieHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{


  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreOsHwAcpiPSn
 *
 * @brief Config FCH ACPI Module (Preliminary) before OS Boot. It's no need to re-run in S3 resume.
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPreOsHwAcpiPSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{


  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreOsHwAcpiSn
 *
 * @brief Config FCH ACPI Module before Os Boot
 *
 * @param FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
void
FchInitPreOsHwAcpiSn (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{


  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * InitializeApiFchHwAcpiSn
 *
 * @brief   Initialize internal APIs for FCH HwAcpi
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchHwAcpiSn (void)
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable (SilId_FchHwAcpiP, &mFchHwAcpiXferSn);
}
