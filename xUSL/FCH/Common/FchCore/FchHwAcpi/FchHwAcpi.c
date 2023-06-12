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
#include "IP/Fch80.h"

extern FCHHWACPI_INPUT_BLK mFchHwAcpiDefaults;

static const ACPI_REG_WRITE MmioPreliminaryPrePcieResetTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {AOAC_BASE >> 8, FCH_AOAC_POWER_SAVING_MODE_CTRL + 0x01, 0x3F, 0x00},
  {PMIO_BASE >> 8, FCH_PM_PCICONTROL + 0x02, 0xEF, BIT_32(4)},
  {PMIO_BASE >> 8, FCH_PM_MISC_PMIO, 0xEF, 0x00},
  {PMIO_BASE >> 8, FCH_PM_PMIODEBUG, 0xBF, 0x00},
  {PMIO_BASE >> 8, FCH_PM_S5_RESET_STATUS, 0x00, BIT_32(1)},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE MmioPrePcieResetTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_DECODEEN + 2, 0xFB, BIT_32(2)},
  {PMIO_BASE >> 8, FCH_PM_PMIODEBUG, 0xCF, 0x00},
  {PMIO_BASE >> 8, FCH_PM_DECODEEN, 0xF7, 0x77},
  {PMIO_BASE >> 8, FCH_PM_PCICONTROL, 0xFE, BIT_32(4)},
  {PMIO_BASE >> 8, FCH_PM_PWRRSTCFG, 0xFC, 0x00},
  {PMIO_BASE >> 8, FCH_PM_SERIALIRQCONFIG, 0x00, BIT_32(4) + BIT_32(6)},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG, 0xF6, BIT_32(0)},
  {PMIO_BASE >> 8, FCH_PM_RESETCOMMAND, 0xEE, 0x04},
  {PMIO_BASE >> 8, FCH_PM_S5_RESET_STATUS + 2, 0xBF, 0x40},
  {PMIO_BASE >> 8, FCH_PM_RESETCOMMAND, 0xFB, 0},
  {PMIO_BASE >> 8, FCH_PM_PMIO_XDC_SATACONFIG, 0x00, 0},
  {PMIO_BASE >> 8, FCH_PM_PMIO_XDC_SATACONFIG + 1, 0x00, 0},
  {PMIO_BASE >> 8, FCH_PM_PMIO_XDC_SATACONFIG + 2, 0x00, 0},
  {PMIO_BASE >> 8, FCH_PM_PMIO_XDC_SATACONFIG + 3, 0x00, 0},
  {PMIO_BASE >> 8, FCH_PM_S0RESETTMR + 2, 0xFB, BIT_32(2)},
  {PMIO_BASE >> 8, FCH_PM_S5_RESET_STATUS + 3, 0x00, BIT_32(5)},
  {MISC_BASE >> 8, FCH_MISC_ECOBIT2 + 3, 0xF7, BIT_32(3)},
  {MISC_BASE >> 8, FCH_MISC_CGPLL_CONFIGURATION2 + 2, 0xFB, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE MmioEnvInitTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG, 0x00, BIT_32(0) + BIT_32(1) + BIT_32(2) + BIT_32(4)},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG + 2, 0x00, BIT_32(1) + BIT_32(2)},
  {PMIO_BASE >> 8, FCH_PM_ACPICONFIG + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PM_S_STATECONTROL, 0xF7, BIT_32(3)},
  {PMIO_BASE >> 8, FCH_PM_THROTTLINGCONTROL, 0xFD, BIT_32(1)},
  {SMI_BASE >> 8, FCH_SMI_GEVENT1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_GEVENT3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_GEVENT4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_GEVENT5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_GEVENT6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_GEVENT23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_GEVENT8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_SCITRIG, 0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_SCILEVL + 2, 0xF7, BIT_32(3)},
  {SMI_BASE >> 8, FCH_SMI_TWRN, 0, 9},
  {SMI_BASE >> 8, FCH_SMI_FANTHGEVENT, 0, 13},
  {SMI_BASE >> 8, SIL_RESERVED_0282, 0, 16},
  {SMI_BASE >> 8, SIL_RESERVED_0281, 0, 17},
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
  {0x04, 0xBF, BIT_32(6)},
  {0x05, 0xFE, BIT_32(0)},
  {0xFF, 0xFF, 0xFF},
  {0x00, LPC_BUS_DEV_FUN, 0},
  {0x04, 0xBF, BIT_32(6)},
  {0x05, 0xFE, BIT_32(0)},
  {0xFF, 0xFF, 0xFF},
};

