/**
 *  @file MpioPcie.h
 *  @brief Pcie training and hotplug configuration.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include "MpioStructs.h"
#include "MpioClass-api.h"

#define DEVFUNC(d, f) ((((uint8_t) d) << 3) | ((uint8_t) f))

typedef union {
  struct {
    uint32_t    DevFnMap:8;
    uint32_t    Reserved_31_8:24;
  } Field;
  uint32_t  Value;
} MPIO_PROG_DEVICE_REMAP_STRUCT;

typedef union {
  struct {
    uint32_t         lowBWThreshold    : 4 ; ///< Low Bandwidth Threshold
    uint32_t         HighBWThreshold   : 4 ; ///< High Bandwidth Threshold
    uint32_t         lowBWThresholdG2  : 4 ; ///< Low Bandwidth Threshold
    uint32_t         HighBWThresholdG2 : 4 ; ///< High Bandwidth Threshold
    uint32_t         lowBWThresholdG3  : 4 ; ///< Low Bandwidth Threshold
    uint32_t         HighBWThresholdG3 : 4 ; ///< High Bandwidth Threshold
    uint32_t         reserved          : 5 ; ///< Reserved for future purpose
    uint32_t         BWHintCount       : 3 ; ///< Bandwidth Count
  } Field;                                         ///<  Union with threshold values
  uint32_t Value;                                  ///<  Value
} MPIO_ANCILLARYPORTDATA_PSPP;


typedef union {
  struct {
    uint32_t         lowBWThresholdG4  : 4 ; ///< Low Bandwidth Threshold
    uint32_t         HighBWThresholdG4 : 4 ; ///< High Bandwidth Threshold
    uint32_t         lowBWThresholdG5  : 4 ; ///< Low Bandwidth Threshold
    uint32_t         HighBWThresholdG5 : 4 ; ///< High Bandwidth Threshold
    uint32_t         Reserved1         : 16; ///< Reservered for future purpose
 } Field;                                    ///< Union with threshold values
  uint32_t Value;                            ///< Value
} MPIO_ANCILLARYPORTDATA_PSPP_2;

void
PcieEarlyTrainFixups (
  MPIOCLASS_INPUT_BLK       *SilData,
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  );

void
PcieConfigureHotplugPorts (
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
MpioCleanUpEarlyBridge (
  PCI_ADDR    Endpoint
  );

void MpioVisibilityControl (void);
