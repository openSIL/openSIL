/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioCfgPointsPhx.c
 *  @brief Callouts from MPIO initialization for PHX
 */

#include <string.h>
#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <Utils.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/Common/MpioPcie.h>
#include <SilSocLogicalId.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_NBIOBASEREG.h>
#include <Nbio/Phx/includePHX/PHX_PCIERCCFG.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL2.h>
#include <Nbio/Phx/includePHX/PHX_NBIFEPFNCFG.h>
#include <Nbio/Phx/includePHX/PHX_NBIFEPFCFG.h>
#include <Nbio/Phx/includePHX/PHX_IOAPIC.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>
#include <Nbio/Phx/includePHX/PHX_NBIFMM.h>
#include <Nbio/NbioIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include "MpioPcieStrapsPhx.h"
#include "MpioCmn2Phx.h"
#include "MpioPhxData.h"

/**
 * @brief MPIO ANCILLARY DATA FOR PSPP
 * @details
 */
#define PsppThreshold0Percent     0
#define PsppThreshold5Percent     1
#define PsppThreshold10Percent    2
#define PsppThreshold15Percent    3
#define PsppThreshold20Percent    4
#define PsppThreshold25Percent    5
#define PsppThreshold30Percent    6
#define PsppThreshold40Percent    7
#define PsppThreshold50Percent    8
#define PsppThreshold60Percent    9
#define PsppThreshold70Percent    10
#define PsppThreshold75Percent    11
#define PsppThreshold80Percent    12
#define PsppThreshold90Percent    13
#define PsppThreshold95Percent    14
#define PsppThreshold100Percent   15

typedef struct {
  uint8_t         DevMap;
  uint8_t         LogicalBrId;
  uint32_t        InterruptRountingData;
} IOAPIC_BR_INTERRUPT;

typedef struct {
  uint8_t         field0;
  uint8_t         field1;
} SIL_RESERVED_STRUCT_0014;

typedef struct {
  GENERIC_ANC_HEADER      header;
  uint16_t                field1;
  uint16_t                field2;
  uint16_t                field3;
  uint16_t                reserved;
} SIL_RESERVED_STRUCT_0002;

typedef struct {
  MPIO_PORT_DESCRIPTOR    *PortList;
  uint32_t                field0;
} SIL_RESERVED_STRUCT_0020;

typedef struct {
  void                    *StrapList;
  MPIO_PORT_DESCRIPTOR    *PortList;
  SIL_RESERVED_STRUCT_0002           *ancList;
  uint32_t                field0;
} SIL_RESERVED_STRUCT_0018;

typedef struct {
  GENERIC_ANC_HEADER                field0;
  MPIO_STRAPENTRY_SHORT_PERPORT     field1[11];
} SIL_RESERVED_STRUCT_0021;

typedef struct {
  GENERIC_ANC_HEADER                header;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field1;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field2;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field3;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field4;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field5;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field6;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field7;
} SIL_RESERVED_STRUCT_0001;

typedef struct {
  GENERIC_ANC_HEADER                field0;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field1;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field2;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field3;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field4;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field5;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field6;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field7;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field8;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field9;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field10;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field11;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field12;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field13;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field14;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field15;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field16;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field17;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field18;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field19;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field20;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field21;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field22;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field23;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field24;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field25;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field26;
  MPIO_STRAPENTRY_LONG_CTRLLER      field27;
  MPIO_STRAPENTRY_LONG_CTRLLER      field28;
} SIL_RESERVED_STRUCT_0007;

typedef struct {
  GENERIC_ANC_HEADER                field0;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field1;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field2;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field3;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field4;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field5;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field6;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field7;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field8;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field9;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field10;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field11;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field12;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field13;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field14;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field15;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field16;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field17;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field18;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field19;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field20;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field21;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field22;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field23;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field24;
  MPIO_STRAPENTRY_SHORT_CTRLLER     field25;
  MPIO_STRAPENTRY_SHORT_ALLPORT     field26;
} SIL_RESERVED_STRUCT_0025;

typedef enum {
  shortCtrllerStrap = 0,
  shortPerPortStrap = 1,
  shortAllPortStrap = 2,
  longCtrllerStrap = 3,
  longPerPortStrap = 4,
  longAllPortStrap = 5,
  extStrap = 6,
} MPIO_STRAPENTRYTYPE;

SIL_RESERVED_STRUCT_0002 AncListData = {
  {ANC_LIST, 0, (uint8_t) (sizeof (SIL_RESERVED_STRUCT_0002) >> 2), 0},
  0, 0, 0, 0
};

SIL_RESERVED_STRUCT_0021 PerPortStraps = {
  MPIO_ANC_PERPORT_STRAP_HEADER,
  { {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx10A
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx15C
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx15A
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx158
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx15B
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx159
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx15E
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx15F
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx161
    {SHORT_DATA, 0, 0},  ///< MPIOSTRAPx138
    {SHORT_DATA, 0, 0}   ///< MPIOSTRAPx139
  }
};

SIL_RESERVED_STRUCT_0001 AllPortStraps = {
  MPIO_ANC_ALLPORT_STRAP_HEADER,
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx10C, 7),
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx94, 1),
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx163, 0),  ///<  Default to 0, update optional
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx164, 0),  ///<  Default to 0, update optional
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx145, 0),  ///<  Default to 0, update optional
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx146, 0),  ///<  Default to 0, update optional
  MPIO_SHORT_ALLPORT_STRAP(MPIOSTRAPx10B, 0x6) ///<  Default to 0x6, update optional
};

SIL_RESERVED_STRUCT_0007     CtrllerStraps = {
  MPIO_ANC_CTRLLER_STRAP_HEADER,
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx60, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxB5, 3),                              ///<  Set default 3
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxB6, 3),                              ///<  Set default 3
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxB7, 4),                              ///<  Set default 4
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxB8, 4),                              ///<  Set default 4
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx7C, 0),                              ///<  Default to 2, update required
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx5D, 0),                              ///<  Always 0
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxE, 1),                               ///<  Always 1
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx75, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx76, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx77, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx78, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx79, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx7A, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx7B, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx21, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxE9, 0),                              ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx13, 0),                               ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx12, 0),                               ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx3E, 1),                               ///<  Default to 1, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx88, 0),                               ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx89, 0),                               ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx5F, 1),                               ///<  Default to 1, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx6, 0),                                ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx4A, 0),                               ///<  Default to 0, update optional
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx4B, 0),                               ///<  Default to 0, update optional
  MPIO_LONG_CTRLLER_STRAP(MPIOSTRAPxB4, 0x1234),                           ///<
  MPIO_LONG_CTRLLER_STRAP(MPIOSTRAPxB3, 0x1022),                           ///<
};

bool AddUsb4CtrllerStrap = true;

SIL_RESERVED_STRUCT_0025 USB4CtrllerStraps = {
  MPIO_ANC_CTRLLER_STRAP_HEADER,
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx7C, 0x22),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx6, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxE, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx14, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx15, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx16, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx21, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx4A, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx4B, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx5F, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx60, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx88, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx89, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxB4, 0x1453),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPxFD, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx10B, 2),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx10C, 7),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx114, 0),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx13F, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx140, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx145, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx146, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx154, 0xff),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx163, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx164, 1),
  MPIO_SHORT_CTRLLER_STRAP(MPIOSTRAPx6F, 0x0)
};

