/**
 *  @file MpioCfgPoints.c
 *  @brief Callouts from MPIO initialization for Genoa
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <SilCommon.h>
#include <Mpio/Common/Deli.h>
#include <Mpio/Common/MpioLib.h>
#include <NBIO/IOD/include/GlobalRegB0.h>
#include <NBIO/IOD/include/IohcReg.h>
#include <NBIO/IOD/include/NbioBaseReg.h>
#include <NBIO/IOD/include/PcierccfgReg.h>
#include <NBIO/IOD/include/Iommul2Reg.h>
#include <NBIO/IOD/include/Nbifepf0cfgReg.h>
#include <NBIO/IOD/GnbRegisters.h>
#include <NBIO/NbioIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include "MpioPcieStraps.h"
#include "MpioCmn2.h"
#include "Mpio.h"

///  Separate Reference Clock with Independent Spread Type Enumeration
typedef enum  {
  SRIS_CMN        = BIT_8(0),    ///< Sris common type
  SRIS_DBG        = BIT_8(1),    ///< Sris debug type
  SRIS_DBG_2      = BIT_8(2),    ///< Sris debug level 2
  SRIS_AUTODETECT = BIT_8(3)     ///< Sris autodetect type
} SRIS_CFG_TYPE;

/**--------------------------------------------------------------------
 *
 * MpioCfgGlobalConfig
 *
 * @brief Callback from MpioLib to update GLOBAL CONFIG
 *
 * @details This function provides platform-specific initialization of the MPIO_GLOBAL_CONFIG
 *
 * @param [in] GnbHandle GNB_HANDLE pointer
 * @param [in] ArgList   Pointer to array of arguments  cast as MPIO_GLOBAL_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgGlobalConfig (
  MPIOCLASS_INPUT_BLK           *SilData,
  GNB_HANDLE                    *GnbHandle,
  uint32_t                      *ArgList
  )
{
  MPIO_GLOBAL_CONFIG    *GlobalConfig;

  GlobalConfig = (MPIO_GLOBAL_CONFIG *) ArgList;

  /*
   * Clock Gating
   */
  if (SilData->CfgDxioClockGating) {
      GlobalConfig->PWRMNGMT_PRFRM_CLK_GATING = 1;
  }

  /*
   * Training Timers
   */
  if (SilData->PcieDxioTimingControlEnable) {
    GlobalConfig->LinkReceiverDetectionPolling = SilData->PCIELinkReceiverDetectionPolling;
    GlobalConfig->LinkResetToTrainingTime = SilData->PCIELinkResetToTrainingTime;
    GlobalConfig->LinkL0Polling = SilData->PCIELinkL0Polling;
  }

  /*
   * Exact Match
   */
  if (SilData->PCIeExactMatchEnable) {
    GlobalConfig->matchPortSizeExactly = 1;
  }

  /*
   * Valid PHY firmware
   */
  if (SilData->DxioPhyValid == 0) {
    GlobalConfig->ValidPhyFWFlag = 0;
  }

  /*
   * Use PHY SRAM
   */
  if (SilData->DxioPhyProgramming == 0) {
    GlobalConfig->usePhySram = 0;
  }

  /*
   * Skip Vetting
   */
  if (SilData->CfgSkipPspMessage == 1) {
    GlobalConfig->skipVetting = 1;
  }

  if (SilData->DxioSaveRestoreModes == 1) {
    GlobalConfig->saverestoremode = 1;
  }

  GlobalConfig->disableSbrTrap = (SilData->SbrBrokenLaneAvoidanceSup? 0 : 1);
  GlobalConfig->disableLaneMarginingTrap = (SilData->AutoFullMarginSup? 0 : 1);
}

