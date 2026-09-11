/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file ProviderContracts.c
 * @brief Native FCH, fabric locality and RAS service regression tests.
 *
 * Production sources and provider types are linked by run.py. Only hardware
 * access and the SIL arena's backing storage are supplied by this fixture.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SilCommon.h>
#include <xSIM.h>
#include <xPRF-api.h>
#include <CommonLib/Io.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/CpuLib.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <DF/DfClass-api.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include <RAS/RasIp2Ip.h>

#define ARRAY_COUNT(Array) (sizeof (Array) / sizeof ((Array)[0]))

SIL_BLOCK_VARIABLES *mSilMemoryBase;
extern const DFCLASS_INPUT_BLK mDfClassDflts;

static union {
  max_align_t Alignment;
  uint8_t     Bytes[65536];
} mMemory;

typedef struct {
  char      Operation;
  unsigned  Width;
  uint16_t  Port;
  uint32_t  Value;
} IO_ACCESS;

static IO_ACCESS mIo[64];
static size_t mIoCount;
static uint8_t mPorts[UINT16_MAX + 1];
static uint16_t mGpeStatusPort;
static unsigned mMmioCount;
static unsigned mCacheFlushes;
static unsigned mDelegatedCalls;
static FCHCLASS_INPUT_BLK *mExpectedFch;
static FCHHWACPI_INPUT_BLK *mExpectedAcpi;
static unsigned mChecks;
static unsigned mFailures;

static void
Check (
  bool Condition,
  const char *Name
  )
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL: %s\n", Name);
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

static void
ResetSil (
  void
  )
{
  memset (&mMemory, 0, sizeof (mMemory));
  SilSetMemoryBase ((SIL_BLOCK_VARIABLES *) mMemory.Bytes);
  mSilMemoryBase->HostBlockSize = sizeof (mMemory.Bytes);
  mSilMemoryBase->FreeSpaceOffset = sizeof (SIL_BLOCK_VARIABLES);
  mSilMemoryBase->FreeSpaceLeft = sizeof (mMemory.Bytes) - sizeof (SIL_BLOCK_VARIABLES);
}

static uint32_t
ReadPort (
  unsigned Width,
  uint16_t Port
  )
{
  uint32_t Value = 0;

  Require ((size_t) Port + Width / 8 <= sizeof (mPorts), "I/O read in fixture bounds");
  memcpy (&Value, &mPorts[Port], Width / 8);
  return Value;
}

static void
SetPort (
  unsigned Width,
  uint16_t Port,
  uint32_t Value
  )
{
  Require ((size_t) Port + Width / 8 <= sizeof (mPorts), "I/O write in fixture bounds");
  memcpy (&mPorts[Port], &Value, Width / 8);
}

static void
RecordIo (
  char Operation,
  unsigned Width,
  uint16_t Port,
  uint32_t Value
  )
{
  Require (mIoCount < ARRAY_COUNT (mIo), "I/O trace capacity");
  mIo[mIoCount++] = (IO_ACCESS) { Operation, Width, Port, Value };
}

static void
WriteIo (
  unsigned Width,
  uint16_t Port,
  uint32_t Value
  )
{
  RecordIo ('W', Width, Port, Value);
  if ((Width == 32) && (Port == mGpeStatusPort)) {
    SetPort (Width, Port, ReadPort (Width, Port) & ~Value);
  } else {
    SetPort (Width, Port, Value);
  }
}

/* NASM_ABI is required even though these functions are implemented in C. */
NASM_ABI void xUSLIoWrite8 (uint16_t Port, uint8_t Value) { WriteIo (8, Port, Value); }
NASM_ABI void xUSLIoWrite16 (uint16_t Port, uint16_t Value) { WriteIo (16, Port, Value); }
NASM_ABI void xUSLIoWrite32 (uint16_t Port, uint32_t Value) { WriteIo (32, Port, Value); }

