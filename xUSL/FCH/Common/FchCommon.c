/**
 * @file  FchCommon.c
 * @brief OpenSIL FCH common functions.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "FchCommon.h"
#include "Fch.h"
#include <FCH/Common/FchHelper.h>
#include <FCH/FchIp2Ip.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <SilPcie.h>
#include <Pci.h>

#define FCH_AOAC_RETRY   100
#define FCHOEM_IO_DELAY_PORT 0x80
#define FCHOEM_ELAPSED_TIME_UNIT 28
#define FCHOEM_ELAPSED_TIME_DIVIDER 100
#define FCH_MAX_TIMER 0xFFFFFFFFul

#define FCH_PMIOA_REG64 0x64 // AcpiPmTmrBlk
#define ACPI_MMIO_BASE 0xFED80000ul
#define AOAC_BASE 0x1E00
#define FCH_AOAC_ESPI         0x1B

static FCH_IP2IP_API mFchApi = {
  .Header = {
    .IpId    = SilId_SmuClass
  },
  .FchStall  = SilFchStall
};

/**
 * SilFchStall
 *
 * @brief This function provides a software delay for a specified number of microseconds
 *
 * @param MicroSeconds The number of microsecond to delay
 */
void
SilFchStall (
  uint32_t MicroSeconds
  )
{
  uint16_t TimerAddr;
  uint32_t StartTime;
  uint32_t ElapsedTime;
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  SilFchReadPmio (FCH_PMIOA_REG64, AccessWidth16, (uint8_t *)&TimerAddr);
  if ((TimerAddr == 0) || (TimerAddr == 0xFFFF)) {
    MicroSeconds = MicroSeconds / 2;
    while (MicroSeconds != 0) {
      StartTime = xUSLIoRead8 (FCHOEM_IO_DELAY_PORT);
      MicroSeconds--;
    }
  } else {
    StartTime = xUSLIoRead8 (TimerAddr);
    for ( ;; ) {
      ElapsedTime = xUSLIoRead8 (TimerAddr);
      if (ElapsedTime < StartTime) {
        ElapsedTime = ElapsedTime + FCH_MAX_TIMER - StartTime;
      } else {
        ElapsedTime = ElapsedTime - StartTime;
      }
      if ((ElapsedTime * FCHOEM_ELAPSED_TIME_UNIT / FCHOEM_ELAPSED_TIME_DIVIDER) > MicroSeconds) {
        break;
      }
    }
  }
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**-----------------------------------------------------------------------------
 * FchAoacPowerOnDev - Power On/Off Fch Device
 *
 * @param[in] Device     - Device to be turn on or off
 * @param[in] On         - 1 to turn on, 0 to turn off
 *
 */

/**
 * FchAoacPowerOnDev - Power On/Off Fch Device
 *
 * @param[in] Device     - Device to be turn on or off
 * @param[in] On         - 1 to turn on, 0 to turn off
 *
 */
void FchAoacPowerOnDev (uint8_t Device, uint8_t On)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  uint8_t  Retry = 0;
  //uint8_t  Check;

  /*Check =*/ xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1)));
  if (On == 1) {
    while (((xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1))) & 7) != 7) && (Retry < FCH_AOAC_RETRY)) {
      xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)),
                                (uint8_t)~(BIT_8(0) + BIT_8(1) + BIT_8(7)), BIT_8(3));
      SilFchStall (10);
      Retry++;
    }

    if ((xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1))) & 7) != 7) {
      // Log timeout failure
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "[FCH]FchAoacPowerOnDev Power ON Device 0x%x failed!\n", Device);
      // Power On one more time
      xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)),  0xFF, BIT_32(3));
    }
  } else {
    while ((xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1))) != 0) && (Retry < FCH_AOAC_RETRY)) {
      xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)),
                                (uint8_t)~(BIT_8(3) + BIT_8(7)), 0);
      SilFchStall (10);
      Retry++;
    }

    if ((xUSLMemRead8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1))) & 7) != 0) {
      // Log timeout fail
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "[FCH]FchAoacPowerOnDev Power OFF Device 0x%x failed!\n", Device);
      // Power Off with SW control
      xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)),  0, 0x80);
      xUSLMemWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)), 0x80);
    }

    if (Device != FCH_AOAC_ESPI) {  //PLAT-26858
      xUSLMemReadModifyWrite8 ((void *)(size_t)(ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)),  0xFF, BIT_32(0) + BIT_32(1));
    }
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * ProgramPciByteTable - Program PCI register by table (8 bits data)
 *
 *
 *
 * @param[in] pPciByteTable    - Table data pointer
 * @param[in] dwTableSize      - Table length
 * @param[in] StdHeader
 *
 */
void ProgramPciByteTable (REG8_MASK *pPciByteTable, uint16_t dwTableSize)
{
  uint8_t     i;
  uint8_t     dbBusNo;
  uint8_t     dbDevFnNo;
  uint32_t     PciAddress;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  dbBusNo = pPciByteTable->RegIndex;
  dbDevFnNo = pPciByteTable->AndMask;
  pPciByteTable++;

  for ( i = 1; i < dwTableSize; i++ ) {
    if ( (pPciByteTable->RegIndex == 0xFF) && (pPciByteTable->AndMask == 0xFF) && (pPciByteTable->OrMask == 0xFF) ) {
      pPciByteTable++;
      dbBusNo = pPciByteTable->RegIndex;
      dbDevFnNo = pPciByteTable->AndMask;
      pPciByteTable++;
      i++;
    } else {
      PciAddress = (dbBusNo << 20) + (dbDevFnNo << 12) + pPciByteTable->RegIndex;
      xUSLPciReadModifyWrite8 (PciAddress, pPciByteTable->AndMask, pPciByteTable->OrMask);
      pPciByteTable++;
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}



/**
 * InitializeFchApi
 *
 * @brief   Initialize Ip-2-Ip APIs for FCH common code
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeFchApi (void)
{
  // Initialize FCH IP-to-IP API
  return SilInitIp2IpApi (SilId_FchClass, (void *)&mFchApi);
}
