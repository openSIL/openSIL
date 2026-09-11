/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc, All rights reserved. */
/**
 *  @file MpioPortVisiblityBrh.c
 *  @brief Set visibility of ports based on status for BRH
 *
 */

#include <xSIM.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/NbioIp2Ip.h>
#include "MpioCmn2Brh.h"

/**--------------------------------------------------------------
 * MpioControlPortsBrh
 *
 * @brief Set the visibility of all PCIe ports
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 *
 * @returns Nothing
 *
 */
void
MpioControlPortsBrh (
  PCIE_PORT_VISIBILITY    Control,
  GNB_HANDLE              *GnbHandle
  )
{
  uint32_t            Value;
  PCIe_ENGINE_CONFIG  *Engine;
  NBIO_IP2IP_API      *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Value = (Control == HidePorts) ?
    ((1 << SIL_RESERVED_0606) | (1 << SIL_RESERVED_0609)) : 0;
  Value |= (1 << SIL_RESERVED_0611);

  Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(GnbHandle->Header)));
  while (Engine != NULL) {
    if (GnbHandle->RBIndex < 4) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
        (uint32_t) ~(SIL_RESERVED_0605 | SIL_RESERVED_0608 | SIL_RESERVED_0610),
        Value
        );
    } else {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
        (uint32_t) ~(SIL_RESERVED_0605 | SIL_RESERVED_0608 | SIL_RESERVED_0610),
        Value
        );
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_GNB);
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return;
}

/**--------------------------------------------------------------
 * MpioEnablePortRs
 *
 * @brief Enable visibility of links that trained or support hotplug
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 *
 * @returns Nothing
 *
 */
void
MpioEnablePortBrh (
  PCIe_ENGINE_CONFIG  *Engine,
  GNB_HANDLE          *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG     *Wrapper;
  NBIO_IP2IP_API          *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  while (Engine != NULL) {
    Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
    if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled) &&
         (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugInboard)) ||
        PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE)
       ) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "%a Enabling port %d, RBIndex %d, Wrapper %d\n",
        Engine->Type.Port.LogicalBridgeId,
        GnbHandle->RBIndex,
        Wrapper->WrapId
        );
      if (GnbHandle->RBIndex < 4) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
          (uint32_t) ~(SIL_RESERVED_0605 | SIL_RESERVED_0608 | SIL_RESERVED_0610),
          (1 << SIL_RESERVED_0611)
          );
      } else {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
          (uint32_t) ~(SIL_RESERVED_0605 | SIL_RESERVED_0608 | SIL_RESERVED_0610),
          (1 << SIL_RESERVED_0611)
          );
      }
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_GNB);
  }
  return;
}
