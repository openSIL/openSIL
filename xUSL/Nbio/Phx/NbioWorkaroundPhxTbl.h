/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioWorkaroundPhxTbl.h
 * @brief This file contains the Nbio workaround configurations
 *
 */

#pragma once
#include <includePHX/PHX_NBIFMM.h>
#include <includePHX/PHX_GnbRegistersPhx.h>
// --------------------------------------------------
// ACP Sub Class
// --------------------------------------------------
  #define NBIO_WA_ACP_SUB_CLASS_TBL \
          SMN_ENTRY_RMW(NBIFMMx10134a34, \
  BITS_8_TO_15_MASK_AAAABER, \
  (0x80 << BIT8_OFFSET_AAAABES) \
  ),
// --------------------------------------------------
// PCIE CV test
// --------------------------------------------------
  #define NBIO_WA_PCI_CV_TEST_TBL \
          SMN_ENTRY_RMW(NBIFMMx101311ac, \
  BIT0_MASK_AAAABCL, \
  (0x1 << BIT0_OFFSET_AAAABCN) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx101313ac, \
  BIT0_MASK_AAAABCL, \
  (0x1 << BIT0_OFFSET_AAAABCN) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx101315ac, \
  BIT0_MASK_AAAABCL, \
  (0x1 << BIT0_OFFSET_AAAABCN) \
  ),
// --------------------------------------------------
// Completion Timeout feature
// --------------------------------------------------
  #define NBIO_WA_CPL_TIMEOUT_TBL \
          SMN_ENTRY_RMW(NBIFMMx1013100c, \
  BIT7_MASK_AAAABFB, \
  (0x1 << BIT7_OFFSET_AAAABFC) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013120c, \
  BIT7_MASK_AAAABFZ, \
  (0x1 << BIT7_OFFSET_AAAABGA) \
  ), \
          SMN_ENTRY_RMW(NBIFMMx1013140c, \
  BIT7_MASK_AAAABGR, \
  (0x1 << BIT7_OFFSET_AAAABGS) \
  ),
// --------------------------------------------------
// RIOMMU disable timeout
// --------------------------------------------------
  #define NBIO_WA_RIOMMU_DIS_TIMEOUT_TBL \
          SMN_ENTRY_RMW(RIOMMUx16b0a0f0, \
  BITS_3_TO_5_MASK_AAAABAW, \
  (0x0 << BIT3_OFFSET_AAAABAX) \
  ), \
          SMN_ENTRY_RMW(RIOMMUx16b0e200, \
  BITS_1_TO_5_MASK_AAAABBI | \
  BITS_6_TO_10_MASK_AAAABBG, \
  (0x1F << BIT1_OFFSET_AAAABBJ) | \
  (0x1F << BIT6_OFFSET_AAAABBH) \
  ), \
          SMN_ENTRY_RMW(RIOMMUx16b0a120, \
  BIT0_MASK_AAAABAY | \
  BIT1_MASK_AAAABBA, \
  (0x0 << BIT0_OFFSET_AAAABAZ) | \
  (0x1 << BIT1_OFFSET_AAAABBB) \
  ),

  #define NBIO_WA_RIOMMU_TW_COHERENT_TBL \
          SMN_ENTRY_RMW(RIOMMUx16b0a0ec, \
  BIT6_MASK_AAAABBC, \
  (1 << BIT6_OFFSET_AAAABBD) \
  ),
// PME_TURNOFF_MODE
  #define NBIO_WA_PME_TURNOFF_MODE_TBL \
          SMN_ENTRY_RMW(NBIFMMx1013a010, \
  BIT28_MASK_AAAABBT, \
  (1 << BIT28_OFFSET_AAAABBU) \
  ),
// RSMU_MASTER_MESSAGE_SEND_ENABLE
  #define NBIO_WA_RSMU_MASTER_MESSAGE_SEND_ENABLE_TBL \
          SMN_ENTRY_RMW(PCIECOREx1118047c, \
  BIT0_MASK_AAAABBE, \
  (1 << BIT0_OFFSET_AAAABBF) \
  ), \
          SMN_ENTRY_RMW(PCIECOREx1128047c, \
  BIT0_MASK_AAAABBE, \
  (1 << BIT0_OFFSET_AAAABBF) \
  ), \

