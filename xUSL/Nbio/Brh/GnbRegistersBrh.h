/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file GnbRegistersBrh.h
 *  @brief Top level header for NBIO Register definitions
 */

#pragma once

#define IOHC_INDEX(HANDLE)   (((HANDLE->RBIndex & 0x4) >> 2) | ((HANDLE->RBIndex & 0x1) << 1))

#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS + ((HANDLE->RBIndex & 0x3) << 20))
#endif

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(HANDLE, WRAPPER, \
            ADDRESS)   (ADDRESS + ((WRAPPER->WrapId == 0) ? (HANDLE->RBIndex << 20): (WRAPPER->WrapId << 23)))
#endif

#ifndef PORT_SPACE
  #define  PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, \
            ADDRESS)   (ADDRESS + ((WRAPPER->WrapId == \
          0) ? (HANDLE->RBIndex << 20): (WRAPPER->WrapId << 23)) + (PORTINDEX << 12))
#endif

#ifndef IOHC_BRIDGE_SPACE
  #define  IOHC_BRIDGE_SPACE(HANDLE, ENGINE, \
            ADDRESS)   (ADDRESS + ((HANDLE->RBIndex & 0x3) << 20) + (ENGINE->Type.Port.LogicalBridgeId << 10))
#endif

#ifndef IOHC_REMAP_SPACE
  #define  IOHC_REMAP_SPACE(HANDLE, ADDRESS, \
            PORTINDEX)   (ADDRESS + (HANDLE->RBIndex << 20) + (PORTINDEX << 2) + ((PORTINDEX < 20)? 0 : 0x280))
#endif

#ifndef IOHC_DEVIND_SPACE
  #define  IOHC_DEVIND_SPACE(HANDLE, PORTINDEX, \
            ADDRESS)   (ADDRESS + ((HANDLE->RBIndex & 0x3) << 20) + (PORTINDEX * 0x400))
#endif

/// CAUTION: This is very specific to SDPMUX registers that use a different address calculation
#ifndef SDPMUX_SPACE
  #define  SDPMUX_SPACE(HANDLE, \
            ADDRESS)   ((HANDLE->RBIndex == 0) ? ADDRESS : (ADDRESS + (1 << 23) + ((HANDLE->RBIndex) << 20)))
#endif

#ifndef PCICFG_OFFSET
  #define PCICFG_OFFSET(SmnAddr) (SmnAddr & 0xFFF)
#endif

#define CXL_CAPABLE_RB(Handle) ((Handle->RBIndex & 5) == 0) || ((Handle->RBIndex & 5) == 5)
