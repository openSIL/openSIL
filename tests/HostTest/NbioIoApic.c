/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file NbioIoApic.c
 * @brief Native IOAPIC programming tests with a PCI index/data fixture.
 *
 * The real NBIO and SMN access sources are linked by run.py. Only PCI reads
 * and writes are replaced, preserving production segment and bus encoding.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <Pci.h>
#include <Nbio/Brh/NbioCmn2RevBrh.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/include/IoapicReg.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))
#define SMN_INDEX_OFFSET  0xb8
#define SMN_DATA_OFFSET   0xbc
#define SENTINEL          0x5a5a5a5aU

typedef struct {
  uint32_t Segment;
  uint32_t Bus;
  uint32_t Address;
  uint32_t Value;
} SMN_REGISTER;

typedef struct {
  char     Operation;
  uint32_t Address;
  uint32_t Value;
} PCI_ACCESS;

typedef struct {
  uint8_t  RbIndex;
  uint32_t BarLow;
  uint32_t BarHigh;
  uint32_t Id;
} IOAPIC_REGISTERS;

/* Both register banks, each with and without an RB offset applied by NBIO_SPACE. */
static const IOAPIC_REGISTERS mIoApics[] = {
  {0, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS,
   SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS},
  {3, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS,
   SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS},
  {4, SIL_RSVD_ADDR_1D4102F0, SIL_RSVD_ADDR_1D4102F4, SIL_RESERVED_0599},
  {7, SIL_RSVD_ADDR_1D4102F0, SIL_RSVD_ADDR_1D4102F4, SIL_RESERVED_0599},
};

static SMN_REGISTER mRegisters[16];
static size_t       mRegisterCount;
static uint32_t     mIndices[16][256];
static PCI_ACCESS   mPci[16];
static size_t       mPciCount;
static unsigned     mChecks;
static unsigned     mFailures;
static unsigned     mCases;
static char         mCase[96];

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

/** Find or create a register, keeping identical local addresses isolated. */
static SMN_REGISTER *
FindRegister (
  uint32_t Segment,
  uint32_t Bus,
  uint32_t Address
  )
{
  for (size_t Index = 0; Index < mRegisterCount; Index++) {
    SMN_REGISTER *Reg = &mRegisters[Index];
    if (Reg->Segment == Segment && Reg->Bus == Bus && Reg->Address == Address) {
      return Reg;
    }
  }

  Require (mRegisterCount < ARRAY_COUNT (mRegisters), "SMN register capacity");
  SMN_REGISTER *Reg = &mRegisters[mRegisterCount++];
  *Reg = (SMN_REGISTER) {Segment, Bus, Address, 0};
  return Reg;
}

/** Capture every PCI transaction made by the production SMN access code. */
static void
RecordPci (
  char Operation,
  uint32_t Address,
  uint32_t Value
  )
{
  Require (mPciCount < ARRAY_COUNT (mPci), "PCI trace capacity");
  mPci[mPciCount++] = (PCI_ACCESS) {Operation, Address, Value};
}

void
xUSLPciWrite32 (
  uint32_t Address,
  uint32_t Value
  )
{
  PCI_ADDR Pci = {.AddressValue = Address};
  uint32_t Segment = Pci.Address.Segment;
  uint32_t Bus = Pci.Address.Bus;

  Require (Pci.Address.Device == 0 && Pci.Address.Function == 0, "SMN device/function");
  RecordPci ('W', Address, Value);
  if (Pci.Address.Register == SMN_INDEX_OFFSET) {
    mIndices[Segment][Bus] = Value;
  } else {
    Require (Pci.Address.Register == SMN_DATA_OFFSET, "SMN data write offset");
    FindRegister (Segment, Bus, mIndices[Segment][Bus])->Value = Value;
  }
}

uint32_t
xUSLPciRead32 (
  uint32_t Address
  )
{
  PCI_ADDR Pci = {.AddressValue = Address};
  uint32_t Segment = Pci.Address.Segment;
  uint32_t Bus = Pci.Address.Bus;

  Require (Pci.Address.Device == 0 && Pci.Address.Function == 0, "SMN device/function");
  Require (Pci.Address.Register == SMN_DATA_OFFSET, "SMN data read offset");
  uint32_t Value = FindRegister (Segment, Bus, mIndices[Segment][Bus])->Value;
  RecordPci ('R', Address, Value);
  return Value;
}

/** Seed all three registers at a target or at a decoy segment/bus. */
static void
SeedRegisters (
  uint32_t Segment,
  uint32_t Bus,
  const IOAPIC_REGISTERS *Regs
  )
{
  FindRegister (Segment, Bus, Regs->BarHigh)->Value = SENTINEL;
  FindRegister (Segment, Bus, Regs->BarLow)->Value = SENTINEL;
  FindRegister (Segment, Bus, Regs->Id)->Value = SENTINEL;
}

/**
 * Check actual BAR/ID state, isolation, and all index/data transactions.
 * The same local bus exists in another segment to expose segment-zero aliases.
 */
