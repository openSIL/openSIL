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

///
/// USB Enable Structure
///
typedef struct {
  uint32_t usb_hc_0             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_1             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_2             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_3             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_4             : 1;  ///< 1: Controller enabled
  uint32_t reserved0            : 11; ///< spare
  uint32_t usb4_rt_0            : 1;  ///< 1: Controller enabled; RMB/PHX/STX: pair with usb_hc_3
  uint32_t usb4_rt_1            : 1;  ///< 1: Controller enabled; RMB.PHX/STX: pair with usb_hc_4
  uint32_t reserved1            : 6;  ///< spare
  uint32_t usb4_phy_0           : 1;  ///< 1: Controller enabled
  uint32_t usb4_phy_1           : 1;  ///< 1: Controller enabled
  uint32_t reserved2            : 5;  ///< spare
  uint32_t usb_init_combined    : 1;  ///< 1: USB 3 and 4 initializations are combined
} USB_ENABLE;

///
/// USB Combo Phy Static Config Structure
///
typedef struct {
  uint32_t usb_hc_0             : 8;  ///< 8: Controller 0
  uint32_t usb_hc_1             : 8;  ///< 8: Controller 1
  uint32_t usb_hc_2             : 8;  ///< 8: Controller 2, RMB/PHX/STX: Unsupported
  uint32_t reserved0            : 8;  ///< spare
  uint32_t usb_hc_3             : 8;  ///< 8: Controller 3
  uint32_t usb_hc_4             : 8;  ///< 8: Controller 4
  uint32_t reserved1            : 16; ///< spare
} USB_COMBO_PHY_STATIC_CONFIG;

///
/// USB Controller Config Structure
///
typedef struct {
  uint32_t usb_hc_0_ss_port0_disable    : 1;  ///< 8: Disable Controller 0 - SS Port 0 of XHCI HC
  uint32_t usb_hc_0_ss_port1_disable    : 1;  ///< 8: Disable Controller 0 - SS Port 1 of XHCI HC
  uint32_t usb_hc_0_ss_port2_disable    : 1;  ///< 8: Disable Controller 0 - SS Port 2 of XHCI HC
  uint32_t usb_hc_0_ss_port3_disable    : 1;  ///< 8: Disable Controller 0 - SS Port 3 of XHCI HC
  uint32_t usb_hc_1_ss_port0_disable    : 1;  ///< 8: Disable Controller 1 - SS Port 0 of XHCI HC
  uint32_t usb_hc_1_ss_port1_disable    : 1;  ///< 8: Disable Controller 1 - SS Port 1 of XHCI HC
  uint32_t usb_hc_1_ss_port2_disable    : 1;  ///< 8: Disable Controller 1 - SS Port 2 of XHCI HC
  uint32_t usb_hc_1_ss_port3_disable    : 1;  ///< 8: Disable Controller 1 - SS Port 3 of XHCI HC
  uint32_t reserved0                    : 16; ///< spare
  uint32_t usb_hc_3_ss_port0_disable    : 1;  ///< 8: Disable Controller 3 - SS Port 0 of USB3 Adapter HC
  uint32_t usb_hc_4_ss_port0_disable    : 1;  ///< 8: Disable Controller 4 - SS Port 0 of USB3 Adapter HC
  uint32_t reserved1                    : 6;  ///< spare
} USB_CTRLR_CONFIG;

///
/// BIOSSMC MSG UsbInit Data Structure
///
typedef struct {
  USB_ENABLE                  Enable;               ///< USB Init enable (DW0)
  USB_COMBO_PHY_STATIC_CONFIG ComboPhyStaticConfig; ///< Combo Phy Static Config (DW1 & DW2)
  USB_CTRLR_CONFIG            UsbControllerConfig;  ///< USB Controller Config (DW3)
  uint16_t                    NumUsb3Entries;       ///< Usb3 entry number
  uint16_t                    NumUsb4Entries;       ///< Usb4 entry number
  uint16_t                    Usb3EntriesOffset;    ///< Usb3 offset, 0 means no entry
  uint16_t                    Usb4EntriesOffset;    ///< Usb4 offset, 0 means no entry
} USB_INIT_DATA;

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
  uint64_t           OemUsbConfigurationTable;       ///< FCH_TC_USB_OEM_PLATFORM_TABLE
  USB_INIT_DATA      UsbInitData;
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
