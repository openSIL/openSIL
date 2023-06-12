/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPcieComplexDataBrh.c
 * @brief Define NBIO PCIe Config default data
 */

#include <SilCommon.h>
#include "NbioPcieComplexDataBrh.h"
#include "NbioCmn2RevBrh.h"
#include <DF/DfIp2Ip.h>
#include <ProjSocConst.h>
#include <Nbio/Common/Nbio.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>


/**----------------------------------------------------------------------------------------*/
//
// BRH Complex configuration for IOHC0
//
// NBIO PCIe default configuration Init. Turin has 2 NBIO containers per socket.
// Each NBIO container has 2 IoHub controllers IOHC 0 and IOHC1. Each IOHC has
// 2 PCIe wrappers with 9 PCIe ports each, however IOHC0 for each NBIO container
// has an additional WAFL wrapper that has 4 PCIe ports. Below is the default init
// structure for IOHC0. Please refer to NBIO_BRH_IOHC0_PCIE_CONFIG structure for
// IOHC data structure and its member
//

static const NBIO_BRH_IOHC0_PCIE_CONFIG ComplexDataBRHIOHC0 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, Silicon)
    },
    0,                  // SocketId
    0,                  // DieNumber
    0,                  // RBIndex
    0,                  // InstanceId
    {0},                // Address
    SIL_RESERVED_1736,
    254,                // EndLane
    0                   // Bus Number Limit
  },
  //Gpp Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, Silicon),
      0,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper)
    },
    SIL_RESERVED_1738,
    0,
    SIL_RESERVED_1736,
    SIL_RESERVED_1735,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR8),
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
        PBR8,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR7),
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
        PBR7,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR6),
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
        PBR6,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR5),
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
        PBR5,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR4),
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
        PBR4,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR3),
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
        PBR3,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR2),
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
        PBR2,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR1),
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
        PBR1,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC0_PCIE_CONFIG, GppWrapper),
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
        PBR0_NATIVE_PCI_DEV,  // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN,  // NativeFunNumber          [5:7]
        PBR0_CORE_ID,         // CoreID                   [0:3]
        PBR0_PORT_ID,         // PortID                   [4:7]
        {PBR0_PCI_ADDRESS},   // Address PCI_ADDRESS
        PBR0,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP WRAPPER END -------------------------------------
};
/*----------------------------------------------------------------------------------------*/
//
// BRH Complex configuration for IOHC1
//

static const NBIO_BRH_IOHC1_PCIE_CONFIG ComplexDataBRHIOHC1 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, Silicon)
    },
    0,                  // SocketId
    0,                  // DieNumber
    0,                  // RBIndex
    0,                  // InstanceId
    {0},                // Address
    SIL_RESERVED_1736,
    254,                // EndLane
    0                   // Bus Number Limit
  },
  //Gpp Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, Silicon),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper)
    },
    SIL_RESERVED_1738,
    0,
    SIL_RESERVED_1736,
    SIL_RESERVED_1735,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Bonus Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, Silicon),
      0,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper)
    },
    SIL_RESERVED_1733,
    0,
    SIL_RESERVED_1731,
    SIL_RESERVED_1729,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR8),
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
        PBR8,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR7),
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
        PBR7,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR7_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR6),
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
        PBR6,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR6_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR5),
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
        PBR5,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR5_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR4),
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
        PBR4,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR4_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR3),
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
        PBR3,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR3_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR2),
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
        PBR2,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR2_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR1),
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
        PBR1,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR1_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR0),
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
        PBR0,                 // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR0_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP WRAPPER END -------------------------------------
//------------------------------ BONUS WRAPPER START-------------------------------------
  //Port PBR16
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR15) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR16),
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
        PBR16,                // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR16_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR15
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR15) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR14) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR15),
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
        PBR15,                // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR15_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR14
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR14) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR13) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR14),
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
        PBR14,                // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR14_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR13
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR13) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR12) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR13),
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
        PBR13,                // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR13_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR12
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR12) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR11) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR12),
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
        PBR12,                // PciBridgeId              [0:6]
        0,                    //Reserved                  [7]
        PBR12_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR11) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR10) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR11),
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
        PBR11,                // PciBridgeId              [0:6]
        0,                    // Reserved                  [7]
        PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR10) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR9) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR10),
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
        PBR10,                // PciBridgeId              [0:6]
        0,                    // Reserved                  [7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, PortPBR9) - offsetof(NBIO_BRH_IOHC1_PCIE_CONFIG, BonusWrapper),
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
        PBR9,                 // PciBridgeId              [0:6]
        0,                    // Reserved                  [7]
        PBR9_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ BONUS WRAPPER END -------------------------------------
};
/*----------------------------------------------------------------------------------------*/
//
// Complex configuration
//

