/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FabricDomain.c
 * @brief Native NUMA domain construction and logical-CCD translation tests.
 *
 * The real domain builder and translator are linked by run.py. The fixture
 * supplies APOB NPS data, discovered CCD topology, and BRH NPS masks instead
 * of hardware. The unused global SoC descriptor deliberately contains stale
 * values, as can happen when the host supplies uncleared SIL backing memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))
#define TEST_SOCKETS 2
#define TEST_CCDS 16

_Static_assert (PROJ_MAX_SOCKETS_SUPPORTED >= TEST_SOCKETS, "2P fixture capacity");
_Static_assert (PROJ_MAX_CCD_DIES_PER_SOCKET >= TEST_CCDS, "sparse CCD fixture capacity");

static SIL_BLOCK_VARIABLES mGlobal;
static DFCLASS_INPUT_BLK mDf;
static DF_DOMAIN_INFO_BLK mDomains;
static APOB_SYSTEM_NPS_INFO_TYPE_STRUCT mNpsInfo;
static APOB_IP2IP_API mApobApi;
static DF_COMMON_2_REV_XFER_BLOCK mDfApi;
static uint32_t mSockets, mNps;
static uint32_t mCcdCount[TEST_SOCKETS], mCcxPerCcd[TEST_SOCKETS];
static uint32_t mPhysicalCcd[TEST_SOCKETS][TEST_CCDS];
static const char *mCase;
static unsigned mCases, mChecks, mFailures;

static bool
Check (bool Condition, const char *Message)
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL: %s: %s\n", mCase, Message);
  }
  return Condition;
}

static void
Require (bool Condition)
{
  if (!Condition) {
    fprintf (stderr, "Invalid fabric-domain fixture: %s\n", mCase);
    abort ();
  }
}

static uint32_t
GetSockets (void)
{
  return mSockets;
}

uint32_t
DfGetNumberOfDiesOnSocket (uint32_t Socket)
{
  Require (Socket < mSockets);
  return 1;
}

static void
GetCaps (APOB_SOC_DIE_INFO *Caps)
{
  memset (Caps, 0, sizeof (*Caps));
  Caps->MaxSocSocketsSupportedValue = TEST_SOCKETS;
  Caps->MaxSocCcdPerSocket = TEST_CCDS;
}

static SIL_STATUS
GetApobEntry (
  uint32_t Group,
  uint32_t Type,
  uint32_t Instance,
  uint32_t Base,
  APOB_TYPE_HEADER **Entry
  )
{
  Require (Group == APOB_FABRIC && Instance == 0 && Base == 0 && Entry != NULL);
  *Entry = NULL;
  if (Type == APOB_SYS_NPS_INFO_TYPE) {
    *Entry = (APOB_TYPE_HEADER *) &mNpsInfo;
    return SilPass;
  }
  Require (Type == APOB_SYS_CXL_INFO_TYPE);
  return SilNotFound;
}

static void
BuildCcdInfo (uint32_t Sockets, uint32_t Dies, void *Block)
{
  DF_DOMAIN_INFO_BLK *Domains = Block;
  Require (Sockets == mSockets && Dies == mSockets && Domains == &mDomains);
  for (uint32_t Socket = 0; Socket < Sockets; Socket++) {
    Domains->CcdCount[Socket] = mCcdCount[Socket];
    Domains->CcxPerCcd[Socket] = mCcxPerCcd[Socket];
    for (uint32_t Ccd = 0; Ccd < mCcdCount[Socket]; Ccd++) {
      Require (mPhysicalCcd[Socket][Ccd] < TEST_CCDS);
      Domains->LogToPhysCcd[Socket][Ccd] = mPhysicalCcd[Socket][Ccd];
    }
  }
}

