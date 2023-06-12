/**
 *  @file MpioPortVisiblity.c
 *  @brief Set visibility of ports based on status for Genoa
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <NBIO/IOD/include/IohcReg.h>
#include <NBIO/IOD/GnbRegisters.h>
#include <CommonLib/SmnAccess.h>
#include <NBIO/NbioIp2Ip.h>
#include "MpioCmn2.h"

/*----------------------------------------------------------------------------------------*/
/**
 * MpioControlPorts
 *
 * @brief Set the visibility of all PCIe ports
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */
void
MpioControlPorts (
    PCIE_PORT_VISIBILITY    Control,
    GNB_HANDLE              *GnbHandle
  )
{
  uint32_t            Value;
  PCIe_ENGINE_CONFIG  *Engine;
  NBIO_IP2IP_API      *NbioIp2Ip;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Value = (Control == HidePorts) ?
      ((1 << SIL_RESERVED_116) | (1 << SIL_RESERVED_118)) : 0;
  Value |= (1 << SIL_RESERVED_120);

  Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &(GnbHandle->Header)));
  while (Engine != NULL) {
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE (GnbHandle, Engine, SIL_RESERVED_746),
        (uint32_t)~(SIL_RESERVED_115 | SIL_RESERVED_117 | SIL_RESERVED_119),
        Value);
      Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_GNB);
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * MpioEnablePort
 *
 * @brief Enable visibility of links that trained or support hotplug
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */
void
MpioEnablePort (
  PCIe_ENGINE_CONFIG  *Engine,
  GNB_HANDLE          *GnbHandle
  )
{

  while (Engine != NULL) {
    if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Enabling %d\n", Engine->Type.Port.LogicalBridgeId);
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE (GnbHandle, Engine, SIL_RESERVED_746),
        (uint32_t)~(SIL_RESERVED_115 | SIL_RESERVED_117 | SIL_RESERVED_119),
        (1 << SIL_RESERVED_120)
      );
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_GNB);
  }
  return;
}
