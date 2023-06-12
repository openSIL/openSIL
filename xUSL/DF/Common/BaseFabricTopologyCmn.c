/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BaseFabricTopologyCmn.c
 * @brief Common implementations of BaseFabricTopology functions
 */

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include "BaseFabricTopologyCmn.h"
#include "DfCmn2Rev.h"

/**
 * DfGetNumberOfPciSegments
 *
 * @brief  Get the number of PCI segments in the system.
 *
 * @return uint8_t  The number of PCI segments
 */
uint32_t
DfGetNumberOfPciSegments (void)
{
  uint32_t                     Socket;
  uint32_t                     SocketCount;
  uint32_t                     Die;
  uint32_t                     DieCount;
  uint32_t                     RB;
  uint32_t                     RBCount;
  uint32_t                     CurrentPciLimit;
  uint32_t                     PciLimit;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                   Status;


  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  CurrentPciLimit = 0;

  SocketCount = DfXfer->DfGetNumberOfProcessorsPresent ();
  for (Socket = 0; Socket < SocketCount; Socket++) {
    DieCount = DfGetNumberOfDiesOnSocket(Socket);
    for (Die = 0; Die < DieCount; Die++) {
      RBCount = DfXfer->DfGetNumberOfRootBridgesOnDie(Socket, Die);
      for (RB = 0; RB < RBCount; RB++) {
        PciLimit = DfXfer->DfGetHostBridgeBusLimit(Socket, Die, RB);
        if (PciLimit > CurrentPciLimit) {
          CurrentPciLimit = PciLimit;
        }
      }
    }
  }

  CurrentPciLimit += 1;
  CurrentPciLimit = (CurrentPciLimit / MAX_PCI_BUS_NUMBER_PER_SEGMENT) +
    (((CurrentPciLimit % MAX_PCI_BUS_NUMBER_PER_SEGMENT) == 0) ? 0 : 1);

  return CurrentPciLimit;
}

/**
 * DfGetNumberOfDiesOnSocket
 *
 * @brief  Get the number of dies on the given socket
 *
 * @param  Socket   The given socket
 *
 * @return uint32_t The number of dies
 *
 */
uint32_t
DfGetNumberOfDiesOnSocket (
  uint32_t Socket
  )
{
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);
  return (DfXfer->DfGetNumberOfSystemDies () / DfXfer->DfGetNumberOfProcessorsPresent ());
}

/**
 * DfGetHostBridgeSystemFabricID
 *
 * @brief The return the Fabric ID of the host bridge on the given die
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Index  The index of the entry in the fabric ID table
 *
 * @return uint32_t The host bridge fabric ID
 *
 */
uint32_t
DfGetHostBridgeSystemFabricID (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  IosEntry = DfXfer->DfFindDeviceTypeEntryInMap(Socket, Die, Ios);
  assert(IosEntry != NULL);

  assert(Index < IosEntry->Count);
  return (DfXfer->DfGetDieSystemOffset(Socket, Die) + IosEntry->IDs[Index].FabricID);
}

/**
 * DfGetRootBridgeInfo
 *
 * @brief This function retrieves information about the given root PCI bridge.
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param Die                       The target root bridge's die identifier
 *                                  within Socket.
 * @param Index                     The target root bridge's 0-based index on
 *                                  die
 * @param SystemFabricID            Pointer to the root bridge's fabric
 *                                  identifier within the system.
 * @param BusNumberBase             Pointer to the root bridge's base PCI bus
 *                                  number in the system.
 * @param BusNumberLimit            Pointer to the root bridge's maximum
 *                                  decoded PCI bus number in the system.
 * @param  PhysicalRootBridgeNumber Physical RootBridge number of RootBridge
 *                                  specified by Socket/Die/Index.
 * @param  HasFchDevice             If this RootBridge has FCH.
 * @param  HasSystemMgmtUnit        If this RootBridge has SMU.
 *
 * @retval SilPass              The Die information was successfully retrieved.
 * @retval SilInvalidParameter  All output parameter pointers are NULL or
 *                              Socket/Die/FabricId is non-existent.
 *
 **/
SIL_STATUS
DfGetRootBridgeInfo (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index,
  uint32_t *SystemFabricID,
  uint32_t *BusNumberBase,
  uint32_t *BusNumberLimit,
  uint32_t *PhysicalRootBridgeNumber,
  bool *HasFchDevice,
  bool *HasSystemMgmtUnit
  )
{
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unnecessary.  This
   * check is only valid for debug builds.
   */
  assert((SystemFabricID != NULL) || (BusNumberBase != NULL) ||
    (BusNumberLimit != NULL) || (PhysicalRootBridgeNumber != NULL) ||
    (HasFchDevice != NULL) || (HasSystemMgmtUnit != NULL)
    );

  if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent ()) {
    return SilInvalidParameter;
  }

  if (Die >= DfGetNumberOfDiesOnSocket(Socket)) {
    return SilInvalidParameter;
  }

  if (Index >= DfXfer->DfGetNumberOfRootBridgesOnDie(Socket, Die)) {
    return SilInvalidParameter;
  }

  if (SystemFabricID != NULL) {
    *SystemFabricID = DfGetHostBridgeSystemFabricID(Socket, Die, Index);
  }

  if (BusNumberBase != NULL) {
    *BusNumberBase = DfXfer->DfGetHostBridgeBusBase(Socket, Die, Index);
  }

  if (BusNumberLimit != NULL) {
    *BusNumberLimit = DfXfer->DfGetHostBridgeBusLimit(Socket, Die, Index);
  }

  if (PhysicalRootBridgeNumber != NULL) {
    *PhysicalRootBridgeNumber = DfXfer->DfGetPhysRootBridgeNumber(Socket, Die, Index);
  }

  if (HasFchDevice != NULL) {
    *HasFchDevice = DfXfer->DfHasFch(Socket, Die, Index);
  }

  if (HasSystemMgmtUnit != NULL) {
    *HasSystemMgmtUnit = DfXfer->DfHasSmu(Socket, Die, Index);
  }

  return SilPass;
}