/* BRH's published physical-CCD masks, supplied at the hardware boundary. */
static void
GetNpsMaps (uint32_t *Nps0, uint32_t *Nps1, uint32_t *Nps2, uint32_t *Nps4)
{
  *Nps0 = UINT32_MAX;
  *Nps1 = 0xFFFF;
  Nps2[0] = 0x5555;
  Nps2[1] = 0xAAAA;
  Nps4[0] = 0x1111;
  Nps4[1] = 0x4444;
  Nps4[2] = 0x8888;
  Nps4[3] = 0x2222;
}

/* Independent expected quadrant order, indexed by physical CCD. */
static uint32_t
PhysicalDomain (uint32_t Socket, uint32_t PhysicalCcd)
{
  static const uint8_t Half[] = {0, 1, 0, 1};
  static const uint8_t Quadrant[] = {0, 3, 1, 2};
  switch (mNps) {
  case 0: return 0;
  case 1: return Socket;
  case 2: return Socket * 2 + Half[PhysicalCcd % ARRAY_COUNT (Half)];
  case 4: return Socket * 4 + Quadrant[PhysicalCcd % ARRAY_COUNT (Quadrant)];
  default: Require (false); return UINT32_MAX;
  }
}

static uint32_t
GetPhysicalDomain (uint32_t Socket, uint32_t Ccd, uint32_t Count, void *Block)
{
  Require (Socket < mSockets && Ccd < TEST_CCDS && Block == &mDomains);
  Require (Count == (mNps ? mSockets * mNps : 1));
  return PhysicalDomain (Socket, Ccd);
}

SIL_BLOCK_VARIABLES *
SilGetMemoryBase (void)
{
  return &mGlobal;
}

void *
xUslFindStructure (SIL_DATA_BLOCK_ID Id, uint16_t Instance)
{
  Require (Id == SilId_DfClass);
  switch (Instance) {
  case DFCLASS_INSTANCE: return &mDf;
  case DF_DOMAIN_INSTANCE: return &mDomains;
  default: Require (false); return NULL;
  }
}

SIL_STATUS
SilGetCommon2RevXferTable (SIL_DATA_BLOCK_ID Id, void **Api)
{
  Require (Id == SilId_DfClass && Api != NULL);
  *Api = &mDfApi;
  return SilPass;
}

SIL_STATUS
SilGetIp2IpApi (SIL_DATA_BLOCK_ID Id, void **Api)
{
  Require (Id == SilId_ApobClass && Api != NULL);
  *Api = &mApobApi;
  return SilPass;
}

static void
Reset (const char *Name, uint32_t Sockets, uint32_t Nps, uint8_t StaleCcx)
{
  mCase = Name;
  mCases++;
  printf ("CASE: %s (global CCX stride %u)\n", Name, StaleCcx);
  Require (Sockets > 0 && Sockets <= TEST_SOCKETS && (Nps || Sockets == 2));
  memset (&mGlobal, 0xA5, sizeof (mGlobal));
  mGlobal.ActiveSoC.NumCcxPerCcd = StaleCcx;
  memset (&mDf, 0, sizeof (mDf));
  memset (&mDomains, 0, sizeof (mDomains));
  memset (&mNpsInfo, 0, sizeof (mNpsInfo));
  memset (&mApobApi, 0, sizeof (mApobApi));
  memset (&mDfApi, 0, sizeof (mDfApi));
  mSockets = Sockets;
  mNps = Nps;
  mNpsInfo.ActualNps = Nps == 4 ? DF_DRAM_NPS4 : Nps;
  mApobApi.ApobGetMaxDieInfo = GetCaps;
  mApobApi.ApobAmdGetApobEntryInstance = GetApobEntry;
  mDfApi.DfGetNumberOfProcessorsPresent = GetSockets;
  mDfApi.DfGetNumberOfSystemDies = GetSockets;
  mDfApi.DfBuildCcdInfo = BuildCcdInfo;
  mDfApi.DfGetNpsMapData = GetNpsMaps;
  mDfApi.DfGetPhysDomainOfCcd = GetPhysicalDomain;
  for (uint32_t Socket = 0; Socket < TEST_SOCKETS; Socket++) {
    mCcdCount[Socket] = 8;
    mCcxPerCcd[Socket] = 1;
    for (uint32_t Ccd = 0; Ccd < TEST_CCDS; Ccd++) {
      mPhysicalCcd[Socket][Ccd] = Ccd;
    }
  }
}

