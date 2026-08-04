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
#include "FchUsbCmn2Tc.h"
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
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable
  )
{
  if (PlatformUsbConfigureTable == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_WARNING, "PlatformUsbConfigureTable for %a is NULL!\n", FCH_XHCI_NAME_TC);
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return false;
  }

  if (
    PlatformUsbConfigureTable->Version_Major != FCH_XHCI_VERSION_MAJOR_TC
    || PlatformUsbConfigureTable->Version_Minor != FCH_XHCI_VERSION_MINOR_TC
    || PlatformUsbConfigureTable->TableLength != sizeof (FCH_TC_USB_OEM_PLATFORM_TABLE)
    ) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "PlatformUsbConfigureTable = 0x%x\n", PlatformUsbConfigureTable);
    FCH_TRACEPOINT(SIL_TRACE_ERROR,
      "Version_Major = 0x%x (Exp: 0x%x) Version_Minor = 0x%x (Exp: 0x%x) TableLength = 0x%x (Exp: 0x%x)\n",
      PlatformUsbConfigureTable->Version_Major,
      FCH_XHCI_VERSION_MAJOR_TC,
      PlatformUsbConfigureTable->Version_Minor,
      FCH_XHCI_VERSION_MINOR_TC,
      PlatformUsbConfigureTable->TableLength,
      sizeof (FCH_TC_USB_OEM_PLATFORM_TABLE)
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
  FCH_USB20_PHY     *Usb2Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  // Param0
  DW0_Index = FCH_XHCI_USB_20LANEPARACTL0_CNTR0 + 0x400 * Port;
  DW1_Mask = 0xF3FFFFDF;
  DW2_Data = (uint32_t) ((Usb2Phy->COMPDISTUNE & 0x0F) +
                        ((Usb2Phy->PLLBTUNE & 0x01) << 4) +
                        ((Usb2Phy->PLLITUNE & 0x03) << 6) +
                        ((Usb2Phy->PLLPTUNE & 0x0F) << 8) +
                        ((Usb2Phy->SQRXTUNE & 0x0F) << 12) +
                        ((Usb2Phy->TXFSLSTUNE & 0x0F) << 16) +
                        ((Usb2Phy->TXPREEMPAMPTUNE & 0x07) << 20) +
                        ((Usb2Phy->TXPREEMPPULSETUNE & 0x01) << 23) +
                        ((Usb2Phy->TXRISETUNE & 0x03) << 24) +
                        ((Usb2Phy->TXVREFTUNE & 0x0F) << 28));
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
  DW2_Data = (uint32_t) ((Usb2Phy->TXHSXVTUNE & 0x03)
    | ((Usb2Phy->TXRESTUNE & 0x03) << 2));


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
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable
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
      &(PlatformUsbConfigureTable->Usb20PhyPort[Port])
      );
  }
  // HC1 (1 USB2): Port0
  FchUsbOemUsb20PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->Usb20PhyPort[5])
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
  FCH_USB3_PHY      *Usb3Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  DW0_Index = FCHUSBx168028 + 0x400 * Port;
  DW1_Mask = 0x00000707;
  DW2_Data = (uint32_t) ((Usb3Phy->TX_TERM_CTRL & 0x07) |
                        ((Usb3Phy->RX_TERM_CTRL & 0x07) << 8));
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
  FCH_USB3_PHY      *Usb3Phy
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  // Port control PHY select
  FchUsb31PhySwitchPort(SilContext, DieBusNum, Controller, Port);

  // PHY internal register
  DW0_Index = FCHOFSTx00120000 + USB0CFGx00140088;
  DW1_Mask = 0xF0;
  DW2_Data = (uint32_t) (((Usb3Phy->TX_VBOOST_LVL_EN & 0x01) << 7) |
                         ((Usb3Phy->TX_VBOOST_LVL & 0x07) << 4));
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
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable,
  uint32_t XhciUsb3PortDisable
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC0 (2 USB3): Port0-1
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    0,
    0,
    &(PlatformUsbConfigureTable->Usb3PhyPort[0])
    );
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    0,
    1,
    &(PlatformUsbConfigureTable->Usb3PhyPort[1])
    );

  FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
    DieBusNum,
    0,
    0,
    &(PlatformUsbConfigureTable->Usb3PhyPort[0])
    );
  if ((XhciUsb3PortDisable & BIT_32(1)) == 0) {
    FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
      DieBusNum,
      0,
      1,
      &(PlatformUsbConfigureTable->Usb3PhyPort[1])
      );
  }

  // HC1 (1 USB3): Port0
  FchUsbOemUsb3PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->Usb3PhyPort[2])
    );

  FchUsbOemUsb3PhyConfigureInternalPerPortTc(SilContext,
    DieBusNum,
    1,
    0,
    &(PlatformUsbConfigureTable->Usb3PhyPort[2])
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
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;

  FCH_TRACEPOINT(SIL_TRACE_INFO, "sizeof (uintptr_t) = 0x%x\n", sizeof (uintptr_t));

  PlatformUsbConfigureTable = (FCH_TC_USB_OEM_PLATFORM_TABLE *)(uintptr_t) FchUsbData->OemUsbConfigurationTable;

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
 * FchXhciDisablePortTc -  Xhci Disable Port Control
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciDisablePortTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_OpGroup;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "XhciUsb2PortDisable: %x, XhciUsb3PortDisable %x\n",
                 FchUsbData->XhciUsb2PortDisable, FchUsbData->XhciUsb3PortDisable);

  // HC0 (2 USB3 + 5 USB2): Port0-4
  DW0_Index = FCHOFSTx00180000 + USB0CFGx0018012c;
  DW1_Mask = 0x0003001F;
  DW2_Data = (FchUsbData->XhciUsb2PortDisable & 0x1F) +
              ((FchUsbData->XhciUsb3PortDisable & 0x3) << 16);
  DW3_OpGroup = GET_USB_OP_GROUP(0, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "xHC0 PortDisable: %x\n",
                 xUSLSmnRead(0,
                   DieBusNum,
                   FCH_TC_USB0_SMN_BASE+ FCHOFSTx00180000 + USB0CFGx0018012c
                   ));

  // HC1 (1 USB3 + 1 USB2): Port0-1
  DW0_Index = FCHOFSTx00180000 + USB0CFGx0018012c;
  DW1_Mask = 0x00010001;
  DW2_Data = ((FchUsbData->XhciUsb2PortDisable >> 5) & 0x1) +
              (((FchUsbData->XhciUsb3PortDisable >> 2) & 0x1) << 16);
  DW3_OpGroup = GET_USB_OP_GROUP(1, FchUsbConfigRegGroup1);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_OpGroup
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO, "xHC1 PortDisable: %x\n",
                 xUSLSmnRead(0,
                   DieBusNum,
                   FCH_TC_USB1_SMN_BASE+ FCHOFSTx00180000 + USB0CFGx0018012c
                   ));

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
FchPlatformIndependentRegConfigPerControllerTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  FCHUSB_INPUT_BLK  *FchUsbData,
  uint32_t          Controller
  )
{
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHOFSTx00180000 + USB0CFGx00180118,
    BIT_32(8),
    BIT_32(8),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1)
    );

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx168000,
    BIT_32(0),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx168000 + 0x400,
    BIT_32(0),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx168040,
    BIT_32(20),
    BIT_32(20),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx168040 + 0x400,
    BIT_32(20),
    BIT_32(20),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup1)
    );

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHOFSTx00180000 + USB0CFGx00180130,
    0x0000F000,
    0x00000000,
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx12478C,
    BIT_32(0),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx124790,
    BIT_32(20),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx124B8C,
    BIT_32(20),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx124B90,
    BIT_32(20),
    BIT_32(0),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx124788,
    BIT_32(3),
    BIT_32(3),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBx124B88,
    BIT_32(3),
    BIT_32(3),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup2)
    );

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxC1B0,
    BIT_32(29),
    BIT_32(29),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup3)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxC12C,
    BIT_32(18),
    0x00000000,
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup3)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxC608,
    0xFFFC0000,
    0x30140000,
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup3)
    );
  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxC604,
    BIT_32(2),
    BIT_32(2),
    GET_USB_OP_GROUP(Controller, FchUsbConfigRegGroup3)
    );
}

