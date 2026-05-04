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

#include <stdint.h>
#include <stdbool.h>

#define PROMCLASS_DATA_SIZE sizeof(PROMCLASS_DATA_BLK)

#define PROM21_NUM_PCIE_LANES 12
#define PROM21_NUM_PCIE_CLKREQ 6
#define PROM21_NUM_SATA_PORTS 4
#define PROM21_XHCI_NUM_USB3_PORTS 6
#define PROM21_XHCI_NUM_USB2_PORTS 12
#define PROM21L4_XHCI_NUM_USB3_PORTS 4
#define PROM21L4_XHCI_NUM_USB2_PORTS 10

typedef struct {
  uint8_t  PT21SataPortGen1Swing; // 0x3
  uint8_t  PT21SataPortGen2Swing; // 0x3
  uint8_t  PT21SataPortGen3Swing; // 0xf
  uint8_t  PT21SataPortGen1EmpLevel; // 0x1
  uint8_t  PT21SataPortGen2EmpLevel; // 0x1
  uint8_t  PT21SataPortGen3EmpLevel; // 0xa
} PROM21_SATA_PHY_TUNING;

typedef struct {
  uint8_t  PT21USB3PortGen1Swing; // 0xf
  uint8_t  PT21USB3PortGen1EmpLevelEn; // 0x1
  uint8_t  PT21USB3PortGen1EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen1PreshootEn; // 0x0
  uint8_t  PT21USB3PortGen1Preshoot; // 0x0
  uint8_t  PT21USB3PortGen2Swing; // 0xf
  uint8_t  PT21USB3PortGen2Cp0EmpLevelEn; // 0x1
  uint8_t  PT21USB3PortGen2Cp0EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen2Cp0PreshootEn; // 0x1
  uint8_t  PT21USB3PortGen2Cp0Preshoot; // 0x0
  uint8_t  PT21USB3PortGen2Cp13EmpLevelEn; // 0x0
  uint8_t  PT21USB3PortGen2Cp13EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen2Cp13PreshootEn; // 0x1
  uint8_t  PT21USB3PortGen2Cp13Preshoot; // 0x0
  uint8_t  PT21USB3PortGen2Cp14EmpLevelEn; // 0x1
  uint8_t  PT21USB3PortGen2Cp14EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen2Cp14PreshootEn; // 0x0
  uint8_t  PT21USB3PortGen2Cp14Preshoot; // 0x0
  uint8_t  PT21USB3PortGen2Cp15EmpLevelEn; // 0x3
  uint8_t  PT21USB3PortGen2Cp15EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen2Cp15PreshootEn; // 0x0
  uint8_t  PT21USB3PortGen2Cp15Preshoot; // 0x0
  uint8_t  PT21USB3PortGen2Cp16EmpLevelEn; // 0x3
  uint8_t  PT21USB3PortGen2Cp16EmpLevel; // 0x3
  uint8_t  PT21USB3PortGen2Cp16PreshootEn; // 0x0
  uint8_t  PT21USB3PortGen2Cp16Preshoot; // 0x0
} PROM21_USB3_PHY_TUNING;

typedef struct {
  uint8_t  PT21USB2P0SlewRate; // 0x2
  uint8_t  PT21USB2P0DrivingCurrent; // 0x0
  uint8_t  PT21USB2P0Termination; // 0x3
} PROM21_USB2_PHY_TUNING;