NASM_ABI uint8_t
xUSLIoRead8 (uint16_t Port)
{
  uint8_t Value = ReadPort (8, Port);
  RecordIo ('R', 8, Port, Value);
  return Value;
}

NASM_ABI uint16_t
xUSLIoRead16 (uint16_t Port)
{
  uint16_t Value = ReadPort (16, Port);
  RecordIo ('R', 16, Port, Value);
  return Value;
}

NASM_ABI uint32_t
xUSLIoRead32 (uint16_t Port)
{
  uint32_t Value = ReadPort (32, Port);
  RecordIo ('R', 32, Port, Value);
  return Value;
}

NASM_ABI void xUslWbinvd (void) { mCacheFlushes++; }

void
xUSLMemReadModifyWrite8 (void *Address, uint8_t AndMask, uint8_t OrMask)
{
  (void) Address;
  (void) AndMask;
  (void) OrMask;
  mMmioCount++;
}

void
xUSLMemReadModifyWrite16 (void *Address, uint16_t AndMask, uint16_t OrMask)
{
  (void) Address;
  (void) AndMask;
  (void) OrMask;
  mMmioCount++;
}

void
xUSLMemReadModifyWrite32 (void *Address, uint32_t AndMask, uint32_t OrMask)
{
  (void) Address;
  (void) AndMask;
  (void) OrMask;
  mMmioCount++;
}

/* These services contain inline MMIO reads and must not execute natively. */
void
__wrap_FchI2cReleaseControl (FCHCLASS_INPUT_BLK *Fch, FCHHWACPI_INPUT_BLK *Acpi)
{
  Check ((Fch == mExpectedFch) && (Acpi == mExpectedAcpi), "SPD release receives actual input blocks");
  mDelegatedCalls++;
}

void
__wrap_FchHwAcpiServiceSmiTimerStart (FCHCLASS_INPUT_BLK *Fch, FCHHWACPI_INPUT_BLK *Acpi)
{
  Check ((Fch == mExpectedFch) && (Acpi == mExpectedAcpi), "timer start receives actual input blocks");
  mDelegatedCalls++;
}

void
__wrap_FchHwAcpiServiceSmiTimerStop (FCHCLASS_INPUT_BLK *Fch, FCHHWACPI_INPUT_BLK *Acpi)
{
  Check ((Fch == mExpectedFch) && (Acpi == mExpectedAcpi), "timer stop receives actual input blocks");
  mDelegatedCalls++;
}

static void
ResetIo (
  const FCHHWACPI_INPUT_BLK *Acpi
  )
{
  memset (mPorts, 0, sizeof (mPorts));
  mIoCount = 0;
  mMmioCount = 0;
  mCacheFlushes = 0;
  mDelegatedCalls = 0;
  mGpeStatusPort = (Acpi == NULL) ? UINT16_MAX : Acpi->AcpiGpe0BlkAddr;
}

static bool
OneAccess (
  char Operation,
  unsigned Width,
  uint16_t Port,
  uint32_t Value
  )
{
  unsigned Matches = 0;

  for (size_t Index = 0; Index < mIoCount; Index++) {
    if ((mIo[Index].Operation == Operation) && (mIo[Index].Port == Port)) {
      if ((mIo[Index].Width != Width) || (mIo[Index].Value != Value)) {
        return false;
      }
      Matches++;
    }
  }
  return Matches == 1;
}

static bool
TimerUntouched (
  uint16_t TimerPort
  )
{
  for (size_t Index = 0; Index < mIoCount; Index++) {
    if ((mIo[Index].Port < (uint32_t) TimerPort + 4) &&
        ((uint32_t) mIo[Index].Port + mIo[Index].Width / 8 > TimerPort)) {
      return false;
    }
  }
  return true;
}

