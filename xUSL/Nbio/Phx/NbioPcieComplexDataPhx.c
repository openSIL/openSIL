/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPcieComplexDataPhx.c
 * @brief Define NBIO PCIe Config default data
 */

#include <SilCommon.h>
#include "NbioPcieComplexDataPhx.h"
#include "NbioCmn2RevPhx.h"


/**----------------------------------------------------------------------------------------*/
//
// Complex configuration for IOHC0
//
// NBIO PCIe default configuration Init.
// Each NBIO container has 2 IoHub controllers IOHC 0 and IOHC1. Each IOHC has
// 2 PCIe wrappers with 9 PCIe ports each, however IOHC0 for each NBIO container
// has an additional WAFL wrapper that has 4 PCIe ports. Below is the default init
// structure for IOHC0.
//

static const NBIO_PHX_COMPLEX_CONFIG ComplexData = {

  //Silicon
  {
    .Header = {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Silicon))
    },
    .SocketId = 0,                                             // SocketId
    .DieNumber = 0,                                            // DieNumber
    .RBIndex = 0,                                              // RBIndex
    .InstanceId = 0,                                           // InstanceId
    .Address = {0},                                            // Address
    .StartLane = PHX_GPP0_START_PHY_LANE,                      // StartLane
    .EndLane = PHX_GFX_END_PHY_LANE - PHX_GPP0_START_PHY_LANE, // EndLane
    .BusNumberLimit = 0                                        // Bus Number Limit
  },
  //Gpp0 Wrapper
  {
    .Header = {
      DESCRIPTOR_PCIE_WRAPPER,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Silicon)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR5) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper))
    },
    .WrapId = PHX_GPP0_WRAP_ID,
    .CxlCoreConfig = 0,
    .StartPhyLane = PHX_GPP0_START_PHY_LANE,
    .EndPhyLane = PHX_GPP0_END_PHY_LANE,
    .StartDxioLane = PHX_GPP0_START_PHY_LANE,
    .EndDxioLane = PHX_GPP0_END_PHY_LANE,
    .Features = {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    .MasterPll = GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Gfx Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Silicon)),
      0,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR11) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper))
    },
    .WrapId = PHX_GFX_WRAP_ID,
    .CxlCoreConfig = 0,
    .StartPhyLane = PHX_GFX_START_PHY_LANE,
    .EndPhyLane = PHX_GFX_END_PHY_LANE,
    .StartDxioLane = PHX_GFX_START_PHY_LANE,
    .EndDxioLane = PHX_GFX_END_PHY_LANE,
    .Features = {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    .MasterPll = GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP0 WRAPPER START-------------------------------------
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR5) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR4) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR5)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR5_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR5_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR5_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR5_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR5_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR5,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR4) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR3) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR4)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR4_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR4_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR4_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR4_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR4_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR4,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR3) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR2) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR3)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR3_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR3_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR3_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR3_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR3_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR3,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR2) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR1) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR2)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR2_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR2_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR2_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR2_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR2_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR2,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR1) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR0) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR1)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR1_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR1_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR1_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR1_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR1_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR1,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR0) - offsetof(NBIO_PHX_COMPLEX_CONFIG, Gpp0Wrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR11) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR0)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR0_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR0_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR0_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR0_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR0_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR0,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP0 WRAPPER END -------------------------------------
//------------------------------ GFX WRAPPER START-------------------------------------
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR11) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR10) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR11)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR11_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR11_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        .CoreId = PBR11_CORE_ID,                 // CoreID                   [0:3]
        .PortId = PBR11_PORT_ID,                 // PortID                   [4:7]
        .Address = {PBR11_PCI_ADDRESS},          // Address PCI_ADDRESS
        .PcieBridgeId = PBR11,                   // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR10) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR9) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR10)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR10_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR10_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        .CoreId = PBR10_CORE_ID,                 // CoreID                   [0:3]
        .PortId = PBR10_PORT_ID,                 // PortID                   [4:7]
        .Address = {PBR10_PCI_ADDRESS},          // Address PCI_ADDRESS
        .PcieBridgeId = PBR10,                   // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR9) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR8) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR9)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR9_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR9_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR9_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR9_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR9_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR9,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR8) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR7) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR8)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR8_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR8_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR8_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR8_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR8_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR8,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR7) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR6) - offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR7)),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR7_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR7_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR7_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR7_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR7_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR7,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      (uint16_t) (offsetof(NBIO_PHX_COMPLEX_CONFIG, PortPBR6) - offsetof(NBIO_PHX_COMPLEX_CONFIG, GfxWrapper)),
      0,
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      .Port = {
        .PortData = {0},                         // Port Data  PCIe_PORT_DATA
        .StartCoreLane = UNUSED_LANE_ID,         // Start Core Lane
        .EndCoreLane = UNUSED_LANE_ID,           // End Core Lane
        .NativeDevNumber = PBR6_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        .NativeFunNumber = PBR6_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        .CoreId = PBR6_CORE_ID,                  // CoreID                   [0:3]
        .PortId = PBR6_PORT_ID,                  // PortID                   [4:7]
        .Address = {PBR6_PCI_ADDRESS},           // Address PCI_ADDRESS
        .PcieBridgeId = PBR6,                    // PciBridgeId              [0:3]
        .IsBmcLocation = 0,                      // Unused                   [4:7]
        .LogicalBridgeId = PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GFX WRAPPER END -------------------------------------
};


/*----------------------------------------------------------------------------------------*/
/*
 * NbioGetPcieComplexSizePhx
 * @brief Function to return PCIe Complex Data size
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier
 *
 * @retval                          return IOHC data structure size
 */
uint32_t
NbioGetPcieComplexSizePhx (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  )
{
  uint32_t   ComplexSize;
  ComplexSize = 0;

  ComplexSize = sizeof (NBIO_PHX_COMPLEX_CONFIG);

  assert(ComplexSize != 0);
  return ComplexSize;
}

/*----------------------------------------------------------------------------------------*/
/*
 * NbioGetPcieComplexPointerPhx
 * @brief Function to return PCIe Complex Data Pointer
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier.
 *
 * @retval                          return IOHC0/1 default data initializer.
 */
void *
NbioGetPcieComplexPointerPhx (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  )
{
  void    *ComplexPtr = NULL;

  ComplexPtr = (void *)&ComplexData;

  assert(ComplexPtr != NULL);
  return ComplexPtr;
}
