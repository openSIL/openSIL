/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file
 * This file contains the Example (Xmp) IPclass init code.
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 *
 * This file holds the revision specific data and procedures for
 * the IP revision 'B'. These are used by the IpBlkFxxMxx.c files
 * for declaring the IP entry routines for the SoC.
 *
 * This file is NOT Host visible - meaning it is not included or
 * referenced from Host code.
 */

#include <SilCommon.h>
#include <XmpClass-api.h>
#include <XmpCmn2Rev.h>
#include <xSIM.h>


/**************** Common-2-Rev Transfer Table for XMP Rev A ******************/

/* prototypes for forward references of functions used in the table */
SIL_STATUS XmpSpecialInitRevB (
    XMPCLASS_INPUT_BLK *LclInpBlk
     );


/** ------------------------ Table ------------------------------
 * @details This is the internal common-2-Rev transfer table for Rev B
 */
XMP_XFER_TABLE
XmpCommon2RevBXfer = {
  .Init4Rev = XmpSpecialInitRevB      // Fcn for Rev Special Init

  // Additional Common-2-Rev functions inserted here

};






/*********** Functions used in the Common-2-Rev transfer table *************/
/**
 * @brief Special Init for the reevision
 * @details This function is called by the IP Common code to handle any
 *  special initialization needs of the revision of the IP.
 *  **This is a contrived use case for the illustration of how to build a
 *  common-2-rev transfer table.
 */
SIL_STATUS
XmpSpecialInitRevB (
    XMPCLASS_INPUT_BLK *LclInpBlk
     )
{
  assert (LclInpBlk != NULL);
  return SilPass;
}

