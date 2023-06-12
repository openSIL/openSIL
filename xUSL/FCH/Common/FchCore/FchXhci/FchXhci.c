/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhci.c
 * @brief FCH xHCI controller functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include "FchXhci.h"
#include <FCH/Common/Fch.h>
#include <FCH/FchClass-api.h>
#include <FCH/FchUsb-api.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <FCH/Common/FchCore/FchXhci/FchUsbCmn2Rev.h>
#include <SMU/SmuIp2Ip.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>

#define FCH_XHCI_MAX_PORT 0x10

static const FCHUSB_INPUT_BLK mFchUsbDefaults = {
  .DieBusNum = 0,
  .Xhci0Enable = false,
  .Xhci1Enable = false,
  .Xhci2Enable = false,
  .Xhci3Enable = false,
  .XhciECCDedErrRptEn = false,
  .Xhci0DevRemovable = 0,
  .DisableXhciPortLate = false,
  .XhciUsb3PortDisable = 0,
  .XhciUsb2PortDisable = 0,
  .XhciOCpinSelect[0] = {0xFFFFFFFF, 0xFFFF},
  .XhciOCpinSelect[1] = {0xFFFFFFFF, 0xFFFF},
  .XhciOCpinSelect[2] = {0xFFFFFFFF, 0xFFFF},
  .XhciOCpinSelect[3] = {0xFFFFFFFF, 0xFFFF},
  .XhciOcPolarityCfgLow = false,
  .Usb3PortForceGen1 = 0x0,
  .OemUsbConfigurationTable = 0,
  .UsbSparseModeEnable = true,
  .UsbDbgSCPipeSwitchEnable = false,
};


/**
 * FchXhciSmuService
 *
 * @brief Xhci1 SMU Service Request
 *
 * @param DieBusNum Bus Number on Current Die.
 * @param RequestId Request ID.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuService (
  uint32_t DieBusNum,
  uint32_t RequestId
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(false);
  }

  SmuApi->SmuServiceInitArguments(SmuArg);
  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);
  Status = SmuApi->SmuServiceRequest(PciAddress, RequestId, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchXhciSmuUsbConfigUpdate
 *
 * @brief Xhci Smu USB Config Update
 *
 * @details This routine is to send USB Configure Update message to SMU. SMU
 * holds USB register programming. This message is used to tell SMU which
 * register needs to be updated. It includes register information of address,
 * bitfield mask, programming value, register group.
 *
 * @param DieBusNum   Bus Number on Current Die.
 * @param SmnRegister Register input
 * @param SmnMask     Mask input
 * @param SmnData     Data input
 * @param SmnGroup    Group input
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  uint32_t DieBusNum,
  uint32_t SmnRegister,
  uint32_t SmnMask,
  uint32_t SmnData,
  uint32_t SmnGroup
  )
{
  SMC_RESULT          Status;
  PCI_ADDR            PciAddress;
  uint32_t            SmcMsgIdUsbConfigUpdate;
  uint32_t            SmuArg[6];
  SMU_IP2IP_API       *SmuApi;
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable(SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return SMC_Result_Failed;
  }

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(Status == SMC_Result_OK);
  }

  SmuApi->SmuServiceInitArguments(SmuArg);
  SmuArg[0] = SmnRegister;
  SmuArg[1] = SmnMask;
  SmuArg[2] = SmnData;
  SmuArg[3] = SmnGroup;
  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);
  SmcMsgIdUsbConfigUpdate = SIL_RESERVED_0293;
  Status = SmuApi->SmuServiceRequest(PciAddress, SmcMsgIdUsbConfigUpdate, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchUsb3PortForceGen1
 *
 * @brief Port Force Gen1
 *
 * @details This routine is to program internal USB register to force XHCI port
 * to limit to Gen1 speed.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] PortNum Usb3 Port count of the controller.
 * @param[in] UsbPortForceGen1 Parameter to set PortForceGen1. bit0 - port0, bit1 - port1, etc.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsb3PortForceGen1 (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t UsbPortForceGen1
  )
{
  uint32_t   Port;
  uint32_t   PortMask;
  uint32_t   DW0_Index;
  uint32_t   DW1_Mask;
  uint32_t   DW2_Data;
  uint32_t   DW3_Op_Group;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_INFO, "Controller UsbPortForceGen1 Parameter = 0x%x\n", UsbPortForceGen1);
  assert(PortNum <= FCH_XHCI_MAX_PORT);

  PortMask = GET_BITMASK(PortNum); //bit map mask of all ports. e.g. if controller has 2 ports, PortMask is 0x11b.
  UsbPortForceGen1 &= PortMask;

  DW0_Index = SIL_RESERVED_0276 + SIL_RESERVED_0350;
  DW1_Mask = PortMask << 16;
  DW2_Data = (uint32_t) (UsbPortForceGen1 << 16);
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    " UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  for (Port = 0; Port < PortNum; Port++) {
    DW0_Index = SIL_RESERVED_0339 + Port * 0x80;
    DW1_Mask = 1 << 10;
    DW2_Data = 0;
    DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup3);
    if (UsbPortForceGen1 & (1 << Port)) {
      DW2_Data = 1 << 10;
    }

    FchXhciSmuUsbConfigUpdate(DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
    FCH_TRACEPOINT(SIL_TRACE_INFO,
      " UsbConfigUpdate Message with %x, %x, %x, %x\n",
      DW0_Index,
      DW1_Mask,
      DW2_Data,
      DW3_Op_Group
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchXhciDbgSC
 *
 * @brief Enable or disable XHCI DbgSC feature
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 * @param[in] EnableValue Enable value of enabling Dbg SC feature.
 * @param[in] DisableValue Disable value of enabling Dbg SC feature.
 *
 */
