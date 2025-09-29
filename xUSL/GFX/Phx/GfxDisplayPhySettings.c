/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file GfxDisplayPhySettings.c
 * @brief This file contain the Gfx Display Phy Setting
 */


#include <stdint.h>
#include <GFX/Common/GfxDisplayPhySettings.h>

#pragma pack (push, 1)

ATOM_N6_DISPLAY_PHY_TUNING_SET display_phy_tuning_info[] = {
// struct atom_common_table_header table_header;
// struct atom_n6_display_phy_tuning_set disp_phy_tuning[];
// struct atom_n6_display_phy_tuning_set
// rbr, 1.62G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    22,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    31,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,    //uint8_t display_signal_type;
    0x1F,                    //uint8_t phy_sel;
    2,                       //uint8_t preset_level;
    0,                       //uint8_t reserved1;
    0,                       //uint32_t reserved2;
    16200,                   //uint32_t speed_upto;
    5,                       //uint8_t tx_vboost_level;
    2,                       //uint8_t tx_vreg_v2i;
    0,                       //uint8_t tx_vregdrv_byp;
    7,                       //uint8_t tx_term_cntl;
    3,                       //uint8_t tx_peak_level;
    0,                       //uint8_t tx_slew_en;
    0,                       //uint8_t tx_eq_pre;
    44,                      //uint8_t tx_eq_main;
    0,                       //uint8_t tx_eq_post;
    0,                       //uint8_t tx_en_inv_pre;
    0,                       //uint8_t tx_en_inv_post;
    0,                       //uint8_t reserved3;
    0,                       //uint32_t reserved4;
    0,                       //uint32_t reserved5;
    0,                       //uint32_t reserved6;
  },

// rbr, 1.62G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,   //uint8_t display_signal_type;
    0x1F,                   //uint8_t phy_sel;
    3,                      //uint8_t preset_level;
    0,                      //uint8_t reserved1;
    0,                      //uint32_t reserved2;
    16200,                  //uint32_t speed_upto;
    5,                      //uint8_t tx_vboost_level;
    2,                      //uint8_t tx_vreg_v2i;
    0,                      //uint8_t tx_vregdrv_byp;
    7,                      //uint8_t tx_term_cntl;
    3,                      //uint8_t tx_peak_level;
    0,                      //uint8_t tx_slew_en;
    0,                      //uint8_t tx_eq_pre;
    62,                     //uint8_t tx_eq_main;
    0,                      //uint8_t tx_eq_post;
    0,                      //uint8_t tx_en_inv_pre;
    0,                      //uint8_t tx_en_inv_post;
    0,                      //uint8_t reserved3;
    0,                      //uint32_t reserved4;
    0,                      //uint32_t reserved5;
    0,                      //uint32_t reserved6;
  },

// rbr, 1.62G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    27,                       //uint8_t tx_eq_main;
    5,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    37,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    53,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    33,                       //uint8_t tx_eq_main;
    11,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    47,                       //uint8_t tx_eq_main;
    15,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// rbr, 1.62G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    16200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    42,                       //uint8_t tx_eq_main;
    20,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    22,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    31,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    27,                       //uint8_t tx_eq_main;
    5,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    37,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    53,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    33,                       //uint8_t tx_eq_main;
    11,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    47,                       //uint8_t tx_eq_main;
    15,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr, 2.7G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    27000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    42,                       //uint8_t tx_eq_main;
    20,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    29,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// // hbr2, 5.4G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr2, 5.4G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    54000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    29,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// hbr3, 8.1G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    81000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p0
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p1
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    56,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p2
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    51,                       //uint8_t tx_eq_main;
    11,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p3
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    14,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p4
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    4,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    43,                       //uint8_t tx_eq_main;
    19,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p5
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    5,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    59,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p6
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    6,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    53,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p7
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    7,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    49,                       //uint8_t tx_eq_main;
    10,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10G, p8
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    45,                       //uint8_t tx_eq_main;
    14,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p9
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    42,                       //uint8_t tx_eq_main;
    17,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p10
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    10,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    6,                        //uint8_t tx_eq_pre;
    56,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p11
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    11,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    6,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p12
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    12,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    6,                        //uint8_t tx_eq_pre;
    47,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p13
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    13,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    6,                        //uint8_t tx_eq_pre;
    43,                       //uint8_t tx_eq_main;
    13,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p14
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    14,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    10,                       //uint8_t tx_eq_pre;
    51,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// uhbr10, 10.0G, p15
  {
    ATOM_ENCODER_MODE_DP2,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    15,                       //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    100000,                   //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    3,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    3,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.16G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    21600,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 2.43G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    24300,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 3.24G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    32400,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,    //uint8_t display_signal_type;
    0x1F,                    //uint8_t phy_sel;
    3,                       //uint8_t preset_level;
    0,                       //uint8_t reserved1;
    0,                       //uint32_t reserved2;
    43200,                   //uint32_t speed_upto;
    5,                       //uint8_t tx_vboost_level;
    2,                       //uint8_t tx_vreg_v2i;
    0,                       //uint8_t tx_vregdrv_byp;
    7,                       //uint8_t tx_term_cntl;
    3,                       //uint8_t tx_peak_level;
    0,                       //uint8_t tx_slew_en;
    0,                       //uint8_t tx_eq_pre;
    62,                      //uint8_t tx_eq_main;
    0,                       //uint8_t tx_eq_post;
    0,                       //uint8_t tx_en_inv_pre;
    0,                       //uint8_t tx_en_inv_post;
    0,                       //uint8_t reserved3;
    0,                       //uint32_t reserved4;
    0,                       //uint32_t reserved5;
    0,                       //uint32_t reserved6;
  },

