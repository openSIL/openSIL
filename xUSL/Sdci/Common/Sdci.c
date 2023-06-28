 /**
 *  @file Sdci.c
 *  @brief Sdci configuration routines
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <NBIO/NbioIp2Ip.h>
#include <Sdci/SdciClass-api.h>
#include "Sdci.h"
#include "SdciCmn2Rev.h"

#define SDCICLASS_MAJOR_REV   0
#define SDCICLASS_MINOR_REV   1
#define SDCICLASS_INSTANCE    0

/**--------------------------------------------------------------------
 * SdciConfig
 *
 * @brief Interface to configure SDCI on each PCIe controller
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle  Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 */
static
void
SdciConfig (
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  SDCICLASS_INPUT_BLK           *SilData;
  GNB_HANDLE                    *GnbHandle;
  SDCI_COMMON_2_REV_XFER_BLOCK  *SdciXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  SDCI_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (SDCICLASS_INPUT_BLK *)SilFindStructure(SilId_SdciClass,  0);

  assert (SilData != NULL);

  /*
   * Get SDCI Cmn2Rev transfer table
   */
  if (SilGetCommon2RevXferTable (SilId_SdciClass, (void **)(&SdciXferTable)) != SilPass) {
    return;
  }

  /*
   * Enable SDCI feature if AmdFabricSdci is set to TRUE. Otherwise, just return.
   */
  if (SilData->AmdFabricSdci == false)
    return;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SDCI_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  SDCI_TRACEPOINT (SIL_TRACE_INFO, "Socket %d\n", GnbHandle->SocketId);

  /*
   * Enable TPH in PCIe root port
   */
  while (GnbHandle != NULL) {
    NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio (
                 DESCRIPTOR_ALL_WRAPPERS,
                 SdciXferTable->CmdaConfigPcieRootPortWrapper,
                 NULL,
                 GnbHandle
                 );
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  /*
   * Initialize NBIF registers
   */
  SdciXferTable->InitMpdmaEngine(Pcie);

  /*
   * Final Configuration. Initialize NBIFEPFNCFG register(s)
   */

  NbioIp2Ip->PcieConfigRunProcForAllEngines (
              DESCRIPTOR_PCIE_ENGINE,
              SdciXferTable->SdciFinalRegisterInit,
              NULL,
              Pcie
              );

  SDCI_TRACEPOINT (SIL_TRACE_EXIT, "\n");

}

/**--------------------------------------------------------------------
 * InitializeSdciTp1
 *
 * @brief This function initializes the SDCI silicon block.
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The SDCI Silicon block is initialized by calling this function
 *           in the IP block list.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS InitializeSdciTp1 (void)
{
  SDCICLASS_INPUT_BLK           *SilData;
  NORTH_BRIDGE_PCIE_SIB         *NbPcieData;
  SIL_STATUS                    Status;

  Status = SilPass;
  SDCI_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (SDCICLASS_INPUT_BLK *)xUslFindStructure (SilId_SdciClass,  0);
  if (SilData == NULL) {
    // Could not find the IP input block
    Status = SilNotFound;
    SDCI_TRACEPOINT (SIL_TRACE_INFO, "SDCI IP block not found \n");
    return Status;
  }

  SDCI_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  SDCI_TRACEPOINT (SIL_TRACE_INFO, "SDCI IP block is located at %x\n", SilData);
  /*
   * Get PCIe topology from platform Host Firmware
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB*)xUslFindStructure (SilId_NorthBridgePcie, 0);
  assert (NbPcieData);

  SdciConfig(&NbPcieData->PciePlatformConfig);

  SDCI_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return Status;
}

/**
 * There are no planned actions for SDCI at TP2.
 */

/**
 * There are no planned actions for SDCI at TP3.
 */

/**--------------------------------------------------------------------
 * SdciClassSetInputBlock
 *
 * @brief Establish SDCI input defaults
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
SIL_STATUS SdciClassSetInputBlock (void)
{
  SDCICLASS_INPUT_BLK *SdciConfigData;

  SdciConfigData = (SDCICLASS_INPUT_BLK *)SilCreateInfoBlock (SilId_SdciClass,
                                 sizeof(SDCICLASS_INPUT_BLK),
                                 SDCICLASS_INSTANCE,
                                 SDCICLASS_MAJOR_REV,
                                 SDCICLASS_MINOR_REV);
  SDCI_TRACEPOINT(SIL_TRACE_INFO, "SdciSetInputBlk at: 0x%x \n", SdciConfigData);
  if (SdciConfigData == NULL) {
    return SilAborted;
  }
  // fill SDCI IP data structure with defaults
  memcpy ((void *)SdciConfigData, &SdciClassDflts, sizeof(SDCICLASS_INPUT_BLK));

  return SilPass;
}
