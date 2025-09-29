/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioTopology.c
 *  @brief Initializes, create, and manages mpio/dxio topology.
 */

#include <Pci.h>
#include <xSIM.h>
#include <Nbio/Common/GnbDxio.h>
#include "MpioInitLib.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"

/**--------------------------------------------------------------------
 *
 * MpioUserDescriptorConfigDump
 *
 * @brief Helper function to dump input configuration to user engine descriptor
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioUserDescriptorConfigDump (
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "  Engine Type - %s\n",
    (EngineDescriptor->EngineData.EngineType == MpioPcieEngine) ? "PCIe Port" : (
      (EngineDescriptor->EngineData.EngineType == MpioUnusedEngine) ? "Unused" : "Invalid")
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
    EngineDescriptor->EngineData.StartLane,
    EngineDescriptor->EngineData.EndLane
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "    Hotplug - %d\n", EngineDescriptor->EngineData.HotPluggable);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "    GpioGroupId - %d\n", EngineDescriptor->EngineData.GpioGroupId);

  if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      " PortPresent - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.PortPresent
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      " DeviceNumber - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.DeviceNumber
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "FunctionNumber - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.FunctionNumber
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LinkSpeedCapability - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkSpeedCapability
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "    ClkPmSupport        - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.ClkPmSupport
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LinkAspm - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspm
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LinkAspmL1.1 - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspmL1_1
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LinkAspmL1.2 - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspmL1_2
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "ClkReq - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.ClkReq
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LinkHotplug - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkHotplug
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "SB link - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.SbLink
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "CS link - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.CsLink
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "IsMasterPLL - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.IsMasterPLL
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "MiscControls - 0x%02x\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "SlotPowerLimit - 0x%02x\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.SlotPowerLimit
      );
  }
}

/**--------------------------------------------------------------------
 *
 * MpioUserConfigDump
 *
 * @brief Helper function to dump input configuration to debug out
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioUserConfigDump (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  /*
   * Some of the variables used can not be currently initialized
   */
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;
  size_t                     Index;
  MPIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "<---------- PCIe User Config Start------------->\n");
  MPIO_TRACEPOINT(SIL_TRACE_INFO, " BMC Link Location is %x\n", ComplexDescriptor->BmcLinkLocation);

  LocalDescriptor = ComplexDescriptor;
  Index = 0;
  while (LocalDescriptor != NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "<---------------- Socket %d ----------------->\n", Index);
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      MpioUserDescriptorConfigDump(EngineDescriptor);
      EngineDescriptor = PcieConfigGetNextDataDescriptor(EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
    Index++;
  }

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "<---------- PCIe User Config End-------------->\n");
}

