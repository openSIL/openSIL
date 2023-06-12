/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file CxlInit.h
 *  @brief All Cxl related defines and structures for MPIO
 */

#pragma once

#include <xSIM.h>
#include <SilCommon.h>
#include <Nbio/NbioIp2Ip.h>
#include <Cxl/CxlClass-api.h>

#define SIL_RESERVED_0017    0x1a34045cUL

#define SIL_RESERVED_0013      1
#define PCIE_LC_ARBMUX_CNTL9_LC_TX_DIRECT_OUT_EN_MASK        0x2

#define PCICFG_SPACE_COMMAND_OFFSET               0x04
/**
 * @brief Primary bus number-legacy software compatibility
 * @details This register not used in PCIe functions but implemented as read write for legacy support.
 */
#define PCICFG_SPACE_PRIMARY_BUS_OFFSET           0x18
#define SIL_RESERVED_0012                0x08
/**
 * @brief Base Address register address register 0
 * @details
 */
#define PCICFG_SPACE_BAR0_OFFSET                  0x10

#define    MEM_32_BAR   0
#define    MEM_64_BAR   2

#define     ABOVE_4GIG    true
#define     BELOW_4GIG    false

#define     GRANULARITY_32BIT   0x100000
#define     GRANULARITY_64BIT   0x1000000
#define     MEM_BAR0_SIZE  0x10000

#define     DVSEC_CAP            0x23
#define     DVSEC_VID            0x8086
#define     DVSEC_VID2           0x1E98
#define     DVSEC_ID             0x00

#define     DVSEC_VID_OFFSET     0x04
#define     DVSEC_ID_OFFSET      0x08
#define     DVSEC_CXL_CAP_OFFSET 0x0A

#define     SIL_RESERVED_0019       (4 * 1024)
#define     SIL_RESERVED_0018   0x06040000

#define     SIL_RESERVED_0015    0x19f01000UL

// Bitfield Description : Controls if HwInit fields are read-only.
#define SIL_RESERVED_0016      1
#define RCEC_CFG_HWINIT_WR_LOCK_MASK        0x2

#define CXL_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_CXL & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)


typedef union {
  struct {
    uint32_t                            field_bits_0_to_7:8;
    uint32_t                            field_bits_8_to_15:8;
    uint32_t                            field_bits_16_to_23:8;
    uint32_t                            field_bits_24_to_31:8;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0034;

#define SIL_RESERVED_0014    0x19f00160UL

/// Cxl Device Info - Describes CXL type devices.
typedef struct {
  /*
   * This is the device info structure to describe CXL devices.
   * This information is used for persistent memory messaging support
   */
  uint32_t function  : 3;
  uint32_t device    : 5;
  uint32_t bus       : 8;
  uint32_t iohc_id   : 4; ///< IOHC0-3
  uint32_t pcie_port : 4; ///< Port0-3 (up to 4 per PCIe Controller/IOHC)
  uint32_t cxl_type  : 4; ///< Type1-3
  uint32_t cxl_ver   : 4; ///< 1=CXL1.1, 2=CXL2.0
} CXL_DEVICE_INFO;

#define     MAX_CXL_PORTS 16

/// Cxl Memory Resource - Container for CXL mem allocations.
typedef struct {
  uint32_t    MemorySize;
  size_t      MemoryBase;
} CXL_MEMORY_RESOURCE;

/// Cxl Bus Limits
typedef struct {
  uint8_t     BusLimits[MAX_NUMBER_OF_COMPLEXES];
} CXL_BUS_LIMITS;

/// Cxl Info List
typedef struct {
  CXL_DEVICE_INFO   CxlInfoS0[MAX_CXL_PORTS];
  CXL_DEVICE_INFO   CxlInfoS1[MAX_CXL_PORTS];
  uint8_t           CxlPortCountS0;
  uint8_t           CxlPortCountS1;
} CXL_INFO_LIST;

typedef union {
  struct {
    uint32_t             MemIo:1;
    uint32_t             MemSize:2;
    uint32_t             Prefetch:1;
    uint32_t             BaseAddress:28;
  } Field;
  uint32_t Value;
} PCIE_BAR_STRUCT;

/** @brief PCIe device type
 *  @details
 */
typedef enum {
  PcieDeviceEndPoint,                       ///< Endpoint (Type 00h Header)
  PcieDeviceLegacyEndPoint,                 ///< Legacy endpoint (Type 00h Header)
  PcieDeviceRootComplex = 4,                ///< Root complex (Type 01h Header)
  PcieDeviceUpstreamPort,                   ///< Upstream port (Type 01h Header)
  PcieDeviceDownstreamPort,                 ///< Downstream Port (Type 01h Header)
  PcieDevicePcieToPcix,                     ///< PCIe to PCI/PCIx bridge (Type 01h Header)
  PcieDevicePcixToPcie,                     ///< PCI/PCIx to PCIe bridge (Type 01h Header)
  PCieDeviceRCiEP,                          ///< CXL RCiEP (Type 00h Header)
  PcieDeviceRcec,                           ///< Root Complex Event Collector (Type 00h Header)
  PcieNotPcieDevice = 0xff                  ///< unknown device
} PCIE_DEVICE_TYPE;

typedef union {
  uint32_t    *RcrbPointer;
  uint32_t    field0;
} SIL_RESERVED_UNION_0041;

extern const CXLCLASS_DATA_BLK CxlClassDflts;

uint32_t
CxlRegisterRead (
  uint32_t *DsRcrb,
  uint32_t RegOffset
  );

uint16_t
CxlGetDvsec (
  uint32_t  Address,
  uint16_t  DvsecVendorId,
  uint16_t  DvsecVendorId2,
  uint16_t  DvsecId
  );

void
CxlRegisterWrite (
  uint32_t *DsRcrb,
  uint32_t RegOffset,
  uint32_t WriteValue
  );

SIL_STATUS
CxlMmioAlloc (
  GNB_HANDLE    *GnbHandle,
  uint32_t      *MmioBarLow,
  uint32_t      *MmioBarHigh,
  uint64_t      MemorySize,
  bool          Above4G
  );

void
AllocateForCxlIo (
  PCIe_ENGINE_CONFIG   *Engine,
  uint32_t             DeviceAddr
  );

void
CxlPortInit (
  PCIe_PLATFORM_CONFIG  *Pcie
  );

SIL_STATUS
CxlClassSetInputBlock (void);

SIL_STATUS
InitializeCxlTp1 (void);

uint32_t
GetMem32BarSize (
  uint32_t     EndpointBar
  );

PCIE_DEVICE_TYPE
SilGnbLibGetPcieDeviceType (
  PCI_ADDR       Device,
  void           *StdHeader
  );
