/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BrhBaseFabricTopology.c
 * @brief Brh specific implementations of BaseFabricTopology functions
 */

#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <ProjSocConst.h>
#include "DfInitBrh.h"
#include "SilFabricInfoBrh.h"
#include "SilFabricRegistersBrh.h"
#include <string.h>

COMPONENT_LOCATION BrhComponentLocation[] = {
  {0, 0, SIL_RESERVED_0203, PrimaryFch},
  {1, 0, SIL_RESERVED_0203, SlaveFch},
  {0, 0, SIL_RESERVED_0203, PrimarySmu},
  {1, 0, SIL_RESERVED_0203, SlaveSmu},
};

/// Structure for program specific DF instance information
typedef struct {
  FABRIC_DEVICE_TYPE field0;           ///< The instance type of the fabric device
  uint32_t             field1;     ///< The instance ID of the first fabric device of
                                           ///< the devices with the same instance type
  uint32_t             field2;      ///< The count of the fabric devices with the same instance type
  bool                 field3;        ///< This type has fabric ID or not.
} SIL_RESERVED_STRUCT_0001;

SIL_RESERVED_STRUCT_0001 SilBrhDeviceBaseInfo[] = {
  {Cs, SIL_RESERVED_0144, SIL_RESERVED_0211, true  },
  {CsUmc, SIL_RESERVED_0164, SIL_RESERVED_0213, true  },
  {CsCmp, SIL_RESERVED_0160, SIL_RESERVED_0212, true  },
  {Ccm, SIL_RESERVED_0132, SIL_RESERVED_0209, true  },
  {Acm, SIL_RESERVED_0121, SIL_RESERVED_0204, true  },
  {Ncm, SIL_RESERVED_0199, SIL_RESERVED_0217, true  },
  {Iom, SIL_RESERVED_0183, SIL_RESERVED_0215, true  },
  {Ios, SIL_RESERVED_0191, SIL_RESERVED_0216, true  },
  {Icng, SIL_RESERVED_0179, SIL_RESERVED_0214, true  },
  {Pie, SIL_RESERVED_0230, SIL_RESERVED_0219, true  },
  {Cake, SIL_RESERVED_0126, SIL_RESERVED_0208, false },
  {Cnli, SIL_RESERVED_0140, SIL_RESERVED_0210, false },
  {Pfx, SIL_RESERVED_0222, SIL_RESERVED_0218, false },
  {Spf, SIL_RESERVED_0231, SIL_RESERVED_0220, false },
  {Tcdx, SIL_RESERVED_0247, SIL_RESERVED_0221, false },
  {FabricDeviceTypeMax, 0, 0, false },
};

/// Buffer for base fabric topology information
static SIL_RESERVED_STRUCT_0010 gBaseFabricTopologyInfo = {
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
  SIL_MAX_UINT32,
};

/// Buffer for the device map array
static AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP gBrhDeviceMap[PROJ_MAX_IO_DIES_PER_SOCKET][FabricDeviceTypeMax + 1];
/// Buffer for device IDs array
static DEVICE_IDS gBrhDeviceIds[PROJ_MAX_IO_DIES_PER_SOCKET][MAX_NUMBER_OF_DEVICE_IDS];

static
uint32_t
SilBitFieldRead32 (
  uint32_t  Operand,
  uint32_t  StartBit,
  uint32_t  EndBit
  )
{
  assert(EndBit < 32);
  assert(StartBit <= EndBit);
  return (uint32_t)(Operand & ~((uint32_t)-2 << EndBit)) >> StartBit;
}

/**
 * SilCopyTopologyMap
 *
 * @brief Prepare/locate fabric topology map, and update to gBaseFabricTopologyInfo, gBrhDeviceMap, and gDeviceIds
 *
 * @details This function must be called to initialize the global data for each instance of openSIL.
 *
 * @param  FabricTopologyMap TBD
 *
 * @return SIL_STATUS
 */