/**--------------------------------------------------------------------
 *
 * MpioManageEngineDescriptor
 *
 * @brief Helper function to dump input configuration to user engine descriptor
 *
 * @param[in]  EngineDescriptor   Pointer to engine descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioManageEngineDescriptor (
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  MPIO_PORT_PARAM *PortParam;
  MPIO_PHY_PARAM  *PhyParam;

  PortParam = (MPIO_PORT_PARAM *) &(EngineDescriptor->PortParams);
  PhyParam = (MPIO_PHY_PARAM *) &(EngineDescriptor->Phy);
  /*
   * Locate first empty PHY Parameter
   */
  while (PhyParam->ParamType != 0) {
    PhyParam++;
  }
  while (PortParam->ParamType != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "    ParamType - 0x%x\n    ParamValue - 0x%x\n",
      PortParam->ParamType,
      PortParam->ParamValue
      );
    switch (PortParam->ParamType) {
    case MPIO_PP_DEVICE:
      EngineDescriptor->Port.DeviceNumber = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_FUNCTION:
      EngineDescriptor->Port.FunctionNumber = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_PORT_PRESENT:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.PortPresent = 1;
      } else {
        EngineDescriptor->Port.PortPresent = 0;
      }
      break;
    case MPIO_PP_LINK_SPEED_CAP:
      EngineDescriptor->Port.LinkSpeedCapability = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_LINK_ASPM:
      EngineDescriptor->Port.LinkAspm = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_HOTPLUG_TYPE:
      EngineDescriptor->Port.LinkHotplug = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_CLKREQ:
      EngineDescriptor->Port.ClkReq = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_ASPM_L1_1:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.LinkAspmL1_1 = 1;
      } else {
        EngineDescriptor->Port.LinkAspmL1_1 = 0;
      }
      break;
    case MPIO_PP_ASPM_L1_2:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.LinkAspmL1_2 = 1;
      } else {
        EngineDescriptor->Port.LinkAspmL1_2 = 0;
      }
      break;
    case MPIO_PP_COMPLIANCE:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.MiscControls.LinkComplianceMode = 1;
      } else {
        EngineDescriptor->Port.MiscControls.LinkComplianceMode = 0;
      }
      break;
    case MPIO_PP_SAFE_MODE:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.MiscControls.LinkSafeMode = 1;
      } else {
        EngineDescriptor->Port.MiscControls.LinkSafeMode = 0;
      }
      break;
    case MPIO_PP_CHIPSET_LINK:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.MiscControls.SbLink = 1;
      } else {
        EngineDescriptor->Port.MiscControls.SbLink = 0;
      }
      break;
    case MPIO_PP_CLOCK_PM:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.MiscControls.ClkPmSupport = 1;
      } else {
        EngineDescriptor->Port.MiscControls.ClkPmSupport = 0;
      }
      break;
    case MPIO_PP_CHANNELTYPE:
      EngineDescriptor->Port.MiscControls.ChannelType = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_TURN_OFF_UNUSED_LANES:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 1;
      } else {
        EngineDescriptor->Port.MiscControls.TurnOffUnusedLanes = 0;
      }
      break;
    case MPIO_PP_MASTER_PLL:
      if (PortParam->ParamValue == true) {
        EngineDescriptor->Port.IsMasterPLL = 1;
      } else {
        EngineDescriptor->Port.IsMasterPLL = 0;
      }
      break;
    case MPIO_PP_SLOT_NUM:
      EngineDescriptor->Port.SlotNum = PortParam->ParamValue;
      break;
    case MPIO_PP_PHY_PARAM:
      *(uint16_t *) PhyParam = PortParam->ParamValue;
      PhyParam++;
      break;
    case MPIO_PP_ESM:
      EngineDescriptor->Port.SetEsmControl = 1;
      EngineDescriptor->Port.EsmControl = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_CXL:
      EngineDescriptor->Port.CxlControl = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_FIXED_PRESET:
      EngineDescriptor->Port.SetGen3FixedPreset = 1;
      EngineDescriptor->Port.Gen3FixedPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN4_FIXED_PRESET:
      EngineDescriptor->Port.SetGen4FixedPreset = 1;
      EngineDescriptor->Port.Gen4FixedPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN5_FIXED_PRESET:
      EngineDescriptor->Port.SetGen5FixedPreset = 1;
      EngineDescriptor->Port.Gen5FixedPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_PRESET_MASK:
      EngineDescriptor->Port.PresetMaskCntl.SetPresetMask8Gt = 1;
      EngineDescriptor->Port.PresetMaskCntl.PresetMask8Gt = (uint32_t)(PortParam->ParamValue & 0x3FF);
      break;
    case MPIO_PP_GEN4_PRESET_MASK:
      EngineDescriptor->Port.PresetMaskCntl.SetPresetMask16Gt = 1;
      EngineDescriptor->Port.PresetMaskCntl.PresetMask16Gt = (uint32_t)(PortParam->ParamValue & 0x3FF);
      break;
    case MPIO_PP_GEN5_PRESET_MASK:
      EngineDescriptor->Port.PresetMaskCntl.SetPresetMask32Gt = 1;
      EngineDescriptor->Port.PresetMaskCntl.PresetMask32Gt = (uint32_t)(PortParam->ParamValue & 0x3FF);
      break;
    case MPIO_PP_GEN4_DLF_CAP_DISABLE:
      EngineDescriptor->Port.Gen4Features |= (((uint8_t)PortParam->ParamValue & 1)? BIT_8(1) : 0);
      break;
    case MPIO_PP_GEN4_DLF_EXCHG_DISABLE:
      EngineDescriptor->Port.Gen4Features |= (((uint8_t)PortParam->ParamValue & 1)? BIT_8(0) : 0);
      break;
    case MPIO_PP_TARGET_LINK_SPEED:
      EngineDescriptor->Port.TargetLinkSpeed = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_PSPP_DC:
      EngineDescriptor->Port.PsppPolicyDC = (uint16_t)PortParam->ParamValue;
      break;
    case MPIO_PP_PSPP_AC:
      EngineDescriptor->Port.PsppPolicyAC = (uint16_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_DS_TX_PRESET:
      EngineDescriptor->Port.LaneEqualizationCntl.SetDsTxPreset = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.DsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_DS_RX_PRESET_HINT:
      EngineDescriptor->Port.LaneEqualizationCntl.SetDsRxPresetHint = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.DsRxPresetHint = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_US_TX_PRESET:
      EngineDescriptor->Port.LaneEqualizationCntl.SetUsTxPreset = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.UsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_US_RX_PRESET_HINT:
      EngineDescriptor->Port.LaneEqualizationCntl.SetUsRxPresetHint = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.UsRxPresetHint = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN4_DS_TX_PRESET:
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.SetDsTxPreset = 1;
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.DsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN4_US_TX_PRESET:
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.SetUsTxPreset = 1;
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.UsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN5_DS_TX_PRESET:
      EngineDescriptor->Port.Gen5LaneEqualizationCntl.SetDsTxPreset = 1;
      EngineDescriptor->Port.Gen5LaneEqualizationCntl.DsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN5_US_TX_PRESET:
      EngineDescriptor->Port.Gen5LaneEqualizationCntl.SetUsTxPreset = 1;
      EngineDescriptor->Port.Gen5LaneEqualizationCntl.UsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN2_DEEMPHASIS:
      EngineDescriptor->Port.TXDeEmphasisOride = 1;
      EngineDescriptor->Port.TXDeEmphasis = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_INVERT_POLARITY:
      EngineDescriptor->Port.InvertPolarity2 = ((uint8_t)PortParam->ParamValue & 2) >> 1;
      EngineDescriptor->Port.InvertPolarity = (uint8_t)PortParam->ParamValue & 1;
      break;
    case MPIO_PP_ALWAYS_EXPOSE:
      EngineDescriptor->Port.AlwaysExpose = (PortParam->ParamValue == true)? 1 : 0;
      break;
    case MPIO_PP_SRIS_ENABLED:
      EngineDescriptor->Port.SrisEnableMode = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_SKIP_INTERVAL:
      EngineDescriptor->Port.SrisSkipInterval = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_LOWER_OS_GEN_SUP:
      EngineDescriptor->Port.LowerSkpOsGenSup = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_LOWER_OS_RCV_SUP:
      EngineDescriptor->Port.LowerSkpOsRcvSup = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_AUTODETECT_MODE:
      EngineDescriptor->Port.SrisAutoDetectMode = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_SKP_INTERVAL_SEL:
      EngineDescriptor->Port.SrisSkpIntervalSel = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_SRIS_AUTODETECT_FACTOR:
      EngineDescriptor->Port.SrisAutodetectFactor = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_LEGACY_SWITCH0_ADDR:
      EngineDescriptor->Port.I2CMuxInfo |= (uint8_t) ((PortParam->ParamValue & 7) << 2);
      break;
    case MPIO_PP_LEGACY_SWITCH0_SELECT:
      EngineDescriptor->Port.I2CMuxInfo |= (uint8_t) ((PortParam->ParamValue & 3) | (1 << 5));
      break;
    case MPIO_PP_NPEM_ENABLE:
      EngineDescriptor->Port.NpemEnable = (uint16_t) PortParam->ParamValue;
      break;
    case MPIO_PP_NPEM_CAPABILITIES:
      EngineDescriptor->Port.NpemCapability = (uint16_t) PortParam->ParamValue;
      break;
    case MPIO_PP_BMC_LOCATION:
      EngineDescriptor->Port.IsBmcLocation = (PortParam->ParamValue == true) ? 1 : 0;
      break;
    case MPIO_PP_SWING_MODE:
      EngineDescriptor->Port.SwingMode = (uint16_t) PortParam->ParamValue;
      break;
    default:
      break;
    }
    PortParam++;
  }
}

