/**
 * @file  SmuDefs.h.h
 * This file contains enums for SMU command and response types.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define SMU_ARGUMENT_SIZE              24
#define MAX_CAC_WEIGHT_NUM             23

#define INVALID_SMC_MESSAGE            0xFF

/// SMU Response Codes
typedef enum {
  SMC_Result_Fatal              = -4,
  SMC_Result_OK                 = 0x1,
  SMC_Result_CmdRejectedBusy    = 0xFC,
  SMC_Result_CmdRejectedPrereq  = 0xFD,
  SMC_Result_UnknownCmd         = 0xFE,
  SMC_Result_Failed             = 0xFF,
} SMC_RESULT;

/// Host Firmware to SMU Message Definitions
typedef enum {
  SMC_MSG_TestMessage                         = 0x1,
  SMC_MSG_GetSmuVersion                       = 0x2,
  SMC_MSG_EnableSmuFeatures                   = 0x3,
  SMC_MSG_DisableSmuFeatures                  = 0x4,
  SMC_MSG_SetBiosDramAddr                     = 0x5,
  SMC_MSG_SetToolsDramAddr                    = 0x6,
  SMC_MSG_SetDebugDramAddr                    = 0x7,
  SMC_MSG_DxioTestMessage                     = 0x8,
  SMC_MSG_ReadCoreCacWeightRegister           = 0x9,
  SMC_MSG_UsbInit                             = 0xA,
  SMC_MSG_SleepEntry                          = 0xB,
  SMC_MSG_DcBtc                               = 0xC,
  SMC_MSG_GetNameString                       = 0xD,
  SMC_MSG_GetClusterOnDieMode                 = 0xE,
  SMC_MSG_GetPerSrcBistPF                     = 0xF,
  SMC_MSG_TransferBiosIfTableToSmu            = 0x10,
  SMC_MSG_spare1                              = 0x11,
  SMC_MSG_TransferPcieHotPlugConfigTableToSmu = 0x12,
  SMC_MSG_UsbConfigUpdate                     = 0x13,
  SMC_MSG_UsbS3Entry                          = 0x14,
  SMC_MSG_UsbS3Exit                           = 0x15,
  SMC_MSG_SetCPPCTableAddr                    = 0x16,
  SMC_MSG_GetCPPCSupportedRegisters           = 0x17,
  SMC_MSG_EnableHotplug                       = 0x18,
  SMC_MSG_DisableHotPlug                      = INVALID_SMC_MESSAGE, // 0x19
  SMC_MSG_SetI2CBusSwitchBaseAddress          = 0x1A,
  SMC_MSG_SetEMBlinkInterval                  = 0x1B,
  SMC_MSG_SetHotplugPollInterval              = 0x1C,
  SMC_MSG_SetHotplugDebugFlags                = 0x1D,
  SMC_MSG_SetGPIOInterruptCommand             = 0x1E,
  SMC_MSG_GetGPIOInterruptStatus              = 0x1F,
  SMC_MSG_GPIOService                         = 0x20,
  SMC_MSG_ColdResetEntry                      = 0x21,
  SMC_MSG_EnableSCM                           = 0x22,
  SMC_MSG_TDP_TJMAX                           = 0x23,
  SMC_MSG_EnableOverclocking                  = 0x24,
  SMC_MSG_DisableOverclocking                 = 0x25,
  SMC_MSG_SetOverclockFreqAllCores            = 0x26,
  SMC_MSG_SetOverclockFreqPerCore             = 0x27,
  SMC_MSG_SetOverclockVID                     = 0x28,
  SMC_MSG_SetBoostLimitFrequency              = 0x29,
  SMC_MSG_SetStaticPowerGating                = 0x2A,
  SMC_MSG_SetBoostLimitFrequencyAllCores      = 0x2B,
  SMC_MSG_GetOverclockCap                     = 0x2C,
  SMC_MSG_HotplugUnblockPort                  = 0x2D,
  SMC_MSG_HotplugCancelAdd                    = 0x2E,
  SMC_MSG_SetFITLimitScalar                   = 0x2F,
  SMC_MSG_spare                               = 0x30,
  SMC_MSG_EnableSataActLWka                   = 0x31,
  SMC_MSG_BXBReserved                         = 0x32,
  SMC_MSG_PBO_EN                              = 0x33,
  SMC_MSG_SetLclkDpmLevelRange                = 0x34,
  SMC_MSG_SetEdcL3CreditMode                  = 0x35,
  SMC_MSG_SetDldoPsmMargin                    = 0x35,
  SMC_MSG_SetAllDldoPsmMargin                 = 0x36,
  SMC_MSG_FllBootTimeCalibration              = 0x37,
  SMC_MSG_SOC_DcBtc                           = 0x38,
  SMC_MSG_GetCPPCNominalFrequency             = 0x39,
  SMC_MSG_GetCPPCLowestFrequency              = 0x3A,
  SMC_MSG_SetResetCpuOnSyncFlood              = 0x3B,
  SMC_MSG_EnableTraps                         = 0x40,
  SMC_MSG_DisableUsbPort0                     = 0x41,
  SMC_MSG_EnableHSMPInterrupts                = 0x41,
  SMC_MSG_SetSyncFloodToApml                  = 0x42,
  SMC_MSG_EnablePostCode                      = 0x42,
  SMC_MSG_FddBootTimeCalibration              = 0x43,
  SMC_MSG_SetVoltageLimit                     = 0x44,
  SMC_MSG_SetEDCExcursionReporting            = 0x45,
  SMC_MSG_DfPstateIrritator                   = 0x46,
  SMC_Message_Count                           = 0x47,
} SMC_MSG;
