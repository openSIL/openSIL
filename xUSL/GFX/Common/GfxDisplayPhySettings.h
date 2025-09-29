/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxDisplayPhySettings.h
 *  @brief Gfx function prototypes, structs, and defines
 */

#pragma once

#define APU_TABLE_FORMAT_REVISION        1
#define APU_TABLE_CONTENT_REVISION       0


/****************************************************************************
 * Common header for all tables (Data table, Command function).
 * Every table pointed in _ATOM_MASTER_DATA_TABLE has this common header.
 * And the pointer actually points to this header.
 ****************************************************************************/
//IPCLEAN_END
typedef struct _atom_common_table_header {
  uint16_t structuresize;
  uint8_t  format_revision;   //mainly used for a hw function, when the parser is not backward compatible
  uint8_t  content_revision;  //change it when a data table has a structure change,
                              //or a hw function has a input/output parameter change
} ATOM_COMMON_TABLE_HEADER;
//IPCLEAN_START

//ucEncoderMode
typedef enum {
  ATOM_ENCODER_MODE_DP = 0,
  ATOM_ENCODER_MODE_DP_SST = 0,
  ATOM_ENCODER_MODE_RESERVED = 1,
  ATOM_ENCODER_MODE_DVI = 2,
  ATOM_ENCODER_MODE_HDMI = 3,
  ATOM_ENCODER_MODE_HDMI_FRL = 4,
  ATOM_ENCODER_MODE_DP_AUDIO = 5,
  ATOM_ENCODER_MODE_DP_MST = 5,
  ATOM_ENCODER_MODE_DP2 = 8,
  ATOM_ENCODER_MODE_CRT = 15,
  ATOM_ENCODER_MODE_DVO = 16,
} ATOM_ENCODE_MODE_DEF;

typedef struct _atom_n6_display_phy_tuning_set {
  uint8_t display_signal_type;
  uint8_t phy_sel;
  uint8_t preset_level;
  uint8_t reserved1;
  uint32_t reserved2;
  uint32_t speed_upto;
  uint8_t tx_vboost_level;
  uint8_t tx_vreg_v2i;
  uint8_t tx_vregdrv_byp;
  uint8_t tx_term_cntl;
  uint8_t tx_peak_level;
  uint8_t tx_slew_en;
  uint8_t tx_eq_pre;
  uint8_t tx_eq_main;
  uint8_t tx_eq_post;
  uint8_t tx_en_inv_pre;
  uint8_t tx_en_inv_post;
  uint8_t tx_slew_ctrl_val;
  uint32_t reserved4;
  uint32_t reserved5;
  uint32_t reserved6;
} ATOM_N6_DISPLAY_PHY_TUNING_SET;

typedef struct _atom_display_phy_tuning_info {
  ATOM_COMMON_TABLE_HEADER table_header;
  ATOM_N6_DISPLAY_PHY_TUNING_SET disp_phy_tuning[];
} ATOM_DISPLAY_PHY_TUNING_INFO;
