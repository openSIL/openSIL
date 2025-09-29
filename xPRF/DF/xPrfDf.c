/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfDf.c
 * @brief Define xPrf Functions
 *        These Function returns Silicon Data of the OpenSIL Modules.
 *        These Silicon Data will send to HOST through service provided by OpenSIL.
 */

#include <SilCommon.h>
#include <xPRF.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfIp2Ip.h>

/**
 * xPrfDfGetNumberOfProcessorsPresent.
 *
 * @brief Get number of sockets on the system.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @return  uint32_t    Number of sockets.
 *
 */
uint32_t
xPrfDfGetNumberOfProcessorsPresent (
  SIL_CONTEXT     *SilContext
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      SocketCount;
  SIL_STATUS                    Status;

  // Set default return value.
  SocketCount = 1;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip));
  if (Status == SilPass) {
    SocketCount = DfIp2Ip->DfGetNumberOfProcessorsPresent(SilContext);
  }

  return SocketCount;
}

/**
 * xPrfDfGetNumberOfRootBridgesOnSocket.
 *
 * @brief Get number of root bridges on the socket.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext  A context structure through which host firmware defined data
 *                    can be passed to openSIL. The host firmware is responsible for initializing
 *                    the SIL_CONTEXT structure.
 * @param Socket      Socket number.
 *
 * @return uint32_t   Number of root bridges.
 *
 */
uint32_t
xPrfDfGetNumberOfRootBridgesOnSocket (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      RootBridgeCount;

  // Set default return value.
  RootBridgeCount = 1;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    RootBridgeCount = DfIp2Ip->DfGetNumberOfRootBridgesOnSocket(SilContext, Socket);
  }

  return RootBridgeCount;
}

/**
 * xPrfDfGetNumberOfRootBridgesOnDie.
 *
 * @brief Get number of root bridges on the die.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext  A context structure through which host firmware defined data
 *                    can be passed to openSIL. The host firmware is responsible for initializing
 *                    the SIL_CONTEXT structure.
 * @param Socket      Socket number.
 * @param Die         Die number
 *
 * @return uint32_t   Number of root bridges.
 *
 */
uint32_t
xPrfDfGetNumberOfRootBridgesOnDie (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      RootBridgeCount;

  // Set default return value.
  RootBridgeCount = 1;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    RootBridgeCount = DfIp2Ip->DfGetNumberOfRootBridgesOnDie(SilContext, Socket, Die);
  }

  return RootBridgeCount;
}

/**
 * xPrfDfGetHostBridgeBusBase.
 *
 * @brief Get bus base number on the root bridge device.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 * @param   Socket        Socket number.
 * @param   Die           Die number
 * @param   Index         Root bridge number.
 *
 * @return uint32_t  Bus base Number.
 *
 */
uint32_t
xPrfDfGetHostBridgeBusBase (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die,
  uint32_t        Index
  )
{
  DF_IP2IP_API                 *DfIp2Ip;
  uint32_t                      HostBridgeBusBase;

  // Set default return value.
  HostBridgeBusBase = 0;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    HostBridgeBusBase = DfIp2Ip->DfGetHostBridgeBusBase(SilContext, Socket, Die, Index);
  }

  return HostBridgeBusBase;
}

/**
 * xPrfDfGetHostBridgeBusLimit.
 *
 * @brief Get bus base number on the root bridge device.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext  A context structure through which host firmware defined data
 *                    can be passed to openSIL. The host firmware is responsible for initializing
 *                    the SIL_CONTEXT structure.
 * @param Socket      Socket number.
 * @param Die         Die number.
 * @param Index       Root bridge number.
 *
 * @return uint32_t   Bus limit number.
 *
 */
uint32_t
xPrfDfGetHostBridgeBusLimit (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die,
  uint32_t        Index
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      HostBridgeBusLimit;

  // Set default return value.
  HostBridgeBusLimit = 0xFF;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    HostBridgeBusLimit = DfIp2Ip->DfGetHostBridgeBusLimit(SilContext, Socket, Die, Index);
  }

  return HostBridgeBusLimit;
}

