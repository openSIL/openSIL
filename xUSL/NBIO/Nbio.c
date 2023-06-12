/**
 * @file  Nbio.c
 * @brief OpenSIL NBIO initialization
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include "Nbio.h"
#include "NbioData.h"
#include "NbioPcie.h"
#include <string.h>
#include "NbioCommon.h"
#include "NbioIoApic.h"

#define NBIOCLASS_MAJOR_REV   0
#define NBIOCLASS_MINOR_REV   1
#define NBIOCLASS_INSTANCE    0

extern NBIO_CONFIG_DATA mNbioConfigDataDflts;
extern NBIOCLASS_INPUT_BLK mNbioClassDflts;

/**
 * NbioGetPcieTopology
 * @brief Get instance of NBIO PCIe topology.
 *
 * @retval pointer to NBIO PCIe topology.
 */
void *NbioGetPcieTopology (void)
{
    return xUslFindStructure (SilId_NorthBridgePcie, 0);
}


/**
 * NbioGetDataBlock
 * @brief   Get instance of host shared NBIO Input Block.
 *
 * @param   Instance The instance of the NBIO data block to get.
 * @retval  Pointer to NBIO input block instance.
 */
void *
NbioGetDataBlock (
  uint8_t Instance
)
{
    return xUslFindStructure (SilId_NbioClass, Instance);
}

/**
 * NbioSetInputBlk
 * @brief Establish NBIO input defaults
 *
 * @retval SIL_STATUS
 */
SIL_STATUS
NbioClassSetInputBlk (void)
{
  NBIOCLASS_DATA_BLOCK *NbioInput;
  NBIOCLASS_DATA       *NbioDfltBlockData = GetNbioBlockData();

  NbioInput = (NBIOCLASS_DATA_BLOCK *)SilCreateInfoBlock (SilId_NbioClass,
                                                          sizeof(NBIOCLASS_DATA_BLOCK),
                                                          NBIOCLASS_INSTANCE,
                                                          NBIOCLASS_MAJOR_REV,
                                                          NBIOCLASS_MINOR_REV);

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "openSIL Nbio Data Block at: 0x%x \n", NbioInput);

  if (NbioInput == NULL) {
    return SilAborted;
  }

  // Initialize the NBIO config block data.
  InitNbioConfigData();

  // Fill NBIO Input Block structure with defaults
  memcpy ((void *)(&NbioInput->NbioInputBlk), &mNbioClassDflts, sizeof(NBIOCLASS_INPUT_BLK));
  // Fill NBIO Config Data structure with defaults
  memcpy ((void *)(&NbioInput->NbioConfigData), &mNbioConfigDataDflts, sizeof(NBIO_CONFIG_DATA));

  // Reassign NbioBlockData->NbioInputBlk to the newely created IP block
  NbioDfltBlockData->NbioInputBlk = &NbioInput->NbioInputBlk;
  NbioDfltBlockData->NbioConfigData = &NbioInput->NbioConfigData;

  return SilPass;
}

/**
 * @brief InitializeNbioTp1 - Initialize NBIO IP for TimePoint 1 (Better details than this is required here)
 *
 */
SIL_STATUS InitializeNbioTp1 (void)
{

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  NBIOCLASS_DATA *NbioData = GetNbioBlockData();

  if (NbioData == NULL) {
      return SilAborted;
  }

  if (NbioData->NbioInputBlk == NULL) {
      return SilAborted;
  }
  // Create and Initialize PCIe Topology Structure
  PcieConfigurationInit();

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Enabled Base Iommu Ioapic in OpenSIL \n");

  NbioBaseConfigurationGenoa();
  NbioIOMMUInit();
  NbioIoApicInit();

  NbioSetTopOfMemory();

  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}


/**
 * @brief InitializeNbioTp2 - Initialize NBIO IP in timepoint 2 (Better details than this is required here)
 *
 */
SIL_STATUS InitializeNbioTp2 (void)
{
  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * @brief InitializeNbioTp3 - Initialize NBIO IP in timepoint 3 (Better details than this is required here)
 *
 */
SIL_STATUS InitializeNbioTp3 (void)
{
  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
