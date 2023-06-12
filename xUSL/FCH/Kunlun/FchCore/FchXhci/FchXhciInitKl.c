/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciInitKl.c
 * @brief Kunlun FCH XHCI Module silicon init functions
 *
 */

#include <SilCommon.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/FchHwAcpi-api.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/Fch.h>
#include <SMU/SmuIp2Ip.h>
#include "FchXhciKl.h"
#include "FchXhciKlReg.h"
#include "FchXhciCmn2Kl.h"
#include "FchXhciIp2IpKl.h"
#include <FCH/Common/FchReg.h>

void
FchKLXhciInitS3EntryProgram (
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  );

/**
 * FchKLXhciSmuServiceUsbInit
 *
 * @brief Xhci SMU Service Request for UsbInit
 *
 * @details This routine is to send Usb INIT message to SMU to trigger USB
 * initialization FSDL programming. It use parameter DW0 (SmuArg[0]) to tell
 * SMU/FSDL XHCI controller is enabled or not.
 *   DW0[0] - 0: XHCI0 is disabled; 1: XHCI0 is enabled.
 *   DW0[1] - 0: XHCI1 is disabled; 1: XHCI1 is enabled.
 *
 *
 * @param[in] DieBusNum  Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
static SMC_RESULT
FchKLXhciSmuServiceUsbInit (
  uint32_t          DieBusNum,
  FCHUSB_INPUT_BLK  *FchUsbData
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
    assert(Status == SMC_Result_OK);
  }

  SmuApi->SmuServiceInitArguments(SmuArg);
  PciAddress.AddressValue = MAKE_SBDFO(DF_GET_SEGMENT(DieBusNum), DF_GET_BUS(DieBusNum), 0, 0, 0);

  SmuArg[0] = 0;
  SmuArg[1] = 0;
  if (FchUsbData->Xhci0Enable) {
    SmuArg[0] |= BIT_32(0);
  }
  if (FchUsbData->Xhci1Enable) {
    SmuArg[0] |= BIT_32(1);
  }

  Status = SmuApi->SmuServiceRequest(PciAddress, SIL_RESERVED_0294, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchKLXhciSubSequenceEnable
 *
 * @brief Xhci SubSequence Enable
 *
 *
 * @param[in] DieBusNum  Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 */
