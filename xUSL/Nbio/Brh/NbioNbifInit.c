/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioNbifInit.c
 * @brief This file contains the Nbif device configurations
 *
 */

/*----------------------------------------------------------------------------------------
 *                     M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <SilSocLogicalId.h>
#include <Nbio/NbioIp2Ip.h>
#include <Nbio/Brh/include/NbifMmReg.h>
#include <Nbio/Brh/include/NbifEpFnCfgReg.h>
#include <Nbio/Brh/include/SyshubmmReg.h>
#include <Mpio/Common/MpioStructs.h>
#include "GnbRegistersBrh.h"
#include "NbioSmnTable.h"
#include "NbioBrh.h"
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/*
   NBIF Device Mapping
   NBIF2
    Port0-F0: dummy
    Port0-F1: NTB
 */
// --------------------------------------------------
// 15.6.7.1 nBIF Configurations
// --------------------------------------------------
// NBIF0:
    // NBIF1: DEV0
    // NBIF1: DEV1
    // NBIF1: DEV2
    // NBIF2: DEV0
    // Device enables
//rb0
/*NBIF0 IOHC0-BOTTOM LEFT
    Port0-F0: dummy
    Port0-F1: MPDMATF
    Port0-F2: Primary NTB/vNTB
    Port0-F3: Secondary vNTB
    Port0-F4: USB(enablement handled in ABL-FCH)
    Port0-F5: PSP/CCP
    Port1-F0: SATA0
    Port1-F1: SATA1*/
SMN_TABLE  NbioNbif0Iohc0DeviceEnTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1106,
    SIL_RESERVED_0917,
    (0x1 << SIL_RESERVED_0918)
    ),
  // SMN_ENTRY_RMW(SIL_RESERVED_1109,
  //   SIL_RESERVED_0933,
  //   (0x1 << SIL_RESERVED_0934)
  //   ),
  SMN_ENTRY_RMW(SIL_RESERVED_1117,
    SIL_RESERVED_0981,
    (0x0 << SIL_RESERVED_0982)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1121,
    SIL_RESERVED_0997,
    (0x0 << SIL_RESERVED_0998)
    ),
  //added usb(epf4) enablement in rb0
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013480C,
    SIL_RESERVED_1009,
    (0x1 << SIL_RESERVED_1010)
    ),
  //added usb(epf4) AER enablement in rb0
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10134808,
    SIL_RESERVED_1007,
    (0x1 << SIL_RESERVED_1008)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1125,
    SIL_RESERVED_1013,
    (0x0 << SIL_RESERVED_1014)
    ),
  //sata usb(epf4) enabled/disabled in ABL-FCH
  //disable unused epf
  SMN_ENTRY_RMW(SIL_RESERVED_1127,
    SIL_RESERVED_1017,
    (0x0 << SIL_RESERVED_1018)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1129,
    SIL_RESERVED_1021,
    (0x0 << SIL_RESERVED_1022)
    ),
  //flr enable for nbif0 only
  SMN_ENTRY_RMW(SIL_RESERVED_1128,
    SIL_RESERVED_1019,
    (0x1 << SIL_RESERVED_1020)
    ),
  SMN_ENTRY_TERMINATE
};
//rb3
/*NBIF1 IOHC2 TOP RIGHT
    Port0-F0: dummy
    Port0-F1: MPDMATF
    Port0-F2: Primary NTB/vNTB
    Port0-F3: Secondary vNTB
    Port0-F4: USB(enabled in ABL FCH)
    Port0-F6: ACP
    Port0-F7: AZ
    PORT1-F0:SATA0
    PORT1-F1:SATA1*/
SMN_TABLE  NbioNbif1Iohc2DeviceEnTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1106,
    SIL_RESERVED_0917,
    (0x1 << SIL_RESERVED_0918)
    ),
  // SMN_ENTRY_RMW(SIL_RESERVED_1109,
  //   SIL_RESERVED_0933,
  //   (0x1 << SIL_RESERVED_0934)
  //   ),
  SMN_ENTRY_RMW(SIL_RESERVED_1117,
    SIL_RESERVED_0981,
    (0x0 << SIL_RESERVED_0982)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1121,
    SIL_RESERVED_0997,
    (0x0 << SIL_RESERVED_0998)
    ),
  //added usb(epf4) enablement in rb3
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013480C,
    SIL_RESERVED_1009,
    (0x1 << SIL_RESERVED_1010)
    ),
  //added usb(epf4) AER enablement in rb3
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10134808,
    SIL_RESERVED_1007,
    (0x1 << SIL_RESERVED_1008)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1127,
    SIL_RESERVED_1017,
    (0x0 << SIL_RESERVED_1018)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1129,
    SIL_RESERVED_1021,
    (0x0 << SIL_RESERVED_1022)
    ),
  //epf4 usb function enabled/disabled in ABL-FCH
  //disable epf5 as not used config
  SMN_ENTRY_RMW(SIL_RESERVED_1125,
    SIL_RESERVED_1013,
    (0x0 << SIL_RESERVED_1014)
    ),
  SMN_ENTRY_TERMINATE
};
/*NBIF0 IOHC0-BOTTOM RIGHT
    PORT0-F0:DUMMY
    PORT0-F1:TF-MPDMA
    PORT0-F2:PRIMARY NTB
    PORT0-F3:SEC NTB
    NBIF1:TOP LEFT NBIF1 IOHC2*/
