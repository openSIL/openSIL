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
#include <SMU/SmuIp2Ip.h>
#include <FCH/Common/FchReg.h>

#define FCH_XHCI_MAX_PORT 0x10

extern FCHUSB_INPUT_BLK mFchUsbDefaults;

/**
 * FchXhciSmuService
 *
 * @brief Xhci1 SMU Service Request
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param RequestId             Request ID.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuService (
  SIL_CONTEXT  *SilContext,
  uint32_t     DieBusNum,
  uint32_t     RequestId
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi) != SilPass) {
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
 * FchXhciUsbInitSmuService
 *
 * @brief Xhci UsbInit SMU Service Request
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param RequestId             Request ID.
 * @param UsbInitData           USB Init Parameters to be passed to the SMU
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT FchXhciUsbInitSmuService (
  SIL_CONTEXT    *SilContext,
  uint32_t       DieBusNum,
  uint32_t       RequestId,
  USB_INIT_DATA  *UsbInitData
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  uint32_t      *SmuArgPtr;
  SMU_IP2IP_API *SmuApi;
  USB_INIT_DATA LocalUsbInitData;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(false);
  }

  memcpy(&LocalUsbInitData, UsbInitData, sizeof(USB_INIT_DATA));
  FCH_TRACEPOINT(SIL_TRACE_INFO, "USB_INIT_DATA @ %x\n", &LocalUsbInitData);

  SmuApi->SmuServiceInitArguments(SmuArg);
  SmuArgPtr = (uint32_t *)&LocalUsbInitData;
  SmuArg[0] = SmuArgPtr[0];
  SmuArg[1] = SmuArgPtr[1];
  SmuArg[2] = SmuArgPtr[2];
  SmuArg[3] = SmuArgPtr[3];

  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);
  Status = SmuApi->SmuServiceRequest(PciAddress, RequestId, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchXhciSmuUsbConfigUpdate
 *
 * @brief Xhci Smu Usb Config Update
 *
 * @details This routine is to send USB Configure Update message to SMU. SMU
 * holds USB register programming. This message is used to tell SMU which
 * register needs to be updated. It includes register information of address,
 * bitfield mask, programming value, register group.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param SmnRegister           Register input
 * @param SmnMask               Mask input
 * @param SmnData               Data input
 * @param SmnGroup              Group input
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  SIL_CONTEXT  *SilContext,
  uint32_t     DieBusNum,
  uint32_t     SmnRegister,
  uint32_t     SmnMask,
  uint32_t     SmnData,
  uint32_t     SmnGroup
  )
{
  SMC_RESULT          Status;
  PCI_ADDR            PciAddress;
  uint32_t            SmuArg[6];
  SMU_IP2IP_API       *SmuApi;
  FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsg;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(false);
    return SMC_Result_Failed;
  }

  FchBiosSmcMsg = (FCH_BIOSSMC_MSG_INPUT_BLK *) xUslFindStructure(SilContext,
    SilId_FchUsb,
    FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH SMU msg block at: 0x%x \n", FchBiosSmcMsg);
  if (FchBiosSmcMsg == NULL) {
    assert(false);
    return SMC_Result_Failed;
  }

  SmuApi->SmuServiceInitArguments(SmuArg);
  SmuArg[0] = SmnRegister;
  SmuArg[1] = SmnMask;
  SmuArg[2] = SmnData;
  SmuArg[3] = SmnGroup;
  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);
  Status = SmuApi->SmuServiceRequest(PciAddress, FchBiosSmcMsg->UsbConfigUpdate, SmuArg, 0);
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
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param PortNum               Usb3 Port count of the controller.
 * @param UsbPortForceGen1      Parameter to set PortForceGen1. bit0 - port0, bit1 - port1, etc.
 * @param Usb3SmnBase           USB3 controller SMN base address.
 *
 */
