/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  RasReg-api.h
 * @brief RAS common register descriptions and addresses that can be expose to host firmware
 */

#pragma once

/*
 * SIL_MCA_IPID_MSR: Collection of MCA IPID information for each MCA bank for
 *                   a given processor.  This structure is filled by openSIL at
 *                   the request of Host firmware for RAS support.
 */
typedef union {
  struct {
    uint64_t  InstanceId:32;  ///< The instance ID of this IP.
    uint64_t  HardwareID:12;  ///< The Hardware ID of the IP associated with
                              ///< this MCA bank.
    uint64_t  InstanceIdHi:4; ///< The Hi value instance ID of this IP.
    uint64_t  McaType:16;     ///< The McaType of the MCA bank within this IP.
  } Field;
  uint64_t  Value;
} SIL_MCA_IPID_MSR;

typedef union {
  struct {
    uint64_t ErrorCode:16;   ///<
    uint64_t ErrorCodeExt:6; ///<
    uint64_t :2;             ///< Reserved
    uint64_t AddrLsb:6;      ///< Specifies the least significant valid bit of
                             ///< the address contained in MCA_ADDR[ErrorAddr].
    uint64_t :14;            ///< Reserved
    uint64_t Deferred:1;     ///< 1 = A deferred error was created.
    uint64_t :8;             ///< Reserved
    uint64_t SyndV:1;        ///< 1 = This error logged information in MCA_SYND
    uint64_t :4;             ///< Reserved
    uint64_t AddrV:1;        ///< 1 = MCA_ADDR contains address information
    uint64_t :3;             ///< Reserved
    uint64_t Overflow:1;     ///< 1 = An error was detected while the valid bit
                             ///< was set
    uint64_t Val:1;          ///< 1 = A valid error has been detected
  } Field;
  uint64_t  Value;
} SIL_MCA_DESTAT_MSR;

///MCA
typedef union {
  struct {
    uint64_t ErrorCode:16;   ///<
    uint64_t ErrorCodeExt:6; ///<
    uint64_t :2;             ///< Reserved
    uint64_t AddrLsb:6;      ///< Specifies the least significant valid bit of
                             ///< the address contained in MCA_ADDR[ErrorAddr].
    uint64_t :2;             ///< Reserved
    uint64_t ErrorCodeId:6;  ///<
    uint64_t :2;             ///< Reserved
    uint64_t Scrub:1;        ///< 1 = The error was the result of a scrub
                             ///< operation.
    uint64_t :2;             ///< Reserved
    uint64_t Poison:1;       ///< 1 = The error was the result of attempting to
                             ///< consume poisoned data.
    uint64_t Deferred:1;     ///< 1 = A deferred error was created.
    uint64_t UECC:1;         ///< 1 = The error was an uncorrectable ECC error
    uint64_t CECC:1;         ///< 1 = The error was a correctable ECC error
    uint64_t :5;             ///< Reserved
    uint64_t Transparent:1;  ///< 1 = This error is a transparent error.
    uint64_t SyndV:1;        ///< 1 = This error logged information in MCA_SYND
    uint64_t :1;             ///< Reserved
    uint64_t TCC:1;          ///< 1 = The thread which consumed the error is
                             ///< not restartable and must be terminated.
    uint64_t ErrCoreIdVal:1; ///< 1 = The ErrCoreId field is valid
    uint64_t PCC:1;          ///< 1 = Hardware context held by the processor
                             ///< may have been corrupted
    uint64_t AddrV:1;        ///< 1 = MCA_ADDR contains address information
    uint64_t MiscV:1;        ///< 1 = Valid thresholding in MCA_MISC0
    uint64_t En:1;           ///< 1 = MCA error reporting is enabled for this
                             ///< error.
    uint64_t UC:1;           ///< 1 = The error was not corrected by hardware
    uint64_t Overflow:1;     ///< 1 = An error was detected while the valid bit
                             ///< was set
    uint64_t Val:1;          ///< 1 = A valid error has been detected
  } Field;
  uint64_t  Value;
} SIL_MCA_STATUS_MSR;

/// MCA Configuration MSR register for associated machine check bank.
typedef union {
  struct {                                         ///< Bitfields of MCA Config MSR
    uint64_t  McaX:1;                              ///< McaX
    uint64_t  TransparentErrorLoggingSupported:1;  ///< TransparentErrorLoggingSupported
    uint64_t  DeferredErrorLoggingSupported:1;     ///< DeferredErrorLoggingSupported
    uint64_t  :2;                                  ///< Reserved
    uint64_t  DeferredIntTypeSupported:1;          ///< DeferredIntTypeSupported
    uint64_t  :26;                                 ///< Reserved
    uint64_t  McaXEnable:1;                        ///< McaXEnable
    uint64_t  TransparentErrorLoggingEnable:1;     ///< TransparentErrorLoggingEnable
    uint64_t  LogDeferredInMcaStat:1;              ///< LogDeferredInMcaStat
    uint64_t  :2;                                  ///< Reserved
    uint64_t  DeferredIntType:2;                   ///< DeferredIntType
    uint64_t  :25;                                 ///< Reserved
  } Field;
  uint64_t  Value;
} SIL_MCA_CONFIG_MSR;
