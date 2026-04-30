/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxV4.h
 *  @brief ATOM GFX structs and defines
 */

#pragma once

#define DEVICE_DFP                                0x1
#define DEVICE_CRT                                0x2
#define DEVICE_LCD                                0x3

#define CONNECTOR_DISPLAYPORT_ENUM                0x3013
#define CONNECTOR_HDMI_TYPE_A_ENUM                0x300c
#define CONNECTOR_SINGLE_LINK_DVI_D_ENUM          0x3003
#define CONNECTOR_DUAL_LINK_DVI_D_ENUM            0x3004
#define CONNECTOR_SINGLE_LINK_DVI_I_ENUM          0x3001
#define CONNECTOR_DUAL_LINK_DVI_I_ENUM            0x3002
#define CONNECTOR_VGA_ENUM                        0x3005
#define CONNECTOR_LVDS_ENUM                       0x300E
#define CONNECTOR_eDP_ENUM                        0x3014
#define CONNECTOR_LVDS_eDP_ENUM                   0x3016

#define ENCODER_DP2VGA_ENUM_ID1                   0x2123
#define ENCODER_DP2LVDS_ENUM_ID2                  0x2223
#define ENCODER_ALMOND_ENUM_ID1                   0x2122
#define ENCODER_NOT_PRESENT                       0x0000

#define eDP_TO_LVDS_SWINIT_ID                     0x02


#define ATOM_DEVICE_CRT1_SUPPORT                  0x0001
#define ATOM_DEVICE_DFP1_SUPPORT                  0x0008
#define ATOM_DEVICE_DFP6_SUPPORT                  0x0040
#define ATOM_DEVICE_DFP2_SUPPORT                  0x0080
#define ATOM_DEVICE_DFP3_SUPPORT                  0x0200
#define ATOM_DEVICE_DFP4_SUPPORT                  0x0400
#define ATOM_DEVICE_DFP5_SUPPORT                  0x0800
#define ATOM_DEVICE_LCD1_SUPPORT                  0x0002
#define ATOM_DEVICE_LCD2_SUPPORT                  0x0020

typedef enum {
  ATOM_ENCODER_MODE_DP        = 0,
  ATOM_ENCODER_MODE_DP_SST    = 0,
  ATOM_ENCODER_MODE_LVDS      = 1,
  ATOM_ENCODER_MODE_DVI       = 2,
  ATOM_ENCODER_MODE_HDMI      = 3,
  ATOM_ENCODER_MODE_HDMI_FRL  = 4,
  ATOM_ENCODER_MODE_DP_AUDIO  = 5,
  ATOM_ENCODER_MODE_DP_MST    = 5,
  ATOM_ENCODER_MODE_DP2       = 8,
  ATOM_ENCODER_MODE_CRT       = 15,
  ATOM_ENCODER_MODE_DVO       = 16,
} ATOM_ENCODE_MODE_DEF;

#pragma pack(1)
typedef struct _atom_common_table_header {
  uint16_t structuresize;
  uint8_t  format_revision;   //mainly used for a hw function, when the parser is not backward compatible
  uint8_t  content_revision;  //change it when a data table has a structure change, or a hw function has a input/output parameter change
} ATOM_COMMON_TABLE_HEADER;

typedef struct _edp_info_table
{
  uint16_t edp_backlight_pwm_hz;
  uint16_t edp_ss_percentage;
  uint16_t edp_ss_rate_10hz;
  uint16_t reserved1;
  uint32_t reserved2;
  uint8_t  edp_pwr_on_off_delay;
  uint8_t  edp_pwr_on_vary_bl_to_blon;
  uint8_t  edp_pwr_down_bloff_to_vary_bloff;
  uint8_t  edp_panel_bpc;
  uint8_t  edp_bootup_bl_level;
  uint8_t   reserved3[3];
  uint32_t reserved4[3];
} EDP_INFO_TABLE;

typedef struct _ext_display_path
{
  uint16_t  usDeviceTag;
  uint16_t  usDeviceACPIEnum;
  uint16_t  usDeviceConnector;
  uint8_t   ucExtAUXDDCLutIndex;
  uint8_t   ucExtHPDPINLutIndex;
  uint16_t  usExtEncoderObjId;
  uint8_t   ucChannelMapping;
  uint8_t   ucChPNInvert;
  uint32_t  usCaps;
} EXT_DISPLAY_PATH;

