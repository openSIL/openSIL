/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCioInitTc.c
 * @brief FCH CIO Module silicon init functions
 *
 */

#include <SilCommon.h>
#include <FchUsb-api.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FchXhci.h>
#include <FCH/Common/FchCore/FchUsb/FchCio.h>
#include <CommonLib/SmnAccess.h>
#include <IP/FchXhciIp.h>
#include "FchUsbRegTc.h"
#include "FchUsbCmn2Tc.h"
#include "FchUsbOemTc.h"
#include "FchXhciInitTc.h"
#include "FchCioInitTc.h"

#pragma pack (push, 1)
static const SIL_RESERVED_STRUCT_0003 usb_phy_tuning_set_ver_a[] = {
  // TUNING SET INDEX = 0,
  // TUNING SET NAME = VBOOST_7_USB3GEN2
  // DESCRIPTION = This tuning set will set VBOOST=7 for USB3GEN2 mode ONLY
  {
    // field0
    0x3,
    // context_list
    0x7fffe,
    // field2
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, {0x0, 0x0}},
    // field3
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, {0x0, 0x0}},
    // context_list
    0x7fffffffe,
    // field5
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0},
    // field6
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0},
    // context_list
    0x7fffffffffffe,
    // field8
    {0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, },
    // field9
    {0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, },
    // context_list
    0x7fffffffffffe,
    // field11
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0},
    // field12
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0},
    // context_list
    0x7ffe,
    // field14
    {
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
      0x0,
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
      {0x0, 0x0, 0x0} // Spare
    },
    // field15
    {
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
      0x0,
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
      {0x0, 0x0, 0x0} // Spare
    }
  }
};
static const uint32_t usb_phy_tuning_set_size = sizeof (usb_phy_tuning_set_ver_a) /
  sizeof (usb_phy_tuning_set_ver_a[0]);
#pragma pack (pop)

/**
 * FchCioSetNbifStrapRegsTc
 *
 * @brief Disable nBIF EP functions for USB4 RT0/1 if they are not enabled in settings.
 *
 * @details Note: On Server programs, nBIF strap register access is disabled from
 * x86 code. As such, on Client this code should eventually be ported to ESID to
 * align with Server.
 */
static void
FchCioSetNbifStrapRegsTc (
  FCHUSB_INPUT_BLK    *FchUsbData
  )
{
  if ((FchUsbData->Usb4Host[0].InitEnable == false) || (FchUsbData->Usb4Host[0].HostEnable == false)) {
    xUSLSmnReadModifyWrite(0, FchUsbData->DieBusNum, FCH_USB4_RT0_NBIF_STRAP0_TC, ~BIT_32(28), 0);
  }
  if ((FchUsbData->Usb4Host[1].InitEnable == false) || (FchUsbData->Usb4Host[1].HostEnable == false)) {
    xUSLSmnReadModifyWrite(0, FchUsbData->DieBusNum, FCH_USB4_RT1_NBIF_STRAP0_TC, ~BIT_32(28), 0);
  }
}

/**
 * FchCioOverCurrentTc
 * @brief Update USB4 OC Pin Mapping
 *
 * @details This routine is to call common routine to program which
 * Over-Current Pin is mapping to each USB4 port of Phoenix.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioOverCurrentTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC3: Port0-1
  FchXhciOverCurrent(SilContext,
    FchUsbData->DieBusNum,
    3,
    2,
    FchUsbData->XhciOCpinSelect[2].OcPinSelect & 0x000000FF
    );
  // HC4: Port 0-1
  FchXhciOverCurrent(SilContext,
    FchUsbData->DieBusNum,
    4,
    2,
    FchUsbData->XhciOCpinSelect[3].OcPinSelect & 0x000000FF
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioPortForceGen1Tc
 *
 * @brief Force xHCI ports in USB4 routers to limit to Gen 1 speed.
 *
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static void
FchCioPortForceGen1Tc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC3: Port0
  FchUsb3PortForceGen1(SilContext,
    FchUsbData->DieBusNum,
    3,
    1,
    FchUsbData->Usb3PortForceGen1 >> 3
    );

  // HC4: Port0
  FchUsb3PortForceGen1(SilContext,
    FchUsbData->DieBusNum,
    4,
    1,
    FchUsbData->Usb3PortForceGen1 >> 4
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioPcieDisable
 *
 * @brief Disable USB4-PCIe controller.
 *
 * @param PcieController USB4 PCIe controller number.
 * @param FchUsbData     Fch Usb configuration structure pointer.
 */