NBIO_BRH_IOHC2_PCIE_CONFIG ComplexDataBRHIOHC2 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    {0},  // Address
    0,    // StartLane
    254,  // EndLane
    0     // Bus Number Limit
  },
  //Gpp Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, Silicon),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper)
    },
    SIL_RESERVED_1738,
    0,
    SIL_RESERVED_1736,
    SIL_RESERVED_1735,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Tunnel Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, Silicon),
      0,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper)
    },
    SIL_RESERVED_1753,
    0,
    SIL_RESERVED_1749,
    SIL_RESERVED_1748,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR8),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR8_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR8_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR8_CORE_ID,        // CoreID                   [0:3]
        PBR8_PORT_ID,        // PortID                   [4:7]
        {PBR8_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR8,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR8_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR7_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR7_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR7_CORE_ID,        // CoreID                   [0:3]
        PBR7_PORT_ID,        // PortID                   [4:7]
        {PBR7_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR7,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR7_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR6_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR6_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR6_CORE_ID,        // CoreID                   [0:3]
        PBR6_PORT_ID,        // PortID                   [4:7]
        {PBR6_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR6,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR6_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR5),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR5_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR5_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR5_CORE_ID,        // CoreID                   [0:3]
        PBR5_PORT_ID,        // PortID                   [4:7]
        {PBR5_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR5,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR5_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR4_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR4_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR4_CORE_ID,        // CoreID                   [0:3]
        PBR4_PORT_ID,        // PortID                   [4:7]
        {PBR4_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR4,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR4_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR3_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR3_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR3_CORE_ID,        // CoreID                   [0:3]
        PBR3_PORT_ID,        // PortID                   [4:7]
        {PBR3_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR3,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR3_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR2_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR2_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR2_CORE_ID,        // CoreID                   [0:3]
        PBR2_PORT_ID,        // PortID                   [4:7]
        {PBR2_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR2,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR2_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR1_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR1_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR1_CORE_ID,        // CoreID                   [0:3]
        PBR1_PORT_ID,        // PortID                   [4:7]
        {PBR1_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR1,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR1_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR0_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR0_CORE_ID,        // CoreID                   [0:3]
        PBR0_PORT_ID,        // PortID                   [4:7]
        {PBR0_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR0,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR0_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP WRAPPER END -------------------------------------
//------------------------------ TUNNEL WRAPPER START-------------------------------------
  //Port PBR19
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR18) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR19),
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
        PBR19,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR19_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR18
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR18) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper),
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR17) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR18),
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
        PBR18,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR18_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR17
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, PortPBR17) - offsetof(NBIO_BRH_IOHC2_PCIE_CONFIG, TunnelWrapper),
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
        PBR17_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR17_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR17_CORE_ID,        // CoreID                   [0:3]
        PBR17_PORT_ID,        // PortID                   [4:7]
        {PBR17_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR17,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR17_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ TUNNEL WRAPPER END -------------------------------------
};
/*----------------------------------------------------------------------------------------*/
//
// Complex configuration
//

NBIO_BRH_IOHC3_PCIE_CONFIG ComplexDataBRHIOHC3 = {

  //Silicon
  {
    {//Header
      DESCRIPTOR_SILICON | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      0,
      0,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, Silicon)
    },
    0,    // SocketId
    0,    // DieNumber
    0,    // RBIndex
    0,    // InstanceId
    {0},  // Address
    0,    // StartLane
    254,  // EndLane
    0     // Bus Number Limit
  },
  //Gpp Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, Silicon),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper)
    },
    SIL_RESERVED_1738,
    0,
    SIL_RESERVED_1736,
    SIL_RESERVED_1735,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Bonus Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, Silicon),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper)
    },
    SIL_RESERVED_1733,
    0,
    SIL_RESERVED_1731,
    SIL_RESERVED_1729,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
  //Tunnel Wrapper
  {
    {
      DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, Silicon),
      0,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper)
    },
    SIL_RESERVED_1753,
    0,
    SIL_RESERVED_1747,
    SIL_RESERVED_1746,
    UNUSED_LANE_ID,
    UNUSED_LANE_ID,
    {
      0 //Features are initialized in PcieWrapperConfigDefaults
    },
    GNB_PCIE_MASTERPLL_A //MasterPll
  },
