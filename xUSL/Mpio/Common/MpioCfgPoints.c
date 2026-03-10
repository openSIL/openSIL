/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioCfgPoints.c
 *  @brief Callouts from MPIO initialization
 */

#include <string.h>
#include <xSIM.h>
#include <SilSocLogicalId.h>
#include <Nbio/NbioIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include "MpioInitLib.h"
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"

/**-------------------------------------------------------------------
 * MpioCfgBeforeReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket after ports
 * are mapped and before reconfig
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  GNB_HANDLE                    *LocalHandle;
  uint32_t                      SocketId;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgBeforeReconfigWrapper,
        NULL,
        LocalHandle
        );

    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports after reconfig and before training
 *
 * @details This function is called once for each socket
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param[in]  GnbHandle        Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  GNB_HANDLE                      *LocalHandle;
  uint32_t                        SocketId;
  AFTER_RECONFIG_BUFFER           AfterReconfigBuffer;
  MPIOCLASS_COMMON_INPUT_BLK      *SilDataCommon;
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  AfterReconfigBuffer.DpcStatusData = NULL;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);
  AfterReconfigBuffer.DpcStatusData = &SilDataCommon->DpcStatusData;

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgAfterReconfigWrapper,
        &AfterReconfigBuffer,
        LocalHandle
        );
    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

//=========================================================================================
// Timepoint before MPIO firmware begins training (after reset deasserts)
//=========================================================================================

/**----------------------------------------------------------------------------------------
 * MpioCfgEarlyLinkAuthentication
 *
 * @brief Interface to perform early link authentication
 *
 * @details This function is called once for each socket
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns   Nothing
 * @retval    Nothing
 **/
void
MpioCfgEarlyLinkAuthentication (
  GNB_HANDLE       *GnbHandle,
  PCI_ADDR         EarlyLinkAddress
  )
{
  // TODO: Add PHX AM5 chipset auth
  return;
}

/*
 *=========================================================================================
 * Timepoint before DXIO firmware initialization begins
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgBeforeDxioInit
 *
 * @brief Configuration Timepoint before DXIO firmware initialization starts
 *
 * @param   SilContext           A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param   Pcie                 Pointer silicon complex descriptor
 * @param   ComplexDescriptor    Pointer to platform complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeDxioInit (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie,
  MPIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  )
{
  MPIO_COMPLEX_DESCRIPTOR       *LocalDescriptor;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  MpioXferTable->MpioConfigurePspp(SilContext, Pcie);

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", LocalDescriptor->SocketId);
    MpioManageTopology(LocalDescriptor);
    MpioXferTable->MpioTopologyWorkarounds(SilContext, LocalDescriptor);

    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
  }
}