//usCaps
typedef enum {
  EXT_DISPLAY_PATH_CAPS__HBR2_DISABLE = 0x0001,
  EXT_DISPLAY_PATH_CAPS__DP_FIXED_VS_EN = 0x0002,
  EXT_DISPLAY_PATH_CAPS__EXT_CHIP_MASK = 0x007C,
  EXT_DISPLAY_PATH_CAPS__HDMI20_PI3EQX1204      = (0x01 << 2),
  EXT_DISPLAY_PATH_CAPS__HDMI20_TISN65DP159RSBT = (0x02 << 2),
  EXT_DISPLAY_PATH_CAPS__HDMI20_PARADE_PS175    = (0x03 << 2),
  EXT_DISPLAY_PATH_CAPS__HBR3_DISABLE = 0x0080,
  EXT_DISPLAY_PATH_CAPS__USB_C_TYPE = 0x100,
  EXT_DISPLAY_PATH_CAPS__HDMI20_DISABLE = 0x200,
  EXT_DISPLAY_PATH_CAPS__DFPx_INTERNAL_DISP_EN = 0x400,
  EXT_DISPLAY_PATH_CAPS__DP_HAS_RETIMER = 0x800,
  EXT_DISPLAY_PATH_CAPS__DP2 = 0x100000,
  EXT_DISPLAY_PATH_CAPS__UHBR10_EN = 0x200000,
  EXT_DISPLAY_PATH_CAPS__UHBR13_5_EN = 0x400000,
  EXT_DISPLAY_PATH_CAPS__RECORD_UHBR20_EN = 0x800000,
  EXT_DISPLAY_PATH_CAPS__HDMI_FRL = 0x2000000,
  EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn = 0x4000000,
  EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn = 0x8000000,
  EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn = 0x10000000,
  EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE = 0x20000000,
} EXT_DISPLAY_PATH_CAP_DEF;

typedef struct _atom_external_display_connection_info {
  ATOM_COMMON_TABLE_HEADER  sHeader;
  uint8_t                  ucGuid[16];                                  // a GUID is a 16 byte long string
  EXT_DISPLAY_PATH         sPath[7];                                   // total of fixed 7 entries.
  uint8_t                  ucChecksum;                                  // a simple Checksum of the sum of whole structure equal to 0x0.
  uint8_t                  stereopinid;                               // use for eDP panel
  uint8_t                  ucRemoteDisplayConfig;
  uint8_t                  uceDPToLVDSRxId;
  uint8_t                  ucFixDPVoltageSwing;                         // usCaps[1]=1, this indicate DP_LANE_SET value
  uint8_t                  reserved[3];                               // for potential expansion
} ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO;

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

typedef struct _atom_integrated_system_info_v2_2
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t  vbios_misc;
  uint32_t  gpucapinfo;
  uint32_t  system_config;
  uint32_t  cpucapinfo;
  uint16_t  gpuclk_ss_percentage;
  uint16_t  gpuclk_ss_type;
  uint16_t  dpphy_override;
  uint8_t   memorytype;
  uint8_t   umachannelnumber;
  uint8_t   htc_hyst_limit;
  uint8_t   htc_tmp_limit;
  uint8_t   reserved1;
  uint8_t   gpu_package_id;
  EDP_INFO_TABLE edp1_info;
  EDP_INFO_TABLE edp2_info;
  uint32_t  reserved3[8];
  ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO extdispconninfo;
  uint32_t  reserved4[25];
  uint32_t  UMACarveoutIndexMax;
  uint32_t  UMACarveoutIndexDefault;
  uint32_t  UMACarveoutIndex;
  uint8_t   UMACarveoutID[4];
  uint32_t  reserved5[160];
} ATOM_INTEGRATED_SYSTEM_INFO_V2_2;

// gpucapinfo
typedef enum {
  SYS_INFO_GPUCAPS__ENABEL_DFS_BYPASS  = 0x10,
  SYS_INFO_GPUCAPS__EXT_HDMI_INIT_PER_PORT = 0x20,
  SYS_INFO_GPUCAPS__DDS_SUPPORT = 0x40,
  SYS_INFO_GPUCAPS__BR3_SDR_SUPPORT = 0x80,
  SYS_INFO_GPUCAPS__SEAMLESS_SUPPORT = 0x100,
  SYS_INFO_GPUCAPS__TCON_INSTANT_ON_LOGO = 0x01,
  SYS_INFO_GPUCAPS__USB4_DPIA_BW_ALLOC = 0x02,
} ATOM_SYSTEM_GPUCAPINF_DEF;
typedef enum {
  OtherMemType = 0x01,
  UnknownMemType,
  DramMemType,
  EdramMemType,
  VramMemType,
  SramMemType,
  RamMemType,
  RomMemType,
  FlashMemType,
  EepromMemType,
  FepromMemType,
  EpromMemType,
  CdramMemType,
  ThreeDramMemType,
  SdramMemType,
  SgramMemType,
  RdramMemType,
  DdrMemType,
  Ddr2MemType,
  Ddr2FbdimmMemType,
  Ddr3MemType = 0x18,
  Fbd2MemType,
  Ddr4MemType,
  LpDdrMemType,
  LpDdr2MemType,
  LpDdr3MemType,
  LpDdr4MemType,
  GDdr6MemType,
  HbmMemType,
  Hbm2MemType,
  Ddr5MemType = 0x22,
  LpDdr5MemType,
} ATOM_DMI_T17_MEM_TYPE_DEF;

typedef struct {
  ATOM_INTEGRATED_SYSTEM_INFO_V2_2   sIntegratedSysInfo;
  uint32_t   powerplayinfo[256];                                // Reserve 1024 bytes space for PowerPlayInfoTable
} ATOM_FUSION_SYSTEM_INFO_V6;

#pragma pack()
