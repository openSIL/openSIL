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
 * the IP revision 'A'.
 *
 * This file is NOT Host visible - meaning it is not included or
 * referenced from Host code.
 */

#include <SilCommon.h>
#include <XmpRevA-api.h>


/**************** Data items for IP: XMP Rev A ******************/


/*--------------------------------------------------------------
 *  Defaults for Rev A variables
 *  Declare the Rev Specific default data values
 *   These values are defined in the KConfig tool by
 *   the platform porting engineer.
 */
XmpDataRevA DefaultsRevA = {
        .XmpIpRev   =   XmpRevA,                 // RevA
        .RevAVarOne =   CONFIG_XMP_A_FEAT_VARINT,
        .RevAVarHex =   CONFIG_XMP_A_FEAT_VARHEX,
        .RevAVarStr =   CONFIG_XMP_A_FEAT_VARSTR
};