/**--------------------------------------------------------------------
 *
 * MpioCfgGlobalConfigPhx
 *
 * @brief Function called from MpioLib to update GLOBAL CONFIG
 *
 * @details This function provides platform-specific initialization of the MPIO_GLOBAL_CONFIG
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           GNB_HANDLE pointer
 * @param   ArgList             Pointer to array of arguments  cast as MPIO_GLOBAL_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgGlobalConfigPhx (
  SIL_CONTEXT                   *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  )
{
  MPIO_GLOBAL_CONFIG        *GlobalConfig;
  MPIOCLASS_PHX_INPUT_BLK   *SilDataPhx;

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  GlobalConfig = (MPIO_GLOBAL_CONFIG *) ArgList;

  /*
   * Clock Gating
   */
  if (SilDataCommon->CfgDxioClockGating) {
    GlobalConfig->PWRMNGMT_PRFRM_CLK_GATING = 1;
  }
  /*
   *  PHY Power Gating in L1.2
   */
  if (SilDataPhx->CfgDxioRefClkShutDown) {
    GlobalConfig->PWRMNGMT_PRFRM_REFCLK_SHUTDOWN = 1;
    if (SilDataPhx->CfgDxioPmaPowerGating) {
      GlobalConfig->PWRMNGMT_PRFRM_PMA_POWER_GATING = 1;
    }
    if (SilDataPhx->CfgDxioPmaClockGating) {
      GlobalConfig->PWRMNGMT_PRFRM_PMA_CLOCK_GATING = 1;
    }
  }

  if (SilDataPhx->CfgDxioStaticPowerGating) {
    GlobalConfig->PWRMNGMT_PRFRM_STATIC_PWR_GATING = 1;
  }
  /*
   * Training Timers
   */
  if (SilDataCommon->PcieDxioTimingControlEnable) {
    GlobalConfig->LinkReceiverDetectionPolling = SilDataCommon->PCIELinkReceiverDetectionPolling;
    GlobalConfig->LinkResetToTrainingTime = SilDataCommon->PCIELinkResetToTrainingTime;
    GlobalConfig->LinkL0Polling = SilDataCommon->PCIELinkL0Polling;
  }

  /*
   * Exact Match
   */
  if (SilDataCommon->PCIeExactMatchEnable) {
    GlobalConfig->matchPortSizeExactly = 1;
  }

  /*
   * Valid PHY firmware
   */
  if (SilDataCommon->DxioPhyValid == 0) {
    GlobalConfig->ValidPhyFWFlag = 0;
  }

  /*
   * Use PHY SRAM
   */
  if (SilDataCommon->DxioPhyProgramming == 0) {
    GlobalConfig->usePhySram = 0;
  }

  /*
   * Skip Vetting
   */
  if (SilDataCommon->CfgSkipPspMessage == 1) {
    GlobalConfig->skipVetting = 1;
  }

  if (SilDataCommon->DxioSaveRestoreModes == 1) {
    GlobalConfig->saverestoremode = 1;
  }

  if (SilDataPhx->CfgPcieLoopbackMode) {
    GlobalConfig->enableLoopbackSupport = 1;
  }
}

/*
 *=========================================================================================
 * Timepoint after port mapping and before reconfig
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 *
 * MpioCfgBeforeReconfigEventPhx
 *
 * @brief Per-Engine function call for port configuration before bifurcation
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Engine configuration info
 * @param   Buffer              Buffer pointer
 * @param   Pcie                PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgBeforeReconfigEventPhx (
  SIL_CONTEXT                       *SilContext,
  PCIe_ENGINE_CONFIG                *Engine,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  uint8_t                   DynLanesPwrState;
  GNB_HANDLE                *GnbHandle;
  uint16_t                  Index;
  MPIOCLASS_PHX_INPUT_BLK   *SilDataPhx;
  NBIO_IP2IP_API            *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Engine->Type.Port.EqPreset = Engine->Type.Port.EqSearchMode;
  if (PcieLibIsEngineAllocated(Engine)) {
    /*
     * These are all programmed per engine
     * Gen3 Bypass Phase3 EQ
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140390),
      (uint32_t) ~(BIT9_MASK_AAAAAFM),
      Engine->Type.Port.BypassGen3EQ << BIT9_OFFSET_AAAAAFN
      );

    /*
     * Gen3 Bypass Phase2/3 EQ
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140390),
      (uint32_t) ~(BIT6_MASK_AAAAAFQ),
      Engine->Type.Port.DisGen3EQPhase << BIT6_OFFSET_AAAAAFR
      );

    /*
     * Gen4 Bypass Phase3 EQ
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140394),
      (uint32_t) ~(BIT9_MASK_AAAAAFG),
      Engine->Type.Port.BypassGen4EQ << BIT9_OFFSET_AAAAAFH
      );

    /*
     * Gen4 Bypass Phase2/3 EQ
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140394),
      (uint32_t) ~(BIT6_MASK_AAAAAFK),
      Engine->Type.Port.DisGen4EQPhase << BIT6_OFFSET_AAAAAFL
      );

    Engine->Type.Port.EqPreset = Engine->Type.Port.EqSearchMode;

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "addr: <%08x>, eqpre: <%d>\n",
      GnbHandle->Address.AddressValue,
      Engine->Type.Port.EqPreset
      );

    /*
     * Gen3 EQ Search Mode
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140390),
      (uint32_t) ~(BITS_2_TO_3_MASK_AAAAAFO),
      Engine->Type.Port.EqSearchMode << BIT2_OFFSET_AAAAAFP
      );

    /*
     * Gen4 EQ Search Mode
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), NBIOBASEREGx11140394),
      (uint32_t) ~(BITS_2_TO_3_MASK_AAAAAFI),
      Engine->Type.Port.EqSearchModeGen4 << BIT2_OFFSET_AAAAAFJ
      );

    /*
     * Data Link feature (DLF)
     */
    if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 0) {
      Index = MPIOSTRAPx163 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapPhx(SilContext, GnbHandle, MPIOSTRAPx13, 1, Wrapper->WrapId);
      WritePcieStrapPhx(SilContext, GnbHandle, Index, 1, Wrapper->WrapId);
    } else if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 1) {
      Index = MPIOSTRAPx163 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
      WritePcieStrapPhx(SilContext, GnbHandle, MPIOSTRAPx13, 1, Wrapper->WrapId);
      WritePcieStrapPhx(SilContext, GnbHandle, Index, 0, Wrapper->WrapId);
    } else if (Engine->Type.Port.PortCapabilities.DataLinkFeature == 2) {
      WritePcieStrapPhx(SilContext, GnbHandle, MPIOSTRAPx13, 0, Wrapper->WrapId);
    }

    /*
     * Data Link feature Exchange Control(DLF)
     */
    Index = MPIOSTRAPx164 + (Engine->Type.Port.PortId * STRAP_BIF_PORT_DIFF);
    WritePcieStrapPhx(SilContext,
      GnbHandle,
      Index,
      Engine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl,
      Wrapper->WrapId
      );

    /*
     * SELECTABLE_DEEMPHASIS
     */
    if (Engine->Type.Port.TXDeEmphasis == 1) {
      WritePcieStrapPhx(SilContext,
        GnbHandle,
        MPIOSTRAPx114 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF),
        1,
        Wrapper->WrapId
        );
    } else if (Engine->Type.Port.TXDeEmphasis == 2) {
      WritePcieStrapPhx(SilContext,
        GnbHandle,
        MPIOSTRAPx114 + ((Engine->Type.Port.PortId) * STRAP_BIF_PORT_DIFF),
        0,
        Wrapper->WrapId
        );
    }
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId % 8),
      NBIOBASEREGx11140378
      ),
      (uint32_t) ~(BIT1_MASK_AAAAAFA),
      (((SilDataPhx->AmdLcLoopbackWaitForAllActiveLanes == true) ? 1 : 0) \
        << BIT1_OFFSET_AAAAAFB)
      );

    if (SilDataPhx->CfgDxioAllowCompPass) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId % 8),
        NBIOBASEREGx11140600
        ),
        (uint32_t) ~(BIT20_MASK_AAAAAGO),
        (0x1 << BIT20_OFFSET_AAAAAGP)
        );
    }

    /*
     * LC Dynamic Lanes Power State
     */
    DynLanesPwrState = (Engine->Type.Port.PortFeatures.DynLanesPwrState != 0xFF) ?
      Engine->Type.Port.PortFeatures.DynLanesPwrState : SilDataPhx->CfgDynamicLanesPowerState;
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId % 8),
      NBIOBASEREGx11140288
      ),
      (uint32_t) ~(BITS_21_TO_22_MASK_AAAAAFY |
      BIT30_MASK_AAAAAGA),
      ((DynLanesPwrState & 0x3) << BIT21_OFFSET_AAAAAFZ) |
      ((Engine->Type.Port.PortFeatures.TurnOffUnusedLanes & 0x1) <<
        BIT30_OFFSET_AAAAAGB)
      );
  }
}

