/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioNbifPhxTbl.h
 * @brief This file contains the Nbif device configurations
 *
 */

#pragma once

#include <includePHX/PHX_NBIFMM.h>
#include <includePHX/PHX_NBIFEPFCFG.h>
#include <includePHX/PHX_PCIERCCFG.h>
#include <includePHX/PHX_SYSHUBMM.h>

// --------------------------------------------------
// nBIF Configurations
// --------------------------------------------------
  #define NBIO_NBIF_DEVICE_CFG_TBL \
          SMN_ENTRY_RMW(NBIFMMx1013460c, \
  BIT30_MASK_AAAABEN, \
  (0x1 << BIT30_OFFSET_AAAABEO) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013480c, \
  BIT30_MASK_AAAABEP, \
  (0x1 << BIT30_OFFSET_AAAABEQ) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013600c, \
  BIT30_MASK_AAAABGH, \
  (0x1 << BIT30_OFFSET_AAAABGI) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013660c, \
  BIT30_MASK_AAAABGJ, \
  (0x1 << BIT30_OFFSET_AAAABGK) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013680c, \
  BIT30_MASK_AAAABGL, \
  (0x1 << BIT30_OFFSET_AAAABGM) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx10136a0c, \
  BIT30_MASK_AAAABGN, \
  (0x1 << BIT30_OFFSET_AAAABGO) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx10136c0c, \
  BIT30_MASK_AAAABGP, \
  (0x1 << BIT30_OFFSET_AAAABGQ) \
  ),

// --------------------------------------------------
// ATC/ATS Support
// --------------------------------------------------

  #define NBIO_NBIF_ATC_ATS_TBL \
          SMN_ENTRY_RMW(NBIFMMx10134008, \
  BIT18_MASK_AAAABDL | \
  BITS_9_TO_13_MASK_AAAABDN | \
  BIT27_MASK_AAAABDR | \
  BIT28_MASK_AAAABDT | \
  BIT29_MASK_AAAABDV | \
  BIT31_MASK_AAAABDX, \
  ((0x1 << BIT18_OFFSET_AAAABDM) | \
  (0x10 << BIT9_OFFSET_AAAABDO) | \
  (0x1 << BIT27_OFFSET_AAAABDS) | \
  (0x1 << BIT28_OFFSET_AAAABDU) | \
  (0x1 << BIT29_OFFSET_AAAABDW) | \
  (0x1 << BIT31_OFFSET_AAAABDY)) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx10131008, \
  BIT2_MASK_AAAABEX | \
  BIT7_MASK_AAAABEZ, \
  ((0x1 << BIT2_OFFSET_AAAABEY) | \
  (0x1 << BIT7_OFFSET_AAAABFA)) \
  ),

// --------------------------------------------------
//Endpoint function interrupt enable
// --------------------------------------------------

  #define NBIO_NBIF_DEVICE_INTERRUPT_TBL \
          SMN_ENTRY_WR(NBIFMMIx1013a008, \
  ((0xFF << BIT0_OFFSET_AAAABCP) | \
  (0x3 << BIT8_OFFSET_AAAABCQ) | \
  (0x79 << BIT16_OFFSET_AAAABCR)) \
  ),

// --------------------------------------------------
// DMA Arbitration
// --------------------------------------------------
  #define NBIO_NBIF_DMA_ARBITRATION_TBL \
          SMN_ENTRY_RMW(NBIFMMx1013a124, \
  BITS_0_TO_7_MASK_AAAABBV | \
  BITS_8_TO_15_MASK_AAAABBX | \
  BITS_16_TO_23_MASK_AAAABBZ | \
  BITS_24_TO_31_MASK_AAAABCB, \
  (0x4 << BIT0_OFFSET_AAAABBW) | \
  (0x4 << BIT8_OFFSET_AAAABBY) | \
  (0x4 << BIT16_OFFSET_AAAABCA) | \
  (0x4 << BIT24_OFFSET_AAAABCC) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013a128, \
  BITS_0_TO_7_MASK_AAAABCD | \
  BITS_8_TO_15_MASK_AAAABCF | \
  BITS_16_TO_23_MASK_AAAABCH | \
  BITS_24_TO_31_MASK_AAAABCJ, \
  (0x4 << BIT0_OFFSET_AAAABCE) | \
  (0x4 << BIT8_OFFSET_AAAABCG) | \
  (0x4 << BIT16_OFFSET_AAAABCI) | \
  (0x4 << BIT24_OFFSET_AAAABCK) \
  ),

