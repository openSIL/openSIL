/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CoreTechnologyService.h
 * @brief CoreTechnology service definition.
 *
 */

#pragma once

#include <APOB/Common/ApobCmn.h>

#define CORE_TOPOLOGY_V3_ITERATION_START   ((uint32_t)-1)

typedef enum {
  CORE_TOPOLOGY_V3_NEXT_SOCKET = 0,
  CORE_TOPOLOGY_V3_NEXT_DIE = 1,
  CORE_TOPOLOGY_V3_NEXT_CCD = 2,
  CORE_TOPOLOGY_V3_NEXT_COMPLEX = 3,
  CORE_TOPOLOGY_V3_NEXT_CORE = 4,
  CORE_TOPOLOGY_V3_NEXT_THREAD = 5,
  CORE_TOPOLOGY_V3_NO_NEXT_ITEM = 6,
} CORE_TOPOLOGY_ITERATION_RESULT;

SIL_STATUS
GetCcdCountOnDie (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      Socket,
  uint32_t                                      Die,
  uint32_t                                      *NumberOfCcds
  );

SIL_STATUS
GetComplexCountOnCcd (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     *NumberOfComplexes
  );

SIL_STATUS
GetCoreCountOnComplex (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     *NumberOfCores
  );

SIL_STATUS
GetThreadCountOnCore (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     Core,
  uint32_t                                     *NumberOfThreads
  );

SIL_STATUS
LogicalToPhysicalLocation (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t  *Socket,
  uint32_t  *Die,
  uint32_t  *Ccd,
  uint32_t  *Complex,
  uint32_t  *Core
  );