void
FchUsb3PortForceGen1 (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      UsbController,
  uint32_t      PortNum,
  uint32_t      UsbPortForceGen1
  )
{
  uint32_t   PortMask;
  uint32_t   DW0_Index;
  uint32_t   DW1_Mask;
  uint32_t   DW2_Data;
  uint32_t   DW3_Op_Group;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_INFO, "Controller UsbPortForceGen1 Parameter = 0x%x\n", UsbPortForceGen1);
  assert(PortNum <= FCH_XHCI_MAX_PORT);

  PortMask = GET_BITMASK(PortNum); //bit map mask of all ports. e.g. if controller has 2 ports, PortMask is 11b.
  UsbPortForceGen1 &= PortMask;

  DW0_Index = FCHOFSTx00180000 + USB0CFGx00180130;
  DW1_Mask = PortMask << 16;
  DW2_Data = (uint32_t) (UsbPortForceGen1 << 16);
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext, DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    " UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchXhciDbgSC
 *
 * @brief Enable or disable XHCI DbgSC feature
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param FchUsbData            USB parameter data structure.
 * @param EnableValue           Enable value of enabling Dbg SC feature.
 * @param DisableValue          Disable value of enabling Dbg SC feature.
 *
 */
void
FchXhciDbgSC (
  SIL_CONTEXT      *SilContext,
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

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
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
 * of a Control transfer in different microframes/frames.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param FchUsbData            USB parameter data structure.
 *
 */
void
FchXhciSparseMode (
  SIL_CONTEXT        *SilContext,
  uint32_t           DieBusNum,
  uint32_t           UsbController,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  if (FchUsbData->UsbSparseModeEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      USB3CFGx0000C12C,
      BIT_32(17),
      BIT_32(17),
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup3)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Sparse mode on Controller %x enabled\n", UsbController);
  } else {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      USB3CFGx0000C12C,
      BIT_32(17),
      0,
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup3)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Sparse mode on Controller %x disabled\n", UsbController);
  }
}

/**
 * FchXhciOCPolarity
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to program Over-current input polarity configure.
 * By default, OC pin is low when OC occurs.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param FchUsbData            USB parameter data structure.
 *
 */
void
FchXhciOCPolarity (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         UsbController,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  if (FchUsbData->XhciOcPolarityCfgLow) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      FCHOFSTx00180000 + USB0CFGx00180130,
      BIT_32(8),
      BIT_32(8),
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Set Polarity to Low\n");
  } else {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      FCHOFSTx00180000 + USB0CFGx00180130,
      BIT_32(8),
      0,
      GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Set Polarity to High\n");
  }
}

/**
 * FchXhciDeviceRemovable
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to program Device Removable register to indicate
 * if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param DeviceRemovable       Parameter to set DeviceRemovable. bit[15:0] - USB2.0 ports, bit[31:16] - USB3 ports.
 */
void
FchXhciDeviceRemovable (
  SIL_CONTEXT   *SilContext,
  uint32_t      DieBusNum,
  uint32_t      UsbController,
  uint32_t      DeviceRemovable
  )
{
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHOFSTx00180000 + USB0CFGx00180114,
    DeviceRemovable,
    DeviceRemovable,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciDeviceRemovable UsbConfigUpdate Message with %x, %x, %x, %x\n",
    FCHOFSTx00180000 + USB0CFGx00180114,
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
 * each USB port.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param PortNum               Port count of the controller.
 * @param OverCurrentMap        Parameter to set OC Mapping.
 */
void
FchXhciOverCurrent (
  SIL_CONTEXT  *SilContext,
  uint32_t     DieBusNum,
  uint32_t     UsbController,
  uint32_t     PortNum,
  uint32_t     OverCurrentMap
  )
{
  uint32_t PortMask;

  PortMask = 0xFFFFFFFF;
  if (PortNum > 8) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "PortNum = 0x%x, > 8!\n", PortNum);
    assert(false);
  } else {
    PortMask = PortMask >> ((8 - PortNum) * 4);
  }

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHOFSTx00180000 + USB0CFGx00180120,
    PortMask,
    OverCurrentMap,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciOverCurrent UsbConfigUpdate Message with %x, %x, %x, %x\n",
    FCHOFSTx00180000 + USB0CFGx00180120,
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
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param Port                  Usb3 port number.
 *
 */
