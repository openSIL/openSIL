/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxDisplayTypeSettings.h
 *  @brief Gfx function prototypes, structs, and defines
 */

#pragma once
#define DDI_TABLE_FORMAT_REVISION        1
#define DDI_TABLE_CONTENT_REVISION       0
#define DDI_TABLE_PORT_NUMBER            5


#pragma pack(push,1)

typedef struct _ddi_table_header {
  uint16_t structuresize;
  uint8_t  format_revision;   //mainly used for a hw function, when the parser is not backward compatible
  uint8_t  content_revision;  //change it when a data table has a structure change
} DDI_TABLE_HEADER;

/**
 * @brief DDI channel lane mapping
 * @details
 */

typedef struct {                                  ///< Structure that describe lane mapping
  uint8_t              Lane0   :2;                /**< Lane 0 mapping
                                                   *  @li @b 0 - Map to lane 0
                                                   *  @li @b 1 - Map to lane 1
                                                   *  @li @b 2 - Map to lane 2
                                                   *  @li @b 2 - Map to lane 3
                                                   */
  uint8_t              Lane1   :2;                ///< Lane 1 mapping (see "Lane 0 mapping")
  uint8_t              Lane2   :2;                ///< Lane 2 mapping (see "Lane 0 mapping")
  uint8_t              Lane3   :2;                ///< Lane 3 mapping (see "Lane 0 mapping")
} DXIO_CHANNEL_MAPPING;

/// @endcond

/**
 * @brief Common channel mapping
 * @details
 */
typedef union {
  uint8_t                ChannelMappingValue;     ///< Raw lane mapping
  DXIO_CHANNEL_MAPPING      ChannelMapping;       ///< Channel mapping
} DXIO_CONN_CHANNEL_MAPPING;

/**
 * @brief DDI configuration data
 * @details
 */
typedef struct  {
  uint8_t                ConnectorType;          /**< Display Connector Type
                                                  *  @li @b 0 - DP
                                                  *  @li @b 1 - eDP
                                                  *  @li @b 2 - Single Link DVI-D
                                                  *  @li @b 3 - Dual  Link DVI-D
                                                  *  @li @b 4 - HDMI
                                                  *  @li @b 5 - DP-to-VGA
                                                  *  @li @b 7 - DP-to-VGA (Reserved)
                                                  *  @li @b 8 - Single Link DVI-I
                                                  *  @li @b 11 - Auto
                                                  */
  uint8_t                AuxIndex;               /**< Indicates which AUX or DDC Line is used
                                                  *  @li @b 0 - AUX1
                                                  *  @li @b 1 - AUX2
                                                  *  @li @b 2 - AUX3
                                                  *  @li @b 3 - AUX4
                                                  *  @li @b 4 - AUX5
                                                  *  @li @b 5 - AUX6
                                                  */
  uint8_t                HdpIndex;               /**< Indicates which HDP pin is used
                                                  *  @li @b 0 - HDP1
                                                  *  @li @b 1 - HDP2
                                                  *  @li @b 2 - HDP3
                                                  *  @li @b 3 - HDP4
                                                  *  @li @b 4 - HDP5
                                                  *  @li @b 5 - HDP6
                                                  */
  DXIO_CONN_CHANNEL_MAPPING Mapping[2];          /**< Set specific mapping of lanes to connector pins
                                                  *  @li Mapping[0]
                                                  *  @li Mapping[1]
                                                  *  if Mapping[x] set to 0 than default mapping assumed
                                                  */
  uint8_t                LanePnInversionMask;    /**< Specifies whether to invert the state of P and N for each lane.
                                                  *  Each bit represents a PCIe lane on the DDI port.
                                                  *  @li 0 - Do not invert (default)
                                                  *  @li 1 - Invert P and N on this lane
                                                  */
  uint8_t                Flags;                  /**< Capabilities flags
                                                  *  @li Flags bit[0] DDI_DATA_FLAGS_DP1_1_ONLY
                                                  *  @li Flags bit[7:1] Reserved
                                                  */
} DDI_DATA;

/**
 * @brief DDI descriptor
 * @details
 */
typedef struct {
  uint32_t          Flags;                    /**< Descriptor flags
                                               * @li @b Bit31 - last descriptor in complex */
  DDI_DATA          Ddi;                      ///< DDI port specific configuration info
} DDI_DESCRIPTOR;

typedef struct _gfx_ddi_config_info {
  DDI_TABLE_HEADER table_header;
  DDI_DESCRIPTOR   ddi_descriptor[];
} GFX_DDI_CONFIG_INFO;
#pragma pack(pop)
