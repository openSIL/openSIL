/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchCmn2Sn.h
 * @brief OpenSIL Multi FCH 9004 C2R transfer block definitions
 *
 */

#pragma once

#include <Sil-api.h>

#define SN_SECOND_SOCKET             1
#define SN_SECOND_SOCKET_DIE         0
#define SN_SECOND_SOCKET_INDEX       2       //> Genoa use Index 2 to get 2nd socket bus number

/** FabricBlockInstanceInformation3_IOS
 * Reg(DF::FabricBlockInstanceInformation3_IOS)
 * Classification(NDA)
 * SOC(Genoa)
 */
#define IOS2_FABRIC_ID                0x22
#define FABRIC_ID_SOCKET_SHIFT        8

/**
 * GetSecFchBusNumberSn
 *
 * @brief Get the bus number of the secondary FCH
 *
 * @param   FchBusNum   Buffer to return the bus number of the secondary FCH.
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
GetSecFchBusNumberSn (
  uint8_t   *FchBusNum
);

/**
 * EnableIoCxxOnDieNSn
 *
 * @brief   Enable IO 0xCxx on Die N
 *
 * @param   IoBaseRegValOrg   Buffer to save the original IO base value.
 * @param   IoLimitRegValOrg  Buffer to save the original IO Limit value
 *
 * @return  SIL_STATUS
 * @retval  SilNotfound if IP-2-IP apis are not found.
 * @retval  SilPass is successfull
 */
SIL_STATUS
EnableIoCxxOnDieNSn (
  uint32_t          *IoBaseRegValOrg,
  uint32_t          *IoLimitRegValOrg
);

/**
 * DisableIoCxxOnDieNSn
 *
 * @brief   Disable IO 0xCxx on Die N
 *
 * @param   IoBaseRegValOrg   Original IO base value to restore.
 * @param   IoLimitRegValOrg  Original IO Limit value to restore.
 *
 * @return  SIL_STATUS
 * @retval  SilNotfound if IP-2-IP apis are not found.
 * @retval  SilPass is successfull
 */
SIL_STATUS
DisableIoCxxOnDieNSn (
  uint32_t          IoBaseRegValOrg,
  uint32_t          IoLimitRegValOrg
);

/**
 * GetSecRootBridgeNumberSn
 *
 * @brief Get the Root Bridge Number of the secondary FCH.
 *
 * @param   RootBridgeNumber  The root bridge number for the secondary FCH.
 *
 * @return  uint8_t  The root bridge of the secondary FCH.
 */
SIL_STATUS
GetSecRootBridgeNumberSn (
  uint8_t   *RootBridgeNumber
);
