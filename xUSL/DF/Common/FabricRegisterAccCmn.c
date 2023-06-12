/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  FabricRegisterAccCmn.c
 * @brief Common function implementations for Df fabric register access
 *
 */

#include "FabricRegisterAccCmn.h"
#include "DfCmn2Rev.h"
#include <APOB/ApobIp2Ip.h>

/**
 * DfFabricRegisterAccRMW
 *
 * @brief Routine to read-modify-write a register
 *
 * @param[in] Socket             Processor socket to read from
 * @param[in] Die                Processor die to read from
 * @param[in] Function           Function number to read from
 * @param[in] Offset             Register to read
 * @param[in] Instance           Instance ID of the target fabric device
 * @param[in] NandValue          Value to NAND with the current register value
 * @param[in] OrValue            Value to OR with the current register value
 * @retval    Value written to target register if interested
 */
uint32_t DfFabricRegisterAccRMW (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t NandValue,
  uint32_t OrValue
  )
{
  uint32_t                    RegisterValue;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  assert(Socket < 2);
  assert(Function < 8);
  assert(Offset < 0x2000);
  assert((Offset & 3) == 0);
  assert(Instance <= FABRIC_REG_ACC_BC);

  RegisterValue = (DfXfer->DfFabricRegisterAccRead(Socket, Die, Function, Offset, Instance) & ~NandValue) | OrValue;
  DfXfer->DfFabricRegisterAccWrite(Socket, Die, Function, Offset, Instance, RegisterValue);

  return RegisterValue;
}

/**
 * DfFabricRegisterAccGetPciDeviceNumberOfDie
 *
 * @brief Routine to get PCI device number of a die
 *
 * @param[in] Socket             Processor socket to read from
 * @retval    PCI device number
 */
uint32_t DfFabricRegisterAccGetPciDeviceNumberOfDie (
  uint32_t Socket,
  uint32_t Die
  )
{
  APOB_IP2IP_API    *ApobIp2IpApi;
  APOB_SOC_DIE_INFO  SocMaxDieInfo;

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  return (Socket << xUslBitWidth(SocMaxDieInfo.MaxSocDiesPerSocketValue - 1)) + Die + 0x18;
}
