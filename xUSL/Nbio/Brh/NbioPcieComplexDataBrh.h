/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPcieComplexDataBrh.h
 * @brief Define NBIO PCIe complex initializers for NBIO
 */


#pragma once

#include <SilPcie.h>
#include <Nbio/Common/GnbDxio.h>

/*
 * For each socket:
 * 6 IOHC without bonus controller
 * 2 IOHC with bonus controller
 */
#define MAX_NUM_NBIO_PCIE_CONFIG 6
#define SIL_RESERVED_1743 2

#define NBIO_PCIE_DATA_LENGTH (sizeof (NBIO_BRH_IOHC0_PCIE_CONFIG) * MAX_NUM_NBIO_PCIE_CONFIG + \
        sizeof (NBIO_BRH_IOHC1_PCIE_CONFIG) * SIL_RESERVED_1743) * PROJ_MAX_SOCKETS_SUPPORTED

#ifndef NON_INITIALIZED_PCI_ADDRESS
#define NON_INITIALIZED_PCI_ADDRESS  0
#endif

#define SIL_RESERVED_1744      8
#define SIL_RESERVED_1745     3

#define SIL_RESERVED_1738                0
#define SIL_RESERVED_1736         0
#define SIL_RESERVED_1735           15
#define SIL_RESERVED_1734                0

#define SIL_RESERVED_1733              1
#define SIL_RESERVED_1731       128
#define SIL_RESERVED_1729         135
#define SIL_RESERVED_1728              1

#define SIL_RESERVED_1753             2
#define SIL_RESERVED_1747     200
#define SIL_RESERVED_1746       203
#define SIL_RESERVED_1749     204
#define SIL_RESERVED_1748       206
#define SIL_RESERVED_1750             2

#define SIL_RESERVED_1739                  0
#define SIL_RESERVED_1740            1
#define SIL_RESERVED_1741 2
#define SIL_RESERVED_1742           3

#define SIL_RESERVED_1737               16
#define SIL_RESERVED_1732             0xF
#define SIL_RESERVED_1730        25
#define SIL_RESERVED_1752            0xF
#define SIL_RESERVED_1751       33

// PBR0
#define PBR0                      0
#define PBR0_NATIVE_PCI_DEV       1
#define PBR0_NATIVE_PCI_FUN       1
#define PBR0_CORE_ID              SIL_RESERVED_1734
#define PBR0_PORT_ID              0
#define PBR0_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR0_UNIT_ID              0

// PBR1
#define PBR1                      1
#define PBR1_NATIVE_PCI_DEV       1
#define PBR1_NATIVE_PCI_FUN       2
#define PBR1_CORE_ID              SIL_RESERVED_1734
#define PBR1_PORT_ID              1
#define PBR1_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR1_UNIT_ID              1

// PBR2
#define PBR2                      2
#define PBR2_NATIVE_PCI_DEV       1
#define PBR2_NATIVE_PCI_FUN       3
#define PBR2_CORE_ID              SIL_RESERVED_1734
#define PBR2_PORT_ID              2
#define PBR2_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR2_UNIT_ID              2

// PBR3
#define PBR3                      3
#define PBR3_NATIVE_PCI_DEV       1
#define PBR3_NATIVE_PCI_FUN       4
#define PBR3_CORE_ID              SIL_RESERVED_1734
#define PBR3_PORT_ID              3
#define PBR3_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR3_UNIT_ID              3

// PBR4
#define PBR4                      4
#define PBR4_NATIVE_PCI_DEV       1
#define PBR4_NATIVE_PCI_FUN       5
#define PBR4_CORE_ID              SIL_RESERVED_1734
#define PBR4_PORT_ID              4
#define PBR4_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR4_UNIT_ID              4

// PBR5
#define PBR5                      5
#define PBR5_NATIVE_PCI_DEV       1
#define PBR5_NATIVE_PCI_FUN       6
#define PBR5_CORE_ID              SIL_RESERVED_1734
#define PBR5_PORT_ID              5
#define PBR5_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR5_UNIT_ID              5

