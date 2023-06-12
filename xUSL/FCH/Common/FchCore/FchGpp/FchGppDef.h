/**
 * @file  FchGppDef.h
 * @brief FCH GPP definitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/// Configuration values for BLDCFG_FCH_GPP_LINK_CONFIG
typedef enum {
  FchPortA4       = 0,                   ///< 4:0:0:0
  FchPortA2B2     = 2,                   ///< 2:2:0:0
  FchPortA2B1C1   = 3,                   ///< 2:1:1:0
  FchPortA1B1C1D1 = 4                    ///< 1:1:1:1
} FCH_GPP_LINKMODE, GPP_LINKMODE;

///
/// FCH_GPP_PORT_CONFIG - Fch GPP port config structure
///
typedef struct {
  bool                  PortPresent;                    ///< Port connection
                                                        ///  @par
                                                        ///  @li <b>0</b> - Port doesn't have slot. No need to train the link
                                                        ///  @li <b>1</b> - Port connection defined and needs to be trained
                                                        ///
  bool                  PortDetected;                   ///< Link training status
                                                        ///  @par
                                                        ///  @li <b>0</b> - EP not detected
                                                        ///  @li <b>1</b> - EP detected
                                                        ///
  bool                  PortIsGen2;                     ///< Port link speed configuration
                                                        ///  @par
                                                        ///  @li <b>00</b> - Auto
                                                        ///  @li <b>01</b> - Forced GEN1
                                                        ///  @li <b>10</b> - Forced GEN2
                                                        ///  @li <b>11</b> - Reserved
                                                        ///
  bool                  PortHotPlug;                    ///< Support hot plug?
                                                        ///  @par
                                                        ///  @li <b>0</b> - No support
                                                        ///  @li <b>1</b> - support
                                                        ///
  uint8_t               PortMisc;                       ///  PortMisc - Reserved
} FCH_GPP_PORT_CONFIG_R;

///
/// GPP structure
///
typedef struct {
  FCH_GPP_PORT_CONFIG_R PortCfg[4];                     /// GPP port configuration structure
  FCH_GPP_LINKMODE      GppLinkConfig;                  ///< GppLinkConfig - PCIE_GPP_Enable[3:0]
                                                        ///  @li  <b>0000</b> - Port ABCD -> 4:0:0:0
                                                        ///  @li  <b>0010</b> - Port ABCD -> 2:2:0:0
                                                        ///  @li  <b>0011</b> - Port ABCD -> 2:1:1:0
                                                        ///  @li  <b>0100</b> - Port ABCD -> 1:1:1:1
                                                        ///
  bool               GppFunctionEnable;              ///< GPP Function - 0:disable, 1:enable
  bool               GppToggleReset;                 ///< Toggle GPP core reset
  uint8_t            GppHotPlugGeventNum;            ///< Hotplug GEVENT # - valid value 0-31
  uint8_t            GppFoundGfxDev;                 ///< Gpp Found Gfx Device
                                                        ///  @li   <b>0</b> - Not found
                                                        ///  @li   <b>1</b> - Found
                                                        ///
  bool               GppGen2;                        ///< GPP Gen2 - 0:disable, 1:enable
  uint8_t            GppGen2Strap;                   ///< GPP Gen2 Strap - 0:disable, 1:enable, FCH itself uses this
  bool               GppMemWrImprove;                ///< GPP Memory Write Improve - 0:disable, 1:enable
  bool               GppUnhidePorts;                 ///< GPP Unhide Ports - 0:disable, 1:enable
  uint8_t            GppPortAspm;                    ///< GppPortAspm - ASPM state for all GPP ports
                                                        ///  @li   <b>01</b> - Disabled
                                                        ///  @li   <b>01</b> - L0s
                                                        ///  @li   <b>10</b> - L1
                                                        ///  @li   <b>11</b> - L0s + L1
                                                        ///
  bool               GppLaneReversal;                ///< GPP Lane Reversal - 0:disable, 1:enable
  bool               GppPhyPllPowerDown;             ///< GPP PHY PLL Power Down - 0:disable, 1:enable
  bool               GppDynamicPowerSaving;          ///< GPP Dynamic Power Saving - 0:disable, 1:enable
  bool               PcieAer;                        ///< PcieAer - Advanced Error Report: 0/1-disable/enable
  bool               PcieRas;                        ///< PCIe RAS - 0:disable, 1:enable
  bool               PcieCompliance;                 ///< PCIe Compliance - 0:disable, 1:enable
  bool               PcieSoftwareDownGrade;          ///< PCIe Software Down Grade
  bool               UmiPhyPllPowerDown;             ///< UMI PHY PLL Power Down - 0:disable, 1:enable
  bool               SerialDebugBusEnable;           ///< Serial Debug Bus Enable
  uint8_t            GppHardwareDownGrade;           ///< GppHardwareDownGrade - Gpp HW Down Grade function 0:Disable, 1-4: portA-D
  uint8_t            GppL1ImmediateAck;              ///< GppL1ImmediateAck - Gpp L1 Immediate ACK 0: enable, 1: disable
  bool               NewGppAlgorithm;                ///< NewGppAlgorithm - New GPP procedure
  uint8_t            HotPlugPortsStatus;             ///< HotPlugPortsStatus - Save Hot-Plug Ports Status
  uint8_t            FailPortsStatus;                ///< FailPortsStatus - Save Failure Ports Status
  uint8_t            GppPortMinPollingTime;          ///< GppPortMinPollingTime - Min. Polling time for Gpp Port Training
} FCH_GPP_R;

///
/// FCH_GPP_PORT_CONFIG - Fch GPP port config structure
///
typedef struct {
  bool               PortPresent;                    ///< Port connection
                                                        ///  @par
                                                        ///  @li <b>0</b> - Port doesn't have slot. No need to train the link
                                                        ///  @li <b>1</b> - Port connection defined and needs to be trained
                                                        ///
  bool               PortDetected;                   ///< Link training status
                                                        ///  @par
                                                        ///  @li <b>0</b> - EP not detected
                                                        ///  @li <b>1</b> - EP detected
                                                        ///
  bool               PortIsGen2;                     ///< Port link speed configuration
                                                        ///  @par
                                                        ///  @li <b>00</b> - Auto
                                                        ///  @li <b>01</b> - Forced GEN1
                                                        ///  @li <b>10</b> - Forced GEN2
                                                        ///  @li <b>11</b> - Reserved
                                                        ///
  bool               PortHotPlug;                    ///< Support hot plug?
                                                        ///  @par
                                                        ///  @li <b>0</b> - No support
                                                        ///  @li <b>1</b> - support
                                                        ///
  uint8_t            PortMisc;                       ///  PortMisc - Reserved
} FCH_GPP_PORT_CONFIG;

///
/// GPP structure
///
typedef struct {
  FCH_GPP_PORT_CONFIG   PortCfg[4];                     /// GPP port configuration structure
  GPP_LINKMODE          GppLinkConfig;                  ///< GppLinkConfig - PCIE_GPP_Enable[3:0]
                                                        ///  @li  <b>0000</b> - Port ABCD -> 4:0:0:0
                                                        ///  @li  <b>0010</b> - Port ABCD -> 2:2:0:0
                                                        ///  @li  <b>0011</b> - Port ABCD -> 2:1:1:0
                                                        ///  @li  <b>0100</b> - Port ABCD -> 1:1:1:1
                                                        ///
  bool                  GppFunctionEnable;              ///< GPP Function - 0:disable, 1:enable
  bool                  GppToggleReset;                 ///< Toggle GPP core reset
  uint8_t               GppHotPlugGeventNum;            ///< Hotplug GEVENT # - valid value 0-31
  uint8_t               GppFoundGfxDev;                 ///< Gpp Found Gfx Device
                                                        ///  @li   <b>0</b> - Not found
                                                        ///  @li   <b>1</b> - Found
                                                        ///
  bool                  GppGen2;                        ///< GPP Gen2 - 0:disable, 1:enable
  uint8_t               GppGen2Strap;                   ///< GPP Gen2 Strap - 0:disable, 1:enable, FCH itself uses this
  bool                  GppMemWrImprove;                ///< GPP Memory Write Improve - 0:disable, 1:enable
  bool                  GppUnhidePorts;                 ///< GPP Unhide Ports - 0:disable, 1:enable
  uint8_t               GppPortAspm;                    ///< GppPortAspm - ASPM state for all GPP ports
                                                        ///  @li   <b>01</b> - Disabled
                                                        ///  @li   <b>01</b> - L0s
                                                        ///  @li   <b>10</b> - L1
                                                        ///  @li   <b>11</b> - L0s + L1
                                                        ///
  bool                  GppLaneReversal;                ///< GPP Lane Reversal - 0:disable, 1:enable
  bool                  GppPhyPllPowerDown;             ///< GPP PHY PLL Power Down - 0:disable, 1:enable
  bool                  GppDynamicPowerSaving;          ///< GPP Dynamic Power Saving - 0:disable, 1:enable
  bool                  PcieAer;                        ///< PcieAer - Advanced Error Report: 0/1-disable/enable
  bool                  PcieRas;                        ///< PCIe RAS - 0:disable, 1:enable
  bool                  PcieCompliance;                 ///< PCIe Compliance - 0:disable, 1:enable
  bool                  PcieSoftwareDownGrade;          ///< PCIe Software Down Grade
  bool                  UmiPhyPllPowerDown;             ///< UMI PHY PLL Power Down - 0:disable, 1:enable
  bool                  SerialDebugBusEnable;           ///< Serial Debug Bus Enable
  uint8_t               GppHardwareDownGrade;           ///< GppHardwareDownGrade - Gpp HW Down Grade function 0:Disable, 1-4: portA-D
  uint8_t               GppL1ImmediateAck;              ///< GppL1ImmediateAck - Gpp L1 Immediate ACK 0: enable, 1: disable
  bool                  NewGppAlgorithm;                ///< NewGppAlgorithm - New GPP procedure
  uint8_t               HotPlugPortsStatus;             ///< HotPlugPortsStatus - Save Hot-Plug Ports Status
  uint8_t               FailPortsStatus;                ///< FailPortsStatus - Save Failure Ports Status
  uint8_t               GppPortMinPollingTime;          ///< GppPortMinPollingTime - Min. Polling time for Gpp Port Training
  bool                  IsCapsuleMode;                  ///< IsCapsuleMode - Support Capsule Mode in FCH
} FCH_GPP;
