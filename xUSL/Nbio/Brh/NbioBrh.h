/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioBrh.h
 * @brief OpenSIL NBIO initialization
 * This is the IP version file used in the IpBlkList files to define
 * the IP entry points. It should only contain info for that purpose.
 */

#pragma once

#include <Nbio/NbioClass-api.h>
#include "NbioPcieComplexDataBrh.h"

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define NBIO_PCIE_DATA_SIZE_BRH sizeof (NORTH_BRIDGE_PCIE_SIB) + NBIO_PCIE_DATA_LENGTH
#define NBIO_DATA_SIZE_BRH sizeof (NBIOCLASS_DATA_BLOCK) + NBIO_PCIE_DATA_SIZE_BRH

/***************************************************************************
 * Declare Function prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */
extern
SIL_STATUS
NbioClassSetInputBlk (
  void
  );

SIL_STATUS
InitializeNbioBrhTp1 (
  void
  );

SIL_STATUS
InitializeNbioBrhTp2 (
  void
  );

SIL_STATUS
InitializeNbioBrhTp3 (
  void
  );

SIL_STATUS
InitializeApiNbioBrh (
  void
  );

SIL_STATUS
NbioBaseConfigurationBrh (
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  );

void
NbioNbifInit (
  GNB_HANDLE  *GnbHandle,
  uint32_t    Property,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  );

bool
IsCpuFamily (
  uint32_t        CpuFamily
  );


bool
IsCpuStepping (
  uint16_t        CpuStep
  );
