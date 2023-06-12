/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioPcieStrapsBrh.h
 * @brief This file contains prototypes for OpenSIL MPIO PCIe Read Write straps
 *
 */

#pragma once

#include <xSIM.h>
#include <Nbio/Common/GnbDxio.h>

#define STRAP_BIF_PORT_DIFF                 (HOLD_TRAINING_B_INDEX - HOLD_TRAINING_A_INDEX)

#define PORT_STRAP_INDEX(Strap, Port)       (Strap + (Port * STRAP_BIF_PORT_DIFF))

bool
IsEarlyTrainedBmcInWrapperBrh (
  PCIe_WRAPPER_CONFIG    *Wrapper,
  GNB_HANDLE             *GnbHandle
  );

#define SIL_RESERVED_0410                                   0x7
#define SIL_RESERVED_0476                            0xf
#define SIL_RESERVED_0420                                   0x14
#define SIL_RESERVED_0453                             0x16
#define SIL_RESERVED_0456                                  0x18
#define SIL_RESERVED_0451                            0x22
#define SIL_RESERVED_0419                         0x29
#define SIL_RESERVED_0479                            0x3b
#define SIL_RESERVED_0427                              0x4d
#define SIL_RESERVED_0426                            0x4e
#define SIL_RESERVED_0413                         0x63
#define SIL_RESERVED_0452                      0x65
#define SIL_RESERVED_0469      0x67
#define SIL_RESERVED_0408                                   0x7c
#define SIL_RESERVED_0409                  0x82
#define SIL_RESERVED_0466                                   0xa8
#define SIL_RESERVED_0422                                   0xab
#define SIL_RESERVED_0429                            0xad
#define SIL_RESERVED_0428                   0xaf
#define SIL_RESERVED_0415                             0xc1
#define SIL_RESERVED_0416                                   0xc4
#define SIL_RESERVED_0414                                0xc5
#define SIL_RESERVED_0417                                 0xc6
#define SIL_RESERVED_0468                            0xcd
#define SIL_RESERVED_0467                                0xce
#define SIL_RESERVED_0458  0xcf
#define SIL_RESERVED_0460  0xd0
#define SIL_RESERVED_0459  0xd1
#define SIL_RESERVED_0461  0xd2
#define SIL_RESERVED_0432  0xd3
#define SIL_RESERVED_0434  0xd4
#define SIL_RESERVED_0433  0xd5
#define SIL_RESERVED_0435  0xd6
#define SIL_RESERVED_0480                            0xd9
#define SIL_RESERVED_0470                              0xf1
#define SIL_RESERVED_0472                       0xf2
#define SIL_RESERVED_0473                         0xf3
#define SIL_RESERVED_0471                              0xfd
#define HOLD_TRAINING_A_INDEX  0x108
#define SIL_RESERVED_0436   0x11d
#define SIL_RESERVED_0440               0x124
#define SIL_RESERVED_0464                             0x128
#define SIL_RESERVED_0430                       0x12a
#define SIL_RESERVED_0418                        0x132
#define SIL_RESERVED_0441              0x133
#define SIL_RESERVED_0444                      0x138
#define SIL_RESERVED_0446                        0x139
#define SIL_RESERVED_0447                        0x13a
#define SIL_RESERVED_0443                       0x13b
#define SIL_RESERVED_0445                       0x13c
#define SIL_RESERVED_0449                             0x13d
#define SIL_RESERVED_0448                  0x13e
#define SIL_RESERVED_0442     0x144
#define SIL_RESERVED_0454                    0x181
#define SIL_RESERVED_0425                          0x184
#define SIL_RESERVED_0477                   0x187
#define SIL_RESERVED_0478                   0x188
#define SIL_RESERVED_0474        0x189
#define SIL_RESERVED_0475        0x18a
#define SIL_RESERVED_0455                           0x18c
#define SIL_RESERVED_0412                         0x193
#define SIL_RESERVED_0463                  0x19e
#define SIL_RESERVED_0462                  0x19f
#define SIL_RESERVED_0411                    0x1a1
#define SIL_RESERVED_0431              0x1a2
#define SIL_RESERVED_0450                    0x1a9
#define SIL_RESERVED_0421                        0x1aa
#define SIL_RESERVED_0465                                 0x1ad
#define SIL_RESERVED_0457                       0x1ae
#define SIL_RESERVED_0423                                 0x1b0
#define SIL_RESERVED_0424                                  1
#define SIL_RESERVED_0437    0x1b3
#define SIL_RESERVED_0438  0x1c2
#define SIL_RESERVED_0439        0x1c8
#define HOLD_TRAINING_B_INDEX  0x1ca