static
SIL_STATUS
SilCopyTopologyMap (
  SIL_RESERVED_STRUCT_0009 *FabricTopologyMap
  )
{
  uint32_t       Die;
  uint32_t       TypeIndex;
  uint32_t       DeviceIndex;
  DEVICE_MAP     *BaseDeviceMap;
  DEVICE_IDS     *BaseDeviceIds;
  SIL_STATUS     Status;

  if (FabricTopologyMap == NULL) {
    assert(FabricTopologyMap != NULL);
    Status = SilInvalidParameter;
  } else {
    memcpy(&gBaseFabricTopologyInfo, &FabricTopologyMap->field0, sizeof (gBaseFabricTopologyInfo));
    for (Die = 0 ; Die < (DfXGetNumberOfSystemDies () / BrhGetNumberOfProcessorsPresent ()) ; Die++ ) {
      BaseDeviceMap = FabricTopologyMap->Map[Die];
      BaseDeviceIds = FabricTopologyMap->DeviceIds[Die];
      for (TypeIndex = 0; BaseDeviceMap[TypeIndex].Type < FabricDeviceTypeMax; TypeIndex++) {
        gBrhDeviceMap[Die][TypeIndex].Type = BaseDeviceMap[TypeIndex].Type;
        gBrhDeviceMap[Die][TypeIndex].Count = BaseDeviceMap[TypeIndex].Count;
        gBrhDeviceMap[Die][TypeIndex].IDs = &gBrhDeviceIds[Die][BaseDeviceMap[TypeIndex].IDsIndex];
        for (DeviceIndex = 0; DeviceIndex < BaseDeviceMap[TypeIndex].Count; DeviceIndex++) {
          gBrhDeviceIds[Die][BaseDeviceMap[TypeIndex].IDsIndex + DeviceIndex].FabricID =
            BaseDeviceIds[BaseDeviceMap[TypeIndex].IDsIndex + DeviceIndex].FabricID;
          gBrhDeviceIds[Die][BaseDeviceMap[TypeIndex].IDsIndex + DeviceIndex].InstanceID =
            BaseDeviceIds[BaseDeviceMap[TypeIndex].IDsIndex + DeviceIndex].InstanceID;
        }
      }

      gBrhDeviceMap[Die][TypeIndex].Type = FabricDeviceTypeMax;
      gBrhDeviceMap[Die][TypeIndex].Count = 0;
      gBrhDeviceMap[Die][TypeIndex].IDs = NULL;
      Status = SilPass;
    }
  }
  return Status;
}

/**
 * SilPrepareTopologyMap
 *
 * @brief   Prepare the topology map buffer to be stored to DF Topology Block
 *
 * @param   FabricTopologyMap Pointer to the fabric topology map that needs to be built.
 *
 * @return  RETURN_STATUS
 */
