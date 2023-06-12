/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsb-api.h
 * @brief FCH USB input block definitions
 *
 */

#pragma once

#define FCHUSB_MAJOR_REV            0
#define FCHUSB_MINOR_REV            1
#define FCHUSB_INPUT_BLK_INSTANCE   0

#pragma pack (push, 1)  // match the packing of it's origin file

typedef struct {
  uint32_t Usb20OcPin; // XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
  uint16_t Usb31OcPin; // XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
} XHCI_OC;

typedef struct {
  uint32_t Rsvd0                      : 9;
  uint32_t SkipAllUSBHostAccess       : 1;           ///< Skip USB init
  uint32_t SkipAllUSBPhyAccess        : 1;           ///< Skip USB PHY init
  uint32_t Rsvd1                      : 21;
} FCH_USB_FLAG;

///
/// FCH USB structure
///
typedef struct {
  uint32_t           DieBusNum;                      ///< IOHC Die bus number of USB controllers
  bool               Xhci0Enable;                    ///< XHCI0 controller enable
  bool               Xhci1Enable;                    ///< XHCI1 controller enable
  bool               Xhci2Enable;                    ///< XHCI2 controller enable
  bool               Xhci3Enable;                    ///< XHCI3 controller enable
  uint32_t           XhciSsid;                       ///< XHCI SSID
  bool               XhciECCDedErrRptEn;
  uint32_t           Xhci0DevRemovable;
  bool               DisableXhciPortLate;
  uint32_t           XhciUsb3PortDisable;
  uint32_t           XhciUsb2PortDisable;
  XHCI_OC            XhciOCpinSelect[4];             ///< XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
  bool               XhciOcPolarityCfgLow;
  uint8_t            Usb3PortForceGen1;
  uint64_t           OemUsbConfigurationTable;
  bool               UsbSparseModeEnable;
  bool               UsbDbgSCPipeSwitchEnable;
  FCH_USB_FLAG       UsbFlag;
} FCHUSB_INPUT_BLK;

#pragma pack (pop)
