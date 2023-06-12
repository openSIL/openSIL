/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioSmnTable.h
 * @brief This file contains the prototypes of Program NBIO Smn Table API
 *
 */

#pragma once

#include <Nbio/Common/GnbDxio.h>

#define PROPERTY_IOHC_CLKGATING_ENABLED                 0x00000001ull
#define PROPERTY_IOHC_CLKGATING_DISABLED                0x00000002ull
#define PROPERTY_TPH_COMPLETER_ENABLED                  0x00000004ull
#define PROPERTY_TPH_COMPLETER_DISABLED                 0x00000008ull
#define PROPERTY_NBIF_MGCG_CLKGATING_ENABLED            0x00000010ull
#define PROPERTY_NBIF_MGCG_CLKGATING_DISABLED           0x00000020ull
#define PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED          0x00000040ull
#define PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED         0x00000080ull
#define PROPERTY_SST_CLKGATING_ENABLED                  0x00000100ull
#define PROPERTY_SST_CLKGATING_DISABLED                 0x00000200ull
#define PROPERTY_IOMMU_DISABLED                         0x00000400ull
#define PROPERTY_IOMMU_L2CLKGATING_ENABLED              0x00000800ull
#define PROPERTY_IOMMU_L2CLKGATING_DISABLED             0x00001000ull
#define PROPERTY_IOMMU_L1CLKGATING_ENABLED              0x00002000ull
#define PROPERTY_IOMMU_L1CLKGATING_DISABLED             0x00004000ull
#define PROPERTY_XAPIC_MODE                             0x00008000ull

#define PROPERTY_PRESENT_IOHC                           0x00010000ull
#define PROPERTY_PRESENT_SYSHUB                         0x00020000ull
#define PROPERTY_PRESENT_NBIF0                          0x00040000ull
#define PROPERTY_PRESENT_NBIF1                          0x00080000ull
#define PROPERTY_PRESENT_NBIF2                          0x00100000ull
#define PROPERTY_PRESENT_SDPMUX                         0x00200000ull
#define PROPERTY_PRESENT_SST                            0x00400000ull

#define SIL_RESERVED_1754    0x00800000ull

#define PROPERTY_DEFAULT_DEVICE_PRESENCE  PROPERTY_PRESENT_IOHC | \
        PROPERTY_PRESENT_SYSHUB | \
        PROPERTY_PRESENT_NBIF0 | \
        PROPERTY_PRESENT_NBIF1 | \
        PROPERTY_PRESENT_NBIF2 | \
        PROPERTY_PRESENT_SDPMUX | \
        PROPERTY_PRESENT_SST

/**
 *
 * Preprocessor defines to populate SmnTable entries
 *
 */
typedef uint32_t SMN_TABLE;

#define SMN_ENTRY_TABLE(Table) \
        (uint32_t) SmnTableEntry, (SMN_TABLE)Table

#define SMN_ENTRY_TABLE_64(Table) \
        {(uint32_t)SmnTableEntry, (SMN_TABLE *)Table}

#define SMN_ENTRY_WR(Address, Value) \
        (uint32_t) SmnEntryWr, (uint32_t) Address, (uint32_t) Value

#define SMN_ENTRY_PROPERTY_WR(Property, Address, Value) \
        (uint32_t) SmnEntryPropertyWr, (uint32_t) Property, (uint32_t) Address, (uint32_t) Value

#define SMN_ENTRY_RMW(Address, AndMask, OrMask) \
        (uint32_t) SmnEntryRmw, (uint32_t) Address, (uint32_t) AndMask, (uint32_t) OrMask

#define SMN_ENTRY_PROPERTY_RMW(Property, Address, AndMask, OrMask) \
        (uint32_t) SmnEntryPropertyRmw, (uint32_t) Property, (uint32_t) Address, (uint32_t) AndMask, (uint32_t) OrMask

#define SMN_ENTRY_TERMINATE (uint32_t) SmnEntryTerminate

#define SMN_ENTRY_TERMINATE_TABLE \
        {(uint32_t)SmnEntryTerminate, (SMN_TABLE *)NULL}


/**
 *
 * This enum defines the type of entry present in the table.
 * This indicates the type of operation to be performed.
 *
 */
typedef enum {
  SmnEntryWr,                     ///< Write register
  SmnEntryRmw,                    ///< Read Modify Write register
  SmnEntryPropertyWr,             ///< Write register based on property or condition
  SmnEntryPropertyRmw,            ///< Read Modify Write register based on property or condition
  SmnTableEntry,                  ///< Table entry
  SmnTableEntryProperty,          ///< Table entry based on property or condition
  SmnEntryTerminate = 0xFF        ///< Terminate table
} SMN_TABLE_ENTRY_TYPE;           ///< Defines the type of the entry present in the table

/**
 *
 * Structure for SmnTable parameters based on property
 *
 */
typedef struct {
  SMN_TABLE_ENTRY_TYPE     EntryType;             ///< Structure descriptor
  SMN_TABLE                *Address;              ///< Table address
} SMN_TABLE_ENTRY;

typedef struct {
  SMN_TABLE_ENTRY_TYPE           EntryType;      ///< Structure descriptor
  uint32_t                       Property;       ///< Property
  SMN_TABLE                      *Address;       ///< Table Address
} SMN_TABLE_ENTRY_PROPERTY;

typedef struct {
  SMN_TABLE_ENTRY_TYPE  EntryType;              ///< Structure descriptor
  uint32_t              Address;                ///< Register address
  uint32_t              Value;                  ///< Value
} SMN_TABLE_ENTRY_WR;

typedef struct {
  SMN_TABLE_ENTRY_TYPE      EntryType;          ///< Structure descriptor
  uint32_t                  Property;           ///< Property
  uint32_t                  Address;            ///< Register address
  uint32_t                  Value;              ///< Value
} SMN_TABLE_ENTRY_PROPERTY_WR;

typedef struct {
  SMN_TABLE_ENTRY_TYPE          EntryType;      ///< Structure descriptor
  uint32_t                      Address;        ///< Register address
  uint32_t                      AndMask;        ///< And Mask
  uint32_t                      OrMask;         ///< Or Mask
} SMN_TABLE_ENTRY_RMW;

typedef struct {
  SMN_TABLE_ENTRY_TYPE          EntryType;      ///< Structure descriptor
  uint32_t                      Property;       ///< Property
  uint32_t                      Address;        ///< Register address
  uint32_t                      AndMask;        ///< And Mask
  uint32_t                      OrMask;         ///< Or Mask
} SMN_TABLE_ENTRY_PROPERTY_RMW;

/**
 * Declare function prototypes here
 */
SIL_STATUS
ProgramNbioSmnTable (
  GNB_HANDLE              *GnbHandle,
  const SMN_TABLE         *Table,
  uint32_t                Modifier,
  uint32_t                Property
  );
