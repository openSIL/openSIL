/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  MpioPhx.c
 * @brief Define MPIO PHX specific initialization functions.
 *
 */

#include <string.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/MpioIp2Ip.h>
#include "MpioPhx.h"
#include "MpioPhxData.h"

extern const MPIOCLASS_PHX_INPUT_BLK  mMpioClassDfltsPhx;
extern MPIO_COMMON_2_REV_XFER_BLOCK MpioXferPhx;
extern MPIO_IP2IP_API  MpioPhxApi;

/**--------------------------------------------------------------------
 *
 * MpioClassSetInputBlockPhx
 *
 * @brief Establish MPIO Phx input defaults
 *
 * @details This function is called by the MpioSetInputBlkPhx function which is called by
 * the xSIM core to populate the IP's input block with its default values. Each
 * block established by the IP must have a unique header.ID and its header.size set
 * to the single block only - so as to allow the FindStruct() to traverse the block chain.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 **/
static
SIL_STATUS
MpioClassSetInputBlockPhx (
  SIL_CONTEXT  *SilContext
  )
{
  MPIOCLASS_PHX_INPUT_BLK *MpioPhxInput;

  MpioPhxInput = (MPIOCLASS_PHX_INPUT_BLK *)SilCreateInfoBlock(SilContext,
    SilId_MpioClass,
    sizeof (MPIOCLASS_PHX_INPUT_BLK),
    MPIOCLASS_PHX_INSTANCE,
    MPIOCLASS_MAJOR_REV,
    MPIOCLASS_MINOR_REV
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioSetInputBlkPhx at: 0x%x \n", MpioPhxInput);
  if (MpioPhxInput == NULL) {
    return SilAborted;
  }
  // fill MPIO IP data structure with defaults
  memcpy((void *)MpioPhxInput, &mMpioClassDfltsPhx, sizeof (MPIOCLASS_PHX_INPUT_BLK));

  return SilPass;
}

/**
 *
 * MpioSetInputBlkPhx
 *
 *  @brief Establish MPIO input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
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
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
MpioSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;

  Status = MpioClassSetInputBlockCommon(SilContext);
  assert(Status == SilPass);

  Status = MpioClassSetInputBlockPhx(SilContext);
  assert(Status == SilPass);

  return (Status);
}

/**
 * InitializeMpioPhxTp1
 *
 * @brief Initialize MPIO IP data at timepoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeMpioPhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  MPIOCLASS_COMMON_INPUT_BLK  *SilDataCommon;
  NORTH_BRIDGE_PCIE_SIB       *NbPcieData;
  SIL_STATUS                  Status;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);
  if (SilDataCommon == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, " Error : MPIO IP block is located Failed\n");
    return SilAborted;
  }

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIO IP block is located at %x\n", SilDataCommon);

  /*
   * Get PCIe topology from platform BIOS
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilContext, SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
  assert(NbPcieData);

  Status = NbioInitializeDxio(SilContext, &NbPcieData->PciePlatformConfig, SilDataCommon);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

/**
 * InitializeMpioPhxTp2
 *
 * Initialize the MPIO IP during timepoint 2 (post-Pcie phase)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioPhxTp2 (void)
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeMpioPhxTp3
 *
 * Initialize the MPIO IP during timepoint 3 (pre-OS)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioPhxTp3 (void)
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiMpioPhx
 *
 * @brief   Initialize internal and external APIs for MPIO Phx
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiMpioPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize MPIO Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_MpioClass, (void *)&MpioXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize MPIO IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_MpioClass, (void *)&MpioPhxApi);
}