// --------------------------------------------------
//Atomic
// --------------------------------------------------
  #define NBIO_NBIF_ATOMIC_TBL \
          SMN_ENTRY_RMW(NBIFMMx10131014, \
  BIT18_MASK_AAAABFP, \
  (0x1 << BIT18_OFFSET_AAAABFQ) \
  ), \
          SMN_ENTRY_RMW(NBIFEPFCFGx1014008C, \
  BIT6_MASK_AAAAAYT | \
  BIT7_MASK_AAAAAYR, \
  (0x1 << BIT6_OFFSET_AAAAAYU) | \
  (0x0 << BIT7_OFFSET_AAAAAYS) \
  ), \
          SMN_ENTRY_RMW(NBIFEPFCFGx1014108C, \
  BIT6_MASK_AAAAAYT | \
  BIT7_MASK_AAAAAYR, \
  (0x1 << BIT6_OFFSET_AAAAAYU) | \
  (0x0 << BIT7_OFFSET_AAAAAYS) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx10131014, \
  BIT18_MASK_AAAABFP, \
  (0x1 << BIT18_OFFSET_AAAABFQ) \
  ), \
          SMN_ENTRY_RMW(NBIFMMSx10134010, \
  BIT20_MASK_AAAABED | \
  BIT21_MASK_AAAABEF, \
  (0x1 << BIT20_OFFSET_AAAABEE) | \
  (0x1 << BIT21_OFFSET_AAAABEG) \
  ),

// --------------------------------------------------
// PerfReg Final Setting
// --------------------------------------------------
  #define NBIO_NBIF_PERF_REG_TBL \
          SMN_ENTRY_RMW(SYSHUBMMx140f954, \
  BITS_0_TO_7_MASK_AAAAAMJ | \
  BITS_8_TO_15_MASK_AAAAAMH | \
  BITS_16_TO_23_MASK_AAAAAML, \
  (0x4 << BIT0_OFFSET_AAAAAMK) | \
  (0x4 << BIT8_OFFSET_AAAAAMI) | \
  (0x4 << BIT16_OFFSET_AAAAAMM) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx140f958, \
  BITS_0_TO_7_MASK_AAAAAMN | \
  BITS_16_TO_23_MASK_AAAAAMR | \
  BITS_24_TO_31_MASK_AAAAAMP, \
  (0x4 << BIT0_OFFSET_AAAAAMO) | \
  (0x0 << BIT16_OFFSET_AAAAAMS) | \
  (0x0 << BIT24_OFFSET_AAAAAMQ) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx140f95c, \
  BITS_0_TO_7_MASK_AAAAAMV | \
  BITS_8_TO_15_MASK_AAAAAMT | \
  BITS_16_TO_23_MASK_AAAAAMZ | \
  BITS_24_TO_31_MASK_AAAAAMX, \
  (0x48 << BIT0_OFFSET_AAAAAMW) | \
  (0x48 << BIT8_OFFSET_AAAAAMU) | \
  (0x0 << BIT16_OFFSET_AAAAANA) | \
  (0x0 << BIT24_OFFSET_AAAAAMY) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx140f960, \
  BITS_0_TO_7_MASK_AAAAANH | \
  BITS_8_TO_15_MASK_AAAAANF | \
  BITS_16_TO_23_MASK_AAAAAND | \
  BITS_24_TO_31_MASK_AAAAANB, \
  (0x0 << BIT0_OFFSET_AAAAANI) | \
  (0x0 << BIT8_OFFSET_AAAAANG) | \
  (0x0 << BIT16_OFFSET_AAAAANE) | \
  (0x0 << BIT24_OFFSET_AAAAANC) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1411990, \
  BITS_16_TO_23_MASK_AAAAANJ | \
  BITS_24_TO_31_MASK_AAAAANL, \
  (0x20 << BIT16_OFFSET_AAAAANK) | \
  (0x20 << BIT24_OFFSET_AAAAANM) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx140f90c, \
  BITS_24_TO_31_MASK_AAAAAMF | \
  BITS_16_TO_23_MASK_AAAAAMD, \
  (0x4 << BIT24_OFFSET_AAAAAMG) | \
  (0x4 << BIT16_OFFSET_AAAAAME) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1013a4cc, \
  BITS_0_TO_31_MASK_AAAAANV, \
  (0x00060006 << BIT0_OFFSET_AAAAANW) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1013a4c8, \
  BITS_0_TO_31_MASK_AAAAANT, \
  (0x00060006 << BIT0_OFFSET_AAAAANU) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1013a42c, \
  BITS_0_TO_31_MASK_AAAAANR, \
  (0x23030303 << BIT0_OFFSET_AAAAANS) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1013a428, \
  BITS_0_TO_31_MASK_AAAAANP, \
  (0x33030303 << BIT0_OFFSET_AAAAANQ) \
  ), \
          SMN_ENTRY_RMW(SYSHUBMMx1013a4f4, \
  BITS_0_TO_7_MASK_AAAAANX, \
  (0xF0 << BIT0_OFFSET_AAAAANY) \
  ),