/**
 * FchXhciCommonRegInitTc  -  Xhci Platform independant register Configuration.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number for Current Die
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciCommonRegInitTc (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchUsbData->Xhci0Enable) {
    FchPlatformIndependentRegConfigPerControllerTc(SilContext, DieBusNum, FchUsbData, 0);
  }

  if (FchUsbData->Xhci1Enable) {
    FchPlatformIndependentRegConfigPerControllerTc(SilContext, DieBusNum, FchUsbData, 1);
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

  FchXhciCommonRegInitTc(SilContext, DieBusNum, FchUsbData);
  FchXhciDisablePortTc(SilContext, DieBusNum, FchUsbData);
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
 * FchTCXhciSmuServiceUsbInit  -  Xhci SMU Service USB Init Request parameters
 *
 * @param DieBusNum             IOHC bus number on current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchXhciSmuServiceUsbInitTc (
  SIL_CONTEXT       *SilContext,
  uint32_t          DieBusNum,
  FCHUSB_INPUT_BLK  *FchUsbData
  )
{
  FCH_TC_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;
  USB_INIT_DATA *UsbInitData = &FchUsbData->UsbInitData;

  PlatformUsbConfigureTable = (FCH_TC_USB_OEM_PLATFORM_TABLE *)FchUsbData->OemUsbConfigurationTable;

  UsbInitData->Enable.usb_init_combined = 1;

  if (FchUsbData->Xhci0Enable) {
    UsbInitData->Enable.usb_hc_0 = 1;
    if (PlatformUsbConfigureTable != NULL) {
      UsbInitData->ComboPhyStaticConfig.usb_hc_0 =
        PlatformUsbConfigureTable->ComboPhyStaticConfig[0] & 0x0F;
    }
    if (FchUsbData->XhciUsb3PortDisable & BIT_32(0)) {
      UsbInitData->UsbControllerConfig.usb_hc_0_ss_port0_disable = 1;
    }
    if (FchUsbData->XhciUsb3PortDisable & BIT_32(1)) {
      UsbInitData->UsbControllerConfig.usb_hc_0_ss_port1_disable = 1;
    }
  }
  if (FchUsbData->Xhci1Enable) {
    UsbInitData->Enable.usb_hc_1 = 1;
    if (FchUsbData->XhciUsb3PortDisable & BIT_32(2)) {
      UsbInitData->UsbControllerConfig.usb_hc_1_ss_port0_disable = 1;
    }
  }
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
  FchXhciSmuServiceUsbInitTc(SilContext, DieBusNum, FchUsbData);

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

/**
 * FchTCXhciInitSsid - Update Xhci SSID
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Ssid       The SSID value to be updated
 *
 */
