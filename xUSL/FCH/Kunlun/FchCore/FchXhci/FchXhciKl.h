/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciKl.h
 * @brief Kunlun FCH XHCI controller functions header
 */

#pragma once

#pragma pack (push, 1)

typedef struct {
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
  uint8_t field4;
  uint8_t field5;
  uint8_t field6;
  uint8_t field7;
  uint8_t field8;
} SIL_RESERVED_STRUCT_0017;

typedef struct {
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
  uint8_t field4;
  uint8_t field5;
  uint8_t field6;
  uint8_t field7;
  uint8_t field8;
  uint8_t field9;
  uint8_t field10;
  uint8_t field11;
} SIL_RESERVED_STRUCT_0018;

typedef struct {
  uint8_t           field0;
  uint8_t           field1;
  uint8_t           field2;
  uint8_t           field3;
  uint8_t           field4;
  SIL_RESERVED_STRUCT_0017  field5[4];
  uint8_t           field6;
  uint8_t           field7;
  SIL_RESERVED_STRUCT_0017  field8[4];
  uint8_t           field9;
  SIL_RESERVED_STRUCT_0018  field10[4];
  uint8_t           field11;
  SIL_RESERVED_STRUCT_0018  field12[4];
} SIL_RESERVED_STRUCT_0019;
#pragma pack (pop)

/**
 * InitializeApiFchUsbkl
 *
 * @brief   Initialize internal APIs for Xhci
 *
 * @returns SIL_STATUS
 *
 */
SIL_STATUS
InitializeApiFchUsbKl (void);

/**
 * InitializeFchUsbKlTp1
 *
 * @brief Config Usb controller during timepoint 1 (pre-pcie)
 *
 * @returns SIL_STATUS
 *
 */
SIL_STATUS
InitializeFchUsbKlTp1 (void);

/**
 * InitializeFchUsbKlTp2
 *
 * @brief Config Usb controller during timepoint 2
 *
 * @returns SIL_STATUS
 *
 */
SIL_STATUS
InitializeFchUsbKlTp2 (void);

/**
 * InitializeFchUsbKlTp3
 *
 * @brief Config Usb controller during timepoint 3
 *
 * @returns SIL_STATUS
 *
 */
SIL_STATUS
InitializeFchUsbKlTp3 (void);


/**
 * FchUsbSetInputBlkKl
 *
 * @brief Establish FCH Usb input defaults for Kunlun
 *
 * @returns SIL_STATUS
 *
 */
SIL_STATUS FchUsbSetInputBlkKl (void);

#define FCHUSB_INPBLK_SIZE_KL sizeof (FCHUSB_INPUT_BLK)
