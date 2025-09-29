/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemSpd4.h
 * @brief This file contains DDR4 SPD Structures
 */

#pragma once

#include <SilCommon.h>

#pragma pack(push, 1)

#define SPD_DRAM_DEVICE_TYPE           2
  #define DEVICE_TYPE_LPDDR5           0x13
  #define DEVICE_TYPE_LPDDR5X          0x15

#define SPD_BASEMODULE_SOUDIMM          9
#define SPD_BASE_NONDIMMSOLUTION        14

#define SPD_BASE_MODULE_TYPE           3
  #define MODULE_TYPE_MASK             0x0F
  #define MOD_TYPE_HYBRID              0x80
  #define MOD_TYPE_HYBRID_TYPE         0x70

#define SPD_CAPACITY                   4
  #define CAPACITY_MASK                0xF

#define SPD_CHANNELS_PER_PKG           6
  #define CHANNELS_PER_PKG_SHIFT       2
  #define CHANNELS_PER_PKG_MASK        3
#define SPD_DIE_COUNT                  6
  #define DIE_COUNT_SHIFT              4
  #define DIE_COUNT_MASK               7

#define SPD_DRAM_VDD                   11

#define SPD_DEVICE_WIDTH               12
  #define DEVICE_WIDTH_MASK             7
#define SPD_RANKS                      12
  #define RANKS_SHIFT                   3
  #define RANKS_MASK                    7
#define SPD_BYTE_MODE_ID               12
  #define BYTE_MODE_ID_SHIFT            6
  #define BYTE_MODE_ID_MASK             1

#define SPD_BUS_WIDTH                  13
#define BUSWIDTH_MASK                   7
#define SPD_BUSWIDTH_EXT               13
  #define BUSWIDTH_EXT_ECC             (1 << 3)
#define SPD_CHANNELS_PER_SYS           13
  #define CHANNELS_PER_SYS_SHIFT        5
  #define CHANNELS_PER_SYS_MASK         7

#define SPD_FINE_TIMEBASE              17
  #define FTB_MSK                       3
#define SPD_MEDIUM_TIMEBASE            17
  #define MTB_SHIFT                     2
  #define MTB_MSK                       3

#define SPD_TCK                        18

#define SPD_TCK_FTB                    125

#define SPD_MODULE_HEIGHT              128

#define SPD_NVDIMM_MODULE_PRODUCT_ID_LSB  192

#define SPD_NVDIMM_MODULE_PRODUCT_ID_MSB  193

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_MANUFACTURER_ID_LSB  194

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_MANUFACTURER_ID_MSB  195

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_ID_LSB  196

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_ID_MSB  197

#define SPD_NVDIMM_SUBSYSTEM_CONTROLLER_REVISION_CODE  198

#define SPD_NVDIMM_HYBRID_MODULE_MEDIA_TYPES_LSB  201

#define SPD_NVDIMM_FUNCTION_0_INTERFACE_DESCRIPTORS_LSB  204

#define SPD_NVDIMM_FUNCTION_7_INTERFACE_DESCRIPTORS_LSB  218

#define SPD_MANUFACTURER_ID_LSB        320

#define SPD_MANUFACTURER_ID_MSB        321

#define SPD_SERIAL_NUMBER              325

#define SPD_PART_NUMBER                329

typedef uint16_t SPD_CAPACITY_TABLE;

#pragma pack(pop)
