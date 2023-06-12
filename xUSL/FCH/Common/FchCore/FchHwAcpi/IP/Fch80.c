/**
 * @file  FchHwAcpi80.c
 * @brief FCH HW ACPI Silicon code for FCH 8.0 IP
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <FCH/Common/FchCommonCfg.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchHelper.h>
#include <CommonLib/Mmio.h>
#include <SMU/SmuIp2Ip.h>
#include <Pci.h>

/*********************************************************************************************************************/
/*                                       FCH 8.0 (Genoa) IP code                                                     */
/*********************************************************************************************************************/
/**
 * FchInitEnableBootTimerV80  - Enable Boot Timer
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void FchInitEnableBootTimerV80 (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  if (FchHwAcpi->BootTimerEnable) {
    // Set boot timer cold reset bit
    if (FchHwAcpi->BootTimerResetType) {
      // Cold Reset
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG90 + 3), 0x7F, 0x80);
    } else {
      // Warm Reset
      xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG90 + 3), 0x7F, 0x00);
    }

    // clear boot timer, enable boot timer
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG44 + 3), 0x67, 0x88);
  } else {
    // clear boot timer, disable boot timer
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG44 + 3), 0x67, 0x80);
  }
}

/**
 * FchInitTogglePwrGdOnCf9 - configure PowerGood behavior on CF9 reset
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void FchInitTogglePwrGdOnCf9 (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  if ( FchHwAcpi->ToggleAllPwrGoodOnCf9 ) {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10), 0xFD, 0x02);
  } else {
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG10), 0xFD, 0x00);
  }
}

/**
 * ProgramFchEnvSpreadSpectrumV80 - Config SpreadSpectrum before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void ProgramFchEnvSpreadSpectrumV80 (FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  uint8_t  FchSpreadSpectrum;
  uint32_t FchMiscCgpllconfig1;

  FchSpreadSpectrum = FchHwAcpi->SpreadSpectrum;

  if (IsExternalClockMode ()) {
    FchSpreadSpectrum = 0;
  }
//  The SSC -0.3% enable sequence is: (replaced the default SSC enable sequence)
//.    Program MISCx14[23:8] FCH::MISC::CGPLLCONFIG4[cg1pll_fcw1_frac_override] == 0x1333
//.    Program MISCx18[31:16] FCH::MISC::CGPLLCONFIG5[cg1pll_fcw_slew_frac_override] == 0x3A
//.    Program MISCx10[29]=1 FCH::MISC::CGPLLCONFIG3[cg1pll_fracn_en_override]= 1.
//.    Program MISCx08[0]=1  FCH::MISC::CGPLLCONFIG1[cg1_spread_spectrum_enable] = 1
//.    Program MISCx40[25]=1   FCH::MISC::MISCCLKCNTRL0[cg1_fbdiv_loaden] = 1.
//.    Program MISCx40[30]=1 FCH::MISC::MISCCLKCNTRL0[cg1_cfg_update_req] = 1.
  FchMiscCgpllconfig1 = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x08));

  if ( FchSpreadSpectrum ) {
    if ( FchMiscCgpllconfig1 & BIT_32(0) ) {
      //SSC is already enabled
    } else {
      //Enable SSC
      SilFchStall (5);
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x14), 0xFF0000FF, 0x00133300);
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x18), 0x0000FFFF, 0x003A0000);
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x10), ~BIT_32(29), BIT_32(29));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x08), ~BIT_32(0), BIT_32(0));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x40), ~BIT_32(25), BIT_32(25));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x48), ~BIT_32(11), BIT_32(11));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x40), ~BIT_32(30), BIT_32(30));
      SilFchStall (15);
    }
  } else {
    if ( FchMiscCgpllconfig1 & BIT_32(0) ) {
      //Disable SSC
      SilFchStall (5);
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x10), ~BIT_32(29), BIT_32(29));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x08), ~BIT_32(0), 0x00);
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x48), ~BIT_32(11), BIT_32(11));
      xUSLMemReadModifyWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + MISC_BASE + 0x40), ~BIT_32(30), BIT_32(30));
      SilFchStall (15);
    } else {
      //SSC is already disabled
    }
  }
}

/**
 * FchUartInitV80 - Config Fch AMBA Uart init
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void FchUartInitV80 (FCHCLASS_INPUT_BLK *FchDataPtr)
{
  uint32_t FchDeviceEnMap;
  FchDeviceEnMap = FchDataPtr->FchRunTime.FchDeviceEnableMap;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // UART0
  //
  if ( FchDeviceEnMap & BIT_32(11) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART0, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x87), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x88), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x89), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8A), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8B), 0, 0x0);

    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_21C_UART0_CTS_L_UART2_RXD_EGPIO135),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_220_UART0_RXD_EGPIO136),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_224_UART0_RTS_L_UART2_TXD_EGPIO137),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_228_UART0_TXD_EGPIO138),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_22C_UART0_INTR_AGPIO139),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART0, 0);
  }

  //
  // UART1
  //
  if ( FchDeviceEnMap & BIT_32(12) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART1, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8D), 0, 0x0);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x8E), 0, 0x0);

    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_234_UART1_RXD_EGPIO141),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_238_UART1_RTS_L_UART3_RXD_EGPIO142),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART1, 0);
  }

  //
  // UART2
  //
  if ( FchDeviceEnMap & BIT_32(16) ) {
    FchAoacPowerOnDev (FCH_AOAC_UART2, 1);
    // IOMUX
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x87), 0, 0x1);
    xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x89), 0, 0x1);

    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_21C_UART0_CTS_L_UART2_RXD_EGPIO135),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
    xUSLMemReadModifyWrite32 (
      (void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_224_UART0_RTS_L_UART2_TXD_EGPIO137),
      (uint32_t) ~(FCH_GPIO_OUTPUT_ENABLE << 16),
      0
      );
  } else {
    //FchAoacPowerOnDev (FCH_AOAC_UART2, 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPspIntr0 - Config PSP_INTR0
 *
 *
 *
 */
