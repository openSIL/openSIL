/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/** @file Native memory inventory tests using raw APOB and DDR5 SPD records. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/BRH/Apob-BRH.h>
#include <APOB/BRH/ApobInitBrh.h>
#include <MEM/MemIp2Ip.h>
#include <MEM/Common/Mem.h>
#include <MEM/Common/MemCmn2Rev.h>
#include <MEM/Common/MemSpd5.h>
#include <MEM/Brh/MemCmn2RevBrh.h>

#define DIMMS 48
#define GUARD UINT64_C(0x123456789abcdef0)
#define CHECK(Condition) Check ((Condition), #Condition, __LINE__)

static union {
  uint64_t Alignment;
  uint8_t Bytes[sizeof (APOB_MEM_DMI_HEADER) + DIMMS *
    (sizeof (APOB_MEM_DMI_PHYSICAL_DIMM_BRH) + sizeof (APOB_MEM_DMI_LOGICAL_DIMM_BRH))];
} mRaw;
static APOB_MEM_DIMM_D5_SPD_DATA_STRUCT mSpd[2];
static MEM_IP2IP_API mMemApi;
static MEM_COMMON_2_REV_XFER_BLOCK mMemC2r;
static APOB_IP2IP_API mApobApi;
static SIL_STATUS mLookupStatus;
static SIL_STATUS mSpdStatus[2];
static SIL_STATUS mC2rStatus;
static int mFailApi;
static int mNullApi;
static bool mNullC2r;
static bool mNullApob;
static bool mNullSpd;
static unsigned mMaxDies;
static unsigned mChecks;
static unsigned mFailures;
static unsigned mCases;
static struct {
  uint64_t Before;
  SIL_DMI_INFO Dmi;
  uint64_t After;
} mOutput;

static void
Check (bool Condition, const char *Text, unsigned Line)
{
  mChecks++;
  if (!Condition) {
    mFailures++;
    printf ("FAIL line %u: %s\n", Line, Text);
  }
}

static APOB_MEM_DMI_HEADER *
Header (void)
{
  return (APOB_MEM_DMI_HEADER *)mRaw.Bytes;
}

static APOB_MEM_DMI_PHYSICAL_DIMM_BRH *
Physical (unsigned Index)
{
  return (APOB_MEM_DMI_PHYSICAL_DIMM_BRH *)(Header () + 1) + Index;
}

static void
GetCaps (APOB_SOC_DIE_INFO *Caps)
{
  memset (Caps, 0, sizeof (*Caps));
  Caps->MaxSocDiesPerSocketValue = mMaxDies;
}

SIL_STATUS
AmdGetApobEntryInstance (
  uint32_t Group, uint32_t Type, uint32_t Instance, uint32_t Address, APOB_TYPE_HEADER **Entry
  )
{
  *Entry = NULL;
  CHECK (Address == 0);
  if (Group == APOB_SMBIOS && Type == APOB_MEM_SMBIOS_TYPE) {
    CHECK (Instance == 0);
    if (!mNullApob) {
      *Entry = &Header ()->ApobTypeHeader;
    }
    return mLookupStatus;
  }
  if (Group == APOB_MEM && Type == APOB_MEM_DIMM_SPD_DATA_TYPE) {
    unsigned Socket = Instance >> 8;
    CHECK (Socket < 2 && (Instance & 0xff) == 0);
    if (Socket >= 2) {
      abort ();
    }
    if (!mNullSpd) {
      *Entry = &mSpd[Socket].ApobTypeHeader;
    }
    return mSpdStatus[Socket];
  }
  abort ();
}

SIL_STATUS
SilGetIp2IpApi (SIL_DATA_BLOCK_ID Id, void **Api)
{
  *Api = NULL;
  if ((int)Id == mFailApi) {
    return SilNotFound;
  }
  if ((int)Id == mNullApi) {
    return SilPass;
  }
  if (Id == SilId_MemClass) {
    *Api = &mMemApi;
  } else if (Id == SilId_ApobClass) {
    *Api = &mApobApi;
  } else {
    abort ();
  }
  return SilPass;
}

SIL_STATUS
SilGetCommon2RevXferTable (SIL_DATA_BLOCK_ID Id, void **Api)
{
  CHECK (Id == SilId_MemClass);
  *Api = mNullC2r ? NULL : &mMemC2r;
  return mC2rStatus;
}

static void
AddDimm (unsigned Socket, unsigned Channel, unsigned Slot, bool Present)
{
  APOB_MEM_DMI_PHYSICAL_DIMM_BRH *Dimm = Physical (Header ()->MaxPhysicalDimms++);
  APOB_D5_SPD_STRUCT *Spd = &mSpd[Socket].DimmSmbusInfo[Channel * 2 + Slot];
  SPD_BASE_CONFIG_0_S *Base = (SPD_BASE_CONFIG_0_S *)Spd->Data;
  SPD_ANNEX_COMMON_S *Module = (SPD_ANNEX_COMMON_S *)&Spd->Data[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN];
  SPD_MANUFACTURING_INFO_S *Mfg = (SPD_MANUFACTURING_INFO_S *)&Spd->Data[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN];

  *Dimm = (APOB_MEM_DMI_PHYSICAL_DIMM_BRH) {
    .Socket = Socket, .Channel = Channel, .Dimm = Slot, .DimmPresent = Present,
    .Handle = Header ()->MaxPhysicalDimms, .ConfigSpeed = 2400, .ConfigVoltage = 1100
  };
  Spd->SocketNumber = Socket;
  Spd->ChannelNumber = Channel;
  Spd->DimmNumber = Slot;
  Spd->DimmPresent = Present;
  Base->KeyByte1.Field.ModuleType = 0x12;
  Base->KeyByte2.Field.BaseModuleType = SPD_BASEMODULE_RDIMM;
  Base->FirstDensity.Field.Density = 4; /* 16 Gb */
  Base->FirstDensity.Field.DiePerPkg = 0; /* one die */
  Base->FirstIoWidth.Field.IoWidth = 0; /* x4 */
  Base->TckAvgMin.Value = 416; /* 2400 MHz */
  Module->ModuleOrg.Field.RanksPerChannel = 1; /* two ranks */
  Module->ChBusWidth.Field.Width = 2; /* 32-bit subchannels */
  Module->ChBusWidth.Field.WidthExt = 2; /* eight ECC bits each */
  Module->ChBusWidth.Field.NumChannels = 1; /* two subchannels */
  Mfg->ModuleMfgId.Value = 0x2c00;
  Mfg->ModuleSerialNumber.String[0] = 0x12 + Socket;
  Mfg->ModuleSerialNumber.String[1] = Channel;
  Mfg->ModuleSerialNumber.String[2] = Slot;
  Mfg->ModuleSerialNumber.String[3] = 0x34;
  memcpy (Mfg->ModulePartNumber.String, "TITANITE-DDR5-TEST", 18);
  Header ()->ApobTypeHeader.TypeSize = sizeof (*Header ()) +
    Header ()->MaxPhysicalDimms * sizeof (*Dimm);
}

