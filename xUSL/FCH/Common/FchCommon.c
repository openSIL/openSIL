/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCommon.c
 * @brief OpenSIL FCH common functions.
 *
 */

#include <SilCommon.h>
#include "FchCommon.h"
#include "Fch.h"
#include <CommonLib/Mmio.h>
#include <CommonLib/Io.h>
#include <SilPcie.h>
#include <Pci.h>
#include <FchMultiFch-api.h>
#include "FchReg.h"

/**
 * SilFchStall
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
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  // Note: AcpiPmTmrBlk is a 32-bit timer. Timer value is at FCH::PM::TMRVALUE_ETMRVALUE register.
  // AcpiPmTmrBlk IO address is defined at FCH::PM::ACPIPMTMRBLK.
  SilFchReadPmio(FCH_PM_ACPIPMTMRBLK, AccessWidth16, (uint8_t *)&TimerAddr);
  if ((TimerAddr == 0) || (TimerAddr == 0xFFFF)) {
    MicroSeconds = MicroSeconds / 2;
    while (MicroSeconds != 0) {
      StartTime = xUSLIoRead8(FCHOEM_IO_DELAY_PORT);
      MicroSeconds--;
    }
  } else {
    StartTime = xUSLIoRead32(TimerAddr);
    for ( ;; ) {
      ElapsedTime = xUSLIoRead32(TimerAddr);
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
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * ProgramPciByteTable
 * @brief Programs PCI register by table (8 bits data)
 *
 * @param[in] pPciByteTable    - Table data pointer
 * @param[in] dwTableSize      - Table length
 *
 */
