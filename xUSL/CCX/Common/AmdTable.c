/**  @file
 *  AMD Register Table Related Functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include "AmdTable.h"
#include <Pci.h>
#include <AmdFeatures.h>
#include <xUslCcxRoles.h>
#include <SMU/SmuIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include "Ccx.h"

static F_DO_TABLE_ENTRY *DoTableEntry[TableEntryTypeMax] = {
  SetPciEntry,
  SetPciProfileFixupEntry,
  SetPciCpuRevEntry,
  SetMsrEntry,
  SetMsrCpuRevEntry,
  SetMsrCoreRevEntry,
  SetMsrPlatformFeatEntry,
  SetMsrCpuRevPlatformFeatEntry,
  SetSmnEntry,
  SetSmnMultiEntry,
  CopyBitFieldEntry
};

static uint32_t SystemDieNumber = 0xFF;

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**--------------------------------------------------------------------
 * GetNextRegisterTable
 *
 * @brief   An iterator for all the Family and Model Register Tables.
 *
 * @details RegisterTableHandle should be set to NULL to begin iteration,
 *          the first time the method is invoked.  Register tables can be
 *          processed, until this method returns NULL.  RegisterTableHandle
 *          should simply be passed back to the method without modification
 *          or use by the caller.  The table selector allows the relevant
 *          tables for different cores to be iterated, if the family
 *          separates tables.  For example, MSRs can be in a table
 *          processed by all cores and PCI registers in a table processed
 *          by primary cores.
 *
 * @param   Selector            Select whether to iterate over tables for
 *                              either all cores, primary cores, bsp, ....
 * @param   RegisterTableList   Register table list
 * @param   RegisterTableHandle IN: The handle of the current register
 *                              table, or NULL if Begin.
 *                              OUT: The handle of the next register table,
 *                              or NULL if End.
 *
 * @retval  The pointer to the next Register Table, or NULL if End.
 */
static
REGISTER_TABLE
**GetNextRegisterTable (
  TABLE_CORE_SELECTOR Selector,
  REGISTER_TABLE      **RegisterTableList,
  REGISTER_TABLE      ***RegisterTableHandle
  )
{
  REGISTER_TABLE      **NextTable;

  assert (Selector < TableCoreSelectorMax);

  NextTable = *RegisterTableHandle;
  if (NextTable == NULL) {
    // Begin
    NextTable = RegisterTableList;
  } else {
    NextTable++;
  }
  // Skip if not selected
  while ((*NextTable != NULL) && (((*NextTable)->Selector != (uint32_t)Selector))) {
    NextTable++;
  }
  if (*NextTable == NULL) {
    // End
    *RegisterTableHandle = NULL;
  } else {
    // Iterate next table
    *RegisterTableHandle = NextTable;
  }
  return NextTable;
}

/**--------------------------------------------------------------------
 * SetSmnTableEntry
 *
 * @brief   Write the entry values to SMN.
 *
 * @details Read/Modify/Write the SMN register, clearing masked
 *          bits, and setting the data bits.
 *
 * @param   InstanceId        Input zero based die index to modify. A
 *                            value of 0xFF means to write it to the
 *                            SMN on all dies.
 * @param   RegisterIndex     Input register address
 * @param   RegisterANDValue  Input bitwise AND value
 * @param   RegisterORValue   Input bitwise OR value
 *
 * @retval  SIL_STATUS.
 * @retval  SilPass    - everything is OK
 * @retval  SilAborted - Something went wrong
 */