//rb 1 and 2
SMN_TABLE  NbioNbifDeviceEnTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1106,
    SIL_RESERVED_0917,
    (0x1 << SIL_RESERVED_0918)
    ),
  // SMN_ENTRY_RMW(SIL_RESERVED_1109,
  //   SIL_RESERVED_0933,
  //   (0x1 << SIL_RESERVED_0934)
  //   ),
  SMN_ENTRY_RMW(SIL_RESERVED_1117,
    SIL_RESERVED_0981,
    (0x0 << SIL_RESERVED_0982)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1121,
    SIL_RESERVED_0997,
    (0x0 << SIL_RESERVED_0998)
    ),
  //sata usb(epf4) enabled/disabled in ABL-FCH
  //disabling unused functions as follows
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10134800,
    SIL_RESERVED_1005,
    (0x0 << SIL_RESERVED_1006)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1125,
    SIL_RESERVED_1013,
    (0x0 << SIL_RESERVED_1014)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1127,
    SIL_RESERVED_1017,
    (0x0 << SIL_RESERVED_1018)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1129,
    SIL_RESERVED_1021,
    (0x0 << SIL_RESERVED_1022)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135000,
    SIL_RESERVED_1067,
    (0x0 << SIL_RESERVED_1068)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135200,
    SIL_RESERVED_1079,
    (0x0 << SIL_RESERVED_1080)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifDeviceTbl
//only rb 0 and 3- cause sata is present on those nbifs
//sata usb function enabled/disabled in ABL-FCH
// Program SATA revision IDs for all potential SATA devices to 0x71
SMN_TABLE  NbioNbifSataRevTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135000,
    SIL_RESERVED_1069,
    (0x0 << SIL_RESERVED_1070)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135000,
    SIL_RESERVED_1071,
    (0x0 << SIL_RESERVED_1072)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135200,
    SIL_RESERVED_1082,
    (0x0 << SIL_RESERVED_1081)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135200,
    SIL_RESERVED_1082,
    (0x0 << SIL_RESERVED_1083)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1114,
    SIL_RESERVED_0967,
    (0x0 << SIL_RESERVED_0968)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1114,
    SIL_RESERVED_0967,
    (0x0 << SIL_RESERVED_0968)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013480C,
    SIL_RESERVED_1009,
    (0x1 << SIL_RESERVED_1010)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013500C,
    SIL_RESERVED_1075,
    (0x0 << SIL_RESERVED_1076)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013520C,
    SIL_RESERVED_1086,
    (0x0 << SIL_RESERVED_1087)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135008,
    SIL_RESERVED_1073,
    (0x1 << SIL_RESERVED_1074)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135208,
    SIL_RESERVED_1084,
    (0x1 << SIL_RESERVED_1085)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifDeviceCfgTbl
// Disable PTDMA on NBIF0 and NBIF1 PORT0
SMN_TABLE GnbNbifDisablePtdma [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1117,
    SIL_RESERVED_0981,
    (0x0 << SIL_RESERVED_0982)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1102,
    (1 << (PCIE_FUNCTION2 + SIL_RESERVED_0889)),
    (0x0 << (PCIE_FUNCTION2 + SIL_RESERVED_0889))
    ),
  SMN_ENTRY_TERMINATE
};
// --------------------------------------------------
// 15.6.7.5.1  Endpoint function interrupt enable
// --------------------------------------------------
SMN_TABLE NbioNbif0DeviceInterruptTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1126,
    SIL_RESERVED_1015,
    (0x1 << SIL_RESERVED_1016)
    ),
  SMN_ENTRY_TERMINATE
};
//rbindex 1 and 3
SMN_TABLE NbioNbif1DeviceInterruptTbl [] = {
  //flr enable address
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_10135010,
    SIL_RESERVED_1077,
    (0x1 << SIL_RESERVED_1078)
    ),
  SMN_ENTRY_TERMINATE
};

//
// FLR
//
// NBIF0:
// NBIF1: DEV0
// NBIF1: DEV1
// NBIF1: DEV2
// NBIF2: DEV0
SMN_TABLE  NbioNbifFlrTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1108,
    SIL_RESERVED_0931,
    (0x1 << SIL_RESERVED_0932)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1115,
    SIL_RESERVED_0971,
    (0x1 << SIL_RESERVED_0972)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1119,
    SIL_RESERVED_0987,
    (0x1 << SIL_RESERVED_0988)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1122,
    SIL_RESERVED_0999,
    (0x1 << SIL_RESERVED_1000)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1124,
    SIL_RESERVED_1011,
    (0x1 << SIL_RESERVED_1012)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifFlrTbl


// --------------------------------------------------
// 15.6.7.5.4  DMA Arbitration
// --------------------------------------------------
//
SMN_TABLE  NbioNbifDmaArbitrationTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1100,
    SIL_RESERVED_0872 |
    SIL_RESERVED_0874 |
    SIL_RESERVED_0876 |
    SIL_RESERVED_0878,
    (0x4 << SIL_RESERVED_0873) |
    (0x4 << SIL_RESERVED_0875) |
    (0x4 << SIL_RESERVED_0877) |
    (0x4 << SIL_RESERVED_0879)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1101,
    SIL_RESERVED_0880 |
    SIL_RESERVED_0882 |
    SIL_RESERVED_0884 |
    SIL_RESERVED_0886,
    (0x4 << SIL_RESERVED_0881) |
    (0x4 << SIL_RESERVED_0883) |
    (0x4 << SIL_RESERVED_0885) |
    (0x4 << SIL_RESERVED_0887)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifDmaArbitrationTbl

// --------------------------------------------------
//  SystemHub Write Performance Enhancement
// --------------------------------------------------

SMN_TABLE  NbioNbifSyshubWrtPerfEnhTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1708,
    SIL_RESERVED_1716 |
    SIL_RESERVED_1718,
    (0x1 << SIL_RESERVED_1717) |
    (0x1 << SIL_RESERVED_1719)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifSyshubWrtPerfEnhTbl


// --------------------------------------------------
// PPR: nBIF Clock Gating Feature
// --------------------------------------------------
SMN_TABLE  NbioNbifClockGatingTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_NBIF_MGCG_CLKGATING_ENABLED,
    SIL_RESERVED_1104,
    SIL_RESERVED_0897,
    (0x1 << SIL_RESERVED_0898)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_NBIF_MGCG_CLKGATING_DISABLED,
    SIL_RESERVED_1104,
    SIL_RESERVED_0897,
    (0x0 << SIL_RESERVED_0898)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED,
    SIL_RESERVED_1707,
    SIL_RESERVED_1700,
    (0x1 << SIL_RESERVED_1701)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED,
    SIL_RESERVED_1707,
    SIL_RESERVED_1700,
    (0x0 << SIL_RESERVED_1701)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED,
    SIL_RESERVED_1711,
    SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_EN_SHUBCLK_MASK,
    (0x1 << SIL_RESERVED_1723)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED,
    SIL_RESERVED_1711,
    SYSHUB_MGCG_CTRL_SHUBCLK_SYSHUB_MGCG_EN_SHUBCLK_MASK,
    (0x0 << SIL_RESERVED_1723)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NbioNbifClockGatingTbl

//
// Enable TPH completer/requester capability
//
SMN_TABLE  NbioTphTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1116,
    SIL_RESERVED_0975 |
    SIL_RESERVED_0973,
    (0x1 << SIL_RESERVED_0976) |
    (0x1 << SIL_RESERVED_0974)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1120,
    SIL_RESERVED_0991 |
    SIL_RESERVED_0989,
    (0x1 << SIL_RESERVED_0992) |
    (0x1 << SIL_RESERVED_0990)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1123,
    SIL_RESERVED_1003 |
    SIL_RESERVED_1001,
    (0x1 << SIL_RESERVED_1004) |
    (0x1 << SIL_RESERVED_1002)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1134,
    SIL_RESERVED_1057,
    (0x1 << SIL_RESERVED_1058)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1137,
    SIL_RESERVED_1090,
    (0x1 << SIL_RESERVED_1091)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// NBIF LCLK Deep Sleep registers must be set for IOAGR to go idle