/**--------------------------------------------------------------------
 *
 * ProgramFapeCoeffList
 *
 * @brief Function to porgram FAPE coeff list
 *
 * @param [in] GnbHandle GNB_HANDLE pointer
 * @param [in] Wrapper   Wrapper pointer
 * @param [in] Engine    Engine onfig info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
ProgramFapeCoeffList (
  GNB_HANDLE           *GnbHandle,
  PCIe_WRAPPER_CONFIG  *Wrapper,
  PCIe_ENGINE_CONFIG   *Engine
)
{
  /*
   * LcFapeReqPostCursor0
   */
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_297),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPostCursor0 <<
          SIL_RESERVED_298)
   );


   /*
    * LcFapeReqPreCursor0
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_303),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPreCursor0 <<
          SIL_RESERVED_304)
   );

    /*
	 * LcFapeReqPostCursor1
	 */
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_299),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPostCursor1 <<
          SIL_RESERVED_300)
   );

   /*
    * LcFapeReqPreCursor1
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_305),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPreCursor1 <<
          SIL_RESERVED_306)
   );

  /*
    * LcFapeReqPostCursor2
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_301),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPostCursor2 <<
          SIL_RESERVED_302)
   );

  /*
    * LcFapeReqPreCursor2
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_492),
        (uint32_t)  ~(SIL_RESERVED_307),
        (Engine->Type.Port.LcFapeSettingsGroup0.LcFapeReqPreCursor2 <<
          SIL_RESERVED_308)
   );

  /*
	 * LcFapeReqPostCursor3
	 */
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_309),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPostCursor3 <<
          SIL_RESERVED_310)
   );

  /*
    * LcFapeReqPreCursor3
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_315),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPreCursor3 <<
          SIL_RESERVED_316)
   );

   /*
    * LcFapeReqPostCursor4
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_311),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPostCursor4 <<
          SIL_RESERVED_312)
   );

   /*
    * LcFapeReqPreCursor4
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_317),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPreCursor4 <<
          SIL_RESERVED_318)
   );

   /*
    * LcFapeReqPostCursor5
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_313),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPostCursor5 <<
          SIL_RESERVED_314)
   );

   /*
    * LcFapeReqPreCursor5
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_493),
        (uint32_t)  ~(SIL_RESERVED_319),
        (Engine->Type.Port.LcFapeSettingsGroup1.LcFapeReqPreCursor5 <<
          SIL_RESERVED_320)
   );

   /*
    * LcFapeReqPostCursor6
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_321),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPostCursor6 <<
          SIL_RESERVED_322)
   );

   /*
    * LcFapeReqPreCursor6
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_327),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPreCursor6 <<
          SIL_RESERVED_328)
   );

   /*
    * LcFapeReqPostCursor7
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_323),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPostCursor7 <<
          SIL_RESERVED_324)
   );

   /*
    * LcFapeReqPreCursor7
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_329),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPreCursor7 <<
          SIL_RESERVED_330)
   );

   /*
    * LcFapeReqPostCursor8
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_325),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPostCursor8 <<
          SIL_RESERVED_326)
   );

   /*
    * LcFapeReqPreCursor8
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_494),
        (uint32_t)  ~(SIL_RESERVED_331),
        (Engine->Type.Port.LcFapeSettingsGroup2.LcFapeReqPreCursor8 <<
          SIL_RESERVED_332)
   );

   /*
    * LcFapeReqPostCursor9
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_337),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPostCursor9 <<
          SIL_RESERVED_338)
   );

   /*
    * LcFapeReqPreCursor9
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_343),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPreCursor9 <<
          SIL_RESERVED_344)
   );

   /*
    * LcFapeReqPostCursor10
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_333),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPostCursor10 <<
          SIL_RESERVED_334)
   );

   /*
    * LcFapeReqPreCursor10
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_339),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPreCursor10 <<
          SIL_RESERVED_340)
   );

   /*
    * LcFapeReqPostCursor11
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_335),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPostCursor11 <<
          SIL_RESERVED_336)
   );

   /*
    * LcFapeReqPreCursor11
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_495),
        (uint32_t)  ~(SIL_RESERVED_341),
        (Engine->Type.Port.LcFapeSettingsGroup3.LcFapeReqPreCursor11 <<
          SIL_RESERVED_342)
   );

   /*
    * LcFapeReqPostCursor12
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_345),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPostCursor12 <<
          SIL_RESERVED_346)
   );

   /*
    * LcFapeReqPreCursor12
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_351),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPreCursor12 <<
          SIL_RESERVED_352)
   );

   /*
    * LcFapeReqPostCursor13
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_347),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPostCursor13 <<
          SIL_RESERVED_348)
   );

   /*
    * LcFapeReqPreCursor13
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_353),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPreCursor13 <<
          SIL_RESERVED_354)
   );

   /*
    * LcFapeReqPostCursor14
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_349),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPostCursor14 <<
          SIL_RESERVED_350)
   );

   /*
    * LcFapeReqPreCursor14
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_496),
        (uint32_t)  ~(SIL_RESERVED_355),
        (Engine->Type.Port.LcFapeSettingsGroup4.LcFapeReqPreCursor14 <<
          SIL_RESERVED_356)
   );

   /*
    * LcFapeReqPostCursor15
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_357),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPostCursor15 <<
          SIL_RESERVED_358)
   );

   /*
    * LcFapeReqPreCursor15
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_363),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPreCursor15 <<
          SIL_RESERVED_364)
   );

   /*
    * LcFapeReqPostCursor16
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_359),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPostCursor16 <<
          SIL_RESERVED_360)
   );

   /*
    * LcFapeReqPreCursor16
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_365),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPreCursor16 <<
          SIL_RESERVED_366)
   );

   /*
    * LcFapeReqPostCursor17
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_361),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPostCursor17 <<
          SIL_RESERVED_362)
   );

   /*
    * LcFapeReqPreCursor17
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_497),
        (uint32_t)  ~(SIL_RESERVED_367),
        (Engine->Type.Port.LcFapeSettingsGroup5.LcFapeReqPreCursor17 <<
          SIL_RESERVED_368)
   );

   /*
    * LcFapeReqPostCursor18
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
        (uint32_t)  ~(SIL_RESERVED_369),
        (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPostCursor18 <<
          SIL_RESERVED_370)
   );

   /*
    * LcFapeReqPreCursor18
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
          (uint32_t)  ~(SIL_RESERVED_375),
          (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPreCursor18 <<
            SIL_RESERVED_376)
   );

   /*
    * LcFapeReqPostCursor19
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
          (uint32_t)  ~(SIL_RESERVED_371),
          (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPostCursor19 <<
            SIL_RESERVED_372)
   );

   /*
    * LcFapeReqPreCursor19
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
          (uint32_t)  ~(SIL_RESERVED_377),
          (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPreCursor19 <<
            SIL_RESERVED_378)
   );

   /*
    * LcFapeReqPostCursor20
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
          (uint32_t)  ~(SIL_RESERVED_373),
          (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPostCursor20 <<
            SIL_RESERVED_374)
   );

   /*
    * LcFapeReqPreCursor20
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_498),
          (uint32_t)  ~(SIL_RESERVED_379),
          (Engine->Type.Port.LcFapeSettingsGroup6.LcFapeReqPreCursor20 <<
            SIL_RESERVED_380)
   );
}

/*
 *=========================================================================================
 * Timepoint after port mapping and before reconfig
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 *
 * MpioCfgBeforeReconfigCallback
 *
 * @brief Per-Engine Callback for port configuration before bifurcation
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgBeforeReconfigCallback (
  PCIe_ENGINE_CONFIG                *Engine,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  GNB_HANDLE            *GnbHandle;
  bool                  *IsCxl;
  uint16_t              Index;
  SIL_STATUS            Status;
  SOC_LOGICAL_ID        LogicalId;
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Wrapper->Header)));

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (PcieLibIsEngineAllocated(Engine)) {

    /*
	 * Enable ten bit tag CAP support
	 */
    if (Engine->Type.Port.PortCapabilities.TenBitTagSupport == 1) {
      Index = SIL_RESERVED_891 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
      WritePcieStrap (
        GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    }
    else if(Engine->Type.Port.PortCapabilities.TenBitTagSupport == 2 ) {
       Index = SIL_RESERVED_892 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
       WritePcieStrap (
        GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    }
    else if(Engine->Type.Port.PortCapabilities.TenBitTagSupport == 3) {
        Index = SIL_RESERVED_891 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
        WritePcieStrap (
        GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );

        Index = SIL_RESERVED_892 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
        WritePcieStrap (
        GnbHandle,
        Index,
        1,
        Wrapper->WrapId
        );
    }

    /*
	 * Lane power state during dynamic link width change
	 */
    if (Engine->Type.Port.PortFeatures.DynLanesPwrState != 0xf) {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
              PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
                SIL_RESERVED_524),
              (uint32_t) ~(SIL_RESERVED_387 |
                SIL_RESERVED_391),
              ((Engine->Type.Port.PortFeatures.DynLanesPwrState & 0x3) <<
                SIL_RESERVED_388) |
                  ((Engine->Type.Port.PortFeatures.TurnOffUnusedLanes & 0x1) <<
                    SIL_RESERVED_392)
      );
    }

  /*
   * Data Link feature (DLF)
   */
  if(Engine->Type.Port.PortCapabilities.DataLinkFeature == 0)
  {
     Index = SIL_RESERVED_867 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
     WritePcieStrap (GnbHandle, SIL_RESERVED_844, 1, Wrapper->WrapId);
     WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);
  }
  else if(Engine->Type.Port.PortCapabilities.DataLinkFeature == 1)
  {
     Index = SIL_RESERVED_867 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
     WritePcieStrap (GnbHandle, SIL_RESERVED_844, 1, Wrapper->WrapId);
     WritePcieStrap (GnbHandle, Index, 0, Wrapper->WrapId);
   }
   else if(Engine->Type.Port.PortCapabilities.DataLinkFeature == 2)
   {
     WritePcieStrap (GnbHandle, SIL_RESERVED_844, 0, Wrapper->WrapId);
   }

  /*
   * Data Link feature Exchange Control(DLF)
   */
  Index = SIL_RESERVED_845 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
  if(Engine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl == 1)
  {
       WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);
  }
  else if(Engine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl == 0)
  {
       WritePcieStrap (GnbHandle, Index, 0, Wrapper->WrapId);
  }

  /*
   * Allow Compliance
   */
  Index = SIL_RESERVED_840 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
  Status = GetCoreLogicalIdOnCurrentCore ((CORE_LOGICAL_ID*)&LogicalId);

  if (Status != SilPass) {
    /*
	 * Set to 1 (disabled) if AX/AB Stepping, otherwise use SilData value
	 */
    WritePcieStrap (GnbHandle, Index,
      ((LogicalId.Revision & (AMD_REV_F19_GENOA_AX)) && (LogicalId.Family & AMD_FAMILY_GENOA)) ?
        1 : SilData->AmdAllowCompliance, Wrapper->WrapId);
  }

  /*
   * Max Payload support
   */
  Index = SIL_RESERVED_871 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
  if(Engine->Type.Port.PortCapabilities.MaxPayloadSupport != 0xf)
  {
       WritePcieStrap (GnbHandle, Index, Engine->Type.Port.PortCapabilities.MaxPayloadSupport, Wrapper->WrapId);
  }

    /*
	 * Program equalization parameters into their corresponding strap. These are written per wrapper
     * but programmed in the per engine call. Different lane values within a wrapper should be
     * avoided to prevent overwriting the desired value for the whole wrapper.
	 */

    /*
	 * Gen3 US/DS Preset
	 */
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_876,
      Engine->Type.Port.LaneEqualizationCntl.DsRxPresetHint,
      Wrapper->WrapId
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_878,
      Engine->Type.Port.LaneEqualizationCntl.UsRxPresetHint,
      Wrapper->WrapId
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_877,
      Engine->Type.Port.LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_879,
      Engine->Type.Port.LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

    /*
	 * Gen4 US/DS Preset
	 */
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_851,
      Engine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_853,
      Engine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

     /*
	  * Gen5 US/DS Preset
	  */
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_852,
      Engine->Type.Port.Gen5LaneEqualizationCntl.DsTxPreset,
      Wrapper->WrapId
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_854,
      Engine->Type.Port.Gen5LaneEqualizationCntl.UsTxPreset,
      Wrapper->WrapId
      );

    /*
	 * Gen5 Precoding Request
	 */
    if (Engine->Type.Port.PortData.LinkSpeedCapability == PcieGen5 ||
        Engine->Type.Port.PortData.LinkSpeedCapability == PcieGenMaxSupported) {
      WritePcieStrap (
          GnbHandle,
          (SIL_RESERVED_860 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
          Engine->Type.Port.Gen5LaneEqualizationCntl.PrecodeRequest,
          Wrapper->WrapId
          );
    }

    /*
	 * Gen5 Advertise EQ To High Rate Support = 0 on P4 and P5
	 */
    if (((GnbHandle->RBIndex & 0x1) == 0) && (Wrapper->WrapId == 2)) {
      WritePcieStrap (
        GnbHandle,
        (SIL_RESERVED_855 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
        0,
        Wrapper->WrapId
        );
    }
    /*
	 * These are all programmed per engine
	 * Gen3 Disable Phase2/3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
      SIL_RESERVED_519),
      (uint32_t) ~(SIL_RESERVED_289),
      Engine->Type.Port.DisGen3EQPhase << SIL_RESERVED_290
      );
    /*
	 * Gen4 Disable Phase2/3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
      SIL_RESERVED_517),
      (uint32_t) ~(SIL_RESERVED_269),
      Engine->Type.Port.DisGen4EQPhase << SIL_RESERVED_270
      );

    /*
	 * Gen5 Disable Phase2/3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
      SIL_RESERVED_518),
      (uint32_t) ~(SIL_RESERVED_279),
      Engine->Type.Port.DisGen5EQPhase << SIL_RESERVED_280
      );

    /*
	 * ASPM
	 */
    Index = SIL_RESERVED_880 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, Engine->Type.Port.PortCapabilities.AspmCapability & 0x2, Wrapper->WrapId);

    /*
	 * Link PM SubState
	 */
    if ((Engine->Type.Port.PortCapabilities.AspmL1_1 == 1) || (Engine->Type.Port.PortCapabilities.AspmL1_2 == 1))  {

      if (Engine->Type.Port.PortCapabilities.AspmL1_2 == 1) {
        Index = SIL_RESERVED_839 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881);
        WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);

        Index = SIL_RESERVED_875 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881);
        WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);
      }

      if (Engine->Type.Port.PortCapabilities.AspmL1_1 == 1)  {
        Index = SIL_RESERVED_839 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881);
        WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);
      }
    }

    /*
	 * DPC capability
	 */
    if (Engine->Type.Port.PortCapabilities.DownstreamPortContainment != 0)
    {
         Index = SIL_RESERVED_846 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881);
         WritePcieStrap (GnbHandle, Index,
          Engine->Type.Port.PortCapabilities.DownstreamPortContainment, Wrapper->WrapId);
    }

    /*
	 * Set SIL_RESERVED_883/SIL_RESERVED_882
	 */
    if ((SilData->AmdHotPlugHandlingMode == 5) &&
        ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerEntSSD) ||
        (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugUBM) ||
        (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugOCP))) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  SFI Straps Engine->Type.Port.PortId = 0x%x\n", Engine->Type.Port.PortId);
          WritePcieStrap (
            GnbHandle,
            PORT_STRAP_INDEX(SIL_RESERVED_883, Engine->Type.Port.PortId),
            1,
            Wrapper->WrapId
        );
          WritePcieStrap (
            GnbHandle,
            PORT_STRAP_INDEX(SIL_RESERVED_882, Engine->Type.Port.PortId),
            1,
            Wrapper->WrapId
        );
          WritePcieStrap (
            GnbHandle,
            PORT_STRAP_INDEX(SIL_RESERVED_874, Engine->Type.Port.PortId),
            1,
            Wrapper->WrapId
        );
    }

    // RTM Margining Support
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_869,
      (Engine->Type.Port.RxMarginPersistence? 0 : 1),
      Wrapper->WrapId
      );

    if (Engine->Type.Port.SrisEnableMode == 1) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, " SRIS Config:\n");
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisCfgType          = 0x%x\n", SilData->SrisCfgType);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisEnableMode       = 0x%x\n", Engine->Type.Port.SrisEnableMode);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisSkipInterval     = 0x%x\n", Engine->Type.Port.SrisSkipInterval);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - LowerSkpOsGenSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsGenSup);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - LowerSkpOsRcvSup     = 0x%x\n", Engine->Type.Port.LowerSkpOsRcvSup);
      WritePcieStrap (
          GnbHandle,
          (SIL_RESERVED_866 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode == 1)? 0: 1),
          Wrapper->WrapId
          );
    }
    if (Engine->Type.Port.SrisAutoDetectMode == 1) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, " SRIS AutoDetect:\n");
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisAutoDetectMode   = 0x%x\n", Engine->Type.Port.SrisAutoDetectMode);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisSkpIntervalSel   = 0x%x\n", Engine->Type.Port.SrisSkpIntervalSel);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "  - SrisAutodetectFactor = 0x%x\n", Engine->Type.Port.SrisAutodetectFactor);
      WritePcieStrap (
          GnbHandle,
          (SIL_RESERVED_865 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
          0x1,
          Wrapper->WrapId
          );
    }

    /*
	 * Symbols per Clock strap programming (All Ports)
     * SPC Gen 1
	 */
    Index = SIL_RESERVED_862 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen1, Wrapper->WrapId);

    /*
	 * SPC Gen 2
	 */
    Index = SIL_RESERVED_863 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen2, Wrapper->WrapId);

    /*
	 * SPC Gen 3
	 */
    Index = SIL_RESERVED_864 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen3, Wrapper->WrapId);

    /*
	 * SPC Gen 4
	 */
    Index = SIL_RESERVED_861 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, Engine->Type.Port.SpcMode.SpcGen4, Wrapper->WrapId);

    /*
	 * PCIe Compliance
	 */
    Index = SIL_RESERVED_847 + (Engine->Type.Port.PortId * SIL_RESERVED_881);
    WritePcieStrap (GnbHandle, Index, 1, Wrapper->WrapId);

    // SELECTABLE_DEEMPHASIS
    if(Engine->Type.Port.TXDeEmphasis != 0xf) {
        WritePcieStrap (
            GnbHandle,
            SIL_RESERVED_843 + (Engine->Type.Port.PortId * SIL_RESERVED_881),
            Engine->Type.Port.TXDeEmphasis,
            Wrapper->WrapId
        );
    }
  }

  /*
   * Only PCIe0 and PCIe3 (Both WrapId 0) support CXL
   */
  if (SilData->AmdCxlOnAllPorts && (Wrapper->WrapId == 0))
  {
    Engine->Type.Port.CxlControl = 1;
  }
  /*
   * CXL Alternate Protocol Support
   */
  if (Engine->Type.Port.CxlControl != 0) {
    IsCxl = (bool *) Buffer;
    *IsCxl = true;

    WritePcieStrap (
      GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_859, Engine->Type.Port.PortId),
      1,
      Wrapper->WrapId
      );

    WritePcieStrap (
      GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_857, Engine->Type.Port.PortId),
      1,
      Wrapper->WrapId
      );

    /*
	 * STRAP_BIF_LC_ALTERNATE_PROTOCOL_DETAILS_2 = 0xF (or 0x7 to disable CXL.cache)
	 */
    WritePcieStrap (
      GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_858, Engine->Type.Port.PortId),
      0x40F,
      Wrapper->WrapId
      );
    /*
	 * SIL_RESERVED_856 = 0x1
	 */
    WritePcieStrap (
      GnbHandle,
      PORT_STRAP_INDEX(SIL_RESERVED_856, Engine->Type.Port.PortId),
      1,
      Wrapper->WrapId
      );

  }

  Status = GetCoreLogicalIdOnCurrentCore ((CORE_LOGICAL_ID*)&LogicalId);
  if (Status != SilPass) {
    if(!((LogicalId.Revision & (AMD_REV_F19_GENOA_AX)) && (LogicalId.Family & AMD_FAMILY_GENOA))) {
      // Temp Gen5 Advertisement for Alternate Protocol
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_525),
        (uint32_t) ~(SIL_RESERVED_393),
        ((SilData->CxlTempGen5AdvertAltPtcl? 1 : 0 ) <<
        SIL_RESERVED_395)
        );
    }
  }

   /*
    * FAPE Enable Gen3
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_522),
        (uint32_t)  ~(SIL_RESERVED_295),
            (Engine->Type.Port.LaneEqualizationCntl.LcFapeEnable8GT <<
              SIL_RESERVED_296)
   );

   /*
    * FAPE Enable Gen4
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_520),
        (uint32_t)  ~(SIL_RESERVED_291),
        (Engine->Type.Port.Gen4LaneEqualizationCntl.LcFapeEnable16GT <<
          SIL_RESERVED_292)
   );

   /*
    * FAPE Enable Gen5
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_521),
        (uint32_t)  ~(SIL_RESERVED_293),
        (Engine->Type.Port.Gen5LaneEqualizationCntl.LcFapeEnable32GT <<
          SIL_RESERVED_294)
   );

   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_BRIDGE_SPACE (GnbHandle, Engine, SIL_RESERVED_747),
        (uint32_t)  ~(SIL_RESERVED_768),
        (Engine->Type.Port.ForceSteering << SIL_RESERVED_769)
   );

   ProgramFapeCoeffList (GnbHandle, Wrapper, Engine);

}

/**-------------------------------------------------------------------
 * MpioCfgBeforeReconfigWrapperCallback
 *
 * @brief Per-Wrapper Callback for wrapper configuration before bifurcation
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgWrapperBeforeBifurcation (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{

  uint16_t         Index;
  bool             IsCxl;
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  IsCxl = false;
  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper (
               DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
               MpioCfgBeforeReconfigCallback,
               (void *) &IsCxl,
               Wrapper
               );

  if (IsCxl) {
    /*
	 * STRAP_BIF_CXL_EN = 0b1
	 */
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_842,
      1,
      Wrapper->WrapId
      );

    /*
	 * CXL correctable error logging
	 */
    xUSLSmnReadModifyWrite (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_750),
          (uint32_t) ~(SIL_RESERVED_229),
          ((SilData->CxlCorrectableErrorLogging? 1 : 0) << SIL_RESERVED_230)
         );

    /*
	 * CXL uncorrectable error logging
	 */
    xUSLSmnReadModifyWrite (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_750),
          (uint32_t) ~(SIL_RESERVED_231),
          ((SilData->CxlUnCorrectableErrorLogging? 1 : 0) << SIL_RESERVED_232)
         );
  }

  /*
   * CPL Abort Error
   */
  WritePcieStrap (
    GnbHandle,
    SIL_RESERVED_841,
    SilData->CfgAEREnable? 1 : 0,
    Wrapper->WrapId
    );

  WritePcieStrap (
    GnbHandle,
    SIL_RESERVED_872,
    (SilData->CfgMcCapEnable || SilData->CfgAEREnable)? 1 : 0,
    Wrapper->WrapId
    );

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_532),
    (uint32_t) ~(SIL_RESERVED2_1092),
    (SilData->CfgRcvErrEnable? 1 : 0) << SIL_RESERVED2_1094
    );

    for (Index = SIL_RESERVED_836; Index <= SIL_RESERVED_837; Index++) {
       WritePcieStrap (
            GnbHandle,
            (uint16_t) Index,
            Wrapper->AcsSupport,
            Wrapper->WrapId
       );
    }

  /*
   * ARI Forwarding
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_888, Wrapper->AriForwarding, Wrapper->WrapId);

  /*
   * Lane Margining
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_870, Wrapper->LaneMargining, Wrapper->WrapId);

  /*
   * Native PCIe Enclosure Management (NPEM)
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_873, Wrapper->NativePCIeEnclosureManagement, Wrapper->WrapId);

  /*
   * Advanced error reporting (AER)
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_838, Wrapper->AdvancedErrorReporting, Wrapper->WrapId);
  WritePcieStrap (GnbHandle, SIL_RESERVED_887, Wrapper->AdvancedErrorReporting, Wrapper->WrapId);
  if (SilData->EarlyBmcLinkTraining) {
    if ((GnbHandle->SocketId == SilData->EarlyBmcLinkSocket) &&
        (SilData->EarlyBmcLinkLaneNum >= Wrapper->StartPhyLane) &&
        (SilData->EarlyBmcLinkLaneNum <= Wrapper->EndPhyLane)) {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_755),
                    (uint32_t) ~(SIL_RESERVED_423),
                    (Wrapper->AdvancedErrorReporting? 1 : 0) << SIL_RESERVED_424
                    );
      /*
       * IOMMU group
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                    WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_755),
                    (uint32_t) ~(SIL_RESERVED_421),
                    1 << SIL_RESERVED_422        //enable ACS capability
                    );
    }
  }

  /*
   * ECRC Capability
   */
  if(Wrapper->ECRCSupport == 1)  // ECRC Generation
  {
      WritePcieStrap (GnbHandle, SIL_RESERVED_849, 1, Wrapper->WrapId);
      WritePcieStrap (GnbHandle, SIL_RESERVED_890, 1, Wrapper->WrapId);
  }
  else if(Wrapper->ECRCSupport == 2) // ECRC Check
  {
       WritePcieStrap (GnbHandle, SIL_RESERVED_848, 1, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_889, 1, Wrapper->WrapId);
  }
  else if(Wrapper->ECRCSupport == 3) // ECRC Generation and Check
  {
       WritePcieStrap (GnbHandle, SIL_RESERVED_849, 1, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_890, 1, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_848, 1, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_889, 1, Wrapper->WrapId);
  }
  else // Not Supported
  {
       WritePcieStrap (GnbHandle, SIL_RESERVED_849, 0, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_890, 0, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_848, 0, Wrapper->WrapId);
       WritePcieStrap (GnbHandle, SIL_RESERVED_889, 0, Wrapper->WrapId);
  }

  /*
   * LTR Support
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_868, Wrapper->LtrSupport, Wrapper->WrapId);

  /*
   * Surprise Down Feature
   */
  WritePcieStrap (GnbHandle, SIL_RESERVED_886,
    SilData->SurpriseDownFeature? 1 : 0, Wrapper->WrapId);

}

