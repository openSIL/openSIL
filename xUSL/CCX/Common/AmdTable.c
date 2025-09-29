/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  AmdTable.c
 *  AMD Register Table Related Functions
 *
 */

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
  SetSmuIndexEntry,
  SetSmuIndexProfileFixupEntry,
  CopyBitFieldEntry
};

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

  assert(Selector < TableCoreSelectorMax);

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
 * @param   SilContext        A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
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
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       RegisterANDValue,
  uint32_t       RegisterORValue
  )
{
  uint32_t      DieLoop;
  SMU_IP2IP_API *SmuApi;
  DF_IP2IP_API  *DfApi;
  SIL_STATUS    Status;
  CCX_DATA_BLOCK *CcxDataBlk = NULL;

  CcxDataBlk = (CCX_DATA_BLOCK *) xUslFindStructure(SilContext, SilId_CcxClass, 1);
  if (CcxDataBlk == NULL) {
    // Could not find the CCX data block
    return SilNotFound;
  }
  CcxDataBlk->SystemDieNumber = 0xFF;

  Status = SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }
  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfApi);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  if (InstanceId == SMN_ON_ALL_DIES) {
    if (CcxDataBlk->SystemDieNumber == 0xFF) {
      // Get SystemDieNumber
      DfApi->DfGetSystemInfo(SilContext, NULL, &(CcxDataBlk->SystemDieNumber), NULL, NULL, NULL);
    }
    for (DieLoop = 0; DieLoop < CcxDataBlk->SystemDieNumber; DieLoop++) {
      SmuApi->SmuRegisterRMWDie(SilContext,
        DieLoop,
        RegisterIndex,
        RegisterANDValue,
        RegisterORValue
        );
    }
  } else {
    SmuApi->SmuRegisterRMWDie(SilContext,
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
 * @param   SilContext A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 * @param   Criteria   Info on the installed config for entry matching.
 * @param   Entry      The PCI register entry to perform
 *
 * retval   void
 */
void
SetPciEntry (
  SIL_CONTEXT         *SilContext,
  ENTRY_CRITERIA      *Criteria,
  uint8_t             **Entry
  )
{
  uint32_t            PciRegData;
  PCI_TYPE_ENTRY_DATA *PciEntry;

  PciEntry = (PCI_TYPE_ENTRY_DATA *) (*Entry);

  assert(PciEntry->Mask != 0);

  PciRegData = xUSLPciRead32(PciEntry->Address.AddressValue);
  PciRegData = PciRegData & (~(PciEntry->Mask));
  PciRegData = PciRegData | PciEntry->Data;
  xUSLPciWrite32(PciEntry->Address.AddressValue, PciRegData);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The Performance Profile register entry to perform
 *
 * @retval    void
 */
void
SetPciProfileFixupEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *ProfileFixupEntry;
  PCI_TYPE_ENTRY_DATA               *PciEntry;

  ProfileFixupEntry = (PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  assert((ProfileFixupEntry->TypeFeats.ProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch(Criteria->ProfileFeats.ProfileValue,
    ProfileFixupEntry->TypeFeats.ProfileValue
    )) {
    PciEntry = &ProfileFixupEntry->PciEntry;
    SetPciEntry(SilContext, Criteria, (uint8_t **) &PciEntry);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetPciCpuRevEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  PCI_CPU_REV_TYPE_ENTRY_DATA *CpuRevPciEntry;
  uint32_t                     PciRegData;

  CpuRevPciEntry = (PCI_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  assert(CpuRevPciEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevPciEntry->CpuRevision.Family) != 0) &&
    ((Criteria->SocLogicalId.Revision & CpuRevPciEntry->CpuRevision.Revision) != 0)) {
    PciRegData = xUSLPciRead32(CpuRevPciEntry->Address.AddressValue);
    PciRegData = PciRegData & (~(CpuRevPciEntry->Mask));
    PciRegData = PciRegData | CpuRevPciEntry->Data;
    xUSLPciWrite32(CpuRevPciEntry->Address.AddressValue, PciRegData);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_TYPE_ENTRY_DATA *MsrEntry;
  uint64_t            MsrVal;

  MsrEntry = (MSR_TYPE_ENTRY_DATA *) (*Entry);

  assert(MsrEntry->Mask != 0);

  MsrVal = xUslRdMsr(MsrEntry->Address);
  MsrVal &= ~(MsrEntry->Mask);
  MsrVal |= MsrEntry->Data;
  xUslWrMsr(MsrEntry->Address, MsrVal);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCpuRevEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CPU_REV_TYPE_ENTRY_DATA *CpuRevMsrEntry;
  MSR_TYPE_ENTRY_DATA         *MsrEntry;

  CpuRevMsrEntry = (MSR_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CpuRevMsrEntry->MsrEntry;
  assert(MsrEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevMsrEntry->CpuRevision.Family) != 0) &&
    ((Criteria->SocLogicalId.Revision & CpuRevMsrEntry->CpuRevision.Revision) != 0)) {
    SetMsrEntry(SilContext, Criteria, (uint8_t **) &MsrEntry);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCoreRevEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CORE_REV_TYPE_ENTRY_DATA *CoreRevMsrEntry;
  MSR_TYPE_ENTRY_DATA          *MsrEntry;

  CoreRevMsrEntry = (MSR_CORE_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CoreRevMsrEntry->MsrEntry;
  assert(MsrEntry->Mask != 0);

  if (((Criteria->CoreLogicalId.CoreFamily & CoreRevMsrEntry->CoreRevision.CoreFamily) != 0) &&
    ((Criteria->CoreLogicalId.CoreRevision & CoreRevMsrEntry->CoreRevision.CoreRevision) != 0)) {
    SetMsrEntry(SilContext, Criteria, (uint8_t **) &MsrEntry);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrPlatformFeatEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_PLATFORM_FEAT_ENTRY_DATA  *PlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA           *MsrEntry;

  PlatformFeatEntry = (MSR_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  assert((PlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch(Criteria->PlatformFeats.PlatformValue,
    PlatformFeatEntry->TypeFeats.PlatformValue
    )) {
    MsrEntry = &PlatformFeatEntry->MsrEntry;
    SetMsrEntry(SilContext, Criteria, (uint8_t **) &MsrEntry);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetMsrCpuRevPlatformFeatEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA  *CpuRevPlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA                   *MsrEntry;

  CpuRevPlatformFeatEntry = (MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  assert((CpuRevPlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  // Check CPU Rev match
  if (((Criteria->SocLogicalId.Family & CpuRevPlatformFeatEntry->CpuRevision.Family) != 0) &&
    ((Criteria->SocLogicalId.Revision & CpuRevPlatformFeatEntry->CpuRevision.Revision) != 0)) {
    // Check platform features match
    if (DoesEntryTypeSpecificInfoMatch(Criteria->PlatformFeats.PlatformValue,
      CpuRevPlatformFeatEntry->TypeFeats.PlatformValue
      )) {
      MsrEntry = &CpuRevPlatformFeatEntry->MsrEntry;
      SetMsrEntry(SilContext, Criteria, (uint8_t **) &MsrEntry);
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmnEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  SMN_TYPE_ENTRY_DATA *SmnEntry;

  if (xUslIsBsp ()) {
    SmnEntry = (SMN_TYPE_ENTRY_DATA *) (*Entry);

    assert(SmnEntry->Mask != 0);

    SetSmnTableEntry(SilContext,
      SmnEntry->InstanceId,
      SmnEntry->RegisterIndex,
      ~(SmnEntry->Mask),
      SmnEntry->Data
      );
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
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmnMultiEntry (
  SIL_CONTEXT     *SilContext,
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

    assert(SmnMultiEntry->Mask != 0);

    InstanceId = SmnMultiEntry->InstanceId;
    AndValue = ~(SmnMultiEntry->Mask);
    OrValue = SmnMultiEntry->Data;
    for (RegisterIndex = SmnMultiEntry->RegisterIndexStart;
      RegisterIndex <= SmnMultiEntry->RegisterIndexEnd;
      RegisterIndex += SmnMultiEntry->Steps) {
      SetSmnTableEntry(SilContext, InstanceId, RegisterIndex, AndValue, OrValue);
    }
  }

  // Entry MUST point to next register entry
  (*((SMN_MULTI_TYPE_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetSmuIndexEntry
 *
 * @brief     Perform the SMU Index/Data Register Entry.
 *
 * @details   Read - Modify - Write the SMU Index/Data register,
 *            clearing masked bits, and setting the data bits.
 *
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmuIndexEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  uint32_t              SmuIndexRegister;
  SMU_INDEX_ENTRY_DATA  *SmuIndexEntry;

  SmuIndexEntry = (SMU_INDEX_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  assert(SmuIndexEntry->Mask != 0);
  /*
   * Software must wait for the busy bit to clear
   * before writing any of the following fields
   */
  if ((SmuIndexEntry->Index == SMUSVI0_PLANE1_LOAD_INDEX) ||
    (SmuIndexEntry->Index == SMUIDXxD8230004) ||
    (SmuIndexEntry->Index == SMUSVI0_TEL_PLANE1_INDEX)) {
    do {
      xUSLIndirectPciRead32(SMU_INDEX_ADDRESS, SMUSVI0_PLANE1_LOAD_INDEX, &SmuIndexRegister);
    } while ((SmuIndexRegister & BIT_32(31)) != 0);
  }

  xUSLIndirectPciRead32(SMU_INDEX_ADDRESS, SmuIndexEntry->Index, &SmuIndexRegister);
  SmuIndexRegister = SmuIndexRegister & (~(SmuIndexEntry->Mask));
  SmuIndexRegister = SmuIndexRegister | SmuIndexEntry->Data;
  xUSLIndirectPciRead32(SMU_INDEX_ADDRESS, SmuIndexEntry->Index, &SmuIndexRegister);
  // Entry MUST point to next register entry
  (*((SMU_INDEX_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * SetSmuIndexProfileFixupEntry
 *
 * @brief     Perform the Performance Profile SMU Index/Data Register
 *            Entry.
 *
 * @details   Check the entry's performance profile features to the
 *            platform's and do the SMU Index/Data register entry if
 *            they match.
 *
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
SetSmuIndexProfileFixupEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  SMU_INDEX_ENTRY_DATA               *SmuIndexEntry;
  SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA *ProfileFixupSmuIndexEntry;

  ProfileFixupSmuIndexEntry = (SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  assert((ProfileFixupSmuIndexEntry->TypeFeats.ProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch(Criteria->ProfileFeats.ProfileValue,
    ProfileFixupSmuIndexEntry->TypeFeats.ProfileValue
    )) {
    SmuIndexEntry = &(ProfileFixupSmuIndexEntry->SmuIndexEntry);
    SetSmuIndexEntry(SilContext, Criteria, (uint8_t **) &SmuIndexEntry);
  }
  // Entry MUST point to next register entry
  (*((SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA **)Entry))++;
}

/**--------------------------------------------------------------------
 * CopyBitFieldEntry
 *
 * @brief     Copy bitfield from register A to register B
 *
 * @param     SilContext  A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param     Criteria    Info on the installed config for entry matching.
 * @param     Entry       The PCI register entry to perform
 *
 * @retval    void
 */
void
CopyBitFieldEntry (
  SIL_CONTEXT     *SilContext,
  ENTRY_CRITERIA  *Criteria,
  uint8_t         **Entry
  )
{
  uint8_t                     *DestEntry;
  uint64_t                    SourceData;
  uint32_t                    SourceData32;
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
    SourceData = xUslRdMsr(Source->Address);
    break;
  case PciRegister:
    PciAddress = Source->Address;
    SourceData = xUSLPciRead32(PciAddress);
    break;
  case SmuIndexRegister:
    xUSLIndirectPciRead32(SMU_INDEX_ADDRESS, Source->Address, &SourceData32);
    SourceData = SourceData32;
    break;
  default:
    assert(false);
    return;
  }

  // Check MSB, LSB, and get SourceMask, SourceData
  if (Source->MSB >= Source->LSB) {
    SourceMask = ((uint64_t)1 << (Source->MSB - Source->LSB + 1)) - 1;
    SourceData = (SourceData >> Source->LSB) & SourceMask;
  } else {
    // The input MSB LSB are wrong. exchange them
    assert(false);
    SourceMask = ((uint64_t)1 << (Source->LSB - Source->MSB + 1)) - 1;
    SourceData = (SourceData >> Source->MSB) & SourceMask;
  }

  // Check MSB, LSB, and get DestMask, DestData
  if (Destination->MSB >= Destination->LSB) {
    DestMask = ((uint64_t)1 << (Destination->MSB - Destination->LSB + 1)) - 1;
    DestMask = DestMask << Destination->LSB;
    DestData = SourceData << Destination->LSB;
  } else {
    // The input MSB LSB are wrong. exchange them
    assert(false);
    DestMask = ((uint64_t)1 << (Destination->LSB - Destination->MSB + 1)) - 1;
    DestMask = DestMask << Destination->MSB;
    DestData = SourceData << Destination->MSB;
  }
  if (SourceMask > DestMask) {
    assert(false);
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
    assert(false);
    return;
  }
  DestEntry = (uint8_t *) &DestEntryStruct;

  DoTableEntry[Destination->RegType] (SilContext, Criteria, (uint8_t **) &DestEntry);
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
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
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
  SIL_CONTEXT           *SilContext,
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
      DoTableEntry[EntryType] (SilContext, Criteria, (uint8_t **) &EntryData);
      Entries = (TABLE_ENTRY_FIELDS *) EntryData; // Get the next entry
      EntryType = Entries->EntryType;             // Get EntryType
      EntryData = &(Entries->EntryData);          // Get EntryData
    } else {
      assert(EntryType < TableEntryTypeMax);
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
  assert(Selector < TableCoreSelectorMax);

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
 * @param   SilContext        A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param   Criteria          Info on the installed config for entry
 *                            matching.
 * @param   RegisterTableList RegisterTableList
 *
 * @retval  void
 */
static
void
SetRegistersFromTableList (
  SIL_CONTEXT           *SilContext,
  ENTRY_CRITERIA        *Criteria,
  REGISTER_TABLE        **RegisterTableList
  )
{
  TABLE_CORE_SELECTOR   Selector;
  REGISTER_TABLE        **TableHandle;
  REGISTER_TABLE        **RegisterTable;

  for (Selector = AllCores; Selector < TableCoreSelectorMax; Selector++) {
    if (IsCoreSelector(Selector)) {
      // If the current core is the selected type of core, work the table list for tables for that type of core.
      TableHandle = NULL;
      RegisterTable = GetNextRegisterTable(Selector, RegisterTableList, &TableHandle);
      while (*RegisterTable != NULL) {
        SetRegistersFromTable(SilContext, Criteria, (uint8_t *) ((*RegisterTable)->Table));
        RegisterTable = GetNextRegisterTable(Selector, RegisterTableList, &TableHandle);
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
 * @param   SilContext            A context structure through which host firmware defined data
 *                                can be passed to openSIL. The host firmware is responsible
 *                                for initializing the SIL_CONTEXT structure.
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
  SIL_CONTEXT                 *SilContext,
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
    assert(TimePoint < MaxAmdRegisterTableTps);
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
      memcpy(&Criteria, InitializedCriteria, sizeof (ENTRY_CRITERIA));
    } else {
      GetPlatformFeatures(&Criteria.PlatformFeats,
        CcxConfigData->CcxInputBlock.AmdApicMode,
        CcxConfigData->CcxInputBlock.AmdIbrsEn,
        CcxConfigData->CcxInputBlock.AmdBranchSampling
        );
      GetPerformanceFeatures(&Criteria.ProfileFeats);
      GetSocLogicalIdOnCurrentCore(&Criteria.SocLogicalId);
      GetCoreLogicalIdOnCurrentCore(&Criteria.CoreLogicalId);
    }

    // Process table list
    SetRegistersFromTableList(SilContext, &Criteria, RegisterTableList);
    Status = SilPass;
  } else {
    Status = SilUnsupported;
  }
  return Status;
}