static void
TestIoApic (
  uint8_t Socket,
  uint8_t Segment,
  uint8_t Bus,
  const IOAPIC_REGISTERS *Bank,
  uint64_t Base
  )
{
  GNB_HANDLE Handle = {.RBIndex = Bank->RbIndex};
  const IOAPIC_REGISTERS Instance = {
    Bank->RbIndex,
    NBIO_SPACE ((&Handle), Bank->BarLow),
    NBIO_SPACE ((&Handle), Bank->BarHigh),
    NBIO_SPACE ((&Handle), Bank->Id),
  };
  const IOAPIC_REGISTERS *Regs = &Instance;
  uint32_t OtherSegment = Segment == 0 ? 1 : 0;
  uint32_t OtherBus = Bus ^ 0x80;
  uint32_t Low = (uint32_t) Base;
  uint32_t High = (uint32_t) (Base >> 32);
  uint32_t Id = (uint32_t) (0xf0 + Socket * 8 + Regs->RbIndex) << 24;
  uint32_t IndexPci = ((uint32_t) Segment << 28) | ((uint32_t) Bus << 20) | SMN_INDEX_OFFSET;
  uint32_t DataPci = ((uint32_t) Segment << 28) | ((uint32_t) Bus << 20) | SMN_DATA_OFFSET;
  const PCI_ACCESS Expected[] = {
    {'W', IndexPci, Regs->BarHigh},
    {'W', DataPci, High},
    {'W', IndexPci, Regs->BarLow},
    {'W', DataPci, Low},
    {'W', IndexPci, Regs->BarLow},
    {'R', DataPci, Low},
    {'W', IndexPci, Regs->BarLow},
    {'W', DataPci, Low | 1},
    {'W', IndexPci, Regs->Id},
    {'W', DataPci, Id},
  };

  snprintf (mCase, sizeof (mCase), "socket %u segment %u bus %02x RB %u BAR %08x:%08x",
            Socket, Segment, Bus, Regs->RbIndex, High, Low);
  mCases++;
  mRegisterCount = 0;
  mPciCount = 0;
  memset (mRegisters, 0, sizeof (mRegisters));
  memset (mIndices, 0, sizeof (mIndices));
  memset (mPci, 0, sizeof (mPci));
  SeedRegisters (Segment, Bus, Regs);
  SeedRegisters (OtherSegment, Bus, Regs);
  SeedRegisters (Segment, OtherBus, Regs);

  Handle.SocketId = Socket;
  Handle.Address.Address.Segment = Segment;
  Handle.Address.Address.Bus = Bus;
  NbioIoApicMmioAddressBrh (&Handle, Base);
  NbioIoApicPreDefIdBrh (&Handle, 0xf0);

  Check (FindRegister (Segment, Bus, Regs->BarHigh)->Value == High, "BAR high at target");
  Check (FindRegister (Segment, Bus, Regs->BarLow)->Value == (Low | 1), "BAR low and enable at target");
  Check (FindRegister (Segment, Bus, Regs->Id)->Value == Id, "predefined ID at target");
  Check (FindRegister (OtherSegment, Bus, Regs->BarHigh)->Value == SENTINEL, "other segment BAR high unchanged");
  Check (FindRegister (OtherSegment, Bus, Regs->BarLow)->Value == SENTINEL, "other segment BAR low unchanged");
  Check (FindRegister (OtherSegment, Bus, Regs->Id)->Value == SENTINEL, "other segment ID unchanged");
  Check (FindRegister (Segment, OtherBus, Regs->BarHigh)->Value == SENTINEL, "other bus BAR high unchanged");
  Check (FindRegister (Segment, OtherBus, Regs->BarLow)->Value == SENTINEL, "other bus BAR low unchanged");
  Check (FindRegister (Segment, OtherBus, Regs->Id)->Value == SENTINEL, "other bus ID unchanged");

  bool TraceMatches = mPciCount == ARRAY_COUNT (Expected);
  for (size_t Index = 0; TraceMatches && Index < mPciCount; Index++) {
    TraceMatches = mPci[Index].Operation == Expected[Index].Operation &&
                   mPci[Index].Address == Expected[Index].Address &&
                   mPci[Index].Value == Expected[Index].Value;
  }
  Check (TraceMatches, "PCI transaction targets, order and values");
}

int
main (
  void
  )
{
  const struct {
    uint8_t Socket;
    uint8_t Segment;
    uint8_t Bus;
  } Targets[] = {
    {0, 0, 0x00},
    {1, 0, 0x60},
    {1, 1, 0x60},
    {1, 3, 0xc0},
  };
  const uint64_t Bases[] = {0x00000000fec10000ULL, 0x00000012fe810000ULL};

  for (size_t Target = 0; Target < ARRAY_COUNT (Targets); Target++) {
    for (size_t Rb = 0; Rb < ARRAY_COUNT (mIoApics); Rb++) {
      for (size_t Base = 0; Base < ARRAY_COUNT (Bases); Base++) {
        TestIoApic (Targets[Target].Socket, Targets[Target].Segment, Targets[Target].Bus,
                    &mIoApics[Rb], Bases[Base]);
      }
    }
  }

  printf ("NBIO IOAPIC: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