static const ACPI_REG_WRITE FchInitEnvRasMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},
  {SMI_BASE >> 8, FCH_SMI_SMICONTROL5 + 1, 0xFC, BIT_32(0)},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchHwAcpiWriteMmioTable
 * @brief Program FCH ACPI MMIO register by table (8 bits data)
 *
 * @param[in] AcpiTbl - Table data pointer
 *
 */
void FchHwAcpiWriteMmioTable (
  ACPI_REG_WRITE *AcpiTbl
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
 * FchHwAcpiEnableLpcIo
 * @brief Enables/Disables LPC IO decoding
 *
 * @param[in] FchDataPtr Fch Data Block
 *
 */
static
void FchHwAcpiEnableLpcIO (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  if ((FchDataPtr->FchBldCfg.CfgSioPmeBaseAddress != 0x00) && (FchDataPtr->FchBldCfg.CfgSioPmeBaseAddress != 0xFFFF)) {
    xUSLPciWrite16(PCI_LIB_ADDRESS(FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCH_LPCPCICFG_PCI_IO_BASE_ADDRESS_FOR_WIDEGENERICPORT
      ),
      (FchDataPtr->FchBldCfg.CfgSioPmeBaseAddress)
      );
    xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCH_LPCPCICFG_IO_MEM_PORT_DECODE_ENABLE
      ),
      0x00,
      BIT_32(2)
      );
  }
}

/**
 * FchHwAcpiConfigreWdt
 * @brief Enables/Disables Watch Dog Timer
 *
 * @param[in] FchDataPtr Fch Data Block
 *
 */
static
void FchHwAcpiConfigureWdt (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
    0x7F,
    FchDataPtr->WdtEnable ? BIT_32(7) : 0
    );
}

/**
 * FchHwAcpiUartInit
 * @brief Configures Fch AMBA Uart
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static
void FchHwAcpiUartInit (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  uint32_t FchDeviceEnMap;
  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilId_FchHwAcpiP, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }

  FchDeviceEnMap = FchDataPtr->FchRunTime.FchDeviceEnableMap;

  // UART0
  if ( FchDeviceEnMap & BIT_32(11)) {
    FchAoacPowerOnDev(FCH_AOAC_UART0, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX135_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX136_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX137_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX138_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX139_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      FCH_GPIO_UART0_CTS_L_UART2_TXD_AGPIO135),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_UART0_RXD_AGPIO136),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      FCH_GPIO_UART0_RTS_L_UART2_RXD_AGPIO137),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_UART0_TXD_AGPIO138),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_UART0_INTR_AGPIO139),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  }

  // UART1
  if ( FchDeviceEnMap & BIT_32(12)) {
    FchAoacPowerOnDev(FCH_AOAC_UART1, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX141_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX142_GPIO), 0, 0x0);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_UART1_RXD_AGPIO141),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + SIL_RESERVED_0314),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  }

  // UART2
  if ( FchDeviceEnMap & BIT_32(16)) {
    FchAoacPowerOnDev(FCH_AOAC_UART2, 1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX135_GPIO), 0, 0x1);
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX137_GPIO), 0, 0x1);
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      FCH_GPIO_UART0_CTS_L_UART2_TXD_AGPIO135),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE +
      FCH_GPIO_UART0_RTS_L_UART2_RXD_AGPIO137),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  }

  // PSP_INTR0
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + FCH_IOMUX_IOMUX89_GPIO), 0, 0x1);

  FchHwAcpiXfer->FchHwAcpiUartLegacyIoInit(FchDataPtr);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiAlinkRasEnable
 * @brief Enable FCH A-Link parity error
 *
 * @param[in] FchHwAcpi Fch HwAcpi configuration structure pointer.
 *
 */