SIL_STATUS
SilPrepareTopologyMap (
  SIL_RESERVED_STRUCT_0009  *FabricTopologyMap
  )
{
  SIL_RESERVED_STRUCT_0001                     *BaseDeviceInfo;
  DEVICE_MAP                           *DeviceMap;
  DEVICE_IDS                           *DeviceIds;
  uint32_t                             Die;
  uint32_t                             BaseDeviceInfoCount;
  uint32_t                             Index;
  uint32_t                             DeviceIndex;
  uint32_t                             TypeIndex;
  uint32_t                             TypeCount;
  uint32_t                             InstanceId;
  uint32_t                             FabricId;
  uint32_t                             FchLocationInstanceId;
  uint32_t                             SmuLocationInstanceId;
  uint32_t                             FabricIdSocketShift;
  uint32_t                             FabricIdDieShift;
  uint32_t                             NumberOfBusRegions;
  uint32_t                             Value32;
  bool                                 Enabled;
  bool                                 IsIodBrhAxWA;
  SOC_LOGICAL_ID                       LogicalId;

  // Get the program specific data
  IsIodBrhAxWA = false;

  BaseDeviceInfo = SilBrhDeviceBaseInfo;
  BaseDeviceInfoCount = (sizeof (SilBrhDeviceBaseInfo) / sizeof ((SilBrhDeviceBaseInfo)[0]));

  FchLocationInstanceId = SIL_RESERVED_0195;
  SmuLocationInstanceId = SIL_RESERVED_0195;
  FabricIdSocketShift = BRH_FABRIC_ID_SOCKET_SHIFT;
  FabricIdDieShift = SIL_RESERVED_0177;
  NumberOfBusRegions = SIL_RESERVED_0205;
  GetSocLogicalIdOnCurrentCore(&LogicalId);

  if (IS_SOC_BRH_Ax) {
    NumberOfBusRegions = SIL_RESERVED_0206;
    if (BrhGetNumberOfProcessorsPresent() > 1) {
      IsIodBrhAxWA = true;
    }
  }

  // Get the total instance count
  TypeCount = 0;

  for (Index = 0; BaseDeviceInfo[Index].field0 != FabricDeviceTypeMax; Index++) {
    TypeCount++;
  }

  if (TypeCount > FabricDeviceTypeMax) {
    assert(false);
    return SilInvalidParameter;
  }

  for (Die = 0 ; Die < (DfXGetNumberOfSystemDies () / BrhGetNumberOfProcessorsPresent ()) ; Die++ ) {
    DeviceMap = FabricTopologyMap->Map[Die];
    DeviceIds = FabricTopologyMap->DeviceIds[Die];

    // Update the base fabric topology information
    FabricTopologyMap->field0.field0 = FabricIdSocketShift;
    FabricTopologyMap->field0.field1 = FabricIdDieShift;
    FabricTopologyMap->field0.field2 = NumberOfBusRegions;
    FabricTopologyMap->field0.field3 = FchLocationInstanceId;
    FabricTopologyMap->field0.field4 = SmuLocationInstanceId;
    FabricTopologyMap->field0.field5 = SIL_MAX_UINT32;
    FabricTopologyMap->field0.field6 = SIL_MAX_UINT32;

    // Check for each instance and update the DeviceMap and DeviceIds
    DeviceIndex = 0;
    for (TypeIndex = 0; TypeIndex < TypeCount; TypeIndex++) {
      if ((TypeIndex >= (sizeof (FabricTopologyMap->Map[Die]) / sizeof ((FabricTopologyMap->Map)[Die][0]))) ||
        (TypeIndex >= BaseDeviceInfoCount)) {
        assert(false);
        break;
      }
      DeviceMap[TypeIndex].Type = BaseDeviceInfo[TypeIndex].field0;
      DeviceMap[TypeIndex].Count = 0;
      DeviceMap[TypeIndex].IDsIndex = DeviceIndex;
      if ((DeviceMap[TypeIndex].Type == Ios) && (BaseDeviceInfo[TypeIndex].field2 > 0)) {
        FabricTopologyMap->field0.field5 = BaseDeviceInfo[TypeIndex].field1;
        FabricTopologyMap->field0.field6 = TypeIndex;
      }
      for (Index = 0; Index < BaseDeviceInfo[TypeIndex].field2; Index++) {
        InstanceId = BaseDeviceInfo[TypeIndex].field1 + Index;
        // Fabric Block Instance Information 3 Register
        Value32 = DfXFabricRegisterAccRead(0,
          Die,
          SIL_RESERVED_0269,
          SIL_RESERVED_0270,
          InstanceId
          );
        FabricId = BaseDeviceInfo[TypeIndex].field3 ? SilBitFieldRead32(Value32, 8, 15) : SIL_RESERVED_0203;
        Value32 = DfXFabricRegisterAccRead(0,
          Die,
          FABRICBLOCKINSTANCEINFORMATION0_FUNC,
          FABRICBLOCKINSTANCEINFORMATION0_REG,
          InstanceId
          );
        Enabled = (Value32 != 0);

        if ((DeviceMap[TypeIndex].Type == Iom) || (DeviceMap[TypeIndex].Type == Ios)) {
          if (IsIodBrhAxWA && ((Index % 2) != 0)) {
            Enabled = false;
          }
        }

        // Do not add the instance in the map if it's disabled
        if (Enabled) {
          assert(DeviceIndex < MAX_NUMBER_OF_DEVICE_IDS);
          if (DeviceIndex < MAX_NUMBER_OF_DEVICE_IDS) {
            DeviceIds[DeviceIndex].InstanceID = InstanceId;
            DeviceIds[DeviceIndex].FabricID = FabricId;
            DeviceMap[TypeIndex].Count++;
            DeviceIndex++;
          }
        }
      }
    }
    //Prepare for FabricDeviceTypeMax
    DeviceMap[TypeIndex].Type = FabricDeviceTypeMax;
    DeviceMap[TypeIndex].Count = 0;
    DeviceMap[TypeIndex].IDsIndex = 0;

    assert(FabricTopologyMap->field0.field5 != SIL_MAX_UINT32);
    assert(FabricTopologyMap->field0.field6 != SIL_MAX_UINT32);
  }
  return SilCopyTopologyMap(FabricTopologyMap);
}

/**
 * BrhGetNumberOfRootBridgesOnDie
 *
 * @brief Get the number of root bridges on the given socket die
 *
 * @param Socket     The Socket
 * @param Die        The Die
 *
 * @return uint32_t  The number of root bridges
 *
 */
uint32_t
BrhGetNumberOfRootBridgesOnDie (
  uint32_t Socket,
  uint32_t Die
  )
{
  return gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].Count;
}