void FchCioPcieDisableTc (
  uint32_t         PcieController,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  // BridgeDis + CfgDis
  xUSLSmnReadModifyWrite(0,
    FchUsbData->DieBusNum,
    PcieController == 0 ? FCH_IOHC_BRIDGE_CNTL_PCIE2_TC : FCH_IOHC_BRIDGE_CTRL_PCIE3_TC,
    ~(BIT_32(0) + BIT_32(2)),
    BIT_32(0) + BIT_32(2)
    );
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioTunnelDisableTc
 *
 * @brief Disable protocol tunneling.
 *
 * @details This routine is to call common routine to disable protocol tunneling
 * for each USB4 router.
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static
void FchCioTunnelDisableTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioTunnelDisable(SilContext,
    FchUsbData->DieBusNum,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioTunnelDisable(SilContext,
    FchUsbData->DieBusNum,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioAdapterHideTc
 *
 * @brief Hide protocol adapters.
 *
 * @details This routine is to call common routine to hide protocol adapters
 * for each USB4 router.
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static
void FchCioAdapterHideTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioAdapterHide(SilContext,
    FchUsbData->DieBusNum,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioAdapterHide(SilContext,
    FchUsbData->DieBusNum,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioTbt3DisableTc
 *
 * @brief Disable USB4 Thunderbolt 3 compatibility.
 *
 * @details This routine is to call common routine to disable USB4 Thunderbolt 3 compatibility
 * for each USB4 router.
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static
void FchCioTbt3DisableTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioTbt3Disable(SilContext,
    FchUsbData->DieBusNum,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioTbt3Disable(SilContext,
    FchUsbData->DieBusNum,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioTxFFEConfigureTc
 *
 * @brief Configure TxFFE.
 *
 * @details This routine is to call common routine to configure TxFFE
 * for each USB4 router.
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static
void FchCioTxFFEConfigureTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioTxFFEConfigure(SilContext,
    FchUsbData->DieBusNum,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioTxFFEConfigure(SilContext,
    FchUsbData->DieBusNum,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioGen3Support
 *
 * @brief Configure USB4 Gen 3 support
 *
 * @details This routine is to call common routine to configure USB4
 * Gen3 support for each USB4 router.
 *
 * @param SilContext          A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param FchUsbData          Fch Usb configuration structure pointer.
 */
static
void FchCioGen3SupportTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioGen3Support(SilContext,
    FchUsbData->DieBusNum,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioGen3Support(SilContext,
    FchUsbData->DieBusNum,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioOemUsb20PhyTc
 *
 * @brief USB 2.0 PHY Platform Configuration for USB4 host controllers.
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param PlatformUsbConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static void
FchCioOemUsb20PhyTc (
  SIL_CONTEXT *SilContext,
  uint32_t DieBusNum,
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // HC3 (1 USB2): Port0
  // Port is 6 because HC0 and HC1 have 6 USB2.0 ports in total
  FchUsbOemUsb20PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    3,
    0,
    &(PlatformUsbConfigureTable->field4[6])
    );
  // HC4 (1 USB2): Port0
  FchUsbOemUsb20PhyConfigurePerPortTc(SilContext,
    DieBusNum,
    4,
    0,
    &(PlatformUsbConfigureTable->field4[7])
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchCioOemUsb4PhyTuningSetDumpTc
 *
 * @brief Dump the contents of the SIL_RESERVED_STRUCT_0003 structure that
 * PhyTuningSetPtr points to.
 *
 * @param PhyTuningSetPtr Pointer to a SIL_RESERVED_STRUCT_0003 structure.
 */
static void
FchCioOemUsb4PhyTuningSetDumpTc (
  const SIL_RESERVED_STRUCT_0003 *PhyTuningSetPtr
  )
{
  uint16_t *Ptr16;
  uint16_t Data16;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FCH_TRACEPOINT(SIL_TRACE_INFO, "phy_list: %lx\n", PhyTuningSetPtr->field0);

  FCH_TRACEPOINT(SIL_TRACE_INFO, "mplla_context_list: %lx\n", PhyTuningSetPtr->field1);

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field2);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "mplla_context_var:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0015) / 2 ; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field3);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "mplla_context_var_mask:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0015) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "mpllb_context_list: %lx\n", PhyTuningSetPtr->field4);

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field5);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "mpllb_context_var:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0016) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field6);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "mpllb_context_var_mask:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0016) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "cmn_context_list: %lx\n", PhyTuningSetPtr->field7);

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field8);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "cmn_context_var:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0004) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field9);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "cmn_context_var_mask:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0004) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "tx_context_list: %lx\n", PhyTuningSetPtr->field10);

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field11);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "tx_context_var:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0024) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field12);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "tx_context_var_mask:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0024) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "rx_context_list: %lx\n", PhyTuningSetPtr->field13);

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field14);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "rx_context_var:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0022) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  Ptr16 = (uint16_t *)(&PhyTuningSetPtr->field15);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "rx_context_var_mask:\n");
  for (Data16 = 0; Data16 < sizeof (SIL_RESERVED_STRUCT_0022) / 2; Data16++) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "0x%x\n", *(Ptr16 + Data16));
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioOemUsb4PhyTc
 *
 * @brief USB4 C20 PHY tuning
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 */
static void
FchCioOemUsb4PhyTc (
  SIL_CONTEXT *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (!(FchUsbData->Usb4PhyTuningEnable)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "USB4 PHY tuning is not enabled.\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  if ((FchUsbData->Usb4PhyTuningSetPtr == 0) || (FchUsbData->Usb4PhyTuningSetSize == 0)) {
    FchUsbData->Usb4PhyTuningSetPtr = (uint32_t)(size_t)usb_phy_tuning_set_ver_a;
    FchUsbData->Usb4PhyTuningSetSize = usb_phy_tuning_set_size;
  }

  FchCioOemUsb4PhyTuningSetDumpTc((SIL_RESERVED_STRUCT_0003 *)(uintptr_t)(FchUsbData->Usb4PhyTuningSetPtr)
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "Updated Usb4PhyTuningSetPtr %x, Usb4PhyTuningSetSize %x\n",
    FchUsbData->Usb4PhyTuningSetPtr,
    FchUsbData->Usb4PhyTuningSetSize
    );

  // USB4 Router 0 PHY
  if (FchUsbData->Usb4Host[0].InitEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      FchUsbData->Usb4PhyTuningSetPtr,
      FchUsbData->Usb4PhyTuningSetSize & 0xFF,
      0x00000000,
      0xF0500001
      );
  }
  // USB4 Router 1 PHY
  if (FchUsbData->Usb4Host[1].InitEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      FchUsbData->Usb4PhyTuningSetPtr,
      FchUsbData->Usb4PhyTuningSetSize & 0xFF,
      0x00000000,
      0xF0700001
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchCioOemConfigureTc  -  USB4 OEM Platform Configuration
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioOemConfigureTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  SIL_RESERVED_STRUCT_0012 *PlatformUsbConfigureTable;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_INFO, "sizeof (uintptr_t) = 0x%x\n", sizeof (uintptr_t));

  PlatformUsbConfigureTable = (SIL_RESERVED_STRUCT_0012 *)(uintptr_t) FchUsbData->OemUsbConfigurationTable;

  if (FchUsbCheckOemTableValidTc(PlatformUsbConfigureTable)) {
    FchCioOemUsb20PhyTc(SilContext, FchUsbData->DieBusNum, PlatformUsbConfigureTable);
    FchCioOemUsb4PhyTc(SilContext, FchUsbData);
  } else {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Oem configure is not called!\n");
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioPassParameterTc
 *
 * @brief USB4 Pass Parameters
 *
 * @details This routine is to call Phoenix/TC specific functions to send
 * register update messages to SMU for USB4 initialization.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioPassParameterTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchCioOverCurrentTc(SilContext, FchUsbData);
  FchCioPortForceGen1Tc(SilContext, FchUsbData);
  FchCioTunnelDisableTc(SilContext, FchUsbData);
  FchCioAdapterHideTc(SilContext, FchUsbData);
  FchCioTbt3DisableTc(SilContext, FchUsbData);
  FchCioTxFFEConfigureTc(SilContext, FchUsbData);
  FchCioGen3SupportTc(SilContext, FchUsbData);
  FchCioOemConfigureTc(SilContext, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioPhyLoadTc
 *
 * @brief USB4 PHY loading
 *
 * @details This routine is to call common routine to enable USB4 PHY loading.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioPhyLoadTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioPhyLoad(SilContext,
    0,
    FCH_USB_HOST_SELECT_USB4PHY0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioPhyLoad(SilContext,
    1,
    FCH_USB_HOST_SELECT_USB4PHY1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioPdInterruptModeTc
 *
 * @details This routine is to call common routine to enable PD interrupt mode
 * for USB4 RT0/1.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioPdInterruptModeTc (
  SIL_CONTEXT        *SilContext,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0: Bit 2
  if (FchUsbData->Usb4Host[0].InitEnable && (FchUsbData->PdInterruptModeEn & BIT_8(2))) {
    FchXhciPdInterruptMode(SilContext, FchUsbData->DieBusNum, FCH_USB_HOST_SELECT_USB4PHY0_TC);
  }
  // USB4 RT1: Bit 3
  if (FchUsbData->Usb4Host[1].InitEnable && (FchUsbData->PdInterruptModeEn & BIT_8(3))) {
    FchXhciPdInterruptMode(SilContext, FchUsbData->DieBusNum, FCH_USB_HOST_SELECT_USB4PHY1_TC);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioClxEnableTc
 *
 * @details This routine is to call common routine to enable Clx mode
 * for USB4 RT0/1.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioClxEnableTc (
  SIL_CONTEXT        *SilContext,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioClxEnable(SilContext,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioClxEnable(SilContext,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioP4PgEnableTc
 *
 * @details This routine is to call common routine to enable P4.PG
 * for USB4 RT0/1.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioP4PgEnableTc (
  SIL_CONTEXT        *SilContext,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioP4PgEnable(SilContext,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioP4PgEnable(SilContext,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioLinkSpeedTc
 *
 * @details This routine is to call common routine to enable Gen3 link speed
 * for USB4 RT0/1.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioLinkSpeedTc (
  SIL_CONTEXT        *SilContext,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // USB4 RT0
  FchCioLinkSpeed(SilContext,
    0,
    FCH_USB_HOST_SELECT_USB4RT0_TC,
    FchUsbData
    );
  // USB4 RT1
  FchCioLinkSpeed(SilContext,
    1,
    FCH_USB_HOST_SELECT_USB4RT1_TC,
    FchUsbData
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioDebugSCDisableTc
 *
 * @details This routine is to call common routine to enable or disable debug
 * streaming control for USB4 RT0/1.
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchCioDebugSCDisableTc (
  SIL_CONTEXT        *SilContext,
  FCHUSB_INPUT_BLK   *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Debug Streaming is programmed in lockstep between HC3/4
  // USB4RT0/1 or USB4PHY0/1 are therefore valid in the message
  if (
    ((FchUsbData->Usb4DebugSCDisable & BIT_16(0)) == 0x0)
    && (FchUsbData->Usb4Host[0].InitEnable || FchUsbData->Usb4Host[1].InitEnable)
    ) {
    FchCioDebugSCDisable(SilContext, FchUsbData->DieBusNum, FCH_USB_HOST_SELECT_USB4RT0_TC, false);
  } else {
    FchCioDebugSCDisable(SilContext, FchUsbData->DieBusNum, FCH_USB_HOST_SELECT_USB4RT0_TC, true);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioInitS3ExitProgramTc
 *
 * @brief Config USB4 controllers during S3 Exit
 *
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch USB configuration structure pointer.
 *
 */
static void
FchCioInitS3ExitProgramTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_BIOSSMC_MSG_INPUT_BLK *FchBiosSmcMsg;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchXhciCheckUsbControllerSkip(FchUsbData->DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip All USB Controller Access\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  FchCioPassParameterTc(SilContext, FchUsbData);

  FchBiosSmcMsg = (FCH_BIOSSMC_MSG_INPUT_BLK *) xUslFindStructure(SilContext,
    SilId_FchUsb,
    FCH_BIOSSMC_MSG_INPUT_BLK_INSTANCE
    );

  if (FchBiosSmcMsg == NULL) {
    assert(false);
    return;
  }

  FchXhciSmuService(SilContext, FchUsbData->DieBusNum, FchBiosSmcMsg->UsbSxExit);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchCioInitBootProgramTc
 *
 * @brief Config USB4 controller during Power-On
 *
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch USB configuration structure pointer.
 *
 */
static void FchCioInitBootProgramTc (
  SIL_CONTEXT      *SilContext,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchXhciCheckUsbControllerSkip(FchUsbData->DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip All USB Controller Access\n");
    FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
    return;
  }

  FchCioPassParameterTc(SilContext, FchUsbData);

  if (FchXhciCheckUsbPhySkip(FchUsbData->DieBusNum)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Skip USB PHY\n");
  } else {
    FchCioPhyLoadTc(SilContext, FchUsbData);
  }

  FchCioPdInterruptModeTc(SilContext, FchUsbData);
  FchCioClxEnableTc(SilContext, FchUsbData);
  FchCioP4PgEnableTc(SilContext, FchUsbData);
  FchCioLinkSpeedTc(SilContext, FchUsbData);
  FchCioDebugSCDisableTc(SilContext, FchUsbData);
}

/**
 * FchInitResetCioTc
 *
 * @brief Initialize USB4 controllers during early Power-On
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
static void
FchInitResetCioTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchCioSetNbifStrapRegsTc(FchUsbData);

  if (FchUsbData->Usb4Host[0].InitEnable || FchUsbData->Usb4Host[1].InitEnable) {
    if (SilFchReadSleepType () == ACPI_SLPTYP_S3) {
      FchCioInitS3ExitProgramTc(SilContext, FchUsbData);
    } else {
      FchCioInitBootProgramTc(SilContext, FchUsbData);
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*
 * FchInitPrePcieCioTc
 *
 * @brief Config FCH CIO Module before PCI enumeration.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   FchUsbData          Fch USB configuration structure pointer.
 *
 */
void
FchInitPrePcieCioTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchInitResetCioTc(SilContext, FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
