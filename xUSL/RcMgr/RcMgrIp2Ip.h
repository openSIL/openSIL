/**
 * @file  RcMgrIp2Ip.h
 * This file contains the definition of the Resource Manager Ip 2 Ip API.
 * This API provides other openSIL IPs with any RcMgr functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>

// current Protocol revision
#define AMD_FABRIC_RESOURCE_PROTOCOL_REV  0x02

/// FABRIC_TARGET.TgtType
#define TARGET_PCI_BUS             0
#define TARGET_RB                  1

/// Alignment
#define ALIGN_1M                   0xFFFFF
#define ALIGN_512K                 0x7FFFF
#define ALIGN_64K                  0x0FFFF
#define ALIGN_4K                   0xFFF

#define MMIO_BELOW_4G              0 // Non-Prefetchable MMIO
#define MMIO_ABOVE_4G              1 // Non-Prefetchable MMIO
#define P_MMIO_BELOW_4G            2 // Prefetchable MMIO
#define P_MMIO_ABOVE_4G            3 // Prefetchable MMIO
#define NON_PCI_DEVICE_BELOW_4G    4 // For non-discoverable devices, such as IO APIC, GPIO, MP0/1 mailbox, IOMMU...
#define NON_PCI_DEVICE_ABOVE_4G    5 // For non-discoverable devices, such as IO APIC, GPIO, MP0/1 mailbox, IOMMU...

/// DF target
typedef struct _FABRIC_TARGET {
  uint16_t  PciBusNum:8;        ///< PCI bus number
  uint16_t  SocketNum:2;        ///< Socket number
  uint16_t  RbNum:5;            ///< Root bridge number
  uint16_t  TgtType:1;          ///< Indicator target type
                                ///< 0 - TARGET_PCI_BUS - set up an MMIO region for the device on a certain PCI bus
                                ///< 1 - TARGET_RB      - set up an MMIO region for the device on a certain Socket
                                ///<                      or RootBridge
  uint16_t  PciSegNum;          ///< PCI segment number
} FABRIC_TARGET;

/// MMIO attribute
typedef struct _FABRIC_MMIO_ATTRIBUTE {
  uint8_t   ReadEnable:1;         ///< Indicator whether the range is readable
  uint8_t   WriteEnable:1;        ///< Indicator whether the range is writable
  uint8_t   NonPosted:1;          ///< Indicator whether the range is posted
  uint8_t   CpuDis:1;             ///< CPU core memory accesses with ReqIo=1 and address in this MMIO range are directed
                                  ///< to compatibility address space.
  uint8_t   :1;                   ///< Reserved
  uint8_t   MmioType:3;           ///< We have 6 pools per RootBridge:
                                  ///<  0) MMIO_BELOW_4G 1) MMIO_ABOVE_4G 2) P_MMIO_BELOW_4G 3) P_MMIO_ABOVE_4G
                                  ///<  4) NON_PCI_DEVICE_BELOW_4G 5) NON_PCI_DEVICE_ABOVE_4G
                                  ///< This function is restricted to support NON_PCI_DEVICE_BELOW_4G and
                                  ///< NON_PCI_DEVICE_ABOVE_4G only.
                                  ///< Other 4 pools are supported by FabricGetAvailableResource
} FABRIC_MMIO_ATTRIBUTE;

typedef SIL_STATUS (*FABRIC_RESERVE_MMIO) (
  uint64_t              *BaseAddress,
  uint64_t              *Length,
  uint64_t              Alignment,
  FABRIC_TARGET         Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  );


// Resource Manager Ip2Ip API

typedef struct {
  FABRIC_RESERVE_MMIO   FabricReserveMmio;
} RCMGR_IP2IP_API;
