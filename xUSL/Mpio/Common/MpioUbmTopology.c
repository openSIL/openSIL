/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioUbmTopology.c
 * @brief Expand host-wired UBM connectors in the fixed, packed port array.
 */
#include <string.h>
#include "MpioLib.h"

#define MAX_UBM_HFCS 32
#define MAX_UBM_DFCS 16

typedef struct {
  size_t Port;
  size_t Complex;
} UBM_LOCATION;

/* Reject non-packed lists before doing pointer arithmetic or changing ports. */
static SIL_STATUS
UbmCountPorts (
  PCIe_PLATFORM_TOPOLOGY *Topology,
  size_t *Used,
  size_t *ComplexCount,
  UBM_LOCATION *Locations,
  size_t *HfcCount
  )
{
  size_t Socket, Index, Other;
  MPIO_COMPLEX_DESCRIPTOR *Complex;
  MPIO_PORT_DESCRIPTOR *Port;
  bool Terminated;

  *Used = *ComplexCount = *HfcCount = 0;
  for (Socket = 0; Socket < PROJ_MAX_SOCKETS_SUPPORTED; Socket++) {
    Complex = &Topology->PlatformData[Socket];
    for (Other = 0; Other < Socket; Other++) {
      if (Complex->SocketId == Topology->PlatformData[Other].SocketId) {
        return SilInvalidParameter;
      }
    }
    if (Complex->PciePortList != NULL) {
      if (*Used == MAX_PORTS_SUPPORTED ||
        Complex->PciePortList != &Topology->PortList[*Used]) {
        return SilInvalidParameter;
      }
      Terminated = false;
      for (Index = *Used; Index < MAX_PORTS_SUPPORTED; Index++) {
        Port = &Topology->PortList[Index];
        if (Port->EngineData.EngineType == MpioUBMHFCEngine ||
          Port->EngineData.EngineType == MpioOCP3Engine) {
          if (*HfcCount == MAX_UBM_HFCS) {
            return SilOutOfResources;
          }
          if (Port->EngineData.StartLane > Port->EngineData.EndLane ||
            Port->EngineData.EndLane - Port->EngineData.StartLane >= MAX_UBM_DFCS) {
            return SilInvalidParameter;
          }
          Locations[*HfcCount].Port = Index;
          Locations[*HfcCount].Complex = Socket;
          ++*HfcCount;
        }
        if (Port->Flags & DESCRIPTOR_TERMINATE_LIST) {
          *Used = Index + 1;
          Terminated = true;
          break;
        }
      }
      if (!Terminated) {
        return SilOutOfBounds;
      }
    }
    *ComplexCount = Socket + 1;
    if (Complex->Flags & DESCRIPTOR_TERMINATE_LIST) {
      return SilPass;
    }
  }
  return SilOutOfBounds;
}

