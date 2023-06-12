/**
 *  @file MpioInit.c
 *  @brief Primary MPIO init routine
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include "MpioLibLocal.h"
#include "MpioLib.h"
#include "MpioPcie.h"
#include <NBIO/NbioIp2Ip.h>

#define MPIOCLASS_MAJOR_REV   0
#define MPIOCLASS_MINOR_REV   1
#define MPIOCLASS_INSTANCE    0

extern MPIO_COMPLEX_DESCRIPTOR PcieComplex;

/*
 * Initialize strap list
 */
uint32_t    StrapList[8] = {0xDB, 0x12C, 0x17D, 0x1Ce, 0x21F, 0x270, 0x2C1, 0x312};

/**
 * InitializeMpioTp1
 *
 * Initialize the MPIO IP during timepoint 1 (pre-Pcie phase)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioTp1 (void)
{
  MPIOCLASS_INPUT_BLK             *SilData;
  NORTH_BRIDGE_PCIE_SIB           *NbPcieData;
  SIL_STATUS                      Status;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure (SilId_MpioClass,  0);
  assert (SilData != NULL);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "MPIO IP block is located at %x\n", SilData);
  /*
   * Get PCIe topology from platform Host Firmware
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB*)SilFindStructure (SilId_NorthBridgePcie, 0);
  assert (NbPcieData);

  Status = NbioInitializeDxio (&NbPcieData->PciePlatformConfig, SilData);

  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return Status;
}

/**
 * InitializeMpioTp2
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
InitializeMpioTp2 (void)
{
  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeMpioTp3
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
InitializeMpioTp3 (void)
{
  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * Establish MPIO input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS MpioClassSetInputBlock (void)
{
  MPIOCLASS_INPUT_BLK *MpioInput;

  MpioInput = (MPIOCLASS_INPUT_BLK *)SilCreateInfoBlock (SilId_MpioClass,
                                 sizeof(MPIOCLASS_INPUT_BLK),
                                 MPIOCLASS_INSTANCE,
                                 MPIOCLASS_MAJOR_REV,
                                 MPIOCLASS_MINOR_REV);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioSetInputBlk at: 0x%x \n", MpioInput);
  if (MpioInput == NULL) {
    return SilAborted;
  }
  // fill MPIO IP data structure with defaults
  memcpy ((void *)MpioInput, &mMpioClassDflts, sizeof(MPIOCLASS_INPUT_BLK));

  return SilPass;
}

/**--------------------------------------------------------------------
 *
 * NbioInitializeDxio
 *
 * @brief Init routine for NBIO
 *
 * @param[in]  Pcie                 PCIe_PLATFORM_CONFIG pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
SIL_STATUS
NbioInitializeDxio (
  PCIe_PLATFORM_CONFIG            *Pcie,
  MPIOCLASS_INPUT_BLK             *SilData
  )
{
  SIL_STATUS                      Status;
  MPIO_COMPLEX_DESCRIPTOR         *PcieTopologyData;
  MPIO_PRESIL_CTRL0               PreSilCtrl0;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  PcieTopologyData = (MPIO_COMPLEX_DESCRIPTOR *)&SilData->PcieTopologyData;

  /*
   * Check for user override of PCIe topology configuration.
   */

  ParseTopologyForUbm (SilData, &PcieTopologyData, Pcie);
  MpioCfgBeforeDxioInit (Pcie, PcieTopologyData);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  PcieEarlyTrainFixups (SilData, NbioIp2Ip->NbioGetHandle (Pcie), PcieTopologyData);

  /*
   * Skip DXIO for emulation if so requested
   */
  PreSilCtrl0.Value = SilData->AmdPreSilCtrl0;
  if (PreSilCtrl0.Field.SkipMpioInitializaion == 0) {
    /*
	 * At this point we know the NBIO topology and SMU services are available
	 * We can now use SMU to initialize DXIO
	 */

    Status = MpioEarlyInitV1 (SilData, Pcie, NbioIp2Ip->NbioGetHandle (Pcie), PcieTopologyData);
    if (Status != SilPass) {
      MPIO_TRACEPOINT (SIL_TRACE_ERROR, "DXIO ERROR!!\n");
      assert (false);
      return Status;
    }
  }

  MpioCfgAfterDxioInit (Pcie);
  PcieConfigureHotplugPorts (Pcie);
  MpioVisibilityControl ();

  return SilPass;
}