typedef struct {
  uint8_t  PT21PciePortLaneRev[PROM21_NUM_PCIE_LANES / 2]; // 0xf
  uint8_t  PT21SIProgEnable; // 0xf
  uint8_t  PT21ThermalThrottle; // 0xf
  uint8_t  PT21ThermalThreshold; // 0xC0
  uint8_t  PT21LtrSmallEnable; // 0xf
  uint8_t  PT21PcieGen1SwingEnable; // 0xf
  uint8_t  PT21PcieGen1Swing[PROM21_NUM_PCIE_LANES]; // {0x1f, 0x1f, 0x1f, 0x1f, 0xf, 0xf, 0xf, 0xf, 0x1f, 0x1f, 0x1f, 0x1f}
  uint8_t  PT21EqPreset; // 0xf
  uint8_t  PT21PcieClkreqPinSelect[PROM21_NUM_PCIE_CLKREQ]; // { 0x8, 0x4, 0x0, 0x2, 0x3, 0xE }
  uint8_t  PT21PcieClkreqMode[PROM21_NUM_PCIE_CLKREQ]; // { 0x0, 0x0, 0x0, 0x0, 0x0, 0x2 }
  uint8_t  PT21PciePortTargetSpeed[PROM21_NUM_PCIE_LANES]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21PciePortEnable[PROM21_NUM_PCIE_LANES]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21Usb3GenSelect; // 0xf
  uint8_t  PT21XhciPortGen[PROM21_XHCI_NUM_USB3_PORTS]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21L4Usb3Port[PROM21L4_XHCI_NUM_USB3_PORTS]; // { 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21L4Usb2Port[PROM21L4_XHCI_NUM_USB2_PORTS]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21Usb3ort[PROM21_XHCI_NUM_USB3_PORTS]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21Usb2Port[PROM21_XHCI_NUM_USB2_PORTS]; // { 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf }
  PROM21_SATA_PHY_TUNING PT21SataPhy[PROM21_NUM_SATA_PORTS];
  PROM21_USB3_PHY_TUNING PT21USB3Phy[PROM21_XHCI_NUM_USB3_PORTS];
  PROM21_USB2_PHY_TUNING PT21USB2Phy[PROM21_XHCI_NUM_USB2_PORTS / 2];
  uint8_t  PT21SataMode; // 0x00
  uint8_t  PT21SataAggressiveDevSlp[PROM21_NUM_SATA_PORTS]; // { 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21SataAggrLinkPmCap; // 0xf
  uint8_t  PT21SataPscCap; // 0xf
  uint8_t  PT21SataSscCap; // 0xf
  uint8_t  PT21SataPortMdPort[PROM21_NUM_SATA_PORTS]; // { 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21SataHotPlug; // 0xf
  uint8_t  PT21SataPTSataCCCSCap; // 0xf
  uint8_t  PT21AhciMsiCap; // 0xf
  uint8_t  PT21SataPortEnable[PROM21_NUM_SATA_PORTS]; // { 0xf, 0xf, 0xf, 0xf }
  uint8_t  PT21Msi; // 0xf
  uint8_t  PT21Msix; // 0xf
  uint8_t  PT21HW_LPM; // 0xf
  uint8_t  PT21DbC; // 0xf
  uint8_t  PT21XHC_PME; // 0xf
  uint8_t  PT21Lock; // 0x01
  uint8_t  PT21XhciLock; // 0x00
  uint32_t PT21XhciID; // 0x00000000
  uint32_t PT21GppPcieAddress; // 0x00000000
  uint32_t PT21XhciMmio; // 0x00000000
  uint32_t PT21GpioMmio; // 0x00000000
  uint32_t PT21PcieDspSsid; // 0x33281B21
  uint32_t PT21PcieDspXhciSsid; // 0x33281B21
  uint32_t PT21PcieDspAhciSsid; // 0x33281B21
  uint8_t  PT21SsidOverride; // 0xf
  uint32_t PT21XhciSsid; // 0x11421B21
  uint32_t PT21AhciSsid; // 0x10621B21
  uint32_t PT21PcieUspSsid; // 0x33281B21
} PROM21_DATA_BLK;

///  PROM openSIL Input Block
typedef struct {
  uint32_t PT21FwInRamAddress; // 0x0B000000
  uint32_t PT21GpioID; // 0x00000000
  uint64_t PT21FwVersion; // 0x0000000000000000
  uint8_t  PT21UsbPortLateDisable; // 0xf
  uint8_t  PT21TempBusNum; // 0x10
  uint8_t  PT21RootBridgeNum; // 0x00
  uint8_t  PT21PcieTargetSpeed; // 0x04
  uint8_t  PT21IoHcBusNum; // 0x00
  uint32_t PT21IohcBridgeCntl; // 0x00000000
  uint8_t  PT21FWLoading; // 0x1
  uint8_t  PT21DbgLoadFw; // 0x00
  uint8_t  PT21DisableUnusedPciePort; // 0xf
  uint8_t  PT21SecondPortNumber; // 0xf
  uint8_t  PT21Revision; // 0x2
  uint8_t  PT21TogglePerst; // 0xf
  uint8_t  PT21RuninRam; // 0x0
  uint8_t  PT21ClkPMEnable; // 0xf
  uint8_t  PT21L1Enable; // 0x1
  uint8_t  PT21L1ssEnable; // 0xf
  uint8_t  PT21ReadThermal; // 0xf
  uint8_t  PT21GpioPerstEnable; // 0xf
  uint8_t  PT21GpioTestEnable; // 0xf
  uint8_t  PT21DelayAfterFWLoading; // 0xf
  uint8_t  PT21XhciPmeEn; // 0xf
  uint32_t PT21CorrErrMask; // 0x6000
  PROM21_DATA_BLK Primary;
  PROM21_DATA_BLK Secondary;
} PROMCLASS_DATA_BLK;