void FchInitPspIntr0 (void)
{
  xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + IOMUX_BASE + 0x59), 0, 0x1);
}

/**
 * FchI2cReleaseControl - Release SPD_HOST_CTRL_L
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
void FchI2cReleaseControl (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi)
{
  uint32_t          Value32;
  uint32_t          SmuStatus;
  uint32_t          SmuArg[6];
  SMU_IP2IP_API     *SmuApi;

  SmuStatus   = SMC_Result_Failed;
  SmuApi      = NULL;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Value32 = xUSLMemRead32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_BP_AGPIO3_SPD_HOST_CTRL_L));


  if ( FchHwAcpi->SpdHostCtrlRelease ) {
    FCH_TRACEPOINT (SIL_TRACE_INFO, "Release SPD host controller to BMC.\n");
    Value32 |= BIT_32(23);   // Output enable
    Value32 |= BIT_32(22);   // Output high
    xUSLMemWrite32 ((void *)(size_t)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_BP_AGPIO3_SPD_HOST_CTRL_L), Value32);
  } else {
    FCH_TRACEPOINT (SIL_TRACE_INFO, "Do not release SPD host controller to BMC.\n");
    if ( FchHwAcpi->DimmTelemetry ) {
      FCH_TRACEPOINT (SIL_TRACE_INFO, "Send message to SMU for PMFW poll DIMM telemetry.\n");

      // Send message to PMFW
      if (SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi) != SilPass) {
      FCH_TRACEPOINT (SIL_TRACE_ERROR, "Smu API not found!\n");
      assert (SmuStatus == SMC_Result_OK);
      }

      SmuApi->SmuServiceInitArguments (SmuArg);

      if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(21)) {
        SmuArg[0] = 1;  // Use I3C controller
      } else if ( FchDataPtr->FchRunTime.FchDeviceEnableMap & BIT_32(5)) {
        SmuArg[0] = 0;  // Use I2C controller
      }

      if ( SMC_Result_OK == SmuStatus ) {
        FCH_TRACEPOINT (SIL_TRACE_INFO, "Send message to SMU for PMFW poll DIMM telemetry successfully.\n");
      } else {
        FCH_TRACEPOINT (SIL_TRACE_INFO, "Send message to SMU for PMFW poll DIMM telemetry failed.\n");
      }
    }
  }

  return;
}
