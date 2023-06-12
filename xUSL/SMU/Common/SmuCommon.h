/**
 * @file  SmuCommon.h
 * @brief OpenSIL SMU ver 11 initialization prototypes
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#pragma once

#include <Pci.h>
#include <xSIM.h>
#include <NBIO/GnbDxio.h>
#include <SMU/SmuDefs.h>

/**********************************************************************************************************************
 * Declare common variables here
 *
 */

#define SMU_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_SMU & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__); \
        }\
  } while (0)


/**********************************************************************************************************************
 * Function prototypes
 *
 */

void
SmuServiceInitArgumentsCommon (
  uint32_t *SmuArg
);

SMC_RESULT
SmuServiceRequestCommon (
  PCI_ADDR  NbioPciAddress,
  SMC_MSG   RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  );

SIL_STATUS
SmuFirmwareTestCommon (
  GNB_HANDLE  *NbioHandle
  );

SMC_RESULT
DxioServiceRequestCommon (
  GNB_HANDLE  *NbioHandle,
  uint32_t    DxioMBoxRequest,
  uint32_t    *DxioMBoxArg,
  uint32_t    AccessFlags
  );

void
SmuNotifyS3EntryCommon (void);

SIL_STATUS SmuGetGnbHandleCommon (
  uint32_t    InstanceId,
  GNB_HANDLE  **Handle
  );

SIL_STATUS
SmuReadBrandStringCommon (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  );

SIL_STATUS
SmuReadBistInfoCommon (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  );

SIL_STATUS
SmuRegisterReadCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

SIL_STATUS
SmuRegisterWriteCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

SIL_STATUS
SmuRegisterRMWCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  RegisterANDValue,
  uint32_t  RegisterORValue
  );

/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define MP1_C2PMSG_ARGUMENT_0_ADDRESS 0x3B109C4ul
#define MP1_C2PMSG_ARGUMENT_1_ADDRESS 0x3B109C8ul
#define MP1_C2PMSG_ARGUMENT_2_ADDRESS 0x3B109CCUL
#define MP1_C2PMSG_ARGUMENT_3_ADDRESS 0x3B109D0UL
#define MP1_C2PMSG_ARGUMENT_4_ADDRESS 0x3B109D4ul
#define MP1_C2PMSG_ARGUMENT_5_ADDRESS 0x3B109D8ul
#define MP1_C2PMSG_MESSAGE_ADDRESS    0x3B10530ul
#define MP1_C2PMSG_RESPONSE_ADDRESS   0x3B1057Cul

#define STRING_COUNT_LIMIT             4
