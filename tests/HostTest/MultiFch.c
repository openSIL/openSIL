/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MultiFch.c
 * @brief Native regression tests for secondary FCH callback dispatch.
 *
 * Link the production MultiFch.c dispatcher and Kunlun transfer table. Only
 * table/data lookup and the platform callbacks are supplied by this fixture;
 * no PCI, MMIO, silicon discovery or initialization executes on the host.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <FCH/FchMultiFch-api.h>
#include <FCH/Common/MultiFch/MultiFch.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include <FCH/Kunlun/MultiFch/MultiFchCmn2Kl.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))

extern SIL_RESERVED_STRUCT_0026 mMultiFchXferKl;

typedef struct {
  char     Step;
  uint32_t Socket;
  uint32_t Die;
  uint32_t Rb;
  uint64_t Mmio;
  uint32_t Bus;
  uint8_t  HasFch;
} INIT_CALL;

static SIL_RESERVED_STRUCT_0026 mTable;
static FCHMULTIFCH_OUTPUT_BLK mOutput;
static INIT_CALL mCalls[16];
static size_t mCallCount;
static unsigned mRbLookups;
static bool mTablePresent;
static bool mOutputPresent;
static char mFailStep;
static SIL_STATUS mCallbackError;
static const char *mCase;
static unsigned mChecks;
static unsigned mFailures;

static void
Check (bool Condition, const char *Name)
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL: %s: %s\n", mCase, Name);
  }
}

static void
Require (bool Condition, const char *Name)
{
  if (!Condition) {
    fprintf (stderr, "Fixture failure: %s: %s\n", mCase, Name);
    abort ();
  }
}

SIL_STATUS
SilGetCommon2RevXferTable (SIL_DATA_BLOCK_ID IpId, void **XferTable)
{
  Require (IpId == SilId_MultiFchClass, "lookup requests MultiFch transfer table");
  *XferTable = mTablePresent ? &mTable : NULL;
  return mTablePresent ? SilPass : SilNotFound;
}

void *
xUslFindStructure (SIL_DATA_BLOCK_ID IpId, uint16_t InstanceNum)
{
  Require ((IpId == SilId_MultiFchClass) && (InstanceNum == SIL_RESERVED_0358),
    "lookup requests MultiFch output instance");
  return mOutputPresent ? &mOutput : NULL;
}

SIL_STATUS
FchMultiFchGetFchRbIndexOnDieKl (uint32_t *RbIndex)
{
  mRbLookups++;
  *RbIndex = 3;
  return SilPass;
}

static SIL_STATUS
Record (char Step, uint32_t Socket, uint32_t Die, uint32_t Rb,
  uint64_t Mmio, uint32_t Bus, uint8_t HasFch)
{
  Require (mCallCount < ARRAY_COUNT (mCalls), "callback trace capacity");
  mCalls[mCallCount++] = (INIT_CALL) {Step, Socket, Die, Rb, Mmio, Bus, HasFch};
  return Step == mFailStep ? mCallbackError : SilPass;
}

SIL_STATUS
FchMultiFchInitAbKl (uint32_t SocketNum, uint32_t DieNum, uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr, uint32_t IohcBusNumber, uint8_t HasFchModule)
{
  return Record ('A', SocketNum, DieNum, RbNum, AcpiMmioBaseAddr, IohcBusNumber, HasFchModule);
}

SIL_STATUS
FchMultiFchInitSdKl (uint32_t SocketNum, uint32_t DieNum, uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr, uint32_t IohcBusNumber, uint8_t HasFchModule)
{
  return Record ('D', SocketNum, DieNum, RbNum, AcpiMmioBaseAddr, IohcBusNumber, HasFchModule);
}

SIL_STATUS
FchMultiFchInitUsbKl (uint32_t SocketNum, uint32_t DieNum, uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr, uint32_t IohcBusNumber, uint8_t HasFchModule)
{
  return Record ('U', SocketNum, DieNum, RbNum, AcpiMmioBaseAddr, IohcBusNumber, HasFchModule);
}

static SIL_STATUS
SataInit (uint32_t SocketNum, uint32_t DieNum, uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr, uint32_t IohcBusNumber, uint8_t HasFchModule)
{
  return Record ('S', SocketNum, DieNum, RbNum, AcpiMmioBaseAddr, IohcBusNumber, HasFchModule);
}

static void
ResetFixture (const char *Name, uint32_t Sockets, uint32_t Dies)
{
  mCase = Name;
  printf ("MultiFch: %s\n", Name);
  /* Preserve the real Kunlun callback population, including its absent SATA slot. */
  mTable = mMultiFchXferKl;
  memset (&mOutput, 0, sizeof (mOutput));
  mOutput.NumberOfSockets = Sockets;
  mOutput.NumberOfDiePerSocket = Dies;
  mOutput.NumberOfRbPerDie = 4;
  mOutput.ArraySize = Sockets * Dies;
  mOutput.HasFchModule[0] = 1;
  mOutput.FchAcpiMmioBase[0] = UINT64_C (0xfed00000);

  /* Distinct per-die metadata exposes wrong indexing and truncated addresses. */
  mOutput.HasFchModule[1] = 1;
  mOutput.FchIohcBusNumber[1] = 0x40;
  mOutput.FchAcpiMmioBase[1] = UINT64_C (0x1fd300000);
  mOutput.HasFchModule[SIL_RESERVED_0356] = 0;
  mOutput.FchIohcBusNumber[SIL_RESERVED_0356] = 0x21;
  mOutput.FchAcpiMmioBase[SIL_RESERVED_0356] = UINT64_C (0x1fd200000);
  mOutput.HasFchModule[SIL_RESERVED_0356 + 1] = 1;
  mOutput.FchIohcBusNumber[SIL_RESERVED_0356 + 1] = 0x61;
  mOutput.FchAcpiMmioBase[SIL_RESERVED_0356 + 1] = UINT64_C (0x1fd400000);

  memset (mCalls, 0, sizeof (mCalls));
  mCallCount = 0;
  mRbLookups = 0;
  mTablePresent = true;
  mOutputPresent = true;
  mFailStep = 0;
  mCallbackError = SilDeviceError;
}

