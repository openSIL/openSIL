/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioPciePhx.c
 *  @brief Pcie training and hotplug configuration.
 */

#include <string.h>
#include <Mpio/Common/MpioPcie.h>
#include <Mpio/Common/MpioLib.h>
#include <Nbio/NbioIp2Ip.h>
#include <SilSocLogicalId.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_NBIOBASEREG.h>
#include <Nbio/Phx/includePHX/PHX_PCIERCCFG.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>
#include "MpioPcieStrapsPhx.h"
#include "MpioCmn2Phx.h"
#include "MpioIp2IpPhx.h"

#define GPP0_SLOT_CTL_STS_ADDRESS                 0x11100068

typedef struct {
  GNB_HANDLE        *GnbHandle;
  uint8_t           LogicalBridgeId;
} TRAINED;

typedef union {
  struct {
    uint32_t        COMMAND:8;
    uint32_t        RETURN_TYPE:2;
    uint32_t        PARAM_TYPE:6;
    uint32_t        PARAM1:8;
    uint32_t        PARAM2:8;
  } Field;
  uint32_t Value;
} SetBmcLinkInit_STRUCT;

static const uint8_t DefaultPortDevMap [] = {
  DEVFUNC(1, 1),
  DEVFUNC(1, 2),
  DEVFUNC(1, 3),
  DEVFUNC(1, 4),
  DEVFUNC(1, 5),
  DEVFUNC(1, 6),
  DEVFUNC(2, 1),
  DEVFUNC(2, 2),
  DEVFUNC(2, 3),
  DEVFUNC(2, 4),
  DEVFUNC(2, 5),
  DEVFUNC(2, 6)
};

/**
 * PcieGetPortStrapIndexPhx
 *
 * @brief Routine to get the PCIe port strap index value
 *
 * @param  Strap        PCIe strap value
 * @param  Port         Port ID on wrapper
 *
 * @return uint16_t     Port strap index
 */
uint16_t
PcieGetPortStrapIndexPhx (
  uint16_t Strap,
  uint16_t Port
  )
{
  uint16_t PortStrapIndex;

  PortStrapIndex = (Strap + (Port * STRAP_BIF_PORT_DIFF));
  return PortStrapIndex;
}

/**
 * PcieGetPortDpcCapabilityStrapPhx
 *
 * @brief   Routine to get the PCIe strap for per port DPC Capability
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The DPC Capability strap is also used by CXL
 *          to disable DPC capability, if CXL is enabled. This function is
 *          called by CXL using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to disable DPC when CXL is
 *          enabled. Since Phx does not support CXL, this is a dummy function
 *          implemented to maintain common Ip2Ip API across multiple programs.
 *          In Phx, this function returns 0.
 *
 * @return  uint16_t  PCIe strap for per port DPC Capability
 */

uint16_t
PcieGetPortDpcCapabilityStrapPhx (void)
{
  return 0;
}

/**
 * PcieGetTphSupportStrapPhx
 *
 * @brief   Routine to get the PCIe strap for overriding the TPH support
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The TPH override strap is also used by SDCI
 *          to enable TPH support for the downstream ports. This function is
 *          called by SDCI using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to enable TPH support for each
 *          program.
 *
 * @return  uint16_t  PCIe strap for TPH support
 */

uint16_t
PcieGetTphSupportStrapPhx (void)
{
  return ((uint16_t)MPIOSTRAPxE);
}

/**-----------------------------------------------------------------------------------
 * PcieGetMembar0SizeStrapPhx
 *
 * @brief   Routine to get the PCIe strap for the size of MEMBAR0 for CXL
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The MEMBAR0 size strap is also used by CXL
 *          to control the MEMBAR0 size. This function is called by CXL
 *          using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to get the value of MEMBAR0
 *          when CXL is enabled. Since Phx does not support CXL, this is a dummy
 *          function implemented to maintain common Ip2Ip API across multiple.
 *          programs. In Phx, this function returns 0.
 *
 * @return  uint16_t  PCIe strap for the size of MEMBAR0
 *
 */
uint16_t
PcieGetMembar0SizeStrapPhx (void)
{
  return 0;
}