/* Only the UBM wiring fields are consumed here. Port policy is inherited. */
static SIL_STATUS
UbmHfc (
  const MPIO_PORT_DESCRIPTOR *Port,
  MPIO_HFC_PORT *Hfc,
  HFC_TO_DFC_DATA *Policy
  )
{
  size_t Index;
  uint16_t Type, Value;
  bool Terminated = false;
  bool Ocp = Port->EngineData.EngineType == MpioOCP3Engine;

  memset(Hfc, 0, sizeof (*Hfc));
  Hfc->NodeType = Ocp ? MPIO_I2C_NODE_TYPE_OCP : MPIO_I2C_NODE_TYPE_UBM;
  Hfc->Start_Lane = Port->EngineData.StartLane;
  if (!Ocp) {
    Hfc->Device.Ubm.Bp_Type = UBM_GPIO_NOT_USED;
    Hfc->Device.Ubm.I2c_Reset = UBM_GPIO_NOT_USED;
  }
  Hfc->Device.Ubm.SlotNum = Port->Port.SlotNum;
  Policy->OcpSlotNum = Port->Port.SlotNum;
  for (Index = 0; Index < PCIE_PORT_PARAMETER_COUNT; Index++) {
    Type = Port->PortParams.PhyParam[Index].ParamType;
    Value = Port->PortParams.PhyParam[Index].ParamValue;
    if (Type == 0) {
      Terminated = true;
      break;
    }
    switch (Type) {
    case MPIO_PP_OCP_PRESENT_START:
      if (!Ocp || Value > 12) {
        return SilInvalidParameter;
      }
      Hfc->Device.Ocp.OcpPresentStart = (uint8_t) Value;
      break;
    case MPIO_PP_OCP_NUM_HOSTS:
    case MPIO_PP_OCP_NUM_SOCKETS:
      /* Cross-host/socket routing needs separate endpoint ownership support. */
      if (!Ocp || Value != 1) {
        return SilUnsupported;
      }
      if (Type == MPIO_PP_OCP_NUM_HOSTS) {
        Hfc->Device.Ocp.OcpNumHosts = Value;
      } else {
        Hfc->Device.Ocp.OcpNumSockets = Value;
      }
      break;
    case MPIO_PP_OCP_BIF_PRIMARY:
    case MPIO_PP_OCP_BIF_SECONDARY:
      if (!Ocp || Value > 7) {
        return SilInvalidParameter;
      }
      if (Type == MPIO_PP_OCP_BIF_PRIMARY) {
        Hfc->Device.Ocp.OcpForm.Field.OcpBifPrim = Value;
      } else {
        Hfc->Device.Ocp.OcpForm.Field.OcpBifSec = Value;
      }
      break;
    case MPIO_PP_OCP_FORM_FACTOR:
      if (!Ocp || Value > 1) {
        return SilInvalidParameter;
      }
      Hfc->Device.Ocp.OcpForm.Field.OcpFormFactor = Value;
      break;
    case MPIO_PP_OCP_DEF_VALID:
      if (!Ocp || Value > 1) {
        return SilInvalidParameter;
      }
      Policy->OcpDefValid = Value;
      break;
    case MPIO_PP_OCP_DEF_PRSNTB_PRIMARY:
    case MPIO_PP_OCP_DEF_PRSNTB_SECONDARY:
      if (!Ocp || Value > 15) {
        return SilInvalidParameter;
      }
      if (Type == MPIO_PP_OCP_DEF_PRSNTB_PRIMARY) {
        Policy->OcpDefPrimPrsntb = Value;
      } else {
        Policy->OcpDefSecPrsntb = Value;
      }
      break;
    case MPIO_PP_I2C_EXPANDER_ADDRESS:
      if (Value > 0x7f) {
        return SilInvalidParameter;
      }
      Hfc->Expander.Addr = (uint8_t) Value;
      break;
    case MPIO_PP_I2C_EXPANDER_TYPE:
      if (Value > 2) {
        return SilInvalidParameter;
      }
      Hfc->Expander.Type = (uint8_t) Value;
      break;
    case MPIO_PP_I2C_CLEAR_ALL_INTS:
      if (Value > 1) {
        return SilInvalidParameter;
      }
      Hfc->Expander.ClrAllInts = (uint8_t) Value;
      break;
    case MPIO_PP_GPIOx_BP_TYPE:
    case MPIO_PP_GPIOx_I2C_RESET:
      if (Ocp) {
        return SilInvalidParameter;
      }
      if (Value != UBM_GPIO_NOT_USED && Value > 15) {
        return SilInvalidParameter;
      }
      if (Type == MPIO_PP_GPIOx_BP_TYPE) {
        Hfc->Device.Ubm.Bp_Type = (uint8_t) Value;
      } else {
        Hfc->Device.Ubm.I2c_Reset = (uint8_t) Value;
      }
      break;
    case MPIO_PP_START_LANE:
      if (Value != Hfc->Start_Lane) {
        return SilInvalidParameter;
      }
      break;
    case MPIO_PP_SLOT_NUM:
      if (Value > 255) {
        return SilInvalidParameter;
      }
      Hfc->Device.Ubm.SlotNum = (uint8_t) Value;
      break;
    case MPIO_PP_UBM_SWITCH0_ADDR:
    case MPIO_PP_UBM_SWITCH1_ADDR:
      if (Value != 0 && (Value < 0x70 || Value > 0x77)) {
        return SilInvalidParameter;
      }
      Hfc->Switch[Type == MPIO_PP_UBM_SWITCH1_ADDR].Addr = (uint8_t) Value;
      break;
    case MPIO_PP_UBM_SWITCH0_SELECT:
    case MPIO_PP_UBM_SWITCH1_SELECT:
      if (Value > 3) {
        return SilInvalidParameter;
      }
      Hfc->Switch[Type == MPIO_PP_UBM_SWITCH1_SELECT].Select = (uint8_t) Value;
      break;
    case MPIO_PP_UBM_SWITCH0_TYPE:
    case MPIO_PP_UBM_SWITCH1_TYPE:
      if (Value > 1) {
        return SilInvalidParameter;
      }
      Hfc->Switch[Type == MPIO_PP_UBM_SWITCH1_TYPE].Type = (uint8_t) Value;
      break;
    default:
      /* Port policy belongs in Port/Phy/AncData, not HFC wiring parameters. */
      return SilUnsupported;
    }
  }
  if (Ocp && (!Hfc->Expander.Addr || Hfc->Device.Ocp.OcpNumHosts != 1 ||
    Hfc->Device.Ocp.OcpNumSockets != 1)) {
    return SilInvalidParameter;
  }
  return Terminated && Hfc->Switch[0].Addr != 0 ? SilPass : SilInvalidParameter;
}

