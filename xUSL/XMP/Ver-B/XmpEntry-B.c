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
#include <XmpRevB-api.h>
#include <XmpClass.h>
#include <XmpIp2Ip.h>
#include <XmpCommon.h>
#include <XmpCmn2Rev.h>


/* prototype for forward references */



/*********** Section for the IP Block List table functions *************/

extern XmpDataRevB DefaultsRevB;
/**
 * @brief Set the Input Block(s) for the device
 * @details This function is called by the openSIL core so that the IP can
 * establish the memory resident data block(s) it needs to operate.
 *
 * @param   none
 * @return  SIL_STATUS
 */
SIL_STATUS
XmpSetInputBlockRevBTp1 ( void )
{
  SIL_STATUS Status = SilPass;
  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // Body of XMP Init routines
  // Call the common routine, passing pointer to the RevB specific data
  Status = XmpCmnSetInputBlk ( (uint8_t *)&DefaultsRevB, sizeof(DefaultsRevB) );

  XUSL_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return Status;
};

/* Note the TP2 and TP3 version of SetInputBlock are no-ops */


/**----------------------------------------------------------------------
 * @brief Primary TP1 Initialize for the Example (XMP) device
 * @details This function initializes the XMP silicon block.
 *
 * This is an IP private function, not visible to the Host
 *
 * In this example, the 'initializeIp' routine is specific to each IP revision.
 *  There is one entry point routine per revision and it is these revision
 *  specific routines that are listed in the SoC IP Block list file(s).
 *
 * @param  none
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
XmpInitializeRevBTp1 ( void )
{
  XMPCLASS_INPUT_BLK *LclInpBlk;    // Local var to hold pointer to input blk

  SIL_STATUS Status = SilPass;
  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpBlk = (XMPCLASS_INPUT_BLK *)SilFindStructure( SilId_XmpClass,  0); //Instance 0
  XUSL_TRACEPOINT (SIL_TRACE_INFO, "openSIL XMP found blk at: 0x%x \n", LclInpBlk);
  if (LclInpBlk == NULL) {
    // Coult not find the IP input block
    return SilNotFound;
  }

  /**********************************
   * Body of XMP Init routines
   * This is where the IP code is located.
   * Use Common routines where ever possible.
   **********************************/

  // File with actions your IP needs to perform for early Si initialization

  /* For this example, the IP will use a common routine to perform the bulk of
   *   the IP Si init, but will also use a IP Rev specific call to perform
   *    some Rev B special actions at a certain point in the init sequence.
   */
  // Here is the call to the common routine...
  Status = XmpCmnFcnSiInit(LclInpBlk);


  XUSL_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return Status;
}

/** ----------------------------------------------------------------------
 * @brief Secondary TP2 Initialize for the Example (XMP) device
 * @details This function initializes the XMP silicon block after the
 * PCIe enumeration has xompleted.
 */
SIL_STATUS
XmpInitializeRevBTp2 ( void )
{
  // Fill with actions your IP needs after PCIe enumeration is completed
  return SilPass;
}

/** ----------------------------------------------------------------------
 * @brief Terciary TP3 Initialize for the Example (XMP) device
 * @details This function initializes the XMP silicon block just before
 * the hand-off to the OS.
 */
SIL_STATUS
XmpInitializeRevBTp3 ( void )
{
  // File with actions your IP needs just before hand-off to the OS
  return SilPass;
}



extern XMP_XFER_TABLE XmpCommon2RevBXfer;
extern const XMP_IP2IP_PUBLISHED_FCNS XmpPublicApiRevB;
/**----------------------------------------------------------------------
 * @brief   Set the APIs for the device
 * @details Initialize internal and external APIs for XMP RevB. The external
 * API is the Ip2IpApi and the internal is the Common-2-Rev transfer table.
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
XmpInitApiRevB ( void )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_XmpClass, &XmpCommon2RevBXfer);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize the openSIL IP-2-IP API

  return SilInitIp2IpApi (SilId_XmpClass, (void *)((uintptr_t)&XmpPublicApiRevB));
}


