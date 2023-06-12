/**
 * @file  FabricResourcemanager.h
 * @brief OpenSIL Fabric resource manager common definition
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include <DF/Common/SilFabricInfo.h>
#include <NBIO/PciStructs.h>
#include "RcMgrIp2Ip.h"
#include <Apob.h>

#define RCMGR_INIT_MAJOR_REV   0
#define RCMGR_INIT_MINOR_REV   1
#define RCMGR_INIT_INSTANCE    0

#define RCMGR_MAX_SOCKETS      2
#define RCMGR_MMIO_MIN_SIZE                    0x10000ul
#define RCMGR_NON_PCI_MMIO_ALIGN_MASK          0xFFFFull    // MMIO for non-PCI devices should be 16 bit aligned
#define RCMGR_NON_PCI_MMIO_ALIGN_MASK_ABOVE4G  0xFFFFFFFull // MMIO for non-PCI devices should be 256MB aligned
#define MMIO_MIN_NON_PCI_SIZE                  0x500000     // 5MB is the minimum size of NonPci MMIO pool
#define MMIO_MIN_NON_PCI_SIZE_ABOVE4G          0x20200000   // 514MB is the minimum size of NonPci MMIO pool
#define DF_IO_LIMIT                            0x2000000    // IO Limit

#define X86IO_LIMIT                0x10000    // X86 IO Limit
#define X86_LEGACY_IO_SIZE         0x1000     // IO size which is reserved for legacy devices
#define RCMGR_IO_SIZE_MASK         0xFFFFF000ull

#define SIZE_16M_ALIGN             0xFFFFFFFFFF000000ull

#define BOTTOM_OF_COMPAT           0xFEC00000ul // From BOTTOM_OF_COMPAT to 4G would be leaved as undescribed (COMPAT)

#define ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START 0xFED00000ul
#define ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END   0xFED0FFFFul

#pragma pack (push, 1)

typedef struct _FABRIC_ADDR_APERTURE {
  uint64_t  Base;
  uint64_t  Size;
  uint64_t  Alignment;            ///< Alignment bit map. 0xFFFFF means 1MB align
} FABRIC_ADDR_APERTURE;

#define MAX_HOST_BRIDGES (MAX_SOCKETS_SUPPORTED * MAX_RBS_PER_SOCKET)

/// Resource for each RootBridge
typedef struct _DFX_FABRIC_RESOURCE_FOR_EACH_RB {
  FABRIC_ADDR_APERTURE  NonPrefetchableMmioSizeAbove4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  FABRIC_ADDR_APERTURE  PrefetchableMmioSizeAbove4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  FABRIC_ADDR_APERTURE  NonPrefetchableMmioSizeBelow4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  FABRIC_ADDR_APERTURE  PrefetchableMmioSizeBelow4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  FABRIC_ADDR_APERTURE  PrimaryRbSecondNonPrefetchableMmioSizeBelow4G;
  FABRIC_ADDR_APERTURE  PrimaryRbSecondPrefetchableMmioSizeBelow4G;
  FABRIC_ADDR_APERTURE  IO[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  uint16_t              PciBusNumber[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
} DFX_FABRIC_RESOURCE_FOR_EACH_RB;

/// Current status
typedef struct _FABRIC_ADDR_SPACE_SIZE {
  uint32_t  IoSize;                ///< IO size required by system resources
  uint32_t  IoSizeReqInc;          ///< The amount needed over the current size
  uint32_t  MmioSizeBelow4G;       ///< Below 4G Mmio size required by system resources
  uint32_t  MmioSizeBelow4GReqInc; ///< The amount needed over the current size
  uint64_t  MmioSizeAbove4G;       ///< Above 4G Mmio size required by system resources
  uint64_t  MmioSizeAbove4GReqInc; ///< The amount needed over the current size
} FABRIC_ADDR_SPACE_SIZE;

/// MMIO Region
typedef struct _FABRIC_MMIO_REGION {
  uint64_t  BaseNonPci;            ///< Base address of non-discoverable devices
  uint64_t  SizeNonPci;            ///< Total size of non-discoverable devices
  uint64_t  UsedSizeNonPci;        ///< Already used size of non-discoverable devices
  uint64_t  BaseNonPrefetch;       ///< Base address of non prefetchable
  uint64_t  SizeNonPrefetch;       ///< Total size of non prefetchable
  uint64_t  UsedSizeNonPrefetch;   ///< Already used size of non prefetchable
  uint64_t  BasePrefetch;          ///< Base address of prefetchable
  uint64_t  SizePrefetch;          ///< Total size of prefetchable
  uint64_t  UsedSizePrefetch;      ///< Already used size of prefetchable
  uint64_t  AlignNonPrefetch;      ///< Alignment bit map. For example, 0xFFFFF means 1MB alignment
  uint64_t  AlignPrefetch;         ///< Alignment bit. For example, 0xFFFFF means 1MB alignment
} FABRIC_MMIO_REGION;

/// MMIO Manager

typedef struct _DFX_FABRIC_MMIO_MANAGER {
  FABRIC_MMIO_REGION    MmioRegionAbove4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  FABRIC_MMIO_REGION    MmioRegionBelow4G[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET];
  bool    AllocateMmioAbove4GOnThisRb[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET]; ///< Allocate MMIO above 4G?
  bool    AllocateMmioBelow4GOnThisRb[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET]; ///< Allocate MMIO below 4G?
  bool    PrimaryRbHas2ndMmioBelow4G;   ///< Indicate if RootBridge 0 has a 2nd MMIO which is below 4G
  bool    ReportToGcd;                  ///< TRUE - Need to report GCD if user call FabricAllocateMmio
  uint8_t PrimaryRb2ndMmioPairBelow4G;  ///< Since AllocateMmioOnThisRootBridge[i][j] is FALSE, use this one for
                                        ///< RootBridge0's 2nd MMIO, RootBridge02ndMmioPair = (i << 4) | j
  uint64_t  Reserved;                   ///< compatibility (ImageHandle)
} DFX_FABRIC_MMIO_MANAGER;

/// IO Region
typedef struct _FABRIC_IO_REGION {
  uint32_t  IoBase;                ///< IO base address
  uint32_t  IoSize;                ///< IO size
  uint32_t  IoUsed;                ///< IO used
} FABRIC_IO_REGION;

/// IO Manager
typedef struct _DFX_FABRIC_IO_MANAGER {
  bool             GlobalCtrl;                                          ///< FALSE - forbid FabricAllocateIo
  FABRIC_IO_REGION IoRegion[RCMGR_MAX_SOCKETS][DFX_MAX_RBS_PER_SOCKET]; ///< IO region for Skt[0~1], RB[0~3]
} DFX_FABRIC_IO_MANAGER;

void
SilGetPrimaryRb (
  uint32_t *SocketNum,
  uint32_t *RootBridgeNum
  );

SIL_STATUS
DfXRcMgrSetInputBlk ( void );

SIL_STATUS
InitializeRcMgrApi (void);

SIL_STATUS FabricReserveMmio (
  uint64_t              *BaseAddress,
  uint64_t              *Length,
  uint64_t              Alignment,
  FABRIC_TARGET         Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  );

#pragma pack (pop)