//------------------------------ GPP WRAPPER START-------------------------------------
  //Port PBR8
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR8) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR8),
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
        PBR8,                 // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR8_UNIT_ID,         // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR7
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR7) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR7),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR7_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR7_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR7_CORE_ID,        // CoreID                   [0:3]
        PBR7_PORT_ID,        // PortID                   [4:7]
        {PBR7_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR7,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR7_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR6
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR6) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR6),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR6_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR6_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR6_CORE_ID,        // CoreID                   [0:3]
        PBR6_PORT_ID,        // PortID                   [4:7]
        {PBR6_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR6,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR6_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR5
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR5) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR5),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR5_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR5_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR5_CORE_ID,        // CoreID                   [0:3]
        PBR5_PORT_ID,        // PortID                   [4:7]
        {PBR5_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR5,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR5_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR4
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR4) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR4),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR4_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR4_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR4_CORE_ID,        // CoreID                   [0:3]
        PBR4_PORT_ID,        // PortID                   [4:7]
        {PBR4_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR4,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR4_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR3
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR3) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR3),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR3_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR3_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR3_CORE_ID,        // CoreID                   [0:3]
        PBR3_PORT_ID,        // PortID                   [4:7]
        {PBR3_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR3,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR3_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR2
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR2) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR2),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR2_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR2_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR2_CORE_ID,        // CoreID                   [0:3]
        PBR2_PORT_ID,        // PortID                   [4:7]
        {PBR2_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR2,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR2_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR1
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR1) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR1),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR1_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR1_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR1_CORE_ID,        // CoreID                   [0:3]
        PBR1_PORT_ID,        // PortID                   [4:7]
        {PBR1_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR1,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR1_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR0
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR0) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, GppWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR0),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR0_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR0_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR0_CORE_ID,        // CoreID                   [0:3]
        PBR0_PORT_ID,        // PortID                   [4:7]
        {PBR0_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR0,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR0_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ GPP WRAPPER END -------------------------------------
//------------------------------ BONUS WRAPPER START-------------------------------------
  //Port PBR16
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR16) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR15) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR16),
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
        PBR16,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR16_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR15
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR15) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR14) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR15),
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
        PBR15,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR15_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR14
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR14) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR13) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR14),
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
        PBR14,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR14_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR13
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR13) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR12) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR13),
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
        PBR13,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR13_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR12
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR12) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR11) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR12),
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
        PBR12,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR12_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR11
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR11) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR10) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR11),
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
        PBR11,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR11_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR10
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR10) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR9) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR10),
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
        PBR10,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR10_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR9
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR9) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, BonusWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR9),
      0
    },
    { PciePortEngine, UNUSED_LANE_ID, UNUSED_LANE_ID},  // EngineData
    0,                                                  // Initialization Status
    0xFF,                                               // Scratch
    {
      {
        {0},                 // Port Data  PCIe_PORT_DATA
        UNUSED_LANE_ID,      // Start Core Lane
        UNUSED_LANE_ID,      // End Core Lane
        PBR9_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR9_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR9_CORE_ID,        // CoreID                   [0:3]
        PBR9_PORT_ID,        // PortID                   [4:7]
        {PBR9_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR9,                // PciBridgeId             [0:6]
        0,                   // Reserved        [7]
        PBR9_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ BONUS WRAPPER END -------------------------------------
//------------------------------ TUNNEL WRAPPER START-------------------------------------
  //Port PBR19
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR19) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR18) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR19),
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
        PBR19,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR19_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR18
  {
    {
      DESCRIPTOR_PCIE_ENGINE,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR18) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper),
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR17) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR18),
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
        PBR18,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR18_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
  //Port PBR17
  {
    {
      DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY,
      offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, PortPBR17) - offsetof(NBIO_BRH_IOHC3_PCIE_CONFIG, TunnelWrapper),
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
        PBR17_NATIVE_PCI_DEV, // NativeDevNumber          [0:4]
        PBR17_NATIVE_PCI_FUN, // NativeFunNumber          [5:7]
        PBR17_CORE_ID,        // CoreID                   [0:3]
        PBR17_PORT_ID,        // PortID                   [4:7]
        {PBR17_PCI_ADDRESS},  // Address PCI_ADDRESS
        PBR17,                // PciBridgeId             [0:6]
        0,                    // Reserved        [7]
        PBR17_UNIT_ID,        // LogicalBridgeId
        // Further values are initialized in PcieEngineConfigDefaults
      },
    },
  },
//------------------------------ TUNNEL WRAPPER END -------------------------------------
};

/*----------------------------------------------------------------------------------------*/
static
bool
IsAiPresent (void)
{
  uint32_t  Value;

  //Read AIED configuration register to determine if PCIe6 is present.
  Value = xUSLPciRead32(MAKE_SBDFO(0, 0, 24, 6, 0x410));
  if ((Value & 0xFF) != 0) {
    return true;
  }
  return false;
}

