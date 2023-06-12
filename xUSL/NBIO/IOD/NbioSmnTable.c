/**
 * @file  NbioSmnTable.c
 * @brief This file contains API definition for programming NBIO Smn Table
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <NbioSmnTable.h>

/*----------------------------------------------------------------------------------------*/
/**
 * ProgramNbioSmnTable
 *
 * @brief  This function programs(read/write/read-modify-write) registers with parameters
 *         defined in the NBIO table.
 *
 * @param[in] GnbHandle         Points to a silicon configuration structure data
 * @param[in] Table             Pointer to the Table. Table referes here an array of register addresses and its values
 *                              that are supposed to be programmed. Example: GnbIommuEnvInitTable for intializing IOMMU.
 * @param[in] Modifier          Modifier value that specifies an NBIO or PORT specific SMN aperture.
 *                              The table write function will OR this value with the address defined in the table
 *                              to generate the register address.
 *                              Therefore the table entries should specify the address of the lowest-valued address of
 *                              the register instance i.e. NBIO0, IOHC0, WRAPPER 0, PORT 0.
 *                              This will allow the logical OR of the modifier to generate the
 *                              correct address for the instance identified by the Modifier.
 * @param[in] Property          Controls the register write operation. Bit position in the propery value
 *                              indicates the register write need to be performed or bypassed
 * @retval SilPass              The Programming Nbio table is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguements are passed
 *
 */

SIL_STATUS ProgramNbioSmnTable (
        GNB_HANDLE             *GnbHandle,
        const SMN_TABLE              *Table,
        uint32_t               Modifier,
        uint32_t               Property
  )
{
  uint8_t     *EntryPointer;
  uint32_t    EntrySize;

  SMN_TABLE_ENTRY_WR              *EntryPointerWr;
  SMN_TABLE_ENTRY_RMW             *EntryPointerRmw;
  SMN_TABLE_ENTRY_PROPERTY_WR     *EntryPointerPropWr;
  SMN_TABLE_ENTRY_PROPERTY_RMW    *EntryPointerPropRmw;
  SMN_TABLE_ENTRY                 *EntryPointerTable;
  SMN_TABLE_ENTRY_PROPERTY        *EntryPointerPropTable;

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  NBIO_TRACEPOINT (SIL_TRACE_INFO, " Property - 0x%08x\n", Property);
  NBIO_TRACEPOINT (SIL_TRACE_INFO, " Modifier - 0x%08x\n", Modifier);

  EntryPointer = (uint8_t *) Table;
  EntrySize = 0;

  while (*EntryPointer != SmnEntryTerminate) {
    switch (*EntryPointer) {
    case SmnEntryWr:
      EntryPointerWr = (SMN_TABLE_ENTRY_WR*)EntryPointer;

      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        (EntryPointerWr->Address + Modifier),
        EntryPointerWr->Value
        );
      EntrySize = sizeof (SMN_TABLE_ENTRY_WR);
      break;
    case SmnEntryRmw:
      EntryPointerRmw = (SMN_TABLE_ENTRY_RMW*)EntryPointer;

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        (EntryPointerRmw->Address + Modifier),
        ~(EntryPointerRmw->AndMask),
        EntryPointerRmw->OrMask
        );
      EntrySize = sizeof (SMN_TABLE_ENTRY_RMW);
      break;
    case SmnEntryPropertyWr:
      EntryPointerPropWr = (SMN_TABLE_ENTRY_PROPERTY_WR*)EntryPointer;

      if ((Property & EntryPointerPropWr->Property) == (EntryPointerPropWr->Property)) {
        xUSLSmnWrite (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          (EntryPointerPropWr->Address + Modifier),
          EntryPointerPropWr->Value
          );
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY_WR);
      break;
    case SmnEntryPropertyRmw:
      EntryPointerPropRmw = (SMN_TABLE_ENTRY_PROPERTY_RMW*)EntryPointer;

      if ((Property & EntryPointerPropRmw->Property) == (EntryPointerPropRmw->Property)) {
        xUSLSmnReadModifyWrite (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          (EntryPointerPropRmw->Address + Modifier),
          ~(EntryPointerPropRmw->AndMask),
          EntryPointerPropRmw->OrMask
          );
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY_RMW);
      break;
    case SmnTableEntry:
      EntryPointerTable = (SMN_TABLE_ENTRY*)EntryPointer;

      ProgramNbioSmnTable (GnbHandle, EntryPointerTable->Address, Modifier, Property);
      EntrySize = sizeof (SMN_TABLE_ENTRY);
      break;
    case SmnTableEntryProperty:
      EntryPointerPropTable = (SMN_TABLE_ENTRY_PROPERTY*)EntryPointer;

      if ((Property & EntryPointerPropTable->Property) == (EntryPointerPropTable->Property)) {
          ProgramNbioSmnTable (GnbHandle, EntryPointerPropTable->Address, Modifier, Property);
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY);
      break;
    default:
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "ERROR : Invalid SMN table entry\n");
      assert (false);
      return SilInvalidParameter;
    }
    EntryPointer = EntryPointer + EntrySize;
  }

  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}
