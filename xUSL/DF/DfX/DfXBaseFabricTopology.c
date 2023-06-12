/**
 * @file  DfXBaseFabricTopology.c
 * @brief DfX specific implementations of BaseFabricTopology functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <DF/Df.h>
#include <DF/Common/SilFabricInfo.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include "SilFabricRegistersDfX.h"
#include "DfXBaseFabricTopology.h"
#include "DfXFabricRegisterAcc.h"

//
// Genoa IOD
//
const uint32_t GenoaPhysIos0FabricId = 0x20;

const COMPONENT_LOCATION  GenoaComponentLocation [] = {
  {0, 0, 0x22, PrimaryFch}, // Physical location, Socket 0, Die 0, Ios2
  {1, 0, 0x22, SecondaryFch},   // Physical location, Socket 1, Die 0, Ios2
  {0, 0, 0x22, PrimarySmu}, // Physical location, Socket 0, Die 0, Ios2
  {1, 0, 0x22, SecondarySmu},   // Physical location, Socket 1, Die 0, Ios2
};

const DEVICE_IDS   GenoaCsMap [] = {
  {0x00000000, 0x00000000},
  {0x00000001, 0x00000001},
  {0x00000002, 0x00000002},
  {0x00000003, 0x00000003},
  {0x00000004, 0x00000004},
  {0x00000005, 0x00000005},
  {0x00000006, 0x00000006},
  {0x00000007, 0x00000007},
  {0x00000008, 0x00000008},
  {0x00000009, 0x00000009},
  {0x0000000A, 0x0000000A},
  {0x0000000B, 0x0000000B},
  {0x0000000C, 0x0000000C},
  {0x0000000D, 0x0000000D},
  {0x0000000E, 0x0000000E},
  {0x0000000F, 0x0000000F}
};

const DEVICE_IDS GenoaCsUmcMap [] = {
  {0x00000000, 0x00000000},
  {0x00000001, 0x00000001},
  {0x00000002, 0x00000002},
  {0x00000003, 0x00000003},
  {0x00000004, 0x00000004},
  {0x00000005, 0x00000005},
  {0x00000006, 0x00000006},
  {0x00000007, 0x00000007},
  {0x00000008, 0x00000008},
  {0x00000009, 0x00000009},
  {0x0000000A, 0x0000000A},
  {0x0000000B, 0x0000000B}
};

const DEVICE_IDS   GenoaCsCmpMap [] = {
  {0x0000000C, 0x0000000C},
  {0x0000000D, 0x0000000D},
  {0x0000000E, 0x0000000E},
  {0x0000000F, 0x0000000F}
};

const DEVICE_IDS GenoaCcmMap [] = {
  {0x00000060, 0x00000010},
  {0x00000061, 0x00000011},
  {0x00000062, 0x00000012},
  {0x00000063, 0x00000013},
  {0x00000064, 0x00000014},
  {0x00000065, 0x00000015},
  {0x00000066, 0x00000016},
  {0x00000067, 0x00000017}
};

const DEVICE_IDS   GenoaAcmMap [] = {
  {0x0000006C, 0x00000018},
  {0x0000006D, 0x00000019},
  {0x0000006E, 0x0000001A},
  {0x0000006F, 0x0000001B}
};

const DEVICE_IDS   GenoaNcmMap [] = {
  {0x00000070, 0x0000001C},
  {0x00000071, 0x0000001D},
  {0x00000072, 0x0000001E},
  {0x00000073, 0x0000001F}
};

const DEVICE_IDS   GenoaIomMap [] = {
  {0x00000078, 0x00000020},
  {0x00000079, 0x00000021},
  {0x0000007A, 0x00000022},
  {0x0000007B, 0x00000023}
};

const DEVICE_IDS   GenoaIosMap [] = {
  {0x00000020, 0x00000024},
  {0x00000021, 0x00000025},
  {0x00000022, 0x00000026},
  {0x00000023, 0x00000027}
};

const DEVICE_IDS   GenoaIcngMap [] = {
  {0x0000007C, 0x00000028},
  {0x0000007D, 0x00000029},
  {0x0000007E, 0x0000002A},
  {0x0000007F, 0x0000002B}
};

const DEVICE_IDS   GenoaPieMap [] = {
  {0x00000077, 0x0000002C}
};

const DEVICE_IDS   GenoaCakeMap [] = {
  {0xFFFFFFFF, 0x0000002D},
  {0xFFFFFFFF, 0x0000002E},
  {0xFFFFFFFF, 0x0000002F},
  {0xFFFFFFFF, 0x00000030},
  {0xFFFFFFFF, 0x00000031},
  {0xFFFFFFFF, 0x00000032},
  {0xFFFFFFFF, 0x00000033},
  {0xFFFFFFFF, 0x00000034}
};

const DEVICE_IDS   GenoaCnliMap [] = {
  {0xFFFFFFFF, 0x00000035},
  {0xFFFFFFFF, 0x00000036},
  {0xFFFFFFFF, 0x00000037},
  {0xFFFFFFFF, 0x00000038}
};

const DEVICE_IDS   GenoaPfxMap [] = {
  {0xFFFFFFFF, 0x00000039},
  {0xFFFFFFFF, 0x0000003A},
  {0xFFFFFFFF, 0x0000003B},
  {0xFFFFFFFF, 0x0000003C},
  {0xFFFFFFFF, 0x0000003D},
  {0xFFFFFFFF, 0x0000003E},
  {0xFFFFFFFF, 0x0000003F},
  {0xFFFFFFFF, 0x00000040}
};

const DEVICE_IDS   GenoaSpfMap [] = {
  {0xFFFFFFFF, 0x00000041},
  {0xFFFFFFFF, 0x00000042},
  {0xFFFFFFFF, 0x00000043},
  {0xFFFFFFFF, 0x00000044},
  {0xFFFFFFFF, 0x00000045},
  {0xFFFFFFFF, 0x00000046},
  {0xFFFFFFFF, 0x00000047},
  {0xFFFFFFFF, 0x00000048},
  {0xFFFFFFFF, 0x00000049},
  {0xFFFFFFFF, 0x0000004A},
  {0xFFFFFFFF, 0x0000004B},
  {0xFFFFFFFF, 0x0000004C},
  {0xFFFFFFFF, 0x0000004D},
  {0xFFFFFFFF, 0x0000004E},
  {0xFFFFFFFF, 0x0000004F},
  {0xFFFFFFFF, 0x00000050}
};

const DEVICE_IDS   GenoaTcdxMap [] = {
  {0xFFFFFFFF, 0x00000051},
  {0xFFFFFFFF, 0x00000052},
  {0xFFFFFFFF, 0x00000053},
  {0xFFFFFFFF, 0x00000054},
  {0xFFFFFFFF, 0x00000055},
  {0xFFFFFFFF, 0x00000056},
  {0xFFFFFFFF, 0x00000057},
  {0xFFFFFFFF, 0x00000058},
  {0xFFFFFFFF, 0x00000059},
  {0xFFFFFFFF, 0x0000005A},
  {0xFFFFFFFF, 0x0000005B},
  {0xFFFFFFFF, 0x0000005C},
  {0xFFFFFFFF, 0x0000005D},
  {0xFFFFFFFF, 0x0000005E},
  {0xFFFFFFFF, 0x0000005F},
  {0xFFFFFFFF, 0x00000060}
};

const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  GenoaDeviceMap[] = {
  {Cs,    sizeof (GenoaCsMap) / sizeof (GenoaCsMap[0]),        &GenoaCsMap[0]     },
  {CsUmc, sizeof (GenoaCsUmcMap) / sizeof (GenoaCsUmcMap[0]),  &GenoaCsUmcMap[0]  },
  {CsCmp, sizeof (GenoaCsCmpMap) / sizeof (GenoaCsCmpMap[0]),  &GenoaCsCmpMap[0]  },
  {Ccm,   sizeof (GenoaCcmMap) / sizeof (GenoaCcmMap[0]),      &GenoaCcmMap[0]    },
  {Acm,   sizeof (GenoaAcmMap) / sizeof (GenoaAcmMap[0]),      &GenoaAcmMap[0]    },
  {Ncm,   sizeof (GenoaNcmMap) / sizeof (GenoaNcmMap[0]),      &GenoaNcmMap[0]    },
  {Iom,   sizeof (GenoaIomMap) / sizeof (GenoaIomMap[0]),      &GenoaIomMap[0]    },
  {Ios,   sizeof (GenoaIosMap) / sizeof (GenoaIosMap[0]),      &GenoaIosMap[0]    },
  {Icng,  sizeof (GenoaIcngMap) / sizeof (GenoaIcngMap[0]),    &GenoaIcngMap[0]   },
  {Pie,   sizeof (GenoaPieMap) / sizeof (GenoaPieMap[0]),      &GenoaPieMap[0]    },
  {Cake,  sizeof (GenoaCakeMap) / sizeof (GenoaCakeMap[0]),    &GenoaCakeMap[0]   },
  {Cnli,  sizeof (GenoaCnliMap) / sizeof (GenoaCnliMap[0]),    &GenoaCnliMap[0]   },
  {Pfx,   sizeof (GenoaPfxMap) / sizeof (GenoaPfxMap[0]),      &GenoaPfxMap[0]    },
  {Spf,   sizeof (GenoaSpfMap) / sizeof (GenoaSpfMap[0]),      &GenoaSpfMap[0]    },
  {Tcdx,  sizeof (GenoaTcdxMap) / sizeof (GenoaTcdxMap[0]),    &GenoaTcdxMap[0]   },
  {FabricDeviceTypeMax, 0, NULL}
};


/**
  *	DfXGetNumberOfProcessorsPresent
  *
  *	@brief TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t DfXGetNumberOfProcessorsPresent (void)
{
  SYSTEM_CFG_REGISTER SysCfg;

  SysCfg.Value = DfXFabricRegisterAccRead (0, SYSTEMCFG_FUNC, SYSTEMCFG_REG, FABRIC_REG_ACC_BC);
  return SysCfg.Field.OtherSocketPresent + 1;
}

/**
  *	DfXGetNumberOfSystemDies
  *
  *	@brief TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t DfXGetNumberOfSystemDies (void)
{
  uint32_t                   Value32;

  Value32 = DfXFabricRegisterAccRead (0, 0x4, 0x184, FABRIC_REG_ACC_BC);

  return Value32 & 0xFF;
}

/**
  *	DfXGetNumberOfSystemRootBridges
  *
  *	@brief TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t DfXGetNumberOfSystemRootBridges (void)
{
  uint32_t                   Value32;

  Value32 = DfXFabricRegisterAccRead (0, 0x4, 0x184, FABRIC_REG_ACC_BC);

  return (Value32 >> 8) & 0xFF;
}

/**
  *	DfXGetNumberOfRootBridgesOnSocket
  *
  *	@brief TBD
  *
  * @param Socket TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t
DfXGetNumberOfRootBridgesOnSocket (
  uint32_t Socket
  )
{
  uint32_t                  RbCount;
  uint32_t                  Value32;

  Value32 = DfXFabricRegisterAccRead (Socket, 0x4, 0x184, FABRIC_REG_ACC_BC);
  RbCount = ((Value32 >> 8) & 0xFF) / DfXGetNumberOfProcessorsPresent ();
  return RbCount;
}

/**
  *	DfXGetNumberOfRootBridgesOnDie
  *
  *	@brief TBD
  *
  * @param Socket TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t
DfXGetNumberOfRootBridgesOnDie (
  uint32_t Socket
  )
{
  uint32_t                   RbCount;
  uint32_t                   Value32;

  Value32 = DfXFabricRegisterAccRead (Socket, 0x4, 0x184, FABRIC_REG_ACC_BC);
  RbCount = ((Value32 >> 8) & 0xFF) / (uint32_t) DfXGetNumberOfProcessorsPresent ();
  RbCount = RbCount / (uint32_t) DfGetNumberOfDiesOnSocket ();

  return RbCount;
}

/**
  *	DfXGetDieSystemOffset
  *
  *	@brief TBD
  *
  * @param Socket TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t
DfXGetDieSystemOffset (
  uint32_t Socket
  )
{
  return (Socket << DFX_FABRIC_ID_SOCKET_SHIFT);
}

const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP * DfXGetDeviceMapOnDie (void)
{
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *DeviceMap;

  DeviceMap = &GenoaDeviceMap[0];
  return DeviceMap;
}

/**
  *	DfXGetHostBridgeBusBase
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
DfXGetHostBridgeBusBase (
  uint32_t Socket,
  uint32_t Index
  )
{
  uint32_t                    CfgAddrMapIndex;
  uint32_t                    TargetFabricId;
  CFG_LIMIT_ADDRESS_REGISTER  CfgLimit;
  CFG_BASE_ADDRESS_REGISTER   CfgBase;

  TargetFabricId = DfGetHostBridgeSystemFabricID (Socket, Index);

  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < DFX_NUMBER_OF_BUS_REGIONS; CfgAddrMapIndex++) {
    CfgLimit.Value = DfXFabricRegisterAccRead (Socket, CFGLIMITADDRESS_0_FUNC,
          (CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * DFX_BUS_REGION_REGISTER_OFFSET)), FABRIC_REG_ACC_BC);
    CfgBase.Value = DfXFabricRegisterAccRead (Socket, CFGBASEADDRESS_0_FUNC,
          (CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * DFX_BUS_REGION_REGISTER_OFFSET)), FABRIC_REG_ACC_BC);
    if ((CfgBase.Field.RE == 1) && (CfgBase.Field.WE == 1) &&
        (CfgLimit.Field.DstFabricID == (uint32_t) TargetFabricId)) {
      return CfgBase.Field.SegmentNum * MAX_PCI_BUS_NUMBER_PER_SEGMENT + CfgBase.Field.BusNumBase;
    }
  }

  return 0xFFFFFFFF;
}

/**
  *	DfXGetHostBridgeBusLimit
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
DfXGetHostBridgeBusLimit (
  uint32_t Socket,
  uint32_t Index
  )
{
  uint32_t                                     BusLimit;
  uint32_t                                   CfgAddrMapIndex;
  uint32_t                                     TargetFabricId;
  CFG_LIMIT_ADDRESS_REGISTER                 CfgLimit;
  CFG_BASE_ADDRESS_REGISTER                  CfgBase;

  TargetFabricId = DfGetHostBridgeSystemFabricID (Socket, Index);

  BusLimit = 0xFF;
  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < DFX_NUMBER_OF_BUS_REGIONS; CfgAddrMapIndex++) {
    CfgLimit.Value = DfXFabricRegisterAccRead (Socket, CFGLIMITADDRESS_0_FUNC,
        (CFGLIMITADDRESS_0_REG + (CfgAddrMapIndex * DFX_BUS_REGION_REGISTER_OFFSET)), FABRIC_REG_ACC_BC);
    CfgBase.Value = DfXFabricRegisterAccRead (Socket, CFGBASEADDRESS_0_FUNC,
        (CFGBASEADDRESS_0_REG + (CfgAddrMapIndex * DFX_BUS_REGION_REGISTER_OFFSET)), FABRIC_REG_ACC_BC);
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
  *	DfXGetPhysRootBridgeNumber
  *
  *	@brief TBD
  *
  * @param Index TBD
  *
  *	@return uint32_t TBD
  *
  */