/*
 *=========================================================================================
 * Timepoint after reconfig and before port training
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfigCallback
 *
 * @brief Per-Engine Callback for configuration after bifurcation
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
MpioCfgAfterReconfigCallback (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_WRAPPER_CONFIG   *Wrapper
  )
{
  GNB_HANDLE            *GnbHandle;
  uint32_t              Value32;
  PCIe_DPC_STATUS_DATA  *DpcStatusData;
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Wrapper->Header)));

  if (Buffer != NULL) {
    DpcStatusData = (PCIe_DPC_STATUS_DATA*) Buffer;
    Value32 = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_508)
      );
    MPIO_TRACEPOINT (SIL_TRACE_INFO,
      "SocketId=%d, DieNumber=%d, BRIndex=%d Bus=0x%x CoreID=%d PortID=%d PCIE_DPC_STATUS(0x388) = 0x%x\n",
      GnbHandle->SocketId,
      GnbHandle->DieNumber,
      GnbHandle->RBIndex,
      GnbHandle->Address.Address.Bus,
      Wrapper->WrapId,
      (Engine->Type.Port.PortId),
      Value32
    );
    if (Value32 & BIT_32(0)) {
      if (DpcStatusData->size < MAX_NUMBER_DPCSTATUS) {
        DpcStatusData->DpcStatusArray[DpcStatusData->size].SocketId = (uint8_t) GnbHandle->SocketId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DieID = (uint8_t) GnbHandle->DieNumber;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].RBIndex = (uint8_t) GnbHandle->RBIndex;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].BusNumber = (uint8_t) GnbHandle->Address.Address.Bus;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIeCoreID = (uint8_t) Wrapper->WrapId;
        DpcStatusData->DpcStatusArray[DpcStatusData->size].PCIePortID = (uint8_t) (Engine->Type.Port.PortId);
        DpcStatusData->DpcStatusArray[DpcStatusData->size].DpcStatus = (uint16_t) Value32;

        xUSLSmnWrite (
          GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_508),
          Value32
          );
        DpcStatusData->size++;
      }
    }
  }

    /*
	 * Gen3 EQ Search Mode
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_519),
      (uint32_t) ~(SIL_RESERVED_283),
      Engine->Type.Port.EqSearchMode << SIL_RESERVED_284
      );

    /*
	 * Gen4 EQ Search Mode
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_517),
      (uint32_t) ~(SIL_RESERVED_263),
      Engine->Type.Port.EqSearchModeGen4 << SIL_RESERVED_264
      );

    /*
	 * Gen5 EQ Search Mode
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_518),
      (uint32_t) ~(SIL_RESERVED_273),
      Engine->Type.Port.EqSearchModeGen5 << SIL_RESERVED_274
      );

    /*
	 * Gen3 Bypass Phase3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_519),
      (uint32_t) ~(SIL_RESERVED_281),
      Engine->Type.Port.BypassGen3EQ << SIL_RESERVED_282
      );

    /*
	 * Gen4 Bypass Phase3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_517),
      (uint32_t) ~(SIL_RESERVED_261),
      Engine->Type.Port.BypassGen4EQ << SIL_RESERVED_262
      );

    /*
	 * Gen5 Bypass Phase3 EQ
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_518),
      (uint32_t) ~(SIL_RESERVED_271),
      Engine->Type.Port.BypassGen5EQ << SIL_RESERVED_272
      );

    /*
	 * Preset Search Mask Gen3
	 */
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_526),
        (uint32_t)  ~(SIL_RESERVED_401),
        ((((SilData->AmdPciePresetMask8GtAllPort) == 0xFFFFFFFF) ?
            Engine->Type.Port.LaneEqualizationCntl.LcPresetMask8Gt : (SilData->AmdPciePresetMask8GtAllPort))
           << SIL_RESERVED_402)
   );

   /*
    * Preset Search Mask Gen4
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_526),
        (uint32_t)  ~(SIL_RESERVED_397),
        ((((SilData->AmdPciePresetMask16GtAllPort) == 0xFFFFFFFF) ?
            Engine->Type.Port.Gen4LaneEqualizationCntl.LcPresetMask16Gt : (SilData->AmdPciePresetMask16GtAllPort))
           << SIL_RESERVED_398)
   );

   /*
    * Preset Search Mask Gen5
	*/
   xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
          SIL_RESERVED_526),
        (uint32_t)  ~(SIL_RESERVED_399),
        ((((SilData->AmdPciePresetMask32GtAllPort) == 0xFFFFFFFF) ?
            Engine->Type.Port.Gen5LaneEqualizationCntl.LcPresetMask32Gt : (SilData->AmdPciePresetMask32GtAllPort))
           << SIL_RESERVED_400)
   );

    /*
	 * Gen3 Fixed Preset
	 */
    if (Engine->Type.Port.SetGen3FixedPreset) {

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_519),
        (uint32_t)  ~(SIL_RESERVED_285),
        (1 << SIL_RESERVED_286)
      );

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_519),
        (uint32_t)  ~(SIL_RESERVED_287),
        (Engine->Type.Port.Gen3FixedPreset << SIL_RESERVED_288)
      );
    }

    /*
	 * Gen4 Fixed Preset
	 */
    if (Engine->Type.Port.SetGen4FixedPreset) {

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_517),
        (uint32_t)  ~(SIL_RESERVED_265),
        (1 << SIL_RESERVED_266)
      );

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_517),
        (uint32_t)  ~(SIL_RESERVED_267),
        (Engine->Type.Port.Gen4FixedPreset << SIL_RESERVED_268)
      );
    }

    /*
	 * Gen5 Fixed Preset
	 */
    if (Engine->Type.Port.SetGen5FixedPreset) {

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_518),
        (uint32_t)  ~(SIL_RESERVED_275),
        (1 << SIL_RESERVED_276)
      );

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_518),
        (uint32_t)  ~(SIL_RESERVED_277),
        (Engine->Type.Port.Gen5FixedPreset << SIL_RESERVED_278)
      );
    }

  /*
   * Multi Upstream Auto Speed Change
   */
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_528),
      (uint32_t) ~(SIL_RESERVED3_1509),
      Engine->Type.Port.PortFeatures.AutoSpdChngEn << SIL_RESERVED3_1508
  );

  /*
   * Multi Auto Speed Change On Last Rate
   */
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_514),
    (uint32_t) ~(SIL_RESERVED3_1507),
    ((SilData->LcMultAutoSpdChgOnLastRateEnable? 1 : 0) <<
      SIL_RESERVED3_1506)
  );

  /*
   * BIFC.LINK_CNTL_32GT.EQ_BYPASS_TO_HIGHEST_RATE_DIS = !(AdvertiseEqToHiRate)
   */
  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_505),
    (uint32_t) ~(SIL_RESERVED_212),
    !(Engine->Type.Port.Gen5LaneEqualizationCntl.AdvertiseEqToHiRate) <<
     SIL_RESERVED_213
    );

  if (Engine->Type.Port.CxlControl != 0) {
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_500),
      (uint32_t) ~(SIL_RESERVED_52),
      1 << SIL_RESERVED2_937
      );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_505),
      (uint32_t) ~(SIL_RESERVED_214),
      2 << SIL_RESERVED_215
      );

    /*
	 * BIFC.DVSEC_FLEX_BUS_PORT_CONTROL.MEM_EN = 0x1
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle,
                  Wrapper,
                  (Engine->Type.Port.PortId),
                  SIL_RESERVED_503),
      (uint32_t) ~(SIL_RESERVED_60),
      1 << SIL_RESERVED_61
    );

    /*
	 * BIFC.DVSEC_FLEX_BUS_PORT_CONTROL.CACHE_EN = 0x0 (or 0x1 to enable CXL.cache)
     * Disabled - Type 1/2 devices are not supported
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle,
                  Wrapper,
                  (Engine->Type.Port.PortId),
                  SIL_RESERVED_503),
      (uint32_t) ~(SIL_RESERVED_56),
      1 << SIL_RESERVED_57
    );

    /*
	 * Sync header bypass
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
        SIL_RESERVED_503),
      (uint32_t) ~(SIL_RESERVED_58),
      (((SilData->SyncHeaderByPass)? 1 : 0) << SIL_RESERVED_59)
      );
  }

}

/**-------------------------------------------------------------------
 * MpioCfgWrapperAfterBifurcation
 *
 * @brief Per-Wrapper Callback for configuration after bifurcation
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgWrapperAfterBifurcation (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);
  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  NbioIp2Ip->PcieConfigRunProcForAllEnginesInWrapper (
               DESCRIPTOR_ALL_ENGINES,
               MpioCfgAfterReconfigCallback,
               Buffer,
               Wrapper
               );

  if (SilData->AmdRxMarginEnabled) {
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_754),
      (uint32_t) ~(SIL_RESERVED_419),
      (0x1 << SIL_RESERVED_420)
    );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_752),
      (uint32_t) ~(SIL_RESERVED_415 | SIL_RESERVED_413),
      (0x10 << SIL_RESERVED_416) |
      (0x19 << SIL_RESERVED_414)
    );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_753),
      (uint32_t) ~(SIL_RESERVED_417),
      (0xF << SIL_RESERVED_418)
    );

  }

  if (SilData->CfgNbioCTOtoSC) {
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_748),
      (uint32_t) ~(SIL_RESERVED_223),
      1 << SIL_RESERVED_224
    );
  }

  if (SilData->CfgNbioCTOIgnoreError) {
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_748),
      (uint32_t) ~(SIL_RESERVED_221),
      (uint32_t)  (1 << SIL_RESERVED_222)
    );
  }

}

/**-------------------------------------------------------------------
 *
 * SubsystemIdSetting
 *
 * @brief PCIE interface to configure register setting after Mpio init done
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
SubsystemIdSetting (
  GNB_HANDLE       *GnbHandle
  )
{

  uint32_t      Value;
  uint32_t      SubsystemDeviceId;
  uint32_t      SubsystemVendorId;
  MPIOCLASS_INPUT_BLK   *SilData;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  MPIO_TRACEPOINT (SIL_TRACE_INFO,
                   " Enter for Socket %d Nbio %d\n",
                   GnbHandle->SocketId,
                   GnbHandle->RBIndex
                   );

  /*
   * NB ADAPTER D0F0
   */
  Value = SilData->CfgNbioSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "CfgNbioSsid = %x\n", Value);
    xUSLSmnWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, SIL_RESERVED_499),
      Value
      );
  }

  /*
   * IOMMU
   */
  Value = SilData->CfgIommuSsid;
  if (Value != 0) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "CfgIommuSsid = %x\n", Value);
    xUSLSmnWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_ADAPTER_ID_W_ADDRESS),
      Value
      );
  }

  if ((GnbHandle->RBIndex * 1) == 0) {
    Value = SilData->CfgPspccpSsid;
    if (Value != 0) {
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1303), Value);
    }

    Value = SilData->CfgNtbccpSsid;
    if (Value != 0) {
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1308), Value);
    }

    Value = SilData->CfgNbifF0Ssid;
    if (Value != 0) {
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1295),
        Value
        );
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1296), Value);
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1297), Value);
    }

    Value = SilData->CfgNtbSsid;
    if (Value != 0) {
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1298), Value);
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED2_1307), Value);
    }
  }

  /*
   * PCIE
   */
  SubsystemDeviceId = (uint32_t) SilData->AmdPcieSubsystemDeviceID;
  if (SubsystemDeviceId != 0) {
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_884,
      SubsystemDeviceId,
      0
      );
    WritePcieStrap (
      GnbHandle,
      SIL_RESERVED_884,
      SubsystemDeviceId,
      1
      );
  }
  SubsystemVendorId = (uint32_t) SilData->AmdPcieSubsystemVendorID;
  if (SubsystemVendorId != 0) {
    WritePcieStrap (
        GnbHandle,
        SIL_RESERVED_885,
        SubsystemVendorId,
        0
        );
    WritePcieStrap (
        GnbHandle,
        SIL_RESERVED_885,
        SubsystemVendorId,
        1
        );
  }

  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 *
 * GetCcdInfo
 *
 * @brief PCIE interface to configure setting after Mpio init done
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
GetCcdInfo (
    uint32_t  *CcdBitfield
  )
{

  uint32_t ApobInstanceId;
  uint32_t Socket;
  uint32_t Index;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  APOB_TYPE_HEADER                              *ApobEntry;

  Socket = 0;
  ApobInstanceId = Socket;
  AmdGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, 0, &ApobEntry);
  memcpy (&ApobCcdLogToPhysMap, ApobEntry, sizeof (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));

  for (Index = 0;Index < MAX_CCDS_PER_DIE; Index++) {
    if (ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber != CCX_NOT_PRESENT) {
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Ccd: %d\n",
          ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber);
      *CcdBitfield |= (1 << ApobCcdLogToPhysMap.CcdMap[Index].PhysCcdNumber);
    } else {
      break;
    }
  }

}


