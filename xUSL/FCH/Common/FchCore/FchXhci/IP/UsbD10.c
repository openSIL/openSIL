/**
 * @file  UsbD10.c
 * @brief FCH USB Silicon code for USB D.10 IP
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <FCH/Common/FchCommonCfg.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchHelper.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>

/*********************************************************************************************************************/
/*                                       USB D.10 (Genoa) IP code                                                     */
/*********************************************************************************************************************/
#define SN_XHCI_USB31_SUP_DIG_LVL_OVRD_IN                           0x120088ul
/**
 * @brief FchUsbOemUsb31PhyConfigurePortD10  -  USB 3.1 PHY Platform Configuration for Port
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] UsbController USB3 controller number.
 * @param[in] Port    Usb3 port number.
 * @param[in] Usb3Phy USB3 PHY configuration parameter.
 * @param[in] Usb3SmnBase USB3 controller SMN base address.
 *
 */
void
FchUsbOemUsb31PhyConfigurePortD10 (
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

  // Switch Port (PHY) first
  FchUsb31PhySwitchPort (
    DieBusNum,
    UsbController,
    Port,
    Usb3SmnBase
  );

  DW0_Index    = Usb3SmnBase + SIL_RESERVED_34;
  DW1_Mask     = 0x0000007F;                                                          //Register bit field
  DW3_Op_Group = 0x01000002 + (UsbController << 20);                                  //OpCode 0x01, Register Group2
  DW2_Data = (uint32_t) (Usb3Phy->RX_ANA_IQ_PHASE_ADJUST & 0x7F);                     //Programming Value
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_RAWLANE0_DIG_PCS_XF_RX_EQ_DELTA_IQ_OVRD_IN;  //Register Address
  DW1_Mask     = 0x0000001F;                                                               //Register bit field
  DW3_Op_Group = 0x01000002 + (UsbController << 20);                                       //OpCode 0x01, Group 2
  DW2_Data = (uint32_t) ((Usb3Phy->RX_EQ_DELTA_IQ_OVRD_VAL & 0xF) +
               ((Usb3Phy->RX_EQ_DELTA_IQ_OVRD_EN & 0x1) << 4));                            //Programming Value
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + FCH_XHCI_USB31_RAWLANE0_DIG_AON_RX_IQ_PHASE_ADJUST;    //Register Address
  DW1_Mask     = 0x0000007F;                                                          //Register Bit field
  DW3_Op_Group = 0x01000002 + (UsbController << 20);                                  //OpCode 0x01, Group 2
  DW2_Data = (uint32_t) (Usb3Phy->RX_IQ_PHASE_ADJUST & 0x7F);                         //Programming Value
  FchXhciSmuUsbConfigUpdate (DieBusNum, DW0_Index, DW1_Mask, DW2_Data, DW3_Op_Group);
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "[FCH]FchHSUsbOemUsb31PhyConfigure through UsbConfigUpdate Message with %x, %x, %x, %x\n",
    DW0_Index,
    DW1_Mask,
    DW2_Data,
    DW3_Op_Group
    );

  DW0_Index    = Usb3SmnBase + SN_XHCI_USB31_SUP_DIG_LVL_OVRD_IN;                     //Register Address
  DW1_Mask     = 0x000000FF;                                                          //Register bit field
  DW3_Op_Group = 0x01000002 + (UsbController << 20);                                  //OpCode 0x01, Group 2
  DW2_Data = (uint32_t) ((Usb3Phy->RX_VREF_CTRL & 0x7) +
                ((Usb3Phy->RX_VREF_CTRL_EN & 0x1) << 3) +
                ((Usb3Phy->TX_VBOOST_LVL & 0x7) << 4) +
                ((Usb3Phy->TX_VBOOST_LVL_EN & 0x1) << 7));                            //Programming Value
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
