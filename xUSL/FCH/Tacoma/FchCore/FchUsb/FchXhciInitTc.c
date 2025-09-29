/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciInitTc.c
 * @brief Tacoma FCH XHCI Module silicon init functions
 *
 */

#include <SilCommon.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/FchCore/FchUsb/FchXhci.h>
#include <FchReg.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/Fch.h>
#include <IP/FchXhciIp.h>
#include <SMU/SmuIp2Ip.h>
#include "FchUsbRegTc.h"
#include "FchXhciInitTc.h"

/**
 * FchXhciSetNbifStrapRegsTc
 *
 * @brief Disable nBIF EP functions for HC0/1 if they are not enabled in settings.
 *
 * @details Note: On Server programs, nBIF strap register access is disabled from
 * x86 code. As such, on Client this code should eventually be ported to ESID to
 * align with Server.
 */
static void
FchXhciSetNbifStrapRegsTc (
  FCHUSB_INPUT_BLK    *FchUsbData
  )
{
  if (FchUsbData->Xhci0Enable == false) {
    xUSLSmnReadModifyWrite(0, FchUsbData->DieBusNum, FCH_USB0_NBIF_STRAP0_TC, ~BIT_32(28), 0);
  }
  if (FchUsbData->Xhci1Enable == false) {
    xUSLSmnReadModifyWrite(0, FchUsbData->DieBusNum, FCH_USB1_NBIF_STRAP0_TC, ~BIT_32(28), 0);
  }
}

/**
 * FchXhciSubSequenceEnableTc
 * @brief Xhci SubSequence Enable
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 */
static void
FchXhciSubSequenceEnableTc (
  SIL_CONTEXT         *SilContext,
  uint32_t            DieBusNum,
  FCHUSB_INPUT_BLK    *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (FchUsbData->Xhci0Enable) {
    // Combo-PHY SRAM loading
    FchXhciSmuUsbConfigUpdate(SilContext, DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0000001);
    // Standalone-PHY SRAM loading
    FchXhciSmuUsbConfigUpdate(SilContext, DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0000002);
  }

  if (FchUsbData->Xhci1Enable) {
    // Standalone-PHY SRAM loading
    FchXhciSmuUsbConfigUpdate(SilContext, DieBusNum, 0x00000000, 0x00000000, 0x00000000, 0xC0100002);
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciPortForceGen1Tc
 *
 * @brief Force xHCI ports in USB3 controllers to limit to Gen 1 speed.
 *
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DieBusNum           Bus Number on Current Die.
 * @param   FchUsbData          Fch Usb configuration structure pointer.
 */
static void
FchXhciPortForceGen1Tc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (2 USB3): Port0-1
  FchUsb3PortForceGen1(SilContext,
    DieBusNum,
    0,
    2,
    FchUsbData->Usb3PortForceGen1
    );

  // HC0 (1 USB3): Port0
  FchUsb3PortForceGen1(SilContext,
    DieBusNum,
    1,
    1,
    FchUsbData->Usb3PortForceGen1 >> 2
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchUsbCheckOemTableValidTc
 *
 * @brief Check that the USB OEM Platform Configuration Table is correctly formatted.
 *
 * @param PlatformUsbConfigureTable Pointer to USB OEM Platform Configuration Table.
 * @retval true if the table is correctly formatted.
 * @retval false if the table is NULL or incorrect.
 */
bool
FchUsbCheckOemTableValidTc (
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable
  )
{
  if (PlatformUsbConfigureTable == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_WARNING, "PlatformUsbConfigureTable for %a is NULL!\n", FCH_XHCI_NAME_TC);
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return false;
  }

  if (
    PlatformUsbConfigureTable->field0 != FCH_XHCI_VERSION_MAJOR_TC
    || PlatformUsbConfigureTable->field1 != FCH_XHCI_VERSION_MINOR_TC
    || PlatformUsbConfigureTable->field2 != sizeof (SIL_RESERVED_STRUCT_0012)
    ) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "PlatformUsbConfigureTable = 0x%x\n", PlatformUsbConfigureTable);
    FCH_TRACEPOINT(SIL_TRACE_ERROR,
      "Version_Major = 0x%x (Exp: 0x%x) Version_Minor = 0x%x (Exp: 0x%x) TableLength = 0x%x (Exp: 0x%x)\n",
      PlatformUsbConfigureTable->field0,
      FCH_XHCI_VERSION_MAJOR_TC,
      PlatformUsbConfigureTable->field1,
      FCH_XHCI_VERSION_MINOR_TC,
      PlatformUsbConfigureTable->field2,
      sizeof (SIL_RESERVED_STRUCT_0012)
      );
    return false;
  }
  return true;
}


