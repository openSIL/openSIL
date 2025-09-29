/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioTopologyPhx.c
 *  @brief Initializes, create, and manages mpio/dxio topology.
 */

#include <Pci.h>
#include <xSIM.h>
#include <Mpio/Common/MpioInitLib.h>
#include <Mpio/Common/MpioLibLocal.h>
#include <Mpio/Common/MpioStructs.h>
#include "MpioCmn2Phx.h"
#include "MpioPhxData.h"

static
void
MpioGenSpeedWorkaround (
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  if ((EngineDescriptor->Port.LinkSpeedCapability == PcieGenMaxSupported) ||
    (EngineDescriptor->Port.LinkSpeedCapability > PcieGen4)) {
    EngineDescriptor->Port.LinkSpeedCapability = PcieGen4;
  }
}

void
MpioTopologyWorkaroundsPhx (
  SIL_CONTEXT                 *SilContext,
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  MPIO_PORT_DESCRIPTOR        *EngineDescriptor;
  MPIO_COMPLEX_DESCRIPTOR     *LocalDescriptor;
  MPIOCLASS_COMMON_INPUT_BLK  *SilDataCommon;
  MPIOCLASS_PHX_INPUT_BLK     *SilDataPhx;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.EngineType == MpioPcieEngine) {
        MpioGenSpeedWorkaround(EngineDescriptor);
        /*
         * Enable TurnOffUnusedLanes
         */
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 1;

        if (SilDataCommon->PcieLinkComplianceModeAllPorts) {
          EngineDescriptor->Port.MiscControls.LinkComplianceMode = 1;
        }

        if (SilDataPhx->AmdDlfCapEnV2 != 0xF) {
          if (SilDataPhx->AmdDlfCapEnV2 == 0) {
            EngineDescriptor->Port.Gen4Features |= BIT_8(1);
          } else {
            EngineDescriptor->Port.Gen4Features &= ~BIT_8(1);
          }
        }
        if (SilDataPhx->AmdDlfExEnV2 != 0xF) {
          if (SilDataPhx->AmdDlfExEnV2 == 0) {
            EngineDescriptor->Port.Gen4Features |= BIT_8(0);
          } else {
            EngineDescriptor->Port.Gen4Features &= ~BIT_8(0);
          }
        }
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor(EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
