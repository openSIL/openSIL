/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 * These definitions are private to the IP.
 */

#include <XmpCmn2Rev.h>

#pragma once

#define XMPCLASS_MAJOR_REV   0
#define XMPCLASS_MINOR_REV   1
#define XMPCLASS_HOST_INSTANCE    0
#define XMPCLASS_PRIVATE_INSTANCE    1


/**************** Section for the IP Private data *****************/
/** Private IP Data Block
 *  @details This structure defines some data elements private to the IP
 *  and stored in a block assigned from the Host buffer.
 *  This can be used for any needs of the IP.
 *  This block is private to the IP, not available to other IPs, nor the Host.
 */
typedef struct {
    uint32_t         MyPrivateVar;
    XMP_XFER_TABLE   *XmpInternalFcns;
} XMP_PRIVATE_BLK;


