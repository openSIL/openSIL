/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file
 * @brief openSIL-Host FCH IP interface
 *
 * @details The FCH is a bridge device underwhich many of the legacy and
 * traditional components of a PC are located.
 *
 *  This file provides the structures details for the Host to configure these
 *  legacy device operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_FCH  FCH api
 *
 *   The FCH is the portion of the SoC that contain most of the IO device and
 *   support IP blocks. The FCH is itself a sollection of smaller IP
 *   blocks which are split out separately in openSIL (e.g. SATA, XHCI
 *   (USB3) ). This still leaves a 'glue logic' level in the FCH module
 *   that needs configuration.
 *
 *   See the 'Files - @ref FCHClass-api.h' section of this document for
 *   further details.
 *
 *   Blocks comprising this class are:
 *   - @subpage IP_SATA "SATA"
 *   - @subpage IP_USB3 "USB 3.0"
 *
 * @endcond
 */

/* >>>Note to Module Owner: This file contains the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP & data structure(s)
 */


#pragma once

#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchCommonCfg.h>