static void
TestFchLookup (
  void
  )
{
  static const struct {
    const char *Name;
    SIL_STATUS (*Call)(void);
    bool NeedsClass;
  } Services[] = {
    { "release SPD bus", xPrfFchReleaseSpdBus, true },
    { "power button", xPrfFchServicePowerButton, false },
    { "ACPI on", xPrfFchServiceAcpiOn, true },
    { "ACPI off", xPrfFchServiceAcpiOff, true },
    { "SMI timer start", xPrfFchServiceSmiTimerStart, true },
    { "SMI timer stop", xPrfFchServiceSmiTimerStop, true },
  };
  char Name[120];

  printf ("FCH input block lookup and dispatch\n");
  for (unsigned Present = 0; Present < 4; Present++) {
    bool HasAcpi = (Present & 1) != 0;
    bool HasClass = (Present & 2) != 0;

    ResetSil ();
    mExpectedFch = NULL;
    mExpectedAcpi = NULL;
    if (HasClass) {
      mExpectedFch = SilCreateInfoBlock (SilId_FchClass, sizeof (*mExpectedFch), 0, 0, 1);
      Require (mExpectedFch != NULL, "create FCH input block");
    }
    if (HasAcpi) {
      Require (FchHwAcpiPreliminarySetInputBlk () == SilPass, "set actual FCH ACPI defaults");
      mExpectedAcpi = SilFindStructure (SilId_FchHwAcpiP, 0);
      Require (mExpectedAcpi != NULL, "find preliminary FCH ACPI block");
    }
    Check (SilFindStructure (SilId_FchHwAcpi, 0) == NULL, "execution-only FCH ID has no input block");
    for (size_t Index = 0; Index < ARRAY_COUNT (Services); Index++) {
      bool Available = HasAcpi && (HasClass || !Services[Index].NeedsClass);
      SIL_STATUS Status;

      ResetIo (mExpectedAcpi);
      Status = Services[Index].Call ();
      snprintf (Name, sizeof (Name), "%s status with ACPI=%u class=%u",
        Services[Index].Name, HasAcpi, HasClass);
      Check (Status == (Available ? SilPass : SilNotFound), Name);
      snprintf (Name, sizeof (Name), "%s %s with ACPI=%u class=%u",
        Services[Index].Name, Available ? "dispatches" : "has no hardware effects", HasAcpi, HasClass);
      Check (((mIoCount + mMmioCount + mCacheFlushes + mDelegatedCalls) != 0) == Available, Name);
    }
  }
}

