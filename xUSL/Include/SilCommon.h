/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilCommon.h
 * @brief
 *  SIL Common declarations. This file is used to declare the non-API
 *  functions, variables and macros available to all of the xSIM and
 *  xPRF modules, but NOT intended for Host usage.
 *
 */

#pragma once

#include <stddef.h>
#include <stdint.h>   // needed for declarations later in this file
#include <stdbool.h>

#include <Sil-api.h>
#include <xSIM-api.h>

/* ***************************************************************************
 * Declare common variables here
 */

/**
 * UNUSED macro
 *
 * @brief This macro is used to explicitly avoid compilation errors due to
 *        unused input parameters.  This macro should be used in APIs that have
 *        a fixed signature but differ in implementations.
 *
 */
#define UNUSED(X) ((void)(X))

/**
 * SIL_ARRAY_SIZE macro
 *
 * @brief This macro is used to calculate the number of elements in an array.
 *
 */
#define SIL_ARRAY_SIZE(ARRAY)  (sizeof (ARRAY) / sizeof ((ARRAY)[0]))

/* ***************************************************************************
 *  openSIL Debug Services
 ****************************************************************************/

/** SIL_DEBUG_ENABLE
 * @brief Master SIL Debug enable
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

// Workaround stc_c missing asm when gnuc is included externally
#ifndef asm
    #define asm              __asm
#endif

#include <assert.h>

/* Debug output filtering by topic
 *   Each enabled topic (bit=1) will output debug info. Those with bit=0
 *   will be skipped. This will help focus the debug output to the area(s) \
 *   of interest and also speed up the boot time.
 */
// NOTE: We are unable to use the BIT_32 macro for these defines because the
//       C preprocessor is not able to understand the type casting when used
//       in #if directives
#define DEBUG_FILTER_APOB           0x00000001UL
#define DEBUG_FILTER_NBIO           0x00000002UL
#define DEBUG_FILTER_CCX            0x00000004UL
#define DEBUG_FILTER_SMU            0x00000008UL
#define DEBUG_FILTER_DF             0x00000010UL
#define DEBUG_FILTER_MPIO           0x00000020UL
#define DEBUG_FILTER_MEM            0x00000040UL
#define DEBUG_FILTER_FCH            0x00000080UL
#define DEBUG_FILTER_RAS            0x00000100UL
#define DEBUG_FILTER_CXL            0x00000200UL
#define SIL_RESERVED_0388           0x00000400UL
#define DEBUG_FILTER_RCMGR          0x00000800UL
#define SIL_RESERVED_0389   0x00001000UL
// set the default Module filter, allowing the Host to pre-define
#ifndef SIL_DEBUG_MODULE_FILTER
  #define SIL_DEBUG_MODULE_FILTER      ( DEBUG_FILTER_APOB | \
          DEBUG_FILTER_NBIO | \
          DEBUG_FILTER_CCX | \
          DEBUG_FILTER_SMU | \
          DEBUG_FILTER_DF | \
          DEBUG_FILTER_MPIO | \
          DEBUG_FILTER_MEM | \
          DEBUG_FILTER_FCH | \
          DEBUG_FILTER_RAS | \
          DEBUG_FILTER_CXL | \
          SIL_RESERVED_0388 | \
          DEBUG_FILTER_RCMGR )
#endif
extern HOST_DEBUG_SERVICE mHostDebugService;
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

void
SilTracePoint (
  size_t      SilMsgLevel,
  const char  *SilPrefix,
  const char  *Message,
  const char  *Function,
  size_t      Line,
  ...
  );

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
#define XSIM_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (SIL_DEBUG_ENABLE && mHostDebugService != NULL) { \
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xSIM:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)
#define XUSL_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (SIL_DEBUG_ENABLE && mHostDebugService != NULL) {			\
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xUSL:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)
#define XPRF_TRACEPOINT(MsgLevel, Message, ...) \
  do { \
    if (SIL_DEBUG_ENABLE && mHostDebugService != NULL) { \
      ((HOST_DEBUG_SERVICE)mHostDebugService) (MsgLevel, "openSIL:xPRF:", \
      Message, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
  } while (false)

/**
 * SIL Common Data structures
 *
 */

/** Descriptors of the Active SoC
 *
 *  Private data sub-structure for describing the Active SoC's
 *  characteristics. Describes the SoC in the socket.
 *
 *  The values in this table should be updated during openSIL data init.
 */
typedef struct {
  uint8_t              NumCcdsPerDie;  ///< Compute clusters on a die
  uint8_t              NumCcxPerCcd;   ///< Complexes per die
} ACTIVE_SOC_DATA;

/** Description of variables defining the platform
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
 *  for the SIL core.
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
 * SIL Common Services Prototypes
 *
 */
void
SilSetMemoryBase (
  SIL_BLOCK_VARIABLES  *BaseAddress
  );

SIL_BLOCK_VARIABLES *
SilGetMemoryBase (void);

uint32_t
xUslFindStructureSize (
  SIL_DATA_BLOCK_ID IpId,
  uint16_t          InstanceNum
  );

void *
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

#if __GNUC__ && defined (__x86_64__)
#define NASM_ABI __attribute__ ((ms_abi))
#else
#define NASM_ABI
#endif

/* Declare references for the module-global variables */
extern SIL_BLOCK_VARIABLES *mSilMemoryBase;
