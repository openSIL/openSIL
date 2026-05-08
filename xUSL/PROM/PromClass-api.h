/* SPDX-License-Identifier: MIT */
/*********************************************************************
 *  Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 ********************************************************************/

/**
 * @file PromClass-api.h
 * @brief openSIL-Host PROM IP interface
 *
 * @details The Promontory21 (PROM) API adds the capability
 * to initialize PROM devices on the PCIe bus.
 * MPIO firmware is responsible for link training the PROM chipset.
 * 
 *  This file provides the structures details for the Host to configure the
 *  PROM operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page PROM  PROM IP api
 *
 *   The Promontory (PROM) OpenSIL module initializes Promontory21 chipset.
 *
 *   See the 'Files - @ref PromClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <SilCommon.h>

#define PROMCLASS_DATA_SIZE           sizeof(PROMCLASS_DATA_BLK)

#define PROM21_NUM_PCIE_LANES         12
#define PROM21_NUM_PCIE_CLKREQ        6
#define PROM21_NUM_SATA_PORTS         4
#define PROM21_XHCI_NUM_USB3_PORTS    6
#define PROM21_XHCI_NUM_USB2_PORTS    12
#define PROM21L4_XHCI_NUM_USB3_PORTS  4
#define PROM21L4_XHCI_NUM_USB2_PORTS  10
#define PROM21_MAX_GPIO_PIN_NUMBER    24

typedef union {
  struct {
    uint16_t GpioDeBounceTimer:3;
    uint16_t GpioDeBounceTimeoutTh:3;
    uint16_t GpioInterruptOutputEn:1;
    uint16_t GpioInterruptActLevel:1;
    uint16_t GPIOInterruptMode:1;
    uint16_t Reserved2:7;
  } Common;
  uint16_t Raw;
} PROM21_GPIO_COMMON_SETTING;

typedef union {
  struct {
    uint16_t OutEnB:1;
    uint16_t Out:1;
    uint16_t interruptEnable:1;
    uint16_t interruptLevelTrigType:1;
    uint16_t interruptType:2;
    uint16_t interruptMask:1;
    uint16_t Reserved2:9;
  } Gpio;
  uint16_t Raw;
} PROM21_GPIO_SETTING;

typedef struct {
  uint16_t Pin;
  PROM21_GPIO_SETTING Setting;
} PROM21_GPIO_ITEM;

typedef struct {
  PROM21_GPIO_COMMON_SETTING GpioCommon; // unused
  PROM21_GPIO_ITEM           GpioList[PROM21_MAX_GPIO_PIN_NUMBER * 2 + 1];
} PROM21_GPIO_INIT_TABLE;

typedef struct {
  uint8_t  PT21SataPortGen1Swing;
  uint8_t  PT21SataPortGen2Swing;
  uint8_t  PT21SataPortGen3Swing;
  uint8_t  PT21SataPortGen1EmpLevel;
  uint8_t  PT21SataPortGen2EmpLevel;
  uint8_t  PT21SataPortGen3EmpLevel;
} PROM21_SATA_PHY_TUNING;

typedef struct {
  uint8_t  PT21USB3PortGen1Swing;
  uint8_t  PT21USB3PortGen1EmpLevelEn;
  uint8_t  PT21USB3PortGen1EmpLevel;
  uint8_t  PT21USB3PortGen1PreshootEn;
  uint8_t  PT21USB3PortGen1Preshoot;
  uint8_t  PT21USB3PortGen2Swing;
  uint8_t  PT21USB3PortGen2Cp0EmpLevelEn;
  uint8_t  PT21USB3PortGen2Cp0EmpLevel;
  uint8_t  PT21USB3PortGen2Cp0PreshootEn;
  uint8_t  PT21USB3PortGen2Cp0Preshoot;
  uint8_t  PT21USB3PortGen2Cp13EmpLevelEn;
  uint8_t  PT21USB3PortGen2Cp13EmpLevel;
  uint8_t  PT21USB3PortGen2Cp13PreshootEn;
  uint8_t  PT21USB3PortGen2Cp13Preshoot;
  uint8_t  PT21USB3PortGen2Cp14EmpLevelEn;
  uint8_t  PT21USB3PortGen2Cp14EmpLevel;
  uint8_t  PT21USB3PortGen2Cp14PreshootEn;
  uint8_t  PT21USB3PortGen2Cp14Preshoot;
  uint8_t  PT21USB3PortGen2Cp15EmpLevelEn;
  uint8_t  PT21USB3PortGen2Cp15EmpLevel;
  uint8_t  PT21USB3PortGen2Cp15PreshootEn;
  uint8_t  PT21USB3PortGen2Cp15Preshoot;
  uint8_t  PT21USB3PortGen2Cp16EmpLevelEn;
  uint8_t  PT21USB3PortGen2Cp16EmpLevel;
  uint8_t  PT21USB3PortGen2Cp16PreshootEn;
  uint8_t  PT21USB3PortGen2Cp16Preshoot;
} PROM21_USB3_PHY_TUNING;

typedef struct {
  uint8_t  PT21USB2SlewRate;
  uint8_t  PT21USB2DrivingCurrent;
  uint8_t  PT21USB2Termination;
} PROM21_USB2_PHY_TUNING;

typedef struct {
  uint8_t  PT21PciePortLaneRev[PROM21_NUM_PCIE_LANES / 2];
  uint8_t  PT21SIProgEnable;
  uint8_t  PT21ThermalThrottle;
  uint8_t  PT21ThermalThreshold;
  uint8_t  PT21LtrSmallEnable;
  uint8_t  PT21PcieGen1SwingEnable;
  uint8_t  PT21PcieGen1Swing[PROM21_NUM_PCIE_LANES];
  uint8_t  PT21EqPreset;
  uint8_t  PT21GpioPerstEnable;
  uint8_t  PT21UsbPortLateDisable;
  uint8_t  PT21PcieClkreqPinSelect[PROM21_NUM_PCIE_CLKREQ];
  uint8_t  PT21PcieClkreqMode[PROM21_NUM_PCIE_CLKREQ];
  uint8_t  PT21PciePortTargetSpeed[PROM21_NUM_PCIE_LANES];
  uint8_t  PT21PciePortEnable[PROM21_NUM_PCIE_LANES];
  uint8_t  PT21Usb3GenSelect;
  uint8_t  PT21XhciPortGen[PROM21_XHCI_NUM_USB3_PORTS];
  uint8_t  PT21Usb3Port[PROM21_XHCI_NUM_USB3_PORTS];
  uint8_t  PT21Usb2Port[PROM21_XHCI_NUM_USB2_PORTS];
  PROM21_SATA_PHY_TUNING PT21SataPhy[PROM21_NUM_SATA_PORTS];
  PROM21_USB3_PHY_TUNING PT21USB3Phy[PROM21_XHCI_NUM_USB3_PORTS];
  PROM21_USB2_PHY_TUNING PT21USB2Phy[PROM21_XHCI_NUM_USB2_PORTS / 2];
  uint8_t  PT21SataEnable;
  uint8_t  PT21SataMode;
  uint8_t  PT21SataAggressiveDevSlp[PROM21_NUM_SATA_PORTS];
  uint8_t  PT21SataAggrLinkPmCap;
  uint8_t  PT21SataPscCap;
  uint8_t  PT21SataSscCap;
  uint8_t  PT21SataPortMdPort[PROM21_NUM_SATA_PORTS];
  uint8_t  PT21SataHotPlug;
  uint8_t  PT21SataPTSataCCCSCap;
  uint8_t  PT21AhciMsiCap;
  uint8_t  PT21SataPortEnable[PROM21_NUM_SATA_PORTS];
  uint8_t  PT21Msi;
  uint8_t  PT21Msix;
  uint8_t  PT21HW_LPM;
  uint8_t  PT21DbC;
  uint8_t  PT21XHC_PME;
  uint32_t PT21PcieDspSsid;
  uint32_t PT21PcieDspXhciSsid;
  uint32_t PT21PcieDspAhciSsid;
  uint8_t  PT21SsidOverride;
  uint32_t PT21XhciSsid;
  uint32_t PT21AhciSsid;
  uint32_t PT21PcieUspSsid;
} PROM21_DATA_BLK;

typedef struct {
  uint8_t  BootMode;
  uint8_t  PT21TempBusNum;
  uint8_t  PT21PcieTargetSpeed;
  uint8_t  PT21FWLoading;
  uint8_t  PT21DbgLoadFw;
  uint8_t  PT21DisableUnusedPciePort;
  uint8_t  PT21SecondPortNumber;
  uint8_t  PT21Revision;
  uint8_t  PT21TogglePerst;
  uint8_t  PT21RuninRam;
  uint8_t  PT21ClkPMEnable;
  uint8_t  PT21L1Enable;
  uint8_t  PT21L1ssEnable;
  uint8_t  PT21ReadThermal;
  uint8_t  PT21GpioTestEnable;
  uint8_t  PT21DelayAfterFWLoading;
  uint8_t  PT21XhciPmeEn;
  uint32_t PT21CorrErrMask;
  uint32_t PT21FwInRamAddress;
  uint32_t PT21ForceGppPcieAddress;
  PROM21_GPIO_INIT_TABLE PT21GpioInitTable;
  PROM21_DATA_BLK Primary;
  PROM21_DATA_BLK Secondary;
} PROMCLASS_INPUT_BLK;

typedef struct {
  bool     SecondaryPTPresent;
  bool     PT21XhciLock[2];
  uint8_t  PT21IoHcBusNum;
  uint32_t PT21IohcBridgeCntl;
  uint8_t  PT21RootBridgeNum;
  uint64_t PT21FwVersion;
  uint32_t PT21GpioID;
  uint32_t PT21XhciMmio[2];
  uint32_t PT21GpioMmio[2];
  uint32_t PT21XhciID[2];
  uint32_t PT21GppPcieAddress[2];
} PROMCLASS_OUTPUT_BLK;

typedef struct {
  PROMCLASS_INPUT_BLK  PromInputBlk;
  PROMCLASS_OUTPUT_BLK PromOutputBlk;
} PROMCLASS_DATA_BLK;
