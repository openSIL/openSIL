/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpi.c
 * @brief FCH ACPI data and functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FchHwAcpi-api.h>
#include <FCH/FchClass-api.h>
#include <FchHwAcpiCmn2Rev.h>
#include <FCH/Common/FchCore/FchAb/FchAlinkLib.h>
#include <FCH/Common/FchCore/FchAb/FchAbReg.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchReg.h>
#include <CommonLib/Io.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>
#include <CpuLib.h>
#include "FchHwAcpi.h"
#include "FchAoacLib.h"

extern FCHHWACPI_INPUT_BLK mFchHwAcpiDefaults;

static const ACPI_REG_WRITE MmioPreliminaryPrePcieResetTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {AOAC_BASE >> 8, AOACx00000094 + 0x01, 0x3F, 0x00},
  {PMIO_BASE >> 8, FCH_PM_PCICONTROL + 0x02, 0xEF, BIT_8(4)},
  {PMIO_BASE >> 8, FCH_PM_PMIODEBUG, 0xBF, 0x00},
  {PMIO_BASE >> 8, FCH_PM_S5_RESET_STATUS, 0x00, BIT_8(1)},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE MmioPrePcieResetTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_DECODEEN + 2, 0xFB, BIT_8(2)},
  {PMIO_BASE >> 8, FCH_PM_PMIODEBUG, 0xCF, 0x00},
  {PMIO_BASE >> 8, FCH_PM_DECODEEN, 0xF7, 0x77},
  {PMIO_BASE >> 8, FCH_PM_PCICONTROL, 0xFE, BIT_8(4)},
  {PMIO_BASE >> 8, FCH_PM_PWRRSTCFG, 0xFC, 0x00},
  {PMIO_BASE >> 8, FCH_PM_SERIALIRQCONFIG, 0x00, BIT_8(4) + BIT_8(6)},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG, 0xF6, BIT_8(0)},
  {PMIO_BASE >> 8, FCH_PM_RESETCOMMAND, 0xEE, 0x04},
  {PMIO_BASE >> 8, FCH_PM_S5_RESET_STATUS + 2, 0xBF, 0x40},
  {PMIO_BASE >> 8, FCH_PM_RESETCOMMAND, 0xFB, 0},
  {MISC_BASE >> 8, FCH_MISC_ECOBIT2 + 3, 0xF7, BIT_8(3)},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE MmioEnvInitTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG, 0x00, BIT_8(0) + BIT_8(1) + BIT_8(2) + BIT_8(4)},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PM_S_STATECONTROL, 0xF7, BIT_8(3)},

  {MISC_BASE >> 8, FCH_MISC_ECOBIT2 + 2, 0xDF, BIT_8(5)},

  {SMI_BASE >> 8, FCH_SMI_GEVENT1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_GEVENT3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_GEVENT4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_GEVENT5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_GEVENT6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_GEVENT23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_GEVENT8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_SCITRIG, 0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_SCILEVL + 2, 0xF7, BIT_8(3)},
  {SMI_BASE >> 8, FCHSMI_48, 0, 9},
  {SMI_BASE >> 8, FCHSMI_44, 0, 13},
  {SMI_BASE >> 8, FCHSMI_29, 0, 16},
  {SMI_BASE >> 8, FCHSMI_30, 0, 17},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