/**-------------------------------------------------------------------
 * MpioCfgWrapperBeforeBifurcationPhx
 *
 * @brief Per-Wrapper function call for wrapper configuration before bifurcation
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   Buffer              Buffer pointer
 * @param   GnbHandle           GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgWrapperBeforeBifurcationPhx (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  NBIO_IP2IP_API        *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioCfgBeforeReconfigEventPhx,
    NULL,
    Wrapper
    );
}

/*
 *=========================================================================================
 * Timepoint after reconfig and before port training
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfigEventPhx
 *
 * @brief Per-Engine function call for configuration after bifurcation
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Engine configuration info
 * @param   Buffer              Buffer pointer
 * @param   Pcie                PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgAfterReconfigEventPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_WRAPPER_CONFIG   *Wrapper
  )
{
  GNB_HANDLE            *GnbHandle;
  uint32_t              Value32;
  PCIe_DPC_STATUS_DATA  *DpcStatusData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));

  if (((AFTER_RECONFIG_BUFFER *) Buffer)->DpcStatusData != NULL) {
    DpcStatusData = (((AFTER_RECONFIG_BUFFER *) Buffer)->DpcStatusData);
    Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100388)
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "SocketId=%d, DieNumber=%d, BRIndex=%d Bus=0x%x CoreID=%d PortID=%d PCIE_DPC_STATUS(0x388) = 0x%x\n",
      GnbHandle->SocketId,
      GnbHandle->DieNumber,
      GnbHandle->RBIndex,
      GnbHandle->Address.Address.Bus,
      Wrapper->WrapId,
      (Engine->Type.Port.PortId % 8),
      Value32
      );
    if (Value32 & BIT_32(0)) {
      if (DpcStatusData->size < MAX_NUMBER_DPCSTATUS) {
        DpcStatusData->DpcStatusArray[DpcStatusData->size].SocketId = (uint8_t) GnbHandle->SocketId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DieID = (uint8_t) GnbHandle->DieNumber;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].RBIndex = (uint8_t) GnbHandle->RBIndex;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].BusNumber = (uint8_t) GnbHandle->Address.Address.Bus;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIeCoreID = (uint8_t) Wrapper->WrapId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIePortID = (uint8_t) (Engine->Type.Port.PortId % 8);
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DpcStatus = (uint16_t) Value32;

        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100388),
          Value32
          );
        DpcStatusData->size++;
      }
    }
  }

  if (PcieLibIsEngineAllocated(Engine)) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140600),
      (uint32_t) ~(BIT15_MASK_AAAAAGQ),
      0 << BIT15_OFFSET_AAAAAGR
      );
  }
}

/**-------------------------------------------------------------------
 * MpioCfgWrapperAfterBifurcationPhx
 *
 * @brief Per-Wrapper function call for configuration after bifurcation
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   Buffer              Buffer pointer
 * @param   GnbHandle           GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgWrapperAfterBifurcationPhx (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  MPIOCLASS_COMMON_INPUT_BLK   *SilDataCommon;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper(SilContext,
    DESCRIPTOR_ALL_ENGINES,
    MpioCfgAfterReconfigEventPhx,
    Buffer,
    Wrapper
    );
  if (SilDataCommon->CfgNbioCTOtoSC) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx11180080),
      (uint32_t) ~(BIT29_MASK_AAAAAEI),
      1 << BIT29_OFFSET_AAAAAEJ
      );
  }

  if (SilDataCommon->CfgNbioCTOIgnoreError) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx11180080),
      (uint32_t) ~(BIT31_MASK_AAAAAEG),
      (uint32_t)  (1 << BIT31_OFFSET_AAAAAEH)
      );
  }
}

/**-------------------------------------------------------------------
 *
 * SubsystemIdSettingPhx
 *
 * @brief PCIE interface to configure register setting after Mpio init done
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
SubsystemIdSettingPhx (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{

  uint32_t                     Value;
  MPIOCLASS_COMMON_INPUT_BLK   *SilDataCommon;
  MPIOCLASS_PHX_INPUT_BLK      *SilDataPhx;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_PHX_INSTANCE
    );
  assert(SilDataPhx != NULL);

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    " Enter for Socket %d Nbio %d\n",
    GnbHandle->SocketId,
    GnbHandle->RBIndex
    );

  /*
   * NB ADAPTER D0F0
   */
  Value = SilDataCommon->CfgNbioSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNbioSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, IOHCx13b00050),
      Value
      );
  }

  /*
   * IOMMU
   */
  Value = SilDataCommon->CfgIommuSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgIommuSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, IOMMUL2x13f00078),
      Value
      );
  }

  /*
   * NBIF Root Bridge Functions
   */
  Value = SilDataPhx->CfgNbifRCSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgNbifRCSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10131004),
      Value
      );
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10131204),
      Value
      );
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10131404),
      Value
      );
  }

  /*
   * GFX
   */
  Value = SilDataPhx->AmdCfgGnbIGPUSSID;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "AmdCfgGnbIGPUSSID = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFEPFCFGx1014004C),
      Value
      );
  }

  /*
   * GFX Audio SSID
   */
  Value = SilDataPhx->AmdCfgGnbIGPUAudioSSID;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "AmdCfgGnbIGPUAudioSSID = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFEPFCFGx1014104C),
      Value
      );
  }

  /*
   * PSPCCP  - F2
   */
  Value = SilDataCommon->CfgPspccpSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgPspccpSsid = %x\n", Value);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFEPFNCFGx1014204C),
      Value
      );
  }

  if (SilDataPhx->AcpController) {
    Value = SilDataPhx->CfgAcpSsid;
    if (Value != 0) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "CfgAcpSsid = %x\n", Value);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFEPFNCFGx1014504C),
        Value
        );
    }
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 * PcieCommonCoreConfigurationPhx
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                PCIe configuration info
 *
 * @returns SIL_STATUS
 * @retval Should always return SilPass
 **/
static
SIL_STATUS
PcieCommonCoreConfigurationPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                       *GnbHandle;
  uint32_t                         Value;
  MPIOCLASS_COMMON_INPUT_BLK       *SilDataCommon;
  NBIO_IP2IP_API                   *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  if (Wrapper->WrapId != 0xFF) {
    GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header)));

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "Enter for Socket %d Nbio %d Wrapper %d\n",
      GnbHandle->SocketId,
      GnbHandle->RBIndex,
      Wrapper->WrapId
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx11180040),
      (uint32_t) ~(BIT17_MASK_AAAAAEK |
      BIT21_MASK_AAAAAEM),
      (0x0 << BIT17_OFFSET_AAAAAEL |
        0x1 << BIT21_OFFSET_AAAAAEN)
      );


    if (SilDataCommon->AmdNbioReportEdbErrors) {
      Value = 0;
    } else {
      Value = 1;
    }
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx11180100),
      (uint32_t) ~(BIT6_MASK_AAAAAGI |
      BITS_14_TO_15_MASK_AAAAAGG),
      Value << BIT6_OFFSET_AAAAAGJ |
        0x1 << BIT14_OFFSET_AAAAAGH
      );

    Value = (Wrapper->WrapId == 0) ? (uint32_t) SilDataCommon->GppAtomicOps : (uint32_t) SilDataCommon->GfxAtomicOps;
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx111802c0),
      (uint32_t) ~(BIT18_MASK_AAAAAGK |
      BIT20_MASK_AAAAAGM),
      (Value << BIT18_OFFSET_AAAAAGL) |
      (Value << BIT20_OFFSET_AAAAAGN)
      );
  }
  return SilPass;
}