static void
TestFchRegisters (
  void
  )
{
  FCHHWACPI_INPUT_BLK *Acpi;
  FCHCLASS_INPUT_BLK Fch = { 0 };

  ResetSil ();
  Require (FchHwAcpiPreliminarySetInputBlk () == SilPass, "set FCH defaults for register tests");
  Acpi = SilFindStructure (SilId_FchHwAcpiP, 0);
  Require (Acpi != NULL, "find FCH defaults for register tests");
  for (unsigned Relocated = 0; Relocated < 2; Relocated++) {
    printf ("FCH registers with %s bases\n", Relocated ? "independently relocated" : "provider default");
    if (Relocated) {
      Acpi->AcpiPm1EvtBlkAddr = 0x1300;
      Acpi->AcpiPm1CntBlkAddr = 0x1604;
      Acpi->AcpiPmTmrBlkAddr = 0x1908;
      Acpi->AcpiGpe0BlkAddr = 0x1c20;
    }

    ResetIo (Acpi);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr, 0x9a54);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr + 2, 0xcafe);
    SetPort (32, Acpi->AcpiGpe0BlkAddr, 0x80420001);
    SetPort (32, Acpi->AcpiGpe0BlkAddr + 4, UINT32_MAX);
    FchHwAcpiServiceAcpiOn (&Fch, Acpi);
    Check (OneAccess ('R', 16, Acpi->AcpiPm1CntBlkAddr, 0x9a54), "SCI enable reads 16-bit PM1 control");
    Check (OneAccess ('W', 16, Acpi->AcpiPm1CntBlkAddr, 0x9a55), "SCI enable preserves every other PM1 bit");
    Check (ReadPort (16, Acpi->AcpiPm1CntBlkAddr + 2) == 0xcafe, "SCI enable preserves adjacent register");
    Check (OneAccess ('W', 32, Acpi->AcpiGpe0BlkAddr + 4, 0), "ACPI on disables the GPE enable register");
    Check (OneAccess ('R', 32, Acpi->AcpiGpe0BlkAddr, 0x80420001), "ACPI on reads current GPE status");
    Check (OneAccess ('W', 32, Acpi->AcpiGpe0BlkAddr, 0x80420001), "GPE clear writes only the observed status bits");
    Check (ReadPort (32, Acpi->AcpiGpe0BlkAddr) == 0, "GPE write-one-to-clear removes pending events");
    Check (ReadPort (32, Acpi->AcpiGpe0BlkAddr + 4) == 0, "GPE events remain disabled");
    Check (TimerUntouched (Acpi->AcpiPmTmrBlkAddr), "SCI enable does not access the PM timer");

    ResetIo (Acpi);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr, 0xcb97);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr + 2, 0xcafe);
    FchHwAcpiServiceAcpiOff (&Fch, Acpi);
    Check (OneAccess ('R', 16, Acpi->AcpiPm1CntBlkAddr, 0xcb97), "SCI disable reads 16-bit PM1 control");
    Check (OneAccess ('W', 16, Acpi->AcpiPm1CntBlkAddr, 0xcb96), "SCI disable preserves every other PM1 bit");
    Check (ReadPort (16, Acpi->AcpiPm1CntBlkAddr + 2) == 0xcafe, "SCI disable preserves adjacent register");
    Check (TimerUntouched (Acpi->AcpiPmTmrBlkAddr), "SCI disable does not access the PM timer");

    ResetIo (Acpi);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr, 0xaba1);
    SetPort (16, Acpi->AcpiPm1CntBlkAddr + 2, 0xcafe);
    FchHwAcpiServicePowerButton (Acpi);
    Check (OneAccess ('W', 16, Acpi->AcpiPm1EvtBlkAddr, 0x100), "power button clears PM1 power-button status");
    Check (OneAccess ('R', 16, Acpi->AcpiPm1CntBlkAddr, 0xaba1), "power button reads 16-bit PM1 control");
    Check (OneAccess ('W', 16, Acpi->AcpiPm1CntBlkAddr, 0xb7a1), "power button sets S5 and preserves other PM1 bits");
    Check (ReadPort (16, Acpi->AcpiPm1CntBlkAddr + 2) == 0xcafe, "power button preserves adjacent register");
    Check (TimerUntouched (Acpi->AcpiPmTmrBlkAddr), "power button does not access the PM timer");
    Check (mCacheFlushes == 1, "power button requests a cache flush through the mock");
  }
}

static void
CreateDfBlocks (
  DFCLASS_INPUT_BLK **Df,
  DF_DOMAIN_INFO_BLK **Domains
  )
{
  uintptr_t NextData;

  ResetSil ();
  NextData = (uintptr_t) mMemory.Bytes + mSilMemoryBase->FreeSpaceOffset + sizeof (SIL_INFO_BLOCK_HEADER);
  if (NextData % _Alignof (DFCLASS_INPUT_BLK) != 0) {
    /* A 5-byte payload plus header advances by 20 bytes, adding 4 modulo 8. */
    Require (SilCreateInfoBlock (SilId_DfClass, 5, UINT16_MAX, 0, 1) != NULL, "create alignment padding block");
  }
  *Df = SilCreateInfoBlock (SilId_DfClass, sizeof (**Df), DFCLASS_INSTANCE, 0, 1);
  *Domains = SilCreateInfoBlock (SilId_DfClass, sizeof (**Domains), DF_DOMAIN_INSTANCE, 0, 1);
  Require ((*Df != NULL) && (*Domains != NULL), "create DF service blocks");
  Require ((uintptr_t) *Df % _Alignof (DFCLASS_INPUT_BLK) == 0, "DF native alignment");
  memcpy (*Df, &mDfClassDflts, sizeof (**Df));
}

