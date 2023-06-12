/**
 *  @file MpioTrainingResults.c
 *  @brief Parses the results of MPIO training and updates PCIe complex status
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "MpioLibLocal.h"
#include "MpioCmn2Rev.h"

/*
 * All of the #defines enumerated below should be removed from the code.
 * Each #define below represent a section of code that is not enabled or must
 * be converted to OpenSIL conventions.  IDS functions are the majority.
 * PPI functions are also the bulk.  Some are memory allocation related too.
 */
//#define ENABLE_MPIO_PCIE_AUTH_AFTER_TRAIN_FUNC_BODY_1 // Uncomment to enable code for testing.  Currently ommited due to build issues.

/**--------------------------------------------------------------------
 *
 * MpioPcieAuthenticationAfterTraining
 *
 * @brief Mpio Pcie Authentication After Training
 *
 * @param[in]  Engine   Pointer per PCIE port Engine data
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioPcieAuthenticationAfterTraining (
  PCIe_ENGINE_CONFIG              *Engine
) {
#ifdef ENABLE_MPIO_PCIE_AUTH_AFTER_TRAIN_FUNC_BODY_1
  SIL_STATUS                      Status;
  PEI_AMD_PCIE_AUTHENTICATION_PPI *AmdPcieAuthentication = NULL;
  PORT_AUTHENTICATION_DESC        PortAuth;
  bool                            Authenticated;

  Authenticated = false;

  PortAuth.PciLocation.AsBits.Segment = Engine->Type.Port.Address.Address.Segment;
  PortAuth.PciLocation.AsBits.Bus = Engine->Type.Port.Address.Address.Bus;
  PortAuth.PciLocation.AsBits.Device = Engine->Type.Port.PortData.DeviceNumber;
  PortAuth.PciLocation.AsBits.Function = Engine->Type.Port.PortData.FunctionNumber;
  PortAuth.StartLaneIndex = Engine->EngineData.StartLane;
  PortAuth.EndLaneIndex = Engine->EngineData.EndLane;

  Status = PcieAuthenticationAfterTraining (&PortAuth, &Authenticated);
  if (Status != SilPass) {
    return;
  }
  if (Authenticated) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "PcieAuthenticationAfterTraining true\n");
  } else {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "PcieAuthenticationAfterTraining false\n");
    Engine->InitStatus = INIT_STATUS_PCIE_PORT_TRAINING_FAIL;
  }
#endif //ENABLE_MPIO_PCIE_AUTH_AFTER_TRAIN_FUNC_BODY_1
}

/**--------------------------------------------------------------------
 *
 * MpioUpdatePortTrainingStatus
 *
 * @brief DXIO Update Port Training Status
 *
 * @param[in]  SilDate    Pointer to MPIO input block data
 * @param[in]  GnbHandle  Pointer to the Silicon Descriptor for this node
 * @param[in]  MpioData   Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioUpdatePortTrainingStatus (
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                *GnbHandle,
  MPIO_DATA                 *MpioData
  )
{
  /*
   * DXIO Firmware Structures
   */
  FW_ASK_STRUCT                 *AskEntry;
  PCIe_ENGINE_CONFIG            *Engine;
  uint32_t                      Index;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  AskEntry = MpioData->MpioAsk;
  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }
  /*
   * Sync Platform Topology, Ask, and Complex Entries
   */
  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Training State for Device %d Function %d is 0x%x\n",
                       AskEntry->desc.link_attributes.devfuncnumber >> 3,
                       AskEntry->desc.link_attributes.devfuncnumber & 0x7,
                       AskEntry->status.state
                       );
      Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
      if (Engine != NULL) {
        if (AskEntry->status.state == LINK_TRAINED) {
          Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;

          /*
		   * PCIE port autentication after training
		   */
          MpioPcieAuthenticationAfterTraining (Engine);

          MPIO_TRACEPOINT (SIL_TRACE_INFO, "%s Device is on Socket %d StartLane %d Endlane %d Device %d Function %d\n",
            (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS)? L"Successful": L"Authentication Failed",
            GnbHandle->SocketId,
            Engine->EngineData.StartLane,
            Engine->EngineData.EndLane,
            Engine->Type.Port.PortData.DeviceNumber,
            Engine->Type.Port.PortData.FunctionNumber
            );
        } else {
          if (Engine->Type.Port.PortData.PortPresent && SilData->AmdCfgExposeUnusedPciePorts != 0) {
            if (SilData->AmdCfgExposeUnusedPciePorts == 1 || Engine->Type.Port.AlwaysExpose == 1) {
              Engine->InitStatus = INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE;
            }
          }
          if (MpioXferTable->MpioIsEverTriedTraining (Engine)) {
            MPIO_TRACEPOINT (SIL_TRACE_INFO, "  linkTrainingState indicates FAILURE\n");
          } else {
            MPIO_TRACEPOINT (SIL_TRACE_INFO, "  linkTrainingState indicates EMPTY_SLOT\n");
          }
        }
      }
    }
    AskEntry++;
  }

  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}