static SIL_STATUS
Dispatch (void)
{
  FCHMULTIFCH_OUTPUT_BLK Before;
  SIL_STATUS Status;

  memcpy (&Before, &mOutput, sizeof (Before));
  Status = FchMultiFchDispatchSecondaryInits ();
  Check (memcmp (&Before, &mOutput, sizeof (Before)) == 0, "dispatch preserves supplied topology metadata");
  return Status;
}

static void
CheckOrder (const char *Order)
{
  size_t Count = strlen (Order);

  Check (mCallCount == Count, "callback count");
  for (size_t Index = 0; Index < Count && Index < mCallCount; Index++) {
    Check (mCalls[Index].Step == Order[Index], "AB/SD/USB/SATA callback order");
  }
}

static void
CheckContext (size_t First, size_t Count, uint32_t Socket, uint32_t Die,
  uint64_t Mmio, uint32_t Bus, uint8_t HasFch)
{
  Check (First + Count <= mCallCount, "expected callbacks exist for this secondary die");
  for (size_t Index = First; Index < First + Count && Index < mCallCount; Index++) {
    const INIT_CALL *Call = &mCalls[Index];
    Check ((Call->Socket == Socket) && (Call->Die == Die) && (Call->Rb == 3) &&
      (Call->Mmio == Mmio) && (Call->Bus == Bus) && (Call->HasFch == HasFch),
      "callback receives secondary socket/die/RB/MMIO/bus/presence metadata");
  }
}

static void
TestSingleSocket (void)
{
  ResetFixture ("1P skips secondary initialization", 1, 1);
  mTable.field5 = SataInit;
  Check (Dispatch () == SilPass, "1P succeeds");
  CheckOrder ("");
  Check (mRbLookups == 0, "1P needs no secondary root-bridge lookup");
}

static void
TestMissingSata (void)
{
  ResetFixture ("2P Kunlun table has no secondary SATA callback", 2, 1);
  Check (mTable.field5 == NULL, "actual Kunlun table leaves secondary SATA unimplemented");
  Check (Dispatch () == SilPass, "missing optional SATA callback succeeds");
  CheckOrder ("ADU");
  CheckContext (0, 3, 1, 0, UINT64_C (0x1fd300000), 0x40, 1);
  Check (mRbLookups == 1, "2P resolves the root bridge once");
}

static void
TestPresentSata (void)
{
  for (uint8_t HasFch = 0; HasFch <= 1; HasFch++) {
    ResetFixture (HasFch ? "2P supplied SATA callback, presence set" :
      "2P supplied SATA callback, presence clear", 2, 1);
    mTable.field5 = SataInit;
    mOutput.HasFchModule[1] = HasFch;
    Check (Dispatch () == SilPass, "implemented SATA callback succeeds");
    CheckOrder ("ADUS");
    CheckContext (0, 4, 1, 0, UINT64_C (0x1fd300000), 0x40, HasFch);
  }
}

static void
TestCallbackErrors (void)
{
  ResetFixture ("2P SATA callback error propagates", 2, 1);
  mTable.field5 = SataInit;
  mFailStep = 'S';
  Check (Dispatch () == SilDeviceError, "SATA error is not converted to success");
  CheckOrder ("ADUS");
  CheckContext (0, 4, 1, 0, UINT64_C (0x1fd300000), 0x40, 1);

  ResetFixture ("2P earlier USB callback error stops dispatch", 2, 1);
  mTable.field5 = SataInit;
  mFailStep = 'U';
  mCallbackError = SilUnsupported;
  Check (Dispatch () == SilUnsupported, "earlier callback error remains visible");
  CheckOrder ("ADU");
}

static void
TestMultipleSecondaryDies (void)
{
  ResetFixture ("missing SATA does not skip remaining secondary dies", 2, 2);
  Check (Dispatch () == SilPass, "all secondary dies succeed without SATA");
  CheckOrder ("ADUADUADU");
  CheckContext (0, 3, 0, 1, UINT64_C (0x1fd200000), 0x21, 0);
  CheckContext (3, 3, 1, 0, UINT64_C (0x1fd300000), 0x40, 1);
  CheckContext (6, 3, 1, 1, UINT64_C (0x1fd400000), 0x61, 1);
}

static void
TestMissingPrerequisites (void)
{
  ResetFixture ("missing whole transfer table remains an error", 2, 1);
  mTablePresent = false;
  Check (Dispatch () == SilNotFound, "missing table is distinct from an optional callback");
  CheckOrder ("");

  ResetFixture ("missing output block remains an error", 2, 1);
  mOutputPresent = false;
  Check (Dispatch () == SilNotFound, "missing output block is reported");
  CheckOrder ("");
}

int
main (void)
{
  setvbuf (stdout, NULL, _IOLBF, 0);
  TestSingleSocket ();
  TestMissingSata ();
  TestPresentSata ();
  TestCallbackErrors ();
  TestMultipleSecondaryDies ();
  TestMissingPrerequisites ();
  printf ("MultiFch dispatch: %u checks, %u failures\n", mChecks, mFailures);
  return mFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