// --------------------------------------------------
SMN_TABLE  NbioLclkDsIdleTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1103,
    SIL_RESERVED_0891,
    0x1 << SIL_RESERVED_0892
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// Enable SOCCLK and SHUBCLK deep sleep
// --------------------------------------------------
SMN_TABLE   NbioEnableSocClkShubClkDsTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1710,
    SIL_RESERVED_1721,
    (0x1 << SIL_RESERVED_1722)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1709,
    SIL_RESERVED_1720,
    (0x1 << SYSHUB_DS_CTRL_SHUBCLK_SYSHUB_SHUBCLK_DS_EN_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// Enable MSI and DOE Capability
// Data Object Exchange (DOE) is a cap for providing a method let system
// FW/SW could exchange data by W/R mailbox mechanics.
// --------------------------------------------------
SMN_TABLE  NbioDoeCapability [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013400C,
    SIL_RESERVED_0929,
    (0x1 << SIL_RESERVED_0930)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1113,
    SIL_RESERVED_0955,
    (0x1 << SIL_RESERVED_0956)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1115,
    SIL_RESERVED_0969,
    (0x1 << SIL_RESERVED_0970)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// Register of Pool Credit Allocation for NBIF 2
// GMI REQ and GMI OrigData
// --------------------------------------------------
SMN_TABLE NbioNbif0PoolCredTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A308,
    SIL_RESERVED_0870,
    (0x2 << SIL_RESERVED_0871)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A30C,
    SIL_RESERVED_0866,
    (0x2 << SIL_RESERVED_0867)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// Register of Pool Credit Allocation for NBIF 1
// GMI REQ and GMI OrigData
// --------------------------------------------------
SMN_TABLE NbioNbif1PoolCredTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A308,
    SIL_RESERVED_0870,
    (0x2 << SIL_RESERVED_0871)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A30C,
    SIL_RESERVED_0866,
    (0x2 << SIL_RESERVED_0867)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A308,
    SIL_RESERVED_0868,
    (0x0 << SIL_RESERVED_0869)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1013A30C,
    SIL_RESERVED_0864,
    (0x0 << SIL_RESERVED_0865)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// Enable Completion Timeout feature
// --------------------------------------------------
SMN_TABLE  NbioEnableCompletionTimeoutTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1132,
    SIL_RESERVED_1037,
    (0x1 << SIL_RESERVED_1038)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1136,
    SIL_RESERVED_1088,
    (0x1 << SIL_RESERVED_1089)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1139,
    SIL_RESERVED_1088,
    (0x1 << SIL_RESERVED_1089)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// DEVICE_ID_DEV* from 0x14C8 to 0x14DC
// --------------------------------------------------
// WRITE ONCE PER SOCKET
SMN_TABLE  NbifDeviceIdDevTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1106,
    SIL_RESERVED_0915,
    (0x14DC << SIL_RESERVED_0916)
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// LTR disable
// --------------------------------------------------

SMN_TABLE  NbioNbifDisableLtrTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(TABLE_PROPERTY_IGFX_DISABLED,
    SIL_RESERVED_1132,
    SIL_RESERVED_1033 |
    SIL_RESERVED_1035,
    (0x0 << SIL_RESERVED_1034) |
    (0x0 << SIL_RESERVED_1036)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NBIO_NBIF_DISABLE_LTR_TBL

//nbif2 initialization
SMN_TABLE  NbioNbif2InitTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1145,
    SIL_RESERVED_0917,
    (0x0 << SIL_RESERVED_0918)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1147,
    SIL_RESERVED_0933,
    (0x0 << SIL_RESERVED_0934)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1150,
    SIL_RESERVED_0981,
    (0x0 << SIL_RESERVED_0982)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1146,
    SIL_RESERVED_0931,
    (0x1 << SIL_RESERVED_0932)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1148,
    SIL_RESERVED_1077,
    (0x1 << SIL_RESERVED_1078)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1151,
    SIL_RESERVED_1077,
    (0x1 << SIL_RESERVED_1078)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1140,
    SIL_RESERVED_0872 |
    SIL_RESERVED_0874 |
    SIL_RESERVED_0876 |
    SIL_RESERVED_0878,
    (0x4 << SIL_RESERVED_0873) |
    (0x4 << SIL_RESERVED_0875) |
    (0x4 << SIL_RESERVED_0877) |
    (0x4 << SIL_RESERVED_0879)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1141,
    SIL_RESERVED_0880 |
    SIL_RESERVED_0882 |
    SIL_RESERVED_0884 |
    SIL_RESERVED_0886,
    (0x4 << SIL_RESERVED_0881) |
    (0x4 << SIL_RESERVED_0883) |
    (0x4 << SIL_RESERVED_0885) |
    (0x4 << SIL_RESERVED_0887)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_NBIF_MGCG_CLKGATING_ENABLED,
    SIL_RESERVED_1143,
    SIL_RESERVED_0897,
    (0x1 << SIL_RESERVED_0898)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_NBIF_MGCG_CLKGATING_ENABLED,
    SIL_RESERVED_1144,
    SIL_RESERVED_0905,
    (0x0 << SIL_RESERVED_0906)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_NBIF_MGCG_CLKGATING_DISABLED,
    SIL_RESERVED_1143,
    SIL_RESERVED_0897,
    (0x0 << SIL_RESERVED_0898)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1149,
    SIL_RESERVED_0975 |
    SIL_RESERVED_0973,
    (0x1 << SIL_RESERVED_0976) |
    (0x1 << SIL_RESERVED_0974)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1152,
    SIL_RESERVED_0991 |
    SIL_RESERVED_0989,
    (0x1 << SIL_RESERVED_0992) |
    (0x1 << SIL_RESERVED_0990)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1153,
    SIL_RESERVED_1003 |
    SIL_RESERVED_1001,
    (0x1 << SIL_RESERVED_1004) |
    (0x1 << SIL_RESERVED_1002)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1155,
    SIL_RESERVED_1057,
    (0x1 << SIL_RESERVED_1058)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_TPH_COMPLETER_ENABLED,
    SIL_RESERVED_1158,
    SIL_RESERVED_1090,
    (0x1 << SIL_RESERVED_1091)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1142,
    SIL_RESERVED_0891,
    0x1 << SIL_RESERVED_0892
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1144,
    SIL_RESERVED_0905,
    (0x0 << SIL_RESERVED_0906)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1154,
    SIL_RESERVED_1037,
    (0x1 << SIL_RESERVED_1038)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1157,
    SIL_RESERVED_1088,
    (0x1 << SIL_RESERVED_1089)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1159,
    SIL_RESERVED_1088,
    (0x1 << SIL_RESERVED_1089)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1145,
    SIL_RESERVED_0915,
    (0x14DC << SIL_RESERVED_0916)
    ),
  SMN_ENTRY_TERMINATE
};


// nbif init table common initialization
SMN_TABLE_ENTRY  GnbnBifInitTable [] = {
  SMN_ENTRY_TABLE_64(NbioNbifDmaArbitrationTbl),       // 15.6.7.5.4 DMA Arbitration
  SMN_ENTRY_TABLE_64(NbioNbifSyshubWrtPerfEnhTbl),     // SystemHub Write Performance Enhancement (Need confirm again)
  SMN_ENTRY_TABLE_64(NbioTphTbl),                      //tph
  SMN_ENTRY_TABLE_64(NbioNbifClockGatingTbl),          // 15.6.7.6.1 : nBIF Clock Gating Feature
  SMN_ENTRY_TABLE_64(NbioNbifFlrTbl),                  //atomic
  SMN_ENTRY_TABLE_64(NbioLclkDsIdleTbl),               //clk
  SMN_ENTRY_TABLE_64(NbioEnableSocClkShubClkDsTbl),    //soc clock syshub
  SMN_ENTRY_TABLE_64(NbioEnableCompletionTimeoutTbl),  //enable timeout completion
  SMN_ENTRY_TABLE_64(NbifDeviceIdDevTbl),              //set device id
  SMN_ENTRY_TABLE_64(GnbNbifDisablePtdma),             //disable ptdma
  SMN_ENTRY_TABLE_64(NbioDoeCapability),               //disable ptdma
  SMN_ENTRY_TERMINATE_TABLE
};
//all
/**--------------------------------------------------------------------
 * NbioNbifMpdmaCapability
 *
 * @brief Mpdma Functionality
 *
 * @details
 *
 * @param GnbHandle
 *
 **/
static
void
NbioNbifMpdmaCapability (
  GNB_HANDLE  *GnbHandle
  )
{
  uint8_t                             Index;
  /* Expose TPH Requester Capability for MPDMA,
   * and set up the TPH modes and where the ST Table locations are.
   * nBIF0, nBIF1, and nBIF2.
   */
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1116),
    (uint32_t) ~(SIL_RESERVED_0975 |
    SIL_RESERVED_0979 |
    SIL_RESERVED_0977),
    (0x1 << SIL_RESERVED_0976) |
    (0x2 << SIL_RESERVED_0980) |
    (0x1 << SIL_RESERVED_0978)
    );

  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1110),
    (uint32_t) ~(SIL_RESERVED_0937 |
    SIL_RESERVED_0939),
    (0x0 << SIL_RESERVED_0938) |
    (0x8 << SIL_RESERVED_0940)
    );
  /* Virtual function configuration
   * nBIF0, nBIF1, and nBIF2.
   */
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1111),
    (uint32_t) ~SIL_RESERVED_0941,
    ((uint32_t)0x1 << SIL_RESERVED_0942)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1112),
    (uint32_t) ~(SIL_RESERVED_0943 |
    SIL_RESERVED_0947 |
    SIL_RESERVED_0945),
    (0x1 << SIL_RESERVED_0944) |
    (0x8 << SIL_RESERVED_0948) |
    (0x2 << SIL_RESERVED_0946)
    );
  for (Index = 0; Index < 4; Index++) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1120 + ((Index * 2) << 8)),
      (uint32_t) ~(SIL_RESERVED_0995 |
      SIL_RESERVED_0993 |
      SIL_RESERVED_0991),
      (0x2 << SIL_RESERVED_0996) |
      (0x1 << SIL_RESERVED_0994) |
      (0x1 << SIL_RESERVED_0992)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1118 + ((Index * 2) << 8)),
      (uint32_t) ~(SIL_RESERVED_0983 |
      SIL_RESERVED_0985),
      (0x1 << SIL_RESERVED_0984) |
      (0x8 << SIL_RESERVED_0986)
      );
  }
}

