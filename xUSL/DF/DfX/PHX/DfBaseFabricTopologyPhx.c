/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfBaseFabricTopologyPhx.c
 * @brief Phx specific implementations of BaseFabricTopology functions
 */


#include <DF/Df.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include "DfSilFabricInfoPhx.h"
#include "DfSilFabricRegistersPhx.h"
#include "DfPhx.h"
#include "DfCmn2Phx.h"

//
// Phx IOD
//
static const uint32_t PhxPhysIos0FabricId = PHX_IOS0_FABRIC_ID;

static const COMPONENT_LOCATION  PhxComponentLocation [] = {
  {0, 0, PHX_IOS0_FABRIC_ID, PrimaryFch}, // Physical location, Socket 0, Die 0, Ios2
  {0, 0, PHX_IOS0_FABRIC_ID, PrimarySmu}, // Physical location, Socket 0, Die 0, Ios2
};

static const DEVICE_IDS   PhxCsMap [] = {
  {PHX_CS0_FABRIC_ID, PHX_CS0_INSTANCE_ID},
  {PHX_CS1_FABRIC_ID, PHX_CS1_INSTANCE_ID},
  {PHX_CS2_FABRIC_ID, PHX_CS2_INSTANCE_ID},
  {PHX_CS3_FABRIC_ID, PHX_CS3_INSTANCE_ID}
};

static const DEVICE_IDS PhxCcmMap [] = {
  {PHX_CCM0_FABRIC_ID, PHX_CCM0_INSTANCE_ID}
};

static const DEVICE_IDS   PhxNcmMap [] = {
  {PHX_NCM0_FABRIC_ID, PHX_NCM0_INSTANCE_ID},
  {PHX_NCM1_FABRIC_ID, PHX_NCM1_INSTANCE_ID}
};

static const DEVICE_IDS   PhxIomMap [] = {
  {PHX_IOM0_FABRIC_ID, PHX_IOM0_INSTANCE_ID}
};

static const DEVICE_IDS   PhxIosMap [] = {
  {PHX_IOS0_FABRIC_ID, PHX_IOS0_INSTANCE_ID}
};

static const DEVICE_IDS   PhxPieMap [] = {
  {PHX_PIE_FABRIC_ID, PHX_PIE_INSTANCE_ID}
};

static const DEVICE_IDS   PhxSpfMap [] = {
  {0xFFFFFFFF, PHX_SPF0_INSTANCE_ID},
  {0xFFFFFFFF, PHX_SPF1_INSTANCE_ID},
  {0xFFFFFFFF, PHX_SPF2_INSTANCE_ID},
  {0xFFFFFFFF, PHX_SPF3_INSTANCE_ID}
};

static const DEVICE_IDS   PhxTcdxMap [] = {
  {0xFFFFFFFF, PHX_TCDX0_INSTANCE_ID},
  {0xFFFFFFFF, PHX_TCDX1_INSTANCE_ID},
  {0xFFFFFFFF, PHX_TCDX2_INSTANCE_ID},
  {0xFFFFFFFF, PHX_TCDX3_INSTANCE_ID},
  {0xFFFFFFFF, PHX_TCDX4_INSTANCE_ID},
  {0xFFFFFFFF, PHX_TCDX5_INSTANCE_ID}
};

static const DEVICE_IDS  PhxGcmMap[] = {
  {PHX_GCM0_FABRIC_ID, PHX_GCM0_INSTANCE_ID},
  {PHX_GCM1_FABRIC_ID, PHX_GCM1_INSTANCE_ID},
  {PHX_GCM2_FABRIC_ID, PHX_GCM2_INSTANCE_ID},
  {PHX_GCM3_FABRIC_ID, PHX_GCM3_INSTANCE_ID}
};

static const DEVICE_IDS  PhxNcsMap[] = {
  {PHX_NCS0_FABRIC_ID, PHX_NCS0_INSTANCE_ID},
  {PHX_NCS1_FABRIC_ID, PHX_NCS1_INSTANCE_ID}
};

static const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  PhxDeviceMap[] = {
  {Cs, sizeof (PhxCsMap) / sizeof (PhxCsMap[0]), &PhxCsMap[0]     },
  {Gcm, sizeof (PhxGcmMap) / sizeof (PhxGcmMap[0]), &PhxGcmMap[0]    },
  {Ccm, sizeof (PhxCcmMap) / sizeof (PhxCcmMap[0]), &PhxCcmMap[0]    },
  {Ncm, sizeof (PhxNcmMap) / sizeof (PhxNcmMap[0]), &PhxNcmMap[0]    },
  {Ncs, sizeof (PhxNcsMap) / sizeof (PhxNcsMap[0]), &PhxNcsMap[0]    },
  {Iom, sizeof (PhxIomMap) / sizeof (PhxIomMap[0]), &PhxIomMap[0]    },
  {Ios, sizeof (PhxIosMap) / sizeof (PhxIosMap[0]), &PhxIosMap[0]    },
  {Pie, sizeof (PhxPieMap) / sizeof (PhxPieMap[0]), &PhxPieMap[0]    },
  {Spf, sizeof (PhxSpfMap) / sizeof (PhxSpfMap[0]), &PhxSpfMap[0]    },
  {Tcdx, sizeof (PhxTcdxMap) / sizeof (PhxTcdxMap[0]), &PhxTcdxMap[0]   },
  {FabricDeviceTypeMax, 0, NULL}
};


