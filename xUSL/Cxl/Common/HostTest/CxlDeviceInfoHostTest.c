/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CxlDeviceInfoHostTest.c
 * @brief Native tests linked with the production CXL device information code.
 *
 * @details PCI registers and DVSEC lookup are fixtures. The capability decoder,
 *          register addressing and endpoint-info population come from the
 *          actual CxlDeviceInfo.c translation unit. See tests/HostTest/README.md
 *          for the native build/run command.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SilCommon.h>
#include <Pci.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/Common/CxlDeviceInfo.h>

static unsigned  mChecks;
static unsigned  mFailures;
static unsigned  mPciReads;
static unsigned  mDvsecLookups;

#define TEST_ENDPOINT  0x28429000u  /* segment 2, bus 0x84, device 5, function 1 */

static uint16_t mDvsecPtr;
static struct {
  uint32_t Address;
  ACCESS_WIDTH Width;
  uint32_t Value;
} mRegisters[8];
static size_t mRegisterCount;

static void
Check (
  bool        Condition,
  const char  *What
  )
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("  FAIL %s\n", What);
  }
}

uint16_t
CxlGetDvsec (
  uint32_t Address,
  uint16_t VendorId,
  uint16_t VendorId2,
  uint16_t DvsecId
  )
{
  mDvsecLookups++;
  Check (Address == TEST_ENDPOINT, "DVSEC lookup preserves the PCI segment and BDF");
  Check ((VendorId == 0x8086) && (VendorId2 == 0x1e98) && (DvsecId == 0),
    "lookup requests the CXL device DVSEC with both supported vendor IDs");
  return mDvsecPtr;
}

void
xUSLPciRead (
  uint32_t Address,
  ACCESS_WIDTH Width,
  void *Value
  )
{
  mPciReads++;
  for (size_t Index = 0; Index < mRegisterCount; Index++) {
    if (mRegisters[Index].Address == Address) {
      Check (Width == mRegisters[Index].Width, "PCI register is read at its defined width");
      if (Width == AccessWidth16) {
        uint16_t Word = mRegisters[Index].Value;
        memcpy (Value, &Word, sizeof (Word));
      } else if (Width == AccessWidth32) {
        memcpy (Value, &mRegisters[Index].Value, sizeof (uint32_t));
      } else {
        Check (false, "unsupported PCI access width");
      }
      return;
    }
  }
  printf ("  Unexpected PCI read: 0x%08x, width %u\n", Address, Width);
  Check (false, "PCI read must address a register in the fixture");
}

static void
ResetPci (
  uint16_t DvsecPtr
  )
{
  mDvsecPtr = DvsecPtr;
  mRegisterCount = 0;
  mPciReads = 0;
  mDvsecLookups = 0;
}

static void
AddRegister (
  uint16_t Offset,
  ACCESS_WIDTH Width,
  uint32_t Value
  )
{
  if (mRegisterCount == sizeof (mRegisters) / sizeof (mRegisters[0])) {
    Check (false, "PCI fixture capacity");
    return;
  }
  mRegisters[mRegisterCount].Address = TEST_ENDPOINT | Offset;
  mRegisters[mRegisterCount].Width = Width;
  mRegisters[mRegisterCount].Value = Value;
  mRegisterCount++;
}

struct DecodeCase {
  const char  *Name;
  uint16_t    Capability;
  uint8_t     ExpectType;
  uint8_t     ExpectHdm;
  uint8_t     ExpectHwInit;
};

/*
 * Capability words taken apart by hand from the CXL DVSEC layout: bit 0 is
 * CXL.cache, bit 2 is CXL.mem, bit 3 is HwInit mode and bits 5:4 are the HDM
 * decoder count.
 */
static const struct DecodeCase  mDecodeCases[] = {
  { "no capabilities",              0x0000, CXL_DEVICE_TYPE_NONE,  0, 0 },
  { "type 1, cache only",           0x0001, CXL_DEVICE_TYPE_CACHE, 0, 0 },
  { "type 2, cache and mem",        0x0005, CXL_DEVICE_TYPE_BOTH,  0, 0 },
  { "type 3, mem only",             0x0004, CXL_DEVICE_TYPE_MEM,   0, 0 },
  { "type 3, one hdm range",        0x0014, CXL_DEVICE_TYPE_MEM,   1, 0 },
  { "type 3, two hdm ranges",       0x0024, CXL_DEVICE_TYPE_MEM,   2, 0 },
  { "type 3, hwinit set",           0x000C, CXL_DEVICE_TYPE_MEM,   0, 1 },
  { "type 3, two ranges + hwinit",  0x002C, CXL_DEVICE_TYPE_MEM,   2, 1 },
  { "hdm count of three clamps",    0x0034, CXL_DEVICE_TYPE_MEM,   2, 0 },
  { "unrelated bits ignored",       0xFFC0 | 0x0004, CXL_DEVICE_TYPE_MEM, 0, 0 },
};