static bool
Build (uint32_t ExpectedReported)
{
  SIL_STATUS Status = DfXBuildDomainInfo ();
  uint32_t ExpectedPhysical = mNps ? mSockets * mNps : 1;
  return Check (Status == SilPass, "domain construction succeeds") &&
    Check (mDomains.DomainInfoValid, "domain information is valid") &&
    Check (mDomains.NumberOfPhysicalDomains == ExpectedPhysical, "physical-domain count") &&
    Check (mDomains.NumberOfReportedDomains == ExpectedReported, "reported-domain count") &&
    Check (mDomains.PhysNps == mNps && mDomains.SystemCxlCount == 0, "NPS and CXL counts");
}

static bool
Translate (uint32_t Socket, uint32_t Ccd, uint32_t Ccx, uint32_t Expected)
{
  uint32_t Domain = UINT32_MAX;
  SIL_STATUS Status = DfXDomainXlat (Socket, 0, Ccd, Ccx, &Domain);
  bool Valid = Status == SilPass && Domain == Expected;
  if (!Valid) {
    printf ("  S%u logical CCD %u physical CCD %u CCX %u: status=%u domain=%u expected=%u\n",
      Socket, Ccd, mPhysicalCcd[Socket][Ccd], Ccx, Status, Domain, Expected);
  }
  return Check (Valid, "CPU maps to its expected NUMA domain") &&
    Check ((mDomains.ReportedDomainInfo[Domain].SocketMap & (1u << Socket)) != 0,
      "reported domain contains the CPU's socket");
}

static void
TestNps (const char *Name, uint32_t Sockets, uint32_t Nps, uint8_t StaleCcx, bool Sparse)
{
  static const uint32_t SparseCcd[TEST_SOCKETS][4] = {{0, 2, 5, 7}, {1, 3, 4, 6}};
  Reset (Name, Sockets, Nps, StaleCcx);
  if (Sparse) {
    for (uint32_t Socket = 0; Socket < Sockets; Socket++) {
      mCcdCount[Socket] = ARRAY_COUNT (SparseCcd[Socket]);
      memcpy (mPhysicalCcd[Socket], SparseCcd[Socket], sizeof (SparseCcd[Socket]));
    }
  }
  if (!Build (Nps ? Sockets * Nps : 1)) {
    return;
  }
  /* Titanite's APIC 0x20 belongs to logical CCD2. Check that first so the
   * failing stale-stride case reports its affinity error before larger old
   * shifts could encounter undefined behavior elsewhere in the CPU list.
   */
  if (!Translate (0, 2, 0, PhysicalDomain (0, mPhysicalCcd[0][2]))) {
    return;
  }
  for (uint32_t Socket = 0; Socket < Sockets; Socket++) {
    for (uint32_t Ccd = 0; Ccd < mCcdCount[Socket]; Ccd++) {
      if (!Translate (Socket, Ccd, 0, PhysicalDomain (Socket, mPhysicalCcd[Socket][Ccd]))) {
        return;
      }
    }
  }
}

