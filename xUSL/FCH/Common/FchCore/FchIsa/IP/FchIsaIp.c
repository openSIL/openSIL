/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsaIp.c
 * @brief FCH ISA sub-controller functions
 *
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchReg.h>
#include <RcMgrIp2Ip.h>
#include <MsrReg.h>
#include <CpuLib.h>
#include <IP/FchIsaIp.h>
#include <Mmio.h>
#include <SmnAccess.h>
#include <Pci.h>

/**
 * FchInitResetLpc
 * @brief Configures Lpc controller during Power-On
 *
 * @param[in] FchLpc Fch configuration structure pointer.
 *
 */
void
FchInitResetLpc (
  FCH_LPC             *FchLpc
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS,
    FCH_LPC_DEV,
    FCH_LPC_FUNC,
    FCHD14F3x0D0 + 1
    ),
    0xDF,
    FchLpc->LpcClk0 ? 0x20 : 0
    );

  xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS,
    FCH_LPC_DEV,
    FCH_LPC_FUNC,
    FCHD14F3x0D0 + 1
    ),
    0xBF,
    FchLpc->LpcClk1 ? 0x40 : 0
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchIsaRelocateRom3
 * @brief Relocate ROM3 if TOM2 exceed 0xFD_0000_0000
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 *
 */
void
FchIsaRelocateRom3 (
  SIL_CONTEXT *SilContext
  )
{
  uint64_t              MmioBase;
  uint64_t              TOM2;
  uint64_t              Length;
  uint32_t              MmioBaseLo, MmioBaseHi;
  FABRIC_TARGET         MmioTarget;
  FABRIC_MMIO_ATTRIBUTE Attributes;
  RCMGR_IP2IP_API       *RcMgrIp2Ip;
  SIL_STATUS            Status;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MMIO allocator API is not found.\n");
    return;
  }

  MmioBase = 0xFD00000000;
  Length = 0x4000000;
  TOM2 = xUslRdMsr(MSR_TOM2);

  MmioTarget.RbNum = 0;
  MmioTarget.SocketNum = 0;

  if (TOM2 > MmioBase) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Relocating ROM3\n");
    MmioTarget.TgtType = TARGET_PCI_BUS;
    MmioTarget.PciBusNum = 0;
    MmioTarget.PciSegNum = 0;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    Attributes.MmioType = NON_PCI_DEVICE_ABOVE_4G;

    MmioBase = 0;
    Status = RcMgrIp2Ip->FabricReserveMmio(SilContext,
      &MmioBase,
      &Length,
      0x3FFFFFF,
      MmioTarget,
      &Attributes
      );
    if (Status != SilPass) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "Allocate MMIO Fail\n");
      return;
    }

    MmioBaseHi = (uint32_t)((MmioBase >> 32) & 0xffffffff);
    MmioBaseLo = (uint32_t)(MmioBase & 0xffffffff);

    //Program BAR_64MB_ROM3
    xUSLMemWrite32((void *)(size_t)(SPI_BASE + SPIx00000064),
      MmioBaseHi
      );
    xUSLMemWrite32((void *)(size_t)(SPI_BASE + SPIx00000060),
      MmioBaseLo
      );

    //Program FCH_SDP_BIOSROM_BAR
    xUSLSmnWrite(0, 0, FCHREGx02DC6000 + SDPRSMUx000000D4, MmioBaseHi);
    xUSLSmnWrite(0, 0, FCHREGx02DC6000 + SDPRSMUx000000D0, MmioBaseLo);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void
FchIsaTpmInit (
  FCH_SPI *FchSpiData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (0 == FchSpiData->SystemTpmConfig) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Setting TPM init registers\n");
    // Enable widertpmen
    xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCHD14F3x07C
      ),
      (uint8_t) ~BIT_8(7),
      BIT_8(7)
      );
    // Enable routetpm2spi
    xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCHD14F3x0A0),
      (uint8_t) ~BIT_8(3),
      BIT_8(3)
      );
    // Enable drv_spiclk_earlier to fix TPM CS error issue
    xUSLMemReadModifyWrite32((void *)(size_t)(SPI_BASE + FCH_LPCHOSTSPIREG_SPI_CNTRL0_REGISTER),
      ~BIT_32(13),
      BIT_32(13)
      );
    // Enable tpm_di_to_enable to fix hang on dTPM selection without SPI TPM connected
    xUSLMemReadModifyWrite16((void *)(size_t)(SPI_BASE + SPIx0000002E),
      (uint16_t) ~BIT_16(9),
      BIT_16(9)
      );
  } else {
    xUSLPciReadModifyWrite8(PCI_LIB_ADDRESS(FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCHD14F3x07C
      ),
      (uint8_t) ~(BIT_8(0) + BIT_8(1)),
      0
      );
  }

  xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCHD14F3x0DC),
    ~BIT_32(0),
    BIT_32(0)
    );
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
