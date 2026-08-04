/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioInitPhx.c
 *  @brief Primary MPIO init routine for Phx
 */

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL2.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>
#include <Nbio/Phx/includePHX/PHX_NBIFMM.h>
#include <Mpio/MpioClass-api.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/Common/MpioLib.h>
#include <APOB/Common/ApobCmn.h>
#include <string.h>
#include "MpioCmn2Phx.h"
#include "MpioPhxData.h"

typedef struct {
  uint8_t      NbifFunctionType;       ///< Function type, see PHX_NBIF_FUNCTION_TYPES
  uint8_t      DeviceNumber;           ///< NBIF Device Number
  uint8_t      FunctionNumber;         ///< NBIF Function Number
} NBIF_CONFIG_ENTRY;

#define NBIF_CONFIG_ENTRY_CONSTRUCTOR(FUNCTION_TYPE, STRAP_DEV, STRAP_FUN) \
        { FUNCTION_TYPE, STRAP_DEV, STRAP_FUN }

/*
 * Define PCIe device/functions to avoid using magic numbers where they are utilized
 */
#define PCIE_DEVICE0    0
#define PCIE_DEVICE1    1
#define PCIE_DEVICE2    2

#define PCIE_FUNCTION0  0
#define PCIE_FUNCTION1  1
#define PCIE_FUNCTION2  2
#define PCIE_FUNCTION3  3
#define PCIE_FUNCTION4  4
#define PCIE_FUNCTION5  5
#define PCIE_FUNCTION6  6
#define PCIE_FUNCTION7  7

/*
 * Provide a port strap address a base address, device and function are given as inputs
 */
#define  NBIF_PORT_STRAP_ADDRESS(BASE_ADDR, DEVICE)   (BASE_ADDR + ((DEVICE * 2) << 8))

/*
 * Provide a device strap address a base address, device and function are given as inputs
 */
#define  NBIF_DEVICE_STRAPx_ADDRESS(BASE_ADDR, DEVICE, FUNCTION)   (BASE_ADDR + ((2 * FUNCTION) << 8) + (DEVICE << 12))

/*
 * Provide the strap0 address
 */
#define  NBIF_DEVICE_STRAP0_ADDRESS(DEVICE, FUNCTION) \
        NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx10134000, DEVICE, FUNCTION)

#define NBIF_STRAP0_FUNCTION_ENABLE_OFFSET              BIT28_OFFSET_AAAABDA
#define NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET              BIT31_OFFSET_AAAABEW
#define NBIF_INTR_LINE_ENABLE_ADDRESS                   NBIFMMx1013a008

/*
 * Provide the interrupt enable bit offset
 */
#define NBIF_INTR_LINE_ENABLE_OFFSET(DEVICE, FUNCTION)  (8 * DEVICE + FUNCTION)

/*
 * Different NBIF device types for use in calling DeviceControl functions
 */
typedef enum {
  NbifFunctionGfx = 0,
  NbifFunctionDceAz,
  NbifFunctionPspCcp,
  NbifFunctionUsb3Gen1_0,
  NbifFunctionUsb3Gen1_1,
  NbifFunctionAcp,
  NbifFunctionHdAudio,
  NbifFunctionMp2,
  NbifFunctionDummy = 8,
  NbifFunctionIPU,
  NbifFunctionDummyUSB2 = 10,
  NbifFunctionNA1,
  NbifFunctionNA2,
  NbifFunctionUSB3,
  NbifFunctionUSB4,
  NbifFunctionUSBRouter0,
  NbifFunctionUSBRouter1,
  NbifFunctionUnused = 0xFF,
} PHX_NBIF_FUNCTION_TYPES;

