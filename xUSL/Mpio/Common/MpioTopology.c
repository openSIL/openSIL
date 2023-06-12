/**
 *  @file MpioTopology.c
 *  @brief Initializes, create, and manages mpio/dxio topology.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <Pci.h>
#include <xSIM.h>
#include "MpioInitLib.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"

/*
 * Pre-defined Mpio Topology Port Descriptor Template Structure
 */
static MPIO_PORT_DESCRIPTOR TemplateTopologyPort =
  {
    0,
    MPIO_ENGINE_DATA_INITIALIZER (MpioUnusedEngine, 0, 0, PcieHotplugDisabled, 0),
    MPIO_PORT_DATA_INITIALIZER_PCIE (
      MpioPortEnabled,                      // Port Present
      0,                                    // Requested Device
      0,                                    // Requested Function
      PcieHotplugDisabled,                  // Hotplug
      PcieGenMaxSupported,                  // Max Link Speed
      PcieGenMaxSupported,                  // Max Link Capability
      AspmL1,                               // ASPM
      AspmDisabled,                         // ASPM L1.1 disabled
      AspmDisabled,                         // ASPM L1.2 disabled
      MpioClkPmSupportDisabled              // Clock PM
    )
  };

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
void
MpioUserDescriptorConfigDump (
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Engine Type - %s\n",
     (EngineDescriptor->EngineData.EngineType == MpioPcieEngine) ? L"PCIe Port" : (
     (EngineDescriptor->EngineData.EngineType == MpioSATAEngine) ? L"SATA Port" : (
     (EngineDescriptor->EngineData.EngineType == MpioEthernetEngine) ? L"Ethernet Port" : (
     (EngineDescriptor->EngineData.EngineType == MpioUnusedEngine) ? L"Unused" : L"Invalid")))
  );
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Start Phy Lane - %d\n    End   Phy Lane - %d\n",
      EngineDescriptor->EngineData.StartLane, EngineDescriptor->EngineData.EndLane);



  if (EngineDescriptor->EngineData.EngineType == PciePortEngine) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, " PortPresent - %d\n",
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.PortPresent);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, " DeviceNumber - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.DeviceNumber);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "FunctionNumber - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.FunctionNumber);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "LinkSpeedCapability - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkSpeedCapability);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "LinkAspm - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkAspm);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "LinkHotplug - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.LinkHotplug);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "SB link - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls.SbLink);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "MiscControls - 0x%02x\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.MiscControls);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "IsMasterPLL - %d\n" ,
      ((MPIO_PORT_DESCRIPTOR *) EngineDescriptor)->Port.IsMasterPLL);
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




  LocalDescriptor = ComplexDescriptor;
  Index = 0;
  while (LocalDescriptor != NULL) {

    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      MpioUserDescriptorConfigDump (EngineDescriptor);
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
    Index++;
  }


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
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "    ParamType - 0x%x\n    ParamValue - 0x%x\n",
      PortParam->ParamType, PortParam->ParamValue);
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
    case MPIO_PP_APIC_GROUPMAP:
      EngineDescriptor->Port.ApicDeviceInfo.GroupMap = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_APIC_SWIZZLE:
      EngineDescriptor->Port.ApicDeviceInfo.Swizzle = (uint8_t) PortParam->ParamValue;
      break;
    case MPIO_PP_APIC_BRIDGEINT:
      EngineDescriptor->Port.ApicDeviceInfo.BridgeInt = (uint8_t) PortParam->ParamValue;
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
      EngineDescriptor->Port.EsmControl = (uint8_t)PortParam->ParamValue;
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
      EngineDescriptor->Port.SetGen3LaneEq = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.DsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_DS_RX_PRESET_HINT:
      EngineDescriptor->Port.SetGen3LaneEq = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.DsRxPresetHint = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_US_TX_PRESET:
      EngineDescriptor->Port.SetGen3LaneEq = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.UsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN3_US_RX_PRESET_HINT:
      EngineDescriptor->Port.SetGen3LaneEq = 1;
      EngineDescriptor->Port.LaneEqualizationCntl.UsRxPresetHint = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN4_DS_TX_PRESET:
      EngineDescriptor->Port.SetGen4LaneEq = 1;
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.DsTxPreset = (uint8_t)PortParam->ParamValue;
      break;
    case MPIO_PP_GEN4_US_TX_PRESET:
      EngineDescriptor->Port.SetGen4LaneEq = 1;
      EngineDescriptor->Port.Gen4LaneEqualizationCntl.UsTxPreset = (uint8_t)PortParam->ParamValue;
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
    case MPIO_PP_NPEM_CAPABILITES:
      EngineDescriptor->Port.NpemCapability = (uint16_t) PortParam->ParamValue;
      break;
    case MPIO_PP_BMC_LOCATION:
      EngineDescriptor->Port.IsBmcLocation = 1;
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
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;
  MPIO_COMPLEX_DESCRIPTOR   *LocalDescriptor;



  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    EngineDescriptor = LocalDescriptor->PciePortList;
    while (EngineDescriptor != NULL) {
      MpioManageEngineDescriptor (EngineDescriptor);
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    LocalDescriptor = PcieConfigGetNextDataDescriptor (LocalDescriptor);
  }
  MpioUserConfigDump (ComplexDescriptor);


}