static SIL_STATUS
UbmReadDfc (
  PCI_ADDR Address,
  size_t Hfc,
  const MPIO_PORT_DESCRIPTOR *Template,
  bool Ocp,
  const HFC_TO_DFC_DATA *Policy,
  DFC_DESCRIPTOR *Dfcs,
  size_t *Count
  )
{
  uint32_t Args[6], Response, Occupied = 0, Lanes;
  size_t Index, Width = Template->EngineData.EndLane - Template->EngineData.StartLane + 1;
  DFC_DESCRIPTOR *Dfc;

  *Count = 1;
  for (Index = 0; Index < *Count; Index++) {
    memset(Args, 0, sizeof (Args));
    Args[0] = (uint32_t) Hfc;
    Args[1] = (uint32_t) Index;
    if (Ocp && Index == 0 && Policy->OcpDefValid) {
      Args[2] = (1u << 31) | ((uint32_t) Policy->OcpDefSecPrsntb << 4) |
        Policy->OcpDefPrimPrsntb;
    }
    Response = MpioServiceRequestCommon(Address, BIOS_MPIO_MSG_I2C_DEVICE_GET, Args, 0);
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "UBM HFC %u DFC %u response %x data %x %x %x %x\n",
      (unsigned) Hfc,
      (unsigned) Index,
      Response,
      Args[0],
      Args[1],
      Args[2],
      Args[3]
      );
    /* Zero is not success: stale request arguments must not become an endpoint. */
    if ((Response & 0xff) != BIOSSMC_Result_OK) {
      return SilDeviceError;
    }
    Dfc = &Dfcs[Index];
    memcpy(Dfc, &Args[1], sizeof (*Dfc));
    if (Dfc->hfc_idx != Hfc || Dfc->num_dfcs > MAX_UBM_DFCS) {
      return SilOutOfBounds;
    }
    if (Index == 0) {
      *Count = Dfc->num_dfcs;
    }
    if (*Count == 0) {
      return SilPass;
    }
    if (Ocp && (Dfc->Device.dfcocp.host != 0 || Dfc->Device.dfcocp.socket != 0 ||
      *Count > 256u - Policy->OcpSlotNum)) {
      return SilUnsupported;
    }
    if (Dfc->num_dfcs != *Count ||
      (Dfc->event != BIOS_EVENT_DEVICE_CONNECTED && Dfc->event != BIOS_EVENT_DEVICE_NOT_PRESENT)) {
      return SilInvalidParameter;
    }
    if (Dfc->lane_width == 0 && Dfc->event == BIOS_EVENT_DEVICE_NOT_PRESENT &&
      (Ocp || Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_EMPTY)) {
      /* The empty descriptor uses lane_start + DFC index for an empty zero-width
       * connector. It is a disabled placeholder, not an occupied lane.
       * Normalize only this case; connected zero-width responses still fail.
       */
      if (Dfc->lane_start >= Width || Index >= Width - Dfc->lane_start) {
        return SilOutOfBounds;
      }
      Dfc->lane_start += (uint8_t) Index;
      Dfc->lane_width = 1;
      continue;
    }
    if (Dfc->lane_width == 0 || Dfc->lane_width > Width ||
      Dfc->lane_start >= Width || Dfc->lane_width > Width - Dfc->lane_start ||
      (Dfc->lane_width & (Dfc->lane_width - 1)) != 0) {
      return SilOutOfBounds;
    }
    if (!Ocp && ((Dfc->Device.dfcubm.type != BIOS_DFC_INFO_TYPE_QUAD_PCI &&
      Dfc->Device.dfcubm.type != BIOS_DFC_INFO_TYPE_SATA_SAS &&
      Dfc->Device.dfcubm.type != BIOS_DFC_INFO_TYPE_EMPTY) ||
      (Dfc->event == BIOS_EVENT_DEVICE_CONNECTED && Dfc->Device.dfcubm.type == BIOS_DFC_INFO_TYPE_EMPTY) ||
      (Dfc->event == BIOS_EVENT_DEVICE_NOT_PRESENT && Dfc->Device.dfcubm.type != BIOS_DFC_INFO_TYPE_EMPTY))) {
      return SilInvalidParameter;
    }
    Lanes = ((1u << Dfc->lane_width) - 1) << Dfc->lane_start;
    if (Occupied & Lanes) {
      return SilInvalidParameter;
    }
    Occupied |= Lanes;
  }
  return SilPass;
}