void
FchXhciDbgSC (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData,
  uint32_t         EnableValue,
  uint32_t         DisableValue
  )
{
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciDbgSC on Controller %x to be set value 0x%x\n",
    UsbController,
    FchUsbData->UsbDbgSCPipeSwitchEnable ? EnableValue : DisableValue
    );

  FchXhciSmuUsbConfigUpdate(DieBusNum,
    0,
    0,
    0,
    FchUsbData->UsbDbgSCPipeSwitchEnable ? EnableValue : DisableValue
    );
}

/**
 * FchXhciSparseMode
 *
 * @brief Sparse Control Transaction Enable
 *
 * @details Some devices are slow in responding to Control transfers.
 * Scheduling multiple transactions in one microframe/frame can cause these
 * devices to misbehave. If enabled, the host controller schedules each phase
 * of a Control transfer in different microframes/frames. Please refer to PPR
 * for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 *
 */
void
FchXhciSparseMode (
  uint32_t           DieBusNum,
  uint32_t           UsbController,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  if (FchUsbData->UsbSparseModeEnable) {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0338,
      BIT_32(17),
      BIT_32(17),
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup3)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciSparseMode on Controller %x to be enabled\n", UsbController);
  } else {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0338,
      BIT_32(17),
      0,
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup3)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciSparseMode on Controller %x to be disabled\n", UsbController);
  }
}

/**
 * FchXhciOCPolarity
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to program Over-current input polarity configure.
 * By default, OC pin is low when OC occurs. Please refer to PPR for more
 * details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 *
 */
void
FchXhciOCPolarity (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  if (FchUsbData->XhciOcPolarityCfgLow) {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0276 + SIL_RESERVED_0350,
      BIT_32(8),
      BIT_32(8),
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchXhciOCPolarity set Polarity to Low\n");
  } else {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0276 + SIL_RESERVED_0350,
      BIT_32(8),
      0,
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchXhciOCPolarity set Polarity to High\n");
  }
}


/**
 * FchXhciRasFeature
 *
 * @brief Xhci RAS Control
 *
 * @details This routine is to enable ECC error reporting on XHCI controller.
 * Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 *
 */
