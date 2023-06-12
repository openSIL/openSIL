/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file
 * XMP Class IP Common
 *
 * This file holds the platform default data values for the XMP device and the
 * IP common functions used by the IP revision specific functions.
 *
 *
 *>>> Note to Module owner:
 *    This is a template file. Please replace, or add to this file
 *    with the code appropriate for your IPClass module.
 */

#include <SilCommon.h>
#include <XmpClass-api.h>
#include <XmpClass.h>
#include <XmpCommon.h>
#include <xSIM.h>
#include <string.h>


/************** IP Data Common Defaults *****************/

/* Declare the common defaults instance */
const XMPCLASS_INPUT_BLK   XmpClassCommonDflts = {
  /* This is where you declare all the common input block vars/values
   * you want to share with the Host.
   * This becomes part of the IP API for the Host.
   */
  .MyFeat1Var       = CONFIG_XMP_CMN_FEAT1_VAR,
  .MyFeat2Var       = CONFIG_XMP_CMN_FEAT2_VAR,
  .MyVarTwo         = CONFIG_XMP_CMN_VAR_TWO,
  .MyFeatXEn        = CONFIG_XMP_CMN_FEAT_X_EN,
  .MyHexVar         = CONFIG_XMP_CMN_VARHEX

};


/************* Common functions *****************/
/* Common functions do not need to use a transfer table, they can be called
 *  directly by the IP revision specific routines.
 */

/**
 * @brief XMP common funtion - Version Info
 * @details This is a IP common function called by the IP versions to
 *     produce a version string for the caller.
 *     Don't pay too much attention to the actual function as it is a
 *     contrived example for illustration. Just note the call paths.
 *
 * @param VerString - For the purpose of this illustration, this is a
 *     pointer to a 256 byte buffer to be filled with our message, a
 *     null terminated string.
 * @param RevString - The revision specific routine will pass in the
 *     string it defines for its revision.
 * @param RevStringLen - The revision specific routine will pass in the
 *     length of its string for its revision.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
XmpCmnFcnVerInfo (
    uint8_t   *VerString,
    uint8_t   *RevString,
    uint8_t   RevStringLen )
{
  uint8_t   BaseString[]  = "The XMP IP block is ";

  // set the base part of the string
  //        Dest,       Src,     length
  memcpy (VerString, BaseString, sizeof(BaseString) );

  // Set the revision specific ending
           //        Dest,       Src,     length
  memcpy ( &(VerString[sizeof(BaseString)-1]),
            RevString,
            RevStringLen );

  return SilPass;
};


/**
 * @brief Central Si init for XMP IP
 * @details This is a IP common routine intended to perform the majority
 * of the IP silicon initialization. The general init flow is set here.
 * There may be some nuances of init that needs to be handled by
 * revision specific code, so this routine uses the internal transfer
 * table to call back to the IP Rev code for those actions.
 *
 * The order of use of common and Rev specific functions is determined
 * by the needs of the IP. There is no fixed format. This is an
 * illustration of how to accomplish such call-backs.
 *
 * @param LclInpBlk - pointer to the IP input block. Optional- depending
 *    on IP needs.This routine could also use FindStructure() to get the
 *    address.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
XmpCmnFcnSiInit(
    XMPCLASS_INPUT_BLK *LclInpBlk
    )
{
  XMP_PRIVATE_BLK *XmpWorkspace;
  SIL_STATUS      LclStatus = SilPass;

  // Some init action performed...
  assert (LclInpBlk != NULL);

  // now we call back to the Rev specific code using the internal transfer table
    // Find the transfer table
  XmpWorkspace = (XMP_PRIVATE_BLK *)SilFindStructure(
                        SilId_XmpClass,
                        XMPCLASS_PRIVATE_INSTANCE);
  if (XmpWorkspace == NULL) {
    return SilNotFound;      // Could not find the IP input block
  }
    // Call the Rev Fcn via the Common-2-Rev Transfer Table
  LclStatus = (*(XmpWorkspace->XmpInternalFcns->Init4Rev))(LclInpBlk);

  // Some clean-up init action performed...
  return LclStatus;
}


extern const XMPCLASS_INPUT_BLK   XmpClassCommonDflts;

/**--------------------------------------------------------------------
 * Establish Example (Xmp) input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values as set by the config tool. Each block
 * established by the IP must have a unique header.ID and its header.size set
 * for this single block only, even if the 'IP' declares an output or other
 * block  - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host
 *
 * The IP may define one common routine to be shared by several IP versions,
 * or may define one per version, as dictated by the needs of the IP.
 *     In this example the 'SetInputBlk' routine is common to all IP versions.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
XmpCmnSetInputBlk (
    uint8_t         *RevData,   //not a struct since it is variant for Rev
    uint32_t        SizeRevData
    )
{
  XMPCLASS_INPUT_BLK *XmpInput;
  XMP_PRIVATE_BLK *XmpWorkspace;

  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /* Create and fill the common data block */
  XmpInput = SilCreateInfoBlock (SilId_XmpClass,
                                  sizeof(XMPCLASS_INPUT_BLK),
                                  XMPCLASS_HOST_INSTANCE,
                                  XMPCLASS_MAJOR_REV,
                                  XMPCLASS_MINOR_REV);
  XUSL_TRACEPOINT (SIL_TRACE_INFO, "openSIL XMPSetInputBlk at: 0x%x \n", XmpInput);
  if (XmpInput == NULL) {
    return SilAborted;
  }

  // fill XMP structure with defaults
  memcpy ( (void *)XmpInput,                      // Dest
           (const void *)&XmpClassCommonDflts ,   // Src
           sizeof(XmpClassCommonDflts)            // Length
         );
  memcpy ( (void *)&(XmpInput->Specific), RevData, SizeRevData);

  /* Create and fill the Private data block */
  XmpWorkspace = SilCreateInfoBlock (SilId_XmpClass,
                                  sizeof(XMP_PRIVATE_BLK),
                                  XMPCLASS_PRIVATE_INSTANCE,
                                  XMPCLASS_MAJOR_REV,
                                  XMPCLASS_MINOR_REV);

  // validate the declared value of XMP_RESERVED_SPACE in XmpClass-api.h
  assert ( sizeof(XMP_PRIVATE_BLK) <= XMP_RESERVED_SPACE);

  XUSL_TRACEPOINT (SIL_TRACE_INFO, "openSIL XMPPrivateBlk at: 0x%x \n", XmpWorkspace);
  if (XmpWorkspace == NULL) {
    return SilAborted;
  }
  // Fill the XMP private vars with values.
  XmpWorkspace->MyPrivateVar = 0x1234;

  XUSL_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
