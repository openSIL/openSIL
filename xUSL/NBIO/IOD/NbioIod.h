/**
 * @file  MpioIod.h
 * @brief OpenSIL MPIO initialization
 * This is the IP version file used in the IpBlkList files to define
 * the IP entry points. It should only contain info for that purpose.
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <NBIO/Nbio.h>
#include <NBIO/NbioClass-api.h>

SIL_STATUS
InitializeApiNbioIod (void);