/**-------------------------------------------------------------------
 * DumpEnginePcieLcStatePhx
 *
 * @brief Routine to dump PCIe LC STATE
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param    Engine             PCIe Engine info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
DumpEnginePcieLcStatePhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine
  )
{
  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  SIL_RESERVED_UNION_0017       PcieLCState;
  uint32_t                    Index;
  NBIO_IP2IP_API              *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Core%d Port%d[B%d:D%d:F%d]:\n",
    Wrapper->WrapId,
    Engine->Type.Port.PortId,
    GnbHandle->Address.Address.Bus,
    Engine->Type.Port.PortData.DeviceNumber,
    Engine->Type.Port.PortData.FunctionNumber
    );
  for (Index = 0; Index <= 5; Index++) {
    PcieLCState.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId % 8),
      NBIOBASEREGx11140294 + Index * 4
      )
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "LC_STATE%d = 0x%08x\n",
      Index,
      PcieLCState.Value
      );
  }
}

/**-------------------------------------------------------------------
 *
 * PcieCommonEngineConfigurationPhx
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @details tbd
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
PcieCommonEngineConfigurationPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  SIL_RESERVED_UNION_0020 TxRequesterId;
  MPIOCLASS_PHX_INPUT_BLK     *SilDataPhx;
  NBIO_IP2IP_API              *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  DumpEnginePcieLcStatePhx(SilContext, Engine);

  if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
    ((Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled) &&
    (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugInboard))) {

    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      " Enter for Socket %d Nbio %d Wrapper %d Engine\n",
      GnbHandle->SocketId,
      GnbHandle->RBIndex,
      Wrapper->WrapId,
      Engine->Type.Port.PortId
      );

    TxRequesterId.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140084)
      );

    TxRequesterId.Field.field_bits_0_to_2 = Engine->Type.Port.PortData.FunctionNumber;
    TxRequesterId.Field.field_bits_3_to_7 = Engine->Type.Port.PortData.DeviceNumber;
    TxRequesterId.Field.field_bits_8_to_15 = GnbHandle->Address.Address.Bus;

    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140084),
      TxRequesterId.Value
      );

    /*
     * Set slot_implemented
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100058),
      (uint32_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
      (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16))
      );

    /*
     * Set Physical Slot Number and Slot Power Limit
     */
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "SlotNum %x SlotPowerLimit %x SlotPowerLimitScale %x\n",
      Engine->Type.Port.PortData.SlotNum,
      Engine->Type.Port.SlotPowerLimit,
      Engine->Type.Port.SlotPowerLimitScale
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx1110006c),
      (uint32_t) ~(SLOT_CAP_PHYSICAL_SLOT_NUM_MASK |
      BITS_7_TO_14_MASK_AAAAAZW |
      SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK),
      (Engine->Type.Port.PortData.SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET) |
      (Engine->Type.Port.SlotPowerLimit << SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET) |
      (Engine->Type.Port.SlotPowerLimitScale << SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET)
      );

    /*
     * Set Completion Timeout
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100080),
      (uint32_t) ~(BITS_0_TO_3_MASK_AAAAAYV),
      (0x6 << BIT0_OFFSET_AAAAAYW)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140304),
      (uint32_t) ~(BIT2_MASK_AAAAAEE |
      BIT1_MASK_AAAAAEC),
      (1 << BIT2_OFFSET_AAAAAEF) |
      (1 << BIT1_OFFSET_AAAAAED)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140280),
      (uint32_t) ~(BIT23_MASK_AAAAAFC |
      BIT16_MASK_AAAAAFE),
      (1 << BIT23_OFFSET_AAAAAFD) |
      (0 << BIT16_OFFSET_AAAAAFF)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140288),
      (uint32_t) ~(BIT19_MASK_AAAAAFW),
      (0x1 << BIT19_OFFSET_AAAAAFX)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402c4),
      (uint32_t) ~(BITS_14_TO_15_MASK_AAAAAES |
      BIT17_MASK_AAAAAEO |
      BIT18_MASK_AAAAAEQ),
      (0x1 << BIT14_OFFSET_AAAAAET) |
      (0x1 << BIT17_OFFSET_AAAAAEP) |
      (0x1 << BIT18_OFFSET_AAAAAER)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402d4),
      (uint32_t) ~(BIT12_MASK_AAAAAEU),
      (0x1 << BIT12_OFFSET_AAAAAEV)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140284),
      (uint32_t) ~(BIT6_MASK_AAAAAGE),
      1 << BIT6_OFFSET_AAAAAGF
      );

    /*
     * Set Tx Margin
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100088),
      (uint32_t) ~(BITS_7_TO_9_MASK_AAAAAYX),
      (Engine->Type.Port.TXMargin << 7)
      );

    /*
     * Set Powerdown state in L1/L1.1/L1.2
     */
    if (Engine->Type.Port.PortFeatures.L1PowerDown != 0xFF) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402d8),
        (uint32_t) ~(BIT4_MASK_AAAAAEY),
        ((Engine->Type.Port.PortFeatures.L1PowerDown & 1) << BIT4_OFFSET_AAAAAEZ)
        );
    }
    if (Engine->Type.Port.PortFeatures.L11PowerDown != 0xFF) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140318),
        (uint32_t) ~(BITS_16_TO_18_MASK_AAAAAFS),
        ((Engine->Type.Port.PortFeatures.L11PowerDown & 7) << BIT16_OFFSET_AAAAAFT)
        );
    }
    if (Engine->Type.Port.PortFeatures.L12PowerDown != 0xFF) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140318),
        (uint32_t) ~(BITS_20_TO_22_MASK_AAAAAFU),
        ((Engine->Type.Port.PortFeatures.L12PowerDown & 7) << BIT20_OFFSET_AAAAAFV)
        );
    }

    /*
     * Program before DXIO training for compatibility issue
     */
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402d4),
      (uint32_t) ~(BIT14_MASK_AAAAAEW),
      (0x1 << BIT14_OFFSET_AAAAAEX)
      );

    if (Engine->Type.Port.PortFeatures.AutoSpdChngEn != 0xFF) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140414),
        (uint32_t) ~(BIT12_MASK_AAAAAGC),
        ((Engine->Type.Port.PortFeatures.AutoSpdChngEn & 1) <<
          BIT12_OFFSET_AAAAAGD)
        );
    }
  }
}

/**-------------------------------------------------------------------
 * InitBusRangesAndAriPhx
 *
 * @brief After Pcie Training Enumerate all Pcie connectors for register setting.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to GnbHandle
 * @param   Pcie                PCIe configuration info
 * @param   CfgPcieAriSupport   ARI support
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
InitBusRangesAndAriPhx (
  SIL_CONTEXT               *SilContext,
  GNB_HANDLE                *GnbHandle,
  PCIe_PLATFORM_CONFIG      *Pcie,
  bool                      CfgPcieAriSupport
  )
{
  GNB_HANDLE                      *InnerHandle;
  PCIE_VDM_NODE_CTRL4_STRUCT      VdmNode;
  uint32_t                        SystemFabricID;
  uint32_t                        BusNumberBase;
  uint32_t                        BusNumberLimit;
  uint32_t                        VdmNodeAddress;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS                      Status;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Enter for Socket %d RB %d\n", GnbHandle->SocketId, GnbHandle->RBIndex);

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)&DfIp2IpApi);
  assert(Status == SilPass);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  InnerHandle = NbioIp2Ip->NbioGetHandle(Pcie);
  while (InnerHandle != NULL) {
    DfIp2IpApi->DfGetRootBridgeInfo(SilContext,
      InnerHandle->SocketId,
      InnerHandle->DieNumber,
      InnerHandle->RBIndex,
      &SystemFabricID,
      &BusNumberBase,
      &BusNumberLimit,
      NULL,
      NULL,
      NULL
      );
    VdmNode.Field.BUS_RANGE_BASE = BusNumberBase;
    VdmNode.Field.BUS_RANGE_LIMIT = BusNumberLimit;
    if (InnerHandle->InstanceId == GnbHandle->InstanceId) {
      VdmNode.Field.NODE_PRESENT = 0;
    } else {
      VdmNode.Field.NODE_PRESENT = 1;
    }
    VdmNodeAddress = IOHCx13b10190 + (InnerHandle->InstanceId * 0x14);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, VdmNodeAddress),
      VdmNode.Value
      );
    InnerHandle = GnbGetNextHandle(InnerHandle);
  }

  SubsystemIdSettingPhx(SilContext, GnbHandle);
  /*
   * Initialize ARI
   */
  if (true == CfgPcieAriSupport) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, IOHCx13b10118),
      (uint32_t) ~(IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK),
      1 << IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET
      );
  }
  return;
}

static
void
MpioCheckPortPresence (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  uint8_t               *WrapperNumbers;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Wrapper = PcieConfigGetParentWrapper(Engine);
  WrapperNumbers = (uint8_t *) Buffer;
  if (Wrapper->WrapId < 2) {
    WrapperNumbers[Wrapper->WrapId] = Wrapper->WrapId;
  }
}

static
SIL_STATUS
MpioCheckWrapperPresence (
  SIL_CONTEXT           *SilContext,
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  uint8_t               *WrapperNumbers;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  WrapperNumbers = (uint8_t *) Buffer;
  if (Wrapper->WrapId < 2) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
      "Setting WrapId %d to 0x%x\n",
      Wrapper->WrapId,
      WrapperNumbers[Wrapper->WrapId]
      );
    Wrapper->WrapId = WrapperNumbers[Wrapper->WrapId];
  }
  return SilPass;
}

