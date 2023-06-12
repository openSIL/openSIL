/**
 * @file  SmuIp2Ip.h
 * This file contains the definition of the SMU Ip 2 Ip API.
 * This API provides other openSIL IPs with any SMU functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <NBIO/GnbDxio.h>
#include <SMU/SmuDefs.h>

// Common function type definitions for functions in SMU's Ip2Ip API

typedef void (*SMU_SERVICE_INIT_ARGS) (
  uint32_t  *SmuArg
  );

typedef SMC_RESULT (*SMU_SERVICE_REQUEST) (
  PCI_ADDR  NbioPciAddress,
  SMC_MSG   RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  );

typedef SIL_STATUS (*SMU_FIRMWARE_TEST) (
  GNB_HANDLE *NbioHandle
  );

typedef SMC_RESULT (*DXIO_SERVICE_REQUEST) (
  GNB_HANDLE  *NbioHandle,
  uint32_t    DxioMBoxRequest,
  uint32_t    *DxioMBoxArg,
  uint32_t    AccessFlags
  );

typedef void (*SMU_NOFITY_S3_ENTRY) (void);

typedef SIL_STATUS (*SMU_GET_GNB_HANDLE) (
  uint32_t    InstanceId,
  GNB_HANDLE  **Handle
  );

typedef SIL_STATUS (*SMU_READ_BRAND_STRING) (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  );

typedef SIL_STATUS (*SMU_LAUNCH_THREAD) (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  LogicalCcd,
  uint32_t  LogicalComplex,
  uint32_t  LogicalCore,
  uint32_t  LogicalThread
  );

typedef SIL_STATUS (*SMU_READ_BIST_INFO) (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  );

typedef SIL_STATUS (*SMU_READ_CAC_WEIGHTS) (
  uint32_t  MaxNumWeights,
  uint64_t  *ApmWeights
  );

typedef SIL_STATUS (*SMU_REGISTER_READ) (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

typedef SIL_STATUS (*SMU_REGISTER_WRITE) (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

typedef SIL_STATUS (*SMU_REGISTER_RMW) (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  RegisterANDValue,
  uint32_t  RegisterORValue
  );

typedef SIL_STATUS (*SMU_DISABLE_SMT) (void);

typedef SIL_STATUS (*SMU_GET_OPN_CORE_PRESENCE) (
  uint32_t SystemDieNumber,
  uint32_t *CcdEnableMap,
  uint32_t *CcdDisableMap,
  uint32_t *CoreDisMap,
  bool     *SmtEnable
  );

typedef SIL_STATUS (*SMU_GET_OPN_CORE_PRESENCE_EX) (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
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
  SMU_FIRMWARE_TEST             SmuFirmwareTest;
  DXIO_SERVICE_REQUEST          DxioServiceRequest;
  SMU_NOFITY_S3_ENTRY           SmuNotifyS3Entry;
  SMU_GET_GNB_HANDLE            SmuGetGnbHandle;
  SMU_READ_BRAND_STRING         SmuReadBrandString;
  SMU_LAUNCH_THREAD             SmuLaunchThread;
  SMU_READ_BIST_INFO            SmuReadBistInfo;
  SMU_READ_CAC_WEIGHTS          SmuReadCacWeights;
  SMU_REGISTER_READ             SmuRegisterRead;
  SMU_REGISTER_WRITE            SmuRegisterWrite;
  SMU_REGISTER_RMW              SmuRegisterRMW;
  SMU_DISABLE_SMT               SmuDisableSmt;
  SMU_GET_OPN_CORE_PRESENCE     SmuGetOpnCorePresence;
  SMU_GET_OPN_CORE_PRESENCE_EX  SmuGetOpnCorePresenceEx;
} SMU_IP2IP_API;