// eDP, 4.32G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 4.32G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    43200,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,    //uint8_t display_signal_type;
    0x1F,                    //uint8_t phy_sel;
    3,                       //uint8_t preset_level;
    0,                       //uint8_t reserved1;
    0,                       //uint32_t reserved2;
    64800,                   //uint32_t speed_upto;
    5,                       //uint8_t tx_vboost_level;
    2,                       //uint8_t tx_vreg_v2i;
    0,                       //uint8_t tx_vregdrv_byp;
    7,                       //uint8_t tx_term_cntl;
    3,                       //uint8_t tx_peak_level;
    0,                       //uint8_t tx_slew_en;
    0,                       //uint8_t tx_eq_pre;
    62,                      //uint8_t tx_eq_main;
    0,                       //uint8_t tx_eq_post;
    0,                       //uint8_t tx_en_inv_pre;
    0,                       //uint8_t tx_en_inv_post;
    0,                       //uint8_t reserved3;
    0,                       //uint32_t reserved4;
    0,                       //uint32_t reserved5;
    0,                       //uint32_t reserved6;
  },

// eDP, 6.48G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x0A,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    55,                       //uint8_t tx_eq_main;
    7,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.48G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    64800,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE0_VS0_0x0(PreEmphasis=0 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    30,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE0_VS1_0x1(PreEmphasis=0 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    1,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    38,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE0_VS2_0x2(PreEmphasis=0 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    2,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    48,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE0_VS3_0x3(PreEmphasis=0 and VolateSwing=3)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    3,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE1_VS0_0x8(PreEmphasis=1 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    8,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    34,                       //uint8_t tx_eq_main;
    4,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE1_VS1_0x9(PreEmphasis=1 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    9,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    44,                       //uint8_t tx_eq_main;
    6,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE1_VS2_0xA(PreEmphasis=1 and VolateSwing=2)
  {
    ATOM_ENCODER_MODE_DP,    //uint8_t display_signal_type;
    0x1F,                    //uint8_t phy_sel;
    0x0A,                    //uint8_t preset_level;
    0,                       //uint8_t reserved1;
    0,                       //uint32_t reserved2;
    67500,                   //uint32_t speed_upto;
    5,                       //uint8_t tx_vboost_level;
    2,                       //uint8_t tx_vreg_v2i;
    0,                       //uint8_t tx_vregdrv_byp;
    7,                       //uint8_t tx_term_cntl;
    3,                       //uint8_t tx_peak_level;
    0,                       //uint8_t tx_slew_en;
    0,                       //uint8_t tx_eq_pre;
    55,                      //uint8_t tx_eq_main;
    7,                       //uint8_t tx_eq_post;
    0,                       //uint8_t tx_en_inv_pre;
    0,                       //uint8_t tx_en_inv_post;
    0,                       //uint8_t reserved3;
    0,                       //uint32_t reserved4;
    0,                       //uint32_t reserved5;
    0,                       //uint32_t reserved6;
  },

// eDP, 6.75G, PE2_VS0_0x10(PreEmphasis=2 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x10,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    39,                       //uint8_t tx_eq_main;
    9,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE2_VS1_0x11(PreEmphasis=2 and VolateSwing=1)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x11,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    50,                       //uint8_t tx_eq_main;
    12,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// eDP, 6.75G, PE3_VS0_0x18(PreEmphasis=3 and VolateSwing=0)
  {
    ATOM_ENCODER_MODE_DP,     //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0x18,                     //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    67500,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    46,                       //uint8_t tx_eq_main;
    16,                       //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// TMDS, 3.0G
  {
    ATOM_ENCODER_MODE_DVI,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    30000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// TMDS, 4.0G
  {
    ATOM_ENCODER_MODE_DVI,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    40000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// TMDS, 6.0G
  {
    ATOM_ENCODER_MODE_DVI,    //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    60000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// HDMI, 3.0G
  {
    ATOM_ENCODER_MODE_HDMI,   //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    30000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// HDMI/TMDS, 4.0G
  {
    ATOM_ENCODER_MODE_HDMI,   //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    40000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// HDMI/TMDS, 6.0G
  {
    ATOM_ENCODER_MODE_HDMI,   //uint8_t display_signal_type;
    0x1F,                     //uint8_t phy_sel;
    0,                        //uint8_t preset_level;
    0,                        //uint8_t reserved1;
    0,                        //uint32_t reserved2;
    60000,                    //uint32_t speed_upto;
    5,                        //uint8_t tx_vboost_level;
    2,                        //uint8_t tx_vreg_v2i;
    0,                        //uint8_t tx_vregdrv_byp;
    7,                        //uint8_t tx_term_cntl;
    3,                        //uint8_t tx_peak_level;
    0,                        //uint8_t tx_slew_en;
    0,                        //uint8_t tx_eq_pre;
    62,                       //uint8_t tx_eq_main;
    0,                        //uint8_t tx_eq_post;
    0,                        //uint8_t tx_en_inv_pre;
    0,                        //uint8_t tx_en_inv_post;
    0,                        //uint8_t reserved3;
    0,                        //uint32_t reserved4;
    0,                        //uint32_t reserved5;
    0,                        //uint32_t reserved6;
  },

// HDMI/FRL3G TxFFE0
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    0,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    30000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    49,                           //uint8_t tx_eq_main;
    8,                            //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL3G TxFFE1
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    1,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    30000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    47,                           //uint8_t tx_eq_main;
    10,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL3G TxFFE2
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    2,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    30000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    44,                           //uint8_t tx_eq_main;
    13,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL3G TxFFE3
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    3,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    30000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    41,                           //uint8_t tx_eq_main;
    16,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL6G TxFFE0
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    0,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    60000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    49,                           //uint8_t tx_eq_main;
    8,                            //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL6G TxFFE1
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    1,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    60000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    47,                           //uint8_t tx_eq_main;
    10,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL6G TxFFE2
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    2,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    60000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    44,                           //uint8_t tx_eq_main;
    13,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL6G TxFFE3
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    3,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    60000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    41,                           //uint8_t tx_eq_main;
    16,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL8G TxFFE0
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    0,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    80000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    49,                           //uint8_t tx_eq_main;
    8,                            //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL8G TxFFE1
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    1,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    80000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    47,                           //uint8_t tx_eq_main;
    10,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL8G TxFFE2
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    2,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    80000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    44,                           //uint8_t tx_eq_main;
    13,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL8G TxFFE3
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    3,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    80000,                        //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    41,                           //uint8_t tx_eq_main;
    16,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL10G TxFFE0
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    0,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    100000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    49,                           //uint8_t tx_eq_main;
    8,                            //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL10G TxFFE1
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    1,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    100000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    47,                           //uint8_t tx_eq_main;
    10,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL10G TxFFE2
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    2,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    100000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    44,                           //uint8_t tx_eq_main;
    13,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL10G TxFFE3
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    3,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    100000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    41,                           //uint8_t tx_eq_main;
    16,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL12G TxFFE0
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    0,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    120000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    49,                           //uint8_t tx_eq_main;
    8,                            //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL12G TxFFE1
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    1,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    120000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    47,                           //uint8_t tx_eq_main;
    10,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL12G TxFFE2
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    2,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    120000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    44,                           //uint8_t tx_eq_main;
    13,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  },

// HDMI/FRL12G TxFFE3
  {
    ATOM_ENCODER_MODE_HDMI_FRL,   //uint8_t display_signal_type;
    0x1F,                         //uint8_t phy_sel;
    3,                            //uint8_t preset_level;
    0,                            //uint8_t reserved1;
    0,                            //uint32_t reserved2;
    120000,                       //uint32_t speed_upto;
    5,                            //uint8_t tx_vboost_level;
    2,                            //uint8_t tx_vreg_v2i;
    0,                            //uint8_t tx_vregdrv_byp;
    7,                            //uint8_t tx_term_cntl;
    3,                            //uint8_t tx_peak_level;
    0,                            //uint8_t tx_slew_en;
    5,                            //uint8_t tx_eq_pre;
    41,                           //uint8_t tx_eq_main;
    16,                           //uint8_t tx_eq_post;
    0,                            //uint8_t tx_en_inv_pre;
    0,                            //uint8_t tx_en_inv_post;
    0,                            //uint8_t reserved3;
    0,                            //uint32_t reserved4;
    0,                            //uint32_t reserved5;
    0,                            //uint32_t reserved6;
  }
};

ATOM_COMMON_TABLE_HEADER table_header = {
  sizeof (display_phy_tuning_info) + sizeof (ATOM_COMMON_TABLE_HEADER), //uint16_t structure size;
  APU_TABLE_FORMAT_REVISION,                                            //uint8_t  format_revision;
  APU_TABLE_CONTENT_REVISION,                                           //uint8_t  content_revision;
};

#pragma pack (pop)