static void
TestDomainCounts (
  void
  )
{
  static const struct {
    const char *Name;
    uint8_t Sockets;
    uint32_t Nps;
    uint32_t Cxl;
    bool CcxAsNuma;
    uint32_t CcdCount;
    uint32_t CcxPerCcd;
    uint32_t Reported;
    uint32_t Physical;
    uint32_t Maximum;
  } Cases[] = {
    { "2P NPS0 interleaved domain", 2, 0, 0, false, 0, 0, 1, 1, 1 },
    { "2P NPS0 plus two CXL domains", 2, 0, 2, false, 0, 0, 3, 3, 3 },
    { "1P NPS1", 1, 1, 0, false, 0, 0, 1, 1, 1 },
    { "2P NPS1", 2, 1, 0, false, 0, 0, 2, 2, 2 },
    { "1P NPS2", 1, 2, 0, false, 0, 0, 2, 2, 2 },
    { "2P NPS2 plus one CXL domain", 2, 2, 1, false, 0, 0, 5, 5, 5 },
    { "2P NPS4", 2, 4, 0, false, 0, 0, 8, 8, 8 },
    { "unspecified socket count defaults to one", 0, 1, 0, false, 0, 0, 1, 1, 1 },
    { "1P CCX domains", 1, 1, 0, true, 4, 2, 8, 1, 8 },
    { "2P CCX plus CXL domains", 2, 1, 2, true, 8, 1, 18, 4, 18 },
    { "maximum covers more reported than estimated domains", 2, 1, 0, true, 2, 1, 6, 2, 6 },
  };
  DFCLASS_INPUT_BLK *Df;
  DF_DOMAIN_INFO_BLK *Domains;
  uint32_t Reported = UINT32_MAX;
  uint32_t Physical, Nps, Cxl, Maximum;
  bool CcxAsNuma;
  SIL_STATUS Status;

  printf ("DF domain counts\n");
  ResetSil ();
  Status = DfXAcpiGetDomainCounts (&Reported, NULL, NULL, NULL, NULL, NULL);
  Check ((Status == SilUnsupported) && (Reported == UINT32_MAX), "missing domain block leaves count untouched");
  CreateDfBlocks (&Df, &Domains);
  Status = DfXAcpiGetDomainCounts (&Reported, NULL, NULL, NULL, NULL, NULL);
  Check ((Status == SilUnsupported) && (Reported == UINT32_MAX), "unbuilt domain info leaves count untouched");
  Domains->DomainInfoValid = true;
  for (size_t Index = 0; Index < ARRAY_COUNT (Cases); Index++) {
    Df->AmdNumberOfPhysicalSocket = Cases[Index].Sockets;
    Domains->PhysNps = Cases[Index].Nps;
    Domains->SystemCxlCount = Cases[Index].Cxl;
    Domains->CcxAsNuma = Cases[Index].CcxAsNuma;
    Domains->CcdCount[0] = Cases[Index].CcdCount;
    Domains->CcxPerCcd[0] = Cases[Index].CcxPerCcd;
    Domains->NumberOfReportedDomains = Cases[Index].Reported;
    Domains->NumberOfPhysicalDomains = Cases[Index].Physical;
    Maximum = UINT32_MAX;
    Status = DfXAcpiGetDomainCounts (&Reported, &Physical, &Nps, &Cxl, &Maximum, &CcxAsNuma);
    Check ((Status == SilPass) && (Reported == Cases[Index].Reported) &&
      (Physical == Cases[Index].Physical) && (Nps == Cases[Index].Nps) &&
      (Cxl == Cases[Index].Cxl) && (CcxAsNuma == Cases[Index].CcxAsNuma) &&
      (Maximum == Cases[Index].Maximum), Cases[Index].Name);
  }
  Check (DfXAcpiGetDomainCounts (NULL, NULL, NULL, NULL, NULL, NULL) == SilPass, "domain count outputs are optional");
}