// PBR6
#define PBR6                      6
#define PBR6_NATIVE_PCI_DEV       1
#define PBR6_NATIVE_PCI_FUN       7
#define PBR6_CORE_ID              SIL_RESERVED_1734
#define PBR6_PORT_ID              6
#define PBR6_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR6_UNIT_ID              6

// PBR7
#define PBR7                      7
#define PBR7_NATIVE_PCI_DEV       2
#define PBR7_NATIVE_PCI_FUN       1
#define PBR7_CORE_ID              SIL_RESERVED_1734
#define PBR7_PORT_ID              7
#define PBR7_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR7_UNIT_ID              7

// PBR8
#define PBR8                      8
#define PBR8_NATIVE_PCI_DEV       2
#define PBR8_NATIVE_PCI_FUN       2
#define PBR8_CORE_ID              SIL_RESERVED_1734
#define PBR8_PORT_ID              8
#define PBR8_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR8_UNIT_ID              8

// PBR9
#define PBR9                      9
#define PBR9_NATIVE_PCI_DEV       3
#define PBR9_NATIVE_PCI_FUN       1
#define PBR9_CORE_ID              SIL_RESERVED_1728
#define PBR9_PORT_ID              0
#define PBR9_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR9_UNIT_ID              9

// PBR10
#define PBR10                      10
#define PBR10_NATIVE_PCI_DEV       3
#define PBR10_NATIVE_PCI_FUN       2
#define PBR10_CORE_ID              SIL_RESERVED_1728
#define PBR10_PORT_ID              1
#define PBR10_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR10_UNIT_ID              10

// PBR11
#define PBR11                      11
#define PBR11_NATIVE_PCI_DEV       3
#define PBR11_NATIVE_PCI_FUN       3
#define PBR11_CORE_ID              SIL_RESERVED_1728
#define PBR11_PORT_ID              2
#define PBR11_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR11_UNIT_ID              11

// PBR12
#define PBR12                      12
#define PBR12_NATIVE_PCI_DEV       3
#define PBR12_NATIVE_PCI_FUN       4
#define PBR12_CORE_ID              SIL_RESERVED_1728
#define PBR12_PORT_ID              3
#define PBR12_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR12_UNIT_ID              12

// PBR13
#define PBR13                      13
#define PBR13_NATIVE_PCI_DEV       3
#define PBR13_NATIVE_PCI_FUN       5
#define PBR13_CORE_ID              SIL_RESERVED_1728
#define PBR13_PORT_ID              4
#define PBR13_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR13_UNIT_ID              13

// PBR14
#define PBR14                      14
#define PBR14_NATIVE_PCI_DEV       3
#define PBR14_NATIVE_PCI_FUN       6
#define PBR14_CORE_ID              SIL_RESERVED_1728
#define PBR14_PORT_ID              5
#define PBR14_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR14_UNIT_ID              14

// PBR15
#define PBR15                      15
#define PBR15_NATIVE_PCI_DEV       3
#define PBR15_NATIVE_PCI_FUN       7
#define PBR15_CORE_ID              SIL_RESERVED_1728
#define PBR15_PORT_ID              6
#define PBR15_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR15_UNIT_ID              15

// PBR16
#define PBR16                      16
#define PBR16_NATIVE_PCI_DEV       4
#define PBR16_NATIVE_PCI_FUN       1
#define PBR16_CORE_ID              SIL_RESERVED_1728
#define PBR16_PORT_ID              7
#define PBR16_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR16_UNIT_ID              16

// PBR17
#define PBR17                      17
#define PBR17_NATIVE_PCI_DEV       5
#define PBR17_NATIVE_PCI_FUN       1
#define PBR17_CORE_ID              SIL_RESERVED_1750
#define PBR17_PORT_ID              0
#define PBR17_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR17_UNIT_ID              17

// PBR18
#define PBR18                      18
#define PBR18_NATIVE_PCI_DEV       5
#define PBR18_NATIVE_PCI_FUN       2
#define PBR18_CORE_ID              SIL_RESERVED_1750
#define PBR18_PORT_ID              1
#define PBR18_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR18_UNIT_ID              18