/**
 * FchUsbOemUsb20PhyConfigurePerPortTc
 *
 * @brief Helper function to update USB 2.0 PHY settings for each USB port.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Controller            USB3 Controller number.
 * @param Port                  USB2.0 port number (starts from 0 for each controller).
 * @param Usb2Phy               USB2.0 PHY configuration parameter.
 */
void
FchUsbOemUsb20PhyConfigurePerPortTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  uint32_t          Controller,
  uint8_t           Port,
  SIL_RESERVED_STRUCT_0010  *Usb2Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  // Param0
  DW0_Index = FCH_XHCI_USB_20LANEPARACTL0_CNTR0 + 0x400 * Port;
  DW1_Mask = 0xF3FFFFDF;
  DW2_Data = (uint32_t) ((Usb2Phy->field0 & 0x0F)
    | ((Usb2Phy->field1 & 0x01) << 4)
    | ((Usb2Phy->field2 & 0x03) << 6)
    | ((Usb2Phy->field3 & 0x0F) << 8)
    | ((Usb2Phy->field4 & 0x0F) << 12)
    | ((Usb2Phy->field5 & 0x0F) << 16)
    | ((Usb2Phy->field6 & 0x07) << 20)
    | ((Usb2Phy->field7 & 0x01) << 23)
    | ((Usb2Phy->field8 & 0x03) << 24)
    | ((Usb2Phy->field9 & 0x0F) << 28));
  DW3_OpGroup = GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );

  // Param1
  DW0_Index = FCH_XHCI_USB_20LANEPARACTL1_CNTR0 + 0x400 * Port;
  DW1_Mask = 0x0000000F;
  DW2_Data = (uint32_t) ((Usb2Phy->field10 & 0x03)
    | ((Usb2Phy->field11 & 0x03) << 2));


  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );
}

/**
 * FchUsbOemUsb20PhyTc
 *
 * @brief USB 2.0 PHY Platform Configuration
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param PlatformUsbConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static void
FchUsbOemUsb20PhyTc (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable
  )
{
  uint8_t  Port;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (5 USB2): Port0-4
  for (Port = 0; Port < 5; Port++) {
    FchUsbOemUsb20PhyConfigurePerPortTc(SilContext,
      DieBusNum,
      0,
      Port,
      &(PlatformUsbConfigureTable->field4[Port])
      );
  }
  // HC1 (1 USB2): Port0
  FchUsbOemUsb20PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->field4[5])
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchUsbOemUsb3PhyConfigurePerPortTc
 *
 * @brief Helper function to update USB 3 PHY settings for each USB port.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Controller            USB3 Controller number.
 * @param Port                  USB3 port number.
 * @param Usb3Phy               USB3 PHY configuration parameter.
 */
static void
FchUsbOemUsb3PhyConfigurePerPortTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  uint32_t          Controller,
  uint8_t           Port,
  SIL_RESERVED_STRUCT_0011  *Usb3Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  DW0_Index = FCHUSBx168028 + 0x400 * Port;
  DW1_Mask = 0x00000707;
  DW2_Data = (uint32_t) ((Usb3Phy->field1 & 0x07)
    | ((Usb3Phy->field0 & 0x07) << 8));
  DW3_OpGroup = GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );
}

/**
 * FchUsbOemUsb3PhyConfigureInternalPerPortTc
 *
 * @brief Helper function to update USB 3 PHY internal register settings for each USB port.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Controller            USB3 Controller number.
 * @param Port                  USB3 port number.
 * @param Usb3Phy               USB3 PHY configuration parameter.
 */
static void
FchUsbOemUsb3PhyConfigureInternalPerPortTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  uint32_t          Controller,
  uint8_t           Port,
  SIL_RESERVED_STRUCT_0011  *Usb3Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  // Port control PHY select
  DW0_Index = FCHOFSTx00180000 + USB0CFGx00180130;
  DW1_Mask = 0x0000F000;
  DW2_Data = Port << 12;
  DW3_OpGroup = GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );

  // PHY internal register
  DW0_Index = FCHOFSTx00120000 + USB0CFGx00140088;
  DW1_Mask = 0xF0;
  DW2_Data = (uint32_t) (((Usb3Phy->field2 & 0x01) << 7)
    | ((Usb3Phy->field3 & 0x07) << 4));
  DW3_OpGroup = GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );
}