/**-------------------------------------------------------------------
 * PcieCommonCoreConfiguration
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns SIL_STATUS
 * @retval Should always return SilPass
 **/

SIL_STATUS
PcieCommonCoreConfiguration (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                *GnbHandle;
  uint32_t                  Value;
  MPIOCLASS_INPUT_BLK       *SilData;
  NBIO_IP2IP_API            *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Wrapper->Header)));

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Enter for Socket %d Nbio %d Wrapper %d\n",
            GnbHandle->SocketId, GnbHandle->RBIndex, Wrapper->WrapId);

  Value = (Wrapper->WrapId == 0) ? (uint32_t) SilData->GppAtomicOps : (uint32_t) SilData->GfxAtomicOps;
  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_755),
    (uint32_t) ~(SIL_RESERVED_425 | SIL_RESERVED_427),
    (Value << SIL_RESERVED_426) | (Value << SIL_RESERVED_428)
    );

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_749),
    (uint32_t) ~(SIL_RESERVED_225 | SIL_RESERVED_227),
    (0x0 << SIL_RESERVED_226 | 0x1 << SIL_RESERVED_228)
    );

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_756),
    (uint32_t) ~(SIL_RESERVED_429 | SIL_RESERVED_431),
    (0x0 << SIL_RESERVED_430 | 0x1 << SIL_RESERVED_432)
    );

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_757),
    (uint32_t) ~(SIL_RESERVED_433),
    (0x0 << SIL_RESERVED_434)
    );

  if (SilData->AmdNbioReportEdbErrors ) {
    Value = 0;
  } else {
    Value = 1;
  }
  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE (GnbHandle, Wrapper, SIL_RESERVED_751),
    (uint32_t) ~(SIL_RESERVED_411 | SIL_RESERVED_409),
    Value << SIL_RESERVED_412 | 0x1 << SIL_RESERVED_410
    );

  return SilPass;
}