/**-----------------------------------------------------------------------------------
 * PcieGetCxlModeStrapPhx
 *
 * @brief   Routine to get the PCIe strap for CXL mode
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The CXL mode strap is also used by CXL.
 *          This function is called by CXL using MPIO Ip2Ip API (instead
 *          of directly accessing the program specific strap value)
 *          to get the value of CXL mode when CXL is enabled. Since Phx
 *          does not support CXL, this is a dummy function implemented to
 *          maintain common Ip2Ip API across multiple programs. In Phx, this
 *          function returns 0.
 *
 * @return  uint16_t  PCIe strap for CXL mode
 *
 */
uint16_t
PcieGetCxlModeStrapPhx (void)
{
  return 0;
}

/**
 * PcieGetDeviceMappingSizePhx
 *
 * @brief Routine to get the size of the device mapping
 *
 * @return uint32_t      Size of the default port map
 */
uint32_t
PcieGetDeviceMappingSizePhx (void)
{
  return ((uint32_t) (sizeof (DefaultPortDevMap) / sizeof (DefaultPortDevMap[0])));
}

/**
 * PcieGetDeviceMappingPhx
 *
 * @brief Routine to get the the device mapping
 *
 * @return uint8_t*      Pointer to the default port map
 */
uint8_t
*PcieGetDeviceMappingPhx (void)
{
  return (uint8_t *)DefaultPortDevMap;
}

/**
 * PcieGetDevFnPhx
 *
 * @brief Routine to get the device Function
 *
 * @param[in]  GnbHandle   Pointer to the Silicon Descriptor for this node
 * @param[in]  Index       Index in the Port Device Map
 *
 * @return uint8_t         The device Function
 */
uint8_t
PcieGetDevFnPhx (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  )
{
  NB_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  DeviceRemap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, IOHCx13b100b8 + (Index << 2))
    );

  return (uint8_t) DeviceRemap.Field.DevFnMap;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 */
void
PcieSetPortPciAddressMapPhx (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  )
{
  uint8_t   Index;
  uint8_t   DevFuncIndex;
  uint8_t   PortDevMapLocal[sizeof (DefaultPortDevMap)];
  MPIO_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  memcpy((void *) PortDevMapLocal, (void *) DefaultPortDevMap, sizeof (DefaultPortDevMap));
  for (Index = 0; Index < sizeof (DefaultPortDevMap); ++Index) {
    if (PortDevMap[Index] != 0) {
      for (DevFuncIndex = 0; DevFuncIndex < sizeof (DefaultPortDevMap); ++DevFuncIndex) {
        if (PortDevMapLocal[DevFuncIndex] == PortDevMap[Index]) {
          PortDevMapLocal[DevFuncIndex] = 0;
          break;
        }
      }
    }
  }
  for (Index = 0; Index < sizeof (DefaultPortDevMap); ++Index) {
    if (PortDevMap[Index] == 0) {
      for (DevFuncIndex = 0; DevFuncIndex < sizeof (DefaultPortDevMap); ++DevFuncIndex) {
        if (PortDevMapLocal[DevFuncIndex] != 0) {
          PortDevMap[Index] = PortDevMapLocal[DevFuncIndex];
          PortDevMapLocal[DevFuncIndex] = 0;
          break;
        }
      }
    }
    DeviceRemap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, IOHCx13b100b8 + (Index << 2))
      );
    DeviceRemap.Field.DevFnMap = PortDevMap[Index];
    xUSLSmnWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, IOHCx13b100b8 + (Index << 2)),
      DeviceRemap.Value
      );
  }
}

/**
 * WritePcieStrapPhx
 *
 * @brief Routine to write pcie soft straps
 *
 * @param  SilContext        A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle         The associated Gnb Handle
 * @param  StrapIndex        Strap index
 * @param  Value             Contains value of strap register to write with
 * @param  Wrapper           Pcie wrapper number
 */