/**
 * @brief FchUsbOemUsb3PhyTc - USB 3.1 PHY Platform Configuration
 *
 * @details This routine is to update USB 3.1 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param PlatformUsbConfigureTable Usb Oem Platform table pointer passed as input params.
 * @param XhciUsb3PortDisable   Bit map of disabled USB3 ports.
 *
 */
static void
FchUsbOemUsb3PhyTc (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable,
  uint32_t XhciUsb3PortDisable
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (2 USB3): Port0-1
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    0,
    0,
    &(PlatformUsbConfigureTable->field5[0])
    );
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    0,
    1,
    &(PlatformUsbConfigureTable->field5[1])
    );

  FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
    DieBusNum,
    0,
    0,
    &(PlatformUsbConfigureTable->field5[0])
    );
  if ((XhciUsb3PortDisable & BIT_32(1)) == 0) {
    FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
      DieBusNum,
      0,
      1,
      &(PlatformUsbConfigureTable->field5[1])
      );
  }

  // HC1 (1 USB3): Port0
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->field5[2])
    );

  FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->field5[2])
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchXhciOemConfigureTc  -  Xhci OEM Platform Configuration
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciOemConfigureTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable;

  FCH_TRACEPOINT(SIL_TRACE_INFO, "sizeof (uintptr_t) = 0x%x\n", sizeof (uintptr_t));

  PlatformUsbConfigureTable = (SIL_RESERVED_STRUCT_0012 *)(uintptr_t) FchUsbData->OemUsbConfigurationTable;

  if (FchUsbCheckOemTableValidTc(PlatformUsbConfigureTable)) {
    FchUsbOemUsb20PhyTc(SilContext, DieBusNum, PlatformUsbConfigureTable);
    FchUsbOemUsb3PhyTc(SilContext,
      DieBusNum,
      PlatformUsbConfigureTable,
      FchUsbData->XhciUsb3PortDisable
      );
  } else {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Oem configure is not called!\n");
  }
}