static
SIL_STATUS
SetSmnTableEntry (
  uint32_t    InstanceId,
  uint32_t    RegisterIndex,
  uint32_t    RegisterANDValue,
  uint32_t    RegisterORValue
  )
{
  uint32_t      DieLoop;
  SMU_IP2IP_API *SmuApi;
  DF_IP2IP_API  *DfApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }
  SilGetIp2IpApi (SilId_DfClass, (void**) &DfApi);
  if (Status != SilPass) {
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  if (InstanceId == SMN_ON_ALL_DIES) {
    if (SystemDieNumber == 0xFF) {
      // Get SystemDieNumber
      DfApi->DfGetSystemInfo (NULL, &SystemDieNumber, NULL, NULL, NULL);
    }
    for (DieLoop = 0; DieLoop < SystemDieNumber; DieLoop++) {
      SmuApi->SmuRegisterRMW (
        DieLoop,
        RegisterIndex,
        RegisterANDValue,
        RegisterORValue
        );
    }
  } else {
    SmuApi->SmuRegisterRMW (
      InstanceId,
      RegisterIndex,
      RegisterANDValue,
      RegisterORValue
      );
  }
  return SilPass;
}

/**--------------------------------------------------------------------
 * SetPciEntry
 *
 * @brief   Perform the PCI Register Entry.
 *
 * @details Make the current core's PCI address with the function and
 *          register for the entry.  Read - Modify - Write the PCI
 *          register, clearing masked bits, and setting the data bits.
 *
 * @param   Criteria  Info on the installed config for entry matching.
 * @param   Entry     The PCI register entry to perform
 *
 * retval   void
 */
