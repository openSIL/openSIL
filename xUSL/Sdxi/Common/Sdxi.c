/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Sdxi.c
 *  @brief Sdxi configuration routines
 */

#include <string.h>
#include <Nbio/NbioIp2Ip.h>
#include <Sdxi/SdxiClass-api.h>
#include "Sdxi.h"
#include "SdxiCmn2Rev.h"

#define SIL_RESERVED_1758   0
#define SIL_RESERVED_1759   1
#define SIL_RESERVED_1757    0

/**--------------------------------------------------------------------
 * SdxiConfig
 *
 * @brief Interface to configure SDXI on each PCIe controller
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle  Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 */
void
SdxiConfig (
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  SDXICLASS_INPUT_BLK           *SilData;
  GNB_HANDLE                    *GnbHandle;
  SDXI_COMMON_2_REV_XFER_BLOCK  *SdxiXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  SDXI_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (SDXICLASS_INPUT_BLK *)xUslFindStructure(SilId_SdxiClass, 0);

  assert(SilData != NULL);

  /*
   * Get SDXI Cmn2Rev transfer table
   */
  if (SilGetCommon2RevXferTable(SilId_SdxiClass, (void **)(&SdxiXferTable)) != SilPass) {
    return;
  }

  /*
   * Enable SDXI feature if PcdAmdFabricSdxi is set to TRUE. Otherwise, just return.
   */
  if (SilData->AmdFabricSdxi == false)
    return;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SDXI_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);
  SDXI_TRACEPOINT(SIL_TRACE_INFO, "Socket %d\n", GnbHandle->SocketId);

  SdxiXferTable->FinalNbifRegConfig(GnbHandle, Pcie);

  SDXI_TRACEPOINT(SIL_TRACE_EXIT, "\n");

}

/**--------------------------------------------------------------------
 * SdxiClassSetInputBlock
 *
 * @brief Establish SDXI input defaults
 *
 * @details This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAborted - Something went wrong
 */
SIL_STATUS SdxiClassSetInputBlock (void)
{
  SDXICLASS_INPUT_BLK *SdxiConfigData;

  SdxiConfigData = (SDXICLASS_INPUT_BLK *)SilCreateInfoBlock(SilId_SdxiClass,
    sizeof (SDXICLASS_INPUT_BLK),
    SIL_RESERVED_1757,
    SIL_RESERVED_1758,
    SIL_RESERVED_1759
    );
  SDXI_TRACEPOINT(SIL_TRACE_INFO, "SdxiSetInputBlk at: 0x%x \n", SdxiConfigData);
  if (SdxiConfigData == NULL) {
    return SilAborted;
  }
  // fill SDXI IP data structure with defaults
  memcpy((void *)SdxiConfigData, &SdxiClassDflts, sizeof (SDXICLASS_INPUT_BLK));

  return SilPass;
}
