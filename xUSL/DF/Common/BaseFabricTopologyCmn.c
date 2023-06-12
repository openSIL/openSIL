/**
 * @file  BaseFabricTopologyCmn.c
 * @brief Common implementations of BaseFabricTopology functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include "BaseFabricTopologyCmn.h"
#include "DfCmn2Rev.h"

/**
  *	DfGetNumberOfDiesOnSocket
  *
  *	@brief TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t DfGetNumberOfDiesOnSocket (void)
{
  return 1;
}

/**
  *	DfGetHostBridgeSystemFabricID
  *
  *	@brief TBD
  *
  * @param Socket TBD
  * @param Index TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t
DfGetHostBridgeSystemFabricID (
  uint32_t Socket,
  uint32_t Index
  )
{
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  IosEntry = DfFindDeviceTypeEntryInMap (Ios);
  assert (IosEntry != NULL);

  assert (Index < IosEntry->Count);
  return (DfXfer->DfGetDieSystemOffset (Socket) + IosEntry->IDs[Index].FabricID);
}

/**
  *	DfGetSystemComponentRootBridgeLocation
  *
  *	@brief TBD
  *
  * @param Component TBD
  * @param Location TBD
  *
  *	@return bool TBD
  *
  */
bool
DfGetSystemComponentRootBridgeLocation (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION *Location
  )
{
  uint32_t  i;
  uint32_t  j;
  uint32_t  Count;
  bool    Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  assert ((Component == PrimaryFch) || (Component == PrimarySmu));

  ComponentLocation = DfXfer->DfFindComponentLocationMap (&Count, NULL);

  Found = false;
  for (i = 0; i < Count; i++) {
    if (ComponentLocation[i].Type == Component) {
      Location->Socket = ComponentLocation[i].Socket;
      Location->Die = ComponentLocation[i].Die;

      IosEntry = DfFindDeviceTypeEntryInMap (Ios);
      assert (IosEntry != NULL);
      for (j = 0; j < (IosEntry->Count); j++) {
        if (IosEntry->IDs[j].FabricID == ComponentLocation[i].IomsFabricId) {
          Location->Index = (uint32_t) j;
          break;
        }
      }
      assert (j < IosEntry->Count);
      Found = true;
      break;
    }
  }
  return Found;
}

/**
  *	DfHasFch
  *
  *	@brief TBD
  *
  * @param Socket TBD
  * @param Die TBD
  * @param Index TBD
  *
  *	@return bool TBD
  *
  */
bool
DfHasFch (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t   i;
  uint32_t   Count;
  uint32_t   IosFabricId;
  uint32_t   Ios0FabricId;
  bool Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  IosEntry = DfFindDeviceTypeEntryInMap (Ios);
  assert (IosEntry != NULL);
  IosFabricId = IosEntry->IDs[Index].FabricID;

  ComponentLocation = DfXfer->DfFindComponentLocationMap (&Count, &Ios0FabricId);

  Found = false;
  for (i = 0; i < Count; i++) {
    if ((ComponentLocation[i].Type < FchTypeMax) &&
        (Socket == ComponentLocation[i].Socket) &&
        (Die == ComponentLocation[i].Die) &&
        (IosFabricId == ComponentLocation[i].IomsFabricId)) {
      Found = true;
      break;
    }
  }
  return Found;
}

/**
  *	DfHasSmu
  *
  *	@brief TBD
  *
  * @param Socket TBD
  * @param Die TBD
  * @param Index TBD
  *
  *	@return bool TBD
  *
  */
bool
DfHasSmu (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t   i;
  uint32_t   Count;
  uint32_t   IosFabricId;
  uint32_t   Ios0FabricId;
  bool Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  IosEntry = DfFindDeviceTypeEntryInMap (Ios);
  assert (IosEntry != NULL);
  IosFabricId = IosEntry->IDs[Index].FabricID;

  ComponentLocation = DfXfer->DfFindComponentLocationMap (&Count, &Ios0FabricId);

  Found = false;

  for (i = 0; i < Count; i++) {
    if ((ComponentLocation[i].Type < SmuTypeMax) &&
        (ComponentLocation[i].Type > FchTypeMax) &&
        (Socket == ComponentLocation[i].Socket) &&
        (Die == ComponentLocation[i].Die) &&
        (IosFabricId == ComponentLocation[i].IomsFabricId)) {
      Found = true;
      break;
    }
  }

  return Found;
}

/**
  *	DfFindDeviceTypeEntryInMap
  *
  *	@brief TBD
  *
  * @param Type TBD
  *
  *	@return const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* TBD
  *
  */
