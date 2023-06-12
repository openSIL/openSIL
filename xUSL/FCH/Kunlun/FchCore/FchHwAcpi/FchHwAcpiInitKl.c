/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiInitKl.c
 * @brief Kunlun FCH ACPI Module silicon init functions
 *
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <FchHwAcpi-api.h>
#include <Fch.h>
#include <FchCommon.h>
#include <FchReg.h>
#include <Pci.h>
#include <FchCore/FchHwAcpi/FchHwAcpiCmn2Rev.h>
#include <FchCore/FchHwAcpi/FchHwAcpi.h>
#include "FchHwAcpiCmn2Kl.h"

#define SIL_RESERVED_0359    0x0900

extern const ACPI_REG_WRITE FchHwAcpiRtcExtTable[];

static const REG8_MASK FchKunlunInitEnvHwAcpiPciTable[] =
{
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {0x10, 0X00, (SIL_RESERVED_0359 & 0xFF)},
  {0x11, 0X00, (SIL_RESERVED_0359 >> 8)},
  {0xFF, 0xFF, 0xFF},
};

typedef struct {
  uint32_t field0;
  uint32_t field1;
  uint32_t field2;
  uint32_t field3;
  uint32_t field4;
  uint32_t field5;
} SIL_RESERVED_STRUCT_0016;

SIL_RESERVED_STRUCT_0016 mESPISlave0Decode[SIL_RESERVED_0273] = {
  // Io Range 0
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(8),                            // Io Range Enable Bit
    SIL_RESERVED_0316,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0324,          // Io Range Size Register Address
    0                                     // Io Range Size Offset
  },

  // Io Range 1
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(9),                            // Io Range Enable Bit
    SIL_RESERVED_0316,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0324,          // Io Range Size Register Address
    8                                     // Io Range Size Offset
  },

  // Io Range 2
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(10),                           // Io Range Enable Bit
    SIL_RESERVED_0317,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0324,          // Io Range Size Register Address
    16                                    // Io Range Size Offset
  },

  // Io Range 3
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(11),                           // Io Range Enable Bit
    SIL_RESERVED_0317,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0324,          // Io Range Size Register Address
    24                                    // Io Range Size Offset
  },

  // Io Range 4
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(16),                           // Io Range Enable Bit
    SIL_RESERVED_0318,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0325,         // Io Range Size Register Address
    0                                     // Io Range Size Offset
  },

  // Io Range 5
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(17),                           // Io Range Enable Bit
    SIL_RESERVED_0318,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0325,         // Io Range Size Register Address
    8                                     // Io Range Size Offset
  },

  // Io Range 6
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(18),                           // Io Range Enable Bit
    SIL_RESERVED_0319,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0325,         // Io Range Size Register Address
    16                                    // Io Range Size Offset
  },

  // Io Range 7
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(19),                           // Io Range Enable Bit
    SIL_RESERVED_0319,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0325,         // Io Range Size Register Address
    24                                    // Io Range Size Offset
  },

  // Io Range 8
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(20),                           // Io Range Enable Bit
    SIL_RESERVED_0320,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0326,         // Io Range Size Register Address
    0                                     // Io Range Size Offset
  },

  // Io Range 9
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(21),                           // Io Range Enable Bit
    SIL_RESERVED_0320,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0326,         // Io Range Size Register Address
    8                                     // Io Range Size Offset
  },

  // Io Range 10
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(22),                           // Io Range Enable Bit
    SIL_RESERVED_0321,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0326,         // Io Range Size Register Address
    16                                    // Io Range Size Offset
  },

  // Io Range 11
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(23),                           // Io Range Enable Bit
    SIL_RESERVED_0321,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0326,         // Io Range Size Register Address
    24                                    // Io Range Size Offset
  },

  // Io Range 12
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(24),                           // Io Range Enable Bit
    SIL_RESERVED_0322,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0327,         // Io Range Size Register Address
    0                                     // Io Range Size Offset
  },

  // Io Range 13
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(25),                           // Io Range Enable Bit
    SIL_RESERVED_0322,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0327,         // Io Range Size Register Address
    8                                     // Io Range Size Offset
  },

  // Io Range 14
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(26),                           // Io Range Enable Bit
    SIL_RESERVED_0323,     // Io Range Base Register Address
    0,                                    // Io Range Base Offset
    SIL_RESERVED_0327,         // Io Range Size Register Address
    16                                    // Io Range Size Offset
  },

  // Io Range 15
  {
    SIL_RESERVED_0315,        // Io Range Enable Register Address
    BIT_32(27),                           // Io Range Enable Bit
    SIL_RESERVED_0323,     // Io Range Base Register Address
    16,                                   // Io Range Base Offset
    SIL_RESERVED_0327,         // Io Range Size Register Address
    24                                    // Io Range Size Offset
  }
};

