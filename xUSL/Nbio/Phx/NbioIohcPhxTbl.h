/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioIohcPhxTbl.h
 * @brief This file contains the Nbio Iohc configurations
 *
 */

#pragma once

#include <includePHX/PHX_IOHC.h>
#include <includePHX/PHX_IOAGR.h>
#include <includePHX/PHX_SST.h>

// --------------------------------------------------
// Enabling Peer-to-Peer
// --------------------------------------------------
  #define NBIO_IOHC_P2P_TBL \
          SMN_ENTRY_RMW(IOHCx13b10118, \
  BITS_1_TO_2_MASK_AAAAATN, \
  (0x1 << IOHC_FEATURE_CNTL_P2P_mode_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOHCx13b10230, \
  PCIE_VDM_CNTL2_VdmP2pMode_MASK, \
  (0x1 << PCIE_VDM_CNTL2_VdmP2pMode_OFFSET) \
  ),

// --------------------------------------------------
// PCI Express Configuration Cycle Hardware Retry on CRS
// --------------------------------------------------
  #define NBIO_IOHC_CRS_TBL \
          SMN_ENTRY_RMW(IOHCx13b10028, \
  BITS_0_TO_15_MASK_AAAAAVD, \
  0x6 << IOHC_PCIE_CRS_Count_CrsDelayCount_OFFSET \
  ), \
          SMN_ENTRY_RMW(IOHCx13b10028, \
  IOHC_PCIE_CRS_Count_CrsLimitCount_MASK, \
  0x6 << IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET \
  ), \

// --------------------------------------------------
// IOHC Clock Gating
// --------------------------------------------------
  #define NBIO_IOHC_CLOCK_GATING_TBL \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOHCx13b10088, \
  BIT22_MASK_AAAAAUH | \
  BIT23_MASK_AAAAAUF | \
  IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK | \
  BIT25_MASK_AAAAAUB | \
  BIT26_MASK_AAAAATZ | \
  BIT27_MASK_AAAAATX | \
  BIT28_MASK_AAAAATV | \
  BIT29_MASK_AAAAATT | \
  BIT30_MASK_AAAAATR | \
  BIT31_MASK_AAAAATP, \
  (0x0 << BIT22_OFFSET_AAAAAUI) | \
  (0x0 << BIT23_OFFSET_AAAAAUG) | \
  (0x0 << BIT24_OFFSET_AAAAAUE) | \
  (0x0 << BIT25_OFFSET_AAAAAUC) | \
  (0x0 << BIT26_OFFSET_AAAAAUA) | \
  (0x0 << BIT27_OFFSET_AAAAATY) | \
  (0x0 << BIT28_OFFSET_AAAAATW) | \
  (0x0 << BIT29_OFFSET_AAAAATU) | \
  (0x0 << BIT30_OFFSET_AAAAATS) | \
  (0x0 << BIT31_OFFSET_AAAAATQ) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOHCx13b1008c, \
  BIT22_MASK_AAAAAVB | \
  BIT23_MASK_AAAAAUZ | \
  BIT24_MASK_AAAAAUX | \
  BIT25_MASK_AAAAAUV | \
  BIT26_MASK_AAAAAUT | \
  BIT27_MASK_AAAAAUR | \
  BIT28_MASK_AAAAAUP | \
  BIT29_MASK_AAAAAUN | \
  IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
  BIT31_MASK_AAAAAUJ, \
  (0x0 << IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET) | \
  (0x0 << BIT23_OFFSET_AAAAAVA) | \
  (0x0 << BIT24_OFFSET_AAAAAUY) | \
  (0x0 << BIT25_OFFSET_AAAAAUW) | \
  (0x0 << BIT26_OFFSET_AAAAAUU) | \
  (0x0 << BIT27_OFFSET_AAAAAUS) | \
  (0x0 << BIT28_OFFSET_AAAAAUQ) | \
  (0x0 << BIT29_OFFSET_AAAAAUO) | \
  (0x0 << BIT30_OFFSET_AAAAAUM) | \
  (0x0 << BIT31_OFFSET_AAAAAUK) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOAGR0CFGx00000000, \
  BIT22_MASK_K | \
  IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_MASK | \
  BIT24_MASK_I | \
  BIT25_MASK_H | \
  BIT26_MASK_G | \
  BIT27_MASK_F | \
  BIT28_MASK_E | \
  IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK | \
  IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK | \
  BIT31_MASK_B, \
  (0x0 << BIT22_OFFSET_K) | \
  (0x0 << BIT23_OFFSET_J) | \
  (0x0 << BIT24_OFFSET_I) | \
  (0x0 << BIT25_OFFSET_H) | \
  (0x0 << BIT26_OFFSET_G) | \
  (0x0 << BIT27_OFFSET_F) | \
  (0x0 << BIT28_OFFSET_E) | \
  (0x0 << BIT29_OFFSET_D) | \
  (0x0 << BIT30_OFFSET_C) | \
  (0x0 << BIT31_OFFSET_B) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOAGR0CFGx00000004, \
  BIT31_MASK_O | \
  BIT29_MASK_N | \
  IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
  IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK, \
  (0x0 << BIT31_OFFSET_O) | \
  (0x0 << BIT29_OFFSET_N) | \
  (0x0 << BIT30_OFFSET_M) | \
  (0x0 << BIT31_OFFSET_L) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED, \
  SSTx17400004, \
  BIT0_MASK_AAAAAKP | \
  BIT16_MASK_AAAAAKN | \
  BITS_6_TO_15_MASK_AAAAAKL, \
  (0x1 << BIT0_OFFSET_AAAAAKQ) | \
  (0x1 << BIT16_OFFSET_AAAAAKO) | \
  (0xF0 << BIT6_OFFSET_AAAAAKM) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED, \
  SSTx17500004, \
  BIT0_MASK_AAAAAKP | \
  BIT16_MASK_AAAAAKN | \
  BITS_6_TO_15_MASK_AAAAAKL, \
  (0x1 << BIT0_OFFSET_AAAAAKQ) | \
  (0x1 << BIT16_OFFSET_AAAAAKO) | \
  (0xF0 << BIT6_OFFSET_AAAAAKM) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED, \
  SSTx17400404, \
  BIT4_MASK_AAAAAKF, \
  (0x1 << BIT4_OFFSET_AAAAAKG) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED, \
  SSTx17500404, \
  BIT4_MASK_AAAAAKF, \
  (0x1 << BIT4_OFFSET_AAAAAKG) \
  ),

