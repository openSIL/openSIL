/**
 *  @file MpioUbmTopology.c
 *  @brief UBM Interface Handler
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <xSIM.h>
#include "MpioLib.h"
#include "MpioInitLib.h"

/**--------------------------------------------------------------------
 *
 * CountEntries
 *
 * @brief Count the number of socket entries so we allocate the right size of a table
 *
 * @param [in] ComplexTable  tbd
 *
 * @returns How many enstries are in the complex table.
 * @retval Number of entries.
 **/
static
size_t
CountEntries (
  MPIO_COMPLEX_DESCRIPTOR         *ComplexTable
  )
{
  size_t        Counter;

  Counter = 0;
  /*
   * Figure out how many entries are currently in the table, including terminator (1 based quantity)
   */
  do {
    Counter++;
  } while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  return ++Counter;
}

/**--------------------------------------------------------------------
 *
 * HfcDescriptorDebugDump
 *
 * @brief Hfc debug helper to dump descriptors
 *
 * @param [in] HfcDescriptor  tbd
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
HfcDescriptorDebugDump (
  MPIO_UBM_HFC_DESCRIPTOR  *HfcDescriptor
  )
{
  uint32_t                          HfcIndex;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  for (HfcIndex = 0; HfcIndex < HfcDescriptor->Size; HfcIndex++) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "HFC Entry\n");
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   StartLane:             %x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Start_Lane);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   SlotNum:               %x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Device.Ubm.SlotNum);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_GPIOx_TYPE:         %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Type);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_GPIOx_ADDRESS:      0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Expander.Addr);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_GPIOx_I2C_RESET:    %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Device.Ubm.I2c_Reset);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_GPIOx_BP_TYPE:      %d\n",
      HfcDescriptor->HfcPortList[HfcIndex].Device.Ubm.Bp_Type);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_UBM_SWITCH0_ADDR:   0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[0].Addr);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_UBM_SWITCH0_SELECT: 0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[0].Select);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_UBM_SWITCH1_ADDR:   0x%x\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[1].Addr);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "   PP_UBM_SWITCH1_SELECT: 0x%x\n\n",
      HfcDescriptor->HfcPortList[HfcIndex].Switch[1].Select);
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
  UbmGetDescriptorHandle
    A hash function that generates a handle from a descriptor

  @param InDescriptor  Pointer to DXIO_PORT_DESCRIPTOR

  @return uint64_t     Hash value meant for comparison,
                       searching etc
**/
static
uint64_t
UbmGetDescriptorHandle (
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

static
SIL_STATUS
UbmAncDataConfig (
  MPIO_COMPLEX_DESCRIPTOR         **TablePointer,
  uint64_t                        EntryHandle,
  MPIO_ANC_DATA                   *AncData
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
    return SilPass;
  }

  if (AncData->Count == 0) {
    return SilPass;
  }

  /*
   * Find entry associated with handle
   */
  while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 ) {
    if (UbmGetDescriptorHandle(&(ComplexTable->PciePortList[Counter])) == EntryHandle) {
      FoundEntry = true;
      break;
    }
    Counter++;
  }

  /*
   * No matching entry
   */
  if (FoundEntry == false) {
    return SilPass;
  }

  /*
   * Add Ancillary data config
   */
  ComplexTable->PciePortList[Counter].AncData.Count = AncData->Count;
  ComplexTable->PciePortList[Counter].AncData.Ovrd = AncData->Ovrd;

  return SilPass;
}