NBIF_CONFIG_ENTRY mNbifConfigurations [] =
{
  /*
   * NBIF0 Port0 F0 : GFX
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionGfx,
    PCIE_DEVICE0,
    PCIE_FUNCTION0
    ),
  /*
   * NBIF0 Port0 F1 : DCE.AZ
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionDceAz,
    PCIE_DEVICE0,
    PCIE_FUNCTION1
    ),
  /*
   * NBIF0 Port0 F2 : PSPPCCP
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionPspCcp,
    PCIE_DEVICE0,
    PCIE_FUNCTION2
    ),
  /*
   * NBIF0 Port0 F3 : USB3.1_0
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUsb3Gen1_0,
    PCIE_DEVICE0,
    PCIE_FUNCTION3
    ),
  /*
   * NBIF0 Port0 F4 : USB3.1_1
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUsb3Gen1_1,
    PCIE_DEVICE0,
    PCIE_FUNCTION4
    ),
  /*
   * NBIF0 Port0 F5 : ACP
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionAcp,
    PCIE_DEVICE0,
    PCIE_FUNCTION5
    ),
  /*
   * NBIF0 Port0 F6 : HDAudio
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionHdAudio,
    PCIE_DEVICE0,
    PCIE_FUNCTION6
    ),
  /*
   * NBIF0 Port0 F7 : MP2
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionMp2,
    PCIE_DEVICE0,
    PCIE_FUNCTION7
    ),
  /*
   * NBIF0 Port1 F0 : Dummy - 8
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionDummy,
    PCIE_DEVICE1,
    PCIE_FUNCTION0
    ),
  /*
   * NBIF0 Port1 F1 : IPU   - 9
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionIPU,
    PCIE_DEVICE1,
    PCIE_FUNCTION1
    ),
  /*
   * NBIF0 Port2 F0 : Dummy USB2 - 10
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionDummyUSB2,
    PCIE_DEVICE2,
    PCIE_FUNCTION0
    ),
  /*
   * NBIF0 Port2 F1 : NA   - 11
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionNA1,
    PCIE_DEVICE2,
    PCIE_FUNCTION1
    ),
  /*
   * NBIF0 Port2 F2 : NA2
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionNA2,
    PCIE_DEVICE2,
    PCIE_FUNCTION2
    ),
  /*
   * NBIF0 Port2 F3 : USB3
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUSB3,
    PCIE_DEVICE2,
    PCIE_FUNCTION3
    ),
  /*
   * NBIF0 Port2 F4 : USB4
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUSB4,
    PCIE_DEVICE2,
    PCIE_FUNCTION4
    ),
  /*
   * NBIF0 Port2 F5 : USB Router 0
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUSBRouter0,
    PCIE_DEVICE2,
    PCIE_FUNCTION5
    ),
  /*
   * NBIF0 Port2 F6 : USB Router 1
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUSBRouter1,
    PCIE_DEVICE2,
    PCIE_FUNCTION6
    ),
  /*
   * End Of List
   */
  NBIF_CONFIG_ENTRY_CONSTRUCTOR(NbifFunctionUnused,
    0,
    0
    ),
};

#define NUM_OF_ELEMENTS_MNBIFCONFIGURATIONS    sizeof (mNbifConfigurations) / sizeof (mNbifConfigurations[0])

const MPIOCLASS_PHX_INPUT_BLK mMpioClassDfltsPhx = {
  .PsppPolicy = CONFIG_MPIO_PSPP_MODE,
  .CfgNbifRCSsid = CONFIG_NBIF_RC_SSID,
  .AmdCfgGnbIGPUSSID = CONFIG_GNB_IGPU_SSID,
  .AmdCfgGnbIGPUAudioSSID = CONFIG_GNB_IGPU_AUDIO_SSID,
  .AcpController = CONFIG_MPIO_ACP_CONTROLLER_ENABLE,
  .CfgSensorHubEnable = CONFIG_MPIO_SENSOR_HUB_ENABLE,
  .CfgHdAudioEnable = CONFIG_MPIO_HD_AUDIO_DEV_ENABLE,
  .CfgAcpSsid = CONFIG_MPIO_ACP_SSID,
  .CfgDxioPCIeRSTGenericReset = false,
  .CfgDxioPCIeGPIOResetEP1 = 0,
  .CfgDxioPCIeGPIOResetEP2 = 0,
  .CfgDxioPCIeGPIOResetEP3 = 0,
  .CfgDxioRefClkShutDown = true,
  .CfgDxioPmaPowerGating = true,
  .CfgDxioPmaClockGating = true,
  .CfgDxioStaticPowerGating = true,
  .CfgPcieLoopbackMode = false,
  .AmdLcLoopbackWaitForAllActiveLanes = CONFIG_MPIO_LOOPBACK_CONTROL,
  .CfgDxioAllowCompPass = true,
  .CfgDynamicLanesPowerState = 0,
  .AmdEnableKPXShallowPstate = 0,
  .CfgTbtCompleterEn = true,
  .CfgTbtRequesterEn = true,
  .PcieOBFF = 0x00,
  .PcieCoreMarginIgnoreCSkip = 0x1,
  .CfgPCIeTPowerOnValue = 0x0,
  .AmdDlfCapEnV2 = CONFIG_MPIO_DLF_CAP_ENABLE_V2,
  .AmdDlfExEnV2 = CONFIG_MPIO_DLF_EX_ENABLE_V2,
  .CfgPcieTbtSupport = CONFIG_MPIO_PCIE_TBT_SUPPORT_ENABLE,
  .CfgACSEnable = CONFIG_MPIO_CFG_ACS_ENABLE,
  .CfgPCIeLTREnable = CONFIG_PCIE_LTR_ENABLE,
  .PcieEcrcEnablement = CONFIG_PCIE_ECRC_ENABLE,
  .CfgCombinTrainingEnable = false,
  .AmdAdvertiseEqToHighRateSupport = false
};

