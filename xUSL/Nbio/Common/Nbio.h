/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Nbio.h
 * @brief OpenSIL NBIO initialization prototypes
 *
 */

#pragma once

#include <Nbio/NbioClass-api.h>
#include <SilPcie.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include "GnbDxio.h"


#define NBIOCLASS_MAJOR_REV       0
#define NBIOCLASS_MINOR_REV       1
#define NBIOCLASS_INSTANCE        0   ///< This Instance will hold NBIO configuration data
#define NBIOPCIECLASS_INSTANCE    1   ///< This instance will hold the PCIe Information Block

/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define NBIO_TABLE_PROPERTY_DEFAULT         0x00000000ul
#define NONPCI_BARSIZE_1MB                  0x100000ul              ///< Size of 1MB BAR
#define NONPCI_BARSIZE_512KB                0x7D000ul               ///< Size of 512KB BAR
#define NONPCI_BARSIZE_4KB                  0x1000                  ///< Size of 4KB BAR
#define SIZE_512KB                          0x00080000
#define SIZE_1KB                            0x00000400

/**********************************************************************************************************************
 * NBIO print function
 *
 */
#define NBIO_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_NBIO & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

/**********************************************************************************************************************
 * Declare common variables here
 *
 */

#pragma pack (push, 1)

typedef struct {
/// PCIe Information Block
  PCIe_PLATFORM_CONFIG          PciePlatformConfig; ///< Platform Config Structure
  uint32_t                      ComplexConfigs;     ///< Allocation for Max Complex Structure supported
} NORTH_BRIDGE_PCIE_SIB;

#pragma pack (pop)

/**
 * Declare function prototypes here
 *
 */

SIL_STATUS
NbioClassSetInputBlk (
  void
  );

SIL_STATUS
PcieComplexHidePcieCore (
  GNB_HANDLE *GnbHandle,
  uint32_t  WrapperNum
  );

SIL_STATUS
PcieConfigurationInit (
  void
  );

SIL_STATUS
NbioIoApicInit (
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  );

void
NonPciBarInit (
  GNB_HANDLE     *GnbHandle,
  uint32_t       MmioBarLow,
  uint32_t       MmioBarHigh,
  uint32_t       MemorySize,
  bool           Enable,
  bool           LockSettings,
  bool            Above4G
  );

void
NbioEnableNbifDevice (
  GNB_HANDLE  *GnbHandle,
  uint8_t     DeviceType
  );

void
NbioDisableNbifDevice (
  GNB_HANDLE  *GnbHandle,
  uint8_t     DeviceType
  );

SIL_STATUS
NbioIommuInit (
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  );

SIL_STATUS
NbioSetTopOfMemory (
  void
  );

void *
NbioGetPcieTopology (
  void
  );

NBIOCLASS_DATA_BLOCK *
GetNbioBlockData (
  void
  );

void
PcieEngineConfigDefaults (
  PCIe_ENGINE_CONFIG *PcieEngine,
  void *Buffer,
  PCIe_PLATFORM_CONFIG *Pcie
  );

SIL_STATUS
PcieWrapperConfigDefaults (
  PCIe_WRAPPER_CONFIG *Wrapper,
  void *Buffer,
  PCIe_PLATFORM_CONFIG *Pcie
  );

void
PcieConfigAttachDescriptors (
  uint32_t Type,
  PCIe_DESCRIPTOR_HEADER *Base,
  PCIe_DESCRIPTOR_HEADER *New
  );
