/**  @file AmdTable.h
 *  AMD Register Table Related Functions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
#pragma once
#pragma pack (push, 1)

#include <Utils.h>
#include <SilSocLogicalId.h>
#include <AmdFeatures.h>
#include <SilPcie.h>
#include <CcxClass-api.h>

/**********************************************************************************************************************
 * variable declaration
 *
 */


/**********************************************************************************************************************
 * Module data structure
 *
 */

/**
 * @brief   These are the available types of table entries.
 *
 * @details Each type corresponds to:
 *          - a semantics for the type specific data, for example semantics for a
 *            Register value, Data value, and Mask value.
 *          - optionally, including a method for type specific matching criteria
 *          - a method for writing the desired update to the hardware.
 *
 *          All types share in common a method to match CPU Family and Model and
 *          a method to match platform feature set.
 *
 */
typedef enum {
  PciRegister,             ///< PCI registers.
  PciProfileFixup,         ///< PCI registers which depend on Profile fixups.
  PciCpuRev,               ///< PCI registers which depend on family / revision.
  MsrRegister,             ///< MSR registers.
  MsrCpuRev,               ///< MSR which depend on family / revision.
  MsrCoreRev,              ///< MSR which depend on family / Core revision.
                           ///< (This type checks CPU core revison, not SoC revison)
  MsrPlatformFeat,         ///< MSR which depend on platform feature.
  MsrCpuRevPlatformFeat,   ///< MSR which depend on family / revision & platform feature.
  SmnRegisters,            ///< SMN registers.
  SmnMultiRegisters,       ///< Multi SMN registers
  SmuIndexRegister,        ///< SMU index data registers.
  SmuIndexProfileFixup,    ///< SMU index data registers which depend on Profile fixups.
  CopyBitField,            ///< Copy bitfield from register A to register B
  TableEntryTypeMax,       ///< Not a valid entry type, use for limit checking.
  TableTerminator = 0xFFFF ///< A signature to indicate end to Jam table.
} TABLE_ENTRY_TYPE;

/*------------------------------------------------------------------------------------------*/
/*
 * Useful types and defines: Selectors, Platform Features, and type specific features.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * @brief   Select tables for the current core.
 *
 * @details This allows more efficient register table processing, by allowing
 *          cores to skip redundantly setting PCI registers, for example.  This
 *          feature is not intended to be relied on for function:  it is valid
 *          to have a single register table with all settings processed by every
 *          core; it's just slower.
 *
 */
typedef enum {
  AllCores,                   ///< Select only tables which apply to all cores.
  ComputeUnitPrimary,         ///< Select tables which apply to the primary core of a compute unit (SharedC, SharedNc).
  ComplexPrimary,             ///< Select tables which apply to the primary core of a complex
  DiePrimary,                 ///< Select tables which apply to the primary core of a DIE
  PrimaryCores,               ///< Select tables which apply to primary cores.
  BscCore,                    ///< Select tables which apply to the boot core.
  TableCoreSelectorMax        ///< Not a valid selector, use for limit checking.
} TABLE_CORE_SELECTOR;

/**
 * @brief   Possible time points at which register tables can be processed.
 */
typedef enum {
  AmdRegisterTableTpBeforeApLaunch,         ///< Cpu code just prior to launching APs.
  AmdRegisterTableTpAfterApLaunch,          ///< Cpu code just after all APs have been launched.
  AmdRegisterTableTpBeforeApLaunchSecureS3, ///< Cpu code just prior to launching APs for secure S3
  AmdRegisterTableTpAfterApLaunchSecureS3,  ///< Cpu code just after all APs have been launched for secure S3
  MaxAmdRegisterTableTps                    ///< Not a valid time point, use for limit checking.
} REGISTER_TABLE_TIME_POINT;

/**
 * @brief   Entry criteria
 */
