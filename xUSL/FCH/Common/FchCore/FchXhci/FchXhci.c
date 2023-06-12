/**
 * @file  FchXhci.c
 * @brief FCH xHCI controller functions
 */
 /* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include "FchXhci.h"
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommonCfg.h>
#include <SMU/SmuIp2Ip.h>

#define FCH_XHCI_MAX_PORT 0x10

static const FCH_USB mFchUsbDefaults = {
  .DieBusNum = 0,
  .Xhci0Enable = false,
  .Xhci1Enable = false,
  .Xhci2Enable = false,
  .XhciECCDedErrRptEn = false,
  .Xhci0DevRemovable = 0,
  .DisableXhciPortLate = false,
  .XhciUsb3PortDisable = 0,
  .XhciUsb2PortDisable = 0,
  .XhciOCpinSelect[0] = {0xFFFFFFFF, 0xFFFF},
  .XhciOCpinSelect[1] = {0xFFFFFFFF, 0xFFFF},
  .XhciOcPolarityCfgLow = false,
  .Usb3PortForceGen1 = 0x0,
  .OemUsbConfigurationTable = {
      .Version_Major = 0x0D,
      .Version_Minor = 0x02,
      .TableLength = 0x50,
      .Usb20PhyEnable = 0x00,
      .Usb20PhyPort = {
                        {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                        {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                        {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                        {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                      },
      .S1Usb20PhyEnable = 0x00,
      .S1Usb20PhyPort = {
                          {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                          {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                          {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                          {0x03, 0x03, 0x03, 0x03, 0x00, 0x01, 0x06, 0x03, 0x01},
                        },
      .Usb31PhyEnable = 0x00,
      .Usb31PhyPort = {{0x00}, {0x00}, {0x00}, {0x00}},
      .S1Usb31PhyEnable = 0x00,
      .S1Usb31PhyPort = {{0x00}, {0x00}, {0x00}, {0x00}},
    },
    .UsbSparseModeEnable = true,
  };


/**
 * FchXhciSmuService
 *
 * @brief Xhci1 SMU Service Request
 *
 * @param DieBusNum Bus Number on Current Die.
 * @param RequestId Request ID.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuService (
  uint32_t DieBusNum,
  uint32_t RequestId
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "Smu API not found!\n");
    assert (false);
  }

  SmuApi->SmuServiceInitArguments (SmuArg);
  PciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  Status = SmuApi->SmuServiceRequest (PciAddress, RequestId, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchXhciSmuUsbConfigUpdate
 *
 * @brief Xhci Smu Usb Config Update
 *
 * @details This routine is to send USB Configure Update message to SMU. SMU
 * holds USB register programming. This message is used to tell SMU which
 * register needs to be updated. It includes register information of address,
 * bitfield mask, programming value, register group.
 *
 * @param DieBusNum   Bus Number on Current Die.
 * @param SmnRegister Register input
 * @param SmnMask     Mask input
 * @param SmnData     Data input
 * @param SmnGroup    Group input
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  uint32_t DieBusNum,
  uint32_t SmnRegister,
  uint32_t SmnMask,
  uint32_t SmnData,
  uint32_t SmnGroup
  )
{
  SMC_RESULT          Status;
  PCI_ADDR            PciAddress;
  uint32_t            SmcMsgIdUsbConfigUpdate;
  uint32_t            SmuArg[6];
  SMU_IP2IP_API       *SmuApi;
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable (SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return SMC_Result_Failed;
  }

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "Smu API not found!\n");
    assert (Status == SMC_Result_OK);
  }

  SmuApi->SmuServiceInitArguments (SmuArg);
  SmuArg[0] = SmnRegister;
  SmuArg[1] = SmnMask;
  SmuArg[2] = SmnData;
  SmuArg[3] = SmnGroup;
  PciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmcMsgIdUsbConfigUpdate = FchXhciXfer->FchGetSmcMsgIdUsbConfigUpdate ();
  Status = SmuApi->SmuServiceRequest (PciAddress, SmcMsgIdUsbConfigUpdate, SmuArg,0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * FchUsb3PortForceGen1
 *
 * @brief Port Force Gen1
 *
 * @details This routine is to program internal USB register to force XHCI port
 * to limit to Gen1 speed.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] PortNum Usb3 Port count of the conntroller.
 * @param[in] UsbPortForceGen1 Parameter to set PortForceGen1. bit0 - port0, bit1 - port1, etc.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsb3PortForceGen1 (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t UsbPortForceGen1,
  uint32_t Usb3SmnBase
  )
{
  uint32_t   Port;
  uint32_t   PortMask;
  uint32_t   DW0_Index;
  uint32_t   DW1_Mask;
  uint32_t   DW2_Data;
  uint32_t   DW3_Op_Group;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_INFO, "Controller UsbPortForceGen1 Parameter = 0x%x\n", UsbPortForceGen1);
  assert (PortNum <= FCH_XHCI_MAX_PORT);

  PortMask = (1 << PortNum) - 1;        //bit map mask of all ports. e.g. if controller has 2 ports, PortMask is 0x11b.
  UsbPortForceGen1 &= PortMask;

  DW0_Index    = Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_PORT_CONTROL_OFFSET;
  DW1_Mask     = PortMask << 16;
  DW2_Data     = (uint32_t) (UsbPortForceGen1 << 16);
  DW3_Op_Group = 0x01000001 + (UsbController << 20);
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    " UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  for (Port = 0; Port < PortNum; Port++) {
    DW0_Index    = Usb3SmnBase + FCH_XHCI_LINK_REGS0_LLUCTL + Port * 0x80;
    DW1_Mask     = 1 << 10;
    DW2_Data     = 0;
    DW3_Op_Group = 0x01000003 + (UsbController << 20);
    if (UsbPortForceGen1 & (1 << Port)) {
      DW2_Data     = 1 << 10;
    }

    FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
    FCH_TRACEPOINT(SIL_TRACE_INFO,
      " UsbConfigUpdate Message with %x, %x, %x, %x\n",
      DW0_Index,
      DW1_Mask,
      DW2_Data,
      DW3_Op_Group
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchUsbOemUsb20PhyConfigure
 *
 * @brief USB 2.0 PHY Platform Configurationb
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. There're 7 USB 2.0 PHY tuning fields
 * available to update:
 *   COMPDSTUNE
 *   TXFSLSTUNE
 *   TXPREEMPAMPTUNE
 *   TXPREEMPPULSETUNE
 *   TXRISETUNE
 *   TXVREFTUNE
 *   TXHSXVTUNE
 *   TXVREFTUNE
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb2.0 port number.
 * @param[in] Usb2Phy USB2.0 PHY configuration parameter.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsbOemUsb20PhyConfigure (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  FCH_USB20_PHY *Usb2Phy,
  uint32_t Usb3SmnBase
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB_20LANEPARACTL0_CNTR0 + 0x400 * Port;
  DW1_Mask     = 0xF3BF7007;
  DW3_Op_Group = 0x01000001 + (UsbController << 20);
  DW2_Data     = (uint32_t) ((Usb2Phy->COMPDSTUNE & 0x07) +
                   ((Usb2Phy->SQRXTUNE & 0x07) << 12) +
                   ((Usb2Phy->TXFSLSTUNE & 0x0F) << 16) +
                   ((Usb2Phy->TXPREEMPAMPTUNE & 0x03) << 20) +
                   ((Usb2Phy->TXPREEMPPULSETUNE & 0x01) << 23) +
                   ((Usb2Phy->TXRISETUNE & 0x03) << 24) +
                   ((Usb2Phy->TXVREFTUNE & 0x0F) << 28)
                   );
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
    FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsbOemUsb20PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB_20LANEPARACTL1_CNTR0 + 0x400 * Port;
  DW1_Mask     = 0x0000000F;
  DW3_Op_Group = 0x01000001 + (UsbController << 20);
  DW2_Data     = (uint32_t) ((Usb2Phy->TXHSXVTUNE & 0x03) +
                   ((Usb2Phy->TXRESTUNE & 0x03) << 2)
                   );
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsbOemUsb20PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchUsbOemUsb31PhyConfigurePort
 *
 * @brief USB 3.1 PHY Platform Configuration for Port
 *
 * @details This routine is to update USB 3.1 PHY settings for each port. Soc
 * has default PHY setting value. Customer may choose to modify the default PHY
 * value according to different board layout design. There're 3 USB 3.1 PHY
 * tuning fields available per port to update:
 *   RX_ANA_IQ_PHASE_ADJUST
 *   RX_EQ_DELTA_IQ_OVRD_VAL
 *   RX_IQ_PHASE_ADJUST
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb3 port number.
 * @param[in] Usb3Phy USB3 PHY configuration parameter.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsbOemUsb31PhyConfigurePort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  FCH_USB31_PHY *Usb3Phy,
  uint32_t Usb3SmnBase
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  DW0_Index    = Usb3SmnBase + SIL_RESERVED_34 + 0x400 * Port;
  DW1_Mask     = 0x0000007F;
  DW3_Op_Group = 0x01000002 + (UsbController << 20);
  DW2_Data = (uint32_t) (Usb3Phy->RX_ANA_IQ_PHASE_ADJUST & 0x7F);
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_RAWLANE0_DIG_PCS_XF_RX_EQ_DELTA_IQ_OVRD_IN + 0x400 * Port;
  DW1_Mask     = 0x0000001F;
  DW3_Op_Group = 0x01000002 + (UsbController << 20);
  DW2_Data = (uint32_t) ((Usb3Phy->RX_EQ_DELTA_IQ_OVRD_VAL & 0xF) +
               ((Usb3Phy->RX_EQ_DELTA_IQ_OVRD_EN & 0x1) << 4));
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST + 0x400 * Port;
  DW1_Mask     = 0x0000007F;
  DW3_Op_Group = 0x01000002 + (UsbController << 20);
  DW2_Data = (uint32_t) (Usb3Phy->RX_IQ_PHASE_ADJUST & 0x7F);
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchUsbOemUsb31PhyConfigureController
 *
 * @brief USB 3.1 PHY Platform Configuration
 *
 * @details This routine is to update USB 3.1 PHY settings for Controller. Soc
 * has default PHY setting value. Customer may choose to modify the default PHY
 * value according to different board layout design. There're 4 USB 3.1 PHY
 * tuning fields available per controller to update:
 *   RX_VREF_CTRL
 *   TX_VBOOST_LVL
 *   RX_VREF_CTRL_X
 *   TX_VBOOST_LVL_X
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Usb2Phy USB3 PHY configuration parameter.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsbOemUsb31PhyConfigureController (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB31_PHY *Usb3Phy,
  uint32_t Usb3SmnBase
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_SUP_DIG_LVL_OVRD_IN;
  DW1_Mask     = 0x000003FF;
  DW3_Op_Group = 0x01000002 + (UsbController << 20);
  DW2_Data = (uint32_t) ((Usb3Phy->RX_VREF_CTRL & 0x1F) +
                ((Usb3Phy->RX_VREF_CTRL_EN & 0x1) << 5) +
                ((Usb3Phy->TX_VBOOST_LVL & 0x7) << 6) +
                ((Usb3Phy->TX_VBOOST_LVL_EN & 0x1) << 9));
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_SUPX_DIG_LVL_OVRD_IN;
  DW1_Mask     = 0x000003FF;
  DW3_Op_Group = 0x01000002 + (UsbController << 20);
  DW2_Data = (uint32_t) ((Usb3Phy->RX_VREF_CTRL_X & 0x1F) +
                ((Usb3Phy->RX_VREF_CTRL_EN_X & 0x1) << 5) +
                ((Usb3Phy->TX_VBOOST_LVL_X & 0x7) << 6) +
                ((Usb3Phy->TX_VBOOST_LVL_EN_X & 0x1) << 9));
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchXhciSparseMode
 *
 * @brief Sparse Control Transaction Enable
 *
 * @details Some devices are slow in responding to Control transfers.
 * Scheduling multiple transactions in one microframe/frame can cause these
 * devices to misbehave. If enabled, the host controller schedules each phase
 * of a Control transfer in different microframes/frames.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchXhciSparseMode(
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  )
{
  if (FchUsbData->UsbSparseModeEnable) {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + FCH_XHCI_GUCTL + 0x200000 * UsbController,
      BIT_32(17),
      BIT_32(17),
      0x01000003 + (UsbController << 20)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciSparseMode on Controller %x to be enabled\n", UsbController);
  } else {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + FCH_XHCI_GUCTL + 0x200000 * UsbController,
      BIT_32(17),
      0,
      0x01000003 + (UsbController << 20)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH]FchHSXhciSparseMode on Controller %x to be disabled\n", UsbController);
  }
}

/**
 * FchXhciOCPolarity
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to program Over-current input polarity configure.
 * By default, OC pin is low when OC occurs.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchXhciOCPolarity (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  )
{
  if (FchUsbData->XhciOcPolarityCfgLow) {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_PORT_CONTROL_OFFSET,
      BIT_32(8),
      BIT_32(8),
      0x01000001 + (UsbController << 20)
      );
    FCH_TRACEPOINT (SIL_TRACE_INFO, "[FCH]FchXhciOCPolarity set Polarity to Low\n");
  } else {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_PORT_CONTROL_OFFSET,
      BIT_32(8),
      0,
      0x01000001 + (UsbController << 20)
      );
    FCH_TRACEPOINT (SIL_TRACE_INFO, "[FCH]FchXhciOCPolarity set Polarity to High\n");
  }
}


/**
 * FchXhciRasFeature
 *
 * @brief Xhci RAS Control
 *
 * @details This routine is to enable ECC error reporting on XHCI controller.
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] FchUsbData USB parameter data structure.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchXhciRasFeature (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  )
{
  if (FchUsbData->XhciECCDedErrRptEn) {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_INTERRUPT_CONTROL_CNTR0,
      BIT_32(12),
      BIT_32(12),
      0x01000001 + (UsbController << 20)
      );
    FCH_TRACEPOINT (SIL_TRACE_INFO, "[FCH]FchHSXhciRasFeature Enable ECC_DedErrRptEn\n");
  } else {
    FchXhciSmuUsbConfigUpdate (
      DieBusNum,
      Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_INTERRUPT_CONTROL_CNTR0,
      BIT_32(12),
      0,
      0x01000001 + (UsbController << 20)
      );
    FCH_TRACEPOINT (SIL_TRACE_INFO, "[FCH]FchHSXhciRasFeature disable ECC_DedErrRptEn\n");
  }
}

/**
 * FchXhciDeviceRemovable
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to program Device Removable register to indicate
 * if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] DeviceRemovable Parameter to set DeviceRemovable. bit[15:0] - USB2.0 ports, bit[31:16] - USB3 ports.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 */
