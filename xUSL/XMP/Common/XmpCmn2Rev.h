/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 * These definitions are private to the IP.
 */



#pragma once


/**************** Section for the Common-2-Rev Transfer Table *****************/

/* Prototypes for Internal functions used in the Common-2-Rev transfer block */

typedef SIL_STATUS ((*InitFcn)(
      XMPCLASS_INPUT_BLK *LclInpBlk
    ));     // special init for the Rev


/** Internal Common-2-Rev Transfer Table
 *  @details This is the definition for the block of transfer structure
 *  listing functions used by the common code that must have content
 *  provided by the code specific to the revison(s) of the SoC.
 *
 *  Each revision of the IP declares an instance of this transfer block
 *  that contains pointers to functions that provide the data/action.
 *
 *  The Common code calls the specific code through these transfer tables.
 *          Common  --> Rev Specific
 *  This table is private to the IP
 *  It is not available to other IPs, nor the Host
 */
typedef struct {
    InitFcn     Init4Rev;       ///< Xfer to Rev Fcn to do special Init

    // Additional Common-2-Rev functions inserted here

} XMP_XFER_TABLE;