// --------------------------------------------------
// First VF offset
// --------------------------------------------------
  #define NBIO_NBIF_FIRST_VF_OFFSET_TBL \
          SMN_ENTRY_RMW(NBIFMMx101236bc, \
  BITS_0_TO_15_MASK_AAAABCW, \
  (8 << BIT0_OFFSET_AAAABCX) \
  ),

  #define NBIO_ENABLE_FLR_TBL \
          SMN_ENTRY_RMW(NBIFMMx10135210, \
  BIT22_MASK_AAAABFX, \
  (1 << BIT22_OFFSET_AAAABFY) \
  ), \

  #define NBIO_NBIF_DS_CTRL_LCLK_TBL \
          SMN_ENTRY_RMW(NBIFMMx1013a220, \
  BIT0_MASK_AAAABCS, \
  (1 << BIT0_OFFSET_AAAABCT) \
  ),

  #define NBIO_SYSHUB_DS_CTRL_SOCCLK_TBL \
          SMN_ENTRY_RMW(SYSHUBMMx1410000, \
  BIT31_MASK_AAAAAOT, \
  (1 << BIT31_OFFSET_AAAAAOU) \
  ),

  #define NBIO_SYSHUB_DS_CTRL_SHUBCLK_TBL \
          SMN_ENTRY_RMW(SYSHUBMMx1411000, \
  BIT31_MASK_AAAAAOR, \
  (1 << BIT31_OFFSET_AAAAAOS) \
  ),

  #define NBIO_SYSHUB_DS_CTRL_HSPCLK_TBL \
          SMN_ENTRY_RMW(SYSHUBMMx1412000, \
  BIT31_MASK_AAAAAOP, \
  (1 << SYSHUB_DS_CTRL_HSPCLK_SYSHUB_HSPCLK_DS_EN_OFFSET) \
  ),

  #define NBIO_SYSHUB_NGDC_PG_MISC_CTRL_TBL \
          SMN_ENTRY_RMW(SYSHUBMMx1403c60, \
  BITS_24_TO_29_MASK_AAAAANN, \
  (0x5 << BIT24_OFFSET_AAAAANO) \
  ),

  #define NBIO_NBIF_DISABLE_IGPU_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx10131000, \
      (0x1 << 31), \
      (0x1 << 31) \
      ), \
    SMN_ENTRY_RMW ( \
        NBIFMMx10134200, \
        BIT28_MASK_AAAABEL, \
        (0 << BIT28_OFFSET_AAAABEM) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134038, \
      0xffff, \
      (0x1022 << 0) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134034, \
      (BITS_0_TO_7_MASK_AAAABDD | \
      BITS_8_TO_15_MASK_AAAABDF | \
      BITS_16_TO_23_MASK_AAAABDB), \
      (0x0 << BIT0_OFFSET_AAAABDE) | \
      (0x0 << BIT8_OFFSET_AAAABDG) | \
      (0x13 << BIT16_OFFSET_AAAABDC) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134010, \
      BITS_28_TO_30_MASK_AAAABEH, \
      (0x0 << BIT28_OFFSET_AAAABEI) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx1013400c, \
      BIT18_MASK_AAAABEB | \
      BIT20_MASK_AAAABDZ, \
      (0x0 << BIT18_OFFSET_AAAABEC) | \
      (0x0 << BIT20_OFFSET_AAAABEA) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134010, \
      BITS_23_TO_27_MASK_AAAABEJ, \
      (0x0 << BIT23_OFFSET_AAAABEK) \
      ), \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134008, \
      (0x1 << 8), \
      (0x0 << 8) \
      ),

  #define NBIO_NBIF_DISABLE_LTR_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx1013100c, \
      0x2 | \
      0x4, \
      (0x0 << 1) | \
      (0x0 << 2) \
      ),

  #define NBIO_NBIF_DISABLE_AER_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134008, \
      (0x1 << 16) | \
      (0x1 << 17) | \
      (0x1 << 18) | \
      (0x1 << 28) | \
      (0x1 << 27) | \
      (0x1 << 8), \
      (0x0 << 16) | \
      (0x0 << 17) | \
      (0x0 << 18) | \
      (0x0 << 28) | \
      (0x0 << 27) | \
      (0x0 << 8) \
      ),

  #define NBIO_NBIF_DISABLE_FLR_ATOMIC_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx10134008, \
      (0x1 << 22) | \
      (0x1 << 21) | \
      (0x1 << 20), \
      (0x0 << 22) | \
      (0x0 << 21) | \
      (0x0 << 20) \
      ),

  #define NBIO_NBIF_CLOCK_GATING_TBL \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_NBIF_MGCG_CLKGATING_ENABLED, \
      NBIFMMx1013a21c, \
      (0x1 << 0), \
      (0x1 << 0) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED, \
      SYSHUBMMx1411020, \
      (0x1 << 0), \
      (0x1 << 0) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED, \
      SYSHUBMMx1410020, \
      (0x1 << 0), \
      (0x1 << 0) \
      ),

  #define NBIO_NBIF_POWER_GATING_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx1013a0e0, \
      (0xff << 0) | \
      (0x1 << 8), \
      (0x10 << 0) | \
      (0x1 << 8) \
      ),

  #define NBIO_NBIF_SHUB_POWER_GATING_TBL \
    SMN_ENTRY_RMW ( \
      SYSHUBMMx1403c64, \
      (0xff << 0) | \
      (0x1 << 8), \
      (0x10 << 0) | \
      (0x1 << 8) \
      ),

  #define NBIO_NBIF_OBFF_CTRL_TBL \
    SMN_ENTRY_RMW ( \
      NBIFMMx1013a1d0, \
      (0x1 << 0), \
      (0x1 << 0) \
      ),

  #define NBIO_NBIF_OBFF_DMA_TBL \
    SMN_ENTRY_RMW ( \
      SYSHUBMMx1410040, \
      (0x1 << 0), \
      (0x1 << 0) \
      ),