/**--------------------------------------------------------------------
 *
 * GetDescriptorHandle
 *
 * @brief A hash function that generates a handle from a descriptor
 *
 * @param [in] InDescriptor  Pointer to MPIO_PORT_DESCRIPTOR
 * @param [in] tbd
 * @param [in] tbd
 *
 * @returns Hash value.
 * @retval 64-bit has meant for comparison, searching, etc.
 **/
uint64_t
GetDescriptorHandle (
  MPIO_PORT_DESCRIPTOR   *InDescriptor
  )
{
  union {
    uint8_t     BitArray[8];
    uint64_t    Value;
  } Handle;

  Handle.Value = 0;
  Handle.BitArray[0] = InDescriptor->EngineData.StartLane;
  Handle.BitArray[1] = InDescriptor->EngineData.EndLane;
  Handle.BitArray[2] = InDescriptor->EngineData.EngineType;
  Handle.BitArray[3] = InDescriptor->EngineData.GpioGroupId;

  return Handle.Value;
}


/**--------------------------------------------------------------------
 *
 * InitializeTopologyEntry
 *
 * @brief This function will initialize the first Unused entry using default values for the specified engine type
 *
 * @param[in] TablePointer        Pointer to table containing entries to work with
 * @param[in] EngineType          Engine Type
 * @param[in] StartLane           Start lane number
 * @param[in] EndLane             End lane number
 * @param[in] ResetId             Reset ID number
 * @param[in] Device              Requested PCI Device (not applicable for non PCI devices)
 * @param[in] Function            Requested PCI Function (not applicable for non PCI devices)
 * @param[out] InitializeSuccess  Topology entry is initialized (true) or not (false).
 * @param[in] EntryHandle         Unique handle ID for entry, pointer is passed in.
 *
 * @returns OpenSIL standard resturn status.
 * @retval SilPass is valid return.
 **/