/**
 * BrhGetNumberOfProcessorsPresent
 *
 * @brief TBD
 *
 * @return uint32_t TBD
 *
 */
uint32_t
BrhGetNumberOfProcessorsPresent (void)
{
  uint32_t   Value32;

  Value32 = DfXFabricRegisterAccRead(0,
    0,
    SIL_RESERVED_0267,
    SIL_RESERVED_0268,
    FABRIC_REG_ACC_BC
    );
  return (((Value32 & 0xC0000000) >> 30) + 1);
}

/**
 * BrhGetDeviceMapOnDie
 *
 * @brief Get the BRH die device map
 *
 * @param Socket The socket
 * @param Die    The die
 *
 * @return A pointer to the die device map
 */
const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
BrhGetDeviceMapOnDie (
  uint32_t Socket,
  uint32_t Die
  )
{
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *DeviceMap;
  UNUSED(Socket);

  DeviceMap = &gBrhDeviceMap[Die][0];
  return DeviceMap;
}

/**
 * BrhGetDieSystemOffset
 *
 * @brief TBD
 *
 * @param Socket The socket
 * @param Die    The die
 *
 * @return uint32_t The encoded offset of socket/die
 *
 */
uint32_t
BrhGetDieSystemOffset (
  uint32_t Socket,
  uint32_t Die
  )
{
  return (Socket << gBaseFabricTopologyInfo.field0) +
         (Die << gBaseFabricTopologyInfo.field1);
}

/**
 * BrhGetNumberOfBusRegions
 *
 * @brief Return number of PCIe config register instance.
 *
 * @retval   uint8_t  SIL_RESERVED_0205    - IOD A0
 *                    BRH_B0_NUMBER_OF_BUS_REGIONS - IOD B0
 *
 * @note NOT COMMON.  Move to xfer
 */
uint8_t
BrhGetNumberOfBusRegions (void)
{

  return (uint8_t) gBaseFabricTopologyInfo.field2;

}

/**
 * BrhGetHostBridgeBusBase
 *
 * @brief Get the host bridge bus base for the given socket/die
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Index  Index in the fabric device table
 *
 * @return uint32_t The host bridge bus base
 *
 */
uint32_t
BrhGetHostBridgeBusBase (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t                    NumberOfBusRegions;
  uint32_t                    TargetFabricId;
  uint32_t                    CfgAddrMapIndex;
  CFG_LIMIT_ADDRESS_REGISTER  CfgLimit;
  CFG_BASE_ADDRESS_REGISTER   CfgBase;

  NumberOfBusRegions = BrhGetNumberOfBusRegions ();
  TargetFabricId = DfGetHostBridgeSystemFabricID(Socket, Die, Index);

  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < NumberOfBusRegions; CfgAddrMapIndex++) {
    CfgLimit.Value = DfXFabricRegisterAccRead(Socket,
      Die,
      CFGLIMITADDRESS_0_FUNC,
      (CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * SIL_RESERVED_0125)),
      FABRIC_REG_ACC_BC
      );
    CfgBase.Value = DfXFabricRegisterAccRead(Socket,
      Die,
      CFGBASEADDRESS_0_FUNC,
      (CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * SIL_RESERVED_0125)),
      FABRIC_REG_ACC_BC
      );
    if ((CfgBase.Field.RE == 1) &&
      (CfgBase.Field.WE == 1) &&
      (CfgLimit.Field.DstFabricID == (uint32_t) TargetFabricId)) {
      return CfgBase.Field.SegmentNum * MAX_PCI_BUS_NUMBER_PER_SEGMENT + CfgBase.Field.BusNumBase;
    }
  }

  return 0xFFFFFFFF;
}

/**
 * BrhGetHostBridgeBusLimit
 *
 * @brief Get the host bridge bus limit for the given socket/die
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Index  Index in the fabric device table
 *
 * @return uint32_t The host bridge bus limit
 *
 */