const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
DfFindDeviceTypeEntryInMap (
  FABRIC_DEVICE_TYPE  Type
  )
{
  uint32_t  i;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *DeviceMap;
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  DeviceMap = DfXfer->DfGetDeviceMapOnDie ();
  for (i = 0; DeviceMap[i].Type < FabricDeviceTypeMax; i++) {
    if (DeviceMap[i].Type == Type) {
      break;
    }
  }

  if (DeviceMap[i].Type == Type) {
    return &DeviceMap[i];
  } else {
    return NULL;
  }
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
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unecessary.  This
   * check is only valid for debug builds.
   */
  assert ((SystemFabricID != NULL) || (BusNumberBase != NULL) ||
    (BusNumberLimit != NULL) || (PhysicalRootBridgeNumber != NULL) ||
    (HasFchDevice != NULL) || (HasSystemMgmtUnit != NULL));

  if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent()) {
    return SilInvalidParameter;
  }

  if (Die >= DfGetNumberOfDiesOnSocket ()) {
    return SilInvalidParameter;
  }

    if (Index >= (uint32_t) DfXfer->DfGetNumberOfRootBridgesOnDie ((uint32_t)Socket)) {
      return SilInvalidParameter;
    }

  if (SystemFabricID != NULL) {
    *SystemFabricID = DfGetHostBridgeSystemFabricID (Socket, Index);
  }

  if (BusNumberBase != NULL) {
    *BusNumberBase = DfXfer->DfGetHostBridgeBusBase ((uint32_t)Socket, Index);
  }

  if (BusNumberLimit != NULL) {
    *BusNumberLimit = DfXfer->DfGetHostBridgeBusLimit ((uint32_t)Socket, Index);
  }

  if (PhysicalRootBridgeNumber != NULL) {
    *PhysicalRootBridgeNumber = DfXfer->DfGetPhysRootBridgeNumber (Index);
  }

  if (HasFchDevice != NULL) {
    *HasFchDevice = DfHasFch (Socket, Die, Index);
  }

  if (HasSystemMgmtUnit != NULL) {
    *HasSystemMgmtUnit = DfHasSmu (Socket, Die, Index);
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
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unecessary.  This
   * check is only valid for debug builds.
   */
  assert ((NumberOfRootBridges != NULL) || (SystemIdOffset != NULL) ||
   (FabricIdMap != NULL));

  if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent()) {
    return SilInvalidParameter;
  }

  if (Die >= DfGetNumberOfDiesOnSocket ()) {
    return SilInvalidParameter;
  }

  if (NumberOfRootBridges != NULL) {
    *NumberOfRootBridges = DfXfer->DfGetNumberOfRootBridgesOnDie ((uint32_t)Socket);
  }

  if (SystemIdOffset != NULL) {
    *SystemIdOffset = (uint32_t) DfXfer->DfGetDieSystemOffset ((uint32_t)Socket);
  }

  if (FabricIdMap != NULL) {
    *FabricIdMap = DfXfer->DfGetDeviceMapOnDie ();
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
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unecessary.  This
   * check is only valid for debug builds.
   */
  assert ((DieCount != NULL) || (RootBridgeCount != NULL));

  if (SocketIndex >= DfXfer->DfGetNumberOfProcessorsPresent ()) {
    return SilInvalidParameter;
  }

  if (DieCount != NULL) {
    *DieCount = (uint32_t) DfGetNumberOfDiesOnSocket();
  }

  if (RootBridgeCount != NULL) {
    *RootBridgeCount = (uint32_t) DfXfer->DfGetNumberOfRootBridgesOnSocket (SocketIndex);
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
  DF_COMMON_2_REV_XFER_BLOCK* DfXfer;
  SIL_STATUS                  Status;

  DF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  /*
   * If any pointer input is != NULL, do not assert.  We should only assert if
   * all inputs are NULL since this function call would be unecessary.  This
   * check is only valid for debug builds.
   */
  assert ((NumberOfInstalledProcessors != NULL) || (TotalNumberOfDie != NULL) ||
      (TotalNumberOfRootBridges != NULL) ||
      (SystemFchRootBridgeLocation != NULL) ||
      (SystemSmuRootBridgeLocation != NULL));

  if (NumberOfInstalledProcessors != NULL) {
    *NumberOfInstalledProcessors = (uint32_t) DfXfer->DfGetNumberOfProcessorsPresent();
  }

  if (TotalNumberOfDie != NULL) {
    *TotalNumberOfDie = (uint32_t) DfXfer->DfGetNumberOfSystemDies ();
  }

  if (TotalNumberOfRootBridges != NULL) {
    *TotalNumberOfRootBridges = (uint32_t) DfXfer->DfGetNumberOfSystemRootBridges ();
  }

  if (SystemFchRootBridgeLocation != NULL) {
    DfGetSystemComponentRootBridgeLocation (
      PrimaryFch,
      SystemFchRootBridgeLocation
      );
  }

  if (SystemSmuRootBridgeLocation != NULL) {
    DfGetSystemComponentRootBridgeLocation (
      PrimarySmu,
      SystemSmuRootBridgeLocation
      );
  }

  DF_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