const ACPI_REG_WRITE FchHwAcpiRtcExtTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x01},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x02},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x03},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x04},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x10},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x11},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x12},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x13},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTINDEX, 0x00, 0x14},
  {PMIO_BASE >> 8, FCH_PM_RTCEXTDATA, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

static const REG8_MASK FchInitEnvRasPciTable[] =
{
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {0x04, 0xBF, BIT_8(6)},
  {0x05, 0xFE, BIT_8(0)},
  {0xFF, 0xFF, 0xFF},
  {0x00, LPC_BUS_DEV_FUN, 0},
  {0x04, 0xBF, BIT_8(6)},
  {0x05, 0xFE, BIT_8(0)},
  {0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE FchInitEnvRasMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {SMI_BASE >> 8, FCH_SMI_SMICONTROL5 + 1, 0xFC, BIT_8(0)},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchHwAcpiWriteMmioTable
 * @brief Program FCH ACPI MMIO register by table (8 bits data)
 *
 * @param[in] AcpiTbl - Table data pointer
 *
 */
void
FchHwAcpiWriteMmioTable (
  const ACPI_REG_WRITE *AcpiTbl
  )
{
  uint8_t  Or8;
  uint8_t  Mask8;
  uint32_t Var;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (AcpiTbl != NULL) {
    if ((AcpiTbl->MmioReg == 0) && (AcpiTbl->MmioBase == 0) &&
      (AcpiTbl->DataAndMask == 0xB0) && (AcpiTbl->DataOrMask == 0xAC)) {
      // Signature Checking
      AcpiTbl++;
      while (AcpiTbl->MmioBase < 0x1D) {
        Var = ACPI_MMIO_BASE | (AcpiTbl->MmioBase) << 8 | AcpiTbl->MmioReg;
        Or8 = AcpiTbl->DataOrMask;
        Mask8 = AcpiTbl->DataAndMask;
        xUSLMemReadModifyWrite8((void *)(size_t)Var, Mask8, Or8);
        AcpiTbl++;
      }
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchHwAcpiConfigureWdt
 * @brief Enables/Disables Watch Dog Timer
 *
 * @param[in] FchDataPtr Fch Data Block
 *
 */
static
void
FchHwAcpiConfigureWdt (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
    0x7F,
    FchDataPtr->WdtEnable ? BIT_8(7) : 0
    );
}

/**
 * FchHwAcpiProgramCpuRstBTmr
 * @brief Configures CPU reset button
 *
 */
void
FchHwAcpiProgramCpuRstBTmr (
  void
  )
{
}

/**
 * IsRtcExtValid
 * @brief Returns true if the external RTC data is valid
 *
 */
static
bool
IsRtcExtValid (
  void
  )
{
  uint8_t  RtcExtData;
  uint8_t  Or8;
  uint8_t  Mask8 = 0x0;
  uint32_t checkField;

  //
  // SprFwdCtrl
  //
  Or8 = 0x1;
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Mask8, Or8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if RTCext data lost
  if (RtcExtData == 0xFF) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "RTCext data lost\n");
    return false;
  }
  // check if SprFwdHour is valid
  checkField = RtcExtData & 0x3F;
  if (checkField > 0x1) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "SprFwdHour is invalid, 0x%x\n", checkField);
    return false;
  }

  //
  // SprFwdMonth
  //
  Or8 = 0x2;
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Mask8, Or8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if SprFwdMonth is valid
  checkField = RtcExtData & 0x1F;
  if (checkField > 0x1) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "SprFwdMonth is invalid, 0x%x\n", checkField);
    return false;
  }

  //
  // FallBackHour
  //
  Or8 = 0x3;
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Mask8, Or8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if FallBackHour is valid
  checkField = RtcExtData & 0x3F;
  if ((checkField > 0x9 && checkField < 0x10)
    || (checkField > 0x19 && checkField < 0x20)
    || (checkField > 0x24)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "FallBackHour is invalid, 0x%x\n", checkField);
    return false;
  }

  //
  // FallBackMonth
  //
  Or8 = 0x4;
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Mask8, Or8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if FallBackMonth is valid
  checkField = RtcExtData & 0x1F;
  if ((checkField > 0x9 && checkField < 0x10)
    || (checkField > 0x12)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "FallBackMonth is invalid, 0x%x\n", checkField);
    return false;
  }

  //
  // WeekTimerControl
  //
  Or8 = 0x10;
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Mask8, Or8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if WeekTimerControl is valid
  checkField = (RtcExtData & 0x6) >> 1;
  if (checkField > 0x2) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "WeekTimerControl is invalid, 0x%x\n", checkField);
    return false;
  }
  return true;
}