/**--------------------------------------------------------------------
 *
 * MpioManageTopology
 *
 * @brief Callback for Topology Management
 *
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioManageTopology (
  MPIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  )
{
  MPIO_PORT_DESCRIPTOR      *PortDescriptor;
  MPIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;
  uint32_t                  ComplexCount;
  uint32_t                  PortCount;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  ComplexCount = 0;
  PortCount = 0;

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL && (ComplexCount < PROJ_MAX_SOCKETS_SUPPORTED)) {
    ComplexCount++;
    PortDescriptor = LocalDescriptor->PciePortList;

    while ((PortDescriptor != NULL) && (PortCount < MAX_PORTS_SUPPORTED)) {
      PortCount++;
      MpioManageEngineDescriptor(PortDescriptor);
      PortDescriptor = PcieConfigGetNextDataDescriptor(PortDescriptor);
    }

    if (PortDescriptor == NULL) {
      // Account for terminating descriptor
      PortCount++;
    }

    if (((PortCount == MAX_PORTS_SUPPORTED) && (PortDescriptor != NULL)) || (PortCount > MAX_PORTS_SUPPORTED)) {
      // Reached max ports, but did not encounter terminator
      MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Port descriptors reached limit without terminator.\n");
      break;
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
  }

  if (LocalDescriptor == NULL) {
    // Account for terminating descriptor
    ComplexCount++;
  }

  if (((LocalDescriptor != NULL) && (ComplexCount == PROJ_MAX_SOCKETS_SUPPORTED)) ||
    (ComplexCount > PROJ_MAX_SOCKETS_SUPPORTED)) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Complex descriptors reached limit without terminator.\n");
  }

  MpioUserConfigDump(ComplexDescriptor);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