/**-------------------------------------------------------------------
 * MpioCfgHideUnusedWrappers
 *
 * @brief Identify unused wrappers and mark them as unused
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                Pointer silicon complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgHideUnusedWrappers (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  uint8_t           WrapperNumbers[2];
  NBIO_IP2IP_API    *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  WrapperNumbers[0] = 0xFF;
  WrapperNumbers[1] = 0xFF;
  NbioIp2Ip->PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioCheckPortPresence,
    WrapperNumbers,
    Pcie
    );

  NbioIp2Ip->PcieConfigRunProcForAllWrappers(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    MpioCheckWrapperPresence,
    WrapperNumbers,
    Pcie
    );
}

static
void
UpdateInputList (
  GNB_HANDLE  *GnbHandle,
  uint8_t     *InputList
  )
{
  uint8_t   RemapIndex;
  uint32_t  Value;

  for (RemapIndex = 0; RemapIndex < NumberOfBridges; RemapIndex++) {
    Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      IOHCx13b100b8 + (RemapIndex * 4)
      );
    InputList[RemapIndex] = (uint8_t)Value;
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "input_list[%d] %x \n", RemapIndex, InputList[RemapIndex]);
  }
}

static
void
IoapicRoutingProcess (
  SIL_CONTEXT  *SilContext,
  GNB_HANDLE   *GnbHandle
  )
{
  uint8_t                   DeviceIndex;
  uint8_t                   DetectIndex;
  uint8_t                   ProgramIndex;
  MPIOCLASS_PHX_INPUT_BLK   *SilDataPhx;

  uint8_t  PhysicalBridgeIdAfterRemap;
  uint8_t  Temp;
  uint8_t  LogicalBridgeId;
  SIL_RESERVED_STRUCT_0014 MappingResult[NumberOfBridges] = {0};
  uint8_t  RemapNxCounter = 0;
  uint8_t  BrIdCounter = 0;
  uint8_t  TmpBufPtr[NumberOfBridges] = {0};

  IOAPIC_BR_INTERRUPT  Map[NumberOfBridges] = {
    {DEVFUNC(1, 1), 0, 0},
    {DEVFUNC(1, 2), 1, 1},
    {DEVFUNC(1, 3), 2, 2},
    {DEVFUNC(1, 4), 3, 3},
    {DEVFUNC(1, 5), 4, 4},
    {DEVFUNC(1, 6), 5, 5},
    {DEVFUNC(2, 1), 6, 0x10005},
    {DEVFUNC(2, 2), 7, 0x10025},
    {DEVFUNC(2, 3), 8, 0x10025},
    {DEVFUNC(2, 4), 9, 0x10024},
    {DEVFUNC(2, 5), 10, 0x10023},
    {DEVFUNC(2, 6), 11, 0x10022},
    {DEVFUNC(3, 1), 12, 0x20021},
    {DEVFUNC(3, 2), 13, 0x20020},
    {DEVFUNC(3, 3), 14, 0x20010},
    {DEVFUNC(3, 4), 15, 0x20011},
    {DEVFUNC(4, 1), 16, 0x30022},
    {DEVFUNC(8, 1), 17, 0x40023},
    {DEVFUNC(8, 2), 18, 0x40024},
    {DEVFUNC(8, 3), 19, 0x40025}
  };

  uint8_t  PhysicalBridgeIdRemapList[NumberOfBridges] = {
    0, 1, 2, 3, 4, 5,           // Core0
    11, 12, 13, 14, 15, 16,     // Core1
    6, 7, 8, 9,                 // Core2
    10,                         // Core3
    17, 18, 19
  };                // nbif0/1/2

  uint8_t InputList[NumberOfBridges] = {0x9, 0xA, 0xB, 0xC, 0xD, 0xE,
                                        0x11, 0x12, 0x13, 0x14, 0x15,
                                        0x16, 0x19, 0x1A, 0x1B, 0x1C,
                                        0x21, 0x41, 0x42, 0x43};

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  UpdateInputList(GnbHandle, InputList);
  memcpy((void *)TmpBufPtr, SilDataPhx->BridgeIdRemappingTable, NumberOfBridges);

  for (DeviceIndex = 0; DeviceIndex < NumberOfBridges; DeviceIndex++) {
    PhysicalBridgeIdAfterRemap = PhysicalBridgeIdRemapList[DeviceIndex];

    /*
     * Read the remap offset to get the real input
     */
    Temp = InputList[PhysicalBridgeIdAfterRemap];
    LogicalBridgeId = InputList[DeviceIndex];

    /*
     * Build logic data
     * Compare map and input list to know which SMN Nx need to program
     */
    for (DetectIndex = 0; DetectIndex < NumberOfBridges; DetectIndex++) {
      if (Temp == Map[DetectIndex].DevMap) {
        MappingResult[RemapNxCounter].field1 = Map[DetectIndex].LogicalBrId;
        RemapNxCounter++;
        /*
         * DetectIndex found the final logic Nx registers
         */
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "DetectIndex:%d  Map[DetectIndex].LogicalBridgeId %d \n",
          DetectIndex,
          Map[DetectIndex].LogicalBrId
          );
      }

      if (LogicalBridgeId == Map[DetectIndex].DevMap) {
        MappingResult[BrIdCounter].field0 = Map[DetectIndex].LogicalBrId;
        BrIdCounter++;
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "DetectIndex:%d MappingResult[DetectIndex].LogicalBridgeId: %x \n",
          DetectIndex,
          MappingResult[DetectIndex].field0
          );
      }
    }
  }

  /*
   * Program interrupt routing
   */
  for (ProgramIndex = 0; ProgramIndex < NumberOfBridges; ProgramIndex++) {
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      IOAPICx14300040 + (MappingResult[ProgramIndex].field1 * 4),
      Map[MappingResult[ProgramIndex].field0].InterruptRountingData
      );
    TmpBufPtr[MappingResult[ProgramIndex].field0] = MappingResult[ProgramIndex].field1;
  }

  memcpy(SilDataPhx->BridgeIdRemappingTable, TmpBufPtr, NumberOfBridges);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**-------------------------------------------------------------------
 *
 * MpioCfgAfterDxioInitPhx
 *
 * @brief Configuration Timepoint before DXIO firmware initialization starts
 *
 * @details tbd
 *
 * @param  SilContext             A context structure through which host firmware defined data
 *                                can be passed to openSIL. The host firmware is responsible
 *                                for initializing the SIL_CONTEXT structure.
 * @param  PCIe_PLATFORM_CONFIG   Pointer silicon complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterDxioInitPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  GNB_HANDLE                         *GnbHandle;
  MPIO_S0I3_GPIO_RST_EP              *GpioReset;
  uint32_t                           MpioArg[6];
  MPIOCLASS_COMMON_INPUT_BLK         *SilDataCommon;
  MPIOCLASS_PHX_INPUT_BLK            *SilDataPhx;
  NBIO_IP2IP_API                     *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_PHX_INSTANCE
    );
  assert(SilDataPhx != NULL);

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigDebugDump(Pcie);
  MpioCfgHideUnusedWrappers(SilContext, Pcie);
  NbioIp2Ip->PcieConfigRunProcForAllWrappers(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    PcieCommonCoreConfigurationPhx,
    NULL,
    Pcie
    );
  NbioIp2Ip->PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieCommonEngineConfigurationPhx,
    NULL,
    Pcie
    );

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);
  InitBusRangesAndAriPhx(SilContext, GnbHandle, Pcie, SilDataCommon->CfgPcieAriSupport);

  /*
   * Update IOAPIC routing
   */
  IoapicRoutingProcess(SilContext, GnbHandle);

  GpioReset = NULL;
  if (SilDataPhx->CfgDxioPCIeRSTGenericReset) {
    if (SilDataPhx->CfgDxioPCIeGPIOResetEP1 != 0) {
      GpioReset = (MPIO_S0I3_GPIO_RST_EP *) ((uintptr_t) SilDataPhx->CfgDxioPCIeGPIOResetEP1);
      if (GpioReset != NULL) {
        MpioArg[0] = 1;
        MpioArg[1] = GpioReset->AssertionSeq;
        MpioArg[2] = GpioReset->Address;
        MpioArg[3] = GpioReset->DeassertionValue;
        MpioArg[4] = GpioReset->AssertionValue;
        MpioArg[5] = 0x0;
        if (GpioReset->AssertionSeq == 2) {
          MpioArg[5] = GpioReset->MillisecodsDelay;
        }
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
          MpioArg[0],
          MpioArg[1],
          MpioArg[2],
          MpioArg[3],
          MpioArg[4],
          MpioArg[5]
          );
        MpioServiceRequestCommon(SilContext, GnbHandle->Address, BIOS_MPIO_MSG_PCIE_GPIO_RESET, MpioArg, 0);
      }
    }
    if (SilDataPhx->CfgDxioPCIeGPIOResetEP2 != 0) {
      GpioReset = (MPIO_S0I3_GPIO_RST_EP *) ((uintptr_t) SilDataPhx->CfgDxioPCIeGPIOResetEP2);
      if (GpioReset != NULL) {
        MpioArg[0] = 2;
        MpioArg[1] = GpioReset->AssertionSeq;
        MpioArg[2] = GpioReset->Address;
        MpioArg[3] = GpioReset->DeassertionValue;
        MpioArg[4] = GpioReset->AssertionValue;
        MpioArg[5] = 0x0;
        if (GpioReset->AssertionSeq == 2) {
          MpioArg[5] = GpioReset->MillisecodsDelay;
        }
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
          MpioArg[0],
          MpioArg[1],
          MpioArg[2],
          MpioArg[3],
          MpioArg[4],
          MpioArg[5]
          );
        MpioServiceRequestCommon(SilContext, GnbHandle->Address, BIOS_MPIO_MSG_PCIE_GPIO_RESET, MpioArg, 0);
      }
    }
    if (SilDataPhx->CfgDxioPCIeGPIOResetEP3 != 0) {
      GpioReset = (MPIO_S0I3_GPIO_RST_EP *) ((uintptr_t) SilDataPhx->CfgDxioPCIeGPIOResetEP3);
      if (GpioReset != NULL) {
        MpioArg[0] = 3;
        MpioArg[1] = GpioReset->AssertionSeq;
        MpioArg[2] = GpioReset->Address;
        MpioArg[3] = GpioReset->DeassertionValue;
        MpioArg[4] = GpioReset->AssertionValue;
        MpioArg[5] = 0x0;
        if (GpioReset->AssertionSeq == 2) {
          MpioArg[5] = GpioReset->MillisecodsDelay;
        }
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
          MpioArg[0],
          MpioArg[1],
          MpioArg[2],
          MpioArg[3],
          MpioArg[4],
          MpioArg[5]
          );
        MpioServiceRequestCommon(SilContext, GnbHandle->Address, BIOS_MPIO_MSG_PCIE_GPIO_RESET, MpioArg, 0);
      }
    }
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 *
 * MpioConfigurePsppPhx
 *
 * @brief Configuration of PSPP parameters in MPIO ancillary data
 *
 * @param   SilContext           A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param PCIe_PLATFORM_CONFIG   Pointer silicon complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void MpioConfigurePsppPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  MPIOCLASS_PHX_INPUT_BLK       *SilDataPhx;
  MPIO_ANCILLARYPORTDATA_PSPP   PsppParams;
  MPIO_ANCILLARYPORTDATA_PSPP_2 PsppParams2;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  Pcie->PsppPolicy = SilDataPhx->PsppPolicy;
  PsppParams.Value = 0;
  PsppParams.Field.lowBWThreshold = PsppThreshold100Percent;
  PsppParams.Field.HighBWThreshold = PsppThreshold95Percent;
  PsppParams.Field.lowBWThresholdG2 = PsppThreshold20Percent;
  PsppParams.Field.HighBWThresholdG2 = PsppThreshold95Percent;
  PsppParams.Field.lowBWThresholdG3 = PsppThreshold20Percent;
  PsppParams.Field.HighBWThresholdG3 = PsppThreshold95Percent;
  PsppParams.Field.BWHintCount = 3;
  Pcie->PsppTuningParams = PsppParams.Value;

  PsppParams2.Value = 0;
  PsppParams2.Field.lowBWThresholdG4 = PsppThreshold50Percent;
  PsppParams2.Field.HighBWThresholdG4 = PsppThreshold0Percent;
  Pcie->PsppTuningParams2 = PsppParams2.Value;

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 *
 * Usb4LaneNumberCfg
 *
 * @brief Configuration Usb4 Lane number before DXIO firmware initialization
 *
 * @details tbd
 *
 * @param[in]  Counter                 Counter of Controller number
 * @param[in]  AskEntry                Pointer to ASK entry
 * @param[in]  TopologyEntry           Pointer to engine topology
 *
 * @returns SIL_STATUS
 * @retval SilUnsupported should always be returned.
 **/
