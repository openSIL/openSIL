/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file NbioNonPciBar.c
 * @brief Native non-PCI BAR allocation/programming regression tests.
 *
 * Production NBIO and SMN code runs against mocked PCI index/data registers
 * and a resource allocator. Registers on other segments and buses are isolated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <Pci.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <Nbio/Common/Nbio.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <Nbio/Brh/include/IohcReg.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))
#define SMN_INDEX_OFFSET  0xb8
#define SMN_DATA_OFFSET   0xbc
#define SENTINEL          0xa5a5a5a5U

typedef struct {
  uint32_t Index;
  uint32_t Low;
  uint32_t High;
} SMN_BANK;

typedef struct {
  uint32_t Address;
  uint32_t Value;
  char     Operation;
} PCI_ACCESS;

typedef struct {
  const char *Name;
  bool        Enable;
  bool        Lock;
  uint32_t    InitialLow;
  uint32_t    InitialHigh;
  SIL_STATUS  ApiStatus;
  SIL_STATUS  ReserveStatus;
} BAR_CASE;

static SMN_BANK             mBanks[16][256];
static PCI_ACCESS           mPci[24];
static size_t               mPciCount;
static unsigned             mDataWrites;
static uint32_t             mBarLow;
static uint32_t             mBarHigh;
static uint64_t             mAllocatedBase;
static uint64_t             mRequestedLength;
static uint64_t             mRequestedAlignment;
static FABRIC_TARGET        mRequestedTarget;
static FABRIC_MMIO_ATTRIBUTE mRequestedAttributes;
static SIL_STATUS           mApiStatus;
static SIL_STATUS           mReserveStatus;
static unsigned             mApiCalls;
static unsigned             mReserveCalls;
static unsigned             mChecks;
static unsigned             mFailures;
static unsigned             mCases;
static char                 mCase[96];

static void
Check (
  bool Condition,
  const char *Name
  )
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL: %s: %s\n", mCase, Name);
  }
}

static void
Require (
  bool Condition,
  const char *Name
  )
{
  if (!Condition) {
    fprintf (stderr, "Fixture failure: %s\n", Name);
    abort ();
  }
}

/** Only the two BAR registers are valid in this PCI index/data fixture. */
static uint32_t *
SelectedRegister (
  SMN_BANK *Bank
  )
{
  Require (Bank->Index == mBarLow || Bank->Index == mBarHigh, "selected BAR register");
  return Bank->Index == mBarLow ? &Bank->Low : &Bank->High;
}

static void
RecordPci (
  char Operation,
  uint32_t Address,
  uint32_t Value
  )
{
  Require (mPciCount < ARRAY_COUNT (mPci), "PCI trace capacity");
  mPci[mPciCount++] = (PCI_ACCESS) {Address, Value, Operation};
}

void
xUSLPciWrite32 (
  uint32_t Address,
  uint32_t Value
  )
{
  PCI_ADDR Pci = {.AddressValue = Address};
  SMN_BANK *Bank = &mBanks[Pci.Address.Segment][Pci.Address.Bus];

  Require (Pci.Address.Device == 0 && Pci.Address.Function == 0, "SMN device/function");
  RecordPci ('W', Address, Value);
  if (Pci.Address.Register == SMN_INDEX_OFFSET) {
    Bank->Index = Value;
  } else {
    Require (Pci.Address.Register == SMN_DATA_OFFSET, "SMN data write offset");
    *SelectedRegister (Bank) = Value;
    mDataWrites++;
  }
}

uint32_t
xUSLPciRead32 (
  uint32_t Address
  )
{
  PCI_ADDR Pci = {.AddressValue = Address};
  SMN_BANK *Bank = &mBanks[Pci.Address.Segment][Pci.Address.Bus];

  Require (Pci.Address.Device == 0 && Pci.Address.Function == 0, "SMN device/function");
  Require (Pci.Address.Register == SMN_DATA_OFFSET, "SMN data read offset");
  uint32_t Value = *SelectedRegister (Bank);
  RecordPci ('R', Address, Value);
  return Value;
}

