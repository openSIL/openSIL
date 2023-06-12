/**
 * @file  FchGpp.c
 * @brief FCH GPP data and functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <FCH/Common/FchCommonCfg.h>

FCH_GPP Gpp = {
  .PortCfg[0] = { false, false, false, false, 0 },
  .PortCfg[1] = { false, false, false, false, 0 },
  .PortCfg[2] = { false, false, false, false, 0 },
  .PortCfg[3] = { false, false, false, false, 0 },
  .GppLinkConfig =                 FchPortA1B1C1D1,
  .GppFunctionEnable =             false,
  .GppToggleReset =                false,
  .GppHotPlugGeventNum =           0,
  .GppFoundGfxDev =                0,
  .GppGen2 =                       false,
  .GppGen2Strap =                  0,
  .GppMemWrImprove =               false,
  .GppUnhidePorts =                false,
  .GppPortAspm =                   0,
  .GppLaneReversal =               false,
  .GppPhyPllPowerDown =            false,
  .GppDynamicPowerSaving =         false,
  .PcieAer =                       false,
  .PcieRas =                       false,
  .PcieCompliance =                false,
  .PcieSoftwareDownGrade =         false,
  .UmiPhyPllPowerDown =            false,
  .SerialDebugBusEnable =          false,
  .GppHardwareDownGrade =          0,
  .GppL1ImmediateAck =             0,
  .NewGppAlgorithm =               false,
  .HotPlugPortsStatus =            0,
  .FailPortsStatus =               0,
  .GppPortMinPollingTime =         40,
  .IsCapsuleMode =                 false,
  };