SIL_STATUS
Usb4LaneNumberCfgPhx (
  uint8_t                      Counter,
  FW_ASK_STRUCT                *AskEntry,
  MPIO_PORT_DESCRIPTOR         *TopologyEntry
  )
{
  uint8_t                      PortStartLane;
  uint8_t                      PortEndLane;

  if (Counter == 0) {
    PortStartLane = 30;
    PortEndLane = 30;
    AskEntry->desc.startLaneIndex = PortStartLane;
    AskEntry->desc.numberOfLanes = 1;
    TopologyEntry->EngineData.MpioStartLane = PortStartLane;
    TopologyEntry->EngineData.MpioEndLane = PortEndLane;
  } else {
    PortStartLane = 34;
    PortEndLane = 34;
    AskEntry->desc.startLaneIndex = PortStartLane;
    AskEntry->desc.numberOfLanes = 1;
    TopologyEntry->EngineData.MpioStartLane = PortStartLane;
    TopologyEntry->EngineData.MpioEndLane = PortEndLane;
  }

  /*
   * Limited to gen1 for usb4
   */
  AskEntry->desc.link_attributes.maxLinkSpeedCap = PcieGen1;
  TopologyEntry->Port.LinkSpeedCapability = PcieGen1;

  return SilPass;
}

/**-------------------------------------------------------------------
 *
 * MpioIsEverTriedTrainingPhx
 *
 * @brief  Check each Engine that ever tried training
 *
 * @details tbd
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Engine configuration info
 *
 * @returns boolean
 * @retval true  - This engine is ever tried training
 *         false - This engine is not tried training
 **/
bool
MpioIsEverTriedTrainingPhx (
  SIL_CONTEXT               *SilContext,
  PCIe_ENGINE_CONFIG        *Engine
  )
{
  GNB_HANDLE                    *GnbHandle;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      Value32;
  uint32_t                      SmnAddress;
  uint32_t                      i, j;
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  Value32 = 0;
  SmnAddress = NBIOBASEREGx11140294;
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  /*
   * Check LC_STATE0 ~ LC_STATE5
   */
  for (i = 0; i < 6; i++) {
    Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SmnAddress + i * 4)
      );
    /*
     * Check LC_CURRENT_STATE and all LC_PREV_STATE
     */
    for (j = 0; j < 4; j++) {
      if (((Value32 >> (j * 8)) & 0xFF) > 0x4) {
        return true;
      }
    }
  }

  return false;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgAddUsb4CtrllerStrapList
 *
 * @brief  Add USB4 Controller straps to the strap list
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] StrapList    Pointer to a pointer to the end of the strap list
 *
 * @returns Total strap size
 * @retval uint32_t Total strap size after adding USB4 Controller straps to the strap list
 **/