static void
FchXhciInitSsid (
  uint32_t DieBusNum,
  uint32_t Ssid
  )
{
  xUSLSmnWrite(0, DieBusNum, FCH_USB0_SMN_PCICFG_TC + 0x4C, Ssid);
  xUSLSmnWrite(0, DieBusNum, FCH_USB0_SMN_PCICFG_TC + 0x4C, Ssid);
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
  if (FchUsbData->XhciSsid != 0) {
    FchXhciInitSsid(FchUsbData->DieBusNum, FchUsbData->XhciSsid);
  }
  FchXhciIohcPmeDisable(FchUsbData->DieBusNum, true);

  /* FchInitEnvUsb */
  FchXhciSmuService(SilContext, FchUsbData->DieBusNum, 0);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void FchUsbAfterPcieTrainingDoneTc (
  SIL_CONTEXT *SilContext
  )
{
  FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsg;
  FCHUSB_INPUT_BLK *FchUsbData;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchBiosSmcMsg = (FCH_BIOSSMC_MSG_INPUT_BLK *) xUslFindStructure(SilContext,
    SilId_FchUsb,
    FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE
    );

  if (FchBiosSmcMsg == NULL) {
    assert(false);
    return;
  }

  FchUsbData = (FCHUSB_INPUT_BLK *) xUslFindStructure(SilContext,
    SilId_FchUsb,
    FCHUSB_INPUT_BLK_INSTANCE
    );

  if (FchUsbData == NULL) {
    assert(false);
    return;
  }

  FchXhciUsbInitSmuService(SilContext,
    FchUsbData->DieBusNum,
    FchBiosSmcMsg->UsbInit,
    &FchUsbData->UsbInitData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
