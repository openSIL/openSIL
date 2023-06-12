/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioEarlyTrain.c
 *  @brief Handles early link config and training.
 */


#include <xSIM.h>
#include "MpioLib.h"
#include "MpioLibLocal.h"
#include <Nbio/NbioIp2Ip.h>
#include <SilPcie.h>

/**--------------------------------------------------------------------
 *
 * MpioProcessEarlyTrain
 *
 * @brief Routine to process early train
 *
 * @param [in]  GnbHandle              Pointer to GnbHandle pointer
 * @param [in]  PlatformTopology       Pointer to the platform BIOS supplied platform configuration
 * @param [in]  MpioData               Pointer to the platform descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioProcessEarlyTrain (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  MPIO_DATA                 *MpioData
  )
{
  uint8_t                   Index;
  FW_ASK_STRUCT             *AskEntry;
  PCIe_ENGINE_CONFIG        *Engine;
  PCI_ADDR                  EarlyLinkAddress;

  Engine = NULL;
  EarlyLinkAddress.AddressValue = 0x0;

  AskEntry = MpioData->MpioAsk;
  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      Engine = MpioFindEngineForAsk(GnbHandle, AskEntry);
      if (Engine != NULL) {
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "  Training State for Dev%d Func%d is 0x%x\n",
          AskEntry->desc.link_attributes.devfuncnumber >> 3,
          AskEntry->desc.link_attributes.devfuncnumber & 0x7,
          AskEntry->status.state
          );
        if ((Engine->Type.Port.PortData.MiscControls.SbLink == 1) && (AskEntry->status.state == LINK_TRAINED)) {
          EarlyLinkAddress.Address.Segment = Engine->Type.Port.Address.Address.Segment;
          EarlyLinkAddress.Address.Bus = Engine->Type.Port.Address.Address.Bus;
          EarlyLinkAddress.Address.Device = Engine->Type.Port.PortData.DeviceNumber;
          EarlyLinkAddress.Address.Function = Engine->Type.Port.PortData.FunctionNumber;
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "EarlyLink: Bus%d, Dev%d, Func%d\n",
            EarlyLinkAddress.Address.Bus,
            EarlyLinkAddress.Address.Device,
            EarlyLinkAddress.Address.Function
            );
          MpioCfgEarlyLinkAuthentication(GnbHandle, EarlyLinkAddress);
          break;
        }
      }
    }
    AskEntry++;
  }
}
