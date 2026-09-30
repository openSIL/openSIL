/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CpuTopology.c
 * @brief Native enabled-CPU query tests with independent per-socket APOB maps.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <APOB/ApobIp2Ip.h>
#include <CCX/CcxIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <xPrfCpu.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))
#define TEST_CPUS 768
#define GUARD UINT64_C(0x123456789abcdef0)

static APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT mMaps[2];
static APOB_SOC_DIE_INFO mCaps;
static APOB_IP2IP_API mApobApi;
static CCX_IP2IP_API mCcxApi;
static DF_IP2IP_API mDfApi;
static uint32_t mSockets;
static uint32_t mTotalDies;
static uint32_t mDies[2];
static unsigned mMapCalls[2];
static SIL_STATUS mSystemStatus;
static SIL_STATUS mProcessorStatus[2];
static SIL_STATUS mMapStatus[2];
static int mFailLookup;
static int mNullLookup;
static bool mDuplicateApic;
static bool mInvalidApic;
static unsigned mCases;
static unsigned mChecks;
static unsigned mFailures;
static const char *mCase;
static struct {
  uint64_t Before;
  XPRF_CPU_TOPOLOGY Cpus[TEST_CPUS];
  uint64_t After;
} mOutput;

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
Require (bool Condition)
{
  if (!Condition) {
    fprintf (stderr, "Invalid CPU topology fixture\n");
    abort ();
  }
}

static SIL_STATUS
GetSystemInfo (
  uint32_t *Sockets,
  uint32_t *Dies,
  uint32_t *RootBridges,
  ROOT_BRIDGE_LOCATION *Fch,
  ROOT_BRIDGE_LOCATION *Smu
  )
{
  Require (Sockets != NULL && Dies != NULL);
  Require (RootBridges == NULL && Fch == NULL && Smu == NULL);
  *Sockets = mSockets;
  *Dies = mTotalDies;
  return mSystemStatus;
}

static SIL_STATUS
GetProcessorInfo (uint32_t Socket, uint32_t *Dies, uint32_t *RootBridges)
{
  Require (Socket < ARRAY_COUNT (mDies) && Dies != NULL && RootBridges == NULL);
  *Dies = mDies[Socket];
  return mProcessorStatus[Socket];
}

static void
GetCaps (APOB_SOC_DIE_INFO *Caps)
{
  *Caps = mCaps;
}

static SIL_STATUS
GetMap (uint32_t Socket, uint32_t Die, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *Map)
{
  Require (Socket < ARRAY_COUNT (mMaps) && Die == 0);
  mMapCalls[Socket]++;
  if (mMapStatus[Socket] == SilPass) {
    *Map = mMaps[Socket];
  }
  return mMapStatus[Socket];
}

static uint32_t
GetApic (uint32_t Socket, uint32_t Die, uint32_t Ccd, uint32_t Complex, uint32_t Core, uint32_t Thread)
{
  Require (Socket < 2 && Die == 0 && Ccd < 16 && Complex < 2 && Core < 16 && Thread < 2);
  if (mDuplicateApic) {
    return 0;
  }
  if (mInvalidApic) {
    return UINT32_MAX;
  }
  return (Socket << 12) | (Ccd << 6) | (Complex << 5) | (Core << 1) | Thread;
}

SIL_STATUS
SilGetIp2IpApi (SIL_DATA_BLOCK_ID Id, void **Api)
{
  *Api = NULL;
  if ((int) Id == mFailLookup) {
    return SilNotFound;
  }
  if ((int) Id == mNullLookup) {
    return SilPass;
  }
  switch (Id) {
  case SilId_ApobClass:
    *Api = &mApobApi;
    break;
  case SilId_CcxClass:
    *Api = &mCcxApi;
    break;
  case SilId_DfClass:
    *Api = &mDfApi;
    break;
  default:
    Require (false);
  }
  return SilPass;
}

