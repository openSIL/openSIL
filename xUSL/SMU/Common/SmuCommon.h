/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuCommon.h
 * @brief OpenSIL SMU ver 11 initialization prototypes
 *
 */
#pragma once

#include <Pci.h>
#include <xSIM.h>
#include <Nbio/Common/GnbDxio.h>
#include <SMU/SmuDefs.h>

/**********************************************************************************************************************
 * Declare common variables here
 *
 */

#define SMU_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_SMU & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__); \
          } \
        } while (0)


/**********************************************************************************************************************
 * Function prototypes
 *
 */

void
SmuRegisterWrite (
  PCI_ADDR    PciAddress,
  uint32_t    RegisterIndex,
  uint32_t    Value,
  uint32_t    Flags
  );

void
SmuRegisterRead (
  PCI_ADDR    PciAddress,
  uint32_t    RegisterIndex,
  uint32_t    *Value,
  uint32_t    Flags
  );

void
SmuServiceInitArgumentsCommon (
  uint32_t *SmuArg
  );

SIL_STATUS
SmuFirmwareTestCommon (
  GNB_HANDLE  *NbioHandle
  );

void
SmuNotifyS3EntryCommon (void);

SIL_STATUS SmuGetGnbHandleCommon (
  uint32_t       InstanceId,
  GNB_HANDLE     **Handle
  );

SIL_STATUS
SmuRegisterReadDie (
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

SIL_STATUS
SmuRegisterWriteDie (
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

SIL_STATUS
SmuRegisterRMWDie (
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       RegisterANDValue,
  uint32_t       RegisterORValue
  );

SIL_STATUS
SmuDisableSmtCommon (void);

/**********************************************************************************************************************
 * Declare macros here
 *
 */

/// Common BIOS to SMU Message Definitions
#define SMC_MSG_TestMessage               0x1
#define SMC_MSG_SleepEntry                0xB

#define STRING_COUNT_LIMIT             4
