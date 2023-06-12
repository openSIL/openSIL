/**
 *  @file MpioEarlyTrain.c
 *  @brief Handles early link config and training.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "MpioLib.h"
#include "MpioLibLocal.h"
#include <NBIO/NbioIp2Ip.h>
#include <SilPcie.h>

/**--------------------------------------------------------------------
 *
 * EarlyLinkConfig
 *
 * @brief tbd
 *
 * @param [in] GnbHandle              Pointer to GnbHandle pointer
 * @param [in] EarlyLinkAddress       Address for early link
 *
 * @returns True / false based on if chipset is located.
 * @retval true  - Chipset is located
 *         false - Chipset is not located
 **/
static
bool
EarlyLinkConfig (
  GNB_HANDLE               *GnbHandle,
  PCI_ADDR                 EarlyLinkAddress
  )
{
  uint32_t        Value;
  uint16_t        Value16;
  bool            ChipsetLocated;
  PCI_ADDR        NbioAddress;
  NBIO_IP2IP_API  *NbioIp2Ip;

  ChipsetLocated = false;
  Value = 0;
  Value16 = 0;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return false;
  }
  NbioAddress = NbioIp2Ip->GetHostPciAddress (GnbHandle);
  NbioAddress.AddressValue |= EarlyLinkAddress.AddressValue;
  xUSLPciRead (NbioAddress.AddressValue | 0x6A, AccessWidth16, &Value16);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Address(0x%x) = 0x%x\n", NbioAddress.AddressValue | 0x6A, Value16);
  if ((Value16 & (1 << 13)) != 0) {
    xUSLPciRMW (NbioAddress.AddressValue | 0x18, AccessWidth32, 0xFF0000FF, 0x020200);
    xUSLPciRead (MAKE_SBDFO (0, 2, 0, 0, 0), AccessWidth32, &Value);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Downstream ID = 0x%x\n", Value);
    if (Value != 0xFFFFFFFF) {
      ChipsetLocated = true;
    }
  }

  return ChipsetLocated;
}

 /**--------------------------------------------------------------------
 *
 * MpioProcessEarlyTrain
 *
 * @brief Routine to process early train
 *
 * @param [in]  GnbHandle              Pointer to GnbHandle pointer
 * @param [in]  PlatformTopology       Pointer to the platform Host Firmware supplied platform configuration
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
  // uint32_t                    MpioArg[6];
  uint8_t                   Index;
  FW_ASK_STRUCT             *AskEntry;
  PCIe_ENGINE_CONFIG        *Engine;
  PCI_ADDR                  EarlyLinkAddress;
  bool                      Allocated;

  Engine = NULL;
  Allocated = false;

  AskEntry = MpioData->MpioAsk;
  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
      if (Engine != NULL) {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Training State for Dev%d Func%d is 0x%x\n",
                        AskEntry->desc.link_attributes.devfuncnumber >> 3,
                        AskEntry->desc.link_attributes.devfuncnumber & 0x7,
                        AskEntry->status.state
                        );
        if ((Engine->Type.Port.PortData.MiscControls.SbLink == 1) && (AskEntry->status.state == LINK_TRAINED)) {
          EarlyLinkAddress.Address.Segment = Engine->Type.Port.Address.Address.Segment;
          EarlyLinkAddress.Address.Bus = Engine->Type.Port.Address.Address.Bus;
          EarlyLinkAddress.Address.Device = Engine->Type.Port.PortData.DeviceNumber;
          EarlyLinkAddress.Address.Function = Engine->Type.Port.PortData.FunctionNumber;
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "EarlyLink: Bus%d, Dev%d, Func%d\n",
                            EarlyLinkAddress.Address.Bus,
                            EarlyLinkAddress.Address.Device,
                            EarlyLinkAddress.Address.Function
                            );
          /*
           * allocate resource
           */
          Allocated = EarlyLinkConfig (GnbHandle, EarlyLinkAddress);

          if (Allocated) {
            /*
             * free resource
             */
            xUSLPciRMW (EarlyLinkAddress.AddressValue | 0x18,
              AccessWidth32, 0xFF0000FF, 0x000000);
          }
          break;
        }
      }
    }
    AskEntry++;
  }
}