/**--------------------------------------------------------------------
 * NbifDdrInitialCheck
 *
 * @brief  Function check NBIF DDR initial sequence
 *
 * @details
 *
 * @param GnbHandle
 * @param NbifDdrInitialCheck
 *
 **/
static
void
NbifDdrInitialCheck (
  GNB_HANDLE            *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{

  uint32_t        BaseAddrLow;
  uint32_t        BaseAddrHigh;

  if (NbioIpBlockData->NbioConfigData.AmdGnbNbifDdrInitEn != 0xFF) {
    return;
  }

  BaseAddrLow = 0;
  BaseAddrHigh = 0;

  // Check NBIF DDR Base address that assign at ABL phase
  BaseAddrHigh = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_10133288)
    );
  BaseAddrLow = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_10133284)
    );

  if (BaseAddrLow == 0 && BaseAddrHigh == 0) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "NBIF DDR Base address not assigned\n");
    NbioIpBlockData->NbioConfigData.AmdGnbNbifDdrInitEn = 0x00;
    return;
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "NBIF DDR Base address assigned\n");
  NbioIpBlockData->NbioConfigData.AmdGnbNbifDdrInitEn = 0x01;
}

/**--------------------------------------------------------------------
 * NbioNbif1RootPort
 *
 * @brief Root port strap selection
 *
 * @details For Nbif0,2-rbindex 0,2
 *
 * @param NbioClass_Data_Block
 * @param GnbHandle
 *
 **/
static
void
NbioNbif1RootPort (
  GNB_HANDLE  *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1135),
    (uint32_t) ~(SIL_RESERVED_1065 |
    SIL_RESERVED_1059 |
    SIL_RESERVED_1061),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1066) |
    (0x7 << SIL_RESERVED_1060) |
    (0x1 << SIL_RESERVED_1062)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1138),
    (uint32_t) ~(SIL_RESERVED_1098 |
    SIL_RESERVED_1092 |
    SIL_RESERVED_1094),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1099) |
    (0x7 << SIL_RESERVED_1093) |
    (0x2 << SIL_RESERVED_1095)
    );
}