void
FchXhciDeviceRemovable (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DeviceRemovable,
  uint32_t Usb3SmnBase
  )
{
  FchXhciSmuUsbConfigUpdate (
    DieBusNum,
    Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_DEVICE_REMOVABLE_CNTR0,
    DeviceRemovable,
    DeviceRemovable,
    0x01000001 + (UsbController << 20)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciDeviceRemovable UsbConfigUpdate Message with %x, %x, %x, %x\n",
    Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_DEVICE_REMOVABLE_CNTR0,
    DeviceRemovable,
    DeviceRemovable,
    0x01000001 + (UsbController << 20)
    );
}

/**
 * FchXhciOverCurrent
 *
 * @brief Update Xhci OC Pin Mapping
 *
 * @details This routine is to program which Over-Current Pin is mapping to
 * each USB port.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] PortNum Port count of the conntroller.
 * @param[in] OverCurrentMap Parameter to set OC Mapping.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 */
void
FchXhciOverCurrent (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t OverCurrentMap,
  uint32_t Usb3SmnBase
  )
{
  uint32_t PortMask;

  PortMask = 0xFFFFFFFF;
  if (PortNum <= 8) {
    PortMask = PortMask >> ((8 - PortNum) * 4);
  }
  FchXhciSmuUsbConfigUpdate (
    DieBusNum,
    Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_OVERCURRENT_MAP_0_CNTR0,
    PortMask,
    OverCurrentMap,
    0x01000001 + (UsbController << 20)
    );
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchXhciOverCurrent UsbConfigUpdate Message with %x, %x, %x, %x\n",
    Usb3SmnBase + SIL_RESERVED_35 + FCH_XHCI_OVERCURRENT_MAP_0_CNTR0,
    PortMask,
    OverCurrentMap,
    0x01000001 + (UsbController << 20)
    );
}