/**
 * PhxGetNumberOfProcessorsPresent
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 *
 * @return uint32_t TBD
 *
 */
uint32_t PhxGetNumberOfProcessorsPresent (
  SIL_CONTEXT   *SilContext
  )
{
  SYSTEM_CFG_REGISTER SysCfg;

  SysCfg.Value = DfXFabricRegisterAccRead(SilContext, 0, 0, SYSTEMCFG_FUNC, SYSTEMCFG_REG, FABRIC_REG_ACC_BC);
  return SysCfg.Field.OtherSocketPresent + 1;
}

/**
 * PhxGetDeviceMapOnDie
 *
 * @brief Return a pointer to the device map for the input socket/die
 *
 * @param Socket  The socket number
 * @param Die     The die number
 *
 * @return const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* Pointer to the device map for the socket/die
 *
 */
const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
PhxGetDeviceMapOnDie (
  uint32_t Socket,
  uint32_t Die
  )
{
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *DeviceMap;

  DeviceMap = &PhxDeviceMap[0];
  return DeviceMap;
}

/**
 * PhxGetDieSystemOffset
 *
 * @brief TBD
 *
 * @param Socket TBD
 *
 * @return uint32_t TBD
 *
 */
uint32_t
PhxGetDieSystemOffset (
  uint32_t Socket,
  uint32_t Die
  )
{
  UNUSED(Die);
  return (Socket << PHX_FABRIC_ID_SOCKET_SHIFT);
}

/**
 * PhxGetNumberOfBusRegions
 *
 * @brief Return number of PCIe config register instance.
 *
 * @retval   uint8_t PHX_NUMBER_OF_BUS_REGIONS
 *
 */
uint8_t
PhxGetNumberOfBusRegions (void)
{
  return PHX_NUMBER_OF_BUS_REGIONS;
}

/**
 * PhxGetHostBridgeBusBase
 *
 * @brief TBD
 *
 * @param SilContext     A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @param Socket         The socket number
 * @param Die            The die number
 * @param Index          The root bridge number
 *
 * @return uint32_t TBD
 *
 */
uint32_t
PhxGetHostBridgeBusBase (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       Index
  )
{
  UNUSED(SilContext);
  UNUSED(Socket);
  UNUSED(Die);
  UNUSED(Index);
  return 0;
}

/**
 * PhxGetHostBridgeBusLimit
 *
 * @brief TBD
 *
 * @param SilContext     A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @param Socket         The socket number
 * @param Die            The die number
 * @param Index          The root bridge number
 *
 * @return uint32_t      The Bus Limit for the host bridge
 *
 */
uint32_t
PhxGetHostBridgeBusLimit (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       Index
  )
{
  uint32_t                                   BusLimit;
  uint32_t                                   CfgAddrMapIndex;
  uint32_t                                   TargetFabricId;
  CFG_LIMIT_ADDRESS_REGISTER                 CfgLimit;
  CFG_BASE_ADDRESS_REGISTER                  CfgBase;

  TargetFabricId = DfGetHostBridgeSystemFabricID(SilContext, Socket, Die, Index);

  BusLimit = 0xFF;
  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < PHX_NUMBER_OF_BUS_REGIONS; CfgAddrMapIndex++) {
    CfgLimit.Value = DfXFabricRegisterAccRead(SilContext,
      Socket,
      Die,
      CFGLIMITADDRESS_0_FUNC,
      (CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * PHX_BUS_REGION_REGISTER_OFFSET)),
      FABRIC_REG_ACC_BC
      );
    CfgBase.Value = DfXFabricRegisterAccRead(SilContext,
      Socket,
      Die,
      CFGBASEADDRESS_0_FUNC,
      (CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * PHX_BUS_REGION_REGISTER_OFFSET)),
      FABRIC_REG_ACC_BC
      );
    if ((CfgBase.Field.RE == 1) &&
      (CfgBase.Field.WE == 1) &&
      (CfgLimit.Field.DstFabricID == (uint32_t)TargetFabricId)) {
      BusLimit = (uint32_t)CfgLimit.Field.BusNumLimit;
      break;
    }
  }

  return BusLimit;
}