/**-------------------------------------------------------------------
 *
 * PrintDeliInfo
 *
 * @brief Print the DXIO Error Log Interface (DELI) information for PCIe engine
 *
 * @details tbd
 *
 * @param[in] DXIO_FW_MP1_DELI_INFO
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
PrintDeliInfo(DXIO_FW_MP1_DELI_INFO *pDeliBuf)
{
  uint8_t LGSCount;

  if(pDeliBuf->Hdr.engType == 0){  // Print the DELI log only for PCIe engine
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "\n //////// Header Data ////////\n");
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Number of Dwords = %d\n", pDeliBuf->Hdr.numDWords);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Hdr.version);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "////////// Lane Group Data //////////\n");
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Number of Lane Groupgs = %d\n", pDeliBuf->Dxio.numLGroups);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Dxio.version);
    for (LGSCount = 0; LGSCount < DXIO_FW_MP1_DELI_MAX_NUM_DXIO_LGS; LGSCount++){
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Phy Start Lane = %d\n",pDeliBuf->Dxio.LGS[LGSCount].KPNP_startPhyLane);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Phy End Lane = %d\n",pDeliBuf->Dxio.LGS[LGSCount].KPNP_endPhyLane);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Lane Req Status = %x\n",pDeliBuf->Dxio.LGS[LGSCount].KPNP_LANE_REQ_STATUS);
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "Hardware Debug = %x\n",pDeliBuf->Dxio.LGS[LGSCount].KPMX_HWDEBUG);
    }
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "//////// PCIe Training Data /////////\n");
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Version = %d\n", pDeliBuf->Proto.Pcie.version);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Link State = %x\n", pDeliBuf->Proto.Pcie.PCIE_link_state);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Link Speed = %d\n", pDeliBuf->Proto.Pcie.PCIE_link_speed);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Link Width = %d\n", pDeliBuf->Proto.Pcie.PCIE_link_width);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "Link Active = %x\n", pDeliBuf->Proto.Pcie.PCIE_link_active);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "SWRST_CONTROL = %x\n", pDeliBuf->Proto.Pcie.SWRST_CONTROL_6);
  }

}

/**-------------------------------------------------------------------
 *
 * GetDeliInfo
 *
 * @brief Get the DXIO Error Log Interface (DELI) information to print the log
 *
 * @details tbd
 *
 * @param[in] GNB_HANDLE
 * @param[in] PCIe_ENGINE_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 *
 **/