static void
CheckDistances (
  uint8_t Forward,
  uint8_t Reverse,
  const char *Name
  )
{
  uint8_t Distance[4] = { 0 };
  SIL_STATUS Status = DfXAcpiGetDistanceInfo (2, sizeof (Distance), Distance);

  Check ((Status == SilPass) && (Distance[0] == 10) && (Distance[3] == 10) &&
    (Distance[1] == Forward) && (Distance[2] == Reverse), Name);
}

static void
TestDistances (
  void
  )
{
  DFCLASS_INPUT_BLK *Df;
  DF_DOMAIN_INFO_BLK *Domains;
  uint8_t Distance[4];
  const uint8_t Untouched[4] = { 0xa5, 0xa5, 0xa5, 0xa5 };
  SIL_STATUS Status;

  printf ("DF default and host locality policy\n");
  CreateDfBlocks (&Df, &Domains);
  Domains->DomainInfoValid = true;
  Domains->NumberOfReportedDomains = 2;
  Domains->NumberOfPhysicalDomains = 2;
  Domains->ReportedDomainInfo[0] = (FABRIC_DOMAIN_INFO2) { NumaDram, 1, 0 };
  Domains->ReportedDomainInfo[1] = (FABRIC_DOMAIN_INFO2) { NumaDram, 1, 1 };
  CheckDistances (12, 12, "provider defaults select same-socket DRAM distance 12");
  Domains->ReportedDomainInfo[1].SocketMap = 2;
  CheckDistances (28, 28, "provider defaults select cross-socket DRAM distance 28");
  Df->AmdFabricSlitAutoRemoteFar = true;
  CheckDistances (32, 32, "provider remote-far policy selects distance 32");
  Df->AmdFabricSlitAutoRemoteFar = false;
  Domains->ReportedDomainInfo[1].PhysicalDomain = 0;
  CheckDistances (11, 11, "provider defaults select virtual-domain distance 11");
  Domains->ReportedDomainInfo[1] = (FABRIC_DOMAIN_INFO2) { NumaCxl, 1, 1 };
  CheckDistances (18, 255, "provider defaults select local CXL distance 18 and unreachable return path");
  Domains->ReportedDomainInfo[1].SocketMap = 2;
  CheckDistances (28, 255, "provider defaults select remote CXL distance 28");

  Df->AmdFabricSlitDistancePcdCtrl = 0;
  Df->AmdFabricSlitLocalDistance = 14;
  Df->AmdFabricSlitRemoteDistance = 37;
  Df->AmdFabricSlitVirtualDistance = 13;
  Df->AmdFabricSlitCxlLocalDistance = 22;
  Df->AmdFabricSlitCxlRemoteDistance = 43;
  Domains->ReportedDomainInfo[1] = (FABRIC_DOMAIN_INFO2) { NumaDram, 1, 1 };
  CheckDistances (14, 14, "host overrides same-socket DRAM distance");
  Domains->ReportedDomainInfo[1].SocketMap = 2;
  Df->AmdFabricSlitAutoRemoteFar = true;
  CheckDistances (37, 37, "host remote override takes precedence over remote-far policy");
  Domains->ReportedDomainInfo[1].PhysicalDomain = 0;
  CheckDistances (13, 13, "host overrides virtual-domain distance");
  Domains->ReportedDomainInfo[1] = (FABRIC_DOMAIN_INFO2) { NumaCxl, 1, 1 };
  CheckDistances (22, 255, "host overrides local CXL distance");
  Domains->ReportedDomainInfo[1].SocketMap = 2;
  CheckDistances (43, 255, "host overrides remote CXL distance");
  Df->AmdFabricSlitDistancePcdCtrl = 1;
  CheckDistances (28, 255, "selecting provider policy ignores explicit host values");

  memcpy (Distance, Untouched, sizeof (Distance));
  Status = DfXAcpiGetDistanceInfo (2, sizeof (Distance) - 1, Distance);
  Check ((Status == SilOutOfBounds) && (memcmp (Distance, Untouched, sizeof (Distance)) == 0),
    "short distance buffer remains untouched");
  Status = DfXAcpiGetDistanceInfo (3, sizeof (Distance), Distance);
  Check ((Status == SilOutOfBounds) && (memcmp (Distance, Untouched, sizeof (Distance)) == 0),
    "excess domain count leaves distance buffer untouched");
  Check (DfXAcpiGetDistanceInfo (2, sizeof (Distance), NULL) == SilInvalidParameter, "NULL distance buffer rejected");
}