uint32_t
DfXGetPhysRootBridgeNumber (
  uint32_t Index
  )
{
  uint32_t  FabricId;
  uint32_t  PhysNumber;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *IosEntry;


  IosEntry = DfFindDeviceTypeEntryInMap (Ios);
  assert (IosEntry != NULL);
  FabricId = IosEntry->IDs[Index].FabricID;

  assert (FabricId >= GenoaPhysIos0FabricId);
  PhysNumber = FabricId - GenoaPhysIos0FabricId;

  return PhysNumber;
}

/**
  *	DfXFindComponentLocationMap
  *
  *	@brief TBD
  *
  * @param Count TBD
  * @param PhysIos0FabricId TBD
  *
  *	@return const COMPONENT_LOCATION* TBD
  *
  */
const
COMPONENT_LOCATION *
DfXFindComponentLocationMap (
  uint32_t *Count,
  uint32_t *PhysIos0FabricId
  )
{
  const COMPONENT_LOCATION *ComponentLocationMap;

  ComponentLocationMap = &GenoaComponentLocation[0];
  if (Count != NULL) {
    *Count = sizeof (GenoaComponentLocation) / sizeof (GenoaComponentLocation[0]);
  }
  if (PhysIos0FabricId != NULL) {
    *PhysIos0FabricId = GenoaPhysIos0FabricId;
  }
  return ComponentLocationMap;
}

/**
  *	DfBaseFabricTopologyConstructor
  *
  *	@brief TBD
  *
  *	@return SIL_STATUS
  *
  */
SIL_STATUS DfXBaseFabricTopologyConstructor (void)
{
  return SilPass;
}
