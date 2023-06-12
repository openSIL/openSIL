/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 *
 * Each IP may supply services to the other IPs in the system. The provided
 * servcies are accessed via a standardized API table published by the IP.
 * The IP provides:
 *   - an Ip2Ipapi.h file for use by the other IPS so as to know what services
 *       are provided, along with parameters and returns for each service.
 *   - published structure containing function pointers to each service. This is
 *       communicated to the sil API handler functions.
 * The services listed in the transfer table may be implemented by the IP
 *     revision specific piece or the IP common functions piece.
 *     The transfer table provided by each IP revision MUST
 *     have the same structure.
 */


#pragma once

#include <SilCommon.h>


/* Public Transfer block */

/* First define the prototypes for each published function */

/** XMP_Info
 *  @details This public function is provided to obtain a version string
 *  for this IP.
 * This is an example and does not imply any requirements for your IP.
 */
typedef SIL_STATUS    (*XMP_VER_INFO) (
    uint8_t        *buffer      // Caller's location to place the version string
    );

/* add additional function prototypes here...*/



/* Now define the transfer block itself */
/**
 *  @details Define the transfer block for our published IP
 *  service routines for use by other IP modules.
 */
typedef struct {
    XMP_VER_INFO        GetVersionInfo;       ///< The Info function
    /* additional public functions here... */

} XMP_IP2IP_PUBLISHED_FCNS;
