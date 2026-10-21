/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file ApobParser.c
 * @brief Exercise the real APOB parser with bounded synthetic buffers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <APOB/Common/ApobCmn.h>

extern APOBLIB_INFO gApobLibInfo;

typedef enum {
  Unmodified,
  ShortBase,
  OffsetBeforeHeader,
  OffsetBeyondBuffer,
  UnalignedOffset,
  ZeroEntry,
  ShortEntry,
  LongEntry,
  UnalignedNextEntry,
  WrongGroup,
  DuplicateInstance,
  OldVersion
} PARSER_MUTATION;

typedef struct {
  const char      *Name;
  uint32_t         Count;
  uint32_t         TailBytes;
  uint32_t         Instance;
  PARSER_MUTATION  Mutation;
  SIL_STATUS       Status;
} PARSER_CASE;

/**
 * @brief Run one lookup and check its exact status and output contract.
 */
static int
RunCase (
  const PARSER_CASE *Test
  )
{
  size_t            Bytes;
  uint8_t          *Blob;
  APOB_BASE_HEADER *Header;
  APOB_TYPE_HEADER *Entry;
  APOB_TYPE_HEADER *Found;
  SIL_STATUS        Status;
  bool              Passed;

  Bytes = sizeof (APOB_BASE_HEADER) + Test->Count * sizeof (APOB_TYPE_HEADER) + Test->TailBytes;
  Blob = calloc(1, Bytes);
  if (Blob == NULL) {
    fputs("APOB parser allocation failed\n", stderr);
    return 1;
  }
  Header = (APOB_BASE_HEADER *) Blob;
  Header->Signature = APOB_SIGNATURE;
  Header->Version = 5;
  Header->Size = (uint32_t) Bytes;
  Header->OffsetOfFirstEntry = sizeof (*Header);
  for (uint32_t Index = 0; Index < Test->Count; Index++) {
    Entry = (APOB_TYPE_HEADER *) (Blob + sizeof (*Header) + Index * sizeof (*Entry));
    Entry->GroupID = Test->Mutation == WrongGroup ? 9 : 1;
    Entry->DataTypeID = 2;
    Entry->InstanceID = Test->Mutation == DuplicateInstance ? 0 : Index;
    Entry->TypeSize = sizeof (*Entry);
  }
  Entry = (APOB_TYPE_HEADER *) (Blob + sizeof (*Header));
  switch (Test->Mutation) {
  case ShortBase:
    Header->Size = sizeof (*Header) - 1;
    break;
  case OffsetBeforeHeader:
    Header->OffsetOfFirstEntry = sizeof (*Header) - 1;
    break;
  case OffsetBeyondBuffer:
    Header->OffsetOfFirstEntry = (uint32_t) Bytes + 1;
    break;
  case UnalignedOffset:
    Header->OffsetOfFirstEntry++;
    break;
  case ZeroEntry:
    Entry->TypeSize = 0;
    break;
  case ShortEntry:
    Entry->TypeSize = sizeof (*Entry) - 1;
    break;
  case LongEntry:
    Entry->TypeSize = (uint32_t) Bytes;
    break;
  case UnalignedNextEntry:
    Entry->TypeSize++;
    break;
  case OldVersion:
    Header->Version = 4;
    break;
  default:
    break;
  }

  /* Use the cached address so native allocations need not be below 4 GiB. */
  gApobLibInfo = (APOBLIB_INFO) {true, (uint32_t) Bytes, (uintptr_t) Blob};
  Found = (APOB_TYPE_HEADER *) (uintptr_t) 1;
  Status = AmdGetApobEntryInstance(1, 2, Test->Instance, 0, &Found);
  Passed = Status == Test->Status;
  if (Test->Status == SilPass) {
    Passed = Passed && Found == (APOB_TYPE_HEADER *)
      (Blob + sizeof (*Header) + Test->Instance * sizeof (*Entry));
  } else {
    Passed = Passed && Found == NULL;
  }
  gApobLibInfo = (APOBLIB_INFO) {0};
  free(Blob);
  printf("APOB parser %s: %s (status=%d)\n", Test->Name, Passed ? "PASS" : "FAIL", Status);
  return Passed ? 0 : 1;
}

/**
 * @brief Keep verdicts active even when provider headers disable assert.
 */
int
main (
  void
  )
{
  static const PARSER_CASE Cases[] = {
    {"valid", 1, 0, 0, Unmodified, SilPass},
    {"capacity", APOB_ENTRY_INSTANCE_MAX, 0, APOB_ENTRY_INSTANCE_MAX - 1, Unmodified, SilPass},
    {"overflow", APOB_ENTRY_INSTANCE_MAX + 1, 0, 0, Unmodified, SilOutOfResources},
    {"empty", 0, 0, 0, Unmodified, SilNotFound},
    {"absent-group", 1, 0, 0, WrongGroup, SilNotFound},
    {"absent-instance", 1, 0, 1, Unmodified, SilNotFound},
    {"duplicate-instance", 2, 0, 0, DuplicateInstance, SilUnsupported},
    {"short-base", 0, 0, 0, ShortBase, SilUnsupported},
    {"offset-before-header", 1, 0, 0, OffsetBeforeHeader, SilUnsupported},
    {"offset-beyond-buffer", 1, 0, 0, OffsetBeyondBuffer, SilUnsupported},
    {"unaligned-offset", 1, 1, 0, UnalignedOffset, SilUnsupported},
    {"zero-size", 1, 0, 0, ZeroEntry, SilUnsupported},
    {"short-entry", 1, 0, 0, ShortEntry, SilUnsupported},
    {"long-entry", 1, 0, 0, LongEntry, SilUnsupported},
    {"truncated-header", 0, 1, 0, Unmodified, SilUnsupported},
    {"malformed-after-match", 1, 1, 0, Unmodified, SilUnsupported},
    {"unaligned-next-entry", 2, 1, 0, UnalignedNextEntry, SilUnsupported},
    {"old-version", 1, 0, 0, OldVersion, SilUnsupported},
  };
  int Failed;

  Failed = 0;
  for (size_t Index = 0; Index < sizeof (Cases) / sizeof (Cases[0]); Index++) {
    Failed |= RunCase(&Cases[Index]);
  }
  if (AmdGetApobEntryInstance(1, 2, 0, 0, NULL) != SilInvalidParameter) {
    fputs("APOB parser null output: FAIL\n", stderr);
    Failed = 1;
  }
  return Failed;
}
