/**
 * @file  FchXhci.h
 * @brief FCH xHCI controller function prototypes
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCommonCfg.h>
#include "FchXhciReg.h"
#include <SMU/SmuDefs.h>

#define ACPI_SLPTYP_S3 0x3
/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

void FchXhciPrePcieInit (FCH_USB *FchUsbData);
void FchInitLateUsbXhci (FCH_USB *FchUsbData);
SIL_STATUS FchUsbSetInputBlk (void);
SIL_STATUS InitializeFchUsbTp1 (void);
SIL_STATUS InitializeFchUsbTp2 (void);
SIL_STATUS InitializeFchUsbTp3 (void);

SMC_RESULT FchXhciSmuService (uint32_t DieBusNum, uint32_t RequestId);

SMC_RESULT
FchXhciSmuUsbConfigUpdate (
  uint32_t DieBusNum,
  uint32_t SmnRegister,
  uint32_t SmnMask,
  uint32_t SmnData,
  uint32_t SmnGroup
  );

void
FchUsb3PortForceGen1 (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t UsbPortForceGen1,
  uint32_t Usb3SmnBase
  );

void
FchUsbOemUsb20PhyConfigure (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  FCH_USB20_PHY *Usb2Phy,
  uint32_t Usb3SmnBase
  );

void
FchUsbOemUsb31PhyConfigurePort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  FCH_USB31_PHY *Usb3Phy,
  uint32_t Usb3SmnBase
  );

void
FchUsbOemUsb31PhyConfigureController (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB31_PHY *Usb3Phy,
  uint32_t Usb3SmnBase
  );

void
FchXhciSparseMode(
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  );

void
FchXhciOCPolarity (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  );

void
FchXhciRasFeature (
  uint32_t DieBusNum,
  uint32_t UsbController,
  FCH_USB *FchUsbData,
  uint32_t Usb3SmnBase
  );

void
FchXhciDeviceRemovable (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t DeviceRemovable,
  uint32_t Usb3SmnBase
  );

void
FchXhciOverCurrent (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t PortNum,
  uint32_t OverCurrentMap,
  uint32_t Usb3SmnBase
  );

void
FchUsb31PhySwitchPort (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  uint32_t Usb3SmnBase
  );

/*********************************************************************************************************************/
/*                                       USB D.10 IP code                                                            */
/*********************************************************************************************************************/
void
FchUsbOemUsb31PhyConfigurePortD10 (
  uint32_t DieBusNum,
  uint32_t UsbController,
  uint32_t Port,
  FCH_USB31_PHY *Usb3Phy,
  uint32_t Usb3SmnBase
  );


/*********************************************************************************************************************/
/*                                       Internal API Interface                                                      */
/*********************************************************************************************************************/
/**
 * FCH_INIT_PREPCIE_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_XHCI) (
  FCH_USB *FchUsbData
  );

/**
 * FCH_INIT_POSTPCIE_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_XHCI) (
  FCH_USB *FchUsbData
  );

/**
 * FCH_INIT_PREOS_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module before OS booting
 *
 */
typedef void (*FCH_INIT_PREOS_XHCI) (
  FCH_USB *FchUsbData
  );

/**
 * FCH_GET_SMC_MSG_ID_USBINIT
 *
 * @brief   Internal API type definition.
 *
 * @details Gey SMC Message ID for UsbInit Message
 *
 */
typedef uint32_t (*FCH_GET_SMC_MSG_ID_USBINIT) (
  void
  );

/**
 * FCH_GET_SMC_MSG_ID_USBSXENTRY
 *
 * @brief   Internal API type definition.
 *
 * @details Gey SMC Message ID for UsbSxEntry Message
 *
 */
typedef uint32_t (*FCH_GET_SMC_MSG_ID_USBSXENTRY) (
  void
  );

/**
 * FCH_GET_SMC_MSG_ID_USBS3EXIT
 *
 * @brief   Internal API type definition.
 *
 * @details Gey SMC Message ID for UsbS3Exit Message
 *
 */
typedef uint32_t (*FCH_GET_SMC_MSG_ID_USBS3EXIT) (
  void
  );

/**
 * FCH_GET_SMC_MSG_ID_USBCONFIGUPDATE
 *
 * @brief   Internal API type definition.
 *
 * @details Gey SMC Message ID for UsbCongigUpdate Message
 *
 */
typedef uint32_t (*FCH_GET_SMC_MSG_ID_USBCONFIGUPDATE) (
  void
  );

typedef struct {
  uint8_t   Version;
} FCH_XHCI_INT_API_HEADER;
/**
 * Internal IP APIs
 *
 * This structure contains an API for internal IP specific functions that need
 * to run from IP common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  FCH_XHCI_INT_API_HEADER            Header;
  FCH_INIT_PREPCIE_XHCI              FchInitPrePcieXhci;
  FCH_INIT_POSTPCIE_XHCI             FchInitPostPcieXhci;
  FCH_INIT_PREOS_XHCI                FchInitPreOsXhci;
  FCH_GET_SMC_MSG_ID_USBINIT         FchGetSmcMsgIdUsbInit;
  FCH_GET_SMC_MSG_ID_USBSXENTRY      FchGetSmcMsgIdUsbSxEntry;
  FCH_GET_SMC_MSG_ID_USBS3EXIT       FchGetSmcMsgIdUsbS3Exit;
  FCH_GET_SMC_MSG_ID_USBCONFIGUPDATE FchGetSmcMsgIdUsbConfigUpdate;
} FCH_XHCI_XFER_TABLE;

/**********************************************************************************************************************
 * @brief Module data structure
 *
 */

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define FCHUSB_MAJOR_REV 0
#define FCHUSB_MINOR_REV 1
#define FCHUSB_INSTANCE  0

