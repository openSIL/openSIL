/**
 * @file MpioClassDflts.c
 * @brief This file holds the platform default values for the MPIO Input Block
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <Mpio/MpioClass-api.h>

const MPIOCLASS_INPUT_BLK mMpioClassDflts = {
    /*
	 * This is where you declare all input block vars/values you want to share with the Host.
     * This becomes part of the IP API for the Host.
	 */
    .CfgDxioClockGating                 = CONFIG_MPIO_CLOCKGATING_ENABLE,
    .PcieDxioTimingControlEnable        = CONFIG_MPIO_TIMINGCTRL_ENABLE,
    .PCIELinkReceiverDetectionPolling   = CONFIG_PCIE_LINK_RECEIVER_DETECT_TIMEOUT,
    .PCIELinkResetToTrainingTime        = CONFIG_PCIE_LINK_RESET_TO_TRAINING_TIMEOUT,
    .PCIELinkL0Polling                  = CONFIG_PCIE_LINK_L0_STATE_TIMEOUT,
    .PCIeExactMatchEnable               = CONFIG_MPIO_EXACT_MATCH_ENABLE,
    .DxioPhyValid                       = CONFIG_MPIO_PHY_VALID,
    .DxioPhyProgramming                 = CONFIG_MPIO_PHY_PROGRAMMING,
    .CfgSkipPspMessage                  = CONFIG_MPIO_SKIP_PSP_MSG,
    .DxioSaveRestoreModes               = CONFIG_MPIO_SAVE_RESTORE_MODE,
    .AmdAllowCompliance                 = CONFIG_MPIO_ALLOW_PCIE_POLLING,
    .AmdHotPlugHandlingMode             = CONFIG_MPIO_HOT_PLUG_MODE,
    .SrisEnableMode                     = CONFIG_MPIO_PCIE_SRIS_CONTROL,
    .SrisSkipInterval                   = CONFIG_MPIO_PCIE_SRIS_SKIP_INTERVAL,
    .SrisSkpIntervalSel                 = CONFIG_MPIO_SRIS_SKIP_INTERVAL_SELECT,
    .SrisCfgType                        = CONFIG_MPIO_SRIS_CONFIG_TYPE,
    .SrisAutoDetectMode                 = CONFIG_MPIO_SRIS_AUTO_DETECT_MODE,
    .SrisAutodetectFactor               = CONFIG_MPIO_SRIS_AUTODETECT_FACTOR,
    .SrisLowerSkpOsGenSup               = CONFIG_MPIO_PCIE_SRIS_SKP_TRANSMISSION_CONTROL,
    .SrisLowerSkpOsRcvSup               = CONFIG_MPIO_PCIE_SRIS_SKP_RECEPTION_CONTROL,
    .AmdCxlOnAllPorts                   = CONFIG_MPIO_CXL_PORT_CONTROL,
    .CxlCorrectableErrorLogging         = CONFIG_MPIO_CXL_CORRECTABLE_ERROR_LOGGING,
    .CxlUnCorrectableErrorLogging       = CONFIG_MPIO_CXL_UNCORRECTABLE_ERROR_LOGGING,
    .CfgAEREnable                       = CONFIG_MPIO_ADVANCED_ERROR_REPORTING_ENABLE,
    .CfgMcCapEnable                     = CONFIG_MPIO_PCIE_MULTICAST_ENABLE,
    .CfgRcvErrEnable                    = CONFIG_MPIO_RECEIVE_ERROR_ENABLE,
    .EarlyBmcLinkTraining               = CONFIG_MPIO_EARLY_BMC_LINK_TRAIN_ENABLE,
    .EarlyBmcLinkSocket                 = CONFIG_MPIO_EARLY_BMC_LINK_SOCKET,
    .EarlyBmcLinkLaneNum                = CONFIG_MPIO_EARLY_BMC_LINK_LANE,
    .EarlyBmcLinkDie                    = CONFIG_MPIO_EARLY_BMC_LINK_DIE,
    .SurpriseDownFeature                = CONFIG_MPIO_SURPRISE_DOWN_ENABLE,
    .LcMultAutoSpdChgOnLastRateEnable   = CONFIG_MPIO_PCIE_LINK_TRAINING_SPEED,
    .AmdRxMarginEnabled                 = CONFIG_MPIO_RX_MARGIN_ENABLE,
    .CfgPcieCVTestWA                    = CONFIG_MPIO_PCIE_CV_TEST_CONFIG,
    .CfgPcieAriSupport                  = CONFIG_MPIO_PCIE_ARI_SUPPORT,
    .CfgNbioCTOtoSC                     = CONFIG_MPIO_TOGGLE_NBIO_TO_SC,
    .CfgNbioCTOIgnoreError              = CONFIG_MPIO_TOGGLE_NBIO_IGNORE_CTO_ERROR,
    .CfgNbioSsid                        = CONFIG_NBIO_CONTROLLER_SSID,
    .CfgIommuSsid                       = CONFIG_IOMMU_CONTROLLER_SSID,
    .CfgPspccpSsid                      = CONFIG_PSP_CCP_CONTROLLER_SSID,
    .CfgNtbccpSsid                      = CONFIG_NTB_CCP_CONTROLLER_SSID,
    .CfgNbifF0Ssid                      = CONFIG_NBIF0_CONTROLLER_SSID,
    .CfgNtbSsid                         = CONFIG_NTB_CONTROLLER_SSID,
    .AmdPcieSubsystemDeviceID           = CONFIG_PCIE_SUBSYSTEM_DEVICE_ID,
    .AmdPcieSubsystemVendorID           = CONFIG_PCIE_SUBSYSTEM_VENDOR_ID,
    .GppAtomicOps                       = CONFIG_MPIO_GPP_ATOMIC_OPS,
    .GfxAtomicOps                       = CONFIG_MPIO_GPFXATOMIC_OPS,
    .AmdNbioReportEdbErrors             = CONFIG_MPIO_EDB_ERROR_REPORTING_ENABLE,
    .OpnSpare                           = CONFIG_MPIO_OPN_SPARE,
    .AmdPreSilCtrl0                     = CONFIG_AMD_PRE_SIL_CONTROL,
    .MPIOAncDataSupport                 = CONFIG_MPIO_ANCILLARY_DATA_SUPPORT_ENABLE,
    .AfterResetDelay                    = CONFIG_MPIO_AFTER_RESET_DELAY,
    .CfgEarlyLink                       = CONFIG_MPIO_EARLY_LINK_TRAINING_ENABLE,
    .AmdCfgExposeUnusedPciePorts        = CONFIG_MPIO_EXPOSE_UNUSED_PCIE_PORTS,
    .CfgForcePcieGenSpeed               = CONFIG_MPIO_MAX_PCIE_LINK_SPEED,
    .CfgSataPhyTuning                   = CONFIG_MPIO_SATA_PHY_TUNING,
    .PcieLinkComplianceModeAllPorts     = CONFIG_PCIE_LINK_COMPILANCE_MODE_ENABLE,
    .AmdMCTPEnable                      = CONFIG_MPIO_MCTP_SUPPORT_ENABLE,
    .SbrBrokenLaneAvoidanceSup          = CONFIG_SBR_BROKEN_LANE_AVOIDANCE_ENABLE,
    .AutoFullMarginSup                  = CONFIG_AUTO_FULL_MARGINING_SUPPORT_ENABLE,
    .AmdPciePresetMask8GtAllPort        = CONFIG_GEN3_PCIE_PRESET_MASK,
    .AmdPciePresetMask16GtAllPort       = CONFIG_GEN4_PCIE_PRESET_MASK,
    .AmdPciePresetMask32GtAllPort       = CONFIG_GEN5_PCIE_PRESET_MASK,
    .PcieLinkAspmAllPort                = CONFIG_PCIE_LINK_ACTIVE_STATE_PWR_MGMT,
    .AmdMCTPMasterSeg                   = CONFIG_MCTP_MASTER_PCI_ADDR_SEGMENT,
    .AmdMCTPMasterID                    = CONFIG_MCTP_MASTER_PCI_ADDR
};
