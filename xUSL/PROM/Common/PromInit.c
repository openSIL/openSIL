/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromInit.c
 *  @brief Promontory21 related functions
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <SilSocLogicalId.h>
#include <PROM/PromClass-api.h>
#include <PROM/PROM21/Prom21Init.h>
#include "PromInit.h"

#define PROMCLASS_MAJOR_REV   0
#define PROMCLASS_MINOR_REV   1
#define PROMCLASS_INSTANCE    0

/**--------------------------------------------------------------------
 * InitializePromTp1
 *
 * @brief This function initializes the PROM IP during timepoint 1 (pre-Pcie phase).
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The PROM Silicon block is initialized by calling this function
 *           in the IP block list.
 *
 * @param   SilContext     A context structure through which host firmware defined data
 *                         can be passed to openSIL. The host firmware is responsible
 *                         for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS
InitializePromTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  PROMCLASS_DATA_BLK          *SilData;
  SIL_STATUS                    Status;

  Status = SilPass;
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (PROMCLASS_DATA_BLK *)xUslFindStructure(SilContext, SilId_PromClass, 0);
  if (SilData == NULL) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PROM IP block not found\n");
    return SilNotFound;
  }

  if (!ISSOCPHXAM5) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PROM IP block supported only on AM5, skipping\n");
    return SilPass;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "PROM IP block is located at %x\n", SilData);

  Status = InitializePromontoryChipset(SilContext, SilData);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

/**--------------------------------------------------------------------
 * PromClassSetInputBlock
 *
 * @brief Establish PROM input defaults
 *
 * @details This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAborted - Something went wrong
 */
SIL_STATUS PromClassSetInputBlock (
  SIL_CONTEXT  *SilContext
  )
{
  PROMCLASS_DATA_BLK *PromConfigData;

  PromConfigData = (PROMCLASS_DATA_BLK *)SilCreateInfoBlock(SilContext,
    SilId_PromClass,
    sizeof (PROMCLASS_DATA_BLK),
    PROMCLASS_INSTANCE,
    PROMCLASS_MAJOR_REV,
    PROMCLASS_MINOR_REV
    );
  PROM_TRACEPOINT(SIL_TRACE_INFO, "PromSetInputBlk at: 0x%x \n", PromConfigData);
  if (PromConfigData == NULL) {
    return SilAborted;
  }

  // fill PROM IP data structure with defaults
  memcpy(&PromConfigData->PromInputBlk, &PromClassDflts, sizeof(PROMCLASS_INPUT_BLK));
  memcpy(&PromConfigData->PromInputBlk.Secondary,
         &PromClassDflts.PromInputBlk.Primary,
         sizeof(PROM21_DATA_BLK));
  memset(&PromConfigData->PromOutputBlk, 0, sizeof(PROMCLASS_OUTPUT_BLK));

  return SilPass;
}
