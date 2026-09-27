/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file XsimDispatch.c
 * @brief Exercise the real xSIM timepoint dispatchers with scripted IPs.
 *
 * The production translation unit and real provider types are linked by the
 * host-test runner. Only the active SoC tables and IP callbacks are fixtures.
 * A duplicate callback returns a bounded sentinel instead of hanging a broken
 * dispatcher; one-shot deferred callbacks independently expose status masking.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <xSIM.h>
#include <IpHandler.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))
#define SCRIPT_SIZE 4

/* Initialize the flexible array, as the production SoC table definitions do. */
SOC_IP_TABLE SocIpTblF1AM00Tp1 = {
  .IpList = { [SCRIPT_SIZE] = { .IpID = SilId_ListEnd } }
};
SOC_IP_TABLE SocIpTblF1AM00Tp2 = {
  .IpList = { [SCRIPT_SIZE] = { .IpID = SilId_ListEnd } }
};
SOC_IP_TABLE SocIpTblF1AM00Tp3 = {
  .IpList = { [SCRIPT_SIZE] = { .IpID = SilId_ListEnd } }
};
HOST_DEBUG_SERVICE mHostDebugService;

static SIL_STATUS mScript[SCRIPT_SIZE];
static unsigned mCalls[SCRIPT_SIZE];
static unsigned mTrace[16];
static unsigned mTraceCount;
static bool mOneShot;
static unsigned mCases;
static unsigned mChecks;
static unsigned mFailures;
static const char *mCase;
static unsigned mTimepoint;

static void
Check (
  bool Condition,
  const char *What
  )
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL TP%u %s: %s\n", mTimepoint, mCase, What);
  }
}

static SIL_STATUS
ScriptedIp (
  unsigned Index
  )
{
  if (mTraceCount >= ARRAY_COUNT (mTrace)) {
    fprintf (stderr, "Fixture callback bound exceeded\n");
    exit (99);
  }
  mTrace[mTraceCount++] = Index;
  mCalls[Index]++;
  if (mCalls[Index] > 1) {
    return mOneShot ? SilPass : SilAborted;
  }
  return mScript[Index];
}

static SIL_STATUS Ip0 (void) { return ScriptedIp (0); }
static SIL_STATUS Ip1 (void) { return ScriptedIp (1); }
static SIL_STATUS Ip2 (void) { return ScriptedIp (2); }
static SIL_STATUS Ip3 (void) { return ScriptedIp (3); }

static void
RunCase (
  const char *Name,
  const SIL_STATUS *Statuses,
  unsigned Count,
  unsigned NullMask,
  bool OneShot,
  SIL_STATUS ExpectedStatus,
  unsigned ExpectedVisited
  )
{
  SIL_STATUS (*Callbacks[SCRIPT_SIZE]) (void) = { Ip0, Ip1, Ip2, Ip3 };
  SIL_STATUS (*Timepoints[3]) (void) = {
    InitializeAMDSiTp1, InitializeAMDSiTp2, InitializeAMDSiTp3
  };
  SOC_IP_TABLE *Tables[3] = {
    &SocIpTblF1AM00Tp1, &SocIpTblF1AM00Tp2, &SocIpTblF1AM00Tp3
  };
  unsigned Tp;

  if (Count > SCRIPT_SIZE || ExpectedVisited > Count) {
    abort ();
  }
  for (Tp = 0; Tp < ARRAY_COUNT (Tables); Tp++) {
    unsigned Index;
    unsigned TraceIndex = 0;
    SIL_STATUS Actual;

    mCases++;
    mCase = Name;
    mTimepoint = Tp + 1;
    mOneShot = OneShot;
    mTraceCount = 0;
    memset (mCalls, 0, sizeof (mCalls));
    memset (mTrace, 0, sizeof (mTrace));
    memset (mScript, 0, sizeof (mScript));
    if (Count) {
      memcpy (mScript, Statuses, Count * sizeof (mScript[0]));
    }
    memset (Tables[Tp], 0, sizeof (*Tables[Tp]));
    memset (Tables[Tp]->IpList, 0, (SCRIPT_SIZE + 1) * sizeof (IP_RECORD));
    for (Index = 0; Index < Count; Index++) {
      Tables[Tp]->IpList[Index].IpID = SilId_DfClass;
      Tables[Tp]->IpList[Index].Initialize =
        (NullMask & (1U << Index)) ? NULL : Callbacks[Index];
    }
    Tables[Tp]->IpList[Count].IpID = SilId_ListEnd;

    Actual = Timepoints[Tp] ();
    Check (Actual == ExpectedStatus, "return preserves the required status");
    for (Index = 0; Index < SCRIPT_SIZE; Index++) {
      unsigned ExpectedCalls = Index < ExpectedVisited && !(NullMask & (1U << Index));
      Check (mCalls[Index] == ExpectedCalls, "each selected IP called once and tail untouched");
      if (ExpectedCalls) {
        Check (TraceIndex < mTraceCount && mTrace[TraceIndex] == Index, "IP call order");
        TraceIndex++;
      }
    }
    Check (mTraceCount == TraceIndex, "no extra IP calls");
  }
}