/**--------------------------------------------------------------------
 * NbioNbifRootPort
 *
 * @brief Root port strap selection
 *
 * @details For Nbif0,2-rbindex 0,2
 *
 * @param FeatureIndex   Description of FeatureIndex
 * @param Enable         Description of Enable
 *
 **/
static
void
NbioNbifRootPort (
  GNB_HANDLE  *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{
  //nBIF ReqID
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1135),
    (uint32_t) ~(SIL_RESERVED_1065 |
    SIL_RESERVED_1059 |
    SIL_RESERVED_1061),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1066) |
    (0x7 << SIL_RESERVED_1060) |
    (0x1 << SIL_RESERVED_1062)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1138),
    (uint32_t) ~(SIL_RESERVED_1098 |
    SIL_RESERVED_1092 |
    SIL_RESERVED_1094),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1099) |
    (0x7 << SIL_RESERVED_1093) |
    (0x2 << SIL_RESERVED_1095)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1156),
    (uint32_t) ~(SIL_RESERVED_1065 |
    SIL_RESERVED_1059 |
    SIL_RESERVED_1061),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1066) |
    (0x7 << SIL_RESERVED_1060) |
    (0x1 << SIL_RESERVED_1062)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1053121C),
    (uint32_t) ~(SIL_RESERVED_1098 |
    SIL_RESERVED_1092 |
    SIL_RESERVED_1094),
    (GnbHandle->Address.Address.Bus << SIL_RESERVED_1099) |
    (0x7 << SIL_RESERVED_1093) |
    (0x2 << SIL_RESERVED_1095)
    );
  if (NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis != 0) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1104),
      (uint32_t) ~(SIL_RESERVED_0901),
      ((NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis) <<
        SIL_RESERVED_0902)
      );
  }

  /*
     Set the port number of the nBIF port to avoid a possible conflict with the CXL root bridge port number.
     Setting the nBIF Port Number will not conflict with PCIe port number assignments.
     When CXL is present on RB Index 0 and/or RB Index 2, its Port Number must be unique and
     cannot be duplicated by the nBIF port.
     Otherwise the OS CXL driver fails to setup the CXL topology.
   */
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1135),
    (uint32_t) ~(SIL_RESERVED_1063),
    (0x71 << SIL_RESERVED_1064)
    );
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1138),
    (uint32_t) ~(SIL_RESERVED_1096),
    (0x72 << SIL_RESERVED_1097)
    );
}


/**--------------------------------------------------------------------
 * Function Name
 *
 * @brief  NBIF common functions
 *
 * @details  Function to enable straps to expose some PCIe capability for MPDMA-TF
 *
 * @param GnbHandle
 * @param NBIOCLASS_DATA_BLOCK
 *
 **/