/**--------------------------------------------------------------------
 *
 * MpioUbmDiscovery
 *
 * @brief Primary UBM discovery flow
 *
 *  @param [in] ComplexDescriptor Description for ComplexDescriptor
 *  @param [in] HfcDescriptor Description for HfcDescriptor
 *  @param [in] HfcStartLanes Description for HfcStartLanes
 *  @param [in] HfcListSize Description for HfcListSize
 *  @param [in] Pcie Description for Pcie
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioUbmDiscovery (
  MPIO_COMPLEX_DESCRIPTOR   **ComplexDescriptor,
  MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor,
  uint8_t                   *HfcStartLanes,
  uint8_t                   HfcListSize,
  size_t                    Socket1Index,
  PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  MPIO_COMPLEX_DESCRIPTOR         *ComplexTable;
  MPIO_COMPLEX_DESCRIPTOR         *TempComplexTable;
  size_t                          EntryCount;
  bool                            Result;
  uint64_t                        EntryHandle;

  DFC_DESCRIPTOR                  *Dfc;

  uint32_t                        HfcIndex;
  uint32_t                        DfcIndex;
  uint32_t                        DfcCount;
  PCI_ADDR                        PciAddress;
  uint32_t                        Response;
  uint32_t                        MpioArg[6];
  uint8_t                         StartLane;
  uint8_t                         EndLane;
  uint8_t                         ResetId;
  MPIO_PORT_PARAM                 PortParam;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  EntryHandle = 0;

  if ((HfcDescriptor == NULL) || (HfcStartLanes == NULL) || (Pcie == 0)) {
    return;
  }

  HfcDescriptorDebugDump (HfcDescriptor);

  PciAddress.AddressValue = 0;

  memset (MpioArg, 0x00, sizeof(MpioArg));
  MpioArg[0] = (uint32_t)(uintptr_t) &HfcDescriptor->HfcPortList[0];
  MpioArg[1] = (uint32_t) HfcListSize;
  Response = MpioServiceRequestCommon (PciAddress, BIOS_MPIO_MSG_I2C_ENUMERATE, MpioArg, 0);
  MPIO_TRACEPOINT (SIL_TRACE_INFO,
    "  MPIO Response = 0x%x\n",
    Response);
  MPIO_TRACEPOINT (SIL_TRACE_INFO,
    "  MPIO Args = 0x%x - 0x%x - 0x%x - 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3]);
  if ((Response & 0xFF) != BIOSSMC_Result_OK) {
    return;
  }

  TempComplexTable = *ComplexDescriptor;
  ComplexTable = *ComplexDescriptor;
  Result = false;
  EntryCount = CountEntries (ComplexTable);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, " Entry Count returned = %d\n", EntryCount);

  for (HfcIndex = 0; HfcIndex < HfcListSize; HfcIndex++) {

    if (HfcIndex == Socket1Index) {
      ComplexTable = PcieConfigGetNextDataDescriptor (ComplexTable);
    }

    if (ComplexTable == NULL) {
      break;
    }

    DfcIndex = 0;
    DfcCount = 1;
    do {
      memset (MpioArg, 0x00, sizeof(MpioArg));
      MpioArg[0] = HfcIndex;
      MpioArg[1] = DfcIndex;
      if ((HfcDescriptor->HfcToDfcData[HfcIndex].OcpDefValid & SIL_RESERVED_831) && DfcIndex == 0) {
        MpioArg[2] = (uint32_t) (((HfcDescriptor->HfcToDfcData[HfcIndex].OcpDefValid & SIL_RESERVED_831) << 31) |
                     ((HfcDescriptor->HfcToDfcData[HfcIndex].OcpDefSecPrsntb & SIL_RESERVED_830) << 4) |
                     (HfcDescriptor->HfcToDfcData[HfcIndex].OcpDefPrimPrsntb & SIL_RESERVED_830));
      }
      Response = MpioServiceRequestCommon (PciAddress, BIOS_MPIO_MSG_I2C_DEVICE_GET, MpioArg, 0);
      MPIO_TRACEPOINT (SIL_TRACE_INFO,
        "  MPIO Response = 0x%x\n",
        Response);
      MPIO_TRACEPOINT (SIL_TRACE_INFO,
        "  MPIO Args = 0x%x - 0x%x - 0x%x - 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3]);
      if (((Response & 0xFF) == BIOSSMC_Result_OK) || ((Response & 0xFF) == 0)) {
        Dfc = (DFC_DESCRIPTOR *) &MpioArg[1];
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  hfc_idx =    0x%x\n", Dfc->hfc_idx);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  num_dfcs =   0x%x\n", Dfc->num_dfcs);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  lane_start = 0x%x\n", Dfc->lane_start);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  lane_width = 0x%x\n", Dfc->lane_width);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  genspeed =   0x%x\n", Dfc->Device.dfcubm.genspeed);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  event =    0x%x\n", Dfc->event);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  type  =      0x%x\n  bifurcate_port = 0x%x\n  secondary_port = 0x%x\n",
            Dfc->Device.dfcubm.type, Dfc->Device.dfcubm.type, Dfc->Device.dfcubm.type);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  features =   0x%x\n", Dfc->Device.dfcubm.dfcFeats);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  OCP Host = 0x%x\n", Dfc->Device.dfcocp.host);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  OCP Socket = 0x%x\n", Dfc->Device.dfcocp.socket);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "  slot =       0x%x\n", Dfc->Device.dfcubm.slot);
        if (DfcIndex == 0) {
          DfcCount = Dfc->num_dfcs;
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "DfcCount is %d\n", DfcCount);
          /*
		   * Add entries for this HFC
		   */
          EntryCount += Dfc->num_dfcs;
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "New EntryCount is %d\n", EntryCount);
          IncreaseTableSize (&ComplexTable, EntryCount, &Result);
          if (Result == false) {
            MPIO_TRACEPOINT (SIL_TRACE_ERROR, " IncreaseTableSize Returned ERROR!!!\n");
            return;
          }  else {
            MPIO_TRACEPOINT (SIL_TRACE_INFO, "IncreaseTableSize SUCCESS!\n", DfcCount);
          }

        }

		/*
		 * Add entry for this DFC
		 */
        StartLane = (HfcStartLanes[HfcIndex]) + (Dfc->lane_start);
        if (Dfc->lane_width == 0) {
          EndLane = StartLane;
        } else {
          EndLane = StartLane + (Dfc->lane_width) - 1;
        }
        ResetId = 0; // TBD - Unclear what is TBD here
        MPIO_TRACEPOINT (SIL_TRACE_INFO, " Calling InitializeTopologyEntry for DfcIndex = \n", DfcIndex);
        if (HfcDescriptor->HfcPortList[HfcIndex].NodeType == MPIO_I2C_NODE_TYPE_UBM) {
          MPIO_TRACEPOINT (SIL_TRACE_INFO, " InitializeTopologyEntry for MPIO_I2C_NODE_TYPE_UBM\n");
          if ((Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_QUAD_PCI) ||
             (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_EMPTY)) {
            InitializeTopologyEntry (&ComplexTable, MpioPcieEngine,
                       StartLane, EndLane, ResetId, 0, 0, &Result, &EntryHandle);
          }

          if (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_SATA_SAS) {
            InitializeTopologyEntry (&ComplexTable, MpioSATAEngine,
                                     StartLane, EndLane, ResetId, 0, 0, &Result, &EntryHandle);
          }
        }
        if ((HfcDescriptor->HfcPortList[HfcIndex].NodeType == MPIO_I2C_NODE_TYPE_OCP) &&
            ((Dfc->event == BIOS_EVENT_DEVICE_CONNECTED ) || (Dfc->event == BIOS_EVENT_DEVICE_NOT_PRESENT) ||
             (Dfc->event == BIOS_EVENT_DEVICE_DISCONNECTED))) {
             if (StartLane != EndLane)  {
               MPIO_TRACEPOINT (SIL_TRACE_INFO, " InitializeTopologyEntry for MPIO_I2C_NODE_TYPE_OCP\n");
               InitializeTopologyEntry (&ComplexTable, MpioPcieEngine,
                          StartLane, EndLane, ResetId, 0, 0, &Result, &EntryHandle);
             }
        }
        if (Result == false) {
          MPIO_TRACEPOINT (SIL_TRACE_ERROR, " InitializeTopologyEntry Returned ERROR!!!\n");
          return;
        }
        /*
        * ubm data
        */
        if (HfcDescriptor->HfcPortList[HfcIndex].NodeType == MPIO_I2C_NODE_TYPE_UBM) {
          MPIO_TRACEPOINT (SIL_TRACE_INFO, " Setting attributes for MPIO_I2C_NODE_TYPE_UBM\n");
          if (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_EMPTY) {
            PortParam.ParamType = MPIO_PP_PORT_PRESENT;
            PortParam.ParamValue = false;
            AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
          }
          if (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_QUAD_PCI) {
            PortParam.ParamType = MPIO_PP_PORT_PRESENT;
            PortParam.ParamValue = true;
            AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
          }
          if (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_SATA_SAS) {
            PortParam.ParamType = MPIO_PP_PORT_PRESENT;
            PortParam.ParamValue = true;
            AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
          }
          /*
          * Add parameters to Topology entry
          */
          if ((Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_QUAD_PCI) ||
            (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_EMPTY)) {
              if (HfcDescriptor->HfcToDfcData[HfcIndex].NpemEnable) {
                PortParam.ParamType = MPIO_PP_NPEM_ENABLE;
                PortParam.ParamValue = HfcDescriptor->HfcToDfcData[HfcIndex].NpemEnable;
                AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
                PortParam.ParamType = MPIO_PP_NPEM_CAPABILITES;
                PortParam.ParamValue = HfcDescriptor->HfcToDfcData[HfcIndex].NpemCap;
                AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              }
              PortParam.ParamType = MPIO_PP_HOTPLUG_TYPE;
              PortParam.ParamValue = PcieHotplugUBM;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_UBM_HFC_INDEX;
              PortParam.ParamValue = Dfc->hfc_idx;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_UBM_DFC_INDEX;
              PortParam.ParamValue = (uint16_t) DfcIndex;
              MPIO_TRACEPOINT (SIL_TRACE_INFO, " HFC Index %x, DFC Index %x\n", HfcIndex, PortParam.ParamValue);
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_SLOT_NUM;
              PortParam.ParamValue = (uint16_t) Dfc->Device.dfcubm.slot;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_DFC_EVENT;
              PortParam.ParamValue = (uint16_t) Dfc->event;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
          }

          if (Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_SATA_SAS) {
              PortParam.ParamType = MPIO_PP_HOTPLUG_TYPE;
              PortParam.ParamValue = PcieHotplugUBM;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_UBM_HFC_INDEX;
              PortParam.ParamValue = Dfc->hfc_idx;
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
              PortParam.ParamType = MPIO_PP_UBM_DFC_INDEX;
              PortParam.ParamValue = (uint16_t) DfcIndex;
              MPIO_TRACEPOINT (SIL_TRACE_INFO, " HFC Index %x, DFC Index %x\n", HfcIndex, PortParam.ParamValue);
              AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
          }

          if (HfcDescriptor->HfcToDfcData[HfcIndex].AncData.Count != 0 && EntryHandle != 0) {
            UbmAncDataConfig (&ComplexTable, EntryHandle,
              (MPIO_ANC_DATA *)&(HfcDescriptor->HfcToDfcData[HfcIndex].AncData));
          }
        }
        /*
        * OCP Data
        */
        if ((HfcDescriptor->HfcPortList[HfcIndex].NodeType == MPIO_I2C_NODE_TYPE_OCP) &&
            ((Dfc->event == BIOS_EVENT_DEVICE_CONNECTED ) || (Dfc->event == BIOS_EVENT_DEVICE_NOT_PRESENT) ||
             (Dfc->event == BIOS_EVENT_DEVICE_DISCONNECTED))) {
             if (StartLane != EndLane)  {
               MPIO_TRACEPOINT (SIL_TRACE_INFO, " Setting attributes for MPIO_I2C_NODE_TYPE_OCP\n");
               if (Dfc->event == BIOS_EVENT_DEVICE_NOT_PRESENT) {
                 MPIO_TRACEPOINT (SIL_TRACE_INFO, " Setting PP_PORT_PRESENT = true\n");
                 PortParam.ParamType = MPIO_PP_PORT_PRESENT;
                 PortParam.ParamValue = true;
                 AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               }
               if ((Dfc->event == BIOS_EVENT_DEVICE_CONNECTED ) || (Dfc->event == BIOS_EVENT_DEVICE_DISCONNECTED)) {
                 MPIO_TRACEPOINT (SIL_TRACE_INFO, " Setting PP_PORT_PRESENT = true\n");
                 PortParam.ParamType = MPIO_PP_PORT_PRESENT;
                 PortParam.ParamValue = true;
                 AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               }
              /*
              * Add parameters to Topology entry
              */
               if (HfcDescriptor->HfcToDfcData[HfcIndex].NpemEnable) {
                 PortParam.ParamType = MPIO_PP_NPEM_ENABLE;
                 PortParam.ParamValue = HfcDescriptor->HfcToDfcData[HfcIndex].NpemEnable;
                 AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
                 PortParam.ParamType = MPIO_PP_NPEM_CAPABILITES;
                 PortParam.ParamValue = HfcDescriptor->HfcToDfcData[HfcIndex].NpemCap;
                 AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               }
               PortParam.ParamType = MPIO_PP_HOTPLUG_TYPE;
               PortParam.ParamValue = PcieHotplugOCP;
               AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               PortParam.ParamType = MPIO_PP_DFC_EVENT;
               PortParam.ParamValue = (uint16_t) Dfc->event;
               AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               PortParam.ParamType = MPIO_PP_UBM_HFC_INDEX;
               PortParam.ParamValue = Dfc->hfc_idx;
               AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               PortParam.ParamType = MPIO_PP_UBM_DFC_INDEX;
               PortParam.ParamValue = (uint16_t) DfcIndex;
               AddParameter (&ComplexTable, EntryHandle, PortParam, &Result);
               MPIO_TRACEPOINT (SIL_TRACE_INFO, " HFC Index %x, DFC Index %x\n", HfcIndex, PortParam.ParamValue);
               if (HfcDescriptor->HfcToDfcData[HfcIndex].AncData.Count != 0 && EntryHandle != 0) {
                 UbmAncDataConfig (&ComplexTable, EntryHandle,
                  (MPIO_ANC_DATA *)&(HfcDescriptor->HfcToDfcData[HfcIndex].AncData));
               }
             }
        }
        DfcIndex++;
      }
    } while (DfcIndex < DfcCount);
  }
  /*
   * Update address of Complex to caller
   */
  *ComplexDescriptor = TempComplexTable;
  return;
}