/**
 * FchInitPreliminaryPrePcieHwAcpiKl
 * @brief Configures FCH ACPI module before PCI enumeration
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchInitPreliminaryPrePcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPrePcieHwAcpiKl
 * @brief Configures FCH ACPI module before PCI enumeration
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchInitPrePcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  ProgramPciByteTable((REG8_MASK *) (&FchKunlunInitEnvHwAcpiPciTable[0]),
    sizeof (FchKunlunInitEnvHwAcpiPciTable) / sizeof (REG8_MASK)
    );
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreliminaryPostPcieHwAcpiKl
 * @brief Configures FCH ACPI module after PCI enumeration.
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 */
void
FchInitPreliminaryPostPcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPostPcieHwAcpiKl
 * @brief Configures FCH ACPI module before PCI enumeration
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchInitPostPcieHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreliminaryPreOsHwAcpiKl
 * @brief Configures FCH ACPI module before OS Boot.
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchInitPreliminaryPreOsHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreOsHwAcpiKl
 * @brief Configures FCH ACPI Module before Os Boot
 *
 * @param FchDataPtr Pointer to FCH data block
 * @param FchHwAcpi  Pointer to FCH ACPI data block
 *
 */
void
FchInitPreOsHwAcpiKl (
  FCHCLASS_INPUT_BLK  *FchDataPtr,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchHwAcpiEnableIxCKl
 * @brief Enables FCH IxC module
 *
 * @param FchDataPtr Pointer to FCH data block
 *
 */
void
FchHwAcpiEnableIxCKl (
  FCHCLASS_INPUT_BLK *FchDataPtr
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
  uint8_t  Mask8 = 0xFF;
  uint32_t checkField;

  Or8 = 0x1; // SprFwdCtrl

  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Or8, Mask8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));

  // check if RTCext data lost
  if (RtcExtData == 0xFF) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  RTCext data lost\n");
    return false;
  }

  // check if SprFwdHour is valid, SprFwdCtrl][5:0] should be 0,1
  checkField = RtcExtData & 0x3F;
  if (checkField > 0x1) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  SprFwdHour is invalid, 0x%x\n", checkField);
    return false;
  }

  Or8 = 0x2; // SprFwdMonth
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Or8, Mask8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));
  // check if SprFwdMonth is valid, SprFwdMonth][4:0] should be 0,1
  checkField = RtcExtData & 0x1F;
  if (checkField > 0x1) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  SprFwdMonth is invalid, 0x%x\n", checkField);
    return false;
  }

  Or8 = 0x3; // FallBackHour
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Or8, Mask8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));

  // check if FallBackHour is valid, FallBackHour[5:0] should be 00h ~ 09h,10h~19h,20~24h
  checkField = RtcExtData & 0x3F;
  if ((checkField > 0x9 && checkField < 0x10)
    || (checkField > 0x19 && checkField < 0x20)
    || (checkField > 0x24)) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  FallBackHour is invalid, 0x%x\n", checkField);
    return false;
  }

  Or8 = 0x4; // FallBackMonth
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Or8, Mask8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));

  // check if FallBackMonth is valid, FallBackMonth][4:0] should be 00h ~ 09h,10h~12h
  checkField = RtcExtData & 0x1F;
  if ((checkField > 0x9 && checkField < 0x10)
    || (checkField > 0x12)) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  FallBackMonth is invalid, 0x%x\n", checkField);
    return false;
  }

  Or8 = 0x10; // WeekTimerControl
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTINDEX), Or8, Mask8);
  RtcExtData = xUSLMemRead8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_RTCEXTDATA));

  // check if WeekTimerControl is valid, WeekTimerControl][2:1] should be 00h ~ 02h
  checkField = (RtcExtData & 0x6) >> 1;
  if (checkField > 0x2) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "FchRtcextValid:  WeekTimerControl is invalid, 0x%x\n", checkField);
    return false;
  }
  return true;
}

/**
 * FchHwAcpiResetRtcExtKl
 * @brief Resets external RTC if it does not carry valid data
 *
 */
void
FchHwAcpiResetRtcExtKl (
  void
  )
{
  if (!IsRtcExtValid ()) {
    FchHwAcpiWriteMmioTable((ACPI_REG_WRITE *)(&FchHwAcpiRtcExtTable[0]));
  }
}

