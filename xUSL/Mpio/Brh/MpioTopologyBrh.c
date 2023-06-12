/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioTopologyBrh.c
 *  @brief Initializes, create, and manages mpio/dxio topology.
 *
 */

#include <Pci.h>
#include <xSIM.h>
#include <Mpio/Common/MpioInitLib.h>
#include <Mpio/Common/MpioLibLocal.h>
#include <Mpio/Common/MpioStructs.h>
#include "MpioCmn2Brh.h"

/**--------------------------------------------------------------------
 *
 * MpioTopologyWorkaroundsBrh
 *
 * @brief Initialization MPIO BRH topology structure
 *
 * @details Initialization settings that must be set
 *          in the MPIO BRH topology structure
 *
 * @param [in]   MPIO_COMPLEX_DESCRIPTOR ComplexDescriptor pointer
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
void
MpioTopologyWorkaroundsBrh (
  MPIO_COMPLEX_DESCRIPTOR      *ComplexDescriptor
  )
{
  MPIO_PORT_DESCRIPTOR     *EngineDescriptor;
  MPIO_COMPLEX_DESCRIPTOR  *LocalDescriptor;
  MPIOCLASS_INPUT_BLK      *SilData;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Get IP block data
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

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
        // US TX
        if (SilData->PcieGen3LaneEqUsTxPreset != 0xFF) {
          EngineDescriptor->Port.LaneEqualizationCntl.SetUsTxPreset = 1;
          EngineDescriptor->Port.LaneEqualizationCntl.UsTxPreset = SilData->PcieGen3LaneEqUsTxPreset;
        }
        // DS TX
        if (SilData->PcieGen3LaneEqDsTxPreset != 0xFF) {
          EngineDescriptor->Port.LaneEqualizationCntl.SetDsTxPreset = 1;
          EngineDescriptor->Port.LaneEqualizationCntl.DsTxPreset = SilData->PcieGen3LaneEqDsTxPreset;
        }
        // Mask
        if (SilData->PcieLaneEqPresetMask8GtConfig != 0xFF) {
          EngineDescriptor->Port.PresetMaskCntl.SetPresetMask8Gt = 1;
          EngineDescriptor->Port.PresetMaskCntl.PresetMask8Gt = SilData->PcieLaneEqPresetMask8Gt & 0x3FF;
        }

        //
        // Preset Settings All Ports (Gen4)
        //

        // US TX
        if (SilData->PcieGen4LaneEqUsTxPreset != 0xFF) {
          EngineDescriptor->Port.Gen4LaneEqualizationCntl.SetUsTxPreset = 1;
          EngineDescriptor->Port.Gen4LaneEqualizationCntl.UsTxPreset = SilData->PcieGen4LaneEqUsTxPreset;
        }

        // DS TX
        if (SilData->PcieGen4LaneEqDsTxPreset != 0xFF) {
          EngineDescriptor->Port.Gen4LaneEqualizationCntl.SetDsTxPreset = 1;
          EngineDescriptor->Port.Gen4LaneEqualizationCntl.DsTxPreset = SilData->PcieGen4LaneEqDsTxPreset;
        }

        // Mask
        if (SilData->PcieLaneEqPresetMask16GtConfig != 0xFF) {
          EngineDescriptor->Port.PresetMaskCntl.SetPresetMask16Gt = 1;
          EngineDescriptor->Port.PresetMaskCntl.PresetMask16Gt = SilData->PcieLaneEqPresetMask16Gt & 0x3FF;
        }

        //
        // Preset Settings All Ports (Gen5)
        //

        // US TX
        if (SilData->PcieGen5LaneEqUsTxPreset != 0xFF) {
          EngineDescriptor->Port.Gen5LaneEqualizationCntl.SetUsTxPreset = 1;
          EngineDescriptor->Port.Gen5LaneEqualizationCntl.UsTxPreset = SilData->PcieGen5LaneEqUsTxPreset;
        }

        // DS TX
        if (SilData->PcieGen5LaneEqDsTxPreset != 0xFF) {
          EngineDescriptor->Port.Gen5LaneEqualizationCntl.SetDsTxPreset = 1;
          EngineDescriptor->Port.Gen5LaneEqualizationCntl.DsTxPreset = SilData->PcieGen5LaneEqDsTxPreset;
        }

        // Mask
        if (SilData->PcieLaneEqPresetMask32GtConfig != 0xFF) {
          EngineDescriptor->Port.PresetMaskCntl.SetPresetMask32Gt = 1;
          EngineDescriptor->Port.PresetMaskCntl.PresetMask32Gt = SilData->PcieLaneEqPresetMask32Gt & 0x3FF;
        }

        // Link Speed Capability
        if (SilData->CfgForcePcieGenSpeed != 0xFF) {
          EngineDescriptor->Port.LinkSpeedCapability = SilData->CfgForcePcieGenSpeed;
        }
        /*
         * CXL workaround - CXL requires Gen5 advertised by root port
         */
        if (EngineDescriptor->EngineData.StartLane < 64) {
          if (SilData->AmdCxlOnAllPorts) {
            EngineDescriptor->Port.CxlControl = 1;
          }

          if (EngineDescriptor->Port.CxlControl == 1) {
            EngineDescriptor->Port.LinkSpeedCapability = PcieGenMaxSupported;
          }
        }

        // SRIS
        if (SilData->SrisEnableMode != 0xFF) {
          SilData->SrisCfgType = SilData->SrisCfgType | BIT_8(0);
          EngineDescriptor->Port.SrisEnableMode = SilData->SrisEnableMode;
          EngineDescriptor->Port.SrisSkipInterval = SilData->SrisSkipInterval;
          EngineDescriptor->Port.LowerSkpOsGenSup = SilData->SrisLowerSkpOsGenSup;
          EngineDescriptor->Port.LowerSkpOsRcvSup = SilData->SrisLowerSkpOsRcvSup;
        }

        if (SilData->TargetPcieGenSpeed != 0xFF) {
          EngineDescriptor->Port.TargetLinkSpeed = SilData->TargetPcieGenSpeed;
        }

        if (SilData->AmdDlfCapEnV2 != 0xF) {
          if (SilData->AmdDlfCapEnV2 == 0) {
            EngineDescriptor->Port.Gen4Features |= BIT_8(1);
          } else {
            EngineDescriptor->Port.Gen4Features &= ~BIT_8(1);
          }
        }

        if (SilData->AmdDlfExEnV2 != 0xF) {
          if (SilData->AmdDlfExEnV2 == 0) {
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

  return;
}