typedef struct {
  SOC_LOGICAL_ID      SocLogicalId;     ///< SoC logical ID.
  CORE_LOGICAL_ID     CoreLogicalId;    ///< Core logical ID.
  PROFILE_FEATS       ProfileFeats;     ///< Profile features.
  PLATFORM_FEATS      PlatformFeats;    ///< Platform features.
} ENTRY_CRITERIA;

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define MAKE_MSR_DATA(Address, Data, Mask) \
          BSU32 (Address), \
          BSU64 (Data), \
          BSU64 (Mask)
#define MAKE_MSR_ENTRY(Address, Data, Mask) \
          MAKE_ENTRY_TYPE (MsrRegister), \
          MAKE_MSR_DATA(Address, Data, Mask)
#define MAKE_MSR_CORE_REV_ENTRY(Family, Revision, Address, Data, Mask) \
          MAKE_ENTRY_TYPE (MsrCoreRev), \
          MAKE_CPU_CORE_LOGICAL_ID (Family, Revision), \
          MAKE_MSR_DATA (Address, Data, Mask)
#define MAKE_MSR_CPU_REV_ENTRY(Family, Revision, Address, Data, Mask) \
          MAKE_ENTRY_TYPE (MsrCpuRev), \
          MAKE_SOC_LOGICAL_ID (Family, Revision), \
          MAKE_MSR_DATA (Address, Data, Mask)
#define MAKE_MSR_PLATFORM_FEAT_ENTRY(TypeFeats, Address, Data, Mask) \
          MAKE_ENTRY_TYPE (MsrPlatformFeat), \
          MAKE_PROFILE_FEATS (TypeFeats), \
          MAKE_MSR_DATA (Address, Data, Mask)
#define MAKE_MSR_CPU_REV_PLATFORM_FEAT_ENTRY(Family, Revision, TypeFeats, Address, Data, Mask) \
          MAKE_ENTRY_TYPE (MsrCpuRevPlatformFeat), \
          MAKE_SOC_LOGICAL_ID (Family, Revision), \
          MAKE_PROFILE_FEATS (TypeFeats), \
          MAKE_MSR_DATA (Address, Data, Mask)

/// The specific data for each table entry.
#define BSU8(u8)      \
          ((uint8_t) (u8)  & 0xFF)
#define BSU16(u16)    \
          ((uint16_t) (u16) & 0xFF), \
          (((uint16_t) (u16) >> 8) & 0xFF)
#define BSU32(u32)    \
          ((uint32_t) (u32) & 0xFF), \
          (((uint32_t) (u32) >> 8) & 0xFF), \
          (((uint32_t) (u32) >> 16) & 0xFF), \
          (((uint32_t) (u32) >> 24) & 0xFF)
#define BSU64(u64)    \
          ((uint64_t) (u64) & 0xFF), \
          (((uint64_t) (u64) >> 8) & 0xFF), \
          (((uint64_t) (u64) >> 16) & 0xFF), \
          (((uint64_t) (u64) >> 24) & 0xFF), \
          (((uint64_t) (u64) >> 32) & 0xFF), \
          (((uint64_t) (u64) >> 40) & 0xFF), \
          (((uint64_t) (u64) >> 48) & 0xFF), \
          (((uint64_t) (u64) >> 56) & 0xFF)

#define MAKE_ENTRY_TYPE(Type)                      BSU16 (Type)
#define MAKE_PROFILE_FEATS(TypeFeats)              BSU32 (TypeFeats)
#define MAKE_SOC_LOGICAL_ID(Family, Revision)      BSU32 (Family), BSU16 (Revision)
#define MAKE_CPU_CORE_LOGICAL_ID(Family, Revision) BSU32 (Family), BSU16 (Revision)
#define MAKE_TABLE_TERMINATOR BSU16 (TableTerminator)

#define NUMBER_OF_TABLE_ENTRIES(Table) ((sizeof (Table) / sizeof (Table[0])) - 1)

#define AMD_PF_X2APIC           BIT_32(4)
#define AMD_PF_APIC             BIT_32(5)
#define AMD_PF_DIS_IBRS         BIT_32(6)
#define AMD_PF_BRANCH_SAMPLING  BIT_32(8)

