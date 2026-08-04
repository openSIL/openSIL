/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21.h
 *  @brief Promontory21 related defines
 */

#pragma once

#include <SilCommon.h>
#include <PROM/PromClass-api.h>

#define PT_SATA_AHCI_DID             0x43F6
#define PT_SATA_RAID_DID             0x43BD

#define PT_SATA_AHCI_CLASS_CODE      0x010601
#define PT_SATA_RAID_CLASS_CODE      0x010400

#define PROM21_MIN_SATA_SPEED        1
#define PROM21_MAX_SATA_SPEED        3

#define PROM21_MAX_DSP_NUMBER        13
#define PROM21_MAX_LANE_REVERSAL_DSP_NUMBER  11

#define PROM21_XHCI_DSP_DEV          12
#define PROM21_SATA_DSP_DEV          13

#define PT21_SATA_ID                  0x43F61022

#define PT21_XHCI_ID_L1               0x43F71022
#define PT21_XHCI_ID_L2               0x43F81022
#define PT21_XHCI_ID_L3               0x43F91022
#define PT21_XHCI_ID_L4               0x43FA1022
#define PT21_XHCI_ID_L5               0x43FB1022
#define PT21_XHCI_ID_L6               0x43FC1022
#define PT21_XHCI_ID_L7               0x43FD1022
#define PT21_XHCI_ID_L8               0x43FE1022

#define PT21_XHCI_DID_L1              0x43F7
#define PT21_XHCI_DID_L2              0x43F8
#define PT21_XHCI_DID_L3              0x43F9
#define PT21_XHCI_DID_L4              0x43FA
#define PT21_XHCI_DID_L5              0x43FB
#define PT21_XHCI_DID_L6              0x43FC
#define PT21_XHCI_DID_L7              0x43FD
#define PT21_XHCI_DID_L8              0x43FE

#define PT21_USP_ID                   0x43F41022
#define PT21_DSP_ID                   0x43F51022

#define PORM21_LANE_REVERSAL_REG(a)      (0x28003L + (a * 0x400))

typedef union {
  struct {
    uint8_t SubVersion[3];
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;
    uint8_t unused[2];
  } Fields;
  uint64_t Raw;
} PROM21_FW_VERSION;

static const uint32_t r_warm_rst_assert[] = {
  0x19220,
  0x1A220,
  0x1B220,
  0x1C220,
  0x1D220,
  0x1E220
};

static const uint32_t r_force_superspeed[] = {
  0x19211,
  0x1A211,
  0x1B211,
  0x1C211,
  0x1D211,
  0x1E211
};

static const uint32_t hw_lpm_en[] = {
  0x1A58C,
  0x1C58C,
  0x1E58C
};

static const uint32_t SATA_gen1gen2_swing[] = {
  0x2D188,
  0x2D388,
  0x2D588,
  0x2D788
};

static const uint32_t SATA_gen3_swing[] = {
  0x2D189,
  0x2D389,
  0x2D589,
  0x2D789
};

static const uint32_t SATA_gen1_emphasis[] = {
  0x2D18B,
  0x2D38B,
  0x2D58B,
  0x2D78B
};

static const uint32_t SATA_gen2_emphasis[] = {
  0x2D18C,
  0x2D38C,
  0x2D58C,
  0x2D78C
};

static const uint32_t SATA_gen3_emphasis[] = {
  0x2D18D,
  0x2D38D,
  0x2D58D,
  0x2D78D
};

static const uint32_t SATA_gen_reg[] = {
  0x2D12BL,
  0x2D32BL,
  0x2D52BL,
  0x2D72BL
};

static const uint32_t USB3_gen1_swing[] = {
  0x19490,
  0x1A490,
  0x1B490,
  0x1C490,
  0x1D490,
  0x1E490
};

typedef enum {
  TxEmphasis = 0,
  TxPreshoot,
  TxSquelch
} USB3_TX;

static const uint32_t USB3_gen1_emphasis_preshoot[] = {
  0x19250,
  0x1A250,
  0x1B250,
  0x1C250,
  0x1D250,
  0x1E250
};

static const uint32_t USB3_gen1_squelch[] = {
  0x19480
};

static const uint32_t USB3_gen2_swing[] = {
  0x194A0,
  0x1A4A0,
  0x1B4A0,
  0x1C4A0,
  0x1D4A0,
  0x1E4A0
};

typedef enum {
  Usb3_Cp0_Cp9 = 0,
  Usb3_Cp13 = 13,
  Usb3_Cp14,
  Usb3_Cp15,
  Usb3_Cp16
} USB3_COMPLIANCE_PATTERN;

static const uint32_t USB3_gen2_cp0_cp9_emphasis_preshoot[] = {
  0x19252,
  0x1A252,
  0x1B252,
  0x1C252,
  0x1D252,
  0x1E252
};

