/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.  */
/**
 * @file  RcManager-api.h
 * @brief openSIL-Host Resource Manager IP interface
 *
 * @details The RCmgr API provides some controls for how to distribute the
 *  resources of the PCIe devices. Note: This is NOT the assignment of
 *  resources for the end-point devices ('PCIe device enumeration'). That is
 *  performed by the Host firmware. The RCmgr distributes the SoC available
 *  resources in groups to the root bridges in preparation for that later
 *  device enumeration.
 *
 *  This file provides the structures details for the Host to configure the
 *  base RCmanager operations.
 */
/*
 * Note: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */


/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_RCMGR   Resource Manager api
 *
 *   The Resource Manager (RCManager) handles the distribution of the MMIO
 *   and IO resources particularly to the PCI root bridges.
 *
 *   Input parameters reflect platform organization of the PCIe devices
 *   especially when there are multiple root bridges - showing a favor to one
 *   root bridge over another for distributing the resources.
 *
 *   See the 'Files - @ref RcManager-api.h' section of this document for
 *   further details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#pragma pack(push, 1)

#define RCMGR_MAX_SOCKETS               2   ///< Max number of sockets supported by DFX input block.
                                            ///< This value is defined by resource manager and does not represent
                                            ///< the value for any SOC.
#define RCMGR_MAX_RBS_PER_SOCKET        20  ///< Max number of root bridges per socket supported by DFX input block.
                                            ///< This value is defined by resource manager and does not represent
                                            ///< the value for any SOC.

typedef struct {
  uint64_t  Base;
  uint64_t  Size;
  uint64_t  Alignment;            ///< Alignment bit map. 0xFFFFF means 1MB align
} SIL_FABRIC_ADDR_APERTURE;

/// Resource for each RootBridge
typedef struct {
  SIL_FABRIC_ADDR_APERTURE  NonPrefetchableMmioSizeAbove4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  SIL_FABRIC_ADDR_APERTURE  PrefetchableMmioSizeAbove4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  SIL_FABRIC_ADDR_APERTURE  NonPrefetchableMmioSizeBelow4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  SIL_FABRIC_ADDR_APERTURE  PrefetchableMmioSizeBelow4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  SIL_FABRIC_ADDR_APERTURE  PrimaryRbSecondNonPrefetchableMmioSizeBelow4G;
  SIL_FABRIC_ADDR_APERTURE  PrimaryRbSecondPrefetchableMmioSizeBelow4G;
  SIL_FABRIC_ADDR_APERTURE  IO[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  uint16_t              PciBusNumber[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
} DFX_FABRIC_RESOURCE_FOR_EACH_RB;

/// MMIO Region
typedef struct {
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
} SIL_FABRIC_MMIO_REGION;

typedef struct {
  SIL_FABRIC_MMIO_REGION    MmioRegionAbove4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  SIL_FABRIC_MMIO_REGION    MmioRegionBelow4G[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET];
  bool    AllocateMmioAbove4GOnThisRb[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET]; ///< Allocate MMIO above 4G?
  bool    AllocateMmioBelow4GOnThisRb[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET]; ///< Allocate MMIO below 4G?
  bool    PrimaryRbHas2ndMmioBelow4G;                                               ///< Indicate if RootBridge 0 has a 2nd MMIO which is below 4G
  bool    ReportToGcd;                                                              ///< TRUE - Need to report GCD if user call FabricAllocateMmio
  uint8_t PrimaryRb2ndMmioPairBelow4G;                                              ///< Since AllocateMmioOnThisRootBridge[i][j] is FALSE, use this one for
                                                                                    ///< RootBridge0's 2nd MMIO, RootBridge02ndMmioPair = (i << 4) | j
  uint64_t  Reserved;                                                               ///< compatibility (ImageHandle)
} DFX_FABRIC_MMIO_MANAGER;

/// IO Region
typedef struct {
  uint32_t  IoBase;                ///< IO base address
  uint32_t  IoSize;                ///< IO size
  uint32_t  IoUsed;                ///< IO used
} SIL_FABRIC_IO_REGION;

typedef struct {
  bool             GlobalCtrl;                                                ///< FALSE - forbid FabricAllocateIo
  SIL_FABRIC_IO_REGION IoRegion[RCMGR_MAX_SOCKETS][RCMGR_MAX_RBS_PER_SOCKET]; ///< IO region for Skt[0~1], RB[0~3]
} DFX_FABRIC_IO_MANAGER;

typedef struct {
  DFX_FABRIC_IO_MANAGER   IoRcMgr;
  DFX_FABRIC_MMIO_MANAGER MmioRcMgr;
  bool      SetRcBasedOnNv;
  bool      SetRdOnNv;                                    ///< is Resource Distribution input valid?
  uint8_t   SocketNumber;
  uint8_t   RbsPerSocket;
  bool      McptEnable;                                   ///< RS
  uint64_t  PciExpressBaseAddress;
  uint32_t  BottomMmioReservedForPrimaryRb;
  uint64_t  MmioLimitAbovePcieCfg;                        ///< PCIe Config MMIO upper limit
  uint64_t  MmioLimitBelowPcieCfg;                        ///< PCIe Config MMIO lower limit
  uint32_t  MmioSizePerRbForNonPciDevice;
  uint64_t  MmioAbove4GLimit;
  uint32_t  Above4GMmioSizePerRbForNonPciDevice;
  uint8_t   BmcSocket;                                    ///< RS
  uint8_t   EarlyBmcLinkLaneNum;                          ///< RS
  bool      AmdSmee;
  uint8_t   AmdFabric1TbRemap;
  bool      ResourceDistributionNv[RCMGR_MAX_SOCKETS * RCMGR_MAX_RBS_PER_SOCKET];
  DFX_FABRIC_RESOURCE_FOR_EACH_RB ResourceSizeForEachRb;
} DFX_RCMGR_INPUT_BLK;

#pragma pack(pop)
