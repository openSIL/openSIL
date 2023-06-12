/**
 *  @file GnbRegisters.h
 *  @brief Top level header for NBIO Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include "include/IoagrReg.h"
#include "include/IoapicReg.h"
#include "include/IohcReg.h"
#include "include/Iommul1Reg.h"
#include "include/Iommul2Reg.h"
#include "include/Nbifepf0cfgReg.h"
#include "include/NbifepfncfgReg.h"
#include "include/NbifmmReg.h"
#include "include/NbifrccfgReg.h"
#include "include/NtbReg.h"
#include "include/PciecoreReg.h"
#include "include/PcierccfgReg.h"
#include "include/SdpmuxReg.h"
#include "include/SstReg.h"
#include "include/SyshubmmReg.h"
#include "include/NbioBaseReg.h"

#define  NBIF0_PORT0_ADDR(Handle)   MAKE_SBDFO(Handle->Address.Address.Segment, Handle->Address.Address.Bus, 7, 1, 0x0)
#define  NBIF0_PORT1_ADDR(Handle)   MAKE_SBDFO(Handle->Address.Address.Segment, Handle->Address.Address.Bus, 7, 2, 0x0)

#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20))
#endif

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(HANDLE, WRAPPER, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId << 22))
#endif

#ifndef PORT_SPACE
  #define  PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId << 22) + (PORTINDEX << 12))
#endif

#ifndef CXL_SPACE
  #define  CXL_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId << 22) + (PORTINDEX << 20))
#endif

#ifndef IOHC_PORT_SPACE
  #define  IOHC_PORT_SPACE(HANDLE, WRAPPER, PORTINDEX, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (WRAPPER->WrapId * 0x100) + (PORTINDEX * 0x20))
#endif

#ifndef IOHC_BRIDGE_SPACE
  #define  IOHC_BRIDGE_SPACE(HANDLE, ENGINE, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20) + (ENGINE->Type.Port.LogicalBridgeId << 10))
#endif

#ifndef IOHC_REMAP_SPACE
  #define  IOHC_REMAP_SPACE(HANDLE, ADDRESS, PORTINDEX)   (ADDRESS + (HANDLE->RBIndex << 20) + (PORTINDEX << 2) + ((PORTINDEX < 20)? 0 : 0x280))
#endif

/// CAUTION: This is very specific to SDPMUX registers that use a different address calculation
#ifndef SDPMUX_SPACE
  #define  SDPMUX_SPACE(HANDLE, ADDRESS)   ((HANDLE->RBIndex == 0) ? ADDRESS : (ADDRESS + (1 << 23) + ((HANDLE->RBIndex + 1) << 20)))
#endif

#ifndef PCICFG_OFFSET
  #define PCICFG_OFFSET(SmnAddr) (SmnAddr & 0xFFF)
#endif


#define SMN_MP0MP0_MP0_C2PMSG_97_ADDRESS    0x3810a84UL

// FASTSIM Definitions
//
#define   FASTSIM_ADDRESS         SMN_IOHUB0NBIO0_NBCFG_SCRATCH_4_ADDRESS
#define   MP0_C2PMSG_97_ADDRESS   SMN_MP0MP0_MP0_C2PMSG_97_ADDRESS



