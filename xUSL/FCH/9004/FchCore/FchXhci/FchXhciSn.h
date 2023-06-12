/**
 * @file  FchXhciSn.h
 * @brief 9004 FCH XHCI controller functions header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define BIOSSMC_MSG_USBINIT_SN                             0xA
#define BIOSSMC_MSG_USBCONFIGUPDATE_SN                     0x13
#define BIOSSMC_MSG_USBSXENTRY_SN                          0x14
#define BIOSSMC_MSG_USBS3EXIT_SN                           0x15

/* 9004 FCH has 2 XHCI controllers and each controller has 2 USB3 ports + 2 USB2 ports. */
#define FCH_SN_XHCI_CONTROLLER_NUM                         0x2
#define FCH_SN_XHCI_USB3_PORT_NUM                          0x2
#define FCH_SN_XHCI_USB2_PORT_NUM                          0x2

#define FCH_SN_USB0_SMN_BASE                               0x16C00000ul        //0x16C00000ul
#define FCH_SN_USB1_SMN_BASE                               0x16E00000ul        //0x16E00000ul
#define FCH_SN_USB_CNTL_STEP                               0x00200000ul

//IOHC::NB_PCI_CTRL
#define FCH_SN_NBIO0_IOHC_NB_PCI_CTRL                      0x13B0004Cul
#define FCH_SN_NBIO1_IOHC_NB_PCI_CTRL                      0x13D0004Cul

/**********************************************************************************************************************
 * Function prototypes
 *
 */
void
FchInitPrePcieXhciSn (
  FCH_USB *FchUsbData
  );

void
FchInitPostPcieXhciSn (
  FCH_USB *FchUsbData
  );

void
FchInitPreOsXhciSn (
  FCH_USB *FchUsbData
  );

uint32_t FchGetSmcMsgIdUsbInitSn (void);
uint32_t FchGetSmcMsgIdUsbSxEntrySn (void);
uint32_t FchGetSmcMsgIdUsbS3ExitSn (void);
uint32_t FchGetSmcMsgIdUsbConfigUpdateSn (void);

SIL_STATUS
InitializeApiFchXhciSn (void);

/*******************************************************************************
 * Module data structure
 *
 */


/*******************************************************************************
 * Declare macros here
 *
 */
