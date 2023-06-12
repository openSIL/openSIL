/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 * These definitions are private to the IP.
 *
 * These are the common functions shared by all the IP revisions to set
 * various data elements or perform specific duties.
 * Design target should be to perform as much as possible in common routines.
 */

#pragma once


/**************** Section for Functions Common to all Revs *****************/

/* Prototypes for the Common Functions  */

SIL_STATUS
XmpCmnSetInputBlk (
    uint8_t     *RevData,   // not a struc since it is variant for Rev
    uint32_t    SizeRevData // size in bytes of the Rev data
    );



SIL_STATUS
XmpCmnFcnVerInfo (
    uint8_t *VerString,
    uint8_t *RevString,
    uint8_t RevStringLen
    );



SIL_STATUS
XmpCmnFcnSiInit (
    XMPCLASS_INPUT_BLK *LclInpBlk
    );