static const uint32_t USB3_gen2_cp13_emphasis_preshoot[] = {
  0x1925C,
  0x1A25C,
  0x1B25C,
  0x1C25C,
  0x1D25C,
  0x1E25C
};

static const uint32_t USB3_gen2_cp14_emphasis_preshoot[] = {
  0x1925E,
  0x1A25E,
  0x1B25E,
  0x1C25E,
  0x1D25E,
  0x1E25E
};

static const uint32_t USB3_gen2_cp15_emphasis_preshoot[] = {
  0x19260,
  0x1A260,
  0x1B260,
  0x1C260,
  0x1D260,
  0x1E260
};

static const uint32_t USB3_gen2_cp16_emphasis_preshoot[] = {
  0x19262,
  0x1A262,
  0x1B262,
  0x1C262,
  0x1D262,
  0x1E262
};

static const uint32_t USB2_tx_reg[] = {
  0x1A598,
  0x1A599,
  0x1A598,
  0x1A59A,
  0x1C598,
  0x1C599,
  0x1C598,
  0x1C59A,
  0x1E598,
  0x1E599,
  0x1E598,
  0x1E59A 
};

typedef enum {
  SlewRate = 0,
  DrivingCurrent,
  Termination
} USB2_TX;

typedef enum {
  PTSataAhci = 0,
  PTSataRaid
} SATA_CLASS;

typedef enum {
  Usb3Port0 = 0,
  Usb3Port1,
  Usb3Port2,
  Usb3Port3,
  Usb3Port4,
  Usb3Port5,
  Usb2Port0,
  Usb2Port1,
  Usb2Port2,
  Usb2Port3,
  Usb2Port4,
  Usb2Port5,
  Usb2Port6,
  Usb2Port7,
  Usb2Port8,
  Usb2Port9,
  Usb2Port10,
  Usb2Port11
} USB_PORTS;

static const uint8_t USBGen2by2_port_mapping[] = {
  BIT_8(1), BIT_8(2), BIT_8(3), BIT_8(4), BIT_8(5), BIT_8(6),
  BIT_8(0), BIT_8(2), BIT_8(3), BIT_8(4), BIT_8(5), BIT_8(6), BIT_8(7),
  BIT_8(0), BIT_8(1), BIT_8(2),
  BIT_8(1),
  BIT_8(3)
};

static const uint8_t USBGen2by1_port_mapping[] = {
  BIT_8(1), BIT_8(2), BIT_8(3), BIT_8(4), BIT_8(5), BIT_8(6),
  BIT_8(0), BIT_8(1), BIT_8(2), BIT_8(3), BIT_8(4), BIT_8(5), BIT_8(6), BIT_8(7),
  BIT_8(0), BIT_8(1), BIT_8(2), BIT_8(3)
};

static const uint8_t USBL4_port_mapping[] = {
  BIT_8(1), BIT_8(2), BIT_8(3), BIT_8(4), BIT_8(5), BIT_8(6),
  BIT_8(0), BIT_8(1), BIT_8(2), BIT_8(3), BIT_8(6), BIT_8(7),
  BIT_8(0), BIT_8(1), BIT_8(2), BIT_8(3)
};

typedef enum {
  PcieEndPoint,
  LegacyPcieEndPoint,
  PcieRootPort = 0x4,
  PcieUpstreamPort = 0x5,
  PcieDownstreamPort = 0x6,
  PcieBridge = 0x8
} PCIE_TYPE;

void
Prom21AhciMsiCap (
  uint32_t  XhciMmio,
  bool      Enable
  );

void
Prom21AhciPortSataSpeed (
  uint32_t XhciMmio,
  uint8_t  PortNum,
  uint8_t  Speed
  );

void
Prom21XhciGen (
  uint32_t XhciMmio,
  uint8_t  XhciGen,
  uint8_t  PortNum
  );

void
Prom21UsbPortSetting (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t XhciId,
  uint32_t XhciMmio,
  uint8_t  BootMode
  );

void
Prom21UsbSetting (
  PROM21_DATA_BLK *DataBlk,
  uint32_t        XhciMmio,
  uint8_t         BootMode
  );

void
Prom21SataSetting (
  PROMCLASS_DATA_BLK *PromDataBlk
  );

SIL_STATUS
Prom21Setting (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  );

SIL_STATUS
Prom21LateSetting (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  );

void
Prom21SIConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  );

void
ScanSecXhciDidVid (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           GppBus,
  uint8_t            GppDev,
  uint8_t            GppFun,
  uint16_t           TempBusNum
  );

SIL_STATUS
AmdProm21FwLoad (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  );

SIL_STATUS
AmdProm21FwLoadA1 (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  );