void
WritePcieStrapPhx (
  SIL_CONTEXT    *SilContext,
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  )
{
  uint32_t  Response;
  uint32_t  MpioArg[6];
  uint8_t   InstanceNumber;

  /*
   * RBIndex is the NBIO #
   */
  InstanceNumber = GnbHandle->RBIndex + Wrapper;

  memset(MpioArg, 0x00, sizeof (MpioArg));
  MpioArg[0] = (uint32_t) StrapIndex + ((uint32_t) InstanceNumber << 16);
  MpioArg[1] = Value;

  Response = MpioServiceRequestCommon(SilContext,
    NbioGetHostPciAddress(GnbHandle),
    MPIO_MSG_PCIE_WRITE_STRAP,
    MpioArg,
    0
    );
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x, InstanceNumber=%d\n", Response, InstanceNumber);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Hot plug pre-initialization on all ports
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Pointer to engine config descriptor
 * @param   Buffer              Not used
 * @param   Pcie                Pointer to global PCIe configuration
 *
 */
void
PcieHotplugPreInitPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UNUSED(Engine);
  UNUSED(Buffer);
  UNUSED(Pcie);
}


/*----------------------------------------------------------------------------------------*/
/**
 * Initialize hotplug features on all hotplug ports
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Pointer to engine config descriptor
 * @param   Buffer              Not used
 * @param   Pcie                Pointer to global PCIe configuration
 *
 */