static
void
NbifCommonOptions (
  GNB_HANDLE            *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{
  //RCC_DEV0_EPF0_STRAP2
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1107),
    (uint32_t) ~(SIL_RESERVED_0923),
    (((NbioIpBlockData->NbioConfigData.CfgAriEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0924)
    );

  //RCC_DEV0_EPF1_STRAP2
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1113),
    (uint32_t) ~(SIL_RESERVED_0961 |
    SIL_RESERVED_0951 |
    SIL_RESERVED_0949 |
    SIL_RESERVED_0953 |
    SIL_RESERVED_0959 |
    SIL_RESERVED_0957),
    (((NbioIpBlockData->NbioConfigData.CfgSriovEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0962) |
    (((NbioIpBlockData->NbioConfigData.CfgAerEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0952) |
    (((NbioIpBlockData->NbioConfigData.CfgAcsEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0950) |
    (((NbioIpBlockData->NbioConfigData.CfgAtsEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0954) |
    (((NbioIpBlockData->NbioConfigData.CfgPasidEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0960) |
    (((NbioIpBlockData->NbioConfigData.CfgPriEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0958)
    );

  //RCC_DEV0_PORT_STRAP0
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1130),
    (uint32_t) ~(SIL_RESERVED_1023 |
    SIL_RESERVED_1025),
    (((NbioIpBlockData->NbioConfigData.AcsEnRccDev0) ?
    1 : 0) << SIL_RESERVED_1024) |
    (((NbioIpBlockData->NbioConfigData.AerEnRccDev0) ?
    1 : 0) << SIL_RESERVED_1026)
    );

  //RCC_BIF_STRAP1
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1105),
    (uint32_t) ~(SIL_RESERVED_0909 |
    SIL_RESERVED_0913 |
    SIL_RESERVED_0911),
    (((NbioIpBlockData->NbioConfigData.DlfEnStrap1) ? 1 : 0) <<
      SIL_RESERVED_0910) |
    (((NbioIpBlockData->NbioConfigData.Phy16gtStrap1) ? 1 : 0) <<
      SIL_RESERVED_0914) |
    (((NbioIpBlockData->NbioConfigData.MarginEnStrap1) ? 1 : 0) <<
      SIL_RESERVED_0912)
    );

  //RCC_DEV0_PORT_STRAP5
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1133),
    (uint32_t) ~(SIL_RESERVED_1045 |
    SIL_RESERVED_1041 |
    SIL_RESERVED_1043 |
    SIL_RESERVED_1047 |
    SIL_RESERVED_1049 |
    SIL_RESERVED_1051 |
    SIL_RESERVED_1039 |
    SIL_RESERVED_1055 |
    SIL_RESERVED_1053),
    (((NbioIpBlockData->NbioConfigData.AcsP2pReqStrap5) ? 1 : 0) <<
      SIL_RESERVED_1046) |
    (((NbioIpBlockData->NbioConfigData.AcsP2pCompStrap5) ? 1 : 0) <<
      SIL_RESERVED_1042) |
    (((NbioIpBlockData->NbioConfigData.AcsP2PEgressStrap5) ? 1 : 0) <<
      SIL_RESERVED_1044) |
    (((NbioIpBlockData->NbioConfigData.AcsSourceValStrap5) ? 1 : 0) <<
      SIL_RESERVED_1048) |
    (((NbioIpBlockData->NbioConfigData.AcsTranslationalBlockingStrap5) ? 1 : 0) <<
      SIL_RESERVED_1050) |
    (((NbioIpBlockData->NbioConfigData.AcsUpstreamFwdStrap5) ? 1 : 0) <<
      SIL_RESERVED_1052) |
    (((NbioIpBlockData->NbioConfigData.AcsDirectTranslatedStrap5) ? 1 : 0) <<
      SIL_RESERVED_1040) |
    (((NbioIpBlockData->NbioConfigData.AcsSsidEnStrap5) ? 1 : 0) <<
      SIL_RESERVED_1056) |
    (((NbioIpBlockData->NbioConfigData.AtomicRoutingEnStrap5) ? 1 : 0) <<
      SIL_RESERVED_1054)
    );

  //PCIE_PASID_CAP
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0856),
    (uint32_t) ~(SIL_RESERVED_0845 << 16),
    (((NbioIpBlockData->NbioConfigData.CfgPasidEnDev0F1)? 1 : 0) <<
      (SIL_RESERVED_0846 + 16))
    );

  //PCIE_ATS_CAP
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0854),
    (uint32_t) ~(SIL_RESERVED_0839 << 16),
    (((NbioIpBlockData->NbioConfigData.AtcEnable) ? 1 : 0) <<
      (SIL_RESERVED_0840 + 16))
    );

//PCIE_PAGE_REQ_CNTL
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0855),
    (uint32_t) ~(SIL_RESERVED_0841 |
    SIL_RESERVED_0843),
    (((NbioIpBlockData->NbioConfigData.PriEnPageReq) ? 1 : 0) <<
      SIL_RESERVED_0842) |
    (((NbioIpBlockData->NbioConfigData.PriResetPageReq) ? 1 : 0) <<
      SIL_RESERVED_0844)
    );

  //PCIE_ACS_CNTL
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0853),
    (uint32_t) ~(SIL_RESERVED_0833 |
    SIL_RESERVED_0835 |
    SIL_RESERVED_0837 |
    SIL_RESERVED_0829 |
    SIL_RESERVED_0831 |
    SIL_RESERVED_0827),
    ((((NbioIpBlockData->NbioConfigData.AcsSourceVal) ? 1 : 0) <<
      SIL_RESERVED_0834) |
    (((NbioIpBlockData->NbioConfigData.AcsTranslationalBlocking) ? 1 : 0) <<
      SIL_RESERVED_0836) |
    (((NbioIpBlockData->NbioConfigData.AcsUpstreamFwd) ? 1 : 0) <<
      SIL_RESERVED_0838) |
    (((NbioIpBlockData->NbioConfigData.AcsP2PEgress) ? 1 : 0) <<
      SIL_RESERVED_0830) |
    (((NbioIpBlockData->NbioConfigData.AcsP2pReq) ? 1 : 0) <<
      SIL_RESERVED_0832) |
    (((NbioIpBlockData->NbioConfigData.AcsP2pComp) ? 1 : 0) <<
      SIL_RESERVED_0828))
    );

//RCC_DEV0_EPF1_STRAP3
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1114),
    (uint32_t) ~(SIL_RESERVED_0963 |
    SIL_RESERVED_0965),
    (((NbioIpBlockData->NbioConfigData.CfgPwrEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0964) |
    (((NbioIpBlockData->NbioConfigData.CfgRtrEnDev0F1) ? 1 : 0) <<
      SIL_RESERVED_0966)
    );

  NbifDdrInitialCheck(GnbHandle, NbioIpBlockData);

  //RCC DEV0 PORT STRAP2
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_1131),
    (uint32_t) ~(SIL_RESERVED_1029 |
    SIL_RESERVED_1031),
    ((NbioIpBlockData->NbioConfigData.RccDev0E2EPrefix ? 1 : 0)
      << SIL_RESERVED_1030) |
    ((NbioIpBlockData->NbioConfigData.RccDev0ExtendedFmtSupported ? 1 : 0)
      << SIL_RESERVED_1032)
    );

  //DEVICE_CNTL2
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0852),
    (uint32_t) ~(SIL_RESERVED_0825),
    (((NbioIpBlockData->NbioConfigData.NbifDev0F1AtomicRequestEn)? 1 : 0) <<
      (SIL_RESERVED_0826))
    );
  if (NbioIpBlockData->NbioConfigData.CfgSriovEnDev0F1 == true) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_10134234),
      (uint32_t) ~(SIL_RESERVED_0935),
      (0x40 << SIL_RESERVED_0936)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1130),
      (uint32_t) ~(SIL_RESERVED_1027),
      (1 << SIL_RESERVED_1028)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1130),
      (uint32_t) ~(SIL_RESERVED_1027),
      (1 << SIL_RESERVED_1028)
      );
  }

  //
  // IOD C0 has been changed to allow LCLK DS when TF MPDMA SDP port is connected.
  // Clear bit 8 of the corresponding NBIF SYSHUBMM::GDC_HW_DEBUG
  //
  if (IS_SOC_BRH_Cx || IS_SOC_BRHD_Bx) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1403B98),
      (uint32_t) ~(SIL_RESERVED_1698),
      (0 << SIL_RESERVED_1699)
      );
  }

  return;
}

