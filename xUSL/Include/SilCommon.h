/**
 * @file  SilCommon.h
 * @brief
 *  openSIL Common declarations. This file is used to declare the non-API
 *  functions, variables and macros available to all of the xSIM and
 *  xPRF modules, but NOT intended for Host usage.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stddef.h>
#include <stdint.h>   // needed for declarations later in this file
#include <stdbool.h>

#include <xSIM-api.h>

/* Declare references for the module-global variables */
extern void *mSilMemoryBase;

/* ***************************************************************************
 * Declare common variables here
 */
#define BIT_64(X)    (uint64_t)(1ull << (X))
#define BIT_32(X)    (uint32_t)(1ul << (X))
#define BIT_16(X)    (uint16_t)(1 << (X))
#define BIT_8(X)     (uint8_t)(1 << (X))

/**
 * UNUSED macro
 *
 * @brief This macro is used to explicitly avoid compilation errors due to
 *        unused input parameters.  This macro should be used in APIs that have
 *        a fixed signature but differ in implementations.
 *
 */
#define UNUSED(X) ((void)(X))

/* ***************************************************************************
 *  openSIL Debug Services
 ****************************************************************************/

/** SIL_DEBUG_ENABLE
 * @brief Master openSIL Debug enable
 * @details This is the master enable switch for the openSIL debug capabilities.
 * The Host may define this value and pass it into the build. This definition
 * sets the program default if the Host does not provide a setting
 */
#ifndef SIL_DEBUG_ENABLE
    #define SIL_DEBUG_ENABLE              true
#endif
// Now include the environment assert declaration
//   Wikipedia says the ISO C standard disablement is via defining 'NDEBUG' macro
#if !SIL_DEBUG_ENABLE
    #define NDEBUG
#endif
#include <assert.h>

/* Debug output filtering by topic
 *   Each enabled topic (bit=1) will output debug info. Those with bit=0
 *   will be skipped. This will help focus the debug ouput to the area(s) \
 *   of interest and also speed up the boot time.
 */
// NOTE: We are unable to use the BIT_32 macro for these defines because the
//       C preprocesor is not able to understand the type casting when used
//       in #if directives
#define DEBUG_FILTER_APOB       0x00000001UL
#define DEBUG_FILTER_NBIO       0x00000002UL
#define DEBUG_FILTER_CCX        0x00000004UL
#define DEBUG_FILTER_SMU        0x00000008UL
#define DEBUG_FILTER_DF         0x00000010UL
#define DEBUG_FILTER_MPIO       0x00000020UL
#define DEBUG_FILTER_MEM        0x00000040UL
#define DEBUG_FILTER_FCH        0x00000080UL
#define DEBUG_FILTER_RAS        0x00000100UL
#define DEBUG_FILTER_CXL        0x00000200UL
#define DEBUG_FILTER_SDCI       0x00000400UL
// set the default Module filter, allowing the Host to pre-define
#ifndef SIL_DEBUG_MODULE_FILTER
  #define SIL_DEBUG_MODULE_FILTER      ( DEBUG_FILTER_APOB | \
                                         DEBUG_FILTER_NBIO | \
                                         DEBUG_FILTER_CCX  | \
                                         DEBUG_FILTER_SMU  | \
                                         DEBUG_FILTER_DF   | \
                                         DEBUG_FILTER_MPIO | \
                                         DEBUG_FILTER_MEM  | \
                                         DEBUG_FILTER_FCH  | \
                                         DEBUG_FILTER_RAS  | \
                                         DEBUG_FILTER_CXL  | \
                                         DEBUG_FILTER_SDCI )
#endif
/* The modules can here, or elsewhere, implement module-topic based debug output
 *  macros, for example:
  #
  #define APOB_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_APOB & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)
 */

/** Message type enables
 * @name Group: Trace Enables
 * @anchor Trace_Enables
 * These values set the level or importance of then information being passed
 * in the message. The most critical/important are lower in value.
 * @{
 */
