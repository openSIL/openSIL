; SPDX-License-Identifier: MIT
;
; Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file ApStartup32.nasm
; @brief 32-bit AP startup code.
; @details
;

%include "Porting.h"

SECTION .text

%define LINEAR_CODE_SEL 10h
%define SYS_DATA_SEL    18h

ALIGN   16

global ASM_TAG(gApStartupCode)
global ASM_TAG(gPatchProtectedModeJump)
global ASM_TAG(gPatchApEntryInCOffset)
global ASM_TAG(gPatchApLaunchGlobalData)
global ASM_TAG(gApStartupCodeEnd)

BITS 16
ASM_TAG(gApStartupCode):
  mov eax, 0B000ACB0h          ; TpCcxApLauchStartupCode
  out 80h, eax
  mov si, 0FFF4h
o32  lgdt       cs:[si]
  mov        eax, cr0                    ; Get control register 0
  or         eax, 000000003h             ; Set PE bit (bit #0)
  mov        cr0, eax
  mov        eax, cr4
  or         eax, 00000600h
  mov        cr4, eax
  mov        ax,  SYS_DATA_SEL
  mov        ds,  ax
  mov        es,  ax
  mov        fs,  ax
  mov        gs,  ax
  mov        ss,  ax
  jmp        LINEAR_CODE_SEL:dword 0
ASM_TAG(gPatchProtectedModeJump):
BITS 32
  mov eax, 0
ASM_TAG(gPatchApEntryInCOffset):
  mov edi, 0
ASM_TAG(gPatchApLaunchGlobalData):
  jmp eax
ASM_TAG(gApStartupCodeEnd):
