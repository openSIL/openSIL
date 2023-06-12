/**
 * @file LegacyInterrupt.c
 * @brief Legacy Interrupt helper functions.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CommonLib/Io.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiReg.h>
#include "LegacyInterrupt.h"
#include "Fch.h"
#include "FchHelper.h"
#include <SilPcie.h>

#define MAX_NUMBER_PIRQS 8

/**-----------------------------------------------------------------------------
 * GetNumberPirqs
 *
 * @brief Get the number of PIRQs supported.
 *
 * @param NumberPirqs    Pointer to variable which would hold number
 *                       of PIRQs supported.
 *
 */
SIL_STATUS
GetNumberPirqs (
    uint8_t *NumberPirqs
  )
{
  if (NumberPirqs == NULL) {
    return SilInvalidParameter;
  }

  *NumberPirqs = MAX_NUMBER_PIRQS;
  return SilPass;
}

/**-----------------------------------------------------------------------------
 * ReadPirq
 *
 * @brief Read the given PIRQ register and return the IRQ value that is assigned to it.
 *
 * @param PirqNumber    Pirq number for which the IRQ mapping is required
 * @param PirqData      IRQ number assigned to the PIRQ index.
 *
 *
 */
SIL_STATUS
ReadPirq (
    uint8_t PirqNumber,
    uint8_t *PirqData
  )
{
  if (NULL == PirqData) {
    return SilInvalidParameter;
  }
  LibFchIndirectIoRead (AccessWidth8, FCH_IOMAP_REGC00, PirqNumber, PirqData);

  return SilPass;
}

/**-----------------------------------------------------------------------------
 * WritePirq
 *
 * @brief    Write an IRQ value to the specified PIRQ register.
 *
 * @param PirqNumber    Pirq number for which the IRQ mapping is to be updated.
 * @param PirqData      IRQ number  to be assigned to the PIRQ index.
 *
 *
 */
SIL_STATUS
WritePirq (
    uint8_t PirqNumber,
    uint8_t PirqData
  )
{
  if (PirqNumber > MAX_NUMBER_PIRQS) {
      return SilInvalidParameter;
  }
  LibFchIndirectIoWrite (AccessWidth8, FCH_IOMAP_REGC00, PirqNumber, &PirqData);

  return SilPass;
}

/**-----------------------------------------------------------------------------
 * InitializelLegacyInterrupt
 *
 * @brief    Initialize PCI IRQ routing registers for INTA#-INTH#
 *
 */

void
InitializelLegacyInterrupt (void)
{
  uint8_t Index;
  uint8_t BValue;

  //
  // Initialize PCI IRQ routing registers for INTA#-INTH#
  //
  for (Index = 0; Index < MAX_NUMBER_PIRQS; Index++) {
    BValue = Index | FCH_IRQ_IOAPIC;    // Select IRQ routing to APIC
    xUSLIoWrite8 (FCH_IOMAP_REGC00, BValue);
    BValue = Index | BIT_32(4);
    xUSLIoWrite8 (FCH_IOMAP_REGC01, BValue);
  }
}
