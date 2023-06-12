/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file
 *   > > > > EXAMPLE FILE < < < <
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 */

/*
 * >>>Note to Modue Owner: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 *
 * This file should be placed into the xSIM level for access by the Host and
 *  the xSIM core routines.
 */

#pragma once

/** XMP IP Revisions
 *  @details List the revisions of this IP device. These are used in the
 *  Data blocks to identify structural variations.
 */
typedef enum {
    XmpRevA     = 0x01,
    XmpRevB,
} XmpRevisions;

/* In order to avoid including a IP private header file, Or exposing an IP
 *   private definition to the Host, this size is defined manually, instead
 *   of using  sizeof( XMP_PRIVATE_BLK )
 * This is used in the Rev-x.h files to set the size of the memory requested
 *   by the IP in the IP block.
 * This is cross checked in XmpCommon.c
 */
#define XMP_RESERVED_SPACE  0x10



/*  Define the Example (XMP) class module's Input Block */
/**
 * @brief  Define the Example (XMP) class module's Input Block
 * @details  Define the IP device data elements. This is comprised of areas for
 *  1) shared variables to the Host, 2) local pointers 3) section for revision
 *  specific data elements.
 *
 * If the Host is not interested in a feature, then it would/should not
 * touch the var.
 */
typedef struct {
    /* Begin IP common feature variables */
    uint32_t        MyFeat1Var;     ///< These are set to default values which
    uint32_t        MyFeat2Var;     ///<  are specified by the platform via
    uint32_t        MyVarTwo;       ///<  Kconfig menus. Once set, the Host may
    uint32_t        MyFeatXEn;      ///<  then perform dynamic configuration
    uint32_t        MyHexVar;       ///<  at POST.

    /* Start of vars specific to a version of the IP. */
    uint32_t        Specific;           ///< Start of the Rev specific data
} XMPCLASS_INPUT_BLK;