/**
 * FchHwAcpiResetRtcExt
 *
 * @brief Reset RTCEXT if any invalid field detected.
 */
static
void
FchHwAcpiResetRtcExt (
  void
  )
{
  if (!IsRtcExtValid ()) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Program RTCEXT table\n");
    FchHwAcpiWriteMmioTable(FchHwAcpiRtcExtTable);
  } else {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "RTCEXT is valid, no need to program\n");
  }
}

/**
 * FchHwAcpiAlinkRasEnable
 * @brief Enable FCH A-Link parity error
 *
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static
void
FchHwAcpiAlinkRasEnable (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (FchHwAcpi->FchAlinkRasSupport) {
    RwAlink(ABx0CDC_xC001_0050 | (uint32_t) (ABCFG << 29), ~BIT_32(3), BIT_32(3));
    ProgramPciByteTable(FchInitEnvRasPciTable,
      sizeof (FchInitEnvRasPciTable) / sizeof (FchInitEnvRasPciTable[0])
      );
    FchHwAcpiWriteMmioTable(FchInitEnvRasMmioTable);
  } else {
    RwAlink(ABx0CDC_xC001_0050 | (uint32_t) (ABCFG << 29), ~BIT_32(3), 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiAl2ahbInit
 * @brief Config Fch AL2AHB init
 *
 */
static
void
FchHwAcpiAl2ahbInit (
  void
  )
{
  // Enable Clock Gating
  xUSLMemReadModifyWrite8((void *)(size_t)(AL2AHBJTAGxFEDC0010), 0xFD, BIT_8(1));
  xUSLMemReadModifyWrite8((void *)(size_t)(AL2AHBJTAGxFFEDC0030), 0xFD, BIT_8(1));
}

/**
 * FchHwAcpiHpetInit
 * @brief Programs FCH HPET
 *
 * @param[in] FchDataPtr Pointer to Fch Data Block
 *
 */
static
void
FchHwAcpiHpetInit (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  bool FchHpetTimer;
  bool FchHpetMsiDis;

  FchHpetTimer = FchDataPtr->Hpet.HpetEnable;
  FchHpetMsiDis = FchDataPtr->Hpet.HpetMsiDis;

  if (FchHpetTimer == true) {
    // Enabling decoding of HPET MMIO. Program the HPET BAR address
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
      0xFF,
      BIT_8(6)
      );
    // Enable/disable HPET MSI support
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
      ~BIT_32(29),
      FchHpetMsiDis ? 0 : BIT_32(29)
      );
  }
}

/**
 * FchHwAcpiAcLossControl
 *
 * @param[in] AcLossControlValue
 *
 */
static
void
FchHwAcpiAcLossControl (
  uint8_t AcLossControlValue
  )
{
  AcLossControlValue &= 0x03;
  AcLossControlValue |= BIT_32(2);
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCSHADOW),
    0xF0,
    AcLossControlValue
    );
}

/**
 * FchHwAcpiProgramPcieNativeMode
 * @brief Configures Pcie Native Mode
 *
 * @param[in] FchDataPtr Pointer to FCH data block
 *
 */
static
void
FchHwAcpiProgramPcieNativeMode (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  // PCIE Native setting
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_S_STATECONTROL + 1),
    0xBF,
    0
    );
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG + 3),
    0xF4,
    FchDataPtr->Misc.NativePcieSupport ? BIT_8(3) + BIT_8(0) : BIT_8(3)
    );
}