SIL_STATUS
GetCoreTopologyOnDieMax (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  *MaxNumberOfCcds,
  uint32_t  *MaxNumberOfComplexes,
  uint32_t  *MaxNumberOfCores,
  uint32_t  *MaxNumberOfThreads
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateSocket (
  uint32_t    *Socket
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateDie (
  uint32_t                        *Socket,
  uint32_t                        *Die
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCcd (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateComplex (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCore (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core
  );

CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateThread (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core,
  uint32_t                                      *Thread
  );


/*
 * MACROS for iterating core topology
 *
 */

/**
 * A macro to loop for all sockets.
 * Example:
 * CORE_TOPOLOGY_ITERATION_RESULT IterationResult;
 * uint32_t                       Socket;
 * CORE_TOPOLOGY_V3_FOR_EACH_SOCKET (IterationResult, Socket) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n");
 * }
 *
 * @param[out]    IterationResult             CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                      uint32_t type zero-based logical socket number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_SOCKET(IterationResult, Socket) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateSocket(&Socket); \
          IterationResult == CORE_TOPOLOGY_V3_NEXT_SOCKET; \
          IterationResult = CoreTopologyV3IterateSocket(&Socket) \
          )

/**
 * A macro to loop for all dies.
 * Example:
 * CORE_TOPOLOGY_ITERATION_RESULT IterationResult;
 * uint32_t                       Socket;
 * uint32_t                       Die;
 * CORE_TOPOLOGY_V3_FOR_EACH_DIE (IterationResult, Socket, Die) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n", Socket);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Die is %d\n", Die);
 * }
 *
 * @param[out]    IterationResult             CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                      uint32_t type zero-based logical socket number.
 * @param[in,out] Die                         uint32_t type zero-based logical die number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_DIE(IterationResult, Socket, Die) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        Die = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateDie(&Socket, &Die); \
          IterationResult <= CORE_TOPOLOGY_V3_NEXT_DIE; \
          IterationResult = CoreTopologyV3IterateDie(&Socket, &Die) \
          )

/**
 * A macro to loop for all CCDs.
 * Example:
 * APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *APOB
 * CORE_TOPOLOGY_ITERATION_RESULT               IterationResult;
 * uint32_t                                     Socket;
 * uint32_t                                     Die;
 * uint32_t                                     Ccd;
 * CORE_TOPOLOGY_V3_FOR_EACH_CCD (APOB, IterationResult, Socket, Die, Ccd) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n", Socket);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Die is %d\n", Die);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Ccd is %d\n", Ccd);
 * }
 *
 * @param[in]     APOB                  Pointer to the APOB CCD logical to physical map structure
 * @param[out]    IterationResult       CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                uint32_t type zero-based logical socket number.
 * @param[in,out] Die                   uint32_t type zero-based logical die number.
 * @param[in,out] Ccd                   uint32_t type zero-based logical CCD number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_CCD(APOB, IterationResult, Socket, Die, Ccd) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        Die = CORE_TOPOLOGY_V3_ITERATION_START; \
        Ccd = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateCcd(APOB, &Socket, &Die, &Ccd); \
          IterationResult <= CORE_TOPOLOGY_V3_NEXT_CCD; \
          IterationResult = CoreTopologyV3IterateCcd(APOB, &Socket, &Die, &Ccd) \
          )

/**
 * A macro to loop for all complexes.
 * Example:
 * APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *APOB
 * CORE_TOPOLOGY_ITERATION_RESULT               IterationResult;
 * uint32_t                                     Socket;
 * uint32_t                                     Die;
 * uint32_t                                     Ccd;
 * uint32_t                                     Complex;
 * CORE_TOPOLOGY_V3_FOR_EACH_COMPLEX (APOB, IterationResult, Socket, Die, Ccd, Complex) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n", Socket);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Die is %d\n", Die);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Ccd is %d\n", Ccd);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Complex is %d\n", Complex);
 * }
 *
 * @param[in]     APOB                  Pointer to the APOB CCD logical to physical map structure
 * @param[out]    IterationResult       CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                uint32_t type zero-based logical socket number.
 * @param[in,out] Die                   uint32_t type zero-based logical die number.
 * @param[in,out] Ccd                   uint32_t type zero-based logical CCD number.
 * @param[in,out] Complex               uint32_t type zero-based logical complex number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_COMPLEX(APOB, IterationResult, Socket, Die, Ccd, Complex) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        Die = CORE_TOPOLOGY_V3_ITERATION_START; \
        Ccd = CORE_TOPOLOGY_V3_ITERATION_START; \
        Complex = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateComplex(APOB, &Socket, &Die, &Ccd, &Complex); \
          IterationResult <= CORE_TOPOLOGY_V3_NEXT_COMPLEX; \
          IterationResult = CoreTopologyV3IterateComplex(APOB, &Socket, &Die, &Ccd, &Complex) \
          )

/**
 * A macro to loop for all cores.
 * Example:
 * APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *APOB
 * CORE_TOPOLOGY_ITERATION_RESULT               IterationResult;
 * uint32_t                                     Socket;
 * uint32_t                                     Die;
 * uint32_t                                     Ccd;
 * uint32_t                                     Complex;
 * uint32_t                                     Core;
 * CORE_TOPOLOGY_V3_FOR_EACH_CORE (APOB, IterationResult, Socket, Die, Ccd, Complex, Core) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n", Socket);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Die is %d\n", Die);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Ccd is %d\n", Ccd);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Complex is %d\n", Complex);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Core is %d\n", Core);
 * }
 *
 * @param[in]     APOB                  Pointer to the APOB CCD logical to physical map structure
 * @param[out]    IterationResult       CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                uint32_t type zero-based logical socket number.
 * @param[in,out] Die                   uint32_t type zero-based logical die number.
 * @param[in,out] Ccd                   uint32_t type zero-based logical CCD number.
 * @param[in,out] Complex               uint32_t type zero-based logical CCX number.
 * @param[in,out] Core                  uint32_t type zero-based logical core number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_CORE(APOB, IterationResult, Socket, Die, Ccd, Complex, Core) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        Die = CORE_TOPOLOGY_V3_ITERATION_START; \
        Ccd = CORE_TOPOLOGY_V3_ITERATION_START; \
        Complex = CORE_TOPOLOGY_V3_ITERATION_START; \
        Core = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateCore(APOB, &Socket, &Die, &Ccd, &Complex, &Core); \
          IterationResult <= CORE_TOPOLOGY_V3_NEXT_CORE; \
          IterationResult = CoreTopologyV3IterateCore(APOB, &Socket, &Die, &Ccd, &Complex, &Core) \
          )

/**
 * A macro to loop for all threads.
 * Example:
 * APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *APOB
 * CORE_TOPOLOGY_ITERATION_RESULT               IterationResult;
 * uint32_t                                     Socket;
 * uint32_t                                     Die;
 * uint32_t                                     Ccd;
 * uint32_t                                     Complex;
 * uint32_t                                     Core;
 * uint32_t                                     Thread;
 * CORE_TOPOLOGY_V3_FOR_EACH_THREAD (APOB, IterationResult, Socket, Die, Ccd, Complex, Core, Thread) {
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Socket is %d\n", Socket);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Die is %d\n", Die);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Ccd is %d\n", Ccd);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Complex is %d\n", Complex);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Core is %d\n", Core);
 *   XUSL_TRACEPOINT (SIL_TRACE_INFO, "Thread is %d\n", Thread);
 * }
 *
 * @param[in]     APOB                  Pointer to the APOB CCD logical to physical map structure
 * @param[out]    IterationResult       CORE_TOPOLOGY_ITERATION_RESULT type for the next item type.
 * @param[in,out] Socket                uint32_t type zero-based logical socket number.
 * @param[in,out] Die                   uint32_t type zero-based logical die number.
 * @param[in,out] Ccd                   uint32_t type zero-based logical CCD number.
 * @param[in,out] Complex               uint32_t type zero-based logical CCX number.
 * @param[in,out] Core                  uint32_t type zero-based logical core number.
 * @param[in,out] Thread                uint32_t type zero-based logical thread number.
 *
 **/
#define CORE_TOPOLOGY_V3_FOR_EACH_THREAD(APOB, IterationResult, Socket, Die, Ccd, Complex, Core, Thread) \
        Socket = CORE_TOPOLOGY_V3_ITERATION_START; \
        Die = CORE_TOPOLOGY_V3_ITERATION_START; \
        Ccd = CORE_TOPOLOGY_V3_ITERATION_START; \
        Complex = CORE_TOPOLOGY_V3_ITERATION_START; \
        Core = CORE_TOPOLOGY_V3_ITERATION_START; \
        Thread = CORE_TOPOLOGY_V3_ITERATION_START; \
        for ( \
          IterationResult = CoreTopologyV3IterateThread(APOB, &Socket, &Die, &Ccd, &Complex, &Core, &Thread); \
          IterationResult <= CORE_TOPOLOGY_V3_NEXT_THREAD; \
          IterationResult = CoreTopologyV3IterateThread(APOB, &Socket, &Die, &Ccd, &Complex, &Core, &Thread) \
          )
