/**
 *  @file MpioDebugOut.c
 *  @brief Configure debug output for MPIO
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "MpioInitLib.h"
#include "MpioLibLocal.h"

/**--------------------------------------------------------------------
 *
 * DxioEntryConfigDump
 *
 * @brief Helper function to dump input configuration to Platform Topology
 *
 * @details Dumps information based on the input "ask" type.
 *
 * @param [in] AskEntry FW_ASK_STRUCT pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
DxioEntryConfigDump (
  FW_ASK_STRUCT    *AskEntry
  )
{
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "\n");

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Engine Type - %s\n",
    (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) ? L"PCIe Port" : (
    (AskEntry->desc.ctrlType == ASK_TYPE_SATA) ? L"SATA Port" : (
    (AskEntry->desc.ctrlType == ASK_TYPE_USB) ? L"USB Port" : L"Invalid"))
    );
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
    AskEntry->desc.startLaneIndex,
    (AskEntry->desc.startLaneIndex + AskEntry->desc.numberOfLanes - 1)
  );
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "    reversed - %d\n", AskEntry->desc.reversed);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "    gpioHandle - %d\n", AskEntry->desc.gpioHandle);
  if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    PortPresent - %d\n    maxLinkSpeedCap - %d\n    targetLinkSpeed = %d\n" ,
      AskEntry->desc.link_attributes.portPresent,
      AskEntry->desc.link_attributes.maxLinkSpeedCap,
      AskEntry->desc.link_attributes.targetLinkSpeed
    );
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Port - %d\n", AskEntry->status.port);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    HFC Index - %d\n", AskEntry->desc.link_attributes.hfc_index);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    DFC Index - %d\n", AskEntry->desc.link_attributes.dfc_index);
  }
}

/**--------------------------------------------------------------------
 *
 * MpioDebugDump
 *
 * @brief Helper function to dump general MPIO data
 *
 * @details tbd
 *
 * @param[in]  MpioData   Pointer MPIO_DATA
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioDebugDump (
  MPIO_DATA       *MpioData
  )
{
  FW_ASK_STRUCT       *AskPointer;
  uint32_t            AskCount;
  uint32_t            Index;

  AskPointer = MpioData->MpioAsk;
  AskCount = MpioData->MpioAskCount;
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Number of Asks is %d\n", AskCount);
  for (Index = 0;Index < AskCount; Index++) {
    DxioEntryConfigDump (AskPointer);
    AskPointer++;
  }

  return;
}