/**
 * DfGetDieInfo
 *
 * @brief This function retrieves information about the given die.
 *
 * @param Socket               The target die's socket identifier.
 * @param Die                  The target die's identifier within Socket.
 * @param NumberOfRootBridges  Pointer to the number of root PCI bridges
 *                             present on the given die.
 * @param SystemIdOffset       Pointer to the die's offset for all of its
 *                             devices.
 * @param FabricIdMap          Pointer to an array describing the devices
 *                             present within the given die's fabric.
 *
 * @retval SilPass              The Die information was successfully
 *                              retrieved.
 * @retval SilInvalidParameter  All output parameter pointers are NULL or
 *                              Socket/Die is non-existent.
 *
 **/
SIL_STATUS
DfGetDieInfo (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfRootBridges,
  uint32_t *SystemIdOffset,
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP **FabricIdMap
  )
{
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unnecessary.  This
   * check is only valid for debug builds.
   */
  assert((NumberOfRootBridges != NULL) || (SystemIdOffset != NULL) ||
    (FabricIdMap != NULL)
    );

  if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent ()) {
    return SilInvalidParameter;
  }

  if (Die >= DfGetNumberOfDiesOnSocket(Socket)) {
    return SilInvalidParameter;
  }

  if (NumberOfRootBridges != NULL) {
    *NumberOfRootBridges = DfXfer->DfGetNumberOfRootBridgesOnDie(Socket, Die);
  }

  if (SystemIdOffset != NULL) {
    *SystemIdOffset = DfXfer->DfGetDieSystemOffset(Socket, Die);
  }

  if (FabricIdMap != NULL) {
    *FabricIdMap = DfXfer->DfGetDeviceMapOnDie(Socket, Die);
  }


  return SilPass;
}

/**
 * DfGetProcessorInfo
 *
 * @brief This function retrieves information about the processor installed in
 *        the given socket.
 *
 * @param Socket              Zero-based socket number to check.
 * @param NumberOfDie         Pointer to the number of die present on the
 *                            given processor.
 * @param NumberOfRootBridges Pointer to the number of root PCI bridges on
 *                            the given processor.
 *
 * @retval SilPass              The socket information was successfully
 *                              retrieved.
 * @retval SilInvalidParameter  All output parameter pointers are NULL or
 *                              Socket is non-existent.
 *
 **/
SIL_STATUS
DfGetProcessorInfo (
  uint32_t SocketIndex,
  uint32_t *DieCount,
  uint32_t *RootBridgeCount
  )
{
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }


  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unnecessary.  This
   * check is only valid for debug builds.
   */
  assert((DieCount != NULL) || (RootBridgeCount != NULL));

  if (SocketIndex >= DfXfer->DfGetNumberOfProcessorsPresent ()) {
    return SilInvalidParameter;
  }

  if (DieCount != NULL) {
    *DieCount = (uint32_t) DfGetNumberOfDiesOnSocket(SocketIndex);
  }

  if (RootBridgeCount != NULL) {
    *RootBridgeCount = (uint32_t) DfXfer->DfGetNumberOfRootBridgesOnSocket(SocketIndex);
  }

  return SilPass;
}

/**
 * DfGetSystemInfo
 *
 * @brief This function retrieves information about the overall system.
 *
 * @param NumberOfInstalledProcessors   Pointer to the total number of populated
 *                                      processor sockets in the system.
 * @param TotalNumberOfDie              Pointer to the total number of die in the
 *                                      system.
 * @param TotalNumberOfRootBridges      Pointer to the total number of root PCI
 *                                      bridges in the system.
 * @param SystemFchRootBridgeLocation   System primary FCH location.
 * @param SystemSmuRootBridgeLocation   System primary SMU location.
 *
 * @retval SilPass                      The system topology information was
 *                                      successfully retrieved.
 * @retval SilInvalidParameter          All output parameter pointers are NULL.
 *
 **/
SIL_STATUS
DfGetSystemInfo (
  uint32_t *NumberOfInstalledProcessors,
  uint32_t *TotalNumberOfDie,
  uint32_t *TotalNumberOfRootBridges,
  ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  )
{
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                  Status;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unnecessary.  This
   * check is only valid for debug builds.
   */
  assert((NumberOfInstalledProcessors != NULL) || (TotalNumberOfDie != NULL) ||
    (TotalNumberOfRootBridges != NULL) ||
    (SystemFchRootBridgeLocation != NULL) ||
    (SystemSmuRootBridgeLocation != NULL)
    );

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  if (NumberOfInstalledProcessors != NULL) {
    *NumberOfInstalledProcessors = DfXfer->DfGetNumberOfProcessorsPresent ();
  }

  if (TotalNumberOfDie != NULL) {
    *TotalNumberOfDie = DfXfer->DfGetNumberOfSystemDies ();
  }

  if (TotalNumberOfRootBridges != NULL) {
    *TotalNumberOfRootBridges = DfXfer->DfGetNumberOfSystemRootBridges ();
  }

  if (SystemFchRootBridgeLocation != NULL) {
    DfXfer->DfGetSystemComponentRootBridgeLocation(PrimaryFch,
      SystemFchRootBridgeLocation
      );
  }

  if (SystemSmuRootBridgeLocation != NULL) {
    DfXfer->DfGetSystemComponentRootBridgeLocation(PrimarySmu,
      SystemSmuRootBridgeLocation
      );
  }

  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