void
SetPciEntry (
  ENTRY_CRITERIA      *Criteria,
  uint8_t             **Entry
  )
{
  uint32_t            PciRegData;
  PCI_TYPE_ENTRY_DATA *PciEntry;

  PciEntry = (PCI_TYPE_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  assert (PciEntry->Mask != 0);

  PciRegData = xUSLPciRead32 (PciEntry->Address.AddressValue);
  PciRegData = PciRegData & (~(PciEntry->Mask));
  PciRegData = PciRegData | PciEntry->Data;
  xUSLPciWrite32 (PciEntry->Address.AddressValue, PciRegData);
  // Entry MUST point to next register entry
  (*((PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * DoesEntryTypeSpecificInfoMatch
 *
 * @brief     Checks register table entry type specific criteria to the
 *            platform.
 *
 * @details   Entry Data Type implementer methods can use this
 *            generically to check their own specific criteria.  The
 *            method collects the actual platform characteristics and
 *            provides them along with the table entry's criteria to
 *            this service.
 *
 *            There are a couple considerations for any implementer
 *            method using this service.  The criteria value has to be
 *            representable as a uint32_t.  The MSB, Bit 31, has to be
 *            used as an AND test request if set in the entry.  (The
 *            platform value should never have that bit set.)
 *
 * @param     PlatformTypeSpecificFeatures  The platform features
 * @param     EntryTypeFeatures             The entry's desired platform
 *                                          features
 *
 * @retval    true                          This entry should be applied
 * @retval    false                         This entry should not be
 *                                          applied
 */
static
bool
DoesEntryTypeSpecificInfoMatch (
  uint32_t  PlatformTypeSpecificFeatures,
  uint32_t  EntryTypeFeatures
  )
{
  bool      Result;

  Result = false;

  if ((EntryTypeFeatures & BIT_32(31)) == 0) {
    // Match if ANY entry feats match a platform feat (an OR test)
    if ((EntryTypeFeatures & PlatformTypeSpecificFeatures) != 0) {
      Result = true;
    }
  } else {
    // Match if ALL entry feats match a platform feat (an AND test)
    if ((EntryTypeFeatures & ~(BIT_32(31))) == (EntryTypeFeatures & PlatformTypeSpecificFeatures)) {
      Result = true;
    }
  }
  return Result;
}

/**--------------------------------------------------------------------
 * SetPciProfileFixupEntry
 *
 * @brief     Perform the Performance Profile PCI Register Entry.
 *
 * @details   Check the entry's performance profile features to the
 *            platform's and do the PCI register entry if they match.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The Performance Profile register entry to perform
 *
 * @retval    void
 */
void
SetPciProfileFixupEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *ProfileFixupEntry;
  PCI_TYPE_ENTRY_DATA               *PciEntry;

  ProfileFixupEntry = (PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  assert ((ProfileFixupEntry->TypeFeats.ProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch (Criteria->ProfileFeats.ProfileValue,
                                      ProfileFixupEntry->TypeFeats.ProfileValue)) {
    PciEntry = &ProfileFixupEntry->PciEntry;
    SetPciEntry (Criteria, (uint8_t **) &PciEntry);
  }
  // Entry MUST point to next register entry
  (*((PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetPciCpuRevEntry
 *
 * @brief     Perform the Processor Revision Dependent PCI Register
 *            Entry.
 *
 * @details   Read - Modify - Write the PCI register, clearing masked
 *            bits and setting the data bits, if the current core's
 *            revision matches the revision specified in the entry.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetPciCpuRevEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  PCI_CPU_REV_TYPE_ENTRY_DATA *CpuRevPciEntry;
  uint32_t                     PciRegData;

  CpuRevPciEntry = (PCI_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  assert (CpuRevPciEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevPciEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevPciEntry->CpuRevision.Revision) != 0)) {
    PciRegData = xUSLPciRead32 (CpuRevPciEntry->Address.AddressValue);
    PciRegData = PciRegData & (~(CpuRevPciEntry->Mask));
    PciRegData = PciRegData | CpuRevPciEntry->Data;
    xUSLPciWrite32 (CpuRevPciEntry->Address.AddressValue, PciRegData);
  }
  // Entry MUST point to next register entry
  (*((PCI_CPU_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetMsrEntry
 *
 * @brief     Perform the MSR Register Entry.
 *
 * @details   Read - Modify - Write the MSR, clearing masked bits, and
 *            setting the data bits.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_TYPE_ENTRY_DATA *MsrEntry;
  uint64_t            MsrVal;

  MsrEntry = (MSR_TYPE_ENTRY_DATA *) (*Entry);
  // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
  assert (MsrEntry->Mask != 0);

  MsrVal = xUslRdMsr (MsrEntry->Address);
  MsrVal &= ~(MsrEntry->Mask);
  MsrVal |= MsrEntry->Data;
  xUslWrMsr (MsrEntry->Address, MsrVal);
  // Entry MUST point to next register entry
  (*((MSR_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetMsrCpuRevEntry
 *
 * @brief     Perform the Processor Revision Dependent MSR Entry.
 *
 * @details   Read - Modify - Write the MSR, clearing masked bits, and
 *            setting the data bits, if the current core's revision
 *            matches the revision specified in the entry.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCpuRevEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CPU_REV_TYPE_ENTRY_DATA *CpuRevMsrEntry;
  MSR_TYPE_ENTRY_DATA         *MsrEntry;

  CpuRevMsrEntry = (MSR_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CpuRevMsrEntry->MsrEntry;
  assert (MsrEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevMsrEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevMsrEntry->CpuRevision.Revision) != 0)) {
    SetMsrEntry (Criteria, (uint8_t **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_CPU_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetMsrCoreRevEntry
 *
 * @brief     Perform the CPU silicon Revision Dependent MSR Entry.
 *
 * @details   Read - Modify - Write the MSR, clearing masked bits, and
 *            setting the data bits, if the current core's revision
 *            matches the revision specified in the entry.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCoreRevEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CORE_REV_TYPE_ENTRY_DATA *CoreRevMsrEntry;
  MSR_TYPE_ENTRY_DATA          *MsrEntry;

  CoreRevMsrEntry = (MSR_CORE_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CoreRevMsrEntry->MsrEntry;
  assert (MsrEntry->Mask != 0);

  if (((Criteria->CoreLogicalId.CoreFamily & CoreRevMsrEntry->CoreRevision.CoreFamily) != 0) &&
      ((Criteria->CoreLogicalId.CoreRevision & CoreRevMsrEntry->CoreRevision.CoreRevision) != 0)) {
    SetMsrEntry (Criteria, (uint8_t **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_CORE_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetMsrPlatformFeatEntry
 *
 * @brief     Perform the Platform Feature Dependent MSR Entry.
 *
 * @details   Read - Modify - Write the MSR, clearing masked bits, and
 *            setting the data bits, if the current core's platform
 *            features matches the features specified in the entry.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrPlatformFeatEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_PLATFORM_FEAT_ENTRY_DATA  *PlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA           *MsrEntry;

  PlatformFeatEntry = (MSR_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  assert ((PlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch (Criteria->PlatformFeats.PlatformValue,
                                      PlatformFeatEntry->TypeFeats.PlatformValue)) {
    MsrEntry = &PlatformFeatEntry->MsrEntry;
    SetMsrEntry (Criteria, (uint8_t **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_PLATFORM_FEAT_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetMsrCpuRevPlatformFeatEntry
 *
 * @brief     Perform the Cpu Rev & Platform Feature Dependent MSR Entry.
 *
 * @details   Read - Modify - Write the MSR, clearing masked bits, and
 *            setting the data bits, if the current core's revision
 *            matches the revision specified & current core's platform
 *            features matches the features specified in the entry.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCpuRevPlatformFeatEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA  *CpuRevPlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA                   *MsrEntry;

  CpuRevPlatformFeatEntry = (MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  assert ((CpuRevPlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  // Check CPU Rev match
  if (((Criteria->SocLogicalId.Family & CpuRevPlatformFeatEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevPlatformFeatEntry->CpuRevision.Revision) != 0)) {
    // Check platform features match
    if (DoesEntryTypeSpecificInfoMatch (Criteria->PlatformFeats.PlatformValue,
                                        CpuRevPlatformFeatEntry->TypeFeats.PlatformValue)) {
      MsrEntry = &CpuRevPlatformFeatEntry->MsrEntry;
      SetMsrEntry (Criteria, (uint8_t **) &MsrEntry);
    }
  }

  // Entry MUST point to next register entry
  (*((MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetSmnEntry
 *
 * @brief     Perform the SMN Register Entry.
 *
 * @details   Read - Modify - Write the SMN register, clearing masked
 *            bits, and setting the data bits.
 *
 *            See if you can use the other entries or make an entry
 *            that covers the fix.  After all, the purpose of having a
 *            table entry is to @b NOT have code which isn't generic
 *            feature code, but is family/model code specific to one
 *            case.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmnEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  SMN_TYPE_ENTRY_DATA *SmnEntry;

  if (xUslIsBsp ()) {
    SmnEntry = (SMN_TYPE_ENTRY_DATA *) (*Entry);
    // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
    assert (SmnEntry->Mask != 0);

    SetSmnTableEntry (SmnEntry->InstanceId, SmnEntry->RegisterIndex, ~(SmnEntry->Mask), SmnEntry->Data);
  }

  // Entry MUST point to next register entry
  (*((SMN_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetSmnMultiEntry
 *
 * @brief     Perform the SMN Multi Registers Entry.
 *
 * @details   Read - Modify - Write the SMN register, clearing masked
 *            bits, and setting the data bits.
 *
 *            See if you can use the other entries or make an entry
 *            that covers the fix.  After all, the purpose of having a
 *            table entry is to @b NOT have code which isn't generic
 *            feature code, but is family/model code specific to one
 *            case.
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmnMultiEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  uint32_t                  InstanceId;
  uint32_t                  RegisterIndex;
  uint32_t                  AndValue;
  uint32_t                  OrValue;
  SMN_MULTI_TYPE_ENTRY_DATA *SmnMultiEntry;

  if (xUslIsBsp ()) {
    SmnMultiEntry = (SMN_MULTI_TYPE_ENTRY_DATA *) (*Entry);
    // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
    assert (SmnMultiEntry->Mask != 0);

    InstanceId = SmnMultiEntry->InstanceId;
    AndValue = ~(SmnMultiEntry->Mask);
    OrValue = SmnMultiEntry->Data;
    for (RegisterIndex = SmnMultiEntry->RegisterIndexStart;
      RegisterIndex <= SmnMultiEntry->RegisterIndexEnd;
      RegisterIndex += SmnMultiEntry->Steps) {
        SetSmnTableEntry (InstanceId, RegisterIndex, AndValue, OrValue);
    }
  }

  // Entry MUST point to next register entry
  (*((SMN_MULTI_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * CopyBitFieldEntry
 *
 * @brief     Copy bitfield from register A to register B
 *
 * @param     Criteria  Info on the installed config for entry matching.
 * @param     Entry     The PCI register entry to perform
 *
 * @retval    void
 */
void
CopyBitFieldEntry (
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  uint8_t                     *DestEntry;
  uint64_t                    SourceData;
  uint64_t                    SourceMask;
  uint64_t                    DestData;
  uint64_t                    DestMask;
  uint32_t                    PciAddress;
  TABLE_ENTRY_DATA            DestEntryStruct;
  COPY_BIT_FIELD_DESCRIPTION  *Source;
  COPY_BIT_FIELD_DESCRIPTION  *Destination;

  Source = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Source);
  Destination = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Destination);
  // Get source data
  switch (Source->RegType) {
  case MsrRegister:
    SourceData = xUslRdMsr (Source->Address);
    break;
  case PciRegister:
    PciAddress = Source->Address;
    SourceData = xUSLPciRead32 (PciAddress);
    break;
  default:
    assert (false);
    return;
  }

  // Check MSB, LSB, and get SourceMask, SourceData
  if (Source->MSB >= Source->LSB) {
    SourceMask = (1 << (Source->MSB - Source->LSB + 1)) - 1;
    SourceData = (SourceData >> Source->LSB) & SourceMask;
  } else {
    // The input MSB LSB are wrong. exchange them
    assert (false); // Logic says these asserts are unecessary
    SourceMask = (1 << (Source->LSB - Source->MSB + 1)) - 1;
    SourceData = (SourceData >> Source->MSB) & SourceMask;
  }

  // Check MSB, LSB, and get DestMask, DestData
  if (Destination->MSB >= Destination->LSB) {
    DestMask = (1 << (Destination->MSB - Destination->LSB + 1)) - 1;
    DestMask = DestMask << Destination->LSB;
    DestData = SourceData << Destination->LSB;
  } else {
    // The input MSB LSB are wrong. exchange them
    assert (false);
    DestMask = (1 << (Destination->LSB - Destination->MSB + 1)) - 1;
    DestMask = DestMask << Destination->MSB;
    DestData = SourceData << Destination->MSB;
  }
  if (SourceMask > DestMask) {
    assert (false);
    return;
  }

  // Set destination data
  switch (Destination->RegType) {
  case MsrRegister:
    DestEntryStruct.MsrEntry.Address = Destination->Address;
    DestEntryStruct.MsrEntry.Data = DestData;
    DestEntryStruct.MsrEntry.Mask = DestMask;
    break;
  case PciRegister:
    DestEntryStruct.PciEntry.Address.AddressValue = Destination->Address;
    DestEntryStruct.PciEntry.Data = (uint32_t) DestData;
    DestEntryStruct.PciEntry.Mask = (uint32_t) DestMask;
    break;
  case SmuIndexRegister:
    DestEntryStruct.SmuIndexEntry.Index = Destination->Address;
    DestEntryStruct.SmuIndexEntry.Data = (uint32_t) DestData;
    DestEntryStruct.SmuIndexEntry.Mask = (uint32_t) DestMask;
    break;
  default:
    assert (false);
    return;
  }
  DestEntry = (uint8_t *) &DestEntryStruct;

  DoTableEntry[Destination->RegType] (Criteria, (uint8_t **) &DestEntry);
  // Entry MUST point to next register entry
  (*((COPY_BIT_FIELD_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetRegistersFromTable
 *
 * @brief   Set the registers for this core based on entries in a list
 *          of Register Tables.
 *
 * @details Determine the platform features and this core's logical id.
 *          Get the specific table entry type implementations for the
 *          logical model, which may be either generic (the ones in this
 *          file) or specific.
 *
 *          Scan the tables starting the with ones for all cores and
 *          progressively narrowing the selection based on this core's
 *          role (ex. primary core).  For a selected table, check for
 *          each entry matching the current core and platform, and call
 *          the implementer method to perform the register set operation
 *          if it matches.
 *
 * @param   Criteria      Info on the installed config for entry
 *                        matching.
 * @param   RegisterEntry RegisterEntry
 *
 * @retval  void
 *
 */
static
void
SetRegistersFromTable (
  ENTRY_CRITERIA        *Criteria,
  uint8_t               *RegisterEntry
  )
{
  TABLE_ENTRY_FIELDS    *Entries;
  TABLE_ENTRY_DATA      *EntryData;
  uint16_t              EntryType;
  /*
   * Entries Format:
   *
   * 0: uint16_t        EntryType
   *    VariableLength  EntryData
   * 1: uint16_t        EntryType
   *    VariableLength  EntryData
   * ...
   * ...
   */
  Entries = (TABLE_ENTRY_FIELDS *) RegisterEntry; // Get the first entry
  EntryType = Entries->EntryType;                 // Get EntryType
  EntryData = &(Entries->EntryData);              // Get EntryData block
  while (EntryType != TableTerminator) {
    if (EntryType < TableEntryTypeMax) {
      /*
       * EntryData will be added with correct size by DoTableEntry ()
       * After that, it points to the next entry
       */
      DoTableEntry[EntryType] (Criteria, (uint8_t **) &EntryData);
      Entries = (TABLE_ENTRY_FIELDS *) EntryData; // Get the next entry
      EntryType = Entries->EntryType;             // Get EntryType
      EntryData = &(Entries->EntryData);          // Get EntryData
    } else {
      assert (EntryType < TableEntryTypeMax);
    }
  }
}

/**--------------------------------------------------------------------
 * IsCoreSelector
 *
 * @brief   Determine this core's Selector matches.
 *
 * @param   Selector  The selector type for the table.
 *
 * @retval  true      The current core matches the selector type
 * @retval  false     The current core does not match the selector type
 */
static
bool
IsCoreSelector (
  TABLE_CORE_SELECTOR Selector
  )
{
  bool Result;

  Result = true;
  assert (Selector < TableCoreSelectorMax);

  if ((Selector == PrimaryCores) && !xUslIsSocketPrimary ()) {
    Result = false;
  }
  if ((Selector == DiePrimary) && !xUslIsDiePrimary ()) {
    Result = false;
  }
  if ((Selector == ComplexPrimary) && !xUslIsCoreComplexPrimary ()) {
    Result = false;
  }
  if ((Selector == ComputeUnitPrimary) && !xUslIsComputeUnitPrimary ()) {
    Result = false;
  }
  if ((Selector == BscCore) && (!xUslIsBsp ())) {
    Result = false;
  }
  return Result;
}

/**--------------------------------------------------------------------
 * SetRegistersFromTableList
 *
 * @brief   Set the registers for this core based on entries in a list
 *          of Register Tables.
 *
 * @details Determine the platform features and this core's logical id.
 *          Get the specific table entry type implementations for the
 *          logical model, which may be either generic (the ones in
 *          this file) or specific.
 *
 *          Scan the tables starting the with ones for all cores and
 *          progressively narrowing the selection based on this core's
 *          role (ex. primary core).  For a selected table, check for
 *          each entry matching the current core and platform, and call
 *          the implementer method to perform the register set operation
 *          if it matches.
 *
 * @param   Criteria          Info on the installed config for entry
 *                            matching.
 * @param   RegisterTableList RegisterTableList
 *
 * @retval  void
 */
static
void
SetRegistersFromTableList (
  ENTRY_CRITERIA        *Criteria,
  REGISTER_TABLE        **RegisterTableList
  )
{
  TABLE_CORE_SELECTOR   Selector;
  REGISTER_TABLE        **TableHandle;
  REGISTER_TABLE        **RegisterTable;

  for (Selector = AllCores; Selector < TableCoreSelectorMax; Selector++) {
    if (IsCoreSelector (Selector)) {
      // If the current core is the selected type of core, work the table list for tables for that type of core.
      TableHandle = NULL;
      RegisterTable = GetNextRegisterTable (Selector, RegisterTableList, &TableHandle);
      while (*RegisterTable != NULL) {
        SetRegistersFromTable (Criteria, (uint8_t *) ((*RegisterTable)->Table));
        RegisterTable = GetNextRegisterTable (Selector, RegisterTableList, &TableHandle);
      }
    } else {
      // Once a selector does not match the current core, quit looking.
      break;
    }
  }
}

/**--------------------------------------------------------------------
 * SetRegistersFromTablesAtGivenTimePoint
 *
 * @brief   Set the registers for this core based on entries in a list
 *          of Register Tables.
 *
 * @details This function acts as a wrapper for calling the
 *          SetRegistersFromTable.
 *
 * @param   RegTableListAtGivenTP Points to the register table list at
 *                                the given time point.
 * @param   TimePoint             Point in time of execution.
 * @param   InitializedCriteria   entry criteria if already exists
 * @param   CcxConfigData         Ccx input block data
 *
 * @retval  SilPass               Register table successfully loaded
 * @retval  SilUnsupported        Register table is not supported
 * @retval  SilOutOfBounds        Timepoint is out of the bounds of
 *                                supported timepoints.
 */
SIL_STATUS
SetRegistersFromTablesAtGivenTimePoint (
  REGISTER_TABLE_AT_GIVEN_TP  *RegTableListAtGivenTP,
  REGISTER_TABLE_TIME_POINT   TimePoint,
  ENTRY_CRITERIA              *InitializedCriteria,
  CCXCLASS_DATA_BLK           *CcxConfigData
  )
{
  REGISTER_TABLE              **RegisterTableList;
  ENTRY_CRITERIA              Criteria;
  SIL_STATUS                  Status;

  if (TimePoint >= MaxAmdRegisterTableTps) {
    assert (TimePoint < MaxAmdRegisterTableTps);
    return SilOutOfBounds;
  }

  RegisterTableList = NULL;
  while (RegTableListAtGivenTP->TimePoint != MaxAmdRegisterTableTps) {
    if (RegTableListAtGivenTP->TimePoint == TimePoint) {
      RegisterTableList = (REGISTER_TABLE **) RegTableListAtGivenTP->TableList;
      break;
    }
    RegTableListAtGivenTP++;
  }

  if (RegisterTableList != NULL) {
    // Prepare criteria for entry matching
    if (InitializedCriteria != NULL) {
      memcpy (&Criteria, InitializedCriteria, sizeof (ENTRY_CRITERIA));
    } else {
      GetPlatformFeatures (&Criteria.PlatformFeats,
                          CcxConfigData->CcxInputBlock.AmdApicMode,
                          CcxConfigData->CcxInputBlock.AmdIbrsEn,
                          CcxConfigData->CcxInputBlock.AmdEnvironmentFlag,
                          CcxConfigData->CcxInputBlock.AmdBranchSampling);
      GetPerformanceFeatures (&Criteria.ProfileFeats);
      GetSocLogicalIdOnCurrentCore (&Criteria.SocLogicalId);
      GetCoreLogicalIdOnCurrentCore (&Criteria.CoreLogicalId);
    }

    // Process table list
    SetRegistersFromTableList (&Criteria, RegisterTableList);
    Status = SilPass;
  } else {
    Status = SilUnsupported;
  }
  return Status;
}