/**
 * FchUsb31PhySwitchPort
 *
 * @brief Select USB 3.1 PHY Port
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb3 port number.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsb31PhySwitchPort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  uint32_t Usb3SmnBase
  )
{
  uint32_t DW0_Index;
  uint32_t DW1_Mask;
  uint32_t DW2_Data;
  uint32_t DW3_Op_Group;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  DW0_Index    = Usb3SmnBase + FCH_XHCI_PORT_CONTROL_OFFSET;
  DW1_Mask     = 0x0000F000;
  DW3_Op_Group = 0x01000001 + (UsbController << 20);
  DW2_Data     = Port << 12;
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchUsb31PhySwitchPort through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitLateUsbXhci
 * @brief Config USB3 controller before OS Boot
 *
 * @param[in] FchUsbData FCH_USB configuration structure pointer.
 *
 */
void FchInitLateUsbXhci (FCH_USB *FchUsbData)
{
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable (SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return;
  }
  FchXhciXfer->FchInitPreOsXhci (FchUsbData);
}

/**
 * FchUsbSetInputBlk
 * @brief Establish FCH Usb input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchUsbSetInputBlk (void)
{
  FCHUSB_INPUT_BLK *FchUsbInput;

  FchUsbInput = (FCHUSB_INPUT_BLK *) SilCreateInfoBlock (SilId_FchUsb,
                                  sizeof(FCHUSB_INPUT_BLK),
                                  FCHUSB_INSTANCE,
                                  FCHUSB_MAJOR_REV,
                                  FCHUSB_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchSpiSetInputBlk at: 0x%x \n", FchUsbInput);
  if (FchUsbInput == NULL) {
    return SilAborted;
  }

  //Fill Fch Usb structure with defaults
  memcpy ((void *)FchUsbInput, &mFchUsbDefaults, sizeof(FCHUSB_INPUT_BLK));

  return SilPass;
}

/**
 * InitializeFchUsbTp1
 *
 * @brief Config Usb controller during timepoint 1 (pre-pcie)
 *
 */