static void
Reset (void)
{
  memset (&mRaw, 0, sizeof (mRaw));
  memset (mSpd, 0, sizeof (mSpd));
  mMemApi = (MEM_IP2IP_API) {.GetSmbiosMemInfo = GetSmbiosTable};
  mMemC2r = (MEM_COMMON_2_REV_XFER_BLOCK) {
    .PopulateSmbiosMemInfo = PopulateSmbiosMemInfoBrh,
    .GetChannelXlatTable = GetChannelXlatTableBrh,
    .ConfigureTable17DimmPresent = ConfigureTable17DimmPresentBrh
  };
  mApobApi = (APOB_IP2IP_API) {
    .ApobAmdGetApobEntryInstance = AmdGetApobEntryInstance,
    .ApobGetMaxDieInfo = GetCaps,
    .ApobGetDimmSpdData = ApobGetDimmSpdDataBrh
  };
  mLookupStatus = mSpdStatus[0] = mSpdStatus[1] = mC2rStatus = SilPass;
  mFailApi = mNullApi = -1;
  mNullC2r = mNullApob = mNullSpd = false;
  mMaxDies = 1;
  for (unsigned Socket = 0; Socket < 2; Socket++) {
    mSpd[Socket].ApobTypeHeader.TypeSize = sizeof (mSpd[Socket]);
  }
  Header ()->EccCapable = 1;
  AddDimm (0, 3, 0, true); /* raw channel 3 is board A */
}

static void
Run (SIL_STATUS Expected)
{
  const SIL_DMI_INFO Zero = {0};
  mCases++;
  memset (&mOutput, 0xa5, sizeof (mOutput));
  mOutput.Before = mOutput.After = GUARD;
  CHECK (xPrfGetSmbiosMemInfo (&mOutput.Dmi) == Expected);
  CHECK (mOutput.Before == GUARD && mOutput.After == GUARD);
  if (Expected != SilPass) {
    CHECK (memcmp (&mOutput.Dmi, &Zero, sizeof (Zero)) == 0);
  }
}

