/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioPcieStrapsPhx.h
 * @brief This file contains prototypes for OpenSIL MPIO PCIe Read Write straps
 *
 */

#pragma once

#define STRAP_BIF_PORT_DIFF                 (HOLD_TRAINING_B_INDEX - HOLD_TRAINING_A_INDEX)

#define MPIOSTRAPx6                                   0x6   ///< STRAP_BIF_AER_EN

#define MPIOSTRAPxE                            0xe   ///< Provides an override for STRAP_TPH_SUPPORTED

#define MPIOSTRAPx12                                   0x12  ///< STRAP_BIF_DPC_EN

#define MPIOSTRAPx13                                   0x13  ///< Enable Data Link Feature Extended Capability

#define MPIOSTRAPx14                                  0x14  ///< Enable Physical Layer 16.0 GT/s Extended
///< Capability

#define MPIOSTRAPx15                             0x15  ///< Enable 16.0 GT/s Margining Extended
///< Capability

#define MPIOSTRAPx16                                  0x16  ///< Enable Physical Layer 32.0 GT/s Extended
///< Capability

#define MPIOSTRAPx21                            0x21  ///< Provides an override for STRAP_LTR_SUPPORTED

#define MPIOSTRAPx3E                   0x3e  ///< STRAP_BIF_LC_UPCONFIGURE_SUPPORT

#define MPIOSTRAPx4A                              0x4a  ///< Provides an override for STRAP_ECRC_GEN_EN

#define MPIOSTRAPx4B                            0x4b  ///< Provides an override for
///< STRAP_F0_ECRC_CHECK_EN
#define MPIOSTRAPx5D                         0x5d  ///< STRAP_BIF_CPL_ABORT_ERR_EN

#define MPIOSTRAPx5F                      0x5f  ///< STRAP_BIF_MARGIN_IGNORE_C_SKP

#define MPIOSTRAPx60                   0x60  ///< STRAP_BIF_SLV_SDP_OPT_POOL_CR_EN

#define MPIOSTRAPx6F                             0x6f  ///< STRAP_BIF_MINOR_REV_ID

#define MPIOSTRAPx75                                   0x75  ///< Provides an override for STRAP_ACS_EN

#define MPIOSTRAPx76                    0x76  ///< STRAP_BIF_ACS_SOURCE_VALIDATION

#define MPIOSTRAPx77                 0x77  ///< STRAP_BIF_ACS_TRANSLATION_BLOCKING

#define MPIOSTRAPx78                0x78  ///< STRAP_BIF_ACS_DIRECT_TRANSLATED_P2P

#define MPIOSTRAPx79              0x79  ///< STRAP_BIF_ACS_P2P_COMPLETION_REDIRECT

#define MPIOSTRAPx7A                 0x7a  ///< STRAP_BIF_ACS_P2P_REQUEST_REDIRECT

#define MPIOSTRAPx7B                  0x7b  ///< STRAP_BIF_ACS_UPSTREAM_FORWARDING

#define MPIOSTRAPx7C                              0x7c  ///< STRAP_BIF_SDP_UNIT_ID

#define MPIOSTRAPx88                       0x88  ///< Provides an override for STRAP_F0_ATOMIC_EN

#define MPIOSTRAPx89               0x89  ///< Provides an override for
///< STRAP_F0_ATOMIC_ROUTING_EN

#define MPIOSTRAPx94                                0x94  ///< Kill 32.0 GT/s data rate and PCIe Base 5.0
///< compliant features

#define MPIOSTRAPxB3                            0xb3  ///< STRAP_BIF_SUBSYS_VEN_ID

#define MPIOSTRAPxB4                                0xb4  ///< STRAP_BIF_SUBSYS_ID

#define MPIOSTRAPxB5  0xb5  ///<

#define MPIOSTRAPxB6  0xb6  ///<

#define MPIOSTRAPxB7  0xb7  ///<

#define MPIOSTRAPxB8  0xb8  ///<


#define MPIOSTRAPxE9                      0xe9  ///< STRAP_BIF_SWUS_OBFF_SUPPORTED

#define HOLD_TRAINING_A_INDEX                                    0xed  ///< HOLD_TRAINING_A

#define MPIOSTRAPxFD                      0xfd  ///< STRAP_BIF_TARGET_LINK_SPEED_A

#define MPIOSTRAPx10A                             0x10a  ///< STRAP_BIF_PM_SUPPORT_A

#define MPIOSTRAPx10B                        0x10b  ///< STRAP_BIF_L1_EXIT_LATENCY_A

#define MPIOSTRAPx10C                       0x10c  ///< STRAP_BIF_L0S_EXIT_LATENCY_A

#define MPIOSTRAPx114                        0x114  ///< STRAP_BIF_DE_EMPHASIS_SEL_A

#define MPIOSTRAPx138                     0x138 ///< STRAP_BIF_LC_PRESET_MASK_8GT_A
#define MPIOSTRAPx139                    0x139  ///< STRAP_BIF_LC_PRESET_MASK_16GT_A

#define MPIOSTRAPx13F                 0x13f  ///< STRAP_BIF_EXTENDED_FMT_SUPPORTED_A

#define MPIOSTRAPx140                          0x140  ///< STRAP_BIF_E2E_PREFIX_EN_A

#define MPIOSTRAPx145        0x145  ///< DEVICE_CAP2.TEN_BIT_TAG_COMPLETER_
///< SUPPORTED[16]. PCIe Base r4.0 feature.

#define MPIOSTRAPx146        0x146  ///< DEVICE_CAP2.TEN_BIT_TAG_REQUESTER_
///< SUPPORTED[17]. PCIe Base r4.0 feature.

#define MPIOSTRAPx154                          0x154  ///< STRAP_BIF_INITIAL_N_FTS_A

#define MPIOSTRAPx158                  0x158  ///< PCI-PM L1.2 is supported when Set

#define MPIOSTRAPx159                  0x159  ///< PCI-PM L1.1 is supported when Set

#define MPIOSTRAPx15A                    0x15a  ///< ASPM L1.2 is supported when Set

#define MPIOSTRAPx15B                    0x15b  ///< ASPM L1.1 is supported when Set

#define MPIOSTRAPx15C              0x15c  ///< STRAP_BIF_L1_PM_SUBSTATES_SUPPORTED_A

#define MPIOSTRAPx15E                        0x15e  ///< Sets value of Tcommonmode (in us) which
///< must be used by the Downstream Port for
///< timing the re-establishment of common
///< mode

#define MPIOSTRAPx15F                           0x15f  ///< STRAP_BIF_T_P_ON_SCALE_A

#define MPIOSTRAPx161                           0x161  ///< STRAP_BIF_T_P_ON_VALUE_A

#define MPIOSTRAPx163                    0x163  ///< Data Link Feature Capabilities - Local Data
///< Link Feature Supported (Base 4.0
///< compliance)

#define MPIOSTRAPx164                        0x164  ///< Data Link Feature Capabilities - Data Link
///< Feature Exchange Enable (Base 4.0
///< compliance)

#define HOLD_TRAINING_B_INDEX                                    0x168  ///< HOLD_TRAINING_B

void
WritePcieStrapPhx (
  SIL_CONTEXT    *SilContext,
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  );