void
PcieHotplugInitPhx (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  uint32_t              Value;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled ) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found Hotplug Engine at:\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PortId = %d\n", Engine->Type.Port.PortId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PcieBridgeId = %d\n", Engine->Type.Port.PcieBridgeId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.Address = %x\n", Engine->Type.Port.Address);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Type = ");

    Wrapper = PcieConfigGetParentWrapper(Engine);
    GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon(Engine);
    if (GnbHandle == NULL) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid input argument: GnbHandle\n");
      assert(false);
    } else {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHCx13b10028,
        (uint32_t) ~(IOHC_PCIE_CRS_Count_CrsLimitCount_MASK),
        0x262 << IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET
        );

      xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
        0,
        Engine->Type.Port.PortData.DeviceNumber,
        Engine->Type.Port.PortData.FunctionNumber,
        GNBREGx6C
        ),
        (uint32_t) ~(BIT6_MASK_AAAAAZG),
        1 << BIT6_OFFSET_AAAAAZH
        );

      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        WRAP_SPACE(GnbHandle, Wrapper, NBIOBASEREGx1470001c),
        (uint32_t) ~(BIT0_MASK_AAAAADT),
        1 << BIT0_OFFSET_AAAAADU
        );

      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140600),
        (uint32_t) ~(BIT15_MASK_AAAAAGQ),
        0 << BIT15_OFFSET_AAAAAGR
        );

      /*
       * Type specific hotplug configuration
       */
      switch (Engine->Type.Port.PortData.LinkHotplug) {
      /*
       * Basic Hotplug Configuration
       */
      case PcieHotplugBasic:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugBasic\n");

        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT5_MASK_AAAABAG),
          1 << BIT5_OFFSET_AAAABAH
          );

        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140040),
          (uint32_t) ~(BIT3_MASK_AAAAADW | BIT7_MASK_AAAAADY),
          ((0 << BIT3_OFFSET_AAAAADX) | (1 << BIT7_OFFSET_AAAAADZ))
          );
        break;

      /*
       * Enhanced Hotplug Configuration
       */
      case PcieHotplugEnhanced:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugEnhanced\n");
        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT5_MASK_AAAABAG),
          1 << BIT5_OFFSET_AAAABAH
          );

        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140040),
          (uint32_t) ~(BIT3_MASK_AAAAADW | BIT7_MASK_AAAAADY),
          ((1 << BIT3_OFFSET_AAAAADX) | (1 << BIT7_OFFSET_AAAAADZ))
          );

        xUSLPciRead(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          AccessWidth32,
          &Value
          );
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "SLOT_CAP Value = 0x%x\n", Value);
        Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402d4)
          );
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PCIE_LC_CNTL3 Value = 0x%x\n", Value);

        break;
      /*
       * Inboard Hotplug Configuration
       * Inboard Hotplug is similar to Enhanced Hotplug, except that the device must be present at boot
       */
      case PcieHotplugInboard:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugInboard\n");
        if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {


          xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
            0,
            Engine->Type.Port.PortData.DeviceNumber,
            Engine->Type.Port.PortData.FunctionNumber,
            GNBREGx6C
            ),
            (uint32_t) ~(BIT5_MASK_AAAABAG),
            1 << BIT5_OFFSET_AAAABAH
            );

          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140040),
            (uint32_t) ~(BIT3_MASK_AAAAADW),
            1 << BIT3_OFFSET_AAAAADX
            );

          xUSLPciRead(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
            0,
            Engine->Type.Port.PortData.DeviceNumber,
            Engine->Type.Port.PortData.FunctionNumber,
            GNBREGx6C
            ),
            AccessWidth32,
            &Value
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "SLOT_CAP Value = 0x%x\n", Value);
          Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx111402d4)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "PCIE_LC_CNTL3 Value = 0x%x\n", Value);
        }
        break;
      case PcieHotplugServerExpress:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugServerExpress\n");
        /*
         * Express Module
         */
        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT0_MASK_AAAAAZA | BIT1_MASK_AAAAAZQ |
          BIT2_MASK_AAAAAZK | BIT3_MASK_AAAAAZC |
          BIT4_MASK_AAAAAZS | BIT5_MASK_AAAAAZI |
          BIT17_MASK_AAAAAZE | BIT18_MASK_AAAAAZM),
          ((1 << BIT0_OFFSET_AAAAAZB) | (1 << BIT1_OFFSET_AAAAAZR) |
          (0 << BIT2_OFFSET_AAAAAZL) | (1 << BIT3_OFFSET_AAAAAZD) |
          (1 << BIT4_OFFSET_AAAAAZT) | (1 << BIT5_OFFSET_AAAAAZJ) |
          (1 << BIT17_OFFSET_AAAAAZF) |
          (0 << BIT18_OFFSET_AAAAAZN))
          );
        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT16_MASK_AAAAAZY | BIT17_MASK_AAAABAO |
          BIT18_MASK_AAAABAI | BIT19_MASK_AAAABAM |
          BIT20_MASK_AAAABAA | BIT24_MASK_AAAABAE),
          ((1 << BIT16_OFFSET_AAAAAZZ) | (1 << BIT17_OFFSET_AAAABAP) |
          (1 << BIT18_OFFSET_AAAABAJ) | (1 << BIT19_OFFSET_AAAABAN) |
          (1 << BIT20_OFFSET_AAAABAB) | (1 << BIT24_OFFSET_AAAABAF))
          );

        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140040),
          (uint32_t) ~(BIT4_MASK_AAAAAEA),
          1 << BIT4_OFFSET_AAAAAEB
          );

        Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), GPP0_SLOT_CTL_STS_ADDRESS)
          );
        if ((Value & (uint32_t)(1 << 29)) == 0) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, IOHCx13b31004),
            (uint32_t) ~(BIT0_MASK_AAAAAGS),
            (1 << BIT0_OFFSET_AAAAAGT)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x\n",
            IOHC_BRIDGE_SPACE(GnbHandle,
            Engine,
            IOHCx13b31004
            )
            );
        }

        break;

      case PcieHotplugServerEntSSD:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugServerEntSSD\n");
        /*
         * Enterprise SSD
         */
        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT0_MASK_AAAAAZA | BIT1_MASK_AAAAAZQ |
          BIT2_MASK_AAAAAZK | BIT3_MASK_AAAAAZC |
          BIT4_MASK_AAAAAZS | BIT5_MASK_AAAAAZI |
          BIT17_MASK_AAAAAZE | BIT18_MASK_AAAAAZM),
          ((0 << BIT0_OFFSET_AAAAAZB) | (0 << BIT1_OFFSET_AAAAAZR) |
          (0 << BIT2_OFFSET_AAAAAZL) | (0 << BIT3_OFFSET_AAAAAZD) |
          (0 << BIT4_OFFSET_AAAAAZT) | (1 << BIT5_OFFSET_AAAAAZJ) |
          (0 << BIT17_OFFSET_AAAAAZF) |
          (1 << BIT18_OFFSET_AAAAAZN))
          );
        xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          GNBREGx6C
          ),
          (uint32_t) ~(BIT16_MASK_AAAAAZY | BIT17_MASK_AAAABAO |
          BIT18_MASK_AAAABAI | BIT19_MASK_AAAABAM |
          BIT20_MASK_AAAABAA | BIT24_MASK_AAAABAE),
          ((1 << BIT16_OFFSET_AAAAAZZ) | (1 << BIT17_OFFSET_AAAABAP) |
          (1 << BIT18_OFFSET_AAAABAJ) | (1 << BIT19_OFFSET_AAAABAN) |
          (1 << BIT20_OFFSET_AAAABAB) | (1 << BIT24_OFFSET_AAAABAF))
          );

        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), NBIOBASEREGx11140040),
          (uint32_t) ~(BIT4_MASK_AAAAAEA),
          1 << BIT4_OFFSET_AAAAAEB
          );

        Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId % 8), PCIERCCFGx11100068)
          );
        if ((Value & (uint32_t)(1 << BIT29_OFFSET_AAAAADV)) == 0) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, IOHCx13b31004),
            (uint32_t) ~(BIT0_MASK_AAAAATD),
            (1 << BIT0_OFFSET_AAAAATE)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x\n",
            IOHC_BRIDGE_SPACE(GnbHandle,
            Engine,
            IOHCx13b31004
            )
            );
        }

        break;

      default:
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "Invalid Hotplug Type\n");
        assert(false);
        break;
      }
    }
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * PcieEarlyTrainFixupsPhx
 *
 * @brief Package specific fixups for early trained link
 *
 * @param Pcie Pointer to PCIe complex data
 *
 * @returns Nothing
 */
