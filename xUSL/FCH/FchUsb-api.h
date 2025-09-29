/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsb-api.h
 * @brief FCH USB input block definitions
 *
 */

#pragma once

#include <stdbool.h>

#define FCHUSB_MAJOR_REV            0
#define FCHUSB_MINOR_REV            1
#define FCHUSB_INPUT_BLK_INSTANCE   0

#define FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE  1
#define FCH_BIOSSMC_MSG_INPUT_BLK_MAJOR_REV 0
#define FCH_BIOSSMC_MSG_INPUT_BLK_MINOR_REV 1

#pragma pack (push, 1)  // match the packing of its origin file

/**
 * Structure for defining the SMU-BIOS message interface (request IDs)
 */
typedef struct {
  uint8_t UsbInit;
  uint8_t UsbConfigUpdate;
  uint8_t UsbSxEntry;
  uint8_t UsbSxExit;
  uint8_t DisableUsbDbgClk;
} FCH_BIOSSMC_MSG_INPUT_BLK;

/**
 * Structure for OC pin select for each USB controller
 *
 * Union to account for use of different grouping methods (grouping by controller vs. by USB version).
 */
typedef union {
  struct {
    uint32_t Usb20OcPin; // OC pin select for USB 2 ports, [3:0] - Port0, [7:4] - Port1, etc.
    uint16_t Usb31OcPin; // OC pin select for USB 3 ports, [3:0] - Port0, [7:4] - Port1, etc.
  } Field;
  uint32_t OcPinSelect; // OC pin select for the whole controller, [3:0] - Port0, [7:4] - Port1, etc.
} XHCI_OC;

typedef struct {
  uint32_t Rsvd0                      : 9;
  uint32_t SkipAllUSBHostAccess       : 1;           ///< Skip USB init
  uint32_t SkipAllUSBPhyAccess        : 1;           ///< Skip USB PHY init
  uint32_t Rsvd1                      : 21;
} FCH_USB_FLAG;

///
/// USB4 Host
///
typedef struct {
  bool               InitEnable;                  ///< USB4 overall Init enable
  bool               HostEnable;                  ///< USB4 Host Router enable
  uint8_t            Usb3HCDisable;               ///< USB3 HC (connected to USB4) disable
  uint8_t            PcieTunnelingDisable;        ///< PCIe Tunneling disable
  uint8_t            Usb3TunnelingDisable;        ///< USB3 Tunneling disable
  bool               DPTunnelingDisable;          ///< DP Tunneling disable
  bool               Tbt3NotSupport;              ///< Not Support TBT3 Compatibility
  uint8_t            PcieAdpHidden;               ///< USB4 PCIe Adapter Hidden
  uint8_t            Usb3AdpHidden;               ///< USB4 USB3 Adapter Hidden
  uint8_t            DPAdpHidden;                 ///< USB4 DP Adapter Hidden
  bool               Gen3Disable;                 ///< USB4 Gen3 disable
  uint8_t            PhyEnable;                   ///< USB4 PHY enable
  uint8_t            Gen3Support;                 ///< USB4 Gen3 Support
  uint8_t            TxFFEMode;                   ///< TxFFE Mode
  uint8_t            TxFFEPreSetValue;            ///< TxFFE Pre-Set Value bit[3:0] - Lane0; bit[7:4] Lane1.
  uint8_t            ClxEnable;                   ///< CLx Enable
  uint8_t            P4PgEnable;                  ///< P4.PG Enable
  uint8_t            LinkSpeed;                   ///< Link Speed - 0: Gen2; 1: Gen2/Gen3
  uint8_t            OnboardRetimer;              ///< Onboard Retimer
  bool               ZprEnable;                   ///< Debug Streaming Control Enable
  uint8_t            SSPortDisable;               ///< SuperSpeed Port Disable
  uint8_t            Reserved[9];
} FCH_USB4_HOST;

///
/// FCH USB structure
///
typedef struct {
  uint32_t           DieBusNum;                      ///< IOHC Die bus number of USB controllers
  FCH_USB_FLAG       UsbFlag;
  /* xHCI input parameters */
  bool               Xhci0Enable;                    ///< XHCI0 controller enable
  bool               Xhci1Enable;                    ///< XHCI1 controller enable
  bool               Xhci2Enable;                    ///< XHCI2 controller enable
  bool               Xhci3Enable;                    ///< XHCI3 controller enable
  uint32_t           XhciSsid;                       ///< XHCI SSID
  bool               XhciECCDedErrRptEn;
  uint32_t           Xhci0DevRemovable;
  uint32_t           XhciUsb3PortDisable;
  uint32_t           XhciUsb2PortDisable;
  XHCI_OC            XhciOCpinSelect[4];             ///< XHCI OverCurrent OC Pin, [3:0] - Port0, [7:4] Port1, etc.
  bool               XhciOcPolarityCfgLow;
  uint8_t            Usb3PortForceGen1;
  uint64_t           OemUsbConfigurationTable;
  bool               UsbSparseModeEnable;
  bool               UsbDbgSCPipeSwitchEnable;
  uint32_t           DdiModeEnable;                  ///< XHCI Combo-PHY DDI mode enable
  bool               UsbDbgClkDisable;
  uint8_t            PdInterruptModeEn;
  /* CIO input parameters */
  FCH_USB4_HOST      Usb4Host[2];
  uint16_t           Usb4DebugSCDisable;             ///< Debug Streaming Control Enable
  bool               Usb4PhyTuningEnable;            ///< USB4 PHY Tuning Enable.
  uint32_t           Usb4PhyTuningSetPtr;            ///< Pointer of USB4 Tuning Set Content
  uint32_t           Usb4PhyTuningSetSize;           ///< Size of USB4 Tuning Set Content
} FCHUSB_INPUT_BLK;

#pragma pack (pop)
