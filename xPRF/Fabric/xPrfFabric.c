/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFabric.c
 * @brief Platform Reference Firmware - fabric topology services.
 *
 * @details Exposes the four DF Ip-2-Ip topology queries as host-callable xPRF
 *          services.
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <xPRF-api.h>
#include <DF/DfIp2Ip.h>
#include <xPrfFabric.h>

/**
 * xPrfFabricGetSystemInfo
 *
 * @brief   Retrieve system-wide data fabric information.
 *
 * @param   NumberOfInstalledProcessors Populated with the processor count
 * @param   TotalNumberOfDie            Populated with the system die count
 * @param   TotalNumberOfRootBridges    Populated with the root bridge count
 * @param   SystemFchRootBridgeLocation Populated with the FCH root bridge
 * @param   SystemSmuRootBridgeLocation Populated with the SMU root bridge
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricGetSystemInfo (
  uint32_t                   *NumberOfInstalledProcessors,
  uint32_t                   *TotalNumberOfDie,
  uint32_t                   *TotalNumberOfRootBridges,
  XPRF_ROOT_BRIDGE_LOCATION   *SystemFchRootBridgeLocation,
  XPRF_ROOT_BRIDGE_LOCATION   *SystemSmuRootBridgeLocation
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  return DfApi->DfGetSystemInfo (
                  NumberOfInstalledProcessors,
                  TotalNumberOfDie,
                  TotalNumberOfRootBridges,
                  SystemFchRootBridgeLocation,
                  SystemSmuRootBridgeLocation
                  );
}

/**
 * xPrfFabricGetProcessorInfo
 *
 * @brief   Retrieve fabric information about one socket.
 *
 * @param   Socket                Socket to query
 * @param   DieCount              Populated with the die count on that socket
 * @param   RootBridgeCount       Populated with the root bridge count
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricGetProcessorInfo (
  uint32_t  Socket,
  uint32_t  *DieCount,
  uint32_t  *RootBridgeCount
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  return DfApi->DfGetProcessorInfo (Socket, DieCount, RootBridgeCount);
}

/**
 * xPrfFabricGetDieInfo
 *
 * @brief   Retrieve fabric information about one die.
 *
 * @param   Socket                Socket the die belongs to
 * @param   Die                   Die to query
 * @param   RootBridgeCount       Populated with the root bridge count
 * @param   SystemIdOffset        Populated with the die's system ID offset
 * @param   FabricIdMap           Populated with the fabric ID map
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricGetDieInfo (
  uint32_t                       Socket,
  uint32_t                       Die,
  uint32_t                       *RootBridgeCount,
  uint32_t                       *SystemIdOffset,
  const XPRF_FABRIC_DEVICE_MAP  **FabricIdMap
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  return DfApi->DfGetDieInfo (Socket, Die, RootBridgeCount, SystemIdOffset, FabricIdMap);
}

/**
 * xPrfFabricGetRootBridgeInfo
 *
 * @brief   Retrieve fabric information about one root PCI bridge.
 *
 * @param   Socket                    Socket the bridge belongs to
 * @param   Die                       Die the bridge belongs to
 * @param   Index                     Bridge index on that die
 * @param   SystemFabricID            Populated with the fabric ID
 * @param   BusNumberBase             Populated with the base bus number
 * @param   BusNumberLimit            Populated with the bus number limit
 * @param   PhysicalRootBridgeNumber  Populated with the physical bridge number
 * @param   HasFchDevice              Populated true when the FCH is here
 * @param   HasSystemMgmtUnit         Populated true when the SMU is here
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricGetRootBridgeInfo (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Index,
  uint32_t  *SystemFabricID,
  uint32_t  *BusNumberBase,
  uint32_t  *BusNumberLimit,
  uint32_t  *PhysicalRootBridgeNumber,
  bool      *HasFchDevice,
  bool      *HasSystemMgmtUnit
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  return DfApi->DfGetRootBridgeInfo (
                  Socket,
                  Die,
                  Index,
                  SystemFabricID,
                  BusNumberBase,
                  BusNumberLimit,
                  PhysicalRootBridgeNumber,
                  HasFchDevice,
                  HasSystemMgmtUnit
                  );
}
