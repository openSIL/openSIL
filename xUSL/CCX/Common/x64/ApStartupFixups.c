/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApStartupFixups32.c
 * @brief AMD CCX Startup code fixups
 *
 */

#include <SilCommon.h>
#include "../Ccx.h"

extern ASSEMBLY_PATCH_LABEL  gApStartupCode;
extern ASSEMBLY_PATCH_LABEL  gPatchProtectedModeJump;
extern ASSEMBLY_PATCH_LABEL  gPatchPageTable;
extern ASSEMBLY_PATCH_LABEL  gPatchLongModeJump;
extern ASSEMBLY_PATCH_LABEL  gPatchApEntryInCOffset;
extern ASSEMBLY_PATCH_LABEL  gPatchApLaunchGlobalData;
extern ASSEMBLY_PATCH_LABEL  gApStartupCodeEnd;

/*
 *  Fix up startup assembly code to new location location
 *
 * @param[in] ApStartupMemoryMap  Structure to location of Startup Assembly code, GDT, etc.
 * @param[in] ApLaunchGlobalData  Pointer to Global Data used by AP Startup code.
 *
 */

void ApStartupFixups (AP_STARTUP_MEMORY_MAP *ApStartupMemoryMap, void *ApLaunchGlobalData)
{
  uint32_t  ApStartupCodeBase = ApStartupMemoryMap->ApStartupCodeBase;
  uint32_t CsBase = ApStartupCodeBase & 0xffff0000;
  uint32_t PatchProtModeJumpOffset = (uint32_t)((uintptr_t)gPatchProtectedModeJump - (uintptr_t)gApStartupCode);
  uint32_t PatchPageTableOffset = (uint32_t)((uintptr_t)gPatchPageTable - (uintptr_t)gApStartupCode);
  uint32_t PatchLongModeJumpOffset = (uint32_t)((uintptr_t)gPatchLongModeJump - (uintptr_t)gApStartupCode);
  uint32_t PatchApEntryOffset = (uint32_t)((uintptr_t)gPatchApEntryInCOffset - (uintptr_t)gApStartupCode);
  uint32_t PatchApLaunchGlobalDataOffset = (uint32_t)((uintptr_t)gPatchApLaunchGlobalData - (uintptr_t)gApStartupCode);
  uint32_t ApEntryInCOffset = (uint32_t)(uintptr_t)ApAsmCode;
  uint32_t ProtModeFarJmpOffset = (uint32_t)ApStartupCodeBase + PatchProtModeJumpOffset;
  uint32_t LongModeFarJmpOffset = (uint32_t)ApStartupCodeBase + PatchLongModeJumpOffset;

  PatchAssemblyInstruction((ASSEMBLY_PATCH_LABEL *)(uintptr_t)(ApStartupCodeBase + PatchProtModeJumpOffset - 2),
    ProtModeFarJmpOffset,
    sizeof (uint32_t)
    );
  PatchAssemblyInstruction((ASSEMBLY_PATCH_LABEL *)(uintptr_t)(ApStartupCodeBase + PatchPageTableOffset),
    CsBase + 0xffe8,
    sizeof (uint32_t)
    );
  PatchAssemblyInstruction((ASSEMBLY_PATCH_LABEL *)(uintptr_t)(ApStartupCodeBase + PatchLongModeJumpOffset - 2),
    LongModeFarJmpOffset,
    sizeof (uint32_t)
    );
  PatchAssemblyInstruction((ASSEMBLY_PATCH_LABEL *)(uintptr_t)(ApStartupCodeBase + PatchApEntryOffset),
    ApEntryInCOffset,
    sizeof (uint32_t)
    );
  PatchAssemblyInstruction((ASSEMBLY_PATCH_LABEL *)(uintptr_t)(ApStartupCodeBase + PatchApLaunchGlobalDataOffset),
    (uint32_t)(uintptr_t)ApLaunchGlobalData,
    sizeof (uint32_t)
    );
}