static
void
FchKLXhciSubSequenceEnable (
  uint32_t            DieBusNum,
  FCHUSB_INPUT_BLK    *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (FchUsbData->Xhci0Enable) {
    // Type-A PHY
    FchXhciSmuUsbConfigUpdate(DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0000002);
  }

#if SIL_RESERVED_0370 > 1
  if (FchUsbData->Xhci1Enable) {
    // Type-A PHY
    FchXhciSmuUsbConfigUpdate(DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0100002);
  }
#endif

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchKLUsbPortForceGen1
 * @brief Port Force Gen1
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchKLUsbPortForceGen1 (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint8_t    UsbPortForceGen1;

  UsbPortForceGen1 = FchUsbData->Usb3PortForceGen1;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_INFO, "Overall UsbPortForceGen1 Parameter = 0x%x\n", UsbPortForceGen1);

  // Controller 0
  FchUsb3PortForceGen1(DieBusNum,
    0,
    SIL_RESERVED_0372,
    UsbPortForceGen1 & 0x3
    );

#if SIL_RESERVED_0370 > 1
  // Controller 1
  UsbPortForceGen1 = UsbPortForceGen1 >> 2;
  FchUsb3PortForceGen1(DieBusNum,
    1,
    SIL_RESERVED_0372,
    UsbPortForceGen1 & 0x3
    );
#endif

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchKLUsbOemUsb20PhyConfigure
 *
 * @brief USB 2.0 PHY Platform Configurationb
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. They're 7 USB 2.0 PHY tuning fields
 * available to update:
 *   COMPDSTUNE
 *   TXFSLSTUNE
 *   TXPREEMPAMPTUNE
 *   TXPREEMPPULSETUNE
 *   TXRISETUNE
 *   TXVREFTUNE
 *   TXHSXVTUNE
 *   TXVREFTUNE
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb2.0 port number.
 * @param[in] Usb2Phy USB2.0 PHY configuration parameter.
 *
 */
static
void
FchKLUsbOemUsb20PhyConfigure (
  uint32_t          DieBusNum,
  uint32_t          UsbController,
  uint32_t          Port,
  SIL_RESERVED_STRUCT_0017  *Usb2Phy
  )
{
  uint32_t  DW0_Index;
  uint32_t  DW1_Mask;
  uint32_t  DW2_Data;
  uint32_t  DW3_Op_Group;

  uint8_t   COMPDSTUNE;
  uint8_t   SQRXTUNE;
  uint8_t   TXFSLSTUNE;
  uint8_t   TXPREEMPAMPTUNE;
  uint8_t   TXPREEMPPULSETUNE;
  uint8_t   TXRISETUNE;
  uint8_t   TXVREFTUNE;
  uint8_t   TXHSXVTUNE;
  uint8_t   TXRESTUNE;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  assert(Port < SIL_RESERVED_0372);
  assert(Usb2Phy != NULL);

  COMPDSTUNE = Usb2Phy->field0;
  SQRXTUNE = Usb2Phy->field1;
  TXFSLSTUNE = Usb2Phy->field2;
  TXPREEMPAMPTUNE = Usb2Phy->field3;
  TXPREEMPPULSETUNE = Usb2Phy->field4;
  TXRISETUNE = Usb2Phy->field5;
  TXVREFTUNE = Usb2Phy->field6;
  TXHSXVTUNE = Usb2Phy->field7;
  TXRESTUNE = Usb2Phy->field8;

  DW0_Index = FCH_XHCI_USB_20LANEPARACTL0_CNTR0 + 0x400 * Port;
  DW1_Mask = ((COMPDSTUNE == 0xFF) ? 0x0 : (0x07 << 0)) \
    | ((SQRXTUNE == 0xFF) ? 0x0 : (0x07 << 12)) \
    | ((TXFSLSTUNE == 0xFF) ? 0x0 : (0x0F << 16)) \
    | ((TXPREEMPAMPTUNE == 0xFF) ? 0x0 : (0x03 << 20)) \
    | ((TXPREEMPPULSETUNE == 0xFF) ? 0x0 : (0x01 << 23)) \
    | ((TXRISETUNE == 0xFF) ? 0x0 : (0x03 << 24)) \
    | ((TXVREFTUNE == 0xFF) ? 0x0 : (0x0F << 28));
  DW2_Data = ((COMPDSTUNE == 0xFF) ? 0x0 : ((COMPDSTUNE & 0x07) << 0)) \
    | ((SQRXTUNE == 0xFF) ? 0x0 : ((SQRXTUNE & 0x07) << 12)) \
    | ((TXFSLSTUNE == 0xFF) ? 0x0 : ((TXFSLSTUNE & 0x0F) << 16)) \
    | ((TXPREEMPAMPTUNE == 0xFF) ? 0x0 : ((TXPREEMPAMPTUNE & 0x03) << 20)) \
    | ((TXPREEMPPULSETUNE == 0xFF) ? 0x0 : ((TXPREEMPPULSETUNE & 0x01) << 23)) \
    | ((TXRISETUNE == 0xFF) ? 0x0 : ((TXRISETUNE & 0x03) << 24)) \
    | ((TXVREFTUNE == 0xFF) ? 0x0 : ((TXVREFTUNE & 0x0F) << 28));
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsbOemUsb20PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index = FCH_XHCI_USB_20LANEPARACTL1_CNTR0 + 0x400 * Port;
  DW1_Mask = ((TXHSXVTUNE == 0xFF) ? 0x0 : (0x03 << 0))  \
    | ((TXRESTUNE == 0xFF) ? 0x0 : (0x03 << 2));
  DW2_Data = ((TXHSXVTUNE == 0xFF) ? 0x0 : ((TXHSXVTUNE & 0x03) << 0))  \
    | ((TXRESTUNE == 0xFF) ? 0x0 : ((TXRESTUNE & 0x03) << 2)),
  DW3_Op_Group = GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsbOemUsb20PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchKLUsbOemUsb20PhyConfigure
 *
 * @brief USB 2.0 PHY Platform Configurationb
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. This KL program routine would check if
 * Customer wants to enable USB 2.0 PHY update and then call to PHY update
 * common routine to program each port.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static
void
FchKLUsbOemUsb20Phy (
  uint32_t  DieBusNum,
  void      *PlatformConfigureTable
  )
{
  uint8_t                       Controller;
  uint8_t                       Port;
  SIL_RESERVED_STRUCT_0019 *FchUsbOemPlatformTable;

  Controller = 0;
  Port = 0;
  FchUsbOemPlatformTable = NULL;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  assert(PlatformConfigureTable != NULL);
  FchUsbOemPlatformTable = (SIL_RESERVED_STRUCT_0019 *) PlatformConfigureTable;

  if (DieBusNum == 0 && FchUsbOemPlatformTable->field4 == 0) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Socket 0 Usb2 PHY is not enabled\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  if (DieBusNum != 0 && FchUsbOemPlatformTable->field7 == 0) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Socket 1 Usb2 PHY is not enabled\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  for (Controller = 0; Controller < SIL_RESERVED_0370; Controller++) {
    for (Port = 0; Port < SIL_RESERVED_0372; Port++) {
      FchKLUsbOemUsb20PhyConfigure(DieBusNum,
        Controller,
        Port,
        (DieBusNum == 0) ? &(FchUsbOemPlatformTable->field5[Controller * SIL_RESERVED_0372 + Port]) : \
        &(FchUsbOemPlatformTable->field8[Controller * SIL_RESERVED_0372 + Port])
        );
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


static
void
FchKLUsbOemUsb31PhyConfigure (
  uint32_t          DieBusNum,
  uint32_t          UsbController,
  uint32_t          Port,
  SIL_RESERVED_STRUCT_0018  *Usb3Phy
  )
{
  uint8_t RX_ANA_IQ_PHASE_ADJUST;
  uint8_t RX_EQ_DELTA_IQ_OVRD_EN;
  uint8_t RX_EQ_DELTA_IQ_OVRD_VAL;
  uint8_t RX_IQ_PHASE_ADJUST;
  uint8_t TX_VBOOST_LVL_EN;
  uint8_t TX_VBOOST_LVL;
  uint8_t RX_VREF_CTRL_EN;
  uint8_t RX_VREF_CTRL;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  assert(Usb3Phy != NULL);

  RX_ANA_IQ_PHASE_ADJUST = Usb3Phy->field0;
  RX_EQ_DELTA_IQ_OVRD_EN = Usb3Phy->field1;
  RX_EQ_DELTA_IQ_OVRD_VAL = Usb3Phy->field2;
  RX_IQ_PHASE_ADJUST = Usb3Phy->field3;
  TX_VBOOST_LVL_EN = Usb3Phy->field4;
  TX_VBOOST_LVL = Usb3Phy->field5;
  RX_VREF_CTRL_EN = Usb3Phy->field6;
  RX_VREF_CTRL = Usb3Phy->field7;

  //
  // Switch Port (PHY) first
  //
  FchUsb31PhySwitchPort(DieBusNum, UsbController, Port);

  FchXhciSmuUsbConfigUpdate(DieBusNum,
    SIL_RESERVED_0277 + SIL_RESERVED_0299,
    (RX_ANA_IQ_PHASE_ADJUST == 0xFF) ? 0x0 : 0x7F,
    (RX_ANA_IQ_PHASE_ADJUST == 0xFF) ? 0x0 : RX_ANA_IQ_PHASE_ADJUST & 0x7F,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup2)
    );

  FchXhciSmuUsbConfigUpdate(DieBusNum,
    SIL_RESERVED_0277 + SIL_RESERVED_0300,
    ((RX_EQ_DELTA_IQ_OVRD_VAL == 0xFF) ? 0x0 : 0x0F)                                     \
    | ((RX_EQ_DELTA_IQ_OVRD_EN == 0xFF) ? 0x0 : (0x01 << SIL_RESERVED_0298)),
    ((RX_EQ_DELTA_IQ_OVRD_VAL == 0xFF) ? 0x0 : (RX_EQ_DELTA_IQ_OVRD_VAL & 0xF))                                  \
    | ((RX_EQ_DELTA_IQ_OVRD_EN == 0xFF) ? 0x0 : ((RX_EQ_DELTA_IQ_OVRD_EN & 1) << SIL_RESERVED_0298)),
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup2)
    );

  FchXhciSmuUsbConfigUpdate(DieBusNum,
    FCH_XHCI_USB31_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST,
    (RX_IQ_PHASE_ADJUST == 0xFF) ? 0x00 : 0x7F,
    (RX_IQ_PHASE_ADJUST == 0xFF) ? 0x00 : RX_IQ_PHASE_ADJUST & 0x7F,
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup2)
    );

  FchXhciSmuUsbConfigUpdate(DieBusNum,
    SIL_RESERVED_0277 + SIL_RESERVED_0301,
    ((RX_VREF_CTRL == 0xFF)?0:(SIL_RESERVED_0364 << SIL_RESERVED_0365))       \
    | ((RX_VREF_CTRL_EN == 0xFF)?0:(SIL_RESERVED_0362 << SIL_RESERVED_0363))    \
    | ((TX_VBOOST_LVL == 0xFF)?0:(SIL_RESERVED_0368 << SIL_RESERVED_0369))      \
    | ((TX_VBOOST_LVL_EN == 0xFF)?0:(SIL_RESERVED_0366 << SIL_RESERVED_0367)),
    ((RX_VREF_CTRL == 0xFF)?0:((RX_VREF_CTRL & SIL_RESERVED_0364) << SIL_RESERVED_0365))     \
    | ((RX_VREF_CTRL_EN == 0xFF)?0:((RX_VREF_CTRL_EN & SIL_RESERVED_0362) << SIL_RESERVED_0363))  \
    | ((TX_VBOOST_LVL == 0xFF)?0:((TX_VBOOST_LVL & SIL_RESERVED_0368) << SIL_RESERVED_0369))    \
    | ((TX_VBOOST_LVL_EN == 0xFF)?0:((TX_VBOOST_LVL_EN & SIL_RESERVED_0366) << SIL_RESERVED_0367)),
    GET_USB_OP_GROUP(UsbController, FchUsbConfigRegGroup2)
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * @brief FchKLUsbOemUsb31PhyConfigure  -  USB 3.1 PHY Platform Configuration
 *
 * @details This routine is to update USB 3.1 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. This KL program routine would check if
 * Customer wants to enable USB 3.1 PHY update and then call to PHY update
 * common routine to program each port.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static
void
FchKLUsbOemUsb31Phy (
  uint32_t  DieBusNum,
  void      *PlatformConfigureTable
  )
{
  uint8_t                       Controller;
  uint8_t                       Port;
  SIL_RESERVED_STRUCT_0019 *FchUsbOemPlatformTable;

  Controller = 0;
  Port = 0;
  FchUsbOemPlatformTable = NULL;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  assert(PlatformConfigureTable != NULL);
  FchUsbOemPlatformTable = (SIL_RESERVED_STRUCT_0019 *) PlatformConfigureTable;

  if (DieBusNum == 0 && FchUsbOemPlatformTable->field9 == 0) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Socket 0 Usb3 PHY is not enabled\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  if (DieBusNum != 0 && FchUsbOemPlatformTable->field11 == 0) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Socket 1 Usb3 PHY is not enabled\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  for (Controller = 0; Controller < SIL_RESERVED_0370; Controller++) {
    for (Port = 0; Port < SIL_RESERVED_0372; Port++) {
      FchKLUsbOemUsb31PhyConfigure(DieBusNum,
        Controller,
        Port,
        (DieBusNum == 0) ? &(FchUsbOemPlatformTable->field10[Controller * SIL_RESERVED_0372 + Port]) :
        &(FchUsbOemPlatformTable->field12[Controller * SIL_RESERVED_0372 + Port])
        );
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchKLXhciOemConfigure
 * @brief Xhci OEM Platform Configuration
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static
void
FchKLXhciOemConfigure (
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  SIL_RESERVED_STRUCT_0019 *PlatformUsbConfigureTable;

  FCH_TRACEPOINT(SIL_TRACE_INFO, "sizeof (uintptr_t) = 0x%x\n", sizeof (uintptr_t));
  PlatformUsbConfigureTable = (SIL_RESERVED_STRUCT_0019 *)(uintptr_t) FchUsbData->OemUsbConfigurationTable;

  if (PlatformUsbConfigureTable != NULL
    && PlatformUsbConfigureTable->field0 == SIL_RESERVED_0373
    && PlatformUsbConfigureTable->field1 == SIL_RESERVED_0374
    && PlatformUsbConfigureTable->field2 == sizeof (SIL_RESERVED_STRUCT_0019)) {
    FchKLUsbOemUsb20Phy(DieBusNum, PlatformUsbConfigureTable);
    FchKLUsbOemUsb31Phy(DieBusNum, PlatformUsbConfigureTable);
  } else {
    FCH_TRACEPOINT(SIL_TRACE_WARNING, "PlatformUsbConfigureTable for %a is not found!\n", SIL_RESERVED_0371);
    FCH_TRACEPOINT(SIL_TRACE_INFO, "PlatformUsbConfigureTable = 0x%x\n", PlatformUsbConfigureTable);
    if (PlatformUsbConfigureTable != NULL) {
      FCH_TRACEPOINT(SIL_TRACE_INFO,
        "Version_Major = 0x%x (Exp: 0x%x) Version_Minor = 0x%x (Exp: 0x%x) TableLength = 0x%x (Exp: 0x%x)\n",
        PlatformUsbConfigureTable->field0,
        SIL_RESERVED_0373,
        PlatformUsbConfigureTable->field1,
        SIL_RESERVED_0374,
        PlatformUsbConfigureTable->field2,
        sizeof (SIL_RESERVED_STRUCT_0019)
        );
    }
  }
}


/**
 * FchKLXhciDbgSC
 *
 * @brief Enable or disable XHCI DbgSC feature
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciDbgSC (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < SIL_RESERVED_0370; UsbController++) {
    FchXhciDbgSC(DieBusNum,
      UsbController,
      FchUsbData,
      0xF5000101,
      0xF5000000
      );
  }
}


/**
 * FchKLXhciSparseMode
 *
 * @brief Enable Sparse Mode for each controller
 *
 * @details Some devices are slow in responding to Control transfers.
 * Scheduling multiple transactions in one microframe/frame can cause these
 * devices to misbehave. If enabled, the host controller schedules each phase
 * of a Control transfer in different microframes/frames. Please refer to PPR
 * for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciSparseMode (
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < SIL_RESERVED_0370; UsbController++) {
    FchXhciSparseMode(DieBusNum,
      UsbController,
      FchUsbData
      );
  }
}

/**
 * FchKLXhciOCPolarity
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to call common routine to program Over-current
 * input polarity configuration for each controller. They're 2 XHCI controller.
 * Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchKLXhciOCPolarity (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < SIL_RESERVED_0370; UsbController++) {
    FchXhciOCPolarity(DieBusNum,
      UsbController,
      FchUsbData
      );
  }
}

/**
 * FchKLXhciDeviceRemovable
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to call common routine to program Device Removable
 * register for each controller and port. This is to tell
 * driver if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not. Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciDeviceRemovable (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t DeviceRemovable;

  // Port0-1 : Controller0
  DeviceRemovable = FchUsbData->Xhci0DevRemovable;
  DeviceRemovable = DeviceRemovable & 0x00030003;
  FchXhciDeviceRemovable(DieBusNum,
    0,
    DeviceRemovable
    );

#if SIL_RESERVED_0370 > 1
  // Port2-3 : Controller1
  DeviceRemovable = FchUsbData->Xhci0DevRemovable;
  DeviceRemovable = DeviceRemovable >> 2;
  DeviceRemovable = DeviceRemovable & 0x00030003;
  FchXhciDeviceRemovable(DieBusNum,
    1,
    DeviceRemovable
    );
#endif
}

/**
 * FchKLXhciOverCurrent
 * @brief Update Xhci OC Pin Mapping
 *
 * @details This routine is to call common routine to program which
 * Over-Current Pin is mapping to each USB port.
 * Please refer to PPR for more details.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciOverCurrent (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t OverCurrentMap;
  uint32_t UsbController;

  for (UsbController = 0; UsbController < SIL_RESERVED_0370; UsbController++) {
    OverCurrentMap = (FchUsbData->XhciOCpinSelect[UsbController].Usb20OcPin & 0xFF);
    OverCurrentMap |= ((FchUsbData->XhciOCpinSelect[UsbController].Usb31OcPin & 0xFF) << 8);
    FchXhciOverCurrent(DieBusNum,
      UsbController,
      SIL_RESERVED_0370 * SIL_RESERVED_0372,
      OverCurrentMap
      );
  }
}

/**
 * FchKLXhciPassParameter
 *
 * @brief Xhci Pass Parameters
 *
 * @details This routine is to call KL specific functions to send
 * register update messages to SMU for USB initialization.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciPassParameter (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchKLXhciDbgSC(DieBusNum, FchUsbData);
  FchKLXhciOverCurrent(DieBusNum, FchUsbData);
  FchKLXhciDeviceRemovable(DieBusNum, FchUsbData);
  FchKLXhciOCPolarity(DieBusNum, FchUsbData);
  FchKLXhciSparseMode(DieBusNum, FchUsbData);
  FchKLXhciOemConfigure(DieBusNum, FchUsbData);
  FchKLUsbPortForceGen1(DieBusNum, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchKLXhciInitS3EntryProgram - Config Xhci controller before
 * entering S3
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchUsbData Fch configuration structure pointer.
 *
 */
void
FchKLXhciInitS3EntryProgram (
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);
  FchXhciSmuService(DieBusNum, SIL_RESERVED_0296);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}


/**
 * FchKLXhciInitS3ExitProgram
 *
 * @brief Config Xhci controller during S3 Exit
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLXhciInitS3ExitProgram (
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);
  FchKLXhciPassParameter(DieBusNum, FchUsbData);
  FchXhciSmuService(DieBusNum, SIL_RESERVED_0295);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}

/**
 * FchKLXhciInitBootProgram
 *
 * @brief Config Xhci controller during Power-On
 *
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 *
 */
static
void
FchKLXhciInitBootProgram (
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);

  if (FchUsbData->UsbFlag.SkipAllUSBHostAccess) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip All USB Controller Access\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
    return;
  }

  FchKLXhciPassParameter(DieBusNum, FchUsbData);

  if (FchUsbData->UsbFlag.SkipAllUSBPhyAccess) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip USB PHY\n");
  } else {
    FchKLXhciSubSequenceEnable(DieBusNum, FchUsbData);
  }

  FchKLXhciSmuServiceUsbInit(DieBusNum, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}

/**
 * FchKLInitResetXhci
 *
 * @brief Initialize Xhci controller during early Power-On
 *
 * @param[in] FchDataBlock FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static
void
FchKLInitResetXhci (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchUsbData->Xhci0Enable || FchUsbData->Xhci1Enable) {
    if (SilFchReadSleepType () == ACPI_SLPTYP_S3) {
      FchKLXhciInitS3ExitProgram(FchUsbData->DieBusNum, FchUsbData);
    } else {
      FchKLXhciInitBootProgram(FchUsbData->DieBusNum, FchUsbData);
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchKLXhciIohcPmeDisable
 *
 * @brief Enable or disable IOHC PME for USB function
 *
 *
 *
 * @param[in] DieBusNum Fch configuration structure pointer.
 * @param[in] PMEDis Fch configuration structure pointer.
 *
 */
static
void
FchKLXhciIohcPmeDisable (
  uint32_t  DieBusNum,
  bool      PMEDis
  )
{
  if (PMEDis) {
    xUSLSmnReadModifyWrite(DF_GET_SEGMENT(DieBusNum),
      DF_GET_BUS(DieBusNum),
      SIL_RESERVED_0360,
      ~BIT_32(4),
      BIT_32(4)
      );
    xUSLSmnReadModifyWrite(DF_GET_SEGMENT(DieBusNum),
      DF_GET_BUS(DieBusNum),
      SIL_RESERVED_0361,
      ~BIT_32(4),
      BIT_32(4)
      );
  } else {
    xUSLSmnReadModifyWrite(DF_GET_SEGMENT(DieBusNum),
      DF_GET_BUS(DieBusNum),
      SIL_RESERVED_0360,
      ~BIT_32(4),
      0
      );
    xUSLSmnReadModifyWrite(DF_GET_SEGMENT(DieBusNum),
      DF_GET_BUS(DieBusNum),
      SIL_RESERVED_0361,
      ~BIT_32(4),
      0
      );
  }
}

/**
 * FchKLInitEnvUsbXhci
 * @brief Config XHCI controller before PCI enumeration
 *
 *
 * @param[in] Usb Fch Usb configuration structure pointer.
 *
 */
static void FchKLInitEnvUsbXhci (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchKLXhciIohcPmeDisable(FchUsbData->DieBusNum, true);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPrePcieXhciKl
 *
 * @brief Config FCH XHCI Module before PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPrePcieXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //
  FchKLInitResetXhci(FchUsbData);
  FchKLInitEnvUsbXhci(FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPostPcieXhciKl
 *
 * @brief Config FCH XHCI Module after PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPostPcieXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  UNUSED(FchUsbData);
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreOsXhciKl
 *
 * @brief Config FCH XHCI Module before OS boot.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPreOsXhciKl (
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  UNUSED(FchUsbData);
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSecondaryFchInitUsbKl
 *
 * @brief  Secondary Fch Usb Initialization.
 *
 * @param DieBusNum   Bus Number for Current Die
 * @param FchUsbData  FCHUSB_INPUT_BLK configuration structure pointer
 *
 * @retval None
 *
 */
void
FchSecondaryFchInitUsbKl (
  uint8_t          DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t  DieBusNum32;
  DieBusNum32 = (uint32_t) DieBusNum;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (!(FchUsbData->Xhci2Enable)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "FchSecondaryFchInitUsb() XHCI2 disabled.\n");
  }

  if (!(FchUsbData->Xhci3Enable)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "FchSecondaryFchInitUsb() XHCI3 disabled.\n");
  }

  //
  // Set Usb parameters for secondary FCH init.
  //
  FchUsbData->DieBusNum = DieBusNum32;
  FchUsbData->Xhci0Enable = FchUsbData->Xhci2Enable;
  FchUsbData->Xhci1Enable = FchUsbData->Xhci3Enable;
  FchUsbData->Xhci0DevRemovable = (uint32_t) (FchUsbData->Xhci0DevRemovable >> 4);
  FchUsbData->XhciUsb3PortDisable = (uint32_t) (FchUsbData->XhciUsb3PortDisable >> 4);
  FchUsbData->XhciUsb2PortDisable = (uint32_t) (FchUsbData->XhciUsb2PortDisable >> 4);
  FchUsbData->XhciOCpinSelect[0].Usb20OcPin = FchUsbData->XhciOCpinSelect[2].Usb20OcPin;
  FchUsbData->XhciOCpinSelect[0].Usb31OcPin = FchUsbData->XhciOCpinSelect[2].Usb31OcPin;
  FchUsbData->XhciOCpinSelect[1].Usb20OcPin = FchUsbData->XhciOCpinSelect[3].Usb20OcPin;
  FchUsbData->XhciOCpinSelect[1].Usb31OcPin = FchUsbData->XhciOCpinSelect[3].Usb31OcPin;

  FchXhciPrePcieInit(FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
