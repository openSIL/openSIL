/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPhx.h
 * @brief OpenSIL MPIO specific initialization
 * This is the IP version file used in the IpBlkList files to define
 * the IP entry points. It should only contain info for that purpose.
 */

#pragma once

#include <Nbio/NbioClass-api.h>
#include "NbioPcieComplexDataPhx.h"

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define NBIO_PCIE_DATA_SIZE_PHX sizeof (NORTH_BRIDGE_PCIE_SIB) + NBIO_PCIE_DATA_LENGTH
#define NBIO_DATA_SIZE_PHX sizeof (NBIOCLASS_DATA_BLOCK) + NBIO_PCIE_DATA_SIZE_PHX

/***************************************************************************
 * Function Prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */

SIL_STATUS
NbioSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeNbioPhxTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeNbioPhxTp2 (
  void
  );

SIL_STATUS
InitializeNbioPhxTp3 (
  void
  );

SIL_STATUS
InitializeApiNbioPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
NbioBaseConfigurationPhx (
  SIL_CONTEXT           *SilContext,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  );

SIL_STATUS
NbioClassSetInputBlk (
  SIL_CONTEXT  *SilContext
  );
