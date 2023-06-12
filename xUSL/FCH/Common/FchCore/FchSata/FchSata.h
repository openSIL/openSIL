/**
 * @file  FchSata.h
 * @brief FCH SATA controller functions header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCommonCfg.h>
#include <FCH/Common/FchCore/FchSata/FchSataDef.h>

extern uint64_t SataMultiDiePortShutDown;
extern uint64_t SataMultiDieDevSlp;
extern uint64_t SataMultiDiePortESP;
extern uint8_t SataSgpioMultiDieEnable;
extern uint64_t SataIoDie1PortMode;

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

void
FchInitEnvProgramSata (
    uint32_t  DieBusNum,
    uint32_t  Controller,
    FCH_SATA2 *FchSata
  );

void
FchInitLateSata (
  FCH_SATA2 *FchSata
  );

void
FchInitLateProgramSataRegs (
  uint32_t  DieBusNum,
  uint32_t  Controller,
  FCH_SATA2 *FchSata
  );

FCH_SATA2*
GetFchSataData (void);

void
FchSataPrePcieInit (
  FCH_SATA2 *FchSata
  );

SIL_STATUS
FchSataSetInputBlk (void);

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define FCHSATA_MAJOR_REV     0
#define FCHSATA_MINOR_REV     1
#define FCHSATA_INSTANCE      0

#define FCH_SATA_AHCI_DID     0x7901
#define FCH_SATA_AMDAHCI_DID  0x7904
#define FCH_SATA_RAID_DID     0x7916
#define FCH_SATA_RAID_DID2    0x7917
#define SATA_PORT_NUM         8
#define SATA_CONTROLLER_NUM   4

#define FCH_GPIO_BANK0_PULLUPENABLE BIT_32(20)
#define FCH_GPIO_BANK0_PULLDOWNENABLE BIT_32(21)