static
void
GetDeliInfo(
  GNB_HANDLE                *GnbHandle,
  PCIe_ENGINE_CONFIG        *Engine
  )
{
  uint32_t                        Response;
  uint32_t                        MpioArg[6];
  uint8_t                         DELIBuf[sizeof(DXIO_FW_MP1_DELI_INFO)];
  DXIO_FW_MP1_DELI_INFO           *pDeliBuf;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  memset ((void *)DELIBuf, 0, sizeof(DELIBuf));

  memset (MpioArg, 0x00, sizeof(MpioArg));
  MpioArg[1] = 0; // Address High
  MpioArg[2] = (uint32_t) (uintptr_t) DELIBuf; //Address Low
  MpioArg[4] = (uint32_t) Engine->EngineData.StartLane; //StartLane

  MPIO_TRACEPOINT (SIL_TRACE_INFO, "DELI INFO for InstanceId = %d, StartLane = %d \n",
                                GnbHandle->InstanceId,Engine->EngineData.StartLane);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  Response = MpioServiceRequestCommon (NbioIp2Ip->GetHostPciAddress (GnbHandle), BIOS_MPIO_MSG_GET_DELI_INFO, MpioArg, 0);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);

  pDeliBuf = (DXIO_FW_MP1_DELI_INFO*) ((uintptr_t)MpioArg[2]);
  PrintDeliInfo(pDeliBuf);

}

