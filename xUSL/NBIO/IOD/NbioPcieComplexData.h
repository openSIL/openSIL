/**
 * @file  NbioPcieComplexData.h
 * @brief Define NBIO PCIe complex initializers for NBIO
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilPcie.h>
#include <NBIO/GnbDxio.h>

#ifndef NON_INITIALIZED_PCI_ADDRESS
#define NON_INITIALIZED_PCI_ADDRESS  0
#endif

#define NUMBER_OF_GPP0_PORTS       9
#define NUMBER_OF_GPP1_PORTS       9
#define NUMBER_OF_WAFL_PORTS       4

#define GPP0_WRAP_ID               0
#define GPP0_START_PHY_LANE        0
#define GPP0_END_PHY_LANE          15
#define GPP0_CORE_ID               0

#define GPP1_WRAP_ID               1
#define GPP1_START_PHY_LANE        16
#define GPP1_END_PHY_LANE          31
#define GPP1_CORE_ID               1

#define WAFL_WRAP_ID               2
#define WAFL_START_PHY_LANE        128
#define WAFL_END_PHY_LANE          131
#define WAFL_CORE_ID               2

#define GPP0_UNIT_ID              16
#define GPP1_UNIT_ID              (GPP0_UNIT_ID + NUMBER_OF_GPP0_PORTS)
#define WAFL_UNIT_ID              0xF
#define WAFL_PORT_UNIT_ID         (GPP1_UNIT_ID + NUMBER_OF_GPP1_PORTS)


// PBR0
#define PBR0                      0
#define PBR0_NATIVE_PCI_DEV       1
#define PBR0_NATIVE_PCI_FUN       1
#define PBR0_CORE_ID              GPP0_CORE_ID
#define PBR0_PORT_ID              0
#define PBR0_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR0_UNIT_ID              0

// PBR1
#define PBR1                      1
#define PBR1_NATIVE_PCI_DEV       1
#define PBR1_NATIVE_PCI_FUN       2
#define PBR1_CORE_ID              GPP0_CORE_ID
#define PBR1_PORT_ID              1
#define PBR1_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR1_UNIT_ID              1

// PBR2
#define PBR2                      2
#define PBR2_NATIVE_PCI_DEV       1
#define PBR2_NATIVE_PCI_FUN       3
#define PBR2_CORE_ID              GPP0_CORE_ID
#define PBR2_PORT_ID              2
#define PBR2_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR2_UNIT_ID              2

// PBR3
#define PBR3                      3
#define PBR3_NATIVE_PCI_DEV       1
#define PBR3_NATIVE_PCI_FUN       4
#define PBR3_CORE_ID              GPP0_CORE_ID
#define PBR3_PORT_ID              3
#define PBR3_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR3_UNIT_ID              3

// PBR4
#define PBR4                      4
#define PBR4_NATIVE_PCI_DEV       1
#define PBR4_NATIVE_PCI_FUN       5
#define PBR4_CORE_ID              GPP0_CORE_ID
#define PBR4_PORT_ID              4
#define PBR4_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR4_UNIT_ID              4

// PBR5
#define PBR5                      5
#define PBR5_NATIVE_PCI_DEV       1
#define PBR5_NATIVE_PCI_FUN       6
#define PBR5_CORE_ID              GPP0_CORE_ID
#define PBR5_PORT_ID              5
#define PBR5_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR5_UNIT_ID              5

// PBR6
#define PBR6                      6
#define PBR6_NATIVE_PCI_DEV       1
#define PBR6_NATIVE_PCI_FUN       7
#define PBR6_CORE_ID              GPP0_CORE_ID
#define PBR6_PORT_ID              6
#define PBR6_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR6_UNIT_ID              6

// PBR7
#define PBR7                      7
#define PBR7_NATIVE_PCI_DEV       2
#define PBR7_NATIVE_PCI_FUN       1
#define PBR7_CORE_ID              GPP0_CORE_ID
#define PBR7_PORT_ID              7
#define PBR7_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR7_UNIT_ID              7

// PBR8
#define PBR8                      8
#define PBR8_NATIVE_PCI_DEV       2
#define PBR8_NATIVE_PCI_FUN       2
#define PBR8_CORE_ID              GPP0_CORE_ID
#define PBR8_PORT_ID              8
#define PBR8_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR8_UNIT_ID              8

// PBR9
#define PBR9                      9
#define PBR9_NATIVE_PCI_DEV       3
#define PBR9_NATIVE_PCI_FUN       1
#define PBR9_CORE_ID              GPP1_CORE_ID
#define PBR9_PORT_ID              0
#define PBR9_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR9_UNIT_ID              9

// PBR10
#define PBR10                      10
#define PBR10_NATIVE_PCI_DEV       3
#define PBR10_NATIVE_PCI_FUN       2
#define PBR10_CORE_ID              GPP1_CORE_ID
#define PBR10_PORT_ID              1
#define PBR10_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR10_UNIT_ID              10

// PBR11
#define PBR11                      11
#define PBR11_NATIVE_PCI_DEV       3
#define PBR11_NATIVE_PCI_FUN       3
#define PBR11_CORE_ID              GPP1_CORE_ID
#define PBR11_PORT_ID              2
#define PBR11_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR11_UNIT_ID              11

// PBR12
#define PBR12                      12
#define PBR12_NATIVE_PCI_DEV       3
#define PBR12_NATIVE_PCI_FUN       4
#define PBR12_CORE_ID              GPP1_CORE_ID
#define PBR12_PORT_ID              3
#define PBR12_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR12_UNIT_ID              12

// PBR13
#define PBR13                      13
#define PBR13_NATIVE_PCI_DEV       3
#define PBR13_NATIVE_PCI_FUN       5
#define PBR13_CORE_ID              GPP1_CORE_ID
#define PBR13_PORT_ID              4
#define PBR13_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR13_UNIT_ID              13

// PBR14
#define PBR14                      14
#define PBR14_NATIVE_PCI_DEV       3
#define PBR14_NATIVE_PCI_FUN       6
#define PBR14_CORE_ID              GPP1_CORE_ID
#define PBR14_PORT_ID              5
#define PBR14_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR14_UNIT_ID              14

// PBR15
#define PBR15                      15
#define PBR15_NATIVE_PCI_DEV       3
#define PBR15_NATIVE_PCI_FUN       7
#define PBR15_CORE_ID              GPP1_CORE_ID
#define PBR15_PORT_ID              6
#define PBR15_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR15_UNIT_ID              15

// PBR16
#define PBR16                      16
#define PBR16_NATIVE_PCI_DEV       4
#define PBR16_NATIVE_PCI_FUN       1
#define PBR16_CORE_ID              GPP1_CORE_ID
#define PBR16_PORT_ID              7
#define PBR16_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR16_UNIT_ID              16

// PBR17
#define PBR17                      17
#define PBR17_NATIVE_PCI_DEV       4
#define PBR17_NATIVE_PCI_FUN       2
#define PBR17_CORE_ID              GPP1_CORE_ID
#define PBR17_PORT_ID              8
#define PBR17_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR17_UNIT_ID              17

// PBR18
#define PBR18                      18
#define PBR18_NATIVE_PCI_DEV       5
#define PBR18_NATIVE_PCI_FUN       1
#define PBR18_CORE_ID              WAFL_CORE_ID
#define PBR18_PORT_ID              0
#define PBR18_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR18_UNIT_ID              18

// PBR19
#define PBR19                      19
#define PBR19_NATIVE_PCI_DEV       5
#define PBR19_NATIVE_PCI_FUN       2
#define PBR19_CORE_ID              WAFL_CORE_ID
#define PBR19_PORT_ID              1
#define PBR19_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR19_UNIT_ID              19

// PBR20
#define PBR20                      20
#define PBR20_NATIVE_PCI_DEV       5
#define PBR20_NATIVE_PCI_FUN       3
#define PBR20_CORE_ID              WAFL_CORE_ID
#define PBR20_PORT_ID              2
#define PBR20_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR20_UNIT_ID              20

// PBR21
#define PBR21                      21
#define PBR21_NATIVE_PCI_DEV       5
#define PBR21_NATIVE_PCI_FUN       4
#define PBR21_CORE_ID              WAFL_CORE_ID
#define PBR21_PORT_ID              3
#define PBR21_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR21_UNIT_ID              21

// Gnb PCIe Master PLL
#define PCIE_MASTERPLL_A   0xA
#define PCIE_MASTERPLL_B   0xB
#define PCIE_MASTERPLL_C   0xC
#define PCIE_MASTERPLL_D   0xD

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     Gpp0Wrapper;            ///< GPP0 Wrapper
  PCIe_WRAPPER_CONFIG     Gpp1Wrapper;            ///< GPP1 Wrapper
  PCIe_WRAPPER_CONFIG     WaflWrapper;            ///< WAFL Wrapper
  // GPP0
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // GPP1
  PCIe_ENGINE_CONFIG      PortPBR17;              ///< Port PBR17
  PCIe_ENGINE_CONFIG      PortPBR16;              ///< Port PBR16
  PCIe_ENGINE_CONFIG      PortPBR15;              ///< Port PBR15
  PCIe_ENGINE_CONFIG      PortPBR14;              ///< Port PBR14
  PCIe_ENGINE_CONFIG      PortPBR13;              ///< Port PBR13
  PCIe_ENGINE_CONFIG      PortPBR12;              ///< Port PBR12
  PCIe_ENGINE_CONFIG      PortPBR11;              ///< Port PBR11
  PCIe_ENGINE_CONFIG      PortPBR10;              ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9
  // WAFL
  PCIe_ENGINE_CONFIG      PortPBR21;              ///< Port PBR21
  PCIe_ENGINE_CONFIG      PortPBR20;              ///< Port PBR20
  PCIe_ENGINE_CONFIG      PortPBR19;              ///< Port PBR19
  PCIe_ENGINE_CONFIG      PortPBR18;              ///< Port PBR18
} NBIO_IOHC0_PCIE_CONFIG;

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     Gpp0Wrapper;            ///< GPP0 Wrapper
  PCIe_WRAPPER_CONFIG     Gpp1Wrapper;            ///< GPP1 Wrapper
  // GPP0
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // GPP1
  PCIe_ENGINE_CONFIG      PortPBR17;              ///< Port PBR17
  PCIe_ENGINE_CONFIG      PortPBR16;              ///< Port PBR16
  PCIe_ENGINE_CONFIG      PortPBR15;              ///< Port PBR15
  PCIe_ENGINE_CONFIG      PortPBR14;              ///< Port PBR14
  PCIe_ENGINE_CONFIG      PortPBR13;              ///< Port PBR13
  PCIe_ENGINE_CONFIG      PortPBR12;              ///< Port PBR12
  PCIe_ENGINE_CONFIG      PortPBR11;              ///< Port PBR11
  PCIe_ENGINE_CONFIG      PortPBR10;              ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9

} NBIO_IOHC1_PCIE_CONFIG;

uint32_t GetPcieComplexSizeRS (uint32_t SocketIndex, uint32_t RootBridgeIndex);
void* GetPcieComplexPointerRS (uint32_t SocketIndex, uint32_t RootBridgeIndex);