/**
 * FchHwAcpiEnableIxCKl
 * @brief Enables FCH IxC module
 *
 * @param FchDataPtr Pointer to FCH data block
 *
 */
static
void
DisableESPILegacyUARTDecoding (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint32_t i;
  uint32_t j;
  uint32_t AL2AHBIoEnableRange[4] = {0x2E8, 0x2F8, 0x3E8, 0x3F8};

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "%a Start\n", __FUNCTION__);

  for (i = 0; i < SIL_RESERVED_0273; i++) {
    FCH_TRACEPOINT(SIL_TRACE_ENTRY, "IO Range %d\n", i);
    for (j = 0; j < sizeof (AL2AHBIoEnableRange) / sizeof (uint32_t); j++) {
      if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & (1 << j)
        && (ACPIMMIO32(SIL_RSVD_ADDR_FEC20000 + mESPISlave0Decode[i].field0) &
        mESPISlave0Decode[i].field1)
        && ((ACPIMMIO32(SIL_RSVD_ADDR_FEC20000 + mESPISlave0Decode[i].field2) >>
        mESPISlave0Decode[i].field3) & 0xFFFF) == AL2AHBIoEnableRange[j]
        ) {
        FCH_TRACEPOINT(SIL_TRACE_ENTRY,
          "Disable decode 0x%x Bit 0x%x\n",
          SIL_RSVD_ADDR_FEC20000 + mESPISlave0Decode[i].field2,
          mESPISlave0Decode[i].field1
          );
        ACPIMMIO32(SIL_RSVD_ADDR_FEC20000 + mESPISlave0Decode[i].field2) &=
          ~mESPISlave0Decode[i].field1;
      }

      if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & (1 << j)
        && (ACPIMMIO32(SIL_RSVD_ADDR_FEC30000 + mESPISlave0Decode[i].field0) &
        mESPISlave0Decode[i].field1)
        && ((ACPIMMIO32(SIL_RSVD_ADDR_FEC30000 + mESPISlave0Decode[i].field2) >>
        mESPISlave0Decode[i].field3) & 0xFFFF) == AL2AHBIoEnableRange[j]
        ) {
        FCH_TRACEPOINT(SIL_TRACE_ENTRY,
          "Disable decode 0x%x Bit 0x%x\n",
          SIL_RSVD_ADDR_FEC30000 + mESPISlave0Decode[i].field2,
          mESPISlave0Decode[i].field1
          );
        ACPIMMIO32(SIL_RSVD_ADDR_FEC30000 + mESPISlave0Decode[i].field2) &=
          ~mESPISlave0Decode[i].field1;
      }
    }
  }
}

/**
 * FchHwAcpiUartLegacyIoInitKl
 * @brief Initializes UARTs legacy IOs
 *
 * @param FchDataPtr Pointer to FCH data block
 *
 */
void
FchHwAcpiUartLegacyIoInitKl (
  FCHCLASS_INPUT_BLK *FchDataPtr
  )
{
  uint32_t UartLegacyClockSelect = 0;
  if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable ) {
    DisableESPILegacyUARTDecoding(FchDataPtr);
    ACPIMMIO16(FCH_AL2AHB_CFG_AL2AHB_LEGACY_UART_IO_ENABLE) = FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable;
    if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & 0x01 ) {
      UartLegacyClockSelect |= (uint32_t) (1 << ((FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable >> 8) & 3));
    }
    if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & 0x02 ) {
      UartLegacyClockSelect |= (uint32_t) (1 << ((FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable >> 10) & 3));
    }
    if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & 0x04 ) {
      UartLegacyClockSelect |= (uint32_t) (1 << ((FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable >> 12) & 3));
    }
    if ( FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable & 0x08 ) {
      UartLegacyClockSelect |= (uint32_t) (1 << ((FchDataPtr->FchRunTime.Al2AhbLegacyUartIoEnable >> 14) & 3));
    }
    xUSLPciReadModifyWrite32(PCI_LIB_ADDRESS(FCH_ISA_BUS, FCH_ISA_DEV, FCH_ISA_FUNC, SIL_RESERVED_0337),
      ~(uint32_t)(BIT_32(31) + BIT_32(30) + BIT_32(29) + BIT_32(28)),
      UartLegacyClockSelect << 28
      );
  }
}

/**
 * FchHwAcpiAoacInitKl
 * @brief Initializes FCH AOAC module
 *
 * @param FchHwAcpi Pointer to FCH ACPI data block
 *
 */
void
FchHwAcpiAoacInitKl (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  )
{
}