// --------------------------------------------------
// IOHC Power Gating
// --------------------------------------------------
  #define NBIO_IOHC_POWER_GATING_TBL \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOHCx13b10340, \
  BIT8_MASK_AAAAAVH, \
  (0x1 << BIT8_OFFSET_AAAAAVI) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOAGR0CFGx00000080, \
  BIT8_MASK_P, \
  (0x1 << BIT8_OFFSET_P) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOHCx13b10348, \
  BITS_0_TO_1_MASK_AAAAATJ, \
  (0x1 << BIT0_OFFSET_AAAAATK) \
  ), \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOAGR0CFGx00000090, \
  BIT0_MASK_A, \
  (0x1 << BIT0_OFFSET_A) \
  ),

// --------------------------------------------------
// IOHC Arbitration Control
// --------------------------------------------------
  #define NBIO_IOHC_ARBITRATION_TBL \
          SMN_ENTRY_RMW(IOHCx13b14040, \
  BITS_0_TO_3_MASK_AAAAAVJ | \
  IOHC_QOS_CONTROL_VC1QoSPriority_MASK | \
  BITS_8_TO_11_MASK_AAAAAVN | \
  BITS_12_TO_15_MASK_AAAAAVP | \
  BITS_16_TO_19_MASK_AAAAAVR | \
  IOHC_QOS_CONTROL_VC5QoSPriority_MASK | \
  BITS_24_TO_27_MASK_AAAAAVV | \
  BITS_28_TO_31_MASK_AAAAAVX, \
  (0x0 << IOHC_QOS_CONTROL_VC0QoSPriority_OFFSET) | \
  (0x0 << BIT4_OFFSET_AAAAAVM) | \
  (0x0 << BIT8_OFFSET_AAAAAVO) | \
  (0x0 << BIT12_OFFSET_AAAAAVQ) | \
  (0x0 << BIT16_OFFSET_AAAAAVS) | \
  (0x0 << BIT20_OFFSET_AAAAAVU) | \
  (0x0 << BIT24_OFFSET_AAAAAVW) | \
  (0x0 << BIT28_OFFSET_AAAAAVY) \
  ),

// --------------------------------------------------
// IOHC POISON_ACTION_CONTROL
// --------------------------------------------------
  #define NBIO_IOHC_POISON_ACTION_TBL \
          SMN_ENTRY_RMW(IOHCx13b20814, \
  POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_MASK | \
  BIT3_MASK_AAAAAWD | \
  BIT4_MASK_AAAAAWF, \
  (0x1 << BIT0_OFFSET_AAAAAWC) | \
  (0x1 << BIT3_OFFSET_AAAAAWE) | \
  (0x1 << BIT4_OFFSET_AAAAAWG) \
  ),
