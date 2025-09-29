/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuIp2Ip.h
 * This file contains the definition of the SMU Ip 2 Ip API.
 * This API provides other openSIL IPs with any SMU functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once

#include <Nbio/Common/GnbDxio.h>
#include <xSIM-api.h>
#include <SMU/SmuDefs.h>

// Common function type definitions for functions in SMU's Ip2Ip API

typedef void (*SMU_SERVICE_INIT_ARGS) (
  uint32_t  *SmuArg
  );

typedef SMC_RESULT (*SMU_SERVICE_REQUEST) (
  PCI_ADDR  NbioPciAddress,
  uint32_t  RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  );

typedef SMC_RESULT (*SMU_SERVICE_REQUEST_ONLY) (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

typedef SIL_STATUS (*SMU_FIRMWARE_TEST) (
  SIL_CONTEXT *SilContext,
  GNB_HANDLE  *NbioHandle
  );

typedef void (*SMU_NOTIFY_S3_ENTRY) (void);

typedef SIL_STATUS (*SMU_GET_GNB_HANDLE) (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  GNB_HANDLE     **Handle
  );

typedef SIL_STATUS (*SMU_READ_BRAND_STRING) (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       BrandStringLength,
  uint8_t        *BrandString
  );

typedef SIL_STATUS (*SMU_READ_BIST_INFO) (
  SIL_CONTEXT    *SilContext,
  uint8_t        InstanceId,
  uint8_t        PhysicalCcx,
  uint32_t       *BistInfo
  );

typedef SIL_STATUS (*SMU_READ_CAC_WEIGHTS) (
  SIL_CONTEXT    *SilContext,
  uint32_t       MaxNumWeights,
  uint64_t       *ApmWeights
  );

typedef SIL_STATUS (*SMU_REGISTER_READ_DIE) (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

typedef SIL_STATUS (*SMU_REGISTER_WRITE_DIE) (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

typedef SIL_STATUS (*SMU_REGISTER_RMW_DIE) (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       RegisterANDValue,
  uint32_t       RegisterORValue
  );

typedef SIL_STATUS (*SMU_DISABLE_SMT) (void);

typedef SIL_STATUS (*SMU_GET_OPN_CORE_PRESENCE) (
  SIL_CONTEXT    *SilContext,
  uint32_t       SystemDieNumber,
  uint32_t       *CcdEnableMap,
  uint32_t       *CcdDisableMap,
  uint32_t       *CoreDisMap,
  uint32_t       *CoreDisMapLength,
  bool           *SmtEnable
  );

typedef SIL_STATUS (*SMU_GET_OPN_CORE_PRESENCE_EX) (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
  uint32_t  *CoreDisMapLength,
  bool      *SmtEnable,
  uint32_t  *SmtEnableBufferSize
  );

typedef struct {
  SIL_DATA_BLOCK_ID     IpId;       ///< Block Id for this ip
  uint8_t               IpVersion;  ///< Ip version in numerical format
} SMU_IP2IP_API_HEADER;

// Define the Ip2Ip API as a struct containing pointers to these functions

typedef struct {
  SMU_IP2IP_API_HEADER          Header;
  SMU_SERVICE_INIT_ARGS         SmuServiceInitArguments;
  SMU_SERVICE_REQUEST           SmuServiceRequest;
  SMU_SERVICE_REQUEST_ONLY      SmuServiceRequestOnly;
  SMU_FIRMWARE_TEST             SmuFirmwareTest;
  SMU_NOTIFY_S3_ENTRY           SmuNotifyS3Entry;
  SMU_GET_GNB_HANDLE            SmuGetGnbHandle;
  SMU_READ_BRAND_STRING         SmuReadBrandString;
  SMU_READ_BIST_INFO            SmuReadBistInfo;
  SMU_READ_CAC_WEIGHTS          SmuReadCacWeights;
  SMU_REGISTER_READ_DIE         SmuRegisterReadDie;
  SMU_REGISTER_WRITE_DIE        SmuRegisterWriteDie;
  SMU_REGISTER_RMW_DIE          SmuRegisterRMWDie;
  SMU_DISABLE_SMT               SmuDisableSmt;
  SMU_GET_OPN_CORE_PRESENCE     SmuGetOpnCorePresence;
  SMU_GET_OPN_CORE_PRESENCE_EX  SmuGetOpnCorePresenceEx;
} SMU_IP2IP_API;
