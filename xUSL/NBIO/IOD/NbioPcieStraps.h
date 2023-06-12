/**
 * @file  NbioPcieStraps.h
 * @brief This file contains prototypes for OpenSIL NBIO PCIe Read Write straps
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once



#define PORT_STRAP_INDEX(Strap, Port)       (Strap + (Port * SIL_RESERVED_881))


void
ReadPcieStrap (
     GNB_HANDLE    *GnbHandle,
     uint16_t      StrapIndex,
     uint32_t      *Value,
     uint8_t       Wrapper
  );

#define HOLD_TRAINING_A_INDEX                                    0xfb
#define HOLD_TRAINING_B_INDEX                                    0x19e
