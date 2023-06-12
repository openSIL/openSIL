/**
 * @file  NbioPcieComplexDataGenoa.c
 * @brief Define NBIO PCIe Config default data for.
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "NbioPcieComplexData.h"
#include <Apob.h>

/**----------------------------------------------------------------------------------------*/
//
// Complex configuration for IOHC0
//
// NBIO PCIe default configuration Init. Genoa has 2 NBIO containers per socket.
// Each NBIO container has 2 IoHub controllers IOHC 0 and IOHC1. Each IOHC has
// 2 PCIe wrappers with 9 PCIe ports each, however IOHC0 for each NBIO container
// has an addtional WAFL wrapper that has 4 PCIe ports. Below is the default init
// structure for IOHC0. Please refer to NBIO_IOHC0_PCIE_CONFIG structure for
// IOHC data structure and its member
//

static const NBIO_IOHC0_PCIE_CONFIG ComplexDataRSIOHC0 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    {0},  // Address
    GPP0_START_PHY_LANE,                           // StartLane
    GPP1_END_PHY_LANE - GPP0_START_PHY_LANE,       // EndLane
    0    // Bus Number Limit
  },
  //Gpp0 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Silicon),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR8) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper)
    },
    GPP0_WRAP_ID,
    0,
    GPP0_START_PHY_LANE,
    GPP0_END_PHY_LANE,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0, 0, 0, 0, 0, 0, 0, 0 //Features are initialized in PcieWrapperConfigDefaults
    },
    PCIE_MASTERPLL_A, //MasterPll

  },
  //Gpp1 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Silicon),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper)
    },
    GPP1_WRAP_ID,
    0,
    GPP1_START_PHY_LANE,
    GPP1_END_PHY_LANE,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0, 0, 0, 0, 0, 0, 0, 0 //Features are initialized in PcieWrapperConfigDefaults
    },
    PCIE_MASTERPLL_A, //MasterPll

  },
  //Wafl Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Silicon),
      0,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR21) - offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper)
    },
    WAFL_WRAP_ID,
    0,
    WAFL_START_PHY_LANE,
    WAFL_END_PHY_LANE,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0, 0, 0, 0, 0, 0, 0, 0 //Features are initialized in PcieWrapperConfigDefaults
    },
    PCIE_MASTERPLL_A, //MasterPll

  },
