/**
 * @file  FchSataCmn2Kl.h
 * @brief Kunlun FCH SATA controller revision specific transfer header
 */
/* Copyright 2025 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCore/FchSata/FchSata.h>

/*
 * Function prototypes
 */

void
FchSataInitHideSataPciKl (
  uint32_t   DieBusNum,
  uint32_t   Controller
  );

void
FchSataInitMsiKl (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataSetBISTLComplianceModeKl (
  uint32_t   DieBusNum,
  uint32_t   Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

void
FchSataInitDevSlpKl (
  uint32_t  DieBusNum,
  uint32_t  Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/*
 * Declare macros here
 */

//  Sata: NBIF1DEVINDCFG1 IOHC_Bridge_CNTL
#define FCH_SN_NBIF1DEVINDCFG1_IOHC_Bridge_CNTL               0x13B38404ul
#define FCH_SN_NBIF1DEVINDCFG1_IOHC_Bridge_CNTL_NBIO_STEP     0x00100000ul

#define FCH_SN_SATA0_SMN_PCICFG       0x10148000ul
#define FCH_SN_SATA1_SMN_PCICFG       0x10149000ul
#define FCH_SN_SATA2_SMN_PCICFG       0x10348000ul
#define FCH_SN_SATA3_SMN_PCICFG       0x10349000ul
#define SIL_RESERVED_47  0x10135000ul
#define SIL_RESERVED_45  0x1013500Cul
#define SIL_RESERVED_46  0x10135034ul
#define SIL_RESERVED_44  0x10135200ul
#define SIL_RESERVED_42  0x1013520Cul
#define SIL_RESERVED_43  0x10135234ul
#define SIL_RESERVED_41  0x10335000ul
#define SIL_RESERVED_39  0x1033500Cul
#define SIL_RESERVED_40  0x10335034ul
#define SIL_RESERVED_38  0x10335200ul
#define SIL_RESERVED_36  0x1033520Cul
#define SIL_RESERVED_37  0x10335234ul
