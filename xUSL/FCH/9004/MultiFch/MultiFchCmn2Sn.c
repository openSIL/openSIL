/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchCmn2Sn.c
 * @brief OpenSIL Multi FCH transfer block functions
 *
 */

#include <FCH/Common/Fch.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include <FCH/9004/MultiFch/MultiFchCmn2Sn.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <DF/Common/FabricRegisterAccCmn.h> // for FABRIC_REG_ACC_BC

MULTI_FCH_COMMON_2_REV_XFER_BLOCK mMultiFchCmn2Sn = {
  .EnableIoCxxOnDieN            = EnableIoCxxOnDieNSn,
  .DisableIoCxxOnDieN           = DisableIoCxxOnDieNSn,
  .GetSecFchRootBridgeNumber    = GetSecRootBridgeNumberSn,
  .GetSecFchBusNumber           = GetSecFchBusNumberSn
};

/*
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
)
{
  SIL_STATUS    Status;
  DF_IP2IP_API  *DfApi;
  uint32_t      BusNumberBase;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status == SilPass) {
    DfApi->DfGetRootBridgeInfo (
      SN_SECOND_SOCKET,
      SN_SECOND_SOCKET_DIE,
      SN_SECOND_SOCKET_INDEX,
      NULL,
      &BusNumberBase,
      NULL,
      NULL,
      NULL,
      NULL
      );
    *FchBusNum = (uint8_t)(BusNumberBase & 0xFF);
  } else {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert (Status == SilPass);
  }
  return Status;
}

/*
 * EnableIoCxxOnDieNSn
 *
 * @brief   Enable IO 0xCxx on Die N
 *
 * @param   IoBaseRegValOrg   Buffer to save the original IO base value.
 * @param   IoLimitRegValOrg  Buffer to save the original IO Limit value.
 *
 * @return  SIL_STATUS
 * @retval  SilNotfound if IP-2-IP apis are not found.
 * @retval  SilPass is successfull
 */
SIL_STATUS
EnableIoCxxOnDieNSn (
  uint32_t          *IoBaseRegValOrg,
  uint32_t          *IoLimitRegValOrg
)
{
  SIL_STATUS        Status;
  DF_IP2IP_API      *DfIp2IpApi;
  uint32_t          IOBaseRegVal_New;
  uint32_t          IOLimitRegVal_New;

  Status = SilGetIp2IpApi (SilId_DfClass, (void *)&DfIp2IpApi);
  assert (Status == SilPass);
  if (Status == SilPass) {
    //enable IO 0xCxx on the dieN
    *IoBaseRegValOrg = DfIp2IpApi->DfFabricRegisterAccRead (
                          0,                                       //Socket0
                          X86IOBASEADDRESS_0_FUNC,                 //0x00
                          X86IOBASEADDRESS_0_REG,                  //0xD00
                          FABRIC_REG_ACC_BC                        //BC
                          );
    IOBaseRegVal_New = 0x00000003;
    DfIp2IpApi->DfFabricRegisterAccWrite (
                          0,                                       //Socket0
                          X86IOBASEADDRESS_0_FUNC,                 //0x00
                          X86IOBASEADDRESS_0_REG,                  //0xD00
                          FABRIC_REG_ACC_BC,                       //BC
                          IOBaseRegVal_New
                          );

    *IoLimitRegValOrg = DfIp2IpApi->DfFabricRegisterAccRead (
                          0,                                       //Socket0
                          X86IOLIMITADDRESS_0_FUNC,                //0x00
                          X86IOLIMITADDRESS_0_REG,                 //0xD04
                          FABRIC_REG_ACC_BC                        //BC
                          );
    IOLimitRegVal_New = IOS2_FABRIC_ID + (1 << FABRIC_ID_SOCKET_SHIFT);  //Node1 + IOS2
    DfIp2IpApi->DfFabricRegisterAccWrite (
                          0,                                       //Socket0
                          X86IOLIMITADDRESS_0_FUNC,                //0x00
                          X86IOLIMITADDRESS_0_REG,                 //0xD04
                          FABRIC_REG_ACC_BC,                       //BC
                          IOLimitRegVal_New
                          );
  }
  return Status;
}

/*
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
)
{
  SIL_STATUS        Status;
  DF_IP2IP_API      *DfIp2IpApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void *)&DfIp2IpApi);
  assert (Status == SilPass);
  if (Status == SilPass) {
    // Disable IO 0xCxx on DieN
    DfIp2IpApi->DfFabricRegisterAccWrite (
      0,                                       //Socket0
      X86IOBASEADDRESS_0_FUNC,                 //0x00
      X86IOBASEADDRESS_0_REG,                  //0xC0
      FABRIC_REG_ACC_BC,                       //BC
      IoBaseRegValOrg
    );

    DfIp2IpApi->DfFabricRegisterAccWrite (
      0,                                      //Socket0
      X86IOLIMITADDRESS_0_FUNC,               //0x00
      X86IOLIMITADDRESS_0_REG,                //0xC4
      FABRIC_REG_ACC_BC,                      //BC
      IoLimitRegValOrg
      );
  }
  return Status;
}

/*
 * GetSecRootBridgeNumberSn
 *
 * @brief Get the Root Bridge Number of the FCH.
 *
 * @param   RootBridgeNumber - The root bridge number for the secondary FCH.
 *
 * @return  uint8_t  The root bridge of the secondary FCH.
 */
SIL_STATUS
GetSecRootBridgeNumberSn (
  uint8_t   *RootBridgeNumber
)
{
  SIL_STATUS        Status;
  DF_IP2IP_API      *DfIp2IpApi;
  uint32_t          PhysicalRbNum;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  Status = DfIp2IpApi->DfGetRootBridgeInfo (
    SN_SECOND_SOCKET,
    SN_SECOND_SOCKET_DIE,
    SN_SECOND_SOCKET_INDEX,
    NULL,
    NULL,
    NULL,
    &PhysicalRbNum,
    NULL,
    NULL
    );
  if (Status == SilPass) {
    *RootBridgeNumber = (uint8_t)(PhysicalRbNum & 0xFF);
  }

  return Status;
}