/** Keep all unused physical entries absent and all unused bool values valid. */
static void
Reset (void)
{
  unsigned Socket;
  unsigned Ccd;
  unsigned Complex;
  unsigned Core;

  memset (mMaps, 0, sizeof (mMaps));
  for (Socket = 0; Socket < ARRAY_COUNT (mMaps); Socket++) {
    for (Ccd = 0; Ccd < ARRAY_COUNT (mMaps[Socket].CcdMap); Ccd++) {
      LOGICAL_CCD_INFO *CcdInfo = &mMaps[Socket].CcdMap[Ccd];
      CcdInfo->PhysCcdNumber = CCX_NOT_PRESENT;
      for (Complex = 0; Complex < ARRAY_COUNT (CcdInfo->ComplexMap); Complex++) {
        LOGICAL_COMPLEX_INFO *ComplexInfo = &CcdInfo->ComplexMap[Complex];
        ComplexInfo->PhysComplexNumber = CCX_NOT_PRESENT;
        for (Core = 0; Core < ARRAY_COUNT (ComplexInfo->CoreInfo); Core++) {
          ComplexInfo->CoreInfo[Core].PhysCoreNumber = CCX_NOT_PRESENT;
        }
      }
    }
  }
  mCaps = (APOB_SOC_DIE_INFO) {
    .MaxSocCcdsPerDieValue = 16,
    .MaxSocCcxPerCcdValue = PROJ_MAX_COMPLEXES_PER_CCD,
    .MaxSocCoresPerComplexValue = 16,
    .MaxSocDiesPerSocketValue = 1,
    .MaxSocSocketsSupportedValue = 2,
    .MaxSocThreadPerCore = 2,
    .MaxSocCcdPerSocket = 16,
  };
  mApobApi = (APOB_IP2IP_API) {
    .ApobGetMaxDieInfo = GetCaps,
    .ApobGetCcdLogToPhysMap = GetMap,
  };
  mCcxApi = (CCX_IP2IP_API) {.CalcLocalApic = GetApic};
  mDfApi = (DF_IP2IP_API) {
    .DfGetSystemInfo = GetSystemInfo,
    .DfGetProcessorInfo = GetProcessorInfo,
  };
  mSockets = 1;
  mTotalDies = 1;
  mDies[0] = mDies[1] = 1;
  memset (mMapCalls, 0, sizeof (mMapCalls));
  mSystemStatus = SilPass;
  mProcessorStatus[0] = mProcessorStatus[1] = SilPass;
  mMapStatus[0] = mMapStatus[1] = SilPass;
  mFailLookup = mNullLookup = -1;
  mDuplicateApic = mInvalidApic = false;
}

static void
Populate (unsigned Socket, unsigned Ccds, unsigned Complexes, unsigned Cores, unsigned Threads)
{
  unsigned Ccd;
  unsigned Complex;
  unsigned Core;
  unsigned Thread;

  Require (Socket < 2 && Ccds <= 16 && Complexes <= PROJ_MAX_COMPLEXES_PER_CCD && Cores <= 16 && Threads <= 2);
  for (Ccd = 0; Ccd < Ccds; Ccd++) {
    mMaps[Socket].CcdMap[Ccd].PhysCcdNumber = Ccd;
    for (Complex = 0; Complex < Complexes; Complex++) {
      LOGICAL_COMPLEX_INFO *Info = &mMaps[Socket].CcdMap[Ccd].ComplexMap[Complex];
      Info->PhysComplexNumber = Complex;
      for (Core = 0; Core < Cores; Core++) {
        Info->CoreInfo[Core].PhysCoreNumber = Core;
        for (Thread = 0; Thread < Threads; Thread++) {
          Info->CoreInfo[Core].IsThreadEnabled[Thread] = true;
        }
      }
    }
  }
}

static bool
Run (const char *Name, uint32_t Capacity, SIL_STATUS ExpectedStatus, uint32_t ExpectedCount)
{
  SIL_STATUS Status;
  uint32_t Count = UINT32_MAX;
  XPRF_CPU_TOPOLOGY Sentinel;

  mCase = Name;
  mCases++;
  memset (&mOutput, 0xa5, sizeof (mOutput));
  memset (&Sentinel, 0xa5, sizeof (Sentinel));
  mOutput.Before = mOutput.After = GUARD;
  Status = xPrfGetEnabledCpuTopology (Capacity, &Count, mOutput.Cpus);
  Check (Status == ExpectedStatus, "status");
  Check (Count == ExpectedCount, "complete count or zero on failure");
  Check (mOutput.Before == GUARD && mOutput.After == GUARD, "buffer guards");
  if (Capacity < TEST_CPUS) {
    Check (memcmp (&Sentinel, &mOutput.Cpus[Capacity], sizeof (Sentinel)) == 0, "capacity boundary");
  }
  return Status == SilPass;
}