/**
 * xPrfDfGetSystemInfo.
 *
 * @brief This function retrieves information about the overall system.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext                   A context structure through which host firmware defined data
 *                                     can be passed to openSIL. The host firmware is responsible for initializing
 *                                     the SIL_CONTEXT structure.
 * @param NumberOfInstalledProcessors   On output, the total number of populated
 *                                      processor sockets in the system.
 * @param TotalNumberOfDie              On output, the total number of die in the
 *                                      system.
 * @param TotalNumberOfRootBridges      On output, the total number of root PCI
 *                                      bridges in the system.
 * @param SystemFchRootBridgeLocation   On output, system primary FCH location.
 * @param SystemSmuRootBridgeLocation   On output, system primary SMU location.
 *
 * @retval SilPass                      The system topology information was
 *                                      successfully retrieved.
 * @retval SilInvalidParameter          All output parameter pointers are NULL.
 *
 */
SIL_STATUS
xPrfDfGetSystemInfo (
  SIL_CONTEXT              *SilContext,
  uint32_t                 *NumberOfInstalledProcessors,
  uint32_t                 *TotalNumberOfDie,
  uint32_t                 *TotalNumberOfRootBridges,
  SIL_ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  SIL_ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  SIL_STATUS                    Status;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip));
  if (Status == SilPass) {
    Status = DfIp2Ip->DfGetSystemInfo(SilContext,
      NumberOfInstalledProcessors,
      TotalNumberOfDie,
      TotalNumberOfRootBridges,
      (ROOT_BRIDGE_LOCATION *)SystemFchRootBridgeLocation,
      (ROOT_BRIDGE_LOCATION *)SystemSmuRootBridgeLocation
      );
  }

  return Status;
}

/**
 * xPrfDfFabricRegisterAccRMW.
 *
 * @brief Routine to read-modify-write a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext    A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param Socket        Processor socket to read from.
 * @param Die           Socket die to read from.
 * @param Function      Function number to read from.
 * @param Offset        Register to read.
 * @param Instance      Instance ID of the target fabric device.
 * @param NandValue     Value to NAND with the current register value.
 * @param OrValue       Value to OR with the current register value.
 * @retval Value written to target register if interested.
 *
 */
uint32_t
xPrfDfFabricRegisterAccRMW (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Function,
  uint32_t          Offset,
  uint32_t          Instance,
  uint32_t          NandValue,
  uint32_t          OrValue
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      RegisterValue;

  // Set default return value.
  RegisterValue = 0;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    RegisterValue =
      (DfIp2Ip->DfFabricRegisterAccRead(SilContext, Socket, Die, Function, Offset, Instance) & ~NandValue) | OrValue;
    DfIp2Ip->DfFabricRegisterAccWrite(SilContext, Socket, Die, Function, Offset, Instance, RegisterValue);
  }

  return RegisterValue;
}

/**
 * xPrfDfFabricRegisterAccRead.
 *
 * @brief Routine to read a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext    A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param Socket        Processor socket to read from.
 * @param Die           Socket die to read from.
 * @param Function      Function number to read from.
 * @param Offset        Register to read.
 * @param Instance      Instance ID of the target fabric device.
 * @retval Current value of the target register.
 *
 */
uint32_t
xPrfDfFabricRegisterAccRead (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket,
  uint32_t      Die,
  uint32_t      Function,
  uint32_t      Offset,
  uint32_t      Instance
  )
{
  DF_IP2IP_API                  *DfIp2Ip;
  uint32_t                      RegisterValue;

  // Set default return value.
  RegisterValue = 0;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    RegisterValue = DfIp2Ip->DfFabricRegisterAccRead(SilContext, Socket, Die, Function, Offset, Instance);
  }

  return RegisterValue;
}

/**
 * xPrfDfFabricRegisterAccWrite.
 *
 * @brief Routine to write a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext    A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param Socket        Processor socket to read from.
 * @param Die           Socket die to read from.
 * @param Function      Function number to read from.
 * @param Offset        Register to read.
 * @param Instance      Instance ID of the target fabric device.
 * @param Value         Value to write.
 *
 */
void
xPrfDfFabricRegisterAccWrite (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket,
  uint32_t      Die,
  uint32_t      Function,
  uint32_t      Offset,
  uint32_t      Instance,
  uint32_t      Value
  )
{
  DF_IP2IP_API                  *DfIp2Ip;

  // SilGetIp2IpApi runs assert error in case the target API is not available.
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2Ip)) == SilPass) {
    DfIp2Ip->DfFabricRegisterAccWrite(SilContext, Socket, Die, Function, Offset, Instance, Value);
  }
}