void
FchUsb31PhySwitchPort (
  SIL_CONTEXT    *SilContext,
  uint32_t       DieBusNum,
  uint32_t       UsbController,
  uint32_t       Port
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  DW0_Index = FCHOFSTx00180000 + USB0CFGx00180130;
  DW1_Mask = 0xF << FCHSEL12;
  DW2_Data = Port << FCHSEL12;
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext, DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
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
 * FchXhciDdiModeEnable
 *
 * @brief Xhci DDI mode enable
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param UsbController         USB3 controller number.
 * @param DdiMode               DDI mode map.
 */
void
FchXhciDdiModeEnable (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DdiMode
  )
{
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    0,
    0,
    0,
    0xD0000000 | ((UsbController & 0xF) << 20) | DdiMode
    );
}

/**
 * FchXhciPdInterruptMode
 *
 * @brief PD Interrupt Mode
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 */
void
FchXhciPdInterruptMode (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  uint32_t UsbController
  )
{
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    0x00000000,
    0x00000000,
    0x00000000,
    0xE0000001 | ((UsbController & 0xF) << 20)
    );
}

/**
 * FchUsbDbgClkDisable
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchUsbDbgClkDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;
  FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsg;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(false);
    return SMC_Result_Failed;
  }

  FchBiosSmcMsg = (FCH_BIOSSMC_MSG_INPUT_BLK *) xUslFindStructure(SilContext,
    SilId_FchUsb,
    FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH SMU msg block at: 0x%x \n", FchBiosSmcMsg);
  if (FchBiosSmcMsg == NULL) {
    assert(false);
    return SMC_Result_Failed;
  }

  SmuApi->SmuServiceInitArguments(SmuArg);
  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);
  SmuArg[0] = (uint32_t) FchUsbData->UsbDbgClkDisable;
  Status = SmuApi->SmuServiceRequest(PciAddress, FchBiosSmcMsg->DisableUsbDbgClk, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchUsbSetInputBlk
 * @brief Establish FCH Usb input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext              A context structure through which host firmware defined data
 *                                  can be passed to openSIL. The host firmware is responsible
 *                                  for initializing the SIL_CONTEXT structure.
 * @param   FchBiosSmcMsgDefs       A pointer to the data structure defining the SMU-BIOS message
 *                                  interface for the specific revision.
 * @retval SIL_STATUS
 */
SIL_STATUS FchUsbSetInputBlk (
  SIL_CONTEXT  *SilContext,
  const FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsgDefs
  )
{
  FCHUSB_INPUT_BLK *FchUsbInput;
  FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsg;

  FchUsbInput = (FCHUSB_INPUT_BLK *) SilCreateInfoBlock(SilContext,
    SilId_FchUsb,
    sizeof (FCHUSB_INPUT_BLK),
    FCHUSB_INPUT_BLK_INSTANCE,
    FCHUSB_MAJOR_REV,
    FCHUSB_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchUsbSetInputBlk at: 0x%x \n", FchUsbInput);
  if (FchUsbInput == NULL) {
    return SilAborted;
  }

  FchBiosSmcMsg = (FCH_BIOSSMC_MSG_INPUT_BLK *) SilCreateInfoBlock(SilContext,
    SilId_FchUsb,
    sizeof (FCH_BIOSSMC_MSG_INPUT_BLK),
    FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE,
    FCH_BIOSSMC_MSG_INPUT_BLK_MAJOR_REV,
    FCH_BIOSSMC_MSG_INPUT_BLK_MINOR_REV
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FchUsbSetInputBlk at: 0x%x \n", FchUsbInput);
  if (FchBiosSmcMsg == NULL) {
    return SilAborted;
  }

  // Fill Fch Usb structures with defaults
  memcpy((void *)FchUsbInput, &mFchUsbDefaults, sizeof (FCHUSB_INPUT_BLK));
  memcpy((void *)FchBiosSmcMsg, FchBiosSmcMsgDefs, sizeof (FCH_BIOSSMC_MSG_INPUT_BLK));

  return SilPass;
}