int
main (
  void
  )
{
  const SIL_STATUS Deferred[] = { SilResetRequestWarmDef, SilResetRequestColdDef };
  const SIL_STATUS Terminal[] = {
    SilUnsupportedHardware, SilUnsupported, SilInvalidParameter, SilAborted,
    SilOutOfResources, SilNotFound, SilOutOfBounds, SilDeviceError,
    SilResetRequestColdImm, SilResetRequestWarmImm, (SIL_STATUS) 0x7f
  };
  SIL_STATUS Script[SCRIPT_SIZE] = { SilPass, SilPass, SilPass, SilPass };
  unsigned D;
  unsigned T;

  RunCase ("empty list", NULL, 0, 0, false, SilPass, 0);
  RunCase ("all pass", Script, 4, 0, false, SilPass, 4);
  RunCase ("registration-only entries", Script, 4, 5, false, SilPass, 4);
  for (D = 0; D < ARRAY_COUNT (Deferred); D++) {
    Script[0] = Deferred[D];
    Script[1] = SilPass;
    Script[2] = SilPass;
    Script[3] = SilPass;
    RunCase ("constant deferred then pass", Script, 4, 0, false, Deferred[D], 4);
    RunCase ("one-shot deferred then pass", Script, 4, 0, true, Deferred[D], 4);
    RunCase ("deferred across null callbacks", Script, 4, 6, false, Deferred[D], 4);
    RunCase ("deferred is final IP", Script, 1, 0, false, Deferred[D], 1);
    Script[1] = Deferred[1 - D];
    RunCase ("cold deferred dominates warm in either order", Script, 4, 0,
             false, SilResetRequestColdDef, 4);
    Script[1] = Deferred[D];
    RunCase ("repeated deferred type on different IPs", Script, 4, 0, false, Deferred[D], 4);
    for (T = 0; T < ARRAY_COUNT (Terminal); T++) {
      Script[1] = Terminal[T];
      RunCase ("constant deferred then terminal", Script, 4, 0, false, Terminal[T], 2);
      RunCase ("one-shot deferred then terminal", Script, 4, 0, true, Terminal[T], 2);
      Script[1] = SilPass;
      Script[2] = Terminal[T];
      RunCase ("deferred then pass then terminal", Script, 4, 0, true, Terminal[T], 3);
      Script[2] = SilPass;
    }
  }
  for (T = 0; T < ARRAY_COUNT (Terminal); T++) {
    Script[0] = SilPass;
    Script[1] = Terminal[T];
    Script[2] = SilResetRequestColdDef;
    Script[3] = SilPass;
    RunCase ("terminal stops before later reset", Script, 4, 0, false, Terminal[T], 2);
  }
  /* A prior call's deferred status must never leak into the next timepoint. */
  memset (Script, 0, sizeof (Script));
  RunCase ("later successful invocation", Script, 4, 0, false, SilPass, 4);
  printf ("xSIM dispatch: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures ? 1 : 0;
}