/**
 * FchHwAcpiEnableIOBase
 * @brief Enables IO Base Addresses in ACPI/SmBus module during Power-On
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void
FchHwAcpiEnableIOBase (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint16_t       SmbusBase;

  // Configure FCH components IO base addresses
  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
    0x00FF,
    SmbusBase + BIT_16(4)
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPM1EVTBLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPm1EvtBlkAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPM1CNTBLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPm1CntBlkAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPMTMRBLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiPmTmrBlkAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICPUCNTBLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgCpuControlBlkAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIGPE0BLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgAcpiGpe0BlkAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPISMICMD),
    0x0000,
    FchDataPtr->FchBldCfg.CfgSmiCmdPortAddr
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPMACNTBLK),
    0x0000,
    FchDataPtr->FchBldCfg.CfgSmiCmdPortAddr + 8
    );
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPM2CNTBLK),
    0x0000,
    0xFFFF
    );
}

/**
 * FchHwAcpiEnableSmBus
 * @brief Enable SmBus during Power-On
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void
FchHwAcpiEnableSmBus (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint16_t       SmbusBase;
  uint8_t        Value;

  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;

  // SmBus init
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
    ~(BIT_32(19) + BIT_32(20)),
    0
    );
  Value = 0x00;
  xUSLIoWrite8(SmbusBase + 0x14, Value);
}

/**
 * FchHwAcpiEnableBootTimer
 * @brief Enables Boot Timer
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void
FchHwAcpiEnableBootTimer (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  if (FchHwAcpi->BootTimerEnable) {
    // Set boot timer cold reset bit
    if (FchHwAcpi->BootTimerResetType) {
      // Cold Reset
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SPIPADPUPD + 3), 0x7F, 0x80);
    } else {
      // Warm Reset
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SPIPADPUPD + 3), 0x7F, 0x00);
    }

    // clear boot timer, enable boot timer
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_BOOTTIMEREN + 3), 0x67, 0x88);
  } else {
    // clear boot timer, disable boot timer
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_BOOTTIMEREN + 3), 0x67, 0x80);
  }
}

/**
 * FchHwAcpiInitWaFeature
 * @brief common configuration and Workaround on ACPI module
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void
FchHwAcpiInitWaFeature (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint16_t       SmbusBase;
  uint8_t        Value;
  uint16_t       AsfPort;
  uint32_t       GeventEnableBits;
  uint32_t       GeventValue;
  uint32_t       ApicIdValue;
  uint32_t       ApicReg;
  uint32_t       ApicIndexBackup;

  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;

  // RTC Workaround for Daylight saving time enable bit
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), 0, 0);
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA), 0xFE, BIT_8(0));
  Value = 0x0B;
  xUSLIoWrite8(FCH_IO_NMI_ENABLE, Value);
  Value = xUSLIoRead8(FCH_IO_RTCDATAPORT);
  Value &= 0xFE;
  xUSLIoWrite8(FCH_IO_RTCDATAPORT, Value);
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), 0, 0);
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA), 0xFE, 0);

  // Prevent RTC error
  Value = 0x0A;
  xUSLIoWrite8(FCH_IO_NMI_ENABLE, Value);
  Value = xUSLIoRead8(FCH_IO_RTCDATAPORT);
  Value &= 0xEF;
  xUSLIoWrite8(FCH_IO_RTCDATAPORT, Value);

  Value = 0x08;
  xUSLIoWrite8(FCH_IO_PCI_INTR_INDEX, Value);
  Value = xUSLIoRead8(FCH_IO_PCI_INTR_DATA);

  if (!FchDataPtr->EcKbd) {
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    Value = Value | 0x0A;
  }
  xUSLIoWrite8(FCH_IO_PCI_INTR_DATA, Value);

  Value = 0x09;
  xUSLIoWrite8(FCH_IO_PCI_INTR_INDEX, Value);
  Value = xUSLIoRead8(FCH_IO_PCI_INTR_DATA);
  if (!FchDataPtr->EcKbd) {
    // Route SIO IRQ1/IRQ12 to USB IRQ1/IRQ12 input
    Value = Value & 0xF9;
  }

  Value = Value & 0x9F;
  Value = Value | BIT_8(4);
  xUSLIoWrite8(FCH_IO_PCI_INTR_DATA, Value);

  AsfPort = SmbusBase + 0x20;
  if ( AsfPort != 0 ) {
    uint8_t  dbValue;
    dbValue = 0x2F;
    xUSLIoWrite8(AsfPort + 0x0A, dbValue);
  }

  // PciExpWakeStatus workaround
  AsfPort = xUSLMemRead16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPIPM1EVTBLK));
  AsfPort++;
  GeventEnableBits = xUSLMemRead32((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_EVENT_ENABLE));
  GeventValue = xUSLMemRead32((void *)(size_t)(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_EVENT_STATUS));
  if ((GeventValue & GeventEnableBits) != 0 ) {
    Value = 0x40;
    xUSLIoWrite8(AsfPort, Value);
  }
  Value = xUSLIoRead8(AsfPort);
  if ((Value & (BIT_8(2) + BIT_8(0))) != 0) {
    Value = 0x40;
    xUSLIoWrite8(AsfPort, Value);
  }

  if (FchDataPtr->FchOscout1ClkContinuous) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SERIALIRQCONFIG), 0xBF, 0);
  }
  if (FchDataPtr->SerialIrqEnable) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SERIALIRQCONFIG), 0x7F, BIT_8(7));
  }

  // Clear RTCD Date Alarm
  Value = 0x0D;
  xUSLIoWrite8(FCH_IO_NMI_ENABLE, Value);
  Value = xUSLIoRead8(FCH_IO_RTCDATAPORT);
  if ((Value & 0x3F) == 0x3F) {
    Value &= 0x80;
    xUSLIoWrite8(FCH_IO_RTCDATAPORT, Value);
  }

  // FCH IOAPIC ID configuration
  ApicIdValue = (FchDataPtr->FchIoApicId << 24);
  ApicReg = FCH_IOAPIC_ID_REG;
  //Backup IOAPIC Index
  ApicIndexBackup = xUSLMemRead32((void *)(size_t)(FCH_IOAPIC_INDEX));
  //Program IOAPIC Index to IOAPIC ID REG
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_INDEX), ApicReg);
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_DATA), ApicIdValue);
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_INDEX), ApicIndexBackup);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "FchIoapicValue Value 0x%x\n", ApicIdValue);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitTogglePwrGdOnCf9
 * @brief Configures PowerGood behavior on CF9 reset
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void
FchInitTogglePwrGdOnCf9 (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PWRRSTCFG),
    (uint8_t) ~BIT_8(1),
    FchHwAcpi->ToggleAllPwrGoodOnCf9 ? BIT_8(1) : 0
    );
}

/**
 * IsExternalClockMode
 * @brief   Returns true if in external mode clock otherwise returns false
 * @retval  true or false
 *
 */