static void
TestCcxDomains (const char *Name, uint8_t StaleCcx, bool DifferentStrides)
{
  uint32_t Domain = 0;
  bool ValidMasks = true;
  Reset (Name, 2, 1, StaleCcx);
  mDf.AmdFabricCcxAsNumaDomain = true;
  mCcdCount[0] = mCcdCount[1] = 2;
  mPhysicalCcd[0][0] = 0;
  mPhysicalCcd[0][1] = 2;
  mPhysicalCcd[1][0] = 1;
  mPhysicalCcd[1][1] = 3;
  mCcxPerCcd[1] = DifferentStrides ? 2 : 1;
  if (!Build (2 + 2 * mCcxPerCcd[1])) {
    return;
  }
  for (uint32_t Socket = 0; Socket < mSockets; Socket++) {
    for (uint32_t Ccd = 0; Ccd < mCcdCount[Socket]; Ccd++) {
      for (uint32_t Ccx = 0; Ccx < mCcxPerCcd[Socket]; Ccx++, Domain++) {
        uint32_t Bit = Socket * 16 + mPhysicalCcd[Socket][Ccd] * mCcxPerCcd[Socket] + Ccx;
        ValidMasks &= Check (mDomains.ReportedDomainCcxMap[Domain] == (UINT32_C (1) << Bit),
          "each reported CCX claims exactly its physical topology bit");
        Check (mDomains.ReportedDomainInfo[Domain].PhysicalDomain == Socket,
          "reported CCX retains the NPS1 physical domain");
        Check (mDomains.ReportedDomainInfo[Domain].SocketMap == (1u << Socket),
          "reported CCX retains its socket");
      }
    }
    Check (mDomains.PhysicalDomainInfo[Socket].SharingEntityCount ==
      mCcdCount[Socket] * mCcxPerCcd[Socket], "physical-domain sharing count");
  }
  if (!ValidMasks) {
    return;
  }
  Domain = 0;
  for (uint32_t Socket = 0; Socket < mSockets; Socket++) {
    for (uint32_t Ccd = 0; Ccd < mCcdCount[Socket]; Ccd++) {
      for (uint32_t Ccx = 0; Ccx < mCcxPerCcd[Socket]; Ccx++, Domain++) {
        if (!Translate (Socket, Ccd, Ccx, Domain)) {
          return;
        }
      }
    }
  }
}

static void
TestSocketOneBit31 (void)
{
  Reset ("2P CCX-as-NUMA, socket1 physical CCD15", 2, 1, 1);
  mDf.AmdFabricCcxAsNumaDomain = true;
  mCcdCount[0] = mCcdCount[1] = 1;
  mPhysicalCcd[1][0] = 15;
  if (!Build (2)) {
    return;
  }
  Check (mDomains.ReportedDomainCcxMap[1] == UINT32_C (0x80000000),
    "socket1 physical CCD15 occupies bit31");
  Translate (1, 0, 0, 1);
}

int
main (void)
{
  setvbuf (stdout, NULL, _IONBF, 0);
  TestNps ("2P NPS1, zero global, Titanite CCD2", 2, 1, 0, false);
  TestNps ("2P NPS1, stale global8, Titanite CCD2", 2, 1, 8, false);
  TestNps ("2P NPS0", 2, 0, 1, false);
  TestNps ("1P NPS1", 1, 1, 1, false);
  TestNps ("1P NPS2", 1, 2, 1, false);
  TestNps ("2P NPS2", 2, 2, 1, false);
  TestNps ("1P NPS4", 1, 4, 1, false);
  TestNps ("2P NPS4", 2, 4, 1, false);
  TestNps ("2P NPS2, zero global", 2, 2, 0, false);
  TestNps ("2P NPS4, zero global", 2, 4, 0, false);
  TestNps ("2P NPS1, sparse physical CCDs", 2, 1, 1, true);
  TestNps ("2P NPS2, sparse physical CCDs", 2, 2, 1, true);
  TestNps ("2P NPS4, sparse physical CCDs", 2, 4, 1, true);
  TestCcxDomains ("2P CCX domains, zero global", 0, false);
  TestCcxDomains ("2P CCX domains, stale global8", 8, false);
  TestCcxDomains ("2P CCX domains, known global1", 1, false);
  TestCcxDomains ("2P CCX domains, different per-socket strides", 1, true);
  TestSocketOneBit31 ();
  printf ("Fabric domains: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures ? EXIT_FAILURE : EXIT_SUCCESS;
}