void
InitializeTopologyEntry (
   MPIO_COMPLEX_DESCRIPTOR  **InTablePointer,
   uint8_t                  EngineType,
   uint8_t                  StartLane,
   uint8_t                  EndLane,
   uint8_t                  ResetId,
   uint8_t                  Device,
   uint8_t                  Function,
   bool                     *InitializeSuccess,
   uint64_t                 *EntryHandle
  )
{
  size_t                      Counter;
  bool                    FoundEntry;
  MPIO_COMPLEX_DESCRIPTOR    *ComplexTable;

  ComplexTable = *InTablePointer;

  /*
   * Checks on parameters
   */
  assert(ComplexTable != NULL);
  assert (EngineType < MpioMaxPcieEngine);
  assert (StartLane <= 129);
  assert (EndLane <= 129);
  assert (ResetId < 129);
  assert (InitializeSuccess != NULL);

  Counter = 0;
  FoundEntry = false;

  /*
   * Make sure entries are available
   */
  if (ComplexTable->PciePortList == NULL) {
    *InitializeSuccess = false;
    return;
  }

  /*
   * Find an unused entry
   */
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    /*
	 * Entry is unused if it is of unused type and startlane and endlane are both 0
	 */
    if ((ComplexTable->PciePortList[Counter].EngineData.EngineType == MpioUnusedEngine) &&
        (ComplexTable->PciePortList[Counter].EngineData.StartLane == 0) &&
        (ComplexTable->PciePortList[Counter].EngineData.EndLane == 0)) {
      FoundEntry = true;
      break;
    }
    Counter++;
  }

  /*
   * No entries available
   */
  if (FoundEntry == false) {
    *InitializeSuccess = false;


    return;
  }

  ComplexTable->PciePortList[Counter].EngineData.EngineType = EngineType;
  ComplexTable->PciePortList[Counter].EngineData.StartLane = StartLane;
  ComplexTable->PciePortList[Counter].EngineData.EndLane = EndLane;
  ComplexTable->PciePortList[Counter].EngineData.GpioGroupId = ResetId;
  switch (EngineType) {
  case MpioUnusedEngine:
    break;
  case MpioPcieEngine:
    ComplexTable->PciePortList[Counter].Port.DeviceNumber = Device;
    ComplexTable->PciePortList[Counter].Port.FunctionNumber = Function;
    break;
  case MpioSATAEngine:
    break;
  case MpioUSB_OVER_PCIE:
    break;
  default:
    assert(false);
    break;
  }

  MpioUserDescriptorConfigDump(&(ComplexTable->PciePortList[Counter]));
  *EntryHandle = GetDescriptorHandle(&(ComplexTable->PciePortList[Counter]));
  *InitializeSuccess = true;



}

/**--------------------------------------------------------------------
 *
 * AddParameter
 *
 * @brief This function will add a parameter initializer to the generic parameter list of the specified topology entry
 *
 * @param [in] TablePointer             PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI instance
 *                                      Pointer to table containing entries to work with
 * @param [in] EntryHandle              Unique handle ID associated with a given entry
 * @param [in] Parameter                Parameter to add to entry
 * @param [out] AddParamSuccess         Function was able to add parameter, true/false is Success/Failure respectively.
 *
 * @returns Nothing, the status is returned in AddParamSuccess.
 **/
void
AddParameter (
   MPIO_COMPLEX_DESCRIPTOR  **TablePointer,
   uint64_t                 EntryHandle,
   MPIO_PORT_PARAM          Parameter,
  bool                      *AddParamSuccess
  )
{
  MPIO_COMPLEX_DESCRIPTOR   *ComplexTable;
  size_t                    Counter;
  size_t                    ParamIndex;
  bool                      FoundEntry;

  /*
   * Checks on parameters
   */
  assert (*TablePointer != NULL);

  ComplexTable = *TablePointer;
  Counter = 0;
  FoundEntry = false;

  /*
   * Make sure entries are available
   */
  if (ComplexTable->PciePortList == NULL) {
    *AddParamSuccess = false;
    return;
  }

  /*
   * Find entry associated with handle
   */
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    if (GetDescriptorHandle(&(ComplexTable->PciePortList[Counter])) == EntryHandle) {
      FoundEntry = true;
      break;
    }
    Counter++;
  }

  /*
   * No matching entry
   */
  if (FoundEntry == false) {
    *AddParamSuccess = false;
    return;
  }

  /*
   * Find unused parameter entry
   */
  FoundEntry = false;
  ParamIndex = 0;
  while ((ParamIndex < PCIE_PORT_PARAMETER_COUNT) && (FoundEntry == false)) {
    if (ComplexTable->PciePortList[Counter].PortParams.PhyParam[ParamIndex].ParamType == 0)
    {
      ComplexTable->PciePortList[Counter].PortParams.PhyParam[ParamIndex] = Parameter;
      FoundEntry = true;
    }
    ParamIndex++;
  }

  *AddParamSuccess = FoundEntry;
}

/**--------------------------------------------------------------------
 *
 * RemoveEntry
 *
 * @brief This function will mark a previously created entry as unused and remove any parameters in the list.
 *
 * @param [in]  TablePointer        PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI instance
 *                                  Pointer to table containing entries to work with EntryHandle
 * @param [in]  EntryHandle         Unique handle ID associated with a given entry
 * @param [out] RemoveEntrySuccess  Function was able to remove entry, where True/False is Success/Failure
 *                                  respectively. Pointer Is passed in.
 *
 * @returns Nothing, the status is in RemoveEntrySuccess
 **/