static
void FchHwAcpiAlinkRasEnable (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (FchHwAcpi->FchAlinkRasSupport) {
    RwAlink(SIL_RESERVED_0310 | (uint32_t) (ABCFG << 29), ~BIT_32(3), BIT_32(3));
    ProgramPciByteTable((REG8_MASK *) (&FchInitEnvRasPciTable[0]), sizeof (FchInitEnvRasPciTable) / sizeof (REG8_MASK));
    FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (&FchInitEnvRasMmioTable[0]));
  } else {
    RwAlink(SIL_RESERVED_0310 | (uint32_t) (ABCFG << 29), ~BIT_32(3), 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiAl2ahbInit
 * @brief Config Fch AL2AHB init
 *
 */
static
void FchHwAcpiAl2ahbInit (
  void
  )
{
  // Enable Clock Gating
  xUSLMemReadModifyWrite8((void *)(size_t)(FCH_AL2AHB_CFG_AL2AHB_CONTROL), 0xFF, BIT_32(1));
  xUSLMemReadModifyWrite8((void *)(size_t)(FCH_AL2AHB_CFG_AL2AHB_CONTROL_HCLK), 0xFF, BIT_32(1));
}

/**
 * FchHwAcpiHpetInit
 * @brief Programs FCH HPET
 *
 * @param[in] FchDataPtr Pointer to Fch Data Block
 *
 */
static
void FchHwAcpiHpetInit (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  bool FchHpetTimer;
  bool FchHpetMsiDis;

  FchHpetTimer = FchDataPtr->Hpet.HpetEnable;
  FchHpetMsiDis = FchDataPtr->Hpet.HpetMsiDis;

  if (FchHpetTimer == true) {
    // Enabling decoding of HPET MMIOProgram the HPET BAR address
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
      0xFF,
      BIT_32(6)
      );
    // Enable HPET MSI support
    if ( FchHpetMsiDis == false ) {
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
        ~BIT_32(29),
        BIT_32(29)
        );
    } else {
      xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN), ~BIT_32(29), 0);
    }
  }
}

/**
 * FchHwAcpiAcLossControl
 *
 * @param[in] AcLossControlValue
 *
 */
static void
FchHwAcpiAcLossControl (
  uint8_t AcLossControlValue
  )
{
  AcLossControlValue &= 0x03;
  AcLossControlValue |= BIT_32(2);
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCSHADOW), 0xF0, AcLossControlValue);
}

/**
 * FchHwAcpiProgramPcieNativeMode
 * @brief Configures Pcie Native Mode
 *
 * @param[in] FchDataPtr Pointer to FCH data block
 *
 */
static
void FchHwAcpiProgramPcieNativeMode (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint8_t        FchNativepciesupport;
  FchNativepciesupport = (uint8_t) FchDataPtr->Misc.NativePcieSupport;

  // PCIE Native setting
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_S_STATECONTROL + 1), 0xBF, 0);
  if ( FchNativepciesupport == 1) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG + 3),
      0xF4,
      BIT_8(3) + BIT_8(0)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ACPICONFIG + 3),
      0xF4,
      BIT_8(3)
      );
  }
}

