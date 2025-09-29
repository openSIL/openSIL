/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricResourceManager.h
 * @brief OpenSIL Fabric resource manager common definition
 */

#pragma once

#include <SilCommon.h>
#include <xSIM.h>
#include <Nbio/Common/PciStructs.h>
#include <RcMgr/RcMgrIp2Ip.h>

#define RCMGR_INIT_MAJOR_REV   0
#define RCMGR_INIT_MINOR_REV   1
#define RCMGR_INIT_INSTANCE    0

#define RCMGR_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_RCMGR & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

#define RCMGR_MMIO_MIN_SIZE                 0x10000ul
#define RCMGR_NON_PCI_MMIO_ALIGN_MASK       0xFFFFull       // MMIO for non-PCI devices should be 16 bit aligned
#define RCMGR_NON_PCI_MMIO_ALIGN_MASK_ABOVE4G  0xFFFFFFFull // MMIO for non-PCI devices should be 256MB aligned
#define MMIO_MIN_NON_PCI_SIZE_ABOVE4G       0x20200000      // 514MB is the minimum size of NonPci MMIO pool
#define DF_IO_LIMIT                         0x2000000       // IO Limit

#define X86IO_LIMIT                0x10000    // X86 IO Limit
#define X86_LEGACY_IO_SIZE         0x1000     // IO size which is reserved for legacy devices
#define RCMGR_IO_SIZE_MASK         0xFFFFF000ull

#define SIZE_16M_ALIGN             0xFFFFFFFFFF000000ull

#define BOTTOM_OF_COMPAT           0xFEC00000ul // From BOTTOM_OF_COMPAT to 4G would be leaved as undescribed (COMPAT)

#define ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_START 0xFED00000ul
#define ADDITIONAL_POSTED_REGION_UNDER_PRIMARY_RB_END   0xFED0FFFFul

#pragma pack (push, 1)

/// Current status
typedef struct _FABRIC_ADDR_SPACE_SIZE {
  uint32_t  IoSize;                ///< IO size required by system resources
  uint32_t  IoSizeReqInc;          ///< The amount needed over the current size
  uint32_t  MmioSizeBelow4G;       ///< Below 4G Mmio size required by system resources
  uint32_t  MmioSizeBelow4GReqInc; ///< The amount needed over the current size
  uint64_t  MmioSizeAbove4G;       ///< Above 4G Mmio size required by system resources
  uint64_t  MmioSizeAbove4GReqInc; ///< The amount needed over the current size
} FABRIC_ADDR_SPACE_SIZE;

void
SilGetPrimaryRb (
  SIL_CONTEXT   *SilContext,
  uint32_t      *SocketNum,
  uint32_t      *DieNum,
  uint32_t      *RootBridgeNum
  );

SIL_STATUS FabricReserveMmio (
  SIL_CONTEXT           *SilContext,
  uint64_t              *BaseAddress,
  uint64_t              *Length,
  uint64_t              Alignment,
  FABRIC_TARGET         Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  );

#pragma pack (pop)