uint32_t
BrhGetHostBridgeBusLimit (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t                                   BusLimit;
  uint32_t                                   CfgAddrMapIndex;
  uint32_t                                   TargetFabricId;
  CFG_LIMIT_ADDRESS_REGISTER                 CfgLimit;
  CFG_BASE_ADDRESS_REGISTER                  CfgBase;
  uint8_t                                    NumberOfBusRegions;

  NumberOfBusRegions = BrhGetNumberOfBusRegions ();
  TargetFabricId = DfGetHostBridgeSystemFabricID(Socket, Die, Index);

  BusLimit = 0xFF;
  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < NumberOfBusRegions; CfgAddrMapIndex++) {
    CfgLimit.Value = DfXFabricRegisterAccRead(Socket,
      Die,
      CFGLIMITADDRESS_0_FUNC,
      (CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * SIL_RESERVED_0125)),
      FABRIC_REG_ACC_BC
      );
    CfgBase.Value = DfXFabricRegisterAccRead(Socket,
      Die,
      CFGBASEADDRESS_0_FUNC,
      (CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * SIL_RESERVED_0125)),
      FABRIC_REG_ACC_BC
      );
    if ((CfgBase.Field.RE == 1) &&
      (CfgBase.Field.WE == 1) &&
      (CfgLimit.Field.DstFabricID == TargetFabricId)) {
      BusLimit = CfgBase.Field.SegmentNum * MAX_PCI_BUS_NUMBER_PER_SEGMENT + CfgLimit.Field.BusNumLimit;
      break;
    }
  }

  return BusLimit;
}

/**
 * BrhGetPhysRootBridgeNumber
 *
 * @brief TBD
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Index  The index in the fabric device map
 *
 * @return uint32_t TBD
 *
 */
uint32_t
BrhGetPhysRootBridgeNumber (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t   InstanceID;
  UNUSED(Socket);

  InstanceID = gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].IDs[Index].InstanceID;
  if (Index < gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].Count) {
    return InstanceID - gBaseFabricTopologyInfo.field5;
  }

  assert(false);
  return 0;
}

/**
 * BrhFindDeviceTypeEntryInMap
 *
 * @brief TBD
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Type   The fabric device type
 *
 * @return const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* TBD
 *
 */
const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
BrhFindDeviceTypeEntryInMap (
  uint32_t            Socket,
  uint32_t            Die,
  FABRIC_DEVICE_TYPE  Type
  )
{
  uint32_t  i;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *DeviceMap;

  DeviceMap = BrhGetDeviceMapOnDie(Socket, Die);
  for (i = 0; DeviceMap[i].Type < FabricDeviceTypeMax; i++) {
    if (DeviceMap[i].Type == Type) {
      return &DeviceMap[i];
    }
  }

  return NULL;
}

/**
 * BrhGetSystemComponentRootBridgeLocation
 *
 * @brief TBD
 *
 * @param Component TBD
 * @param Location TBD
 *
 * @return bool TBD
 *
 */
bool
BrhGetSystemComponentRootBridgeLocation (
  COMPONENT_TYPE Component,
  ROOT_BRIDGE_LOCATION *Location
  )
{
  AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *IosMap;
  uint32_t                           TargetInstanceId;
  uint32_t                           Index;

  if (!Location) {
    return false;
  }
  switch (Component) {
  case PrimaryFch:
    TargetInstanceId = gBaseFabricTopologyInfo.field3;
    break;
  case PrimarySmu:
    TargetInstanceId = gBaseFabricTopologyInfo.field4;
    break;
  default:
    assert(false);
    return false;
  }

  IosMap = &gBrhDeviceMap[0][gBaseFabricTopologyInfo.field6];
  if (IosMap) {
    for (Index = 0; Index < IosMap->Count; Index++) {
      if (IosMap->IDs[Index].InstanceID == TargetInstanceId) {
        Location->Socket = 0;
        Location->Die = 0;
        Location->Index = Index;
        return true;
      }
    }
  }

  assert(false);
  return false;
}

/**
 * BrhHasFch
 *
 * @brief Check if FCH exists
 *
 * @param Socket The socket
 * @param Die    The die
 * @param Index  The index in the fabric device table
 *
 * @return bool TBD
 *
 */
bool
BrhHasFch (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t   InstanceID;
  UNUSED(Socket);

  InstanceID = gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].IDs[Index].InstanceID;

  if (Index < gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].Count) {
    return (InstanceID == gBaseFabricTopologyInfo.field3);
  }

  assert(false);
  return false;
}

/**
 * BrhHasSmu
 *
 * @brief Check if SMU exists
 *
 * @param Socket TBD
 * @param Die TBD
 * @param Index TBD
 *
 * @return bool TBD
 *
 */
bool
BrhHasSmu (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  uint32_t   InstanceID;
  UNUSED(Socket);

  InstanceID = gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].IDs[Index].InstanceID;

  if (Index < gBrhDeviceMap[Die][gBaseFabricTopologyInfo.field6].Count) {
    return (InstanceID == gBaseFabricTopologyInfo.field4);
  }

  assert(false);
  return false;
}
