/**
 * @file  Nbio.h
 * @brief OpenSIL NBIO initialization prototypes
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilPcie.h>
#include <NBIO/GnbDxio.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <NBIO/IOD/NbioPcieComplexData.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define MAX_NUM_OF_ROOT_COMPLEXES_SUPPORTED 8
#define MAX_NUM_OF_CORES_PER_ROOT_COMPLEX   3
#define MAX_NUM_OF_PORTS_PER_ROOT_COMPLEX   22

#define MAX_NUM_NBIO_PCIE_CONFIG 6
#define MAX_NUM_NBIO_PCIE_CONFIG_2 2

#define NBIO_PCIE_DATA_LENGTH sizeof(NBIO_IOHC0_PCIE_CONFIG) * MAX_NUM_NBIO_PCIE_CONFIG + sizeof(NBIO_IOHC1_PCIE_CONFIG) * MAX_NUM_NBIO_PCIE_CONFIG_2

#define NBIO_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_NBIO & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)



/**********************************************************************************************************************
 * Declare common variables here
 *
 */
#pragma pack (push, 1)

/// Complex Configuration for silicon module
typedef struct {
  PCIe_SILICON_CONFIG     Silicon;                                        ///< Silicon
  PCIe_WRAPPER_CONFIG     Wrapper[MAX_NUM_OF_CORES_PER_ROOT_COMPLEX];       ///< PCIe Core Descriptors
  PCIe_ENGINE_CONFIG      Ports[MAX_NUM_OF_PORTS_PER_ROOT_COMPLEX];         ///< PCIe Port Descriptors
  uint8_t                 PortDevMap[MAX_NUM_OF_PORTS_PER_ROOT_COMPLEX];    ///< PortDevMap Allocation
} COMPLEX_CONFIG_MODEL;

typedef struct {
/// PCIe Information Block
  PCIe_PLATFORM_CONFIG        PciePlatformConfig;                      ///< Platform Config Structure
  //COMPLEX_CONFIG_MODEL        ComplexConfigs[MAX_NUM_OF_ROOT_COMPLEXES_SUPPORTED];    ///< Allocation for Max Complex Structure suported
  uint32_t                      ComplexConfigs;    ///< Allocation for Max Complex Structure suported
} NORTH_BRIDGE_PCIE_SIB;

#pragma pack (pop)

typedef PCIe_SILICON_CONFIG GNB_HANDLE;

/**
 * Declare function prototypes here
 *
 */
void *NbioGetPcieTopology (void);
void *NbioGetDataBlock (uint8_t Instance);
SIL_STATUS NbioClassSetInputBlk (void);
SIL_STATUS InitializeNbioTp1 (void);
SIL_STATUS InitializeNbioTp2 (void);
SIL_STATUS InitializeNbioTp3 (void);