/**-------------------------------------------------------------------
 *
 * PcieCommonEngineGetDeliInfo
 *
 * @brief Get the DXIO Error Log Interface (DELI) information for PCIe engine
 *
 * @details tbd
 *
 * @param[in] PCIe_ENGINE_CONFIG
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
PcieCommonEngineGetDeliInfo (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
)
{

  GNB_HANDLE                  *GnbHandle;
  NBIO_IP2IP_API              *NbioIp2Ip;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));

  GetDeliInfo (GnbHandle, Engine);

}

/**-------------------------------------------------------------------
 *
 * PcieCommonEngineConfiguration
 *
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @details tbd
 *
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
PcieCommonEngineConfiguration (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  GNB_HANDLE                  *GnbHandle;
  PCIe_WRAPPER_CONFIG         *Wrapper;
  SIL_RESERVED_5              TxRequesterId;
  uint32_t                    Value32;
  MPIOCLASS_INPUT_BLK         *SilData;
  NBIO_IP2IP_API              *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)SilFindStructure(SilId_MpioClass,  0);
  assert (SilData != NULL);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Value32 = 0;
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  if ((Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
       ((Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled) &&
        (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugInboard))) {

    MPIO_TRACEPOINT (SIL_TRACE_INFO,
                     " Enter for Socket %d Nbio %d Wrapper %d Engine\n",
                     GnbHandle->SocketId,
                     GnbHandle->RBIndex,
                     Wrapper->WrapId,
                     Engine->Type.Port.PortId
                     );

    TxRequesterId.Value = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_531)
      );

    TxRequesterId.Field.TX_REQUESTER_ID_FUNCTION = Engine->Type.Port.PortData.FunctionNumber;
    TxRequesterId.Field.TX_REQUESTER_ID_DEVICE = Engine->Type.Port.PortData.DeviceNumber;
    TxRequesterId.Field.TX_REQUESTER_ID_BUS = GnbHandle->Address.Address.Bus;

    xUSLSmnWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_531),
      TxRequesterId.Value
      );

    /*
	 * Set Slot Power Limit
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_535),
      (uint32_t) ~(SIL_RESERVED_490),
      (Engine->Type.Port.SlotPowerLimit << SIL_RESERVED_491)
      );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_535),
      (uint32_t) ~(SIL_RESERVED_488),
      (Engine->Type.Port.SlotPowerLimitScale << SIL_RESERVED_489)
      );

    /*
	 * Set slot_implemented
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_507),
      (uint32_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
      (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16))
      );

    /*
	 * Set Physical Slot Number
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_535),
      (uint32_t) ~(SIL_RESERVED_486),
      (Engine->Type.Port.PortData.SlotNum << SIL_RESERVED_487)
      );

    /*
	 * Set Completion Timeout
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_502),
      (uint32_t) ~(SIL_RESERVED_54),
      (0x6 << SIL_RESERVED_55)
      );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_512),
      (uint32_t) ~(SIL_RESERVED_241),
      (0x1 << SIL_RESERVED_242)
      );
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_524),
      (uint32_t) ~(SIL_RESERVED_385 |
        SIL_RESERVED_389),
      (0x1 << SIL_RESERVED_386) |
        (0x1 << SIL_RESERVED_390)
      );
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_513),
      (uint32_t) ~(SIL_RESERVED_243 |
        SIL_RESERVED_245),
      (0x1 << SIL_RESERVED_244) |
      (0 << SIL_RESERVED_246)
      );
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_533 ),
      (uint32_t) ~(SIL_RESERVED_440),
      (0 << SIL_RESERVED_441)
      );

    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_514),
      (uint32_t) ~(SIL_RESERVED3_1505),
      (0x1 << SIL_RESERVED3_1504)
      );
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_530),
      (uint32_t) ~(SIL_RESERVED_407),
      1 << SIL_RESERVED_408
      );
    /*
	 * Set Tx Margin
	 */
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_506),
      (uint32_t) ~(SIL_RESERVED_216),
      (Engine->Type.Port.TXMargin << SIL_RESERVED_218)
      );

    /*
	 * Set Powerdown state in L1/L1.1/L1.2
	 */
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_515),
      (uint32_t) ~(SIL_RESERVED_247),
      ((Engine->Type.Port.PortFeatures.L1PowerDown & 1) << SIL_RESERVED_248)
    );

    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_523),
      (uint32_t) ~(SIL_RESERVED_381),
      ((Engine->Type.Port.PortFeatures.L11PowerDown & 7) << SIL_RESERVED_382)
    );

    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_523),
      (uint32_t) ~(SIL_RESERVED_383),
      ((Engine->Type.Port.PortFeatures.L12PowerDown & 7) << SIL_RESERVED_384)
    );

    /*
	 * SRNS Enable Mode
	 */
    if (Engine->Type.Port.SrisEnableMode == 1 || Engine->Type.Port.SrisAutoDetectMode == 1) {
      Value32 = xUSLSmnRead (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_527)
        );
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "PCIE_LC_SPEED_CNTL = 0x%x\n", Value32);
      Value32 = ((Value32 & SIL_RESERVED_406)? 0x1F :\
                ((Value32 & SIL_RESERVED_405)? 0xF :\
                ((Value32 & SIL_RESERVED_404)? 0x7 :\
                ((Value32 & SIL_RESERVED_403)? 0x3 : 0x1))));
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "set STRAP_LOWER_SKP_OS_GEN_SUPPORT = 0x%x\n", Value32);
      WritePcieStrap (
          GnbHandle,
          (SIL_RESERVED_894 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode != 1 && (SilData->SrisCfgType & (SRIS_DBG | SRIS_DBG_2)))?
           ((Engine->Type.Port.LowerSkpOsGenSup <= Value32)? Engine->Type.Port.LowerSkpOsGenSup : Value32) : 0),
          Wrapper->WrapId
          );
      WritePcieStrap (
          GnbHandle,
          (SIL_RESERVED_895 + (SIL_RESERVED_881 * Engine->Type.Port.PortId)),
          ((Engine->Type.Port.SrisAutoDetectMode != 1 && (SilData->SrisCfgType & (SRIS_DBG | SRIS_DBG_2)))?
           ((Engine->Type.Port.LowerSkpOsRcvSup <= Value32)? Engine->Type.Port.LowerSkpOsRcvSup : Value32) : 0),
          Wrapper->WrapId
          );
      Value32 = xUSLSmnRead (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_516)
        );
      if (Engine->Type.Port.SrisAutoDetectMode == 1) {
        /*
		 * Set AutoDetect Mode & Autodetection factor
		 */
        Value32 &= ~(SIL_RESERVED_258 | SIL_RESERVED_256);
        Value32 |= (Engine->Type.Port.SrisSkpIntervalSel << SIL_RESERVED_259) |\
                   (Engine->Type.Port.SrisAutodetectFactor << SIL_RESERVED_257) |\
                   (1 << SIL_RESERVED_255);
      } else {
        /*
		 * Set SRIS Skip Interval
		 */
        if (Engine->Type.Port.SrisEnableMode == 1 && (SilData->SrisCfgType & (SRIS_DBG | SRIS_DBG_2))) {
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_534),
            (uint32_t) ~(SIL_RESERVED_442),
            (Engine->Type.Port.SrisSkipInterval << SIL_RESERVED_443)
            );
        }
        /*
		 * SRIS
		 */
        if (Engine->Type.Port.SrisEnableMode == 1) {
          Value32 |= (1 << SIL_RESERVED_260);
        }
      }
      xUSLSmnWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_516),
        Value32
        );
    }
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "spc: %d\n", Engine->Type.Port.SpcMode.SpcGen3);
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_516),
      (uint32_t) ~(SIL_RESERVED_249 |
                 SIL_RESERVED_251 |
                 SIL_RESERVED_253),
      (Engine->Type.Port.SpcMode.SpcGen1 << SIL_RESERVED_250) |
      (Engine->Type.Port.SpcMode.SpcGen2 << SIL_RESERVED_252) |
      (Engine->Type.Port.SpcMode.SpcGen3 << SIL_RESERVED_254)
      );

    /*
	 * PCI-E specification states L0 Exit latency should be 7
     * to discourage older software from enabling Rx-L0s (ie. enabling TX-L0s in the EP)
	 */
    WritePcieStrap(
      GnbHandle,
      SIL_RESERVED_850 + ((Engine->Type.Port.PortId) * SIL_RESERVED_881),
      7,
      Wrapper->WrapId
      );

  } else {
    /*
	 * Expose Unused PCIE port
	 */
    if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE)) {
      /*
	   * Set slot_implemented
	   */
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SIL_RESERVED_507),
        (uint32_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK << 16),
        (1 << (PCIE_CAP_SLOT_IMPLEMENTED_OFFSET + 16))
      );
      /*
	   * Assign slot number
	   */
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), SIL_RESERVED_535),
        (uint32_t) ~(SIL_RESERVED_486),
        (Engine->Type.Port.PortData.SlotNum << SIL_RESERVED_487)
      );
    }
  }

  if (SilData->CfgPcieCVTestWA == 1) {
    Value32 = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
       GnbHandle->Address.Address.Bus,
       PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_504)
       );
    if ((Value32 & 0xF) >= 0x03) {
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_513),
        (uint32_t) ~(SIL_RESERVED_245),
        (0 << SIL_RESERVED_246)
      );
    }
  }
}