static
uint32_t
MpioCfgAddUsb4CtrllerStrapList (
  uint8_t                    Usb4Counter,
  void                       **StrapList
  )
{
  SIL_RESERVED_STRUCT_0025  *StrapEntry;
  uint32_t            totalStrapSize = 0;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  StrapEntry = (SIL_RESERVED_STRUCT_0025 *) *StrapList;
  memcpy((void *) StrapEntry, (void *) &USB4CtrllerStraps, sizeof (SIL_RESERVED_STRUCT_0025));

  StrapEntry->field1.data = (Usb4Counter == 0) ? 0x22 : 0x26;

  if (ISSOCPHX2) {
    StrapEntry->field26.data = 1;
  }

  totalStrapSize += sizeof (SIL_RESERVED_STRUCT_0025);
  *StrapList = (void *) (StrapEntry + 1);
  StrapEntry->field0.length = (uint8_t) (totalStrapSize >> 2);

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapList = 0x%x\n", *StrapList);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return totalStrapSize;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgAddCtrllerStrapList
 *
 * @brief  Add Controller straps to the strap list
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   StrapList           Pointer to a pointer to the end of the strap list
 *
 * @returns Total strap size
 * @retval uint32_t Total strap size after adding Controller straps to the strap list
 **/
static
uint32_t
MpioCfgAddCtrllerStrapList (
  SIL_CONTEXT                *SilContext,
  PCIe_WRAPPER_CONFIG        *Wrapper,
  void                       **StrapList
  )
{
  SIL_RESERVED_STRUCT_0007              *StrapEntry;
  uint32_t                    SubsystemDeviceId;
  uint32_t                    SubsystemVendorId;
  uint32_t                    totalStrapSize = 0;
  uint32_t                    tmpStrapSize = 0;
  MPIOCLASS_COMMON_INPUT_BLK  *SilDataCommon;
  MPIOCLASS_PHX_INPUT_BLK     *SilDataPhx;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_PHX_INSTANCE
    );
  assert(SilDataPhx != NULL);

  StrapEntry = (SIL_RESERVED_STRUCT_0007 *) *StrapList;
  memcpy((void *) StrapEntry, (void *) &CtrllerStraps, sizeof (SIL_RESERVED_STRUCT_0007));

  /*
   * Update Controller strap data++
   * SDP Unit ID
   */
  StrapEntry->field6.data = (Wrapper->WrapId == 0) ? 0x10 : 0x19;

  /*
   * ACS Enablement
   */
  if (SilDataPhx->CfgACSEnable) {
    StrapEntry->field9.data = 1;
    StrapEntry->field10.data = 1;
    StrapEntry->field11.data = 1;
    StrapEntry->field12.data = 1;
    StrapEntry->field13.data = 1;
    StrapEntry->field14.data = 1;
    StrapEntry->field15.data = 1;
  }

  if (SilDataPhx->CfgPCIeLTREnable) {
    StrapEntry->field16.data = 1;
  }

  if (SilDataPhx->PcieOBFF == true) {
    StrapEntry->field17.data = 0x2;
  }

  /*
   * Data Link Feature Extended Capability
   */
  if (SilDataPhx->AmdDlfCapEnV2 == 0xF) {
    StrapEntry->field18.data = 0xF;
  }

  /*
   * Atomic
   */
  if ((SilDataCommon->GppAtomicOps == 1) || (SilDataCommon->GfxAtomicOps == 1)) {
    StrapEntry->field21.data = 1;
    StrapEntry->field22.data = 1;
  }

  if (SilDataPhx->PcieCoreMarginIgnoreCSkip == 0) {
    StrapEntry->field23.data = 0;
  }

  /*
   * AER
   */
  if (SilDataCommon->CfgAEREnable) {
    StrapEntry->field24.data = 1;
    StrapEntry->field7.data = 1;
  }

  /*
   * ECRC
   */
  if (SilDataPhx->PcieEcrcEnablement || SilDataCommon->CfgAEREnable) {
    StrapEntry->field25.data = 1;
    StrapEntry->field26.data = 1;
  }

  /*
   * PCIE Subsystem
   */
  SubsystemDeviceId = (uint32_t) SilDataCommon->AmdPcieSubsystemDeviceID;
  if (SubsystemDeviceId != 0) {
    StrapEntry->field27.data = SubsystemDeviceId;
  }
  SubsystemVendorId = (uint32_t) SilDataCommon->AmdPcieSubsystemVendorID;
  if (SubsystemVendorId != 0) {
    StrapEntry->field28.data = SubsystemVendorId;
  }

  /*
   * Update Controller strap data
   */
  totalStrapSize += sizeof (SIL_RESERVED_STRUCT_0007);
  *StrapList = (void *) (StrapEntry + 1);

  totalStrapSize += tmpStrapSize;
  StrapEntry->field0.length = (uint8_t) (totalStrapSize >> 2);

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapList = 0x%x\n", *StrapList);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return totalStrapSize;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgAddAllPortStrapList
 *
 * @brief  Add All-port straps to the strap list
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   StrapList           Pointer to a pointer to the end of the strap list
 *
 * @returns Total strap size
 * @retval uint32_t Total strap size after adding All-port straps to the strap list
 **/
static
uint32_t
MpioCfgAddAllPortStrapList (
  SIL_CONTEXT                *SilContext,
  PCIe_WRAPPER_CONFIG        *Wrapper,
  void                       **StrapList
  )
{
  SIL_RESERVED_STRUCT_0001               *StrapEntry;
  uint32_t                     totalStrapSize = 0;
  uint32_t                     tmpStrapSize = 0;
  MPIOCLASS_COMMON_INPUT_BLK   *SilDataCommon;
  MPIOCLASS_PHX_INPUT_BLK      *SilDataPhx;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_PHX_INSTANCE
    );
  assert(SilDataPhx != NULL);

  StrapEntry = (SIL_RESERVED_STRUCT_0001 *) *StrapList;
  memcpy((void *) StrapEntry, (void *) &AllPortStraps, sizeof (SIL_RESERVED_STRUCT_0001));

  /*
   * Update Allport strap data
   * Data Link Feature Extended Capability V2
   */
  if (SilDataPhx->AmdDlfCapEnV2 == 0xF) {
    StrapEntry->field3.data = 0xF;
  }

  /*
   * Data Link Feature Exchange Enable V2
   */
  if (SilDataPhx->AmdDlfExEnV2 == 0xF) {
    StrapEntry->field4.data = 0xF;
  }

  /*
   * 10-Bit TAG
   */
  if (SilDataPhx->CfgPcieTbtSupport) {
    StrapEntry->field5.data = 1;
    StrapEntry->field6.data = 1;
    if (false == SilDataPhx->CfgTbtCompleterEn) {
      StrapEntry->field5.data = 0;
    }
    if (false == SilDataPhx->CfgTbtRequesterEn) {
      StrapEntry->field6.data = 0;
    }
  }

  /*
   * Update Allport strap data
   */
  totalStrapSize += sizeof (SIL_RESERVED_STRUCT_0001);
  *StrapList = (void *) (StrapEntry + 1);

  totalStrapSize += tmpStrapSize;
  StrapEntry->header.length = (uint8_t) (totalStrapSize >> 2);

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapList = 0x%x\n", *StrapList);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return totalStrapSize;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgAddPortStrapList
 *
 * @brief  Add per-port straps to the strap list
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   PortPointer         Pointer to topology entry
 * @param   EngineId            Index into EngineConfig
 * @param   StrapList           Pointer to a pointer to the end of the strap list
 *
 * @returns Total strap size
 * @retval uint32_t Total strap size after adding per-port straps to the strap list
 **/
static
uint32_t
MpioCfgAddPortStrapList (
  SIL_CONTEXT                *SilContext,
  PCIe_WRAPPER_CONFIG        *Wrapper,
  MPIO_PORT_DESCRIPTOR       *PortPointer,
  uint32_t                   EngineId,
  void                       **StrapList
  )
{
  SIL_RESERVED_STRUCT_0021            *StrapEntry;
  uint32_t                  totalStrapSize = 0;
  uint32_t                  tmpStrapSize = 0;
  uint32_t                  Index;
  uint32_t                  Value8 = 0;
  MPIOCLASS_PHX_INPUT_BLK   *SilDataPhx;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);

  StrapEntry = (SIL_RESERVED_STRUCT_0021 *) *StrapList;
  memcpy((void *) StrapEntry, (void *) &PerPortStraps, sizeof (SIL_RESERVED_STRUCT_0021));

  Index = 0;
  StrapEntry->field1[Index].strapIdx = MPIOSTRAPx10A;
  StrapEntry->field1[Index].data = PortPointer->Port.LinkAspm & 0x2;
  Index++;

  if ((PortPointer->Port.LinkAspmL1_1 == 1) || (PortPointer->Port.LinkAspmL1_2 == 1)) {
    if (PortPointer->Port.LinkAspmL1_2 == 1) {
      StrapEntry->field1[Index].strapIdx = MPIOSTRAPx15A;
      StrapEntry->field1[Index].data = 1;
      Index++;
      StrapEntry->field1[Index].strapIdx = MPIOSTRAPx158;
      StrapEntry->field1[Index].data = 1;
      Index++;
    }
    if (PortPointer->Port.LinkAspmL1_1 == 1) {
      StrapEntry->field1[Index].strapIdx = MPIOSTRAPx15B;
      StrapEntry->field1[Index].data = 1;
      Index++;
      StrapEntry->field1[Index].strapIdx = MPIOSTRAPx159;
      StrapEntry->field1[Index].data = 1;
      Index++;
    }

    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx15C;
    StrapEntry->field1[Index].data = 1;
    Index++;

    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx15E;
    StrapEntry->field1[Index].data = 0xA;
    Index++;

    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx15F;
    StrapEntry->field1[Index].data = 1;
    Index++;

    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx161;
    StrapEntry->field1[Index].data = 0xF;
    Value8 = SilDataPhx->CfgPCIeTPowerOnValue;
    if (Value8 != 0) {
      StrapEntry->field1[Index].data = Value8;
    }
    Index++;
  }
  if (PortPointer->Port.PresetMaskCntl.PresetMask8Gt != 0) {
    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx138;
    StrapEntry->field1[Index].data = PortPointer->Port.PresetMaskCntl.PresetMask8Gt;
    Index++;
  }
  if (PortPointer->Port.PresetMaskCntl.PresetMask16Gt != 0) {
    StrapEntry->field1[Index].strapIdx = MPIOSTRAPx139;
    StrapEntry->field1[Index].data = PortPointer->Port.PresetMaskCntl.PresetMask16Gt;
    Index++;
  }

  totalStrapSize = sizeof (MPIO_STRAPENTRY_SHORT_PERPORT) * Index + sizeof (GENERIC_ANC_HEADER);
  *StrapList = (void *) ((uint8_t *)StrapEntry + totalStrapSize);

  totalStrapSize += tmpStrapSize;
  StrapEntry->field0.length = (uint8_t) (totalStrapSize >> 2);

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapList = 0x%x\n", *StrapList);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return totalStrapSize;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgStrapListWrapperCallback
 *
 * @brief  Per-Wrapper Callback for wrapper strap settings
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   Buffer              Buffer pointer
 * @param   GnbHandle           GnbHandle Pointer
 *
 * @returns Nothing
 * @retval  Nothing
 **/
static
void
MpioCfgStrapListWrapperCallback (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  SIL_RESERVED_STRUCT_0018  *StrapData;
  uint8_t                   *StrapEntry;
  uint32_t                  EngineId;
  MPIO_PORT_DESCRIPTOR      *PortPointer;
  SIL_RESERVED_STRUCT_0002             *ancListData;
  uint16_t                  ancListIdx;
  uint32_t                  tmpStrapSize = 0;
  uint8_t                   Usb4Counter = 0;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Buffer = 0x%x\n", Buffer);

  StrapData = (SIL_RESERVED_STRUCT_0018 *) Buffer;
  StrapEntry = StrapData->StrapList;
  ancListData = (SIL_RESERVED_STRUCT_0002 *)StrapData->StrapList;

  /*
   * Adding ANC_LIST
   */
  ancListIdx = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;// bcz of Ancillary data header
  memcpy((void *) StrapEntry, (void *) &AncListData, sizeof (AncListData));
  StrapData->field0 += sizeof (AncListData);
  StrapEntry += sizeof (AncListData);

  /*
   * Adding Allport Straps
   */
  tmpStrapSize = MpioCfgAddAllPortStrapList(SilContext, Wrapper, (void **) &StrapEntry);
  if (tmpStrapSize > 0) {
    ancListData->field2 = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;
    StrapData->field0 += tmpStrapSize;
  }

  /*
   * Adding Controller straps
   */
  tmpStrapSize = MpioCfgAddCtrllerStrapList(SilContext, Wrapper, (void **) &StrapEntry);
  if (tmpStrapSize > 0) {
    ancListData->field1 = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;
    StrapData->field0 += tmpStrapSize;
  }

  /*
   * Adding PerPort Straps, EngineId logic need to align with Cbs PerPort strap parsing
   */
  EngineId = 0;
  PortPointer = StrapData->PortList;
  while (PortPointer != NULL) {
    switch (PortPointer->EngineData.EngineType) {
    case MpioPcieEngine:
      if ((PortPointer->EngineData.StartLane >= Wrapper->StartPhyLane) &&
        (PortPointer->EngineData.StartLane <= Wrapper->EndPhyLane)) {
        tmpStrapSize = MpioCfgAddPortStrapList(SilContext, Wrapper, PortPointer, EngineId, (void *) &StrapEntry);
        if (tmpStrapSize > 0) {
          if (ancListData->field3 == 0) {
            /*
             * ANC_LIST points to the 1st engine in a wrapper
             */
            ancListData->field3 = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;
            PortPointer->Port.MpioAncDataIdx = ancListIdx;
          } else {
            PortPointer->Port.MpioAncDataIdx = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;
          }
          StrapData->field0 += tmpStrapSize;
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "StartLane=0x%x-0x%x, MpioAncDataIdx=0x%x\n",
            PortPointer->EngineData.StartLane,
            PortPointer->Port.MpioAncDataIdx
            );
        }
      }
      EngineId++;
      break;
    case MpioUSB_OVER_PCIE:
      if (AddUsb4CtrllerStrap && (Wrapper->WrapId == 0)) {
        tmpStrapSize = MpioCfgAddUsb4CtrllerStrapList(Usb4Counter, (void **) &StrapEntry);
        Usb4Counter++;
        if (tmpStrapSize > 0) {
          PortPointer->Port.MpioAncDataIdx = (uint16_t) (StrapData->field0 >> 2) + ANC_DATA_HEADER_SIZE;
          StrapData->field0 += tmpStrapSize;
        }
      }
      break;
    default:
      break;
    }
    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  StrapData->StrapList = StrapEntry;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapData->field0 0x%x\n", StrapData->field0);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 * MpioCfgAddStrapList
 *
 * @brief Interface to configure MPIO/PCIe ports
 *
 * @details This function is called once for each socket and is used
 * after ports are mapped and before reconfig.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to Silicon Descriptor for this node
 * @param   AncillaryData       Pointer to next available entry in Ancillary Data for this node
 * @param   PortList            Pointer to topology structures for this node
 *
 * @returns The size of the strap list after adding ancillary data.
 * @retval uint32_t Numeric strap list size returned in bytes? (not sure)
 **/
uint32_t
MpioCfgAddStrapListPhx (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  void                   *AncillaryData,
  MPIO_PORT_DESCRIPTOR   *PortList
  )
{
  SIL_RESERVED_STRUCT_0018      StrapData;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "Enter with Ancillary Data Pointer 0x%x, StrapData = 0x%x\n",
    AncillaryData,
    &StrapData
    );

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  StrapData.PortList = PortList;
  StrapData.StrapList = AncillaryData;
  StrapData.field0 = 0;

  NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    MpioCfgStrapListWrapperCallback,
    (void *)&StrapData,
    GnbHandle
    );

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapData.field0 0x%x\n", StrapData.field0);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return StrapData.field0;
}