static void
TestPopulations (void)
{
  unsigned Index;

  Reset ();
  Populate (0, 2, 1, 2, 2);
  Run ("one socket, SMT", TEST_CPUS, SilPass, 8);
  Check (mMapCalls[0] == 1 && mMapCalls[1] == 0, "only installed socket read");
  Check (mOutput.Cpus[7].Ccd == 1 && mOutput.Cpus[7].Core == 1 && mOutput.Cpus[7].Thread == 1,
         "logical coordinates");
  Check (mOutput.Cpus[7].ApicId == 0x43, "CCX APIC calculator used");

  Reset ();
  mSockets = mTotalDies = 2;
  Populate (0, 1, 1, 1, 2);
  Populate (1, 2, 1, 3, 1);
  Run ("different populations on two sockets", TEST_CPUS, SilPass, 8);
  Check (mMapCalls[0] == 1 && mMapCalls[1] == 1, "each socket map read once");
  Check (mOutput.Cpus[2].Socket == 1 && mOutput.Cpus[2].ApicId == 0x1000, "S1 APIC ID");
  Check (mOutput.Cpus[7].Ccd == 1 && mOutput.Cpus[7].Complex == 0 && mOutput.Cpus[7].Core == 2,
         "S1 topology differs from S0");

  Reset ();
  Populate (0, 1, 1, 2, 2);
  mMaps[0].CcdMap[0].PhysCcdNumber = 7;
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[0].PhysCoreNumber = 11;
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[1].PhysCoreNumber = 15;
  Run ("physical harvesting preserves logical indices", TEST_CPUS, SilPass, 4);
  Check (mOutput.Cpus[3].Ccd == 0 && mOutput.Cpus[3].Complex == 0 && mOutput.Cpus[3].Core == 1,
         "physical numbers are not exported as logical coordinates");

  Reset ();
  Populate (0, 1, 1, 3, 2);
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[0].IsThreadEnabled[0] = false;
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[1].IsThreadEnabled[1] = false;
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[2].IsThreadEnabled[0] = false;
  mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[2].IsThreadEnabled[1] = false;
  Run ("disabled cores and individual threads", TEST_CPUS, SilPass, 2);
  Check (mOutput.Cpus[0].Thread == 1 && mOutput.Cpus[1].Core == 1, "enabled flags honored individually");

  Reset ();
  Populate (0, 12, 1, 16, 2);
  Run ("384 threads on one socket", TEST_CPUS, SilPass, 384);
  mSockets = mTotalDies = 2;
  Populate (1, 12, 1, 16, 2);
  if (Run ("768 threads on two sockets", TEST_CPUS, SilPass, 768)) {
    for (Index = 0; Index < TEST_CPUS; Index++) {
      Check (mOutput.Cpus[Index].Socket == Index / 384, "complete socket population");
      Check (mOutput.Cpus[Index].Thread == Index % 2, "complete thread population");
    }
  }
  Run ("one record short", TEST_CPUS - 1, SilOutOfBounds, 0);
}

static void
TestFailures (void)
{
  unsigned Index;
  const SIL_DATA_BLOCK_ID Ids[] = {SilId_ApobClass, SilId_CcxClass, SilId_DfClass};

  for (Index = 0; Index < ARRAY_COUNT (Ids); Index++) {
    Reset ();
    mFailLookup = Ids[Index];
    Run ("Ip-2-Ip lookup failure", TEST_CPUS, SilNotFound, 0);
    mFailLookup = -1;
    mNullLookup = Ids[Index];
    Run ("Ip-2-Ip success with NULL table", TEST_CPUS, SilNotFound, 0);
  }
  for (Index = 0; Index < 5; Index++) {
    Reset ();
    switch (Index) {
    case 0: mApobApi.ApobGetMaxDieInfo = NULL; break;
    case 1: mApobApi.ApobGetCcdLogToPhysMap = NULL; break;
    case 2: mCcxApi.CalcLocalApic = NULL; break;
    case 3: mDfApi.DfGetSystemInfo = NULL; break;
    case 4: mDfApi.DfGetProcessorInfo = NULL; break;
    }
    Run ("missing required callback", TEST_CPUS, SilUnsupported, 0);
  }
  Reset ();
  mSystemStatus = SilDeviceError;
  Run ("DF system query failed", TEST_CPUS, SilDeviceError, 0);

  Reset ();
  Populate (0, 1, 1, 1, 1);
  mSockets = mTotalDies = 2;
  mProcessorStatus[1] = SilDeviceError;
  Run ("S1 processor query fails after S0", TEST_CPUS, SilDeviceError, 0);
  mProcessorStatus[1] = SilPass;
  mMapStatus[1] = SilNotFound;
  Run ("S1 APOB missing after S0", TEST_CPUS, SilNotFound, 0);
  Check (mMapCalls[1] == 1, "S1 map requested");
  mMapStatus[1] = SilPass;
  Run ("present S1 has no enabled CPUs", TEST_CPUS, SilAborted, 0);

  Reset ();
  Populate (0, 1, 1, 2, 1);
  mDuplicateApic = true;
  Run ("duplicate APIC ID", TEST_CPUS, SilAborted, 0);
  mDuplicateApic = false;
  mInvalidApic = true;
  Run ("broadcast APIC ID", TEST_CPUS, SilAborted, 0);

  Reset ();
  Populate (0, 1, 1, 1, 1);
  Populate (1, 1, 1, 1, 1);
  mSockets = 2;
  Run ("inconsistent system die total", TEST_CPUS, SilAborted, 0);
}

