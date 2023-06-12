/**
 * @file  NbioIohcTbl.h
 * @brief This file contains the Nbio Iohc configurations
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

// --------------------------------------------------
// 15.1.4.3 Enabling Peer-to-Peer
// --------------------------------------------------


//To enable peer-to-peer operation, set IOHC::IOHC_FEATURE_CNTL[P2P_mode] = 1 and
//IOHC::PCIE_VDM_CNTL2[VdmP2pMode] = 1.

  // IOHC::IOHC_FEATURE_CNTL[P2P_mode] = 1
  // IOHC::PCIE_VDM_CNTL2[VdmP2pMode] = 1
  #define NBIO_IOHC_P2P_TBL \
    SMN_ENTRY_RMW ( \
      SMN_IOHUB0NBIO0_IOHC_FEATURE_CNTL_ADDRESS, \
      IOHC_FEATURE_CNTL_P2P_mode_OFFSET, \
      (0x1 << IOHC_FEATURE_CNTL_P2P_mode_OFFSET) \
      ), \
    SMN_ENTRY_RMW ( \
      SMN_IOHUB0NBIO0_PCIE_VDM_CNTL2_ADDRESS, \
      PCIE_VDM_CNTL2_VdmP2pMode_MASK, \
      (0x1 << PCIE_VDM_CNTL2_VdmP2pMode_OFFSET) \
      ),
  // End of NBIO_IOHC_P2P_TBL

// --------------------------------------------------
// 15.1.4.5 PCI Express Configuration Cycle Hardware Retry on CRS
// --------------------------------------------------

  //IOHC::IOHC_PCIE_CRS_Count[CrsDelayCount] = 0x6
  //For Hot-plug slots only: IOHC::IOHC_PCIE_CRS_Count[CrsLimitCount] = 0x262 (Configured later)
  //For non-Hot-plug slots: IOHC::IOHC_PCIE_CRS_Count[CrsLimitCount] = 0x6
  //For each bridge, IOHC::IOHC_Bridge_CNTL[CrsEnable] must be set. (a part of PcieControlPorts function)
  #define NBIO_IOHC_CRS_TBL \
    SMN_ENTRY_RMW ( \
      SMN_IOHUB0NBIO0_IOHC_PCIE_CRS_Count_ADDRESS, \
      IOHC_PCIE_CRS_Count_CrsDelayCount_MASK | \
      IOHC_PCIE_CRS_Count_CrsLimitCount_MASK, \
      (0x6 << IOHC_PCIE_CRS_Count_CrsDelayCount_OFFSET) | \
      (0x262 << IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET) \
      ), \
  // End of NBIO_IOHC_P2P_TBL

  #define NBIO_IOHC_CLOCK_GATING_TBL \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED, \
      SIL_RESERVED_732, \
      SIL_RESERVED_154 | \
      SIL_RESERVED_152 | \
      IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_149 | \
      SIL_RESERVED_147 | \
      SIL_RESERVED_145 | \
      SIL_RESERVED_143 | \
      SIL_RESERVED_141 | \
      SIL_RESERVED_139 | \
      SIL_RESERVED_137, \
      (0x0 << SIL_RESERVED_155) | \
      (0x0 << SIL_RESERVED_153) | \
      (0x0 << SIL_RESERVED_151) | \
      (0x0 << SIL_RESERVED_150) | \
      (0x0 << SIL_RESERVED_148) | \
      (0x0 << SIL_RESERVED_146) | \
      (0x0 << SIL_RESERVED_144) | \
      (0x0 << SIL_RESERVED_142) | \
      (0x0 << SIL_RESERVED_140) | \
      (0x0 << SIL_RESERVED_138) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_DISABLED, \
      SIL_RESERVED_732, \
      SIL_RESERVED_154 | \
      SIL_RESERVED_152 | \
      IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_149 | \
      SIL_RESERVED_147 | \
      SIL_RESERVED_145 | \
      SIL_RESERVED_143 | \
      SIL_RESERVED_141 | \
      SIL_RESERVED_139 | \
      SIL_RESERVED_137, \
      (0x1 << SIL_RESERVED_155) | \
      (0x1 << SIL_RESERVED_153) | \
      (0x1 << SIL_RESERVED_151) | \
      (0x1 << SIL_RESERVED_150) | \
      (0x1 << SIL_RESERVED_148) | \
      (0x1 << SIL_RESERVED_146) | \
      (0x1 << SIL_RESERVED_144) | \
      (0x1 << SIL_RESERVED_142) | \
      (0x1 << SIL_RESERVED_140) | \
      (0x1 << SIL_RESERVED_138) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED, \
      SIL_RESERVED_733, \
      SIL_RESERVED_172 | \
      SIL_RESERVED_170 | \
      SIL_RESERVED_168 | \
      SIL_RESERVED_166 | \
      SIL_RESERVED_164 | \
      SIL_RESERVED_162 | \
      SIL_RESERVED_160 | \
      SIL_RESERVED_158 | \
      SIL_RESERVED_157 | \
      IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK, \
      (0x0 << IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET) | \
      (0x0 << SIL_RESERVED_171) | \
      (0x0 << SIL_RESERVED_169) | \
      (0x0 << SIL_RESERVED_167) | \
      (0x0 << SIL_RESERVED_165) | \
      (0x0 << SIL_RESERVED_163) | \
      (0x0 << SIL_RESERVED_161) | \
      (0x0 << SIL_RESERVED_159) | \
      (0x0 << SIL_RESERVED_177) | \
      (0x0 << SIL_RESERVED_156) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_DISABLED, \
      SIL_RESERVED_733, \
      SIL_RESERVED_172 | \
      SIL_RESERVED_170 | \
      SIL_RESERVED_168 | \
      SIL_RESERVED_166 | \
      SIL_RESERVED_164 | \
      SIL_RESERVED_162 | \
      SIL_RESERVED_160 | \
      SIL_RESERVED_158 | \
      SIL_RESERVED_157 | \
      IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK, \
      (0x1 << IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET) | \
      (0x1 << SIL_RESERVED_171) | \
      (0x1 << SIL_RESERVED_169) | \
      (0x1 << SIL_RESERVED_167) | \
      (0x1 << SIL_RESERVED_165) | \
      (0x1 << SIL_RESERVED_163) | \
      (0x1 << SIL_RESERVED_161) | \
      (0x1 << SIL_RESERVED_159) | \
      (0x1 << SIL_RESERVED_177) | \
      (0x1 << SIL_RESERVED_156) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED, \
      SIL_RESERVED_734, \
      SIL_RESERVED_189 | \
      SIL_RESERVED_187 | \
      IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_185 | \
      SIL_RESERVED_183 | \
      SIL_RESERVED_181 | \
      SIL_RESERVED_179 | \
      IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK2_MASK | \
      SIL_RESERVED_175 | \
      SIL_RESERVED_173, \
      (0x0 << SIL_RESERVED_190) | \
      (0x0 << SIL_RESERVED_188) | \
      (0x0 << SIL_RESERVED_186) | \
      (0x0 << IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK6_OFFSET) | \
      (0x0 << SIL_RESERVED_184) | \
      (0x0 << SIL_RESERVED_182) | \
      (0x0 << SIL_RESERVED_180) | \
      (0x0 << SIL_RESERVED_178) | \
      (0x0 << SIL_RESERVED_176) | \
      (0x0 << SIL_RESERVED_174) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_DISABLED, \
      SIL_RESERVED_734, \
      SIL_RESERVED_189 | \
      SIL_RESERVED_187 | \
      IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_185 | \
      SIL_RESERVED_183 | \
      SIL_RESERVED_181 | \
      SIL_RESERVED_179 | \
      IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK2_MASK | \
      SIL_RESERVED_175 | \
      SIL_RESERVED_173, \
      (0x1 << SIL_RESERVED_190) | \
      (0x1 << SIL_RESERVED_188) | \
      (0x1 << SIL_RESERVED_186) | \
      (0x1 << IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK6_OFFSET) | \
      (0x1 << SIL_RESERVED_184) | \
      (0x1 << SIL_RESERVED_182) | \
      (0x1 << SIL_RESERVED_180) | \
      (0x1 << SIL_RESERVED_178) | \
      (0x1 << SIL_RESERVED_176) | \
      (0x1 << SIL_RESERVED_174) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED, \
      SIL_RESERVED_728, \
      SIL_RESERVED_77 | \
      SIL_RESERVED_76 | \
      SIL_RESERVED_74 | \
      SIL_RESERVED_72 | \
      SIL_RESERVED_70 | \
      SIL_RESERVED_68 | \
      SIL_RESERVED_66 | \
      IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK | \
      IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK | \
      SIL_RESERVED_62, \
      (0x0 << SIL_RESERVED_78) | \
      (0x0 << IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET) | \
      (0x0 << SIL_RESERVED_75) | \
      (0x0 << SIL_RESERVED_73) | \
      (0x0 << SIL_RESERVED_71) | \
      (0x0 << SIL_RESERVED_69) | \
      (0x0 << SIL_RESERVED_67) | \
      (0x0 << SIL_RESERVED_65) | \
      (0x0 << SIL_RESERVED_64) | \
      (0x0 << SIL_RESERVED_63) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_DISABLED, \
      SIL_RESERVED_728, \
      SIL_RESERVED_77 | \
      SIL_RESERVED_76 | \
      SIL_RESERVED_74 | \
      SIL_RESERVED_72 | \
      SIL_RESERVED_70 | \
      SIL_RESERVED_68 | \
      SIL_RESERVED_66 | \
      IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK | \
      IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK1_MASK | \
      SIL_RESERVED_62, \
      (0x1 << SIL_RESERVED_78) | \
      (0x1 << IOAGR_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK8_OFFSET) | \
      (0x1 << SIL_RESERVED_75) | \
      (0x1 << SIL_RESERVED_73) | \
      (0x1 << SIL_RESERVED_71) | \
      (0x1 << SIL_RESERVED_69) | \
      (0x1 << SIL_RESERVED_67) | \
      (0x1 << SIL_RESERVED_65) | \
      (0x1 << SIL_RESERVED_64) | \
      (0x1 << SIL_RESERVED_63) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED, \
      SIL_RESERVED_729, \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_MASK | \
      SIL_RESERVED_91 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_88 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_MASK | \
      SIL_RESERVED_85 | \
      SIL_RESERVED_83 | \
      SIL_RESERVED_81 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK, \
      (0x0 << SIL_RESERVED_93) | \
      (0x0 << SIL_RESERVED_92) | \
      (0x0 << SIL_RESERVED_90) | \
      (0x0 << SIL_RESERVED_89) | \
      (0x0 << SIL_RESERVED_87) | \
      (0x0 << SIL_RESERVED_86) | \
      (0x0 << SIL_RESERVED_84) | \
      (0x0 << SIL_RESERVED_82) | \
      (0x0 << SIL_RESERVED_80) | \
      (0x0 << SIL_RESERVED_79) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_DISABLED, \
      SIL_RESERVED_729, \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_MASK | \
      SIL_RESERVED_91 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK7_MASK | \
      SIL_RESERVED_88 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK5_MASK | \
      SIL_RESERVED_85 | \
      SIL_RESERVED_83 | \
      SIL_RESERVED_81 | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
      IOAGR_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK, \
      (0x1 << SIL_RESERVED_93) | \
      (0x1 << SIL_RESERVED_92) | \
      (0x1 << SIL_RESERVED_90) | \
      (0x1 << SIL_RESERVED_89) | \
      (0x1 << SIL_RESERVED_87) | \
      (0x1 << SIL_RESERVED_86) | \
      (0x1 << SIL_RESERVED_84) | \
      (0x1 << SIL_RESERVED_82) | \
      (0x1 << SIL_RESERVED_80) | \
      (0x1 << SIL_RESERVED_79) \
      ),

  #define NBIO_SST_CLOCK_GATING_TBL \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_759, \
      SIL_RESERVED_766 | \
      SIL_RESERVED_764, \
      (0x1 << SIL_RESERVED_767) | \
      (0x1 << SIL_RESERVED_765) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_DISABLED, \
      SIL_RESERVED_759, \
      SIL_RESERVED_766 | \
      SIL_RESERVED_764, \
      (0x0 << SIL_RESERVED_767) | \
      (0x0 << SIL_RESERVED_765) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_759, \
      SIL_RESERVED_762, \
      (0xF0 << SIL_RESERVED_763) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_758, \
      SIL_RESERVED_484, \
      (0x1 << SIL_RESERVED_485) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_DISABLED, \
      SIL_RESERVED_758, \
      SIL_RESERVED_484, \
      (0x0 << SIL_RESERVED_485) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_761, \
      SIL_RESERVED_766 | \
      SIL_RESERVED_764, \
      (0x1 << SIL_RESERVED_767) | \
      (0x1 << SIL_RESERVED_765) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_DISABLED, \
      SIL_RESERVED_761, \
      SIL_RESERVED_766 | \
      SIL_RESERVED_764, \
      (0x0 << SIL_RESERVED_767) | \
      (0x0 << SIL_RESERVED_765) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_761, \
      SIL_RESERVED_762, \
      (0xF0 << SIL_RESERVED_763) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_ENABLED, \
      SIL_RESERVED_760, \
      SIL_RESERVED_484, \
      (0x1 << SIL_RESERVED_485) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_SST_CLKGATING_DISABLED, \
      SIL_RESERVED_760, \
      SIL_RESERVED_484, \
      (0x0 << SIL_RESERVED_485) \
      ),

  #define SIL_RESERVED_784 \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_741, \
      SIL_RESERVED_465 | \
      SIL_RESERVED_463 | \
      SIL_RESERVED_461 | \
      SIL_RESERVED_459 | \
      SIL_RESERVED_457 | \
      SIL_RESERVED_455 | \
      SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK | \
      SIL_RESERVED_453 | \
      SIL_RESERVED_451 | \
      SIL_RESERVED_449, \
      (0x0 << SIL_RESERVED_466) | \
      (0x0 << SIL_RESERVED_464) | \
      (0x0 << SIL_RESERVED_462) | \
      (0x0 << SIL_RESERVED_460) | \
      (0x0 << SIL_RESERVED_458) | \
      (0x0 << SIL_RESERVED_456) | \
      (0x0 << SIL_RESERVED_454) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET) | \
      (0x0 << SIL_RESERVED_452) | \
      (0x0 << SIL_RESERVED_450) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_742, \
      SIL_RESERVED_482 | \
      SIL_RESERVED_481 | \
      SIL_RESERVED_479 | \
      SIL_RESERVED_477 | \
      SIL_RESERVED_475 | \
      SIL_RESERVED_473 | \
      SIL_RESERVED_472 | \
      SIL_RESERVED_470 | \
      SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
      SIL_RESERVED_467, \
      (0x0 << SIL_RESERVED_483) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_OFFSET) | \
      (0x0 << SIL_RESERVED_480) | \
      (0x0 << SIL_RESERVED_478) | \
      (0x0 << SIL_RESERVED_476) | \
      (0x0 << SIL_RESERVED_474) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_OFFSET) | \
      (0x0 << SIL_RESERVED_471) | \
      (0x0 << SIL_RESERVED_469) | \
      (0x0 << SIL_RESERVED_468) \
      ),\
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_744, \
      SIL_RESERVED_465 | \
      SIL_RESERVED_463 | \
      SIL_RESERVED_461 | \
      SIL_RESERVED_459 | \
      SIL_RESERVED_457 | \
      SIL_RESERVED_455 | \
      SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK | \
      SIL_RESERVED_453 | \
      SIL_RESERVED_451 | \
      SIL_RESERVED_449, \
      (0x0 << SIL_RESERVED_466) | \
      (0x0 << SIL_RESERVED_464) | \
      (0x0 << SIL_RESERVED_462) | \
      (0x0 << SIL_RESERVED_460) | \
      (0x0 << SIL_RESERVED_458) | \
      (0x0 << SIL_RESERVED_456) | \
      (0x0 << SIL_RESERVED_454) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET) | \
      (0x0 << SIL_RESERVED_452) | \
      (0x0 << SIL_RESERVED_450) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_745, \
      SIL_RESERVED_482 | \
      SIL_RESERVED_481 | \
      SIL_RESERVED_479 | \
      SIL_RESERVED_477 | \
      SIL_RESERVED_475 | \
      SIL_RESERVED_473 | \
      SIL_RESERVED_472 | \
      SIL_RESERVED_470 | \
      SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK | \
      SIL_RESERVED_467, \
      (0x0 << SIL_RESERVED_483) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_OFFSET) | \
      (0x0 << SIL_RESERVED_480) | \
      (0x0 << SIL_RESERVED_478) | \
      (0x0 << SIL_RESERVED_476) | \
      (0x0 << SIL_RESERVED_474) | \
      (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_OFFSET) | \
      (0x0 << SIL_RESERVED_471) | \
      (0x0 << SIL_RESERVED_469) | \
      (0x0 << SIL_RESERVED_468) \
      ),
  #define NBIO_IOHC_POWER_GATING_TBL \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_PWR_GATING_ENABLED, \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_PWR_GATING_ENABLED, \
      SIL_RESERVED_735, \
      SIL_RESERVED_191, \
      (0x1 << SIL_RESERVED_192) \
      ), \
    SMN_ENTRY_PROPERTY_RMW ( \
      PROPERTY_IOHC_PWR_GATING_ENABLED, \
      SIL_RESERVED_730, \
      SIL_RESERVED_94, \
      (0x1 << SIL_RESERVED_95) \
      ), \

  #define NBIO_IOHC_ARBITRATION_TBL \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_548, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_582, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_619, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_654, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_685, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_712, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_722, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N0NBIO0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_583, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_620, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N3NBIO0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_686, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_713, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_723, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_549, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_584, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_621, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_550, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_585, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_622, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_655, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_687, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_656, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_688, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_714, \
      0x85218521 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_715, \
      0x85218521 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_553, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_588, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_624, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_659, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_691, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_718, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_726, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_554, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_589, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_660, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_692, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N5NBIO0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_727, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_546, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N1NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_652, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_683, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N5NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_720, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_547, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_581, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_653, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_684, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_711, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_721, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_551, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_586, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_657, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_689, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_716, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_724, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_552, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_587, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_623, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_658, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_690, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_717, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_725, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_540, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_573, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_612, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_647, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_541, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_574, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOAGR_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_648, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_542, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_575, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_613, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N0NBIO0_IOAGR_SION_S0_Client_Req_TimeSlot_Upper_ADDRESS, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_576, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_614, \
      0x21212121 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N3NBIO0_IOAGR_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_649, \
      0x84218421 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N0NBIO0_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_579, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOAGR_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_650, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_545, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_580, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_617, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_651, \
      0x08080808 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_538, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N1NBIO0_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N2NBIO0_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_646, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_539, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_572, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_611, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N3NBIO0_IOAGR_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_543, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_577, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_615, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N3NBIO0_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Lower_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_544, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_578, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SIL_RESERVED_616, \
      0x02020202 \
      ), \
    SMN_ENTRY_WR ( \
      SMN_IOHUB0_N3NBIO0_IOAGR_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_RMW ( \
      SIL_RESERVED_743, \
      SIL_RESERVED_773 | \
      SIL_RESERVED_776 | \
      SIL_RESERVED_775 | \
      SIL_RESERVED_778 | \
      USB_QoS_CNTL_UnitID1QoSPriority_MASK | \
      SIL_RESERVED_780, \
      (0x2F << SIL_RESERVED_774) | \
      (0x0 << SIL_RESERVED_777) | \
      (0x1 << USB_QoS_CNTL_UnitID0Enable_OFFSET) | \
      (0x30 << SIL_RESERVED_779) | \
      (0x0 << SIL_RESERVED_782) | \
      (0x1 << SIL_RESERVED_781) \
      ), \
    SMN_ENTRY_RMW ( \
      SIL_RESERVED_736, \
      SIL_RESERVED_193 | \
      IOHC_QOS_CONTROL_VC1QoSPriority_MASK | \
      SIL_RESERVED_195 | \
      SIL_RESERVED_197 | \
      SIL_RESERVED_199 | \
      IOHC_QOS_CONTROL_VC5QoSPriority_MASK | \
      SIL_RESERVED_202 | \
      SIL_RESERVED_204, \
      (0x0 << IOHC_QOS_CONTROL_VC0QoSPriority_OFFSET) | \
      (0x0 << SIL_RESERVED_194) | \
      (0x0 << SIL_RESERVED_196) | \
      (0x0 << SIL_RESERVED_198) | \
      (0x0 << SIL_RESERVED_200) | \
      (0x0 << SIL_RESERVED_201) | \
      (0x0 << SIL_RESERVED_203) | \
      (0x0 << SIL_RESERVED_205) \
      ),

  #define SIL_RESERVED_783 \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_557, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_565, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_592, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_603, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_626, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_636, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_663, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_674, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_695, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_704, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_558, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_566, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_593, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_604, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_627, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_637, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_664, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_675, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N4NBIO0_SDPMUX_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N4NBIO1_SDPMUX_SION_S0_Client_Req_BurstTarget_Upper_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_559, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N0NBIO1_SDPMUX_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_594, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_605, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N2NBIO0_SDPMUX_SION_S0_Client_Req_TimeSlot_Lower_ADDRESS, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_638, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_665, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_676, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_696, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_705, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_567, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_595, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_606, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_628, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_639, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_666, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_677, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_697, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_706, \
      0x21212121 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_562, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N0NBIO1_SDPMUX_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_597, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_608, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_631, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N2NBIO1_SDPMUX_SION_S1_Client_Req_BurstTarget_Lower_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_668, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_679, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_700, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_708, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N0NBIO0_SDPMUX_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_569, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_598, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_609, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_632, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_641, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_669, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_680, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_701, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_709, \
      0x08080808 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_563, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_570, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_599, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_610, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_633, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_642, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_670, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_681, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_702, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N4NBIO1_SDPMUX_SION_S1_Client_Req_TimeSlot_Lower_ADDRESS, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N0NBIO0_SDPMUX_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_571, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_600, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N1NBIO1_SDPMUX_SION_S1_Client_Req_TimeSlot_Upper_ADDRESS, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_634, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_645, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_671, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_682, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_703, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_710, \
      0x0 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_555, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_590, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_625, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_661, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_693, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_564, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_601, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N2NBIO1_SDPMUX_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_672, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N4NBIO1_SDPMUX_SION_S0_Client_RdRsp_BurstTarget_Lower_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_556, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_591, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N2NBIO0_SDPMUX_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_662, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_694, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N0NBIO1_SDPMUX_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_602, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_635, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_673, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N4NBIO1_SDPMUX_SION_S0_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_560, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_596, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_629, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_667, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_698, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_561, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N1NBIO0_SDPMUX_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_630, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SMN_IOHUB0_N3NBIO0_SDPMUX_SION_S1_Client_RdRsp_BurstTarget_Upper_ADDRESS, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_699, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_568, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_607, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_640, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_678, \
      0x02020202 \
      ), \
    SMN_ENTRY_PROPERTY_WR ( \
      PROPERTY_PRESENT_SDPMUX, \
      SIL_RESERVED_707, \
      0x02020202 \
      ),
