/*****************************************************************************
 *
 * Copyright (C) 2008 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIO_USB_OVER_PCIE_PHX_TABLE_
#define _NBIO_USB_OVER_PCIE_PHX_TABLE_

#include <includePHX/PHX_NBIOBASEREG.h>
#include <includePHX/PHX_PCIEPORT.h>

// --------------------------------------------------
//  PCIe hot plug MSI interrupt (Clean up the pending packets before any transaction send to PCIe.)
// --------------------------------------------------

  #define PCIE23_TX_PORT_CTRL \
          SMN_ENTRY_RMW(PCIEPORTx11340600, \
  BIT15_MASK_AAAAAGQ, \
  (0 << BIT15_OFFSET_AAAAAGR) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11440600, \
  BIT15_MASK_AAAAAGQ, \
  (0 << BIT15_OFFSET_AAAAAGR) \
  ), \

  #define PCIE23_TX_REQUESTER_ID_DEVICE \
          SMN_ENTRY_RMW(PCIEPORTx11340084, \
  (BITS_3_TO_7_MASK_AAAAABH | \
  BITS_0_TO_2_MASK_AAAAABJ), \
  (3 << BIT3_OFFSET_AAAAABI) | \
  (1 << BIT0_OFFSET_AAAAABK) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11440084, \
  (BITS_3_TO_7_MASK_AAAAABH | \
  BITS_0_TO_2_MASK_AAAAABJ), \
  (4 << BIT3_OFFSET_AAAAABI) | \
  (1 << BIT0_OFFSET_AAAAABK) \
  ), \
// --------------------------------------------------
// enable L1
// --------------------------------------------------
  #define PCIE23_Enable_L1 \
          SMN_ENTRY_RMW(PCIEPORTx11340318, \
  (BIT23_MASK_AAAAAFC | BIT16_MASK_AAAAAFE), \
  (1 << BIT23_OFFSET_AAAAAFD) | \
  (0 << BIT16_OFFSET_AAAAAFF) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11440318, \
  (BIT23_MASK_AAAAAFC | BIT16_MASK_AAAAAFE), \
  (1 << BIT23_OFFSET_AAAAAFD) | \
  (0 << BIT16_OFFSET_AAAAAFF) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx113402d8, \
  BIT4_MASK_AAAAAEY, \
  (1 << BIT4_OFFSET_AAAAAEZ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx114402d8, \
  BIT4_MASK_AAAAAEY, \
  (1 << BIT4_OFFSET_AAAAAEZ) \
  ), \


// --------------------------------------------------
// PCIE SDP disconnect control.
// --------------------------------------------------
  #define PCIE23_SDP_DISCONNECT_CNTL \
          SMN_ENTRY_RMW(PCIEPORTx11380714, \
  (BIT18_MASK_AAAAAAP | \
  BIT17_MASK_AAAAAAR), \
  (1 << BIT18_OFFSET_AAAAAAQ) | \
  (1 << BIT17_OFFSET_AAAAAAS) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480714, \
  (BIT18_MASK_AAAAAAP | \
  BIT17_MASK_AAAAAAR), \
  (1 << BIT18_OFFSET_AAAAAAQ) | \
  (1 << BIT17_OFFSET_AAAAAAS) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx1138018c, \
  BIT5_MASK_AAAAAAX, \
  (1 << BIT5_OFFSET_AAAAAAY) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx1148018c, \
  BIT5_MASK_AAAAAAX, \
  (1 << BIT5_OFFSET_AAAAAAY) \
  ), \
// --------------------------------------------------
// LCLK DS and LCLK Gating.
// --------------------------------------------------
  #define PCIE23_LCLK_DS_AND_LCLK_GATING \
          SMN_ENTRY_RMW(PCIEPORTx11380468, \
  (BIT5_MASK_AAAAAAB | \
  BIT4_MASK_AAAAAAD \
  | BITS_2_TO_3_MASK_AAAAAAF | \
  BIT6_MASK_AAAAAAH), \
  (1 << BIT5_OFFSET_AAAAAAC) | \
  (1 << BIT4_OFFSET_AAAAAAE) | \
  (0 << BIT2_OFFSET_AAAAAAG) | \
  (1 << BIT6_OFFSET_AAAAAAI) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480468, \
  (BIT5_MASK_AAAAAAB | \
  BIT4_MASK_AAAAAAD \
  | BITS_2_TO_3_MASK_AAAAAAF | \
  BIT6_MASK_AAAAAAH), \
  (1 << BIT5_OFFSET_AAAAAAC) | \
  (1 << BIT4_OFFSET_AAAAAAE) | \
  (0 << BIT2_OFFSET_AAAAAAG) | \
  (1 << BIT6_OFFSET_AAAAAAI) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx113805c4, \
  BITS_0_TO_7_MASK_AAAAABL, \
  (0xFE << BIT0_OFFSET_AAAAABM) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx114805c4, \
  BITS_0_TO_7_MASK_AAAAABL, \
  (0xFE << BIT0_OFFSET_AAAAABM) \
  ), \
// --------------------------------------------------
// RSMU Enable Messages
// --------------------------------------------------
  #define PCIE23_RSMU_MASTER_MESSAGE_SEND_ENABLE \
          SMN_ENTRY_RMW(PCIEPORTx1138047c, \
  BIT0_MASK_AAAABBE, \
  (1 << BIT0_OFFSET_AAAABBF) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx1148047c, \
  BIT0_MASK_AAAABBE, \
  (1 << BIT0_OFFSET_AAAABBF) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx113801b0, \
  BITS_0_TO_31_MASK_AAAAADP, \
  (0x197F2080 << BIT0_OFFSET_AAAAADQ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx114801b0, \
  BITS_0_TO_31_MASK_AAAAADP, \
  (0x198F2080 << BIT0_OFFSET_AAAAADQ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx113801b4, \
  BITS_0_TO_31_MASK_AAAAADR, \
  (0x197D5D00 << BIT0_OFFSET_AAAAADS) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx114801b4, \
  BITS_0_TO_31_MASK_AAAAADR, \
  (0x198D5D00 << BIT0_OFFSET_AAAAADS) \
  ), \
// --------------------------------------------------
// PerfReg Final Setting
// --------------------------------------------------
  #define PCIE23_PERFREG_FINAL_SETTING \
          SMN_ENTRY_RMW(PCIEPORTx140f954, \
  BITS_0_TO_7_MASK_AAAAAMJ | \
  BITS_8_TO_15_MASK_AAAAAMH | \
  BITS_16_TO_23_MASK_AAAAAML, \
  (0x4 << BIT0_OFFSET_AAAAAMK) | \
  (0x4 << BIT8_OFFSET_AAAAAMI) | \
  (0xB << BIT16_OFFSET_AAAAAMM) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx140f958, \
  BITS_0_TO_7_MASK_AAAAAMN | \
  BITS_16_TO_23_MASK_AAAAAMR | \
  BITS_24_TO_31_MASK_AAAAAMP, \
  (0x4 << BIT0_OFFSET_AAAAAMO) | \
  (0x0 << BIT16_OFFSET_AAAAAMS) | \
  (0x0 << BIT24_OFFSET_AAAAAMQ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx140f95c, \
  BITS_0_TO_7_MASK_AAAAAMV | \
  BITS_8_TO_15_MASK_AAAAAMT | \
  BITS_16_TO_23_MASK_AAAAAMZ | \
  BITS_24_TO_31_MASK_AAAAAMX, \
  (0x4A << BIT0_OFFSET_AAAAAMW) | \
  (0x4A << BIT8_OFFSET_AAAAAMU) | \
  (0x0 << BIT16_OFFSET_AAAAANA) | \
  (0x0 << BIT24_OFFSET_AAAAAMY) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx140f960, \
  BITS_0_TO_7_MASK_AAAAANH | \
  BITS_8_TO_15_MASK_AAAAANF | \
  BITS_16_TO_23_MASK_AAAAAND | \
  BITS_24_TO_31_MASK_AAAAANB, \
  (0x0 << BIT0_OFFSET_AAAAANI) | \
  (0x0 << BIT8_OFFSET_AAAAANG) | \
  (0x0 << BIT16_OFFSET_AAAAANE) | \
  (0x0 << BIT24_OFFSET_AAAAANC) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx140f96c, \
  BITS_8_TO_15_MASK_AAAAABN | \
  BITS_16_TO_23_MASK_AAAAABP, \
  (0x3 << BIT8_OFFSET_AAAAABO) | \
  (0x0 << BIT16_OFFSET_AAAAABQ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx140f970, \
  BITS_0_TO_7_MASK_AAAAABR | \
  BITS_8_TO_15_MASK_AAAAABT | \
  BITS_16_TO_23_MASK_AAAAABV | \
  BITS_24_TO_31_MASK_AAAAABX, \
  (0x0 << BIT0_OFFSET_AAAAABS) | \
  (0x47 << BIT8_OFFSET_AAAAABU) | \
  (0x0 << BIT16_OFFSET_AAAAABW) | \
  (0x0 << BIT24_OFFSET_AAAAABY) \
  ), \

// --------------------------------------------------
// PCIE MISC Setting
// --------------------------------------------------
  #define PCIE23_MISC_SETTING \
          SMN_ENTRY_RMW(PCIEPORTx11380070, \
  BIT20_MASK_AAAAAAL | \
  BIT29_MASK_AAAAAAJ, \
  (0x0 << BIT20_OFFSET_AAAAAAM) | \
  (0x0 << BIT29_OFFSET_AAAAAAK) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480070, \
  BIT20_MASK_AAAAAAL | \
  BIT29_MASK_AAAAAAJ, \
  (0x0 << BIT20_OFFSET_AAAAAAM) | \
  (0x0 << BIT29_OFFSET_AAAAAAK) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11380074, \
  BIT13_MASK_AAAAAAV | \
  BIT14_MASK_AAAAAAT, \
  (0x0 << BIT13_OFFSET_AAAAAAW) | \
  (0x0 << BIT14_OFFSET_AAAAAAU) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480074, \
  BIT13_MASK_AAAAAAV | \
  BIT14_MASK_AAAAAAT, \
  (0x0 << BIT13_OFFSET_AAAAAAW) | \
  (0x0 << BIT14_OFFSET_AAAAAAU) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11380080, \
  BIT31_MASK_AAAAAEG, \
  (0x1 << BIT31_OFFSET_AAAAAEH) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480080, \
  BIT31_MASK_AAAAAEG, \
  (0x1 << BIT31_OFFSET_AAAAAEH) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11380100, \
  BIT6_MASK_AAAAAGI, \
  (0x1 << BIT6_OFFSET_AAAAAGJ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11480100, \
  BIT6_MASK_AAAAAGI, \
  (0x1 << BIT6_OFFSET_AAAAAGJ) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx1138061c, \
  BIT1_MASK_AAAAAAZ | \
  BIT2_MASK_AAAAABB | \
  BIT4_MASK_AAAAABD | \
  BIT5_MASK_AAAAABF, \
  (0x0 << BIT1_OFFSET_AAAAABA) | \
  (0x0 << BIT2_OFFSET_AAAAABC) | \
  (0x0 << BIT4_OFFSET_AAAAABE) | \
  (0x0 << BIT5_OFFSET_AAAAABG) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx1148061c, \
  BIT1_MASK_AAAAAAZ | \
  BIT2_MASK_AAAAABB | \
  BIT4_MASK_AAAAABD | \
  BIT5_MASK_AAAAABF, \
  (0x0 << BIT1_OFFSET_AAAAABA) | \
  (0x0 << BIT2_OFFSET_AAAAABC) | \
  (0x0 << BIT4_OFFSET_AAAAABE) | \
  (0x0 << BIT5_OFFSET_AAAAABG) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11340280, \
  BIT23_MASK_AAAAAFC, \
  (0x1 << BIT23_OFFSET_AAAAAFD) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11440280, \
  BIT23_MASK_AAAAAFC, \
  (0x1 << BIT23_OFFSET_AAAAAFD) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx113402f0, \
  BIT12_MASK_AAAAAAN, \
  (0x1 << BIT12_OFFSET_AAAAAAO) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx114402f0, \
  BIT12_MASK_AAAAAAN, \
  (0x1 << BIT12_OFFSET_AAAAAAO) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11340600, \
  BIT20_MASK_AAAAAGO, \
  (0x1 << BIT20_OFFSET_AAAAAGP) \
  ), \
          SMN_ENTRY_RMW(PCIEPORTx11440600, \
  BIT20_MASK_AAAAAGO, \
  (0x1 << BIT20_OFFSET_AAAAAGP) \
  ), \

#endif /* _NBIO_USB_OVER_PCIE_PHX_TABLE_ */