/**
 * Called after SMU/NBIO setup, before MpioCfgBeforeDxioInit and ASK parsing,
 * at the pre-DXIO UBM hook. The mailbox service waits for
 * firmware readiness. The host's input block must be firmware-addressable.
 *
 * Opt-in is one-shot. Hosts which already expanded UBM leave it off.
 * Mailbox/malformed-response failure leaves that HFC unused and continues other
 * HFCs. No failed HFC contributes a PCIe/SATA endpoint.
 */
SIL_STATUS
MpioDiscoverUbm (
  MPIOCLASS_INPUT_BLK *SilData,
  PCI_ADDR Address
  )
{
  PCIe_PLATFORM_TOPOLOGY *Topology;
  MPIO_UBM_HFC_DESCRIPTOR *Hfc;
  UBM_LOCATION Locations[MAX_UBM_HFCS];
  DFC_DESCRIPTOR Dfcs[MAX_UBM_DFCS];
  MPIO_PORT_DESCRIPTOR Template, *Port;
  size_t Used, ComplexCount, HfcCount, Index, Other, Count, Extra, Offset, Socket, Start, End;
  uintptr_t Buffer;
  uint32_t Args[6], Flags;
  SIL_STATUS Status;
  bool HasExpander = false;

  if (SilData == NULL) {
    return SilInvalidParameter;
  }
  if (!SilData->DiscoverUbm) {
    return SilPass;
  }
  SilData->DiscoverUbm = false;
  Topology = &SilData->PcieTopologyData;
  Status = UbmCountPorts(Topology, &Used, &ComplexCount, Locations, &HfcCount);
  if (Status != SilPass || HfcCount == 0) {
    return Status;
  }

  /* HFC ranges must not overlap existing endpoints or another HFC. */
  for (Index = 0; Index < HfcCount; Index++) {
    Port = &Topology->PortList[Locations[Index].Port];
    Socket = Locations[Index].Complex;
    Start = Topology->PlatformData[Socket].PciePortList - Topology->PortList;
    End = Start;
    while (!(Topology->PortList[End++].Flags & DESCRIPTOR_TERMINATE_LIST)) { }
    for (Other = Start; Other < End; Other++) {
      const MPIO_ENGINE_DATA *Engine = &Topology->PortList[Other].EngineData;
      size_t Low = Engine->StartLane < Engine->EndLane ? Engine->StartLane : Engine->EndLane;
      size_t High = Engine->StartLane > Engine->EndLane ? Engine->StartLane : Engine->EndLane;
      if (Other != Locations[Index].Port && Engine->EngineType != MpioUnusedEngine &&
        Low <= Port->EngineData.EndLane && High >= Port->EngineData.StartLane) {
        return SilInvalidParameter;
      }
    }
  }

  Hfc = &SilData->HfcDescriptor;
  /* The firmware-facing HFC list needs aligned, firmware-addressable storage
   * that remains valid for the input block's lifetime. The packed descriptor
   * list does not provide the required alignment. */
  Buffer = ((uintptr_t) SilData->UbmHfcDmaBuffer + 15) & ~(uintptr_t) 15;
  if (Buffer > UINT32_MAX || HfcCount * sizeof (MPIO_HFC_PORT) - 1 > UINT32_MAX - Buffer) {
    return SilOutOfBounds;
  }
  memset(Hfc, 0, sizeof (*Hfc));
  Hfc->Size = (uint32_t) (offsetof(MPIO_UBM_HFC_DESCRIPTOR, HfcPortList) +
    HfcCount * sizeof (MPIO_HFC_PORT));
  for (Index = 0; Index < HfcCount; Index++) {
    Status = UbmHfc(&Topology->PortList[Locations[Index].Port],
      &Hfc->HfcPortList[Index],
      &Hfc->HfcToDfcData[Index]
      );
    if (Status != SilPass) {
      return Status;
    }
    HasExpander |= Hfc->HfcPortList[Index].Expander.Addr != 0;
  }
  /* The firmware interface requires a switch and expander in the global list. */
  if (!HasExpander) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "UBM topology has no I2C expander\n");
    return SilInvalidParameter;
  }
  /* Unknown engine types are never presented to later topology parsing. */
  for (Index = 0; Index < HfcCount; Index++) {
    Topology->PortList[Locations[Index].Port].EngineData.EngineType = MpioUnusedEngine;
  }
  memset(Args, 0, sizeof (Args));
  memcpy((void *) Buffer, Hfc->HfcPortList, HfcCount * sizeof (MPIO_HFC_PORT));
  /* Distinguish mailbox transport from an I2C enumeration-specific stall.
   * Use the same standard status query as SendAsk's WaitReady path. */
  {
    uint32_t Response = MpioServiceRequestCommon(Address, MPIO_GET_STATUS, Args, 0);
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "UBM preflight response %x command %x cycle %x POST %x firmware %x\n",
      Response,
      Args[0],
      Args[1],
      Args[2],
      Args[3]
      );
    if ((Response & 0xff) != BIOSSMC_Result_OK) {
      MPIO_TRACEPOINT(SIL_TRACE_ERROR, "UBM status query failed; no discovered ports\n");
      return SilPass;
    }
    memset(Args, 0, sizeof (Args));
  }

  Args[0] = (uint32_t) Buffer;
  Args[1] = (uint32_t) HfcCount;
  if ((MpioServiceRequestCommon(Address, BIOS_MPIO_MSG_I2C_ENUMERATE, Args, 0) & 0xff) != BIOSSMC_Result_OK) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "UBM enumeration failed; no discovered ports\n");
    return SilPass;
  }

  for (Index = 0; Index < HfcCount; Index++) {
    Offset = Locations[Index].Port;
    Template = Topology->PortList[Offset];
    bool Ocp = Hfc->HfcPortList[Index].NodeType == MPIO_I2C_NODE_TYPE_OCP;
    Status = UbmReadDfc(Address,
      Index,
      &Template,
      Ocp,
      &Hfc->HfcToDfcData[Index],
      Dfcs,
      &Count
      );
    if (Status != SilPass) {
      MPIO_TRACEPOINT(SIL_TRACE_ERROR, "UBM HFC %u rejected: %u\n", (unsigned) Index, Status);
      continue;
    }
    if (Count == 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "UBM HFC %u: no connectors\n", (unsigned) Index);
      continue;
    }
    Extra = Count - 1;
    if (Extra > MAX_PORTS_SUPPORTED - Used) {
      MPIO_TRACEPOINT(SIL_TRACE_ERROR, "UBM HFC %u: port capacity exhausted\n", (unsigned) Index);
      continue;
    }
    /* Replace this HFC in place; shift every later socket as one packed tail. */
    memmove(&Topology->PortList[Offset + Count],
      &Topology->PortList[Offset + 1],
      (Used - Offset - 1) * sizeof (*Port)
      );
    for (Other = Locations[Index].Complex + 1; Other < ComplexCount; Other++) {
      if (Topology->PlatformData[Other].PciePortList != NULL) {
        Topology->PlatformData[Other].PciePortList += Extra;
      }
    }
    for (Other = Index + 1; Other < HfcCount; Other++) { Locations[Other].Port += Extra;
    }
    Used += Extra;
    Flags = Template.Flags;
    for (Other = 0; Other < Count; Other++) {
      Port = &Topology->PortList[Offset + Other];
      *Port = Template;
      Port->Flags = Flags & ~DESCRIPTOR_TERMINATE_LIST;
      if (Other == Count - 1) {
        Port->Flags = Flags;
      }
      Port->EngineData.EngineType = !Ocp && Dfcs[Other].Device.dfcubm.type == BIOS_DFC_INFO_TYPE_SATA_SAS ?
        MpioSATAEngine : MpioPcieEngine;
      Port->EngineData.StartLane = Template.EngineData.StartLane + Dfcs[Other].lane_start;
      Port->EngineData.EndLane = Port->EngineData.StartLane + Dfcs[Other].lane_width - 1;
      Port->Port.PortPresent = Dfcs[Other].event == BIOS_EVENT_DEVICE_CONNECTED;
      Port->Port.LinkHotplug = Ocp ? PcieHotplugOCP : PcieHotplugUBM;
      Port->Port.DeviceNumber = Port->Port.FunctionNumber = 0;
      Port->Port.SlotNum = Ocp ? Hfc->HfcToDfcData[Index].OcpSlotNum + Other :
        Dfcs[Other].Device.dfcubm.slot;
      /* Keep host policy fields, but never reapply HFC start-lane/slot parameters. */
      memset(&Port->PortParams, 0, sizeof (Port->PortParams));
      Port->PortParams.PhyParam[0] = (MPIO_PORT_PARAM) { MPIO_PP_UBM_HFC_INDEX, (uint16_t) Index };
      Port->PortParams.PhyParam[1] = (MPIO_PORT_PARAM) { MPIO_PP_UBM_DFC_INDEX, (uint16_t) Other };
      Port->PortParams.PhyParam[2] = (MPIO_PORT_PARAM) { MPIO_PP_DFC_EVENT, Dfcs[Other].event };
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "UBM HFC %u DFC %u: type %u lanes %u-%u present %u\n",
        (unsigned) Index,
        (unsigned) Other,
        Port->EngineData.EngineType,
        Port->EngineData.StartLane,
        Port->EngineData.EndLane,
        Port->Port.PortPresent
        );
    }
  }
  return SilPass;
}