/**-------------------------------------------------------------------
 *
 * MpioCfgStrapSizeWrapperCallback
 *
 * @brief  Per-Wrapper Callback to count straps to be set
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Wrapper             Wrapper configuration info
 * @param   Buffer              Buffer pointer
 * @param   GnbHandle           GnbHandle Pointer
 *
 * @returns Nothing
 * @retval  Nothing
 **/
static
void
MpioCfgStrapSizeWrapperCallback (
  SIL_CONTEXT                       *SilContext,
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  SIL_RESERVED_STRUCT_0020  *StrapData;
  MPIO_PORT_DESCRIPTOR          *PortPointer;

  StrapData = (SIL_RESERVED_STRUCT_0020 *) Buffer;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapData.field0 0x%x\n", StrapData->field0);

  StrapData->field0 += sizeof (CtrllerStraps);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapData.field0 0x%x\n", StrapData->field0);

  StrapData->field0 += sizeof (AllPortStraps);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapData.field0 0x%x\n", StrapData->field0);

  PortPointer = StrapData->PortList;
  while (PortPointer != NULL) {
    switch (PortPointer->EngineData.EngineType) {
    case MpioPcieEngine:
      if ((PortPointer->EngineData.StartLane >= Wrapper->StartPhyLane) &&
        (PortPointer->EngineData.StartLane <= Wrapper->EndPhyLane)) {
        StrapData->field0 += sizeof (PerPortStraps);
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioPcieEngine StrapData->field0=0x%x\n", StrapData->field0);
      }
      break;
    case MpioUSB_OVER_PCIE:
      if (AddUsb4CtrllerStrap && (Wrapper->WrapId == 0)) {
        StrapData->field0 += sizeof (USB4CtrllerStraps);
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioUSB_OVER_PCIE StrapData->field0=0x%x\n", StrapData->field0);
      }
      break;
    default:
      break;
    }
    PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
  }

  /*
   * Add size of strap subheap here only if there's any strap setting
   */
  if (StrapData->field0 != 0) {
    StrapData->field0 += sizeof (AncListData);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Size of strap subheap StrapData->field0=0x%x\n", StrapData->field0);
  }

  return;
}

/**--------------------------------------------------------------------
 * MpioCfgGetStrapListSize
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket and is used
 * to configure ports after ports are mapped and before reconfig.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to Silicon Descriptor for this node
 * @param   PortPointer         Pointer to first topology entry
 *
 * @returns Size of ancillary data required to store the strap subheap
 * @retval uint32_t Numeric heap size returned in bytes? (not sure)
 **/
uint32_t
MpioCfgGetStrapListSizePhx (
  SIL_CONTEXT           *SilContext,
  GNB_HANDLE            *GnbHandle,
  MPIO_PORT_DESCRIPTOR  *PortPointer
  )
{
  SIL_RESERVED_STRUCT_0020  StrapSizeData;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  StrapSizeData.field0 = 0;
  StrapSizeData.PortList = PortPointer;
  NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    MpioCfgStrapSizeWrapperCallback,
    (void *)&StrapSizeData,
    GnbHandle
    );

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "StrapSizeData.field0 0x%x\n", StrapSizeData.field0);
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return StrapSizeData.field0;
}