/**--------------------------------------------------------------------
 *
 * NbioDisableEnableNbifDevicePhx
 *
 * @brief NBIF Device Enable/Disable
 *
 * @param[in]  GnbHandle          Handle to current NBIO instance
 * @param[in]  DeviceType         Device type as per PHX_NBIF_FUNCTION_TYPES
 * @param[in]  EnableOrDisable    Boolean indicating if device should be disabled or enabled
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
NbioDisableEnableNbifDevicePhx (
  GNB_HANDLE         *GnbHandle,
  uint8_t            DeviceType,
  bool               EnableOrDisable
  )
{
  uint32_t Mask;
  uint8_t  Dev, Fun;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "DeviceType=%x\n", DeviceType);
  /*
   * Bounds check and make sure the entry is actually the one we think it is
   */
  assert((DeviceType < NbifFunctionUnused) && (DeviceType < NUM_OF_ELEMENTS_MNBIFCONFIGURATIONS));
  assert(DeviceType == mNbifConfigurations[DeviceType].NbifFunctionType);

  Dev = mNbifConfigurations[DeviceType].DeviceNumber;
  Fun = mNbifConfigurations[DeviceType].FunctionNumber;

  /*
   * Interrupt line disable/enable
   */
  Mask = (uint32_t) ~(1 << NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun));

  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, NBIF_INTR_LINE_ENABLE_ADDRESS),
    Mask,
    (EnableOrDisable << NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun))
    );
  /*
   * Hit the disable/enable bits in strap
   */
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAP0_ADDRESS(Dev, Fun)),
    (uint32_t) ~(1 << NBIF_STRAP0_FUNCTION_ENABLE_OFFSET),
    (EnableOrDisable << NBIF_STRAP0_FUNCTION_ENABLE_OFFSET)
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "D %d F %d Int Offset %d Strap Addr %x \n",
    Dev,
    Fun,
    NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun),
    NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAP0_ADDRESS(Dev, Fun))
    );

  /*
   * If a function 0 device was requested to be disabled we must enable the dummy function to not lose the other devices
   * NOTE: no support for re enabling fn0 after disabling. We can add this later in the unlikely event it is ever needed
   */
  if (Fun == 0 && EnableOrDisable == false) {
    Mask = (uint32_t) ~(1 << NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET);
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIF_PORT_STRAP_ADDRESS(NBIFMMx10131000, Dev)),
      ~BIT31_MASK_AAAABEV,
      (uint32_t)(1 << NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET)
      );
    if (Dev == 0) {
      /*
       * Graphics disable only
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFMMx10134200),
        (uint32_t) ~BIT28_MASK_AAAABEL,
        (uint32_t)(0 << BIT28_OFFSET_AAAABEM)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx10134000, Dev, Fun)),
        (uint32_t) ~BITS_0_TO_15_MASK_AAAABCY,
        (uint32_t)(0x145A << BIT0_OFFSET_AAAABCZ)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx10134034, Dev, Fun)),
        (uint32_t) ~(BITS_0_TO_7_MASK_AAAABDD |
        BITS_8_TO_15_MASK_AAAABDF |
        BITS_16_TO_23_MASK_AAAABDB),
        (0x0 << BIT0_OFFSET_AAAABDE) |
        (0x0 << BIT8_OFFSET_AAAABDG) |
        (0x13 << BIT16_OFFSET_AAAABDC)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx10134010, Dev, Fun)),
        (uint32_t) ~BITS_28_TO_30_MASK_AAAABEH,
        (uint32_t)(0x0 << BIT28_OFFSET_AAAABEI)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx1013400c, Dev, Fun)),
        (uint32_t) ~(BIT18_MASK_AAAABEB |
        BIT20_MASK_AAAABDZ),
        (0x0 << BIT18_OFFSET_AAAABEC) |
        (0x0 << BIT20_OFFSET_AAAABEA)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(NBIFMMx10134010, Dev, Fun)),
        (uint32_t) ~BITS_23_TO_27_MASK_AAAABEJ,
        (uint32_t)(0x0 << BIT23_OFFSET_AAAABEK)
        );
    }
  }
}

/**--------------------------------------------------------------------
 *
 * NbioDisableNbifDevicePhx
 *
 * @brief NBIF Device Disable
 *
 * @param[in]  GnbHandle          Handle to current NBIO instance
 * @param[in]  DeviceType         Device type as per PHX_NBIF_FUNCTION_TYPES
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
NbioDisableNbifDevicePhx (
  GNB_HANDLE         *GnbHandle,
  uint8_t            DeviceType
  )
{
  NbioDisableEnableNbifDevicePhx(GnbHandle, DeviceType, false);
}

/**--------------------------------------------------------------------
 *
 * NbifDeviceEarlyControlPhx
 *
 * @brief Nbif Device setting Before Dxio init
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @retval    AGESA_STATUS
 **/
void
NbifDeviceEarlyControlPhx (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  uint32_t                  Data;
  MPIOCLASS_PHX_INPUT_BLK   *SilDataPhx;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  /*
   * Get IP block data
   */
  SilDataPhx = (MPIOCLASS_PHX_INPUT_BLK *)xUslFindStructure(SilContext, SilId_MpioClass, MPIOCLASS_PHX_INSTANCE);
  assert(SilDataPhx != NULL);


  if (!SilDataPhx->CfgSensorHubEnable) {
    /*
     * MP2 0x7
     */
    NbioDisableNbifDevicePhx(GnbHandle, NbifFunctionMp2);
  }

  if (!SilDataPhx->AcpController) {
    /*
     * ACP 0x5
     */
    NbioDisableNbifDevicePhx(GnbHandle, NbifFunctionAcp);
  }
  if (!SilDataPhx->CfgHdAudioEnable) {
    /*
     * Hd Audio 0x6
     */
    NbioDisableNbifDevicePhx(GnbHandle, NbifFunctionHdAudio);
  }

  Data = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx5d978
    );

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "GnbHandle->Address.Address.Bus %x Data %x \n",
    GnbHandle->Address.Address.Bus,
    Data
    );
  if (Data & BIT_32(20)) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "NbifFunctionIPU\n");
    /*
     * IPU
     */
    NbioDisableNbifDevicePhx(GnbHandle, NbifFunctionIPU);
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 *
 * IommuInitAfterMpioInitPhx
 *
 * @brief IOMMU Initialization after MPIO init
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
IommuInitAfterMpioInitPhx (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  PCIe_ENGINE_CONFIG    *Engine;
  bool                  CoreUsed;
  uint32_t              Value32;
  NBIO_IP2IP_API        *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  CoreUsed = false;
  Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(GnbHandle->Header)));
  while ((Wrapper != NULL) && (CoreUsed == false)) {
    Engine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));
    while (Engine != NULL) {
      if (PcieLibIsEngineAllocated(Engine)) {
        if ((Engine->Type.Port.PortData.PortPresent == 1) && (Engine->Type.Port.CoreId == 0)) {
          Value32 = xUSLSmnRead(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, IOHCx13b31004)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "Read IOHC_Bridge_CNTL as 0x%x from 0x%x\n",
            Value32,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, IOHCx13b31004)
            );
          if ((Value32 & BIT0_MASK_AAAAATD) == 0) {
            CoreUsed = true;
            MPIO_TRACEPOINT(SIL_TRACE_INFO, "EngineId=%d in use(%d)\n", Engine->Type.Port.LogicalBridgeId, CoreUsed);
            break;
          }
        }
      }
      Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor(Engine, DESCRIPTOR_TERMINATE_TOPOLOGY);
    }
    Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor(Wrapper, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }

  if (CoreUsed == false) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      IOMMUL2x13f0115c,
      (uint32_t) ~(BITS_0_TO_15_MASK_AAAAARR),
      1 << BIT0_OFFSET_AAAAARS
      );
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "set L2_CP_CONTROL_1_CPL1Off\n");
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 *
 * MpioRemoveCxlLinksPhx
 *
 * @brief      Remove CXL links from PCIe topology
 *
 * @details    This function is not supported for Phx
 *
 * @param      SilDataCommon       Mpio input block pointer
 * @param      ComplexDescriptor   Pointer to platform complex descriptor
 * @param      Pcie                Pcie Platform Configuration descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioRemoveCxlLinksPhx (
  MPIOCLASS_COMMON_INPUT_BLK    *SilDataCommon,
  MPIO_COMPLEX_DESCRIPTOR       *PcieTopologyData,
  PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  UNUSED(SilDataCommon);
  UNUSED(PcieTopologyData);
  UNUSED(PcieTopologyData);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

SIL_STATUS
MpioPcieSetSpeed (
  SIL_CONTEXT                   *SilContext,
  PCIe_PLATFORM_CONFIG          *Pcie,
  uint8_t                       PciDevice,
  uint8_t                       PciFunction,
  uint8_t                       TargetSpeed
  )
{
  SIL_STATUS                     Status;
  GNB_HANDLE                     *GnbHandle;
  uint32_t                       EngineId;
  uint32_t                       StartLaneId;
  uint32_t                       EndLaneId;
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;
  uint32_t                       Response;
  uint32_t                       MpioArg[6];
  NBIO_IP2IP_API                 *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "Device %d Function %d to Gen%d\n", PciDevice, PciFunction, TargetSpeed);

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);
  Status = SilUnsupported;
  EngineId = 0xFF;
  StartLaneId = 0xFF;
  EndLaneId = 0xFF;

  PcieWrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(GnbHandle->Header));
  while (PcieWrapper != NULL) {
    PcieEngine =(PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(PcieWrapper->Header)));
    while (PcieEngine != NULL) {
      if ((PcieEngine->Type.Port.PortData.DeviceNumber == PciDevice)
          && (PcieEngine->Type.Port.PortData.FunctionNumber == PciFunction)
          && ((PcieEngine->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) == DESCRIPTOR_ALLOCATED)) {
        EngineId = PcieEngine->Type.Port.PcieBridgeId;
        StartLaneId = PcieEngine->EngineData.StartLane;
        EndLaneId = PcieEngine->EngineData.EndLane;
        if (StartLaneId > EndLaneId) {
          //Handle the port reversal case
          StartLaneId = PcieEngine->EngineData.EndLane;
          EndLaneId = PcieEngine->EngineData.StartLane;
        }
        break;
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    if (EngineId != 0xFF) {
      if (StartLaneId != 0xFF) {

        memset(MpioArg, 0x00, sizeof (MpioArg));
        MpioArg[0] = StartLaneId;
        MpioArg[1] = TargetSpeed;
        Response = MpioServiceRequestCommon(SilContext, GnbHandle->Address, MPIO_MSG_PCIE_SPEED_CHANGE, MpioArg, 0);
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "  StartLaneId = %d, EndLaneId = %d TargetSpeed = %d, MPIO Response = 0x%x\n",
          StartLaneId, EndLaneId, TargetSpeed, Response);
        if ((Response & 0xFF) == 0x01) {
          Status = SilPass;
        }
        break;
      }
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  return Status;
}

static void
FindEarlyLink (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE            *GnbHandle;
  EARLY_LINK_STATUS     *EarlyLinkStatus;

  if (Engine->Type.Port.PortData.MiscControls.SbLink == 1) {
    if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
      EarlyLinkStatus = (EARLY_LINK_STATUS *)Buffer;
      EarlyLinkStatus->EarlyLinkStatus = true;
      GnbHandle = (GNB_HANDLE *)PcieConfigGetParentSilicon(Engine);
      EarlyLinkStatus->PhysicalRootBridge = GnbHandle->RBIndex;
      EarlyLinkStatus->LogicalRootBridge = GnbHandle->LogicalRBIndex;
      EarlyLinkStatus->RootPortBus = (uint8_t) GnbHandle->Address.Address.Bus;
      EarlyLinkStatus->RootPortDevice = Engine->Type.Port.PortData.DeviceNumber;
      EarlyLinkStatus->RootPortFunction = Engine->Type.Port.PortData.FunctionNumber;
    }
  }
}

SIL_STATUS
MpioGetEarlyLinkConfig (
  SIL_CONTEXT                   *SilContext,
  EARLY_LINK_STATUS             *EarlyLinkStatus
  )
{
  PCIe_PLATFORM_CONFIG          *Pcie;
  NBIO_IP2IP_API                *NbioIp2Ip;
  NORTH_BRIDGE_PCIE_SIB         *NbPcieData;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilContext,
                                          SilId_NbioClass,
                                          NBIOPCIECLASS_INSTANCE);
  if (NbPcieData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO Pcie config not found.\n");
    return SilNotFound;
  }

  Pcie = &NbPcieData->PciePlatformConfig;

  EarlyLinkStatus->EarlyLinkStatus = false;
  EarlyLinkStatus->PhysicalRootBridge = 0;
  EarlyLinkStatus->LogicalRootBridge = 0;
  EarlyLinkStatus->RootPortBus = 0;
  EarlyLinkStatus->RootPortDevice = 0;
  EarlyLinkStatus->RootPortFunction = 0;

  NbioIp2Ip->PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    FindEarlyLink,
    EarlyLinkStatus,
    Pcie
    );

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}
