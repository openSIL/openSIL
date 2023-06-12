/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
 /**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 *
 * This file describes the IP revision entry points for use in the
 * IP Block Lists. Only the functions used in the IP block lists are
 * to be listed here so as to avoid unnecessary linkages into the heart
 * of the IP's code.
 * These definitions are private to the IP. They are not used by the Host.
 */



#pragma once

#include <XmpClass-api.h>


/**
 *  Define a structure for IP version specific 'public' variables
 *   These variables are an extension to the IP common variables and
 *   are visible to the Host.
 */
typedef struct {
    XmpRevisions    XmpIpRev;       ///< Revision of the IP for use by Host.
    uint32_t        RevBVar1;
} XmpDataRevB;

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define XMP_INPBLK_SIZE_B  ( sizeof( XMPCLASS_INPUT_BLK )   + \
                            sizeof( XmpDataRevB )           + \
                            XMP_RESERVED_SPACE  )   // IP private block


/* Function Prototypes
 * Initialize the Example (XMP) device
 *
 * In this example, the 'initializeIp' routine is specific to each IP version.
 * This file holds the prototypes for the Revision B of the device.
 * These are used in the IP Block list tables.
 */

SIL_STATUS
XmpSetInputBlockRevBTp1 ( void );


SIL_STATUS
XmpInitializeRevBTp1 ( void );

SIL_STATUS
XmpInitializeRevBTp2 ( void );

SIL_STATUS
XmpInitializeRevBTp3 ( void );

SIL_STATUS
XmpInitApiRevB ( void );