/** Capture the real helper's allocation request and supply a deterministic BAR. */
static SIL_STATUS
ReserveMmio (
  uint64_t *BaseAddress,
  uint64_t *Length,
  uint64_t Alignment,
  FABRIC_TARGET Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  )
{
  mReserveCalls++;
  mRequestedLength = *Length;
  mRequestedAlignment = Alignment;
  mRequestedTarget = Target;
  mRequestedAttributes = *Attributes;
  if (mReserveStatus == SilPass) {
    *BaseAddress = mAllocatedBase;
  }
  return mReserveStatus;
}

SIL_STATUS
SilGetIp2IpApi (
  SIL_DATA_BLOCK_ID IpId,
  void **Api
  )
{
  static RCMGR_IP2IP_API RcMgr = {.FabricReserveMmio = ReserveMmio};

  Require (IpId == SilId_RcManager, "resource manager API lookup");
  mApiCalls++;
  *Api = mApiStatus == SilPass ? &RcMgr : NULL;
  return mApiStatus;
}

/** Exercise both helpers, including the locked path that can hide alias writes. */
static void
TestBar (
  bool Psp,
  uint8_t Socket,
  uint8_t Segment,
  uint8_t Bus,
  const BAR_CASE *Test
  )
{
  GNB_HANDLE Handle = {0};
  uint32_t OtherSegment = Segment == 0 ? 1 : 0;
  uint32_t OtherBus = Bus ^ 0x80;
  uint32_t RootPci = ((uint32_t) Segment << 28) | ((uint32_t) Bus << 20);
  bool Unassigned = Test->InitialLow == 0 && Test->InitialHigh == 0;
  bool Allocates = Unassigned && Test->ApiStatus == SilPass;
  bool Programs = Allocates && Test->ReserveStatus == SilPass;
  bool Above4G = !Psp && Segment == 3;
  uint32_t ExpectedLow = Test->InitialLow;
  uint32_t ExpectedHigh = Test->InitialHigh;

  snprintf (mCase, sizeof (mCase), "%s socket %u segment %u bus %02x: %s",
            Psp ? "PSP" : "SMU", Socket, Segment, Bus, Test->Name);
  mCases++;
  memset (mBanks, 0xa5, sizeof (mBanks));
  mBanks[Segment][Bus].Low = Test->InitialLow;
  mBanks[Segment][Bus].High = Test->InitialHigh;
  memset (mPci, 0, sizeof (mPci));
  mPciCount = 0;
  mDataWrites = 0;
  mApiCalls = 0;
  mReserveCalls = 0;
  mApiStatus = Test->ApiStatus;
  mReserveStatus = Test->ReserveStatus;
  mAllocatedBase = Above4G ? 0x12fed00000ULL : 0xfed00000ULL;
  Handle.SocketId = Socket;
  Handle.RBIndex = Psp ? 0 : 3;
  mBarLow = (uint32_t) NBIO_SPACE ((&Handle), (Psp ? SIL_RSVD_ADDR_13B102E0 : SIL_RSVD_ADDR_13B102E8));
  mBarHigh = (uint32_t) NBIO_SPACE ((&Handle), (Psp ? SIL_RSVD_ADDR_13B102E4 : SIL_RSVD_ADDR_13B102EC));
  Handle.Address.Address.Segment = Segment;
  Handle.Address.Address.Bus = Bus;

  if (Psp) {
    NonPciPspBarInit (&Handle, mBarLow, mBarHigh, 0x100000, Test->Enable, Test->Lock);
  } else {
    NonPciBarInit (&Handle, mBarLow, mBarHigh, 0x100000, Test->Enable, Test->Lock, Above4G);
  }

  if (Programs) {
    ExpectedLow = (uint32_t) mAllocatedBase | (Test->Enable ? 1U : 0U);
    ExpectedLow |= Test->Lock ? (Psp ? 0x100U : 2U) : 0U;
    ExpectedHigh = (uint32_t) (mAllocatedBase >> 32);
  }
  Check (mBanks[Segment][Bus].Low == ExpectedLow, "target BAR low, enable and lock");
  Check (mBanks[Segment][Bus].High == ExpectedHigh, "target BAR high");
  Check (mBanks[OtherSegment][Bus].Low == SENTINEL, "other segment BAR low unchanged");
  Check (mBanks[OtherSegment][Bus].High == SENTINEL, "other segment BAR high unchanged");
  Check (mBanks[Segment][OtherBus].Low == SENTINEL, "other bus BAR low unchanged");
  Check (mBanks[Segment][OtherBus].High == SENTINEL, "other bus BAR high unchanged");
  Check (mApiCalls == (unsigned) Unassigned, "lookup only for an unassigned BAR");
  Check (mReserveCalls == (unsigned) Allocates, "allocation only after successful lookup");
  Check (mDataWrites == (Programs ? 2U + 2U * Test->Enable + 2U * Test->Lock : 0U),
         "no extra BAR writes on disabled, preassigned or failed paths");
  Check (mPciCount == 4U + 2U * mDataWrites, "two initial BAR reads and expected write transactions");

  bool TargetsMatch = true;
  for (size_t Index = 0; Index < mPciCount; Index++) {
    TargetsMatch &= (mPci[Index].Address & 0xfffff000U) == RootPci;
  }
  Check (TargetsMatch, "all PCI index/data transactions target the handle");

  if (Allocates) {
    Check (mRequestedLength == 0x100000 && mRequestedAlignment == ALIGN_1M, "allocation size and alignment");
    Check (mRequestedTarget.SocketNum == Socket && mRequestedTarget.RbNum == Handle.RBIndex,
           "allocation socket and root bridge");
    Check (mRequestedTarget.TgtType == (Psp ? TARGET_RB : TARGET_PCI_BUS), "allocation target type");
    if (!Psp) {
      Check (mRequestedTarget.PciSegNum == Segment && mRequestedTarget.PciBusNum == Bus,
             "allocation PCI segment and bus");
    }
    Check (mRequestedAttributes.MmioType == (Above4G ? NON_PCI_DEVICE_ABOVE_4G : NON_PCI_DEVICE_BELOW_4G) &&
           mRequestedAttributes.ReadEnable && mRequestedAttributes.WriteEnable &&
           !mRequestedAttributes.NonPosted, "allocation attributes");
  }
}

