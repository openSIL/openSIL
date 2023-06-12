/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file MemClass-api.h
 * @brief openSIL-Host MEM IP interface.
 *
 * @details ** Deprecated **
 */

#pragma once


#define ABL_APOB_MAX_SOCKETS_SUPPORTED   2   ///< Max number of sockets in system
#define ABL_APOB_MAX_CHANNELS_PER_SOCKET 12  ///< Max Channels per sockets
#define ABL_APOB_MAX_DIMMS_PER_CHANNEL   2   ///< Max dimms per die
#define ABL_APOB_MAX_CHANNELS_PER_DIE    8   ///< Max channels per die

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page Memory Memory IP api
 *
 *   The Mem
 *
 *   See the 'Files - @ref MemClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
/**
 * @cond FullCode_Doc
 * @ingroup MEM_group
 * @endcond
 *
 * Definition for the Mem class module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the MEM IP module.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP.
 *
 */
typedef struct {
      uint8_t   SocketNumber;
      uint8_t   ChannelNumber;
      uint8_t   DimmNumber;
      bool      DimmPresent;
      uint32_t  DeviceWidth;
      uint16_t  Size;
      uint16_t  ClockSpeed;
} MEM_DIMM_SPD_DATA_BLK;


typedef struct {
      uint8_t   MaxSocketsSupported;
      uint8_t   MaxDimmsPerChannel;
      uint8_t   MaxChannelsPerSocket;
      MEM_DIMM_SPD_DATA_BLK  DimmInfo[ABL_APOB_MAX_CHANNELS_PER_DIE * ABL_APOB_MAX_DIMMS_PER_CHANNEL];
} MEM_CLASS_INPUT_BLK;


