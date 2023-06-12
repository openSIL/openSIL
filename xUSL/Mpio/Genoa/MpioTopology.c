/**
 *  @file MpioTopology.c
 *  @brief Initializes, create, and manages mpio/dxio topology.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <Pci.h>
#include <xSIM.h>
#include <Mpio/Common/MpioInitLib.h>
#include <Mpio/Common/MpioLibLocal.h>
#include <Mpio/Common/MpioStructs.h>
#include "MpioCmn2.h"

void
MpioTopologyWorkarounds (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;
  MPIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;
  MPIOCLASS_INPUT_BLK       *SilData;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.EngineType == MpioSATAEngine) {
        EngineDescriptor->Port.MiscControls.ChannelType = SilData->CfgSataPhyTuning;
      }
      if (EngineDescriptor->EngineData.EngineType == MpioPcieEngine) {
        /*
		 * EqPreset
		 */
        EngineDescriptor->Port.EqPreset = 3;
        /*
		 * Enable TurnOffUnusedLanes
		 */
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 1;
        if ((SilData->PcieLinkAspmAllPort) != 0xFF) {
          EngineDescriptor->Port.LinkAspm = SilData->PcieLinkAspmAllPort;
        }
        /*
		 * Set SATA Channel Type
		 */
        if (SilData->PcieLinkComplianceModeAllPorts) {
          EngineDescriptor->Port.MiscControls.LinkComplianceMode = 1;
          EngineDescriptor->Port.LinkHotplug = 0;
        }
        if (SilData->CfgForcePcieGenSpeed != 0) {
          EngineDescriptor->Port.LinkSpeedCapability = SilData->CfgForcePcieGenSpeed;
        }
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
  }
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}
