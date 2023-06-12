/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file Fch.c
 * @brief Initializes Fch Reset, Env and Late stages of FCH
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include "Fch.h"
#include <string.h>
#include "FchHelper.h"
#include "MultiFchInit.h"
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/SilFabricInfo.h>
#include <DF/DfIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <SMU/SmuIp2Ip.h>
#include <FCH/Common/FchCore/FchXhci/FchXhciReg.h>
#include <FCH/Common/FchCore/FchAb/FchAb.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>

extern FCHCLASS_INPUT_BLK InitEnvCfgDefault;
FCHCLASS_INPUT_BLK* const gPtrFchDataBlockParams = &InitEnvCfgDefault;


/**
 * FchUsbDisablePort
 * @brief Disable Fch Usb controller ports before PCI emulation
 *
 *
 * @param[in] Socket Fch configuration structure pointer
 * @param[in] USB3DisableMap Usb3 disabe map data
 * @param[in] USB2DisableMap Usb2 disabe map data
 *
 * @retval SIL_STATUS
 *
 */
SIL_STATUS
FchUsbDisablePort (
  uint8_t Socket,
  uint32_t USB3DisableMap,
  uint32_t USB2DisableMap
  )
{
  uint32_t      FchBusNum = 0;
  uint32_t      Usb2PortDisable;
  uint32_t      Usb3PortDisable;
  uint32_t      UsbPortDisable;
  uint32_t      SmuUsbPort0Disable = 0;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;
  DF_IP2IP_API  *DfIp2IpApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "Smu API not found!\n");
    return Status;
  }
  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Usb2PortDisable = USB2DisableMap;
  Usb3PortDisable = USB3DisableMap;

  FCH_TRACEPOINT(SIL_TRACE_INFO,
      "[FCH]FchEnableUsbPort with XhciUsb3PortDisable = 0x%x, XhciUsb2PortDisable = 0x%x\n",
      Usb3PortDisable,
      Usb2PortDisable);

  if (Usb2PortDisable & BIT_32(0)){
    SmuUsbPort0Disable |= BIT_32(0);
  }
  if (Usb2PortDisable & BIT_32(2)){
    SmuUsbPort0Disable |= BIT_32(8);
  }
  if (Usb3PortDisable & BIT_32(0)){
    SmuUsbPort0Disable |= BIT_32(16);
  }
  if (Usb3PortDisable & BIT_32(2)){
    SmuUsbPort0Disable |= BIT_32(24);
  }

  if (SmuUsbPort0Disable) {
     SmuApi->SmuServiceInitArguments (SmuArg);
     SmuArg[0] = SmuUsbPort0Disable;
     PciAddress.AddressValue = MAKE_SBDFO (0, FchBusNum, 0, 0, 0);
     SmuApi->SmuServiceRequest (PciAddress, SMC_MSG_DisableUsbPort0, SmuArg, 0);
  }

  Usb3PortDisable &= 0x0A;
  Usb2PortDisable &= 0x0A;

  // Port0-1 : Controller0
  UsbPortDisable = (Usb2PortDisable & 0x03) + ((Usb3PortDisable & 0x3) << 16);
  xUSLSmnReadModifyWrite (0, FchBusNum,
      FCH_HS_USB_PORT_DISABLE0,
    ~(uint32_t)(FCH_USB_PORT_DISABLE_MASK),
    UsbPortDisable);

  FCH_TRACEPOINT (SIL_TRACE_INFO,
      "[FCH]FchEnableUsbPort Update USB0 0x%x with 0x%x 0x%x\n",
    FCH_HS_USB_PORT_DISABLE0,
    ~(uint32_t)(FCH_USB_PORT_DISABLE_MASK),
    UsbPortDisable);

  // Port2-3 : Controller1
  Usb3PortDisable = Usb3PortDisable >> 2;
  Usb2PortDisable = Usb2PortDisable >> 2;
  UsbPortDisable = (Usb2PortDisable & 0x03) + ((Usb3PortDisable & 0x3) << 16);
  xUSLSmnReadModifyWrite (0, FchBusNum,
    FCH_HS_USB_PORT_DISABLE0 + FCH_HS_USB_CNTL_STEP,
    ~(uint32_t)(FCH_USB_PORT_DISABLE_MASK),
    UsbPortDisable);

  FCH_TRACEPOINT (SIL_TRACE_INFO,
    "[FCH]FchEnableUsbPort Update USB1 0x%x with 0x%x 0x%x\n",
    FCH_HS_USB_PORT_DISABLE0 + FCH_HS_USB_CNTL_STEP,
    ~(uint32_t)(FCH_USB_PORT_DISABLE_MASK),
    UsbPortDisable);

  return SilPass;
}

/**
 * GetFchDataBlock
 *
 * @brief Get Fch Data Block
 *
 * @param None
 *
 * @retval pointer to FCHCLASS_INPUT_BLK.
*/
FCHCLASS_INPUT_BLK*
GetFchDataBlock (void)
{
  return gPtrFchDataBlockParams;
}

/**
 * FchClassSetInputBlk
 * @brief Establish FCH Parent input defaults
 *
 *
 * This is an IP private function, not visible to the Host
 *
 */
SIL_STATUS FchClassSetInputBlk (
                                void
  )
{
  FCHCLASS_INPUT_BLK *FchInput;

  FchInput = (FCHCLASS_INPUT_BLK *)SilCreateInfoBlock (SilId_FchClass,
                                  sizeof(FCHCLASS_INPUT_BLK),
                                  FCHCLASS_INSTANCE,
                                  FCHCLASS_MAJOR_REV,
                                  FCHCLASS_MINOR_REV);
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FchClassSetInputBlk at: 0x%x \n", FchInput);
  if (FchInput == NULL) {
    return SilAborted;
  }

  // fill FCH structure with defaults
  memcpy ((void *)FchInput, gPtrFchDataBlockParams, sizeof(FCHCLASS_INPUT_BLK));

  return SilPass;
}

/**
 * InitializeFchClassTp1
 * @brief This is an FCH IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS InitializeFchClassTp1 (void) {
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to input blk
  FCHAB_INPUT_BLK *LclInpFchAbBlk; //pointer to Fch Ab input blk
  FCHUSB_INPUT_BLK *LclInpFchUsbBlk; //pointer to Fch Usb input blk
  FCHSATA_INPUT_BLK *LclInpFchSataBlk; //pointer to Fch Sata input blk
  SIL_STATUS Status = SilPass;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure (SilId_FchClass, 0); //Instance0
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH found blk at: 0x%x \n", LclInpFchBlk);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchAbBlk = (FCHAB_INPUT_BLK *) xUslFindStructure (SilId_FchAb, 0); //Instance0
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Ab found blk at: 0x%x \n", LclInpFchAbBlk);
  if (LclInpFchAbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchUsbBlk = (FCHUSB_INPUT_BLK *) xUslFindStructure (SilId_FchUsb, 0); //Instance0
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Usb found blk at: 0x%x \n", LclInpFchUsbBlk);
  if (LclInpFchUsbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchSataBlk = (FCHSATA_INPUT_BLK *) xUslFindStructure (SilId_FchSata, 0); //Instance0
  FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Sata found blk at: 0x%x \n", LclInpFchSataBlk);
  if (LclInpFchSataBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return Status;
}
