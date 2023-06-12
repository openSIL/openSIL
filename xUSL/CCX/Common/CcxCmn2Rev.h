/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file CcxCmn2Rev.h
 * @brief Common-2-Rev core specific functions
 * @details  This files declares the structure for the common-2-Rev
 *  transfer table and the functions' prototypes therein.
 *
 */

#pragma once

/* *************** Section for the Common-2-Rev Transfer Table *****************/

/* **********  P R O T O T Y P E S
 *   for the Internal functions used in the Common-2-Rev transfer block */

/**
 * SetMiscMsrs
 *
 * @brief   IP specific function to configure miscellaneous MSRs
 *
 * @details The IP specific implementation should handle configuration of
 *          miscellaneous MSRs that are not part of the common configuration.
 */
typedef void (*SET_MISC_MSRS) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

typedef APIC_MODE (*GET_AMD_APIC_MODE) (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  );

typedef uint32_t (*CALC_LOCAL_APIC) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Ccd,
  uint32_t Complex,
  uint32_t Core,
  uint32_t Thread
  );

typedef void (*CALC_RMP_TABLEBASE) (
  uint64_t    *RmpTableSize,
  uint64_t    *RmpTableBase,
  uint64_t    *SnpMemSizeToCover,
  uint8_t     SplitRmpTableValue
  );

typedef struct {
  CcxRevisions   Version;
} CCX_INT_API_HEADER;


/* **********  T R A N S F E R   B L O C K  **********
 *   for the Internal Rev specific functions used in the Common code
 */

/** Internal Common-2-Rev Transfer Block
 *  @details This is the definition for the block of transfer structure
 *  listing functions used by the common code that must have content
 *  provided by the code specific to the revison(s) of the SoC.
 *
 *  Each revision of the IP declares an instance of this transfer block
 *  that contains pointers to functions that provide the data/action.
 *
 *  The Common code calls the specific code through these transfer blocks.
 *          Common  --> Rev Specific
 *  This table is private to the IP
 *  It is not available to other IPs, nor the Host
 */
typedef struct {
  CCX_INT_API_HEADER  Header;
  SET_MISC_MSRS       SetMiscMsrs;
  GET_AMD_APIC_MODE   GetAmdApicMode;
  CALC_LOCAL_APIC     CalcLocalApic;
  CALC_RMP_TABLEBASE  CalcRMPTableBase;
} CCX_XFER_TABLE;
