/**
 * @file FchHelper.c
 * @brief FCH Initialization helper functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "FchHelper.h"
#include "Fch.h"
#include <CommonLib/Io.h>
#include <CommonLib/Mmio.h>
#include <SilPcie.h>


#define ACPI_BM_MASK (7 << 10)
#define ACPI_S3 (3 << 10)
#define ACPI_S4 (4 << 10)
#define ACPI_S5 (5 << 10)
#define FCH_IOMAP_REGCD7 0x0CD7 // PM_Data
#define FCH_PMIOA_REG60 0x60 // AcpiPm1EvtBlk #define FCH_PMIOA_REG60 0x60 // AcpiPm1EvtBlk
#define FCH_PMIOA_REG62 0x62 // AcpiPm1CntBlk #define FCH_PMIOA_REG62 0x62 // AcpiPm1CntBlk
#define ACPI_MMIO_BASE 0xFED80000ul
#define PMIO_BASE 0x300

/*----------------------------------------------------------------------------------------*/
/**
 * FchGetAcpiPmBase
 *
 *
 *
 * @param[in] AcpiPmBase  - ACPI PM Base
 *
 */
void FchGetAcpiPmBase (uint16_t *AcpiPmBase)
{
  SilFchReadPmio (FCH_PMIOA_REG60, AccessWidth16, (uint8_t*)AcpiPmBase);
}

/*----------------------------------------------------------------------------------------*/
/*
 * SilFchReadSleepType
 *
 *
 */

uint8_t SilFchReadSleepType (void)
{
  uint16_t     AcpiPmCntPort;
  uint16_t     Value16;

  SilFchReadPmio (FCH_PMIOA_REG62, AccessWidth16, (uint8_t*)&AcpiPmCntPort);

  if ((AcpiPmCntPort == 0) || (AcpiPmCntPort == 0xFFFF)) {
    return 0xFF;
  }

  Value16 = xUSLIoRead16 (AcpiPmCntPort);
  return (uint8_t) ((Value16 >> 10) & 7);
}

/*----------------------------------------------------------------------------------------*/
/*
 * SilFchWriteSleepType
 *
 *
 */
void SilFchWriteSleepType (uint16_t SleepType)
{
  uint16_t     AcpiPmCntPort;

  if (SleepType > 5) {
    return;
  }

  SilFchReadPmio (FCH_PMIOA_REG62, AccessWidth16, (uint8_t*)&AcpiPmCntPort);

  if ((AcpiPmCntPort == 0) || (AcpiPmCntPort == 0xFFFF)) {
    return;
  }

  xUSLIoReadModifyWrite16(AcpiPmCntPort, (uint16_t)~(BIT_16(12) + BIT_16(11) + BIT_16(10)), (SleepType << 10));
}



/*----------------------------------------------------------------------------------------*/
/**
 * Read PMIO
 *
 *
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] Bytecount- Access sizes
 * @param[in] Value    - Read Data Buffer
 *
 */
void SilFchReadPmio (uint8_t Address, uint8_t AccessWidth, uint8_t *Values)
{
  for (size_t index = 0; index < AccessWidth; index++, Address++, Values++) {
    *Values = xUSLMemRead8 ((void *)((uintptr_t)(ACPI_MMIO_BASE + PMIO_BASE + Address)));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Write PMIO
 *
 *
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value    - Write Data Buffer
 *
 */
void SilFchWritePmio (uint8_t Address, uint8_t AccessWidth, uint8_t *Values)
{
  for (size_t index = 0; index < AccessWidth; index++, Address++, Values++) {
    xUSLMemWrite8 ((void*)((uintptr_t)(ACPI_MMIO_BASE + PMIO_BASE + Address)), *Values);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * RwPmio - Read/Write PMIO
 *
 *
 *
 * @param[in] Address    - PMIO Offset value
 * @param[in] AccessWidth     - Access sizes
 * @param[in] AndMask    - Data And Mask 32 bits
 * @param[in] OrMask     - Data OR Mask 32 bits
 *
 */
void SilFchRwPmio(uint8_t Address, uint8_t AccessWidth, uint32_t AndMask, uint32_t OrMask)
{
  uint64_t Result = 0;

  SilFchReadPmio (Address, AccessWidth, (uint8_t*)&Result);
  Result = (Result & AndMask) | OrMask;
  SilFchWritePmio (Address, AccessWidth, (uint8_t*)&Result);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Indirect I/O Read Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
void LibFchIndirectIoRead (uint8_t AccessWidth, uint16_t IoBase, uint8_t IndexAddress, uint8_t *Value)
{
  for (int i = 0; i < AccessWidth; i++, IndexAddress++) {
    xUSLIoWrite8 (IoBase, IndexAddress);
    Value[i] = xUSLIoRead8 (IoBase + 1);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Indirect I/O Write Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
void LibFchIndirectIoWrite (uint8_t AccessWidth, uint16_t IoBase, uint8_t IndexAddress, uint8_t *Value)
{
  for (int i = 0; i < AccessWidth; i++, IndexAddress++) {
    xUSLIoWrite8 (IoBase, IndexAddress);
    xUSLIoWrite8 (IoBase + 1, Value[i]);
  }
}

/**-----------------------------------------------------------------------------
 * Name: FchGetBootMode
 *
 * @brief This function determines whether the platform is resuming from an S state
 *   using the FCH ACPI registers
 *
 * @param None
 *
 * @retval Boot mode from SB.
*/
uint32_t
FchGetBootMode (void) {
  uint16_t BootMode = 0;
  uint16_t AcpiPm1Ctl = 0;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Check the FCH WAK_STS bit in the ACPI_PM1_CTL register
  //
  // get the address PM1_CTL register address
  //appended underscopre to avoid IoRead duplicated definition in BaseIoLibIntrinsic
  //libAMDxUSL.lib(Io_Ops.obj) : error LNK2005: _IoRead16 already defined in BaseIoLibIntrinsic.lib(IoLibMsc.obj)
  SilFchReadPmio (FCH_PMIOA_REG62, AccessWidth16, (uint8_t *)&AcpiPm1Ctl);

  // get the boot mode as seen by the south bridge
  //appended underscopre to avoid IoRead duplicated definition in BaseIoLibIntrinsic
  //libAMDxUSL.lib(Io_Ops.obj) : error LNK2005: _IoRead16 already defined in BaseIoLibIntrinsic.lib(IoLibMsc.obj)
  BootMode = (xUSLIoRead16 (AcpiPm1Ctl) & ACPI_BM_MASK);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "Entry BootMode=0x%x.\n", BootMode);

  // convert the boot mode to the EFI version
  if (ACPI_S3 == BootMode) {
    return (BOOT_ON_S3_RESUME);
  }
  if (ACPI_S4 == BootMode) {
    return (BOOT_ON_S4_RESUME);
  }
  if (ACPI_S5 == BootMode) {
    return (BOOT_ON_S5_RESUME);
  }
  // S0 or unsupported Sx mode
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Exit BootMode=0x%x.\n", BOOT_WITH_FULL_CONFIGURATION);
  return (BOOT_WITH_FULL_CONFIGURATION);
}

/**-----------------------------------------------------------------------------
 * Name: FchUpdateBootMode
 *
 * @brief This function update the platform boot mode based on the information
 *   gathered from the south bridge.
 *
 * @param None
 *
 * @retval None if the reset is successful.
*/
SIL_STATUS
FchUpdateBootMode (void
  )
{
  // Get FCH Boot mode
  FchGetBootMode();

  return SilPass;
}