static void
TestRasUnsupported (
  void
  )
{
  RAS_IP2IP_API Ras = { 0 };
  SIL_NORMALIZED_ADDRESS Normalized, OriginalNormalized;
  SIL_DIMM_INFO Dimm, OriginalDimm;
  SIL_ADDR_DATA Map = { 0 };
  SIL_ADDR_DATA OriginalMap;
  const uint64_t OriginalAddress = UINT64_C (0x1122334455667788);
  const uint64_t OriginalDpa = UINT64_C (0xaabbccddeeff0011);
  uint64_t Address;
  uint64_t Dpa;
  SIL_STATUS Status;

  printf ("RAS unsupported translation contracts\n");
  memset (&Normalized, 0x5a, sizeof (Normalized));
  memcpy (&OriginalNormalized, &Normalized, sizeof (Normalized));
  memset (&Dimm, 0xa5, sizeof (Dimm));
  memcpy (&OriginalDimm, &Dimm, sizeof (Dimm));
  Map.RANK_SIZE_PER_UMCCH_ADDR_TRANS[0][0] = UINT64_C (0x56781234);
  memcpy (&OriginalMap, &Map, sizeof (Map));
  for (unsigned Registered = 0; Registered < 2; Registered++) {
    ResetSil ();
    if (Registered) {
      Require (SilInitIp2IpApi (SilId_RasClass, &Ras) == SilPass, "register RAS without translation callbacks");
    }
    Address = OriginalAddress;
    Dpa = OriginalDpa;
    Status = xPrfMcaErrorAddrTranslate (&Address, &Normalized, &Dimm, &Map);
    Check ((Status == SilUnsupported) && (Address == OriginalAddress) &&
      (memcmp (&Normalized, &OriginalNormalized, sizeof (Normalized)) == 0) &&
      (memcmp (&Dimm, &OriginalDimm, sizeof (Dimm)) == 0), "unsupported normalized-to-system leaves outputs untouched");
    Status = xPrfTranslateSysAddrToDpa (&Address, &Dpa, &Map);
    Check ((Status == SilUnsupported) && (Address == OriginalAddress) && (Dpa == OriginalDpa),
      "unsupported system-to-DPA leaves outputs untouched");
    if (Registered) {
      Status = xPrfTranslateSysAddrToCS (&Address, &Normalized, &Dimm, &Map);
      Check ((Status == SilUnsupported) && (Address == OriginalAddress) &&
        (memcmp (&Normalized, &OriginalNormalized, sizeof (Normalized)) == 0) &&
        (memcmp (&Dimm, &OriginalDimm, sizeof (Dimm)) == 0), "missing CalcNormAddr callback leaves outputs untouched");
    }
    Check (memcmp (&Map, &OriginalMap, sizeof (Map)) == 0, "unsupported translations preserve input DIMM map");
  }
}

int
main (
  void
  )
{
  setvbuf (stdout, NULL, _IOLBF, 0);
  TestFchLookup ();
  TestFchRegisters ();
  TestDomainCounts ();
  TestDistances ();
  TestRasUnsupported ();
  printf ("Provider contracts: %u checks, %u failures\n", mChecks, mFailures);
  return (mFailures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