/*----------------------------------------------------------------------------------------*/
static
uint8_t
GetIohcConfig (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  )
{
  DF_IP2IP_API                     *DfIp2IpApi;
  uint32_t                         BusNumberBase;
  uint8_t                          RbValue;

  if (SilGetIp2IpApi(SilId_DfClass, (void **)(&DfIp2IpApi)) != SilPass) {
    return SilNotFound;
  }


  DfIp2IpApi->DfGetRootBridgeInfo(SocketIndex,
    DieIndex,
    RootBridgeIndex,
    NULL,
    &BusNumberBase,
    NULL,
    NULL,
    NULL,
    NULL
    );

  xUSLPciRead(MAKE_SBDFO((BusNumberBase >> 8),
    (BusNumberBase & 0xFF),
    0,
    0,
    PCICFG_OFFSET(SIL_RSVD_ADDR_1D400070)
    ),
    AccessWidth8,
    &RbValue
    );

  if (IsAiPresent ()) {
    switch (RbValue) {
    case 1:
      return SIL_RESERVED_1741;
    case 3:
      return SIL_RESERVED_1742;
    default:
      break;
    }
  } else {
    switch (RbValue) {
    case 1:
      return SIL_RESERVED_1740;
    default:
      break;
    }
  }
  return SIL_RESERVED_1739;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioGetPcieComplexSizeBrh
 * @brief Function to return PCIe Complex Data size for Banaras(Turin)
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param DieIndex                  The target root bridge's Die identifier.
 * @param RootBridgeIndex           The target root bridge identifier
 *
 * @retval                          return IOHC data structure size
 */
uint32_t
NbioGetPcieComplexSizeBrh (
  uint32_t                SocketIndex,
  uint32_t                DieIndex,
  uint32_t                RootBridgeIndex
  )
{
  uint32_t                ComplexSize;
  APOB_IP2IP_API          *ApobIp2IpApi;
  APOB_SOC_DIE_INFO       SocMaxDieInfo;
  SIL_STATUS              Status;

  ComplexSize = 0;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if (SocketIndex < SocMaxDieInfo.MaxSocSocketsSupportedValue) {
    /* Check if IOCH0 on NBIO 0/1 */
    switch (GetIohcConfig(SocketIndex, DieIndex, RootBridgeIndex)) {
    case SIL_RESERVED_1739:
      ComplexSize = sizeof ComplexDataBRHIOHC0;
      break;
    case SIL_RESERVED_1740:
      ComplexSize = sizeof ComplexDataBRHIOHC1;
      break;
    case SIL_RESERVED_1742:
      ComplexSize = sizeof ComplexDataBRHIOHC2;
      break;
    case SIL_RESERVED_1741:
      ComplexSize = sizeof ComplexDataBRHIOHC3;
      break;
    }
  }
  assert(ComplexSize != 0);

  return ComplexSize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioGetPcieComplexPointerBrh
 * @brief Function to return PCIe Complex Data Pointer for Banaras(Turin)
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param DieIndex                  The target root bridge's Die identifier.
 * @param RootBridgeIndex           The target root bridge identifier.
 *
 * @retval                          return IOHC0/1 default data initializer.
 */
void *
NbioGetPcieComplexPointerBrh (
  uint32_t               SocketIndex,
  uint32_t               DieIndex,
  uint32_t               RootBridgeIndex
  )
{
  void                   *ComplexPtr = NULL;
  APOB_IP2IP_API         *ApobIp2IpApi;
  APOB_SOC_DIE_INFO      SocMaxDieInfo;

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    assert(ComplexPtr == NULL);
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if (SocketIndex < SocMaxDieInfo.MaxSocSocketsSupportedValue) {
    /* Check if IOCH0 on NBIO 0/1 */
    switch (GetIohcConfig(SocketIndex, DieIndex, RootBridgeIndex)) {
    case SIL_RESERVED_1739:
      ComplexPtr = (void *) &ComplexDataBRHIOHC0;
      break;
    case SIL_RESERVED_1740:
      ComplexPtr = (void *) &ComplexDataBRHIOHC1;
      break;
    case SIL_RESERVED_1742:
      ComplexPtr = (void *) &ComplexDataBRHIOHC2;
      break;
    case SIL_RESERVED_1741:
      ComplexPtr = (void *) &ComplexDataBRHIOHC3;
      break;
    }
  }
  assert(ComplexPtr != NULL);
  return ComplexPtr;
}