int
main (
  void
  )
{
  const BAR_CASE Cases[] = {
    {"enabled and locked", true, true, 0, 0, SilPass, SilPass},
    {"enabled and unlocked", true, false, 0, 0, SilPass, SilPass},
    {"disabled", false, false, 0, 0, SilPass, SilPass},
    {"disabled and locked", false, true, 0, 0, SilPass, SilPass},
    {"already enabled", true, true, 0xfa000001, 0, SilPass, SilPass},
    {"already assigned, disabled", true, true, 0xfa000000, 0, SilPass, SilPass},
    {"already assigned high word", true, true, 0, 0x12, SilPass, SilPass},
    {"API unavailable", true, true, 0, 0, SilNotFound, SilPass},
    {"allocation failed", true, true, 0, 0, SilPass, SilAborted},
  };
  const struct {
    uint8_t Socket;
    uint8_t Segment;
    uint8_t Bus;
  } Targets[] = {{0, 0, 0}, {1, 0, 0x60}, {1, 1, 0x60}, {1, 3, 0xc0}};

  for (unsigned Helper = 0; Helper < 2; Helper++) {
    for (size_t Target = 0; Target < ARRAY_COUNT (Targets); Target++) {
      for (size_t Test = 0; Test < ARRAY_COUNT (Cases); Test++) {
        TestBar (Helper != 0, Targets[Target].Socket, Targets[Target].Segment,
                 Targets[Target].Bus, &Cases[Test]);
      }
    }
  }

  printf ("NBIO non-PCI BAR: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