static
bool
IsExternalClockMode (void)
{
  uint8_t    MISC80;
  MISC80 = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_STRAPSTATUS + 2));
  return ((bool) ((MISC80 & BIT_8(1)) == 0));
}

/**
 * FchHwAcpiProgramSpreadSpectrum
 * @brief Configures SpreadSpectrum before PCI
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param[in] FchHwAcpi Pointer to FCH ACPI configuration structure
 *
 */
static
void
FchHwAcpiProgramSpreadSpectrum (
  SIL_CONTEXT         *SilContext,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;
  uint32_t FchMiscCgpllconfig1;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SilGetCommon2RevXferTable(SilContext, SilId_FchHwAcpi, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }

  if (IsExternalClockMode ()) {
    FchHwAcpi->SpreadSpectrum = false;
  }
  FchMiscCgpllconfig1 = xUSLMemRead32((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_CGPLLCONFIG1));

  if ((uint32_t)FchHwAcpi->SpreadSpectrum != (FchMiscCgpllconfig1 & BIT_32(0))) {
    // SSC config different from current state, need to update
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Need to update SSC enable status to: %u\n", FchHwAcpi->SpreadSpectrum);
    FchHwAcpiXfer->FchHwAcpiSpreadSpectrum(FchHwAcpi->SpreadSpectrum);
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
FchHidInitPerCtrlr (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  uint32_t Controller
  )
{
  xUSLMemReadModifyWrite32(
    (void *)(size_t)(FCH_HID_BASE_ADDRESS - (Controller * HID_CONTROLLER_STEP) + 0x00),
    ~(BIT_32(18) + BIT_32(29) + BIT_32(30)),
    ((FchDataPtr->FchRunTime.HidControl[Controller].SpiReadMode & 1) << 18) |
      ((FchDataPtr->FchRunTime.HidControl[Controller].SpiReadMode & 6) << 28)
    );
  xUSLMemReadModifyWrite32(
    (void *)(size_t)(FCH_HID_BASE_ADDRESS - (Controller * HID_CONTROLLER_STEP) + 0x20),
    0x000FFFFF,
    ((FchDataPtr->FchRunTime.HidControl[Controller].SpiSpeed & 0xf) << 20) |
    ((FchDataPtr->FchRunTime.HidControl[Controller].SpiSpeed & 0xf) << 24) |
    ((FchDataPtr->FchRunTime.HidControl[Controller].SpiSpeed & 0xf) << 28)
    );
  xUSLMemReadModifyWrite16(
    (void *)(size_t)(FCH_HID_BASE_ADDRESS - (Controller * HID_CONTROLLER_STEP) + 0x6c),
    0xc0c0,
    (FchDataPtr->FchRunTime.HidControl[Controller].Spi_spd6 & 0x3f) |
      ((FchDataPtr->FchRunTime.HidControl[Controller].Spi_spd7 & 0x3f) << 8)
    );
  xUSLMemReadModifyWrite32(
    (void *)(size_t)(FCH_HID_BASE_ADDRESS - (Controller * HID_CONTROLLER_STEP) + 0x150),
    ~(uint32_t) (BIT_32(15)),
    BIT_32(15)
    );
}

/**
 * ProgramFchEnvHfpInit - Host Finger Print configuration
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
ProgramFchEnvHfpInit (
    FCHCLASS_INPUT_BLK *FchDataPtr,
    FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  if (FchHwAcpi->FchHfpEnable) {
    //
    // Enable ACPI HFP device
    //
    FchDataPtr->FchRunTime.FchDeviceEnableMap |= BIT_32(29);
  }

  xUSLMemReadModifyWrite8((void *)(size_t)(FCH_HFP_BASE_ADDRESS + 0x1d), 0xFC, 0x2);
  xUSLMemReadModifyWrite8((void *)(size_t)(FCH_HFP_BASE_ADDRESS + 0x02), 0x3F, 0x0);
}

/**
 * ProgramFchEnvHidInit - Host Hid configuration
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void
ProgramFchEnvHidInit (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  if (FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(30)) {
    FchHidInitPerCtrlr(FchDataPtr, 0);
  }
  if (FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(31)) {
    FchHidInitPerCtrlr(FchDataPtr, 1);
  }
}

/**
 * FchHwAcpiPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchDataPtr            FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param FchHwAcpi             Fch HwAcpi configuration structure pointer.
 *
 */
void
FchHwAcpiPrePcieInit (
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  // FchInitResetHwAcpi

  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;
  if (SilGetCommon2RevXferTable(SilContext, SilId_FchHwAcpi, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }

  // Configure FCH components IO base addresses
  FchHwAcpiEnableIOBase(FchDataPtr);

  // Enable SmBus
  FchHwAcpiEnableSmBus(FchDataPtr);

  // Write pre-pcie Mmio table
  FchHwAcpiWriteMmioTable(MmioPrePcieResetTable);

  // Boot timer configurations

  FchHwAcpiEnableBootTimer(FchHwAcpi);

  FchHwAcpiConfigureWdt(FchDataPtr);

  FchHwAcpiProgramCpuRstBTmr ();

  FchHwAcpiResetRtcExt ();

  FchHwAcpiInitWaFeature(FchDataPtr);

  FchInitTogglePwrGdOnCf9(FchHwAcpi);

  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG),
    ~BIT_32(17),
    FchHwAcpi->AcpiShutdownMessage ? BIT_32(17) : 0
    );

  // FchInitEnvHwAcpiP

  FchHwAcpiWriteMmioTable(MmioEnvInitTable);

  if (FchHwAcpi->NoClearThermalTripSts == false) {
    // Note: Other bits are Write-1-to-clear. Hence the AND Mask of 0x00
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_S5_RESET_STATUS), 0, BIT_8(0));
  }

  FchHwAcpiProgramSpreadSpectrum(SilContext, FchHwAcpi);

  // FchInitEnvHwAcpi

  FchHwAcpiXfer->FchHwAcpiPrePcieInit(FchDataPtr, FchHwAcpi);

  if ( FchDataPtr->Smbus.SmbusSsid != 0 ) {
    xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_ISA_BUS, FCH_ISA_DEV, FCH_ISA_FUNC, 0x2C),
      0,
      FchDataPtr->Smbus.SmbusSsid
      );
  }

  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_USBGATING),
    (uint8_t) ~BIT_8(1),
    FchDataPtr->Misc.NoneSioKbcSupport ? BIT_8(1) : 0
    );

  FchHwAcpiProgramPcieNativeMode(FchDataPtr);

  // Enable handling AC loss
  FchHwAcpiAcLossControl((uint8_t) FchHwAcpi->PwrFailShadow);

  FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (FchHwAcpi->OemProgrammingTablePtr));

  // Enable HPET
  FchHwAcpiHpetInit(FchDataPtr);

  // Enable AL2AHB
  FchHwAcpiAl2ahbInit ();

  // Enable I2C/I3C
  FchHwAcpiXfer->FchHwAcpiI2cI3cInit(FchDataPtr);

  // Enable Uart
  FchHwAcpiXfer->FchHwAcpiUartInit(FchDataPtr);

  // Enable Alink RAS
  FchHwAcpiAlinkRasEnable(FchHwAcpi);

  // Configure AOAC
  FchHwAcpiXfer->FchHwAcpiAoacInit(FchHwAcpi);

  ProgramFchEnvHfpInit(FchDataPtr, FchHwAcpi);
  ProgramFchEnvHidInit(FchDataPtr);

  FchHwAcpiXfer->FchHwAcpiZstateTiming ();
}