/**--------------------------------------------------------------------
 * NbioNbifInit
 *
 * @brief  This function does
 *
 * @details
 *
 * @param GnbHandle
 * @param Property
 * @param NBIOCLASS_DATA_BLOCK
 *
 **/
void
NbioNbifInit (
  GNB_HANDLE  *GnbHandle,
  uint32_t    Property,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "FUNCTION:NBIONBIFINIT\n");
  uint8_t rbindex = GnbHandle->RBIndex;
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "RBINDEX %d NBIFINIT\n", rbindex);
  switch (rbindex) {
  case 0:
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "RBINDEX %d NBIFINIT\n", GnbHandle->RBIndex);
    ProgramNbioSmnTable(GnbHandle, NbioNbif0Iohc0DeviceEnTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbifSataRevTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif2InitTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif0DeviceInterruptTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif0PoolCredTbl, NBIO_SPACE(GnbHandle, 0), Property);
    NbioNbifMpdmaCapability(GnbHandle);
    NbioNbifRootPort(GnbHandle, NbioIpBlockData);
    NbifCommonOptions(GnbHandle, NbioIpBlockData);
    if (NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_1138),
        (uint32_t) ~(SIL_RESERVED_0901),
        ((NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis) <<
          SIL_RESERVED_0902)
        );
    }
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1102),
      (uint32_t) ~(SIL_RESERVED_0888),
      ((0x33 << SIL_RESERVED_0889) |
      (0x3 << SIL_RESERVED_0890))
      );
    break;
  case 1:
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "RBINDEX %d NBIFINIT\n", GnbHandle->RBIndex);
    ProgramNbioSmnTable(GnbHandle, NbioNbifDeviceEnTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif1DeviceInterruptTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif1PoolCredTbl, NBIO_SPACE(GnbHandle, 0), Property);
    NbioNbifMpdmaCapability(GnbHandle);
    NbioNbif1RootPort(GnbHandle, NbioIpBlockData);
    NbifCommonOptions(GnbHandle, NbioIpBlockData);
    //Enable PSP/CCP in nbio nbif1
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1125),
      (uint32_t) ~(SIL_RESERVED_1013),
      (0x1 << SIL_RESERVED_1014)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1102),
      (uint32_t) ~(SIL_RESERVED_0888),
      ((0x33 << SIL_RESERVED_0889) |
      (0x3 << SIL_RESERVED_0890))
      );
    break;
  case 2:
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "RBINDEX %d NBIFINIT\n", GnbHandle->RBIndex);
    ProgramNbioSmnTable(GnbHandle, NbioNbifDeviceEnTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif2InitTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif0DeviceInterruptTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif0PoolCredTbl, NBIO_SPACE(GnbHandle, 0), Property);
    NbioNbifMpdmaCapability(GnbHandle);
    NbioNbifRootPort(GnbHandle, NbioIpBlockData);
    NbifCommonOptions(GnbHandle, NbioIpBlockData);
    if (NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RESERVED_1138),
        (uint32_t) ~(SIL_RESERVED_0901),
        ((NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis) <<
          SIL_RESERVED_0902)
        );
    }
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1102),
      (uint32_t) ~(SIL_RESERVED_0888),
      ((0x33 << SIL_RESERVED_0889) |
      (0x3 << SIL_RESERVED_0890))
      );
    break;
  case 3:
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "RBINDEX %d NBIFINIT\n", GnbHandle->RBIndex);
    ProgramNbioSmnTable(GnbHandle, NbioNbif1Iohc2DeviceEnTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbifSataRevTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif1DeviceInterruptTbl, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, NbioNbif1PoolCredTbl, NBIO_SPACE(GnbHandle, 0), Property);
    NbioNbifMpdmaCapability(GnbHandle);
    NbioNbif1RootPort(GnbHandle, NbioIpBlockData);
    NbifCommonOptions(GnbHandle, NbioIpBlockData);
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1102),
      (uint32_t) ~(SIL_RESERVED_0888),
      ((0x33 << SIL_RESERVED_0889) |
      (0x3 << SIL_RESERVED_0890))
      );
    break;
  default:
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "NBIFINIT INVALID RBINDEX %d\n", GnbHandle->RBIndex);
  }
}
