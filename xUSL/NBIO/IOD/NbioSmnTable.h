/**
 * @file  NbioSmnTable.h
 * @brief This file contains the prototypes of Program NBIO Smn Table API
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <NBIO/Nbio.h>

/**
 *
 * Preporcessor defines to populate SmnTable entries
 *
 */
#define SMN_ENTRY_TABLE(Table) \
  (uint32_t) SmnTableEntry, (SMN_TABLE *) &Table

#define SMN_ENTRY_TABLE_PROPERTY (Property, Table) \
  (uint32_t) SmnTableEntryProperty, (uint32_t) Property, (SMN_TABLE *) &Table

#define SMN_ENTRY_WR(Address, Value) \
  (uint32_t) SmnEntryWr, (uint32_t) Address, (uint32_t) Value

#define SMN_ENTRY_PROPERTY_WR(Property, Address, Value) \
  (uint32_t) SmnEntryPropertyWr, (uint32_t) Property, (uint32_t) Address, (uint32_t) Value

#define SMN_ENTRY_RMW(Address, AndMask, OrMask) \
  (uint32_t) SmnEntryRmw, (uint32_t) Address, (uint32_t) AndMask, (uint32_t) OrMask

#define SMN_ENTRY_PROPERTY_RMW(Property, Address, AndMask, OrMask) \
  (uint32_t) SmnEntryPropertyRmw, (uint32_t) Property, (uint32_t) Address, (uint32_t) AndMask, (uint32_t) OrMask

#define SMN_ENTRY_TERMINATE (uint32_t) SmnEntryTerminate

typedef uint32_t SMN_TABLE;

/**
 *
 * This enum defines the type of entry present in the table.
 * This indicates the type of operation to be performed.
 *
 */
typedef enum {
  SmnEntryWr,                     ///< Write register
  SmnEntryRmw,                    ///< Read Modify Write register
  SmnEntryPropertyWr,             ///< Write register based on propery or condition
  SmnEntryPropertyRmw,            ///< Read Modify Write register based on propery or condition
  SmnTableEntry,                  ///< Table entry
  SmnTableEntryProperty,          ///< Table entry based on propery or condition
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
        const SMN_TABLE               *Table,
        uint32_t                Modifier,
        uint32_t                Property
  );