//------------------------------ GPP0 WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR8) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR7) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR8),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR8_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR8_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR8_CORE_ID,         // CoreID                   [0:3]
        PBR8_PORT_ID,         // PortID                   [4:7]
        {PBR8_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR8,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR7) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR6) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR7_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR7_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR7_CORE_ID,         // CoreID                   [0:3]
        PBR7_PORT_ID,         // PortID                   [4:7]
        {PBR7_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR7,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR6) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR5) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR6_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR6_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR6_CORE_ID,         // CoreID                   [0:3]
        PBR6_PORT_ID,         // PortID                   [4:7]
        {PBR6_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR6,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR5) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR4) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR5),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR5_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR5_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR5_CORE_ID,         // CoreID                   [0:3]
        PBR5_PORT_ID,         // PortID                   [4:7]
        {PBR5_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR5,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR4) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR3) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR4_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR4_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR4_CORE_ID,         // CoreID                   [0:3]
        PBR4_PORT_ID,         // PortID                   [4:7]
        {PBR4_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR4,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR3) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR2) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR3_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR3_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR3_CORE_ID,         // CoreID                   [0:3]
        PBR3_PORT_ID,         // PortID                   [4:7]
        {PBR3_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR3,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR2) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR1) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR2_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR2_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR2_CORE_ID,         // CoreID                   [0:3]
        PBR2_PORT_ID,         // PortID                   [4:7]
        {PBR2_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR2,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR1) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR0) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR1_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR1_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR1_CORE_ID,         // CoreID                   [0:3]
        PBR1_PORT_ID,         // PortID                   [4:7]
        {PBR1_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR1,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR0) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR0_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR0_CORE_ID,         // CoreID                   [0:3]
        PBR0_PORT_ID,         // PortID                   [4:7]
        {PBR0_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR0,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP0 WRAPPER END -------------------------------------
//------------------------------ GPP1 WRAPPER START-------------------------------------
  //Port PBR17
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR16) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR17),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR17_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR17_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR17_CORE_ID,        // CoreID                   [0:3]
        PBR17_PORT_ID,        // PortID                   [4:7]
        {PBR17_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR17,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR17_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR16
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR16) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR15) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR16),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR16_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR16_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR16_CORE_ID,        // CoreID                   [0:3]
        PBR16_PORT_ID,        // PortID                   [4:7]
        {PBR16_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR16,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR16_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR15
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR15) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR14) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR15),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR15_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR15_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR15_CORE_ID,        // CoreID                   [0:3]
        PBR15_PORT_ID,        // PortID                   [4:7]
        {PBR15_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR15,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR15_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR14
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR14) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR13) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR14),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR14_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR14_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR14_CORE_ID,        // CoreID                   [0:3]
        PBR14_PORT_ID,        // PortID                   [4:7]
        {PBR14_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR14,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR14_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR13
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR13) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR12) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR13),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR13_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR13_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR13_CORE_ID,        // CoreID                   [0:3]
        PBR13_PORT_ID,        // PortID                   [4:7]
        {PBR13_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR13,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR13_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR12
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR12) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR11) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR12),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR12_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR12_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR12_CORE_ID,        // CoreID                   [0:3]
        PBR12_PORT_ID,        // PortID                   [4:7]
        {PBR12_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR12,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR12_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR11) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR10) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR11),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR11_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR11_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR11_CORE_ID,        // CoreID                   [0:3]
        PBR11_PORT_ID,        // PortID                   [4:7]
        {PBR11_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR11,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR10) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR9) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR10),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR10_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR10_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR10_CORE_ID,        // CoreID                   [0:3]
        PBR10_PORT_ID,        // PortID                   [4:7]
        {PBR10_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR10,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR9) - offsetof (NBIO_IOHC0_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR21) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR9),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR9_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR9_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR9_CORE_ID,         // CoreID                   [0:3]
        PBR9_PORT_ID,         // PortID                   [4:7]
        {PBR9_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR9,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP1 WRAPPER END -------------------------------------
//------------------------------ WAFL WRAPPER START-------------------------------------
  //Port PBR21
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR21) - offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR20) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR21),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR21_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR21_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR21_CORE_ID,        // CoreID                   [0:3]
        PBR21_PORT_ID,        // PortID                   [4:7]
        {PBR21_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR21,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR21_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR20
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR20) - offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR19) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR20),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR20_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR20_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR20_CORE_ID,        // CoreID                   [0:3]
        PBR20_PORT_ID,        // PortID                   [4:7]
        {PBR20_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR20,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR20_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR19
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR19) - offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper),
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR18) - offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR19),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR19_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR19_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR19_CORE_ID,        // CoreID                   [0:3]
        PBR19_PORT_ID,        // PortID                   [4:7]
        {PBR19_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR19,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR19_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR18
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (NBIO_IOHC0_PCIE_CONFIG, PortPBR18) - offsetof (NBIO_IOHC0_PCIE_CONFIG, WaflWrapper),
      0,
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR18_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR18_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR18_CORE_ID,        // CoreID                   [0:3]
        PBR18_PORT_ID,        // PortID                   [4:7]
        {PBR18_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR18,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR18_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ WAFL WRAPPER END -------------------------------------
};

/**----------------------------------------------------------------------------------------*/
//
// Complex configuration for IOHC1
//
// NBIO PCIe default configuration Init. Genoa has 2 NBIO containers per socket.
// Each NBIO container has 2 IoHub controllers IOHC 0 and IOHC1. Each IOHC has
// 2 PCIe wrappers with 9 PCIe ports each, however IOHC0 for each NBIO container
// has an addtional WAFL wrapper that has 4 PCIe ports. Below is the default init
// structure for IOHC0. Please refer to NBIO_IOHC0_PCIE_CONFIG structure for
// IOHC data structure and its member
//

static const NBIO_IOHC1_PCIE_CONFIG ComplexDataRSIOHC1 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    {0},    // Address
    GPP0_START_PHY_LANE,                           // StartLane
    GPP1_END_PHY_LANE - GPP0_START_PHY_LANE,       // EndLane
    0    // Bus Number Limit
  },
  //Gpp0 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Silicon),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR8) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper)
    },
    GPP0_WRAP_ID,
    0,
    GPP0_START_PHY_LANE,
    GPP0_END_PHY_LANE,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0, 0, 0, 0, 0, 0, 0, 0 //Features are initialized in PcieWrapperConfigDefaults
    },
    PCIE_MASTERPLL_A //MasterPll
  },
  //Gpp1 Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Silicon),
      0,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper)
    },
    GPP1_WRAP_ID,
    0,
    GPP1_START_PHY_LANE,
    GPP1_END_PHY_LANE,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0, 0, 0, 0, 0, 0, 0, 0 //Features are initialized in PcieWrapperConfigDefaults
    },
    PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP0 WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR8) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR7) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR8),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR8_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR8_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR8_CORE_ID,         // CoreID                   [0:3]
        PBR8_PORT_ID,         // PortID                   [4:7]
        {PBR8_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR8,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR7) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR6) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR7_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR7_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR7_CORE_ID,         // CoreID                   [0:3]
        PBR7_PORT_ID,         // PortID                   [4:7]
        {PBR7_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR8,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR6) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR5) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR6_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR6_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR6_CORE_ID,         // CoreID                   [0:3]
        PBR6_PORT_ID,         // PortID                   [4:7]
        {PBR6_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR6,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR5) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR4) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR5),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR5_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR5_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR5_CORE_ID,         // CoreID                   [0:3]
        PBR5_PORT_ID,         // PortID                   [4:7]
        {PBR5_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR5,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR4) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR3) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR4_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR4_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR4_CORE_ID,         // CoreID                   [0:3]
        PBR4_PORT_ID,         // PortID                   [4:7]
        {PBR4_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR4,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR3) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR2) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR3_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR3_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR3_CORE_ID,         // CoreID                   [0:3]
        PBR3_PORT_ID,         // PortID                   [4:7]
        {PBR3_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR3,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR2) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR1) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR2_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR2_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR2_CORE_ID,         // CoreID                   [0:3]
        PBR2_PORT_ID,         // PortID                   [4:7]
        {PBR2_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR2,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR1) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR0) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR1_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR1_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR1_CORE_ID,         // CoreID                   [0:3]
        PBR1_PORT_ID,         // PortID                   [4:7]
        {PBR1_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR1,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR0) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp0Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR0_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR0_CORE_ID,         // CoreID                   [0:3]
        PBR0_PORT_ID,         // PortID                   [4:7]
        {PBR0_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR0,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP0 WRAPPER END -------------------------------------
//------------------------------ GPP1 WRAPPER START-------------------------------------
  //Port PBR17
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR17) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR16) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR17),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR17_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR17_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR17_CORE_ID,        // CoreID                   [0:3]
        PBR17_PORT_ID,        // PortID                   [4:7]
        {PBR17_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR17,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR17_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR16
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR16) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR15) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR16),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR16_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR16_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR16_CORE_ID,        // CoreID                   [0:3]
        PBR16_PORT_ID,        // PortID                   [4:7]
        {PBR16_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR16,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR16_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR15
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR15) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR14) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR15),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR15_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR15_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR15_CORE_ID,        // CoreID                   [0:3]
        PBR15_PORT_ID,        // PortID                   [4:7]
        {PBR15_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR15,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR15_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR14
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR14) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR13) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR14),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR14_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR14_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR14_CORE_ID,        // CoreID                   [0:3]
        PBR14_PORT_ID,        // PortID                   [4:7]
        {PBR14_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR14,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR14_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR13
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR13) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR12) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR13),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR13_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR13_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR13_CORE_ID,        // CoreID                   [0:3]
        PBR13_PORT_ID,        // PortID                   [4:7]
        {PBR13_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR13,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR13_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR12
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR12) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR11) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR12),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR12_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR12_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR12_CORE_ID,        // CoreID                   [0:3]
        PBR12_PORT_ID,        // PortID                   [4:7]
        {PBR12_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR12,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR12_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR11) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR10) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR11),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR11_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR11_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR11_CORE_ID,        // CoreID                   [0:3]
        PBR11_PORT_ID,        // PortID                   [4:7]
        {PBR11_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR11,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR10) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR9) - offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR10),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR10_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR10_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR10_CORE_ID,        // CoreID                   [0:3]
        PBR10_PORT_ID,        // PortID                   [4:7]
        {PBR10_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR10,                // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof (NBIO_IOHC1_PCIE_CONFIG, PortPBR9) - offsetof (NBIO_IOHC1_PCIE_CONFIG, Gpp1Wrapper),
      0,
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                  // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,       // Start Core Lane
        UNUSED_LANE_ID,       // End Core Lane
        PBR9_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR9_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR9_CORE_ID,         // CoreID                   [0:3]
        PBR9_PORT_ID,         // PortID                   [4:7]
        {PBR9_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR9,                 // PciBridgeId              [0:3]
        0,                    // Unused                   [4:7]
        PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP1 WRAPPER END -------------------------------------
};

/*----------------------------------------------------------------------------------------*/
/*
 * GetPcieComplexSizeRS
 * @brief Function to return PCIe Complex Data size for Genoa
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier
 *
 * @retval                          return IOHC data structure size
 */
uint32_t
GetPcieComplexSizeRS (
  uint32_t   SocketIndex,
  uint32_t   RootBridgeIndex
  )
{
  uint32_t   ComplexSize;
  ComplexSize = 0;

  if (SocketIndex < MAX_SOCKETS_SUPPORTED) {
    /* Check if IOCH0 on NBIO 0/1 */
    if ((RootBridgeIndex & 0x1) == 0) {
      ComplexSize = sizeof (NBIO_IOHC0_PCIE_CONFIG);
    } else {
      ComplexSize = sizeof (NBIO_IOHC1_PCIE_CONFIG);
    }
  }
  assert(ComplexSize != 0);

  return ComplexSize;
}

/*----------------------------------------------------------------------------------------*/
/*
 * GetPcieComplexPointerRS
 * @brief Function to return PCIe Complex Data Pointer for Genoa
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier.
 *
 * @retval                          return IOHC0/1 default data initilalizer.
 */
void* GetPcieComplexPointerRS (
  uint32_t   SocketIndex,
  uint32_t   RootBridgeIndex
  )
{
  void    *ComplexPtr = NULL;

  if (SocketIndex < MAX_SOCKETS_SUPPORTED) {
    /* Check if IOCH0 on NBIO 0/1 */
    if ((RootBridgeIndex & 0x1) == 0) {
      ComplexPtr = (void *)&ComplexDataRSIOHC0;
    } else {
      ComplexPtr = (void *)&ComplexDataRSIOHC1;
    }
  }
  assert(ComplexPtr != NULL);
  return ComplexPtr;
}
