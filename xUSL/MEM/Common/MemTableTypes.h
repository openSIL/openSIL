/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemTableTypes.h
 * @brief Common Memory Table Structures
 *
 */

#pragma once

#include <stdint.h>

/**
 * @brief Host to APCB Channel Translation structure
 */
typedef struct _HOST_TO_APCB_CHANNEL_XLAT {
  uint8_t   RequestedChannelId;   ///< Requested Channel ID
  uint8_t   TranslatedChannelId;  ///< Translated Channel ID
} HOST_TO_APCB_CHANNEL_XLAT;

/**
 * @brief Structure defining Memory ticks
 * @details This provides the memory clock to tick ps value relationship, to be tabulated for reference
 */
typedef struct _MEM_TCK_ENTRY {
  uint16_t  Memclk; ///< Memory clock
  uint16_t  TckPs;  ///< Ticks ps
} MEM_TCK_ENTRY;