SIL_STATUS
InitializeFchUsbTp1 (void)
{
  FCHUSB_INPUT_BLK  *LclInpUsbBlk; //pointer to Usb input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpUsbBlk = (FCHUSB_INPUT_BLK *) SilFindStructure (SilId_FchUsb, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Usb found blk at: 0x%x \n", LclInpUsbBlk);
  if (LclInpUsbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchXhciPrePcieInit(LclInpUsbBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchUsbTp2
 *
 * @brief Config Usb controller during timepoint 2
 *
 */
SIL_STATUS
InitializeFchUsbTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchUsbTp3
 *
 * @brief Config Usb controller during timepoint 3
 *
 */
SIL_STATUS
InitializeFchUsbTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/***********************************************************************
 * FchXhciPrePcieInit
 * @brief Config Xhci controller during Power-On
 *
 *
 * @param[in] FchDataBlockParams FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchUsbData FCH_USB configuration structure pointer.
 *
 */
void FchXhciPrePcieInit (
  FCH_USB *FchUsbData
  )
{
  FCH_XHCI_XFER_TABLE *FchXhciXfer;

  if (SilGetCommon2RevXferTable (SilId_FchUsb, (void **)(&FchXhciXfer)) != SilPass) {
    return;
  }
  FchXhciXfer->FchInitPrePcieXhci (FchUsbData);
}