/**
 * FchHwAcpiEnableIOBase
 * @brief Enables IO Base Addresses in ACPI/SmBus module during Power-On
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static void FchHwAcpiEnableIOBase (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint16_t       SmbusBase;

  // Configure FCH components IO base addresses
  SmbusBase = FchDataPtr->FchBldCfg.CfgSmbus0BaseAddress;
  SmbusBase &= 0xFF00;
  xUSLMemReadModifyWrite16((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_DECODEEN),
    0x00FF,
    SmbusBase + BIT_32(4)
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
}

/**
 * FchHwAcpiEnableSmBus
 * @brief Enable SmBus during Power-On
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void FchHwAcpiEnableSmBus (
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
 * FchHwAcpiInitWaFeature
 * @brief common configuration and Workaround on ACPI module
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
static
void FchHwAcpiInitWaFeature (
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
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA), 0xFE, BIT_32(0));
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
  Value = Value | BIT_32(4);
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
  if ((Value & (BIT_32(2) + BIT_32(0))) != 0) {
    Value = 0x40;
    xUSLIoWrite8(AsfPort, Value);
  }

  if (FchDataPtr->FchOscout1ClkContinous) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SERIALIRQCONFIG), 0xBF, 0);
  }
  if (FchDataPtr->SerialIrqEnable) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_SERIALIRQCONFIG), 0x7F, BIT_32(7));
  }

  // Clear RTCD Date Alarm
  Value = 0x0D;
  xUSLIoWrite8(FCH_IO_NMI_ENABLE, Value);
  Value = xUSLIoRead8(FCH_IO_RTCDATAPORT);
  if ((Value & 0x3F) == 0x3F) {
    Value &= 0x80;
    xUSLIoWrite8(FCH_IO_RTCDATAPORT, Value);
  }

  // Disable ASF slave
  if (FchDataPtr->FchAsfCfg.DisableSlave) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + ASF_BASE + SIL_RESERVED_0313), 0xEF, BIT_32(4));
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + ASF_BASE + SIL_RESERVED_0313), 0xEF, 0);
  }

  // FCH IOAPIC ID configuration
  ApicIdValue = (FchDataPtr->FchIoApicId << 24);
  ApicReg = FCH_IOAPIC_ID_REG;
  //Backup IOAPIC Index
  ApicIndexBackup = xUSLMemRead32((void *)(size_t)(FCH_IOAPIC_INDEX));
  //Program IOAPIC Index to IOAPIC ID REG
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_INDEX), ApicReg);
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_INDEX), ApicIdValue);
  xUSLMemWrite32((void *)(size_t)(FCH_IOAPIC_INDEX), ApicIndexBackup);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "FchIoapicValue Value 0x%x\n", ApicIdValue);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param[in] FchDataPtr FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchHwAcpi  Fch HwAcpi configuration structure pointer.
 *
 */
void FchHwAcpiPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  // Note: FchInitResetHwAcpi starts here

  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;
  if (SilGetCommon2RevXferTable(SilId_FchHwAcpiP, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }

  // Enable LPC IO decoding
  FchHwAcpiEnableLpcIO(FchDataPtr);

  // Configure FCH components IO base addresses
  FchHwAcpiEnableIOBase(FchDataPtr);

  // Enable SmBus
  FchHwAcpiEnableSmBus(FchDataPtr);

  // Write pre-pcie Mmio table
  FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (&MmioPrePcieResetTable[0]));

  // Boot timer configurations starts from here

  FchHwAcpiEnableBootTimerV80(FchHwAcpi);

  FchHwAcpiConfigureWdt(FchDataPtr);

  FchHwAcpiXfer->FchHwAcpiResetRtcExt ();

  FchHwAcpiInitWaFeature(FchDataPtr);

  FchHwAcpiXfer->FchHwAcpiEnableIxC(FchDataPtr);

  if ( FchDataPtr->ToggleAllPwrGoodOnCf9 ) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PWRRSTCFG), 0xFD, 2);
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_PWRRSTCFG), 0xFD, 0);
  }

  // Note: FchInitEnvHwAcpiP starts here

  FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (&MmioEnvInitTable[0]));

  if (FchHwAcpi->NoClearThermalTripSts == false) {
    // Note: Other bits are Write-1-to-clear. Hence the AND Mask of 0x00
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_S5_RESET_STATUS), 0, BIT_32(0));
  }

  FchHwAcpiProgramSpreadSpectrumV80(FchHwAcpi);

  // Note: FchInitEnvHwAcpi starts here

  FchHwAcpiXfer->FchInitPrePcieHwAcpi(FchDataPtr, FchHwAcpi);

  if ( FchDataPtr->Smbus.SmbusSsid != 0 ) {
    xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_ISA_BUS, FCH_ISA_DEV, FCH_ISA_FUNC, 0x2C),
      0,
      FchDataPtr->Smbus.SmbusSsid
      );
  }

  if ( FchDataPtr->Misc.NoneSioKbcSupport ) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_USBGATING),
      (uint8_t) ~BIT_8(1),
      BIT_8(1)
      );
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_USBGATING),
      (uint8_t) ~BIT_8(1),
      0
      );
  }

  FchHwAcpiProgramPcieNativeMode(FchDataPtr);

  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_ECOBIT2 + 2), 0xDF, 0x20);
  // DESSPIODRTL-353 (GPIO120:PD=0/PU=1)
  xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK1_BASE + FCH_GPIO_INT_ISOCEVENT2),
    ~(BIT_32(21) + BIT_32(20)),
    BIT_32(20)
    );

  // Enable handling AC loss
  FchHwAcpiAcLossControl((uint8_t) FchHwAcpi->PwrFailShadow);

  FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (FchHwAcpi->OemProgrammingTablePtr));

  // Enable HPET
  FchHwAcpiHpetInit(FchDataPtr);

  // Enable AL2AHB
  FchHwAcpiAl2ahbInit ();

  // Enable Uart
  FchHwAcpiUartInit(FchDataPtr, FchHwAcpi);

  // Enable Alink RAS
  FchHwAcpiAlinkRasEnable(FchHwAcpi);

  // Configure AOAC
  FchHwAcpiXfer->FchHwAcpiAoacInit(FchHwAcpi);
}

/**
 * FchHwAcpiPreliminaryPrePcieInit
 * @brief Config HwAcpi controller during power-on
 *
 * @param[in] FchDataPtr FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchHwAcpi  Fch HwAcpi configuration structure pointer.
 *
 */
void FchHwAcpiPreliminaryPrePcieInit (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *) (&MmioPreliminaryPrePcieResetTable[0]));

  // SoC-Specific call
  FCH_HWACPI_XFER_TABLE  *FchHwAcpiXfer;
  if (SilGetCommon2RevXferTable(SilId_FchHwAcpiP, (void **)(&FchHwAcpiXfer)) != SilPass) {
    return;
  }
  FchHwAcpiXfer->FchInitPreliminaryPrePcieHwAcpi(FchDataPtr, FchHwAcpi);
}

/**
 * FchHwAcpiPreliminarySetInputBlk
 * @brief Sets FCH ACPI Default Common Input Data
 *
 * @retval SIL_STATUS
 */
SIL_STATUS
FchHwAcpiPreliminarySetInputBlk (
  void
  )
{
  FCHHWACPI_INPUT_BLK *FchHwAcpiPreliminaryInput;

  FchHwAcpiPreliminaryInput = (FCHHWACPI_INPUT_BLK *) SilCreateInfoBlock(SilId_FchHwAcpiP,
    sizeof (FCHHWACPI_INPUT_BLK),
    FCHHWACPI_INSTANCE,
    FCHHWACPI_MAJOR_REV,
    FCHHWACPI_MINOR_REV
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchHwAcpiPreliminarySetInputBlk at: 0x%x \n", FchHwAcpiPreliminaryInput);
  if (FchHwAcpiPreliminaryInput == NULL) {
    return SilAborted;
  }

  //Fill Fch SPI structure with defaults
  memcpy((void *)FchHwAcpiPreliminaryInput, &mFchHwAcpiDefaults, sizeof (FCHHWACPI_INPUT_BLK));

  return SilPass;
}