/**
 * FchHwAcpiPreliminaryPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchDataPtr            FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param FchHwAcpi             Fch HwAcpi configuration structure pointer.
 *
 */
void
FchHwAcpiPreliminaryPrePcieInit (
  SIL_CONTEXT        *SilContext,
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FchHwAcpiWriteMmioTable(MmioPreliminaryPrePcieResetTable);
}

/**
 * FchHwAcpiPreliminarySetInputBlk
 * @brief Sets FCH ACPI Default Common Input Data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval SIL_STATUS
 */
SIL_STATUS
FchHwAcpiSetInputBlk (
  SIL_CONTEXT          *SilContext
  )
{
  FCHHWACPI_INPUT_BLK *FchHwAcpiPreliminaryInput;

  FchHwAcpiPreliminaryInput = (FCHHWACPI_INPUT_BLK *) SilCreateInfoBlock(SilContext,
    SilId_FchHwAcpi,
    sizeof (FCHHWACPI_INPUT_BLK),
    FCHHWACPI_INSTANCE,
    FCHHWACPI_MAJOR_REV,
    FCHHWACPI_MINOR_REV
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchHwAcpiPreliminarySetInputBlk at: 0x%x \n", FchHwAcpiPreliminaryInput);
  if (FchHwAcpiPreliminaryInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Spi structure with defaults
  memcpy((void *)FchHwAcpiPreliminaryInput, &mFchHwAcpiDefaults, sizeof (FCHHWACPI_INPUT_BLK));

  return SilPass;
}