#define SIL_TRACE_ERROR   BIT_32(0)   ///< Unexpected/unwanted (problematic) conditions
#define SIL_TRACE_WARNING BIT_32(1)   ///< Result is unexpected but not necessarily a problem
#define SIL_TRACE_ENTRY   BIT_32(2)   ///< Messages showing entry flow
#define SIL_TRACE_EXIT    BIT_32(3)   ///< Messages showing exit flow
#define SIL_TRACE_INFO    BIT_32(4)   ///< Discovery/Configuration information that is likely useful to any log reader
                                      ///< but is useful for debugging (loops/repetitive info, binary dumps)
#define SIL_TRACE_RAW     BIT_32(31)  ///< Used for printing raw data without openSIL prefixes
/** @} end group name Trace_Enables */
extern HOST_DEBUG_SERVICE mHostDebugService;


/*
 * Trace macros
 *
 * The macros use single execution do-while loops to prevent a double end
 * statement when using the TRACEPOINT MACROs in c code.  The MACRO calls are
 * ended with a semicolon to make it similar to other lines of c code.  Double
 * statement endings can have unintended consequences. For example, an if-else
 * statement without {} encapsulating the code.
 *
 */
#if SIL_DEBUG_ENABLE
#define XSIM_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (mHostDebugService != NULL) { \
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xSIM:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)
#define XUSL_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (mHostDebugService != NULL) { \
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xUSL:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)
#define XPRF_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (mHostDebugService != NULL) { \
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xPRF:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)
#else
#define XSIM_TRACEPOINT(MsgLevel, Message, ...)
#define XUSL_TRACEPOINT(MsgLevel, Message, ...)
#define XPRF_TRACEPOINT(MsgLevel, Message, ...)
#endif

/**
 * openSIL Common Data structures
 *
 */

/** Descriptors of the Active SoC
 *
 *  Private data sub-structure for describing the Active SoC's
 *  characteristics. Describes the SoC in the socket.
 */
typedef struct {
  uint16_t              NumCcdsPerDie;  ///< Compute clusters on a die
} ACTIVE_SOC_DATA;

/** Description of varibles defining the platorm
 *
*/
typedef struct {
  uint32_t              ApobBaseAddress; ///< location Platform places the APOB
  uint16_t              PlatNumSockets;  ///< motherboard socket count
  uint16_t              PlatTypeSockets; ///< motherboard socket name/type
} PLATFORM_DESC;

/** Block (private) variables for xSIM
 *
 *  These are variables held in the Host memory, dereferenced by
 *  the mSilMemoryBase global. Think of them as the input block
 *  for the openSIL core.
 *
 */
typedef struct {
  uint32_t                HostBlockSize;                      ///< Size of block allocated by the Host
  uint32_t                FreeSpaceOffset;                    ///< Offset to next area of free space
  uint32_t                FreeSpaceLeft;                      ///< tracking remaining free space
  ACTIVE_SOC_DATA         ActiveSoC;                          ///< Descriptors for SoC in the socket
  PLATFORM_DESC           PlatformData;                       ///< Descriptors for the platform
  uint64_t                Ip2IpApi[SilId_ListEnd];            ///< IP to IP API table.  This table provides IP
                                                              ///< abstraction between IPs.
  uint64_t                Common2RevXferTable[SilId_ListEnd]; ///< IP Common to Rev specific transfer (Xfer) table.
                                                              ///< This table provides abstraction IP version specific
                                                              ///< code from common code.
} SIL_BLOCK_VARIABLES;

/**
 * openSIL Common Services Prototypes
 *
 */
void
SilSetMemoryBase (
  void  *BaseAddress
  );

void*
SilGetMemoryBase (void);

void*
xUslFindStructure (
  SIL_DATA_BLOCK_ID IpId,
  uint16_t          InstanceNum
  );

SIL_STATUS
SilGetIp2IpApi (
  SIL_DATA_BLOCK_ID   IpId,
  void                **Api
  );

SIL_STATUS
SilInitIp2IpApi (
  SIL_DATA_BLOCK_ID   IpId,
  void                *IpApi
  );

SIL_STATUS
SilInitCommon2RevXferTable (
  SIL_DATA_BLOCK_ID   IpId,
  void                *XferTable
  );

SIL_STATUS
SilGetCommon2RevXferTable (
  SIL_DATA_BLOCK_ID   IpId,
  void                **XferTable
  );
