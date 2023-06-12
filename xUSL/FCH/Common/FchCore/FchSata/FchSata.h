/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSata.h
 * @brief FCH SATA Controller Functions Prototypes
 *
 *
 */

#pragma once

#include <FCH/FchClass-api.h>
#include <FCH/FchSata-api.h>

void
FchSataInitDisableSata (
  uint32_t DieBusNum,
  uint32_t Controller
  );

void
FchSataShutdownUnconnectedSataPortClock (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataAhciInitLate (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataRegsInitLateProgram (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataDisableWriteAccess (
  uint32_t DieBusNum,
  uint32_t Controller
  );

void
FchSataEnableWriteAccess (
  uint32_t DieBusNum,
  uint32_t Controller
  );

void
FchSataPrePcieInit (
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataControllerSetPortGenMode (
  uint32_t DieBusNum,
  uint32_t Controller,
  uint16_t PortMode
  );

void
FchSataSetPortGenMode (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitEnableErr (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitEsata (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitRsmuCtrl (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitCtrlReg (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataGpioInitial (
  uint32_t DieBusNum,
  uint32_t Controller
  );

void
FchSataRaidInitLate (
  uint32_t DieBusNum,
  uint32_t Controller
  );

void
FchSataInitMpssMap (
  uint32_t DieBusNum
  );

void
FchSataEnableSataMac (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataEnableSataStaggeredSpinup (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitEnableSata (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitMmc (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitLate (
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitHideNbifDev1Pci (
  uint32_t DieBusNum,
  uint32_t NbioSata
  );

void
FchSataInitEnv (
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitEnvProgram (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitReset (
  uint32_t          DieBusNum,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataSetIrqIntResource (
  void
  );

void
FchSataSetGpioPad (
  uint32_t DieBusNum
  );

#define FCHSATA_MAJOR_REV     0
#define FCHSATA_MINOR_REV     1
#define FCHSATA_INSTANCE      0
#define SATA_CONTROLLER_NUM   4
#define SATA_PORT_NUM         8

// NBIF
#define SIL_RESERVED_0284           0x13B38404ul
#define SIL_RESERVED_0285 0x00100000ul
