/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioPortVisibilityPhx.c
 *  @brief Set visibility of ports based on status
 */

#include <xSIM.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>
#include <Nbio/Phx/NbioPcieComplexDataPhx.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/NbioIp2Ip.h>
#include "MpioCmn2Phx.h"

/*----------------------------------------------------------------------------------------*/
/**
 * MpioControlPortsPhx
 *
 * @brief Set the visibility of all PCIe ports
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */
void
MpioControlPortsPhx (
  PCIE_PORT_VISIBILITY    Control,
  GNB_HANDLE              *GnbHandle
  )
{
  uint32_t            Value;
  uint32_t            Index;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Value = (1 << BIT18_OFFSET_AAAAATI);

  if (Control == HidePorts) {
    Value |= ((1 << BIT0_OFFSET_AAAAATE) | (1 << BIT2_OFFSET_AAAAATG));
  }

  for (Index = 0; Index < NUMBER_OF_PHX_GPP0_PORTS; Index++) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, (IOHCx13b31004 + (Index << 10))),
      (uint32_t) ~(BIT0_MASK_AAAAATD | BIT2_MASK_AAAAATF | BIT18_MASK_AAAAATH),
      Value
      );
  }
  for (Index = 0; Index < NUMBER_OF_PHX_GFX_PORTS; Index++) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, (IOHCx13b33004 + (Index << 10))),
      (uint32_t) ~(BIT0_MASK_AAAAATD | BIT2_MASK_AAAAATF | BIT18_MASK_AAAAATH),
      Value
      );
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * MpioEnablePortPhx
 *
 * @brief Enable visibility of links that trained or support hotplug
 *
 * @param[in]  Control             Control Hide/Unhide ports
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */
void
MpioEnablePortPhx (
  PCIe_ENGINE_CONFIG  *Engine,
  GNB_HANDLE          *GnbHandle
  )
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  while (Engine != NULL) {
    if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Enabling %d\n", Engine->Type.Port.LogicalBridgeId);
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE(GnbHandle, Engine, IOHCx13b31004),
        (uint32_t) ~(BIT0_MASK_AAAAATD | BIT2_MASK_AAAAATF | BIT18_MASK_AAAAATH),
        (1 << BIT18_OFFSET_AAAAATI)
        );
    }
    Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_GNB);
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