void
PcieEarlyTrainFixupsPhx (
  MPIOCLASS_COMMON_INPUT_BLK       *SilDataCommon,
  GNB_HANDLE                       *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR          *PcieTopologyData
  )
{
  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (ISSOCPHXAM5) {
    SilDataCommon->CfgSkipPspMessage = 0;
  }

  PcieTopologyData->BmcLinkLocation = 0xFF;

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * MpioHotplugConfigureUSB4Phx
 *
 * @brief Map engine to specific PCI device address
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 */
void
MpioHotplugConfigureUSB4Phx (
  GNB_HANDLE  *GnbHandle
  )
{
  uint32_t      ControllerDevice;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  for (ControllerDevice = 3; ControllerDevice < 5; ControllerDevice++) {
    xUSLPciReadModifyWrite32(MAKE_SBDFO(0, 0, ControllerDevice, 1, GNBREGx6C),
      (uint32_t) ~(BIT5_MASK_AAAAAZI | BIT6_MASK_AAAAAZG),
      ((1 << BIT5_OFFSET_AAAAAZJ) | (1 << BIT6_OFFSET_AAAAAZH))
      );
    xUSLPciReadModifyWrite32(MAKE_SBDFO(0, 0, ControllerDevice, 1, GNBREGx70),
      (uint32_t) ~(BIT5_MASK_AAAABAG),
      1 << BIT5_OFFSET_AAAABAH
      );
    xUSLPciReadModifyWrite16(MAKE_SBDFO(0, 0, ControllerDevice, 1, PCIE_CAP_LIST_ADDRESS),
      (uint16_t) ~(PCIE_CAP_SLOT_IMPLEMENTED_MASK),
      1 << PCIE_CAP_SLOT_IMPLEMENTED_OFFSET
      );
    xUSLPciReadModifyWrite32(MAKE_SBDFO(0, 0, ControllerDevice, 1, GNBREGx70),
      (uint32_t) ~(BIT12_MASK_AAAABAC),
      1 << BIT12_OFFSET_AAAABAD
      );
    xUSLPciReadModifyWrite32(MAKE_SBDFO(0, 0, ControllerDevice, 1, GNBREGx70),
      (uint32_t) ~(BIT3_MASK_AAAABAK),
      1 << BIT3_OFFSET_AAAABAL
      );
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * SwapLogicalBridgeIdPhx
 *
 * @brief Swap logical bridge ID (unused in Phx)
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in]       NewBdgIdx       New bridge index to be used
 *
 * @returns Nothing
 */
void
SwapLogicalBridgeIdPhx (
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t                NewBdgIdx
  )
{
  UNUSED(Engine);
  UNUSED(NewBdgIdx);
}

/**
 * FixupLaneMappingForAM5
 *
 * @brief Routine to fixup Pcie Lane mapping in Platform Topology for AM5 package
 *
 * @param[in]     PcieTopologyData    Platform Topology Data
 *
 * @returns Nothing
 */
static
void
FixupLaneMappingForAM5 (
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (ISSOCPHX2) {
    EngineDescriptor = PcieTopologyData->PciePortList;
    while (EngineDescriptor != NULL) {
      if ((EngineDescriptor->EngineData.StartLane == 24) || (EngineDescriptor->EngineData.StartLane == 25)) {
        EngineDescriptor->EngineData.StartLane -= 16;
      }
      if ((EngineDescriptor->EngineData.EndLane == 24) || (EngineDescriptor->EngineData.EndLane == 25)) {
        EngineDescriptor->EngineData.EndLane -= 16;
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor(EngineDescriptor);
    }
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FixupLaneMappingForFP8
 *
 * @brief Routine to fixup Pcie Lane mapping in Platform Topology for FP8 package
 *
 * @param[in]     GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]     PcieTopologyData    Platform Topology Data
 *
 * @returns Nothing
 */
static
void
FixupLaneMappingForFP8 (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  MPIO_PORT_DESCRIPTOR      *EngineDescriptor;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (ISSOCPHX2) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Lane Mapping for PHX2 and HPT2\n");
    EngineDescriptor = PcieTopologyData->PciePortList;
    while (EngineDescriptor != NULL) {
      if ((EngineDescriptor->EngineData.StartLane >= 12) && (EngineDescriptor->EngineData.StartLane <= 19)) {
        EngineDescriptor->EngineData.StartLane += 4;
      }
      if ((EngineDescriptor->EngineData.EndLane >= 12) && (EngineDescriptor->EngineData.EndLane <= 19)) {
        EngineDescriptor->EngineData.EndLane += 4;
      }
      if ((EngineDescriptor->EngineData.StartLane == 4) || (EngineDescriptor->EngineData.StartLane == 5)) {
        EngineDescriptor->EngineData.StartLane += 4;
      }
      if ((EngineDescriptor->EngineData.EndLane == 4) || (EngineDescriptor->EngineData.EndLane == 5)) {
        EngineDescriptor->EngineData.EndLane += 4;
      }
      EngineDescriptor = PcieConfigGetNextDataDescriptor(EngineDescriptor);
    }
  } else {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Lane Mapping for PHX1 and HPT1\n");

    EngineDescriptor = PcieTopologyData->PciePortList;
    while (EngineDescriptor != NULL) {
      if (EngineDescriptor->EngineData.StartLane >= 8) {
        EngineDescriptor->EngineData.StartLane += 8;
      }
      if (EngineDescriptor->EngineData.EndLane >= 8) {
        EngineDescriptor->EngineData.EndLane += 8;
      }

      // Reversal the start lane and end lane for PHX special design.
      if (EngineDescriptor->EngineData.StartLane <= 7) {
        EngineDescriptor->EngineData.StartLane = 7 - EngineDescriptor->EngineData.StartLane;
        EngineDescriptor->EngineData.EndLane = 7 - EngineDescriptor->EngineData.EndLane;
      }

      EngineDescriptor = PcieConfigGetNextDataDescriptor(EngineDescriptor);
    }
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * MpioLaneMappingFixupsPhx
 *
 * @brief Package specific fixups for PCIe lane mapping
 *
 * @param Pcie Pointer to PCIe complex data
 *
 * @returns Nothing
 */
void
MpioLaneMappingFixupsPhx (
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  if (ISSOCPHXAM5) {
    FixupLaneMappingForAM5(PcieTopologyData);
  } else {
    FixupLaneMappingForFP8(GnbHandle, PcieTopologyData);
  }
}

/**
 * MpioGetPortIdPhx
 *
 * @brief Get LogicalBridgeId to map engine to specific PCI device address
 *
 * @param[in] Engine    Pointer to engine config descriptor
 *
 * @returns   PortID
 */
uint8_t MpioGetPortIdPhx (
  PCIe_ENGINE_CONFIG     *Engine
  )
{
  uint8_t    PortId;

  PortId = Engine->Type.Port.LogicalBridgeId;

  return PortId;
}