void
RemoveEntry (
   MPIO_COMPLEX_DESCRIPTOR         **TablePointer,
   uint64_t                          EntryHandle,
  bool                         *RemoveEntrySuccess
  )
{
  MPIO_COMPLEX_DESCRIPTOR    *ComplexTable;
  size_t                      Counter;
  bool                    FoundEntry;

  /*
   * Checks on parameters
   */
  assert (*TablePointer != NULL);

  ComplexTable = *TablePointer;
  Counter = 0;
  FoundEntry = false;

  /*
   * Make sure entries are available
   */
  if (ComplexTable->PciePortList == NULL) {
    *RemoveEntrySuccess = false;
    return;
  }

  /*
   * Find entry associated with handle
   */
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    if (GetDescriptorHandle(&(ComplexTable->PciePortList[Counter])) == EntryHandle) {
      FoundEntry = true;
      break;
    }

    Counter++;
  }

  /*
   * No matching entry
   */
  if (FoundEntry == false) {
    *RemoveEntrySuccess = false;
    return;
  }

  /*
   * Re copy empty entry template over disbanded entry
   */
  ComplexTable->PciePortList[Counter] = TemplateTopologyPort;
  *RemoveEntrySuccess = true;
}

/**--------------------------------------------------------------------
 *
 * IncreaseTableSize
 *
 * @brief This function is used to grow the table beyond the initial allocation.
 *
 * @details New memory is allocated and the old table is copied over.
 *          Do not use on any memory not associated with the heap.
 *
 * @param[in] TablePointer   Pointer to table containing entries to work with
 * @param[in] NewSize        New size of the table, in entries. Must be greater than the current size
 * @param[out] IncreaseSuccess Returns false if there is an issue with the input parameters
 *                           or the allocation process. True if success.
 *
 * @returns Nothing, the status is returned in IncreaseSuccess
 **/
void
IncreaseTableSize (
  MPIO_COMPLEX_DESCRIPTOR   **TablePointer,
  size_t                    NewSize,
  bool                      *IncreaseSuccess
  )
{
  // ComplexTable is a static data, it can not be increased
  *IncreaseSuccess = false;
  return;

#if 0
Here is the original reallocation code, kept for reference only
  MPIO_COMPLEX_DESCRIPTOR    *ComplexTable;
  size_t                      Counter;
  size_t                      NewListIndex;



  /*
   * Checks on parameters
   */
  assert (*TablePointer != NULL);
  assert (IncreaseSuccess != NULL);
  assert (NewSize < 256);

  ComplexTable = *TablePointer;
  Counter = 0;
  *IncreaseSuccess = false;

  /*
   * Make sure entries are available
   */
  if (ComplexTable->PciePortList == NULL) {
    *IncreaseSuccess = false;
    return SilPass;
  }

  /*
   * Figure out how many entries are currently in the table, including terminator (1 based quantity)
   */
  do {
    Counter++;
  } while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );

  /*
   * Account for terminator at the end in new list size
   */
  Counter++;
  NewSize++;

  /*
   * Make sure table is actually requested to get larger
   */
  if (NewSize < Counter) {
    *IncreaseSuccess = false;


    return SilInvalidParameter;
  }




  /*
   * Finally, allocate the memory
   */
  ComplexTable->PciePortList = ReallocatePool (sizeof (MPIO_PORT_DESCRIPTOR) * Counter,
                                               sizeof (MPIO_PORT_DESCRIPTOR) * NewSize, ComplexTable->PciePortList);

  ComplexTable->PciePortList[Counter - 1].Flags &= ~DESCRIPTOR_TERMINATE_LIST;
  /*
   * Copy template entry to additional entries, overwritign terminator entry in initial list
   */
  for (NewListIndex = Counter; NewListIndex < (NewSize - 1); NewListIndex++) {
    ComplexTable->PciePortList[NewListIndex] = TemplateTopologyPort;
  }

  /*
   * Program terminator entry in new list
   */
  ComplexTable->PciePortList[NewSize - 1].Flags = DESCRIPTOR_TERMINATE_LIST;

  *IncreaseSuccess = true;


  return SilPass;
#endif
}
