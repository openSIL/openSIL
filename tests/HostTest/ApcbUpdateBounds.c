/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file ApcbUpdateBounds.c
 * @brief Test the real APCB consumer with exact allocations and payload bounds.
 */
#include <stdio.h>
#include <stdlib.h>
#include <APOB/Common/ApobCmn.h>

/* Explicit verdicts remain active when provider headers define NDEBUG. */
#define CHECK(Condition) do { \
          if (!(Condition)) { \
            fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #Condition); \
            return 1; \
          } \
} while (0)

#define PREFIX offsetof(APOB_APCB_UPDATES_STRUCT, Item)
#define ITEM_BYTES sizeof (APCB_ITEM32)
#define SENTINEL UINT32_C(0xaabbccdd)

typedef struct {
  const char *Name;
  size_t Allocated;
  uint32_t Declared;
  uint32_t Count;
  uint32_t Token;
  SIL_STATUS Status;
  uint32_t Value;
  bool Duplicate;
} APCB_TEST_CASE;

static int
RunCase (
  const APCB_TEST_CASE *Test
  )
{
  APOB_APCB_UPDATES_STRUCT *Entry = calloc(1, Test->Allocated);
  CHECK(Entry != NULL);
  Entry->ApobTypeHeader.TypeSize = Test->Declared;
  if (Test->Allocated >= offsetof(APOB_APCB_UPDATES_STRUCT, NumItems) +
    sizeof (Entry->NumItems)) {
    Entry->NumItems = Test->Count;
  }
  if (Test->Allocated >= PREFIX) {
    size_t Capacity = (Test->Allocated - PREFIX) / ITEM_BYTES;
    for (size_t Index = 0; Index < Capacity && Index < MAX_APCB_UPDATES; ++Index) {
      Entry->Item[Index] = (APCB_ITEM32){Index, Index + 100};
    }
  }
  if (Test->Duplicate) {
    Entry->Item[1].Token = 0;
  }
  uint32_t Value = SENTINEL;
  SIL_STATUS Status = ApobGetApcbUpdate(Entry, Test->Token, &Value);
  free(Entry);
  printf("APCB update %s: status=%d value=%08x\n", Test->Name, Status, Value);
  CHECK(Status == Test->Status);
  CHECK(Value == Test->Value);
  return 0;
}

int
main (
  void
  )
{
  const APCB_TEST_CASE Cases[] = {
    {"zero-size", sizeof (APOB_TYPE_HEADER), 0, 0, 0, SilInvalidParameter, SENTINEL, false},
    {"header-only", sizeof (APOB_TYPE_HEADER), sizeof (APOB_TYPE_HEADER), 0, 0,
     SilInvalidParameter, SENTINEL, false},
    {"count-only", PREFIX - 4, PREFIX - 4, 0, 0, SilInvalidParameter, SENTINEL, false},
    {"short-prefix", PREFIX - 1, PREFIX - 1, 0, 0, SilInvalidParameter, SENTINEL, false},
    {"empty", PREFIX, PREFIX, 0, 0, SilNotFound, SENTINEL, false},
    {"missing-item", PREFIX, PREFIX, 1, 0, SilInvalidParameter, SENTINEL, false},
    {"partial-item", PREFIX + ITEM_BYTES - 1, PREFIX + ITEM_BYTES - 1, 1, 0,
     SilInvalidParameter, SENTINEL, false},
    {"one-item", PREFIX + ITEM_BYTES, PREFIX + ITEM_BYTES, 1, 0, SilPass, 100, false},
    {"absent", sizeof (APOB_APCB_UPDATES_STRUCT), sizeof (APOB_APCB_UPDATES_STRUCT),
     MAX_APCB_UPDATES, UINT32_MAX, SilNotFound, SENTINEL, false},
    {"maximum", sizeof (APOB_APCB_UPDATES_STRUCT), sizeof (APOB_APCB_UPDATES_STRUCT),
     MAX_APCB_UPDATES, MAX_APCB_UPDATES - 1, SilPass, MAX_APCB_UPDATES + 99, false},
    {"duplicate-last-wins", PREFIX + 2 * ITEM_BYTES, PREFIX + 2 * ITEM_BYTES,
     2, 0, SilPass, 101, true},
    {"declared-short-first-match", sizeof (APOB_APCB_UPDATES_STRUCT), PREFIX + ITEM_BYTES,
     2, 0, SilInvalidParameter, SENTINEL, false},
    {"declared-short-later-match", sizeof (APOB_APCB_UPDATES_STRUCT), PREFIX + ITEM_BYTES,
     2, 1, SilInvalidParameter, SENTINEL, false},
    {"count-overflow", sizeof (APOB_APCB_UPDATES_STRUCT), sizeof (APOB_APCB_UPDATES_STRUCT),
     MAX_APCB_UPDATES + 1, 0, SilInvalidParameter, SENTINEL, false},
    {"huge-count", sizeof (APOB_APCB_UPDATES_STRUCT), sizeof (APOB_APCB_UPDATES_STRUCT),
     UINT32_MAX, 0, SilInvalidParameter, SENTINEL, false},
    {"large-extent-invalid-count", sizeof (APOB_APCB_UPDATES_STRUCT), UINT32_MAX,
     MAX_APCB_UPDATES + 1, 0, SilInvalidParameter, SENTINEL, false},
  };
  for (size_t Index = 0; Index < sizeof (Cases) / sizeof (Cases[0]); ++Index) {
    CHECK(RunCase(&Cases[Index]) == 0);
  }

  uint32_t Value = SENTINEL;
  CHECK(ApobGetApcbUpdate(NULL, 0, &Value) == SilInvalidParameter);
  CHECK(Value == SENTINEL);
  APOB_APCB_UPDATES_STRUCT Entry = {0};
  Entry.ApobTypeHeader.TypeSize = sizeof (Entry);
  CHECK(ApobGetApcbUpdate(&Entry, 0, NULL) == SilInvalidParameter);
  Entry.NumItems = 1;
  CHECK(ApobGetApcbUpdate(&Entry, 0, NULL) == SilInvalidParameter);
  puts("APCB update bounds and valid lookup semantics: PASS");
  return 0;
}