// PBR19
#define PBR19                      19
#define PBR19_NATIVE_PCI_DEV       5
#define PBR19_NATIVE_PCI_FUN       3
#define PBR19_CORE_ID              SIL_RESERVED_1750
#define PBR19_PORT_ID              2
#define PBR19_PCI_ADDRESS          NON_INITIALIZED_PCI_ADDRESS
#define PBR19_UNIT_ID              19



/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     GppWrapper;             ///< GPP Wrapper
  // GPP
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0

} NBIO_BRH_IOHC0_PCIE_CONFIG;

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     GppWrapper;             ///< GPP Wrapper
  PCIe_WRAPPER_CONFIG     BonusWrapper;           ///< BONUS Wrapper
  // GPP
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // BONUS
  PCIe_ENGINE_CONFIG      PortPBR16;              ///< Port PBR16
  PCIe_ENGINE_CONFIG      PortPBR15;              ///< Port PBR15
  PCIe_ENGINE_CONFIG      PortPBR14;              ///< Port PBR14
  PCIe_ENGINE_CONFIG      PortPBR13;              ///< Port PBR13
  PCIe_ENGINE_CONFIG      PortPBR12;              ///< Port PBR12
  PCIe_ENGINE_CONFIG      PortPBR11;              ///< Port PBR11
  PCIe_ENGINE_CONFIG      PortPBR10;              ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9

} NBIO_BRH_IOHC1_PCIE_CONFIG;

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     GppWrapper;             ///< GPP Wrapper
  PCIe_WRAPPER_CONFIG     TunnelWrapper;          ///< TUNNEL Wrapper
  // GPP
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // TUNNEL
  PCIe_ENGINE_CONFIG      PortPBR19;              ///< Port PBR19
  PCIe_ENGINE_CONFIG      PortPBR18;              ///< Port PBR18
  PCIe_ENGINE_CONFIG      PortPBR17;              ///< Port PBR17

} NBIO_BRH_IOHC2_PCIE_CONFIG;

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                ///< Silicon

  PCIe_WRAPPER_CONFIG     GppWrapper;             ///< GPP Wrapper
  PCIe_WRAPPER_CONFIG     BonusWrapper;           ///< BONUS Wrapper
  PCIe_WRAPPER_CONFIG     TunnelWrapper;          ///< TUNNEL Wrapper
  // GPP
  PCIe_ENGINE_CONFIG      PortPBR8;               ///< Port PBR8
  PCIe_ENGINE_CONFIG      PortPBR7;               ///< Port PBR7
  PCIe_ENGINE_CONFIG      PortPBR6;               ///< Port PBR6
  PCIe_ENGINE_CONFIG      PortPBR5;               ///< Port PBR5
  PCIe_ENGINE_CONFIG      PortPBR4;               ///< Port PBR4
  PCIe_ENGINE_CONFIG      PortPBR3;               ///< Port PBR3
  PCIe_ENGINE_CONFIG      PortPBR2;               ///< Port PBR2
  PCIe_ENGINE_CONFIG      PortPBR1;               ///< Port PBR1
  PCIe_ENGINE_CONFIG      PortPBR0;               ///< Port PBR0
  // BONUS
  PCIe_ENGINE_CONFIG      PortPBR16;              ///< Port PBR16
  PCIe_ENGINE_CONFIG      PortPBR15;              ///< Port PBR15
  PCIe_ENGINE_CONFIG      PortPBR14;              ///< Port PBR14
  PCIe_ENGINE_CONFIG      PortPBR13;              ///< Port PBR13
  PCIe_ENGINE_CONFIG      PortPBR12;              ///< Port PBR12
  PCIe_ENGINE_CONFIG      PortPBR11;              ///< Port PBR11
  PCIe_ENGINE_CONFIG      PortPBR10;              ///< Port PBR10
  PCIe_ENGINE_CONFIG      PortPBR9;               ///< Port PBR9
  // TUNNEL
  PCIe_ENGINE_CONFIG      PortPBR19;             ///< Port PBR19
  PCIe_ENGINE_CONFIG      PortPBR18;             ///< Port PBR18
  PCIe_ENGINE_CONFIG      PortPBR17;             ///< Port PBR17

} NBIO_BRH_IOHC3_PCIE_CONFIG;
