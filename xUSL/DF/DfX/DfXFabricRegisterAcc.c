/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXFabricRegisterAcc.c
 * @brief Data fabric register access functions for DfX
 *
 */

#include <xSIM.h>
#include <Pci.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include "DfXFabricRegisterAcc.h"
#include "SilFabricRegistersDfX.h"

/**
 * DfXFabricRegisterAccRead
 *
 * @brief Routine to read a register
 *
 * @param SilContext         A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param Socket             Processor socket to read from
 * @param Die                Processor die to read from
 * @param Function           Function number to read from
 * @param Offset             Register to read
 * @param Instance           Instance ID of the target fabric device
 * @retval    Current value of the target register
 */
uint32_t
DfXFabricRegisterAccRead (
  SIL_CONTEXT         *SilContext,
  uint32_t            Socket,
  uint32_t            Die,
  uint32_t            Function,
  uint32_t            Offset,
  uint32_t            Instance
  )
{
  uint32_t                             RegisterValue;
  PCI_ADDR                             PciAddr;
  FABRIC_INDIRECT_CONFIG_ACCESS_ADDRESS_REGISTER FICAA3;

  assert(Socket < 2);
  assert(Die < 2);
  assert(Function < 8);
  assert(Offset < 0x2000);
  assert((Offset & 3) == 0);
  assert(Instance <= FABRIC_REG_ACC_BC);

  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = DfFabricRegisterAccGetPciDeviceNumberOfDie(SilContext, Socket, Die);
  if (Instance == FABRIC_REG_ACC_BC) {
    PciAddr.Address.Function = Function;
    PciAddr.Address.Register = Offset;
    RegisterValue = xUSLPciRead32(PciAddr.AddressValue);
  } else {
    PciAddr.Address.Function = FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC;
    PciAddr.Address.Register = FABRICINDIRECTCONFIGACCESSADDRESS_3_REG;
    FICAA3.Value = 0;
    FICAA3.Field.CfgRegInstAccEn = 1;
    FICAA3.Field.IndCfgAccRegNum = ((uint32_t) Offset) >> 2;
    FICAA3.Field.IndCfgAccFuncNum = (uint32_t) Function;
    FICAA3.Field.CfgRegInstID = (uint32_t) Instance;
    xUSLPciWrite32(PciAddr.AddressValue, FICAA3.Value);

    PciAddr.Address.Function = FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC;
    PciAddr.Address.Register = FABRICINDIRECTCONFIGACCESSDATALO_3_REG;
    RegisterValue = xUSLPciRead32(PciAddr.AddressValue);
  }

  return RegisterValue;
}

/**
 * DfXFabricResterAccWrite
 *
 * @brief Routine to write a register
 *
 * @param SilContext         A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param Socket             Processor socket to read from
 * @param Die                Processor die to read from
 * @param Function           Function number to read from
 * @param Offset             Register to read
 * @param Instance           Instance ID of the target fabric device
 * @param Value              Value to write
 */
void
DfXFabricRegisterAccWrite (
  SIL_CONTEXT         *SilContext,
  uint32_t            Socket,
  uint32_t            Die,
  uint32_t            Function,
  uint32_t            Offset,
  uint32_t            Instance,
  uint32_t            Value
  )
{
  uint32_t                             RegisterValue;
  PCI_ADDR                             PciAddr;
  FABRIC_INDIRECT_CONFIG_ACCESS_ADDRESS_REGISTER  FICAA3;

  assert(Socket < 2);
  assert(Die < 2);
  assert(Function < 8);
  assert(Offset < 0x2000);
  assert((Offset & 3) == 0);
  assert(Instance <= FABRIC_REG_ACC_BC);

  RegisterValue = Value;
  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = DfFabricRegisterAccGetPciDeviceNumberOfDie(SilContext, Socket, Die);
  if (Instance == FABRIC_REG_ACC_BC) {
    PciAddr.Address.Function = (uint32_t) Function;
    PciAddr.Address.Register = (uint32_t) Offset;
    xUSLPciWrite32(PciAddr.AddressValue, RegisterValue);
  } else {
    PciAddr.Address.Function = FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC;
    PciAddr.Address.Register = FABRICINDIRECTCONFIGACCESSADDRESS_3_REG;
    FICAA3.Value = 0;
    FICAA3.Field.CfgRegInstAccEn = 1;
    FICAA3.Field.IndCfgAccRegNum = ((uint32_t) Offset) >> 2;
    FICAA3.Field.IndCfgAccFuncNum = (uint32_t) Function;
    FICAA3.Field.CfgRegInstID = (uint32_t) Instance;
    xUSLPciWrite32(PciAddr.AddressValue, FICAA3.Value);
    PciAddr.Address.Function = FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC;
    PciAddr.Address.Register = FABRICINDIRECTCONFIGACCESSDATALO_3_REG;
    xUSLPciWrite32(PciAddr.AddressValue, RegisterValue);
  }
}