void
FchXhciRasFeature (
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  if (FchUsbData->XhciECCDedErrRptEn) {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0276 + SIL_RESERVED_0348,
      BIT_32(12),
      BIT_32(12),
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciRasFeature Enable ECC_DedErrRptEn\n");
  } else {
    FchXhciSmuUsbConfigUpdate(DieBusNum,
      SIL_RESERVED_0276 + SIL_RESERVED_0348,
      BIT_32(12),
      0,
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciRasFeature disable ECC_DedErrRptEn\n");
  }
}

/**
 * FchXhciDeviceRemovable
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to program Device Removable register to indicate
 * if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not. Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] DeviceRemovable Parameter to set DeviceRemovable. bit[15:0] - USB2.0 ports, bit[31:16] - USB3 ports.
 */
void
FchXhciDeviceRemovable (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DeviceRemovable
  )
{
  FchXhciSmuUsbConfigUpdate(DieBusNum,
    SIL_RESERVED_0276 + SIL_RESERVED_0347,
    DeviceRemovable,
    DeviceRemovable,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciDeviceRemovable UsbConfigUpdate Message with %x, %x, %x, %x\n",
    SIL_RESERVED_0276 + SIL_RESERVED_0347,
    DeviceRemovable,
    DeviceRemovable,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
}

/**
 * FchXhciOverCurrent
 *
 * @brief Update Xhci OC Pin Mapping
 *
 * @details This routine is to program which Over-Current Pin is mapping to
 * each USB port. Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] PortNum Port count of the controller.
 * @param[in] OverCurrentMap Parameter to set OC Mapping.
 */
void
FchXhciOverCurrent (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t OverCurrentMap
  )
{
  uint32_t PortMask;

  PortMask = 0xFFFFFFFF;
  if (PortNum <= 8) {
    PortMask = PortMask >> ((8 - PortNum) * 4);
  }
  FchXhciSmuUsbConfigUpdate(DieBusNum,
    SIL_RESERVED_0276 + SIL_RESERVED_0349,
    PortMask,
    OverCurrentMap,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciOverCurrent UsbConfigUpdate Message with %x, %x, %x, %x\n",
    SIL_RESERVED_0276 + SIL_RESERVED_0349,
    PortMask,
    OverCurrentMap,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
}

/**
 * FchUsb31PhySwitchPort
 *
 * @brief Select USB 3.1 PHY Port
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb3 port number.
 *
 */
void
FchUsb31PhySwitchPort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  DW0_Index = SIL_RESERVED_0276 + SIL_RESERVED_0350;
  DW1_Mask = 0xF << SIL_RESERVED_0297;
  DW2_Data = Port << SIL_RESERVED_0297;
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsb31PhySwitchPort through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitLateUsbXhci
 *
 * @brief Config USB3 controller before OS Boot
 *
 * @param[in] FchUsbData FCH USB parameter data structure.
 *
 */
void FchInitLateUsbXhci (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable(SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return;
  }
  FchXhciXfer->FchInitPreOsXhci(FchUsbData);
}

/**
 * FchUsbSetInputBlk
 * @brief Establish FCH Usb input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchUsbSetInputBlk (void)
{
  FCHUSB_INPUT_BLK *FchUsbInput;

  FchUsbInput = (FCHUSB_INPUT_BLK *) SilCreateInfoBlock(SilId_FchUsb,
    sizeof (FCHUSB_INPUT_BLK),
    FCHUSB_INPUT_BLK_INSTANCE,
    FCHUSB_MAJOR_REV,
    FCHUSB_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchUsbSetInputBlk at: 0x%x \n", FchUsbInput);
  if (FchUsbInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Usb structure with defaults
  memcpy((void *)FchUsbInput, &mFchUsbDefaults, sizeof (FCHUSB_INPUT_BLK));

  return SilPass;
}

/**
 * FchXhciPrePcieInit
 * @brief Config Xhci controller during Power-On
 *
 *
 * @param[in] FchDataBlockParams FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchUsbData FCH USB configuration structure pointer.
 *
 */
void FchXhciPrePcieInit (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable(SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return;
  }
  FchXhciXfer->FchInitPrePcieXhci(FchUsbData);
}
