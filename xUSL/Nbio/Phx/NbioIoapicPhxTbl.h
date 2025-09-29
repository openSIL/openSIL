/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioIoapicPhxTbl.h
 * @brief This file contains the Nbio Ioapic configurations
 *
 */

#pragma once

#include <includePHX/PHX_IOAPIC.h>

// --------------------------------------------------
// IOAPIC Clock Gating and Power Management
// --------------------------------------------------

  #define NBIO_IOAPIC_CLOCK_GATING_TBL \
          SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED, \
  IOAPICx14300100, \
  IOAPIC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_MASK | \
  BIT30_MASK_AAAAAXR | \
  BIT31_MASK_AAAAAXP, \
  (0x0 << BIT29_OFFSET_AAAAAXU) | \
  (0x0 << BIT30_OFFSET_AAAAAXS) | \
  (0x0 << BIT31_OFFSET_AAAAAXQ) \
  ),

// --------------------------------------------------
// IOAPIC Initialization
// --------------------------------------------------
  #define NBIO_IOAPIC_INTR_ROUTING_TBL \
          SMN_ENTRY_RMW(IOAPICx14300040, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300044, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300048, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx1430004c, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300050, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300054, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300058, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx1430005c, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300060, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300064, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300068, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx1430006c, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300070, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300074, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300078, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x0 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx1430007c, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300080, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x2 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300084, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x3 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx14300088, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ), \
          SMN_ENTRY_RMW(IOAPICx1430008c, \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_MASK | \
  IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_MASK, \
  (0x5 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_grp_OFFSET) | \
  (0x1 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_swz_OFFSET) | \
  (0x4 << IOAPIC_BR_INTERRUPT_ROUTING_Br_ext_Intr_map_OFFSET) \
  ),
