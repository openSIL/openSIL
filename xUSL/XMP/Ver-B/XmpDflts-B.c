/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file
 * This file contains the Example (Xmp) IPclass init code.
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 *
 * This file holds the revision specific data variables for
 * the IP revision 'B'.
 *
 * This file is NOT Host visible - meaning it is not included or
 * referenced from Host code.
 */

#include <SilCommon.h>
#include <XmpRevB-api.h>



/**************** Data items for IP: XMP Rev A ******************/


/*--------------------------------------------------------------
 *  Defaults for Rev B variables
 *  Declare the Rev Specific default data values
 *   These values are defined in the KConfig tool by
 *   the platform porting engineer.
 */
XmpDataRevB DefaultsRevB = {
        .XmpIpRev   =   XmpRevB,                 // RevB
        .RevBVar1   =   CONFIG_XMP_B_VARINT
};