void ProgramPciByteTable (
  REG8_MASK *pPciByteTable,
  uint16_t dwTableSize
  )
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
    if ((pPciByteTable->RegIndex == 0xFF) && (pPciByteTable->AndMask == 0xFF) && (pPciByteTable->OrMask == 0xFF)) {
      pPciByteTable++;
      dbBusNo = pPciByteTable->RegIndex;
      dbDevFnNo = pPciByteTable->AndMask;
      pPciByteTable++;
      i++;
    } else {
      PciAddress = (dbBusNo << 20) + (dbDevFnNo << 12) + pPciByteTable->RegIndex;
      xUSLPciReadModifyWrite8(PciAddress, pPciByteTable->AndMask, pPciByteTable->OrMask);
      pPciByteTable++;
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * SilFchReadSleepType
 * @brief Returns current sleep type
 *
 */
uint8_t
SilFchReadSleepType (
  void
  )
{
  uint16_t     AcpiPmCntPort;
  uint16_t     Value16;

  SilFchReadPmio(FCH_PM_ACPIPM1CNTBLK, AccessWidth16, (uint8_t *)&AcpiPmCntPort);

  if ((AcpiPmCntPort == 0) || (AcpiPmCntPort == 0xFFFF)) {
    return 0xFF;
  }

  Value16 = xUSLIoRead16(AcpiPmCntPort);
  return (uint8_t) ((Value16 >> 10) & 7);
}

/**
 * SilFchWriteSleepType
 * @brief Sets the sleep type
 *
 *
 */
void
SilFchWriteSleepType (
  uint16_t SleepType
  )
{
  uint16_t     AcpiPmCntPort;

  if (SleepType > 5) {
    return;
  }

  SilFchReadPmio(FCH_PM_ACPIPM1CNTBLK, AccessWidth16, (uint8_t *)&AcpiPmCntPort);
  if ((AcpiPmCntPort == 0) || (AcpiPmCntPort == 0xFFFF)) {
    return;
  }
  xUSLIoReadModifyWrite16(AcpiPmCntPort, (uint16_t) ~(BIT_16(12) + BIT_16(11) + BIT_16(10)), (SleepType << 10));
}

/**
 * SilFchReadPmio
 * @brief Read from a PMIO register
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] Bytecount- Access sizes
 * @param[in] Value    - Read Data Buffer
 *
 */
void SilFchReadPmio (uint8_t Address, uint8_t AccessWidth, uint8_t *Values)
{
  for (size_t index = 0; index < AccessWidth; index++, Address++, Values++) {
    *Values = xUSLMemRead8((void *)((uintptr_t)(ACPI_MMIO_BASE + PMIO_BASE + Address)));
  }
}

/**
 * SilFchWritePmio
 * @brief Writes to a PMIO register
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value    - Write Data Buffer
 *
 */
void SilFchWritePmio (
  uint8_t Address,
  uint8_t AccessWidth,
  uint8_t *Values
  )
{
  for (size_t index = 0; index < AccessWidth; index++, Address++, Values++) {
    xUSLMemWrite8((void *)((uintptr_t)(ACPI_MMIO_BASE + PMIO_BASE + Address)), *Values);
  }
}

/**
 * SilFchRwPmio
 * @brief Read/Write PMIO
 *
 * @param[in] Address    - PMIO Offset value
 * @param[in] AccessWidth     - Access sizes
 * @param[in] AndMask    - Data And Mask 32 bits
 * @param[in] OrMask     - Data OR Mask 32 bits
 *
 */
void SilFchRwPmio (
  uint8_t  Address,
  uint8_t  AccessWidth,
  uint32_t AndMask,
  uint32_t OrMask
  )
{
  uint64_t Result = 0;

  SilFchReadPmio(Address, AccessWidth, (uint8_t *)&Result);
  Result = (Result & AndMask) | OrMask;
  SilFchWritePmio(Address, AccessWidth, (uint8_t *)&Result);
}

/**
 * LibFchIndirectIoRead
 * @brief Indirect I/O Read Access
 *
 */
void LibFchIndirectIoRead (uint8_t AccessWidth, uint16_t IoBase, uint8_t IndexAddress, uint8_t *Value)
{
  for (int i = 0; i < AccessWidth; i++, IndexAddress++) {
    xUSLIoWrite8(IoBase, IndexAddress);
    Value[i] = xUSLIoRead8(IoBase + 1);
  }
}

/**
 * LibFchIndirectIoWrite
 * @brief Indirect I/O Write Access
 *
 */
void
LibFchIndirectIoWrite (
  uint8_t  AccessWidth,
  uint16_t IoBase,
  uint8_t  IndexAddress,
  uint8_t  *Value
  )
{
  for (int i = 0; i < AccessWidth; i++, IndexAddress++) {
    xUSLIoWrite8(IoBase, IndexAddress);
    xUSLIoWrite8(IoBase + 1, Value[i]);
  }
}

/**
 * SilFchGpioSlotResetControl
 *
 * @brief   Gpio reset control.
 *
 * @details This function will try to deassert slot reset for each die in the system. If multi-die
 *          is not supported for the SoC, this function will utilize the AcpiMmioBase from the FCH data
 *          block to deassert the primary die.
 *
 * @note    For SoC that support multi-die, this function should not be used until openSIL has generated the
 *          FCHMULTIFCH_OUTPUT_BLK.
 *
 * @retval  SilPass        Function returns successfully
 * @retval  SilNotFound    Not multi die or FCH data block found
 *
 */
SIL_STATUS
SilFchGpioSlotResetControl (
  void
  )
{
  FCHMULTIFCH_OUTPUT_BLK            *MultiFchOutputBlock;
  uint64_t                          AcpiMmioBaseAddr;
  uint32_t                          Index;

  MultiFchOutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );
  if (MultiFchOutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH MultiFch Output block not found.\n");
    assert(MultiFchOutputBlock != NULL);
    return SilNotFound;
  }

  for (Index = 0; Index < MultiFchOutputBlock->ArraySize; Index++) {
    AcpiMmioBaseAddr = MultiFchOutputBlock->FchAcpiMmioBase[Index];
    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Index: %d, deassert PCIe Slot Reset, ACPI MMIO Base 0x%llx.\n",
      Index,
      AcpiMmioBaseAddr
      );
    // FCH::GPIO::GPIO_26     (GPIO26_0)
    xUSLMemWrite32((void *)(size_t)(AcpiMmioBaseAddr + GPIO_BANK0_BASE + FCH_GPIO_PCIE_RST1_L_AGPIO26), 0xc40000);
    // FCH::RMTGPIO::GPIO_266 (GPIO26_1)
    xUSLMemWrite32((void *)(size_t)(AcpiMmioBaseAddr + REMOTE_GPIO_BASE + FCH_RMTGPIO_PCIE_RST0_L_AGPIO266), 0xc40000);
  }

  return SilPass;
}
