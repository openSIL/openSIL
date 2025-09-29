/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioTrainingResults.c
 *  @brief Parses the results of MPIO training and updates PCIe complex status
 */

#include <xSIM.h>
#include "MpioLibLocal.h"
#include "MpioCmn2Rev.h"

/**--------------------------------------------------------------------
 *
 * MpioUpdatePortTrainingStatus
 *
 * @brief DXIO Update Port Training Status
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   SilDate             Pointer to MPIO input block data
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   MpioData            Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioUpdatePortTrainingStatus (
  SIL_CONTEXT                          *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK           *SilDataCommon,
  GNB_HANDLE                           *GnbHandle,
  MPIO_DATA                            *MpioData
  )
{
  /*
   * DXIO Firmware Structures
   */
  FW_ASK_STRUCT                 *AskEntry;
  PCIe_ENGINE_CONFIG            *Engine;
  uint32_t                      Index;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  AskEntry = MpioData->MpioAsk;
  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }
  /*
   * Sync Platform Topology, Ask, and Complex Entries
   */
  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "  Training State for Device %d Function %d is 0x%x\n",
        AskEntry->desc.link_attributes.devfuncnumber >> 3,
        AskEntry->desc.link_attributes.devfuncnumber & 0x7,
        AskEntry->status.state
        );
      Engine = MpioFindEngineForAsk(SilContext, GnbHandle, AskEntry);
      if (Engine != NULL) {
        if (AskEntry->status.state == LINK_TRAINED) {
          Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;

          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "%s Device is on Socket %d StartLane %d Endlane %d Device %d Function %d\n",
            (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS)? "Successful": "Authentication Failed",
            GnbHandle->SocketId,
            Engine->EngineData.StartLane,
            Engine->EngineData.EndLane,
            Engine->Type.Port.PortData.DeviceNumber,
            Engine->Type.Port.PortData.FunctionNumber
            );
        } else {
          if (Engine->Type.Port.PortData.PortPresent && SilDataCommon->AmdCfgExposeUnusedPciePorts != 0) {
            if (SilDataCommon->AmdCfgExposeUnusedPciePorts == 1 || Engine->Type.Port.AlwaysExpose == 1) {
              Engine->InitStatus = INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE;
            }
          }
          if (MpioXferTable->MpioIsEverTriedTraining(SilContext, Engine)) {
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "  linkTrainingState indicates FAILURE\n");
          } else {
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "  linkTrainingState indicates EMPTY_SLOT\n");
          }
        }
      }
    }
    AskEntry++;
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
