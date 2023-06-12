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
 * the IP revision 'A'. These are used by the IpBlkFxxMxx.c files
 * for declaring the IP entry routines for the SoC.
 *
 * This file is NOT Host visible - meaning it is not included or
 * referenced from Host code.
 */

#include <SilCommon.h>
#include <XmpRevA-api.h>
#include <XmpClass.h>
#include <XmpIp2Ip.h>
#include <XmpCommon.h>


/* prototype for forward references */
// Ip2IpApi functions
SIL_STATUS GetXmpInfoRevA (
    uint8_t *Verstring
   );

/**************** Data items for IP: XMP Rev A ******************/

/*--------------------------------------------------------------
 * Public IP-2-IP Transfer block for Rev A
 */
const XMP_IP2IP_PUBLISHED_FCNS
XmpPublicApiRevA = {
  .GetVersionInfo  = GetXmpInfoRevA
  // additional published functions...
};






/*********** Functions used in the Ip-2-Ip API  *************/

/**
 * XMP Public function - Version Info
 * @details This is the public function called by another IP block.
 * It is called through the public Ip2IpApi transfer table. The foriegn
 * IP uses the IP-2-IP transfer table to call this, a rev specific
 * function, to obtain the version string of the IP.
 *
 * This function then calls an IP common function, including a ver
 * string, which sets the caller's return value.
 *     Ip2IpApi  -->  IP Rev Specific  -->  IP Common
 *
 * @param VerString For the purpose of this illustration, this is a
 * pointer to a 256 byte buffer to be filled with our message, a null
 * terminated string.
 *
 * @return SIL_STATUS
 */
SIL_STATUS GetXmpInfoRevA (
    uint8_t *VerString
    )
{
  uint8_t StringRevA[] = "Revision A";    // My revison string

  return (XmpCmnFcnVerInfo(VerString,
                          StringRevA,
                          sizeof(StringRevA)));
}