// Not a platform type, but treat all others as AND
#define AMD_PF_AND              BIT_32(31)

#define AMD_PF_ALL    (AMD_PF_X2APIC | AMD_PF_APIC | \
                      AMD_PF_DIS_IBRS | AMD_PF_BRANCH_SAMPLING)
// Do not include AMD_PF_AND in AMD_PF_ALL !

/**
 * @brief   SMUIO register indices
 *
 * @details These registers contain fields whose writes are block by
 *          SMUSVI0_PLANE1_LOAD.SVIBUSY.
 *
 */
#define SMUSVI0_TEL_PLANE1_INDEX  0xD823000C


/**
 * @brief Instance ID indicating to write to SMN on all dies
 */
#define SMN_ON_ALL_DIES 0xFF

/**
 * @brief   Table Entry Data for MSR Registers.
 *
 * @details Apply data to register after mask, for MSRs.
 */
typedef struct {
  uint32_t  Address;                  ///< MSR address
  uint64_t  Data;                     ///< Data to set in the MSR
  uint64_t  Mask;                     ///< Mask to be applied to the MSR. Set every bit of all updated fields.
} MSR_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for PCI Registers.
 *
 * @details Apply data to register after mask, for PCI Config registers.
 */
typedef struct {
  PCI_ADDR  Address;                  ///< Address should contain Function, Offset only.  It will apply to all CPUs
  uint32_t  Data;                     ///< Data to be written into PCI device
  uint32_t  Mask;                     ///< Mask to be used before data write. Set every bit of all updated fields.
} PCI_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for Profile Fixup Registers.
 *
 * @details If TypeFeats matches current config, apply data to register
 *          after mask for PCI Config registers.
 */
typedef struct {
  PROFILE_FEATS       TypeFeats;      ///< Profile Fixup Features.
  PCI_TYPE_ENTRY_DATA PciEntry;       ///< The PCI Register entry data.
} PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for CPU revision specific PCI Registers.
 *
 * @details Apply data to register after mask, for PCI Config registers.
 */
typedef struct {
  SOC_LOGICAL_ID  CpuRevision;        ///< Common CPU Logical ID match criteria.
  PCI_ADDR        Address;            ///< Address should contain Function, Offset only.  It will apply to all CPUs
  uint32_t        Data;               ///< Data to be written into PCI device
  uint32_t        Mask;               ///< Mask to be used before data write. Set every bit of all updated fields.
} PCI_CPU_REV_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for CPU revision (SoC revison) specific MSRs.
 *
 * @details Apply data to register after mask, for MSRs.
 */
typedef struct {
  SOC_LOGICAL_ID      CpuRevision;    ///< Common CPU Logical ID match criteria.
  MSR_TYPE_ENTRY_DATA MsrEntry;       ///< MSR entry data.
} MSR_CPU_REV_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for Core revision (Silicon revison) specific MSRs.
 *
 * @details Apply data to register after mask, for MSRs.
 */
typedef struct {
  CORE_LOGICAL_ID CoreRevision;       ///< Common CPU Core Logical ID match criteria.
  MSR_TYPE_ENTRY_DATA MsrEntry;       ///< MSR entry data.
} MSR_CORE_REV_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for MSRs that depend on Platform Features.
 *
 * @details Apply data to register after mask, for MSRs.
 */
typedef struct {
  PLATFORM_FEATS      TypeFeats;      ///< Platform feature match criteria.
  MSR_TYPE_ENTRY_DATA MsrEntry;       ///< MSR entry data.
} MSR_PLATFORM_FEAT_ENTRY_DATA;

/**
 * @brief   Table Entry Data for MSRs that depend on CPU revision & Platform
 *          Features.
 *
 * @details Apply data to register after mask, for MSRs.
 */