static void
TestDvsecDecode (
  void
  )
{
  uint8_t  Type;
  uint8_t  Hdm;
  uint8_t  HwInit;
  size_t   Index;

  printf ("DVSEC capability decode\n");
  for (Index = 0; Index < sizeof (mDecodeCases) / sizeof (mDecodeCases[0]); Index++) {
    const struct DecodeCase  *Case = &mDecodeCases[Index];

    Type = 0xFF;
    Hdm = 0xFF;
    HwInit = 0xFF;
    CxlDecodeDvsecCapability (Case->Capability, &Type, &Hdm, &HwInit);

    if ((Type != Case->ExpectType) || (Hdm != Case->ExpectHdm) || (HwInit != Case->ExpectHwInit)) {
      printf ("  FAIL %s: cap 0x%04x gave type %u hdm %u hwinit %u,"
              " expected type %u hdm %u hwinit %u\n",
              Case->Name, Case->Capability, Type, Hdm, HwInit,
              Case->ExpectType, Case->ExpectHdm, Case->ExpectHwInit);
      mFailures++;
    }
    mChecks++;
  }
}

static void
TestNullOutputsTolerated (
  void
  )
{
  uint8_t  Hdm = 0xFF;

  printf ("optional outputs\n");
  /* A caller that only wants one field passes NULL for the rest */
  CxlDecodeDvsecCapability (0x0024, NULL, &Hdm, NULL);
  Check (Hdm == 2, "hdm count still decoded when type and hwinit are NULL");
  CxlDecodeDvsecCapability (0x0024, NULL, NULL, NULL);
  Check (true, "all outputs NULL does not fault");
}

static void
TestRangeReads (
  void
  )
{
  uint32_t Value = 0;
  SIL_STATUS Status;

  printf ("HDM range register reads\n");
  ResetPci (0x200);
  AddRegister (0x21c, AccessWidth32, 0x10000003);
  AddRegister (0x22c, AccessWidth32, 0x8000e007);
  Status = CxlGetRangeSizeLow (TEST_ENDPOINT, mDvsecPtr, 0, &Value);
  Check ((Status == SilPass) && (Value == 0x10000003), "first HDM size register is returned unchanged");
  Status = CxlGetRangeSizeLow (TEST_ENDPOINT, mDvsecPtr, 1, &Value);
  Check ((Status == SilPass) && (Value == 0x8000e007), "second HDM size register preserves size and status bits");
  Check (mPciReads == 2, "one PCI read per HDM request");

  Value = 0x12345678;
  Status = CxlGetRangeSizeLow (TEST_ENDPOINT, mDvsecPtr, 2, &Value);
  Check ((Status == SilInvalidParameter) && (Value == 0x12345678), "third HDM range is rejected without output");
  Status = CxlGetRangeSizeLow (TEST_ENDPOINT, 0, 0, &Value);
  Check ((Status == SilInvalidParameter) && (Value == 0x12345678), "absent DVSEC is rejected without output");
  Check (CxlGetRangeSizeLow (TEST_ENDPOINT, mDvsecPtr, 0, NULL) == SilInvalidParameter,
    "NULL range output is rejected");
  Check (mPciReads == 2, "invalid range requests do not read PCI registers");
}

static void
TestEndpointInfo (
  void
  )
{
  CXL_ENDPOINT_INFO Info;
  const uint8_t Empty[sizeof (Info)] = { 0 };
  SIL_STATUS Status;

  printf ("CXL endpoint information\n");
  ResetPci (0);
  Check (CxlGetDeviceInfo (TEST_ENDPOINT, NULL) == SilInvalidParameter, "NULL endpoint output is rejected");
  Check ((mPciReads == 0) && (mDvsecLookups == 0), "NULL endpoint output does not touch PCI");
  memset (&Info, 0xa5, sizeof (Info));
  Status = CxlGetDeviceInfo (TEST_ENDPOINT, &Info);
  Check ((Status == SilPass) && (memcmp (&Info, &Empty, sizeof (Info)) == 0),
    "device without a CXL DVSEC returns an empty description");
  Check ((mPciReads == 0) && (mDvsecLookups == 1), "absent DVSEC avoids capability and HDM reads");

  ResetPci (0x180);
  AddRegister (0x18a, AccessWidth16, 0x003c); /* mem, hwinit, reserved count of 3 */
  AddRegister (0x198, AccessWidth32, 0x00000002);
  AddRegister (0x19c, AccessWidth32, 0x80000003);
  AddRegister (0x1a8, AccessWidth32, 0x00000001);
  AddRegister (0x1ac, AccessWidth32, 0x10000007);
  AddRegister (0x008, AccessWidth32, 0x05020001);
  Status = CxlGetDeviceInfo (TEST_ENDPOINT, &Info);
  Check ((Status == SilPass) && (Info.DvsecPtr == 0x180) &&
    (Info.CxlType == CXL_DEVICE_TYPE_MEM) && (Info.HdmCount == 2) && (Info.MemHwInitMode == 1),
    "endpoint decode clamps the range count and preserves hardware-init mode");
  Check ((Info.RangeSizeHi[0] == 2) && (Info.RangeSizeLo[0] == 0x80000003) &&
    (Info.RangeSizeHi[1] == 1) && (Info.RangeSizeLo[1] == 0x10000007),
    "endpoint returns both complete HDM ranges");
  Check (Info.BaseClassCode == 5, "endpoint extracts the PCI base class");
  Check ((mPciReads == 6) && (mDvsecLookups == 1), "reserved count does not read a third HDM range");
}

int
main (
  void
  )
{
  printf ("CxlDeviceInfo host test\n\n");

  TestDvsecDecode ();
  TestNullOutputsTolerated ();
  TestRangeReads ();
  TestEndpointInfo ();

  printf ("\n%u checks, %u failures\n", mChecks, mFailures);
  return (mFailures == 0) ? 0 : 1;
}