/**
 * FchXhciOCPolarityTc
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to call common routine to program Over-current
 * input polarity configuration for each controller.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciOCPolarityTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t UsbController;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 and HC1
  for (UsbController = 0; UsbController < 2; UsbController++) {
    FchXhciOCPolarity(SilContext,
      DieBusNum,
      UsbController,
      FchUsbData
      );
  }
  // HC3 and HC4
  for (UsbController = 3; UsbController < 5; UsbController++) {
    FchXhciOCPolarity(SilContext,
      DieBusNum,
      UsbController,
      FchUsbData
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciDeviceRemovableTc
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to call common routine to program Device Removable
 * register for each controller and port on Phoenix/Tacoma. This is to tell
 * driver if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciDeviceRemovableTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t Usb3Removable;
  uint32_t Usb2Removable;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (2 USB3 + 5 USB2): Port0-6
  FchXhciDeviceRemovable(SilContext,
    DieBusNum,
    0,
    FchUsbData->Xhci0DevRemovable & 0x0003001F
    );

  // HC1 (1 USB3 + 1 USB2): Port0-1
  Usb3Removable = (FchUsbData->Xhci0DevRemovable & 0xFFFF0000) >> 16;
  Usb3Removable = Usb3Removable >> 2;
  Usb2Removable = FchUsbData->Xhci0DevRemovable & 0x0000FFFF;
  Usb2Removable = Usb2Removable >> 5;
  FchXhciDeviceRemovable(SilContext,
    DieBusNum,
    1,
    (Usb2Removable & 0x1) + ((Usb3Removable & 0x1) << 16)
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciOverCurrentTc
 * @brief Update Xhci OC Pin Mapping
 *
 * @details This routine is to call common routine to program which
 * Over-Current Pin is mapping to each USB port of Phoenix.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciOverCurrentTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (2 USB3 + 5 USB2): Port0-6
  FchXhciOverCurrent(SilContext,
    DieBusNum,
    0,
    7,
    FchUsbData->XhciOCpinSelect[0].OcPinSelect & 0x0FFFFFFF
    );
  // HC1 (1 USB3 + 1 USB2): Port0-1
  FchXhciOverCurrent(SilContext,
    DieBusNum,
    1,
    2,
    FchUsbData->XhciOCpinSelect[1].OcPinSelect & 0x000000FF
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciSparseModeTc
 *
 * @brief Enable Sparse Mode for each controller
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
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static
void
FchXhciSparseModeTc (
  SIL_CONTEXT        *SilContext,
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  uint32_t UsbController;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 and HC1
  for (UsbController = 0; UsbController < 2; UsbController++) {
    FchXhciSparseMode(SilContext,
      DieBusNum,
      UsbController,
      FchUsbData
      );
  }
  // HC3 and HC4
  for (UsbController = 3; UsbController < 5; UsbController++) {
    FchXhciSparseMode(SilContext,
      DieBusNum,
      UsbController,
      FchUsbData
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciDdiModeEnableTc
 *
 * @brief Xhci DDI mode enable
 *
 * @details This routine is to call common routine to program DDI mode
 * for Combo-PHYs.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciDdiModeEnableTc (
  SIL_CONTEXT        *SilContext,
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchUsbData->Xhci0Enable) {
    // HC0 has 1 Combo-PHY
    FchXhciDdiModeEnable(SilContext,
      DieBusNum,
      0,
      FchUsbData->DdiModeEnable & 0x0000000F
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciPdInterruptModeTc
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciPdInterruptModeTc (
  SIL_CONTEXT        *SilContext,
  uint32_t           DieBusNum,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0
  if (FchUsbData->Xhci0Enable && (FchUsbData->PdInterruptModeEn & BIT_8(0))) {
    FchXhciPdInterruptMode(SilContext, DieBusNum, 0);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciPassParameterTc
 *
 * @brief Xhci Pass Parameters
 *
 * @details This routine is to call Phoenix/TC specific functions to send
 * register update messages to SMU for USB initialization.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void FchXhciPassParameterTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchXhciOverCurrentTc(SilContext, DieBusNum, FchUsbData);
  FchXhciOCPolarityTc(SilContext, DieBusNum, FchUsbData);
  FchXhciDeviceRemovableTc(SilContext, DieBusNum, FchUsbData);
  FchXhciPortForceGen1Tc(SilContext, DieBusNum, FchUsbData);
  FchXhciSparseModeTc(SilContext, DieBusNum, FchUsbData);
  FchXhciOemConfigureTc(SilContext, DieBusNum, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciInitS3ExitProgramTc
 *
 * @brief Config Xhci controller during S3 Exit
 *
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             IOCH bus number on current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void FchXhciInitS3ExitProgramTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);

  if (FchXhciCheckUsbControllerSkip(DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip All USB Controller Access\n", DieBusNum);
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  FchXhciPassParameterTc(SilContext, DieBusNum, FchUsbData);
  FchXhciDdiModeEnableTc(SilContext, DieBusNum, FchUsbData);
  FchUsbDbgClkDisable(SilContext, DieBusNum, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciInitBootProgramTc
 *
 * @brief Config Xhci controller during Power-On
 *
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             IOHC bus number on current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void FchXhciInitBootProgramTc (
  SIL_CONTEXT      *SilContext,
  uint32_t          DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);

  if (FchXhciCheckUsbControllerSkip(DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip All USB Controller Access\n", DieBusNum);
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  FchXhciPassParameterTc(SilContext, DieBusNum, FchUsbData);

  if (FchXhciCheckUsbPhySkip(DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip USB PHY\n");
  } else {
    FchXhciSubSequenceEnableTc(SilContext, DieBusNum, FchUsbData);
  }

  FchXhciDdiModeEnableTc(SilContext, DieBusNum, FchUsbData);
  FchUsbDbgClkDisable(SilContext, DieBusNum, FchUsbData);
  FchXhciPdInterruptModeTc(SilContext, DieBusNum, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}

/**
 * FchInitResetXhciTc
 *
 * @brief Initialize Xhci controller during early Power-On
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchInitResetXhciTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchXhciSetNbifStrapRegsTc(FchUsbData);

  if (FchUsbData->Xhci0Enable || FchUsbData->Xhci1Enable) {
    if (SilFchReadSleepType () == ACPI_SLPTYP_S3) {
      FchXhciInitS3ExitProgramTc(SilContext, FchUsbData->DieBusNum, FchUsbData);
    } else {
      FchXhciInitBootProgramTc(SilContext, FchUsbData->DieBusNum, FchUsbData);
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*
 * FchInitPrePcieXhciTc
 *
 * @brief Config FCH XHCI Module before PCI enumeration.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   FchUsbData          Fch USB configuration structure pointer.
 *
 */
void
FchInitPrePcieXhciTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchInitResetXhciTc(SilContext, FchUsbData);
  FchXhciIohcPmeDisable(FchUsbData->DieBusNum, true);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
