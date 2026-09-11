/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioInit.c
 *  @brief Primary MPIO init routine
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include "MpioLibLocal.h"
#include "MpioLib.h"
#include "MpioPcie.h"
#include "MpioCmn2Rev.h"
#include <Nbio/NbioIp2Ip.h>
#include <Cxl/CxlIp2Ip.h>

#define MPIOCLASS_MAJOR_REV   0
#define MPIOCLASS_MINOR_REV   1
#define MPIOCLASS_INSTANCE    0

extern MPIO_COMPLEX_DESCRIPTOR PcieComplex;

/*
 * Initialize strap list
 */
uint32_t    StrapList[8] = {0xDB, 0x12C, 0x17D, 0x1Ce, 0x21F, 0x270, 0x2C1, 0x312};

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

  MpioInput = (MPIOCLASS_INPUT_BLK *)SilCreateInfoBlock(SilId_MpioClass,
    sizeof (MPIOCLASS_INPUT_BLK),
    MPIOCLASS_INSTANCE,
    MPIOCLASS_MAJOR_REV,
    MPIOCLASS_MINOR_REV
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioSetInputBlk at: 0x%x \n", MpioInput);
  if (MpioInput == NULL) {
    return SilAborted;
  }
  // fill MPIO IP data structure with defaults
  memcpy((void *)MpioInput, &mMpioClassDflts, sizeof (MPIOCLASS_INPUT_BLK));

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
  NBIO_IP2IP_API                  *NbioIp2Ip;
  CXL_IP2IP_API                   *CxlIp2Ip;
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilId_CxlClass, (void **)(&CxlIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " CXL API is not found.\n");
    return SilNotFound;
  }

  MpioVisibilityControl (false);

  PcieTopologyData = (MPIO_COMPLEX_DESCRIPTOR *)&SilData->PcieTopologyData;

  if (SilData->AmdCXlEarlyLinkTraining) {
    CxlIp2Ip->RemoveCxlLinksFromTopology(&PcieTopologyData, Pcie);
  }
  MpioCfgBeforeDxioInit(Pcie, PcieTopologyData);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  PcieEarlyTrainFixups(SilData, NbioIp2Ip->NbioGetHandle(Pcie), PcieTopologyData);

  /*
   * At this point we know the NBIO topology and SMU services are available
   * We can now use SMU to initialize DXIO
   */
  Status = MpioEarlyInitV1(SilData, Pcie, NbioIp2Ip->NbioGetHandle(Pcie), PcieTopologyData);
  if (Status != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "DXIO ERROR!!\n");
    assert(false);
    return Status;
  }

  MpioXferTable->MpioCfgAfterDxioInit(Pcie);
  Status = PcieConfigureHotplugPorts(Pcie);
  MpioVisibilityControl (true);

  return Status;
}