/**
 * PhxGetPhysRootBridgeNumber
 *
 * @brief Return the physical root bridge number for the input socket/die/rootbridge
 *
 * @param Socket         The socket number
 * @param Die            The die number
 * @param Index          The root bridge number
 *
 * @return uint32_t      The physical root bridge number
 *
 */
uint32_t
PhxGetPhysRootBridgeNumber (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Index
  )
{
  UNUSED(Socket);
  UNUSED(Die);
  UNUSED(Index);

  return 0;
}

/**
 * PhxFindDeviceTypeEntryInMap
 *
 * @brief Return the device map entry for in the input type on socket/die
 *
 * @param Socket         The socket number
 * @param Die            The die number
 * @param Type           The fabric device type
 *
 * @return const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP* The device map entry
 *
 */
const
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
PhxFindDeviceTypeEntryInMap (
  uint32_t            Socket,
  uint32_t            Die,
  FABRIC_DEVICE_TYPE  Type
  )
{
  uint32_t  i;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *DeviceMap;

  DeviceMap = PhxGetDeviceMapOnDie(Socket, Die);
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
 * PhxFindComponentLocationMap
 *
 * @param Count
 * @param PhysIos0FabricId
 *
 * @return const COMPONENT_LOCATION*
 *
 */
const
COMPONENT_LOCATION *
PhxFindComponentLocationMap (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  )
{
  const COMPONENT_LOCATION *ComponentLocationMap;

  ComponentLocationMap = &PhxComponentLocation[0];
  if (Count != NULL) {
    *Count = sizeof (PhxComponentLocation) / sizeof (PhxComponentLocation[0]);
  }
  if (PhysIos0FabricId != NULL) {
    *PhysIos0FabricId = PhxPhysIos0FabricId;
  }
  return ComponentLocationMap;
}

/**
 * PhxGetSystemComponentRootBridgeLocation
 *
 * @brief
 *
 * @param SilContext   A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 *
 * @return bool
 *
 */
bool
PhxGetSystemComponentRootBridgeLocation (
  SIL_CONTEXT          *SilContext,
  COMPONENT_TYPE       Component,
  ROOT_BRIDGE_LOCATION *Location
  )
{
  uint32_t  i;
  uint32_t  j;
  uint32_t  Count;
  bool    Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;

  UNUSED(SilContext);

  assert((Component == PrimaryFch) || (Component == PrimarySmu));

  ComponentLocation = PhxFindComponentLocationMap(&Count, NULL);

  Found = false;
  for (i = 0; i < Count; i++) {
    if (ComponentLocation[i].Type == Component) {
      Location->Socket = ComponentLocation[i].Socket;
      Location->Die = ComponentLocation[i].Die;

      IosEntry = PhxFindDeviceTypeEntryInMap(Location->Socket, Location->Die, Ios);
      assert(IosEntry != NULL);
      for (j = 0; j < (IosEntry->Count); j++) {
        if (IosEntry->IDs[j].FabricID == ComponentLocation[i].IomsFabricId) {
          Location->Index = (uint32_t) j;
          break;
        }
      }
      assert(j < IosEntry->Count);
      Found = true;
      break;
    }
  }
  return Found;
}

/**
 * PhxHasFch
 *
 * @brief TBD
 *
 * @param SilContext   A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 * @param Socket      The socket
 * @param Die         The die
 * @param Index       The index in the fabric device table
 *
 * @return bool TBD
 *
 */
bool
PhxHasFch (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Index
  )
{
  uint32_t   i;
  uint32_t   Count;
  uint32_t   IosFabricId;
  uint32_t   Ios0FabricId;
  bool Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;

  UNUSED(SilContext);

  IosEntry = PhxFindDeviceTypeEntryInMap(Socket, Die, Ios);
  assert(IosEntry != NULL);
  IosFabricId = IosEntry->IDs[Index].FabricID;

  ComponentLocation = PhxFindComponentLocationMap(&Count, &Ios0FabricId);

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
 * PhxHasSmu
 *
 * @brief TBD
 *
 * @param SilContext   A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 * @param Socket      The socket
 * @param Die         The die
 * @param Index       The index in the fabric device table
 *
 * @return bool TBD
 *
 */
bool
PhxHasSmu (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Index
  )
{
  uint32_t   i;
  uint32_t   Count;
  uint32_t   IosFabricId;
  uint32_t   Ios0FabricId;
  bool Found;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;
  const COMPONENT_LOCATION                  *ComponentLocation;

  UNUSED(SilContext);

  IosEntry = PhxFindDeviceTypeEntryInMap(Socket, Die, Ios);
  assert(IosEntry != NULL);
  IosFabricId = IosEntry->IDs[Index].FabricID;

  ComponentLocation = PhxFindComponentLocationMap(&Count, &Ios0FabricId);

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