static void
TestTopology (void)
{
  static const unsigned Translation[12] = {2, 4, 5, 0, 1, 3, 8, 10, 11, 6, 7, 9};
  Reset ();
  Run (SilPass);
  CHECK (mOutput.Dmi.T16.NumberOfMemoryDevices == 1);
  CHECK (mOutput.Dmi.T17[0][0][0].ExtSize == 65536);
  CHECK (mOutput.Dmi.T17[0][0][0].DataWidth == 64);
  CHECK (mOutput.Dmi.T17[0][0][0].TotalWidth == 80);
  CHECK (mOutput.Dmi.T17[0][0][0].Speed == 2400);
  CHECK (mOutput.Dmi.T17[0][0][0].ConfigSpeed == 2400);
  CHECK (strcmp (mOutput.Dmi.T17[0][0][0].SerialNumber, "12030034") == 0);
  CHECK (strcmp (mOutput.Dmi.T17[0][0][0].BankLocator, "P0 CHANNEL A") == 0);

  Reset ();
  Header ()->MaxPhysicalDimms = 0;
  for (unsigned Socket = 0; Socket < 2; Socket++) {
    for (unsigned Channel = 0; Channel < 12; Channel++) {
      AddDimm (Socket, Channel, 0, true);
      if (Channel == 3 || Channel == 9) {
        AddDimm (Socket, Channel, 1, true);
      }
    }
  }
  Run (SilPass);
  CHECK (mOutput.Dmi.T16.NumberOfMemoryDevices == 28);
  for (unsigned Socket = 0; Socket < 2; Socket++) {
    for (unsigned Channel = 0; Channel < 12; Channel++) {
      SIL_TYPE17_DMI_INFO *Dimm = &mOutput.Dmi.T17[Socket][Translation[Channel]][0];
      CHECK (Dimm->MemorySize == 0x7fff && Dimm->ExtSize == 65536);
      CHECK (Dimm->BankLocator[1] == (char)('0' + Socket));
      CHECK (Dimm->BankLocator[11] == (char)('A' + Translation[Channel]));
      CHECK (Dimm[1].MemorySize == ((Channel == 3 || Channel == 9) ? 0x7fff : 0));
    }
  }
  Physical (0)->DimmPresent = false;
  Run (SilPass);
  CHECK (mOutput.Dmi.T16.NumberOfMemoryDevices == 28);
  CHECK (mOutput.Dmi.T17[0][2][0].MemorySize == 0);
  CHECK (mOutput.Dmi.T17[0][2][0].MemoryType == SilUnknownMemType);
}

static void
TestFailures (void)
{
  Reset ();
  CHECK (xPrfGetSmbiosMemInfo (NULL) == SilInvalidParameter);
  for (unsigned Which = 0; Which < 2; Which++) {
    Reset ();
    mFailApi = Which ? SilId_ApobClass : SilId_MemClass;
    Run (SilNotFound);
    mNullApi = mFailApi;
    mFailApi = -1;
    Run (SilNotFound);
  }
  for (unsigned Which = 0; Which < 8; Which++) {
    Reset ();
    switch (Which) {
    case 0: mMemApi.GetSmbiosMemInfo = NULL; break;
    case 1: mMemC2r.PopulateSmbiosMemInfo = NULL; break;
    case 2: mMemC2r.GetChannelXlatTable = NULL; break;
    case 3: mMemC2r.ConfigureTable17DimmPresent = NULL; break;
    case 4: mApobApi.ApobAmdGetApobEntryInstance = NULL; break;
    case 5: mApobApi.ApobGetMaxDieInfo = NULL; break;
    case 6: mApobApi.ApobGetDimmSpdData = NULL; break;
    case 7: mNullC2r = true; break;
    }
    Run (SilNotFound);
  }
  Reset (); mC2rStatus = SilAborted; Run (SilAborted);
  Reset (); mLookupStatus = SilNotFound; Run (SilNotFound);
  Reset (); mNullApob = true; Run (SilNotFound);
  Reset (); Header ()->ApobTypeHeader.TypeSize = sizeof (*Header ()) - 1; Run (SilOutOfBounds);
  Reset (); Header ()->ApobTypeHeader.TypeSize--; Run (SilOutOfBounds);
  Reset (); Header ()->MaxPhysicalDimms = 49; Run (SilOutOfBounds);
  Reset (); Header ()->MaxLogicalDimms = 1; Run (SilOutOfBounds);
  Reset (); Physical (0)->Socket = 2; Run (SilOutOfBounds);
  Reset (); Physical (0)->Channel = 15; Run (SilOutOfBounds);
  Reset (); AddDimm (0, 3, 0, true); Run (SilOutOfBounds);
  Reset (); mMaxDies = 0; Run (SilOutOfBounds);
  Reset (); mMaxDies = 2; Run (SilOutOfBounds);
  Reset (); mSpdStatus[0] = SilNotFound; Run (SilNotFound);
  Reset (); mNullSpd = true; Run (SilNotFound);
  Reset (); AddDimm (1, 9, 1, true); mSpdStatus[1] = SilAborted; Run (SilAborted);
  Reset (); mSpd[0].DimmSmbusInfo[6].DimmPresent = false; Run (SilNotFound);
  Reset (); mSpd[0].ApobTypeHeader.TypeSize = 0; Run (SilOutOfBounds);
  Reset (); mSpd[0].ApobTypeHeader.TypeSize = sizeof (mSpd[0]) + 1; Run (SilOutOfBounds);
  Reset (); Physical (0)->DimmPresent = false; Run (SilNotFound);
}

