/**
 * @file
 * @brief FCH Initialization data blocks
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#pragma once
#include <FCH/Common/FchCommonCfg.h>

typedef struct _FCH_INIT_PROTOCOL FCH_INIT_PROTOCOL;
/// FCH USB Over Current Data Block
typedef struct {
  uint32_t ChipType;         // Bitfield: Applied chip types
  uint8_t  Usb1OcPinMap[5];  // USB1 over current pin mapping
  uint8_t  Usb2OcPinMap[5];  // USB2 over current pin mapping
  uint8_t  Usb3OcPinMap[4];  // USB3 over current pin mapping
  uint8_t  Usb4OcPinMap[2];  // USB4 over current pin mapping
  uint8_t  Xhci0OcPinMap[2]; // XHCI0 over current pin mapping
  uint8_t  Xhci1OcPinMap[2]; // XHCI1 over current pin mapping
} USB_OC;

FCHCLASS_INPUT_BLK*
GetFchDataBlock (
);

//
// Protocol prototypes
//
/// FCH INIT definition for FCHCLASS_INPUT_BLK
typedef
FCHCLASS_INPUT_BLK*
(*FP_FCH_DATA_BLOCK) (
  );

/// FCH INIT definition for PT_DATA_BLOCK
typedef
FCH_PT*
(*FP_FCH_PT_DATA_BLOCK) (
  );

typedef
void
(*FP_FCH_USB_OC) (
  USB_OC      *UsbOc
  );

//
// Rev2 for USB OC mapping
//
typedef int (*FP_FCH_USB_OC2) (
  const FCH_INIT_PROTOCOL     *This,              ///< FCH INIT Protocol
  uint8_t                     Socket,             ///< CPU/Socket number in system
  uint8_t                     Port,               ///< Bitmap (Bit0 - Port0, Bit1 - Port1, etc.) to disable USB3 ports
  uint8_t                     OCPin               ///< Bitmap (Bit0 - Port0, Bit1 - Port1, etc.) to disable USB2 ports
);

//
// USB port disable function
//
typedef SIL_STATUS (*FP_FCH_USB_PORT_DISABLE2) (
  uint8_t                 Socket,                   ///< CPU/Socket number in system
  uint32_t                USB3DisableMap,           ///< Bitmap (Bit0 - Port0, Bit1 - Port1, etc.) to disable USB3 ports
  uint32_t                USB2DisableMap            ///< Bitmap (Bit0 - Port0, Bit1 - Port1, etc.) to disable USB2 ports
);

/// FCH INIT Protocol
typedef struct _FCH_INIT_PROTOCOL {
  FP_FCH_DATA_BLOCK         FpGetFchPolicy;           ///< Fch Config Data Block
  FP_FCH_USB_OC             FpUsbOverCurrent;         ///< Service: USB Over Current
  FP_FCH_USB_OC2            FpUsbOverCurrent2;        ///< Service: USB Over Current2
  FP_FCH_USB_PORT_DISABLE2  FpUsbPortDisable2;        ///< Service: USB Port Disable2
} FCH_INIT_PROTOCOL;