/**--------------------------------------------------------------------
 *
 * AddHfcDescriptor
 *
 * @brief Helper function to dump input configuration to user engine descriptor
 *
 * @param [in] EngineDescriptor  tbd
 * @param [in] HfcDescriptor     tbd
 * @param [in] HfcStartLanes     tbd
 * @param [in] HfcListSize      tbd
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
AddHfcDescriptor (
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor,
  MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor,
  uint8_t                     *HfcStartLanes,
  uint8_t                     *HfcListSize
  )
{
  MPIO_PORT_PARAM      *PortParam;
  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Verify input parameters
   */
  if ((HfcDescriptor == NULL) || (HfcStartLanes == NULL) || (HfcListSize == NULL)) {
    return;
  }

  HfcDescriptor->Size += sizeof (MPIO_HFC_PORT);
  HfcStartLanes[*HfcListSize] = (uint8_t)EngineDescriptor->EngineData.StartLane;
  if (EngineDescriptor->EngineData.EngineType == MpioOCP3Engine) {
    HfcDescriptor->HfcPortList[*HfcListSize].NodeType = MPIO_I2C_NODE_TYPE_OCP;
  } else if (EngineDescriptor->EngineData.EngineType == MpioUBMHFCEngine) {
    HfcDescriptor->HfcPortList[*HfcListSize].NodeType = MPIO_I2C_NODE_TYPE_UBM;
  }
  HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Expander.Addr = UBM_INVALID_SWITCH_ADDR;
  HfcDescriptor->HfcPortList[*HfcListSize].Device.Ubm.Bp_Type = UBM_GPIO_NOT_USED;
  HfcDescriptor->HfcPortList[*HfcListSize].Device.Ubm.I2c_Reset = UBM_GPIO_NOT_USED;

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Adding this entry\n");
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  HfcListSize = 0x%x\n", *HfcListSize);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  StartLane   = 0x%x\n", HfcStartLanes[*HfcListSize]);

  PortParam = (MPIO_PORT_PARAM *) &(EngineDescriptor->PortParams);

  /*
   * Process port parameters until a an empty is found
   */
  while (PortParam->ParamType != 0) {
    switch (PortParam->ParamType) {
    case MPIO_PP_I2C_EXPANDER_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Type = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_I2C_EXPANDER_ADDRESS:
      HfcDescriptor->HfcPortList[*HfcListSize].Expander.Addr = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_GPIOx_BP_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ubm.Bp_Type = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_GPIOx_I2C_RESET:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ubm.I2c_Reset = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_START_LANE:
      HfcDescriptor->HfcPortList[*HfcListSize].Start_Lane = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_SLOT_NUM:
      if (EngineDescriptor->EngineData.EngineType == MpioUBMHFCEngine) {
        HfcDescriptor->HfcPortList[*HfcListSize].Device.Ubm.SlotNum = (uint8_t) (PortParam->ParamValue);
      } else if (EngineDescriptor->EngineData.EngineType == MpioOCP3Engine) {
        HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.SlotNum = (uint8_t) (PortParam->ParamValue);
      }
      break;
    case MPIO_PP_UBM_SWITCH0_ADDR:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Addr = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_UBM_SWITCH0_SELECT:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Select = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_UBM_SWITCH0_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[0].Type = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_UBM_SWITCH1_ADDR:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Addr = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_UBM_SWITCH1_SELECT:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Select = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_UBM_SWITCH1_TYPE:
      HfcDescriptor->HfcPortList[*HfcListSize].Switch[1].Type = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_PRESENT_START:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpPresentStart = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_NUM_HOSTS:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpNumHosts = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_NUM_SOCKETS:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpNumSockets = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_BIF_PRIMARY:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpForm.Field.OcpBifPrim = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_BIF_SECONDARY:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpForm.Field.OcpBifSec = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_FORM_FACTOR:
      HfcDescriptor->HfcPortList[*HfcListSize].Device.Ocp.OcpForm.Field.OcpFormFactor =
      (uint8_t) (PortParam->ParamValue);
      break;
    /*
    * Populate additional data that needs to propagate to DFC entries
    */
    case MPIO_PP_NPEM_ENABLE:
      HfcDescriptor->HfcToDfcData[*HfcListSize].NpemEnable = (uint16_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_NPEM_CAPABILITES:
      HfcDescriptor->HfcToDfcData[*HfcListSize].NpemCap = (uint16_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_DEF_VALID:
      HfcDescriptor->HfcToDfcData[*HfcListSize].OcpDefValid = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_DEF_PRSNTB_PRIMARY:
      HfcDescriptor->HfcToDfcData[*HfcListSize].OcpDefPrimPrsntb = (uint8_t) (PortParam->ParamValue);
      break;
    case MPIO_PP_OCP_DEF_PRSNTB_SECONDARY:
      HfcDescriptor->HfcToDfcData[*HfcListSize].OcpDefSecPrsntb = (uint8_t) (PortParam->ParamValue);
      break;
    default:
      break;
    }
    PortParam++;
  }

  /*
  * Ancillary data for UBM
  */
  if (EngineDescriptor->AncData.Count != 0) {
    HfcDescriptor->HfcToDfcData[*HfcListSize].AncData.Count = EngineDescriptor->AncData.Count;
    HfcDescriptor->HfcToDfcData[*HfcListSize].AncData.Ovrd = EngineDescriptor->AncData.Ovrd;
  }

  (*HfcListSize)++;
}

/**--------------------------------------------------------------------
 *
 * ParseTopologyForUbm
 *
 * @brief Callback for Topology Management
 *
 * @param[in]  SilDate             Pointer to MPIO input block data
 * @param[in]  ComplexDescriptor   Pointer to user defined complex descriptor
 * @param[in]  Pcie                Pcie Platform Configuration descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
ParseTopologyForUbm (
  MPIOCLASS_INPUT_BLK       *SilData,
  MPIO_COMPLEX_DESCRIPTOR   **ComplexDescriptor,
  PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;
  size_t                    Index, Socket1Index;
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology;
  MPIO_UBM_HFC_DESCRIPTOR   *HfcDescriptor;
  uint8_t                   HfcStartLanes[32];
  uint8_t                   HfcListSize;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  PlatformTopology = *ComplexDescriptor;
  Index = 0;
  Socket1Index = 0xFF;

  HfcDescriptor = &SilData->HfcDescriptor;
  memset ((void*)HfcDescriptor, 0, sizeof(MPIO_UBM_HFC_DESCRIPTOR));
  HfcDescriptor->Size = offsetof (MPIO_UBM_HFC_DESCRIPTOR, HfcPortList);

  HfcListSize = 0;
  while (PlatformTopology != NULL) {
    EngineDescriptor = PlatformTopology->PciePortList;
    while (EngineDescriptor != NULL) {

      if (EngineDescriptor->EngineData.EngineType == MpioUBMHFCEngine) {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "Adding descriptor to list\n");
        AddHfcDescriptor (EngineDescriptor, HfcDescriptor, &HfcStartLanes[0], &HfcListSize);

        EngineDescriptor->EngineData.EngineType = MpioUnusedEngine;
        Index++;
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
    }
    PlatformTopology = PcieConfigGetNextDataDescriptor (PlatformTopology);
    if (PlatformTopology != NULL) {
      Socket1Index = Index;
    }
  }
  if ((HfcDescriptor != NULL) && (HfcListSize != 0)) {
    /*
	 * Send HfcDescriptor to MPIO and get DFC topology back
	 */
    HfcDescriptor->Size = (uint32_t) HfcListSize;
    MpioUbmDiscovery (ComplexDescriptor, HfcDescriptor, HfcStartLanes, HfcListSize, Socket1Index, Pcie);
  }
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}