/**-------------------------------------------------------------------
 * InitBusRangesAndAri
 *
 * @brief After Pcie Training Enumerate all Pcie connectors for register setting.
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in]     Pcie    PCIe configuration info
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
InitBusRangesAndAri (
  GNB_HANDLE                           *GnbHandle,
  PCIe_PLATFORM_CONFIG                 *Pcie,
  bool                                 CfgPcieAriSupport
  )
{
  GNB_HANDLE                      *InnerHandle;
  PCIE_VDM_NODE_CTRL4_STRUCT      VdmNode;
  uint32_t                        SystemFabricID;
  uint32_t                        BusNumberBase;
  uint32_t                        BusNumberLimit;
  uint32_t                        VdmNodeAddress;
  DF_IP2IP_API*                   DfIp2IpApi;
  SIL_STATUS                      Status;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  MPIO_TRACEPOINT (SIL_TRACE_INFO, " Enter for Socket %d RB %d\n", GnbHandle->SocketId, GnbHandle->RBIndex);

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfIp2IpApi);
  assert (Status == SilPass);

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  InnerHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  while (InnerHandle != NULL) {
    DfIp2IpApi->DfGetRootBridgeInfo (InnerHandle->SocketId, InnerHandle->DieNumber, InnerHandle->RBIndex,
                &SystemFabricID, &BusNumberBase, &BusNumberLimit,
                NULL, NULL, NULL);
    VdmNode.Field.BUS_RANGE_BASE = BusNumberBase;
    VdmNode.Field.BUS_RANGE_LIMIT = BusNumberLimit;
    if (InnerHandle->InstanceId == GnbHandle->InstanceId) {
      VdmNode.Field.NODE_PRESENT = 0;
    } else {
      VdmNode.Field.NODE_PRESENT = 1;
    }
    VdmNodeAddress = SMN_IOHUB0_N0NBIO0_PCIE_VDM_NODE_CTRL4_ADDRESS + (InnerHandle->InstanceId * 0x10);
    xUSLSmnWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, VdmNodeAddress),
                       VdmNode.Value
                      );
    InnerHandle = GnbGetNextHandle (InnerHandle);
  }

  SubsystemIdSetting (GnbHandle);
  /*
   * Initialize ARI
   */
  if (true == CfgPcieAriSupport) {
    xUSLSmnReadModifyWrite (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOHC_FEATURE_CNTL_ADDRESS),
      (uint32_t)~(IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK),
      1 << IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET
      );
    PcieAriInit (GnbHandle);
  }
  return;
}

/**-------------------------------------------------------------------
 *
 * MpioIsEverTriedTraining
 *
 * @brief  Check each Engine that ever tried training
 *
 * @details tbd
 *
 * @param[in]     Engine  Engine configuration info
 *
 * @returns boolean
 * @retval true  - This engine is ever tried training
 *         false - This engine is not tried training
 **/
bool
MpioIsEverTriedTraining (
  PCIe_ENGINE_CONFIG        *Engine
)
{
  GNB_HANDLE                    *GnbHandle;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      Value32;
  uint32_t                      SmnAddress;
  uint32_t                      i, j;
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  Value32 = 0;
  SmnAddress = SIL_RESERVED_529;
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  /*
   * Check LC_STATE0 ~ LC_STATE5
   */
  for (i = 0; i < 6; i++) {
    Value32 = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, Engine->Type.Port.PortId, SmnAddress + i * 4)
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