typedef struct {
  SOC_LOGICAL_ID      CpuRevision;    ///< Common CPU Logical ID match criteria.
  PLATFORM_FEATS      TypeFeats;      ///< Platform feature match criteria.
  MSR_TYPE_ENTRY_DATA MsrEntry;       ///< MSR entry data.
} MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA;

/**
 * @brief   Table Entry Data for SMN registers.
 *
 * @details Apply data to register after mask, for SMNs.
 */
typedef struct {
  uint32_t  InstanceId;               ///< The unique identifier of the NBIO instance associated with this socket/die
  uint32_t  RegisterIndex;            ///< The index of the register
  uint32_t  Data;                     ///< Data to be written into SMN
  uint32_t  Mask;                     ///< Mask to be used before data write. Set every bit of all updated fields.
} SMN_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for SMN Multi registers.
 *
 * @details Apply data to register after mask, for SMNs.
 */
typedef struct {
  uint32_t  InstanceId;               ///< The unique identifier of the NBIO instance associated with this socket/die
  uint32_t  RegisterIndexStart;       ///< Index Start
  uint32_t  RegisterIndexEnd;         ///< Index End
  uint32_t  Steps;                    ///< Steps
  uint32_t  Data;                     ///< Data to be written into SMN
  uint32_t  Mask;                     ///< Mask to be used before data write. Set every bit of all updated fields.
} SMN_MULTI_TYPE_ENTRY_DATA;

/**
 * @brief   Table Entry Data for SMU Index/Data D0F0xBC_xxxx_xxxx Registers.
 *
 * @details Apply data to register after mask, for PCI Config registers.
 */
typedef struct {
  uint32_t  Index;                    ///< SMU index address
  uint32_t  Data;                     ///< Data to be written into PCI device
  uint32_t  Mask;                     ///< Mask to be used before data write. Set every bit of all updated fields.
} SMU_INDEX_ENTRY_DATA;

/**
 * @brief   Table Entry Data for Profile Fixup to SMU Index/Data D0F0xBC_xxxx_xxxx
 *          Registers.
 *
 * @details If TypeFeats matches current config, apply data to register after mask
 *          for SMU Index/Data D0F0xBC_xxxx_xxxx registers.
 */
typedef struct {
  PROFILE_FEATS         TypeFeats;      ///< Profile Fixup Features.
  SMU_INDEX_ENTRY_DATA  SmuIndexEntry;  ///< The SMU Index/Data D0F0xBC_xxxx_xxxx register entry data.
} SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA;

/**
 * @brief   Bit field description
 *
 * @details Describe register type, address, MSB, LSB
 */
typedef struct {
  uint16_t  RegType;                  ///< Register type
  uint32_t  Address;                  ///< Address
  uint8_t   MSB;                      ///< Most Significant Bit
  uint8_t   LSB;                      ///< Least Significant Bit
} COPY_BIT_FIELD_DESCRIPTION;

/**
 * @brief   Table Entry Data for copying bitfield from register A to register B.
 *
 * @details Copy bitfield from register A to register B.
 */
typedef struct {
  COPY_BIT_FIELD_DESCRIPTION  Destination;  ///< Destination register descriptor
  COPY_BIT_FIELD_DESCRIPTION  Source;       ///< Source register descriptor
} COPY_BIT_FIELD_ENTRY_DATA;

/// A complete register table and table entries.

/*
 * Format of table entries :
 *
 *  uint16_t        EntryType \
 *  VariableLength  EntryData / one entry
 *  uint16_t        EntryType \
 *  VariableLength  EntryData / one entry
 *  ...                       \
 *  ...                       / more entries...
 */

/**
 * @brief   All the available entry data types.
 *
 * @details Use TABLE_ENTRY_DATA in copy bitfield entry
 *
 */
