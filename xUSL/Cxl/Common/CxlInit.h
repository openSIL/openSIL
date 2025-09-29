/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file CxlInit.h
 *  @brief All Cxl related defines and structures for MPIO
 */

#pragma once

#include <xSIM.h>
#include <SilCommon.h>
#include <Nbio/NbioIp2Ip.h>
#include <Cxl/CxlClass-api.h>


#ifndef CNLI_SPACE
  #define CNLI_SPACE(HANDLE, ENGINE, ADDRESS)  (ADDRESS + (HANDLE->RBIndex << 16) + (ENGINE->Type.Port.PortId << 13))
#endif


#define    MEM_32_BAR   0
#define    MEM_64_BAR   2

#define     GRANULARITY_32BIT   0x100000
#define     GRANULARITY_64BIT   0x1000000

#define     DVSEC_CAP            0x23

#define     DVSEC_VID_OFFSET     0x04
#define     DVSEC_ID_OFFSET      0x08

#define CXL_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_CXL & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)


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
  uint32_t unused    : 4;
} CXL_DEVICE_INFO;

#define     MAX_CXL_PORTS 16

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
  SIL_CONTEXT   *SilContext,
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
  SIL_CONTEXT           *SilContext,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

SIL_STATUS
CxlClassSetInputBlock (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeCxlTp1 (
  SIL_CONTEXT  *SilContext
  );

uint32_t
GetMem32BarSize (
  uint32_t     EndpointBar
  );
