/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
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
 * @param SilContext         A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param Socket             Processor socket to read from
 * @param Die                Processor die to read from
 * @param Function           Function number to read from
 * @param Offset             Register to read
 * @param Instance           Instance ID of the target fabric device
 * @param NandValue          Value to NAND with the current register value
 * @param OrValue            Value to OR with the current register value
 * @retval    Value written to target register if interested
 */
uint32_t
DfFabricRegisterAccRMW (
  SIL_CONTEXT          *SilContext,
  uint32_t             Socket,
  uint32_t             Die,
  uint32_t             Function,
  uint32_t             Offset,
  uint32_t             Instance,
  uint32_t             NandValue,
  uint32_t             OrValue
  )
{
  uint32_t                    RegisterValue;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  assert(Socket < 2);
  assert(Function < 8);
  assert(Offset < 0x2000);
  assert((Offset & 3) == 0);
  assert(Instance <= FABRIC_REG_ACC_BC);
  RegisterValue =
    (DfXfer->DfFabricRegisterAccRead(SilContext, Socket, Die, Function, Offset, Instance) & ~NandValue) | OrValue;
  DfXfer->DfFabricRegisterAccWrite(SilContext, Socket, Die, Function, Offset, Instance, RegisterValue);

  return RegisterValue;
}

/**
 * DfFabricRegisterAccGetPciDeviceNumberOfDie
 *
 * @brief Routine to get PCI device number of a die
 *
 * @param SilContext         A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param Socket             Processor socket to read from
 * @param Die                Processor die to read from
 * @retval    PCI device number
 */
uint32_t
DfFabricRegisterAccGetPciDeviceNumberOfDie (
  SIL_CONTEXT          *SilContext,
  uint32_t             Socket,
  uint32_t             Die
  )
{
  APOB_IP2IP_API    *ApobIp2IpApi;
  APOB_SOC_DIE_INFO  SocMaxDieInfo;
  if (SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  return (Socket << xUslBitWidth(SocMaxDieInfo.MaxSocDiesPerSocketValue - 1)) + Die + 0x18;
}