typedef union {
  PCI_TYPE_ENTRY_DATA                   PciEntry;                   ///< PCI entry.
  PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA     PciProfileFixupEntry;       ///< PCI Profile Fixup entry.
  PCI_CPU_REV_TYPE_ENTRY_DATA           PciCpuRevEntry;             ///< PCI CPU revision dependent entry.
  MSR_TYPE_ENTRY_DATA                   MsrEntry;                   ///< MSR entry.
  MSR_CPU_REV_TYPE_ENTRY_DATA           MsrCpuRevEntry;             ///< MSR Cpu Rev dependent entry.
  MSR_PLATFORM_FEAT_ENTRY_DATA          MsrPlatformFeatEntry;       ///< MSR Platform feature dependent entry.
  MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA  MsrCpuRevPlatformFeatEntry; ///< MSR Cpu Rev & Platform feature dependent
                                                                    ///< entry.
  SMU_INDEX_ENTRY_DATA                  SmuIndexEntry;              ///< SMU Index Data entry.
  SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA    ProfileFixupSmuIndexEntry;  ///< SMU index Data dependent entry.
  COPY_BIT_FIELD_ENTRY_DATA             CopyBitFieldEntry;          ///< Copy bitfield entry.
} TABLE_ENTRY_DATA;

/**
 * @brief   Register Table Entry common fields.
 *
 * @details All the various types of register table entries are subclasses
 *          of this object.
 */
typedef struct {
  uint16_t          EntryType;          ///< The type of table entry this is.
  TABLE_ENTRY_DATA  EntryData;          ///< The pointer to the first entry.
} TABLE_ENTRY_FIELDS;

/**
 * @brief   An entire register table.
 */
typedef struct {
  uint32_t      Selector;               ///< For efficiency, these cores should process this table
  const uint8_t *Table;                 ///< The table entries.
} REGISTER_TABLE;

/**
 * @brief   An entire register table at given time point.
 */
typedef struct {
  REGISTER_TABLE_TIME_POINT TimePoint;  ///< Time point
  const REGISTER_TABLE**    TableList;  ///< The table list.
} REGISTER_TABLE_AT_GIVEN_TP;

/*
 * Describe implementers for table entries.
 */

/**
 * @brief   Definition of a Table Entry Type Function.
 *
 * @details Each instance of this function type is responsible for applying
 *          the table entry instance it is designed for.
 *
 * @param Criteria      Info on the installed config for entry matching.
 * @param CurrentEntry  The type specific entry data to be implemented
 *                      (that is written).
 *
 * @retval void
 */
typedef void F_DO_TABLE_ENTRY (
  ENTRY_CRITERIA    *Criteria,
  uint8_t           **CurrentEntry
  );

/**********************************************************************************************************************
 * Function prototypes
 *
 */

SIL_STATUS
SetRegistersFromTablesAtGivenTimePoint (
  REGISTER_TABLE_AT_GIVEN_TP  *RegTableListAtGivenTP,
  REGISTER_TABLE_TIME_POINT   TimePoint,
  ENTRY_CRITERIA              *InitializedCriteria,
  CCXCLASS_DATA_BLK           *CcxConfigData
  );

/*------------------------------------------------------------------------------------------*/
/*
 * Table related function prototypes (many are instance of F_DO_TABLE_ENTRY method).
 */
/*------------------------------------------------------------------------------------------*/

void
SetPciEntry (
  ENTRY_CRITERIA         *Criteria,
  uint8_t                **Entry
  );

void
SetPciCpuRevEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetPciProfileFixupEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetMsrEntry (
  ENTRY_CRITERIA         *Criteria,
  uint8_t                **Entry
  );

void
SetMsrCpuRevEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetMsrCoreRevEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetMsrPlatformFeatEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetMsrCpuRevPlatformFeatEntry (
  ENTRY_CRITERIA          *Criteria,
  uint8_t                 **Entry
  );

void
SetSmnEntry (
  ENTRY_CRITERIA         *Criteria,
  uint8_t                **Entry
  );

void
SetSmnMultiEntry (
  ENTRY_CRITERIA         *Criteria,
  uint8_t                **Entry
  );

void
CopyBitFieldEntry (
  ENTRY_CRITERIA         *Criteria,
  uint8_t                **Entry
  );

#pragma pack (pop)