static HOST_TO_APCB_CHANNEL_XLAT *
InvalidChannelMap (void)
{
  static HOST_TO_APCB_CHANNEL_XLAT Map[] = {{3, 12}, {0xff, 0xff}};
  return Map;
}

static void
TestLogicalBounds (void)
{
  APOB_MEM_DMI_LOGICAL_DIMM_BRH Data;
  APOB_MEM_DMI_LOGICAL_DIMM_BRH *Logical = &Data;
  for (unsigned Which = 0; Which < 5; Which++) {
    Reset ();
    Header ()->MaxLogicalDimms = 1;
    Header ()->ApobTypeHeader.TypeSize += sizeof (*Logical);
    *Logical = (APOB_MEM_DMI_LOGICAL_DIMM_BRH) {.Channel = 3, .DimmPresent = true};
    switch (Which) {
    case 0: Logical->Socket = 2; break;
    case 1: Logical->Channel = 15; break;
    case 2: Logical->Dimm = 1; break; /* no matching physical connector */
    case 3: Logical->DimmPresent = false; Logical->Channel = 15; break;
    }
    memcpy (Physical (1), Logical, sizeof (*Logical));
    Run (Which < 3 ? SilOutOfBounds : SilPass);
  }
  Reset ();
  mMemC2r.GetChannelXlatTable = InvalidChannelMap;
  Run (SilOutOfBounds);
}

static void
TestGeometry (void)
{
  SPD_BASE_CONFIG_0_S *Base;
  SPD_ANNEX_COMMON_S *Module;
  for (unsigned Which = 0; Which < 8; Which++) {
    Reset ();
    Base = (SPD_BASE_CONFIG_0_S *)mSpd[0].DimmSmbusInfo[6].Data;
    Module = (SPD_ANNEX_COMMON_S *)&mSpd[0].DimmSmbusInfo[6].Data[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN];
    switch (Which) {
    case 0: Base->KeyByte1.Field.ModuleType = 0; break;
    case 1: Base->TckAvgMin.Value = 0; break;
    case 2: Base->FirstDensity.Field.Density = 0; break;
    case 3: Base->FirstDensity.Field.DiePerPkg = 1; break;
    case 4: Module->ChBusWidth.Field.NumChannels = 0; break;
    case 5: Module->ChBusWidth.Field.Width = 7; break;
    case 6: Module->ChBusWidth.Field.WidthExt = 3; break;
    case 7: Base->TckAvgMin.Value = 1; break;
    }
    Run (SilUnsupported);
  }
  Reset ();
  Module = (SPD_ANNEX_COMMON_S *)&mSpd[0].DimmSmbusInfo[6].Data[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN];
  Module->ModuleOrg.Field.RanksPerChannel = 3;
  Run (SilPass);
  CHECK (mOutput.Dmi.T17[0][0][0].ExtSize == 131072);
  CHECK (mOutput.Dmi.T17[0][0][0].Attributes == 4);
  Reset ();
  Base = (SPD_BASE_CONFIG_0_S *)mSpd[0].DimmSmbusInfo[6].Data;
  Module = (SPD_ANNEX_COMMON_S *)&mSpd[0].DimmSmbusInfo[6].Data[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN];
  Module->ModuleOrg.Field.RankMix = RankMixAsymmetrical;
  Base->SecondDensity.Field.Density = 2; /* second rank uses 8 Gb parts */
  Run (SilPass);
  CHECK (mOutput.Dmi.T17[0][0][0].ExtSize == 49152);
}

int
main (void)
{
  TestTopology ();
  TestFailures ();
  TestLogicalBounds ();
  TestGeometry ();
  printf ("Memory DMI: %u cases, %u checks, %u failures\n", mCases, mChecks, mFailures);
  return mFailures ? 1 : 0;
}