static void
TestBounds (void)
{
  unsigned Index;
  uint32_t Count = 123;

  Reset ();
  mCase = "invalid pointer arguments";
  mCases++;
  Check (xPrfGetEnabledCpuTopology (TEST_CPUS, NULL, mOutput.Cpus) == SilInvalidParameter, "NULL count");
  Check (xPrfGetEnabledCpuTopology (TEST_CPUS, &Count, NULL) == SilInvalidParameter, "NULL buffer");
  Check (Count == 0, "invalid argument clears count");
  Run ("zero capacity", 0, SilInvalidParameter, 0);

  for (Index = 0; Index < 6; Index++) {
    Reset ();
    switch (Index) {
    case 0: mCaps.MaxSocCcdsPerDieValue = 0; break;
    case 1: mCaps.MaxSocCcxPerCcdValue = 0; break;
    case 2: mCaps.MaxSocCoresPerComplexValue = 0; break;
    case 3: mCaps.MaxSocDiesPerSocketValue = 0; break;
    case 4: mCaps.MaxSocSocketsSupportedValue = 0; break;
    case 5: mCaps.MaxSocThreadPerCore = 0; break;
    }
    Run ("zero topology limit", TEST_CPUS, SilOutOfBounds, 0);
  }
  for (Index = 0; Index < 4; Index++) {
    Reset ();
    switch (Index) {
    case 0: mCaps.MaxSocCcdsPerDieValue = PROJ_MAX_CCD_DIES_PER_SOCKET + 1; break;
    case 1: mCaps.MaxSocCcxPerCcdValue = PROJ_MAX_COMPLEXES_PER_CCD + 1; break;
    case 2: mCaps.MaxSocCoresPerComplexValue = PROJ_MAX_CCX_CORES_PER_COMPLEX + 1; break;
    case 3: mCaps.MaxSocThreadPerCore = PROJ_MAX_CCX_THREADS_PER_CORE + 1; break;
    }
    Run ("topology limit exceeds array bounds", TEST_CPUS, SilOutOfBounds, 0);
    Check (mMapCalls[0] == 0 && mMapCalls[1] == 0, "reject before APOB map copy");
  }
  Reset ();
  mSockets = 0;
  Run ("no installed sockets", TEST_CPUS, SilOutOfBounds, 0);
  mSockets = 3;
  Run ("too many installed sockets", TEST_CPUS, SilOutOfBounds, 0);
  mSockets = 1;
  mDies[0] = 0;
  Run ("no die on installed socket", TEST_CPUS, SilOutOfBounds, 0);
  mDies[0] = 2;
  Run ("too many dies on socket", TEST_CPUS, SilOutOfBounds, 0);

  for (Index = 0; Index < 3; Index++) {
    Reset ();
    Populate (0, 2, 1, 2, 1);
    switch (Index) {
    case 0: mMaps[0].CcdMap[0].PhysCcdNumber = 16; break;
    case 1: mMaps[0].CcdMap[0].ComplexMap[0].PhysComplexNumber = PROJ_MAX_COMPLEXES_PER_CCD; break;
    case 2: mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[0].PhysCoreNumber = 16; break;
    }
    Run ("out-of-range physical location", TEST_CPUS, SilOutOfBounds, 0);
  }
  for (Index = 0; Index < 2; Index++) {
    Reset ();
    Populate (0, 2, 1, 2, 1);
    switch (Index) {
    case 0: mMaps[0].CcdMap[0].PhysCcdNumber = CCX_NOT_PRESENT; break;
    case 1: mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[0].PhysCoreNumber = CCX_NOT_PRESENT; break;
    }
    Run ("gap in compact logical indices", TEST_CPUS, SilOutOfBounds, 0);
  }
  for (Index = 0; Index < 2; Index++) {
    Reset ();
    Populate (0, 2, 1, 2, 1);
    switch (Index) {
    case 0: mMaps[0].CcdMap[1].PhysCcdNumber = 0; break;
    case 1: mMaps[0].CcdMap[0].ComplexMap[0].CoreInfo[1].PhysCoreNumber = 0; break;
    }
    Run ("duplicate physical location", TEST_CPUS, SilAborted, 0);
  }
}

int
main (void)
{
  TestPopulations ();
  TestFailures ();
  TestBounds ();
  printf ("CPU topology: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures != 0;
}
