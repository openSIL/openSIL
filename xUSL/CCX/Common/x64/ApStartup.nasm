; SPDX-License-Identifier: MIT
;
; Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file ApStartup64.nasm
; @brief 64-bit AP startup code.
; @details
;

SECTION .text

%define LINEAR_CODE_SEL   10h
%define SYS_DATA_SEL      18h
%define LINEAR_CODE64_SEL 38h

ALIGN 16

global gApStartupCode
global gPatchProtectedModeJump
global gPatchPageTable
global gPatchLongModeJump
global gPatchApEntryInCOffset
global gPatchApLaunchGlobalData
global gApStartupCodeEnd

BITS 16
gApStartupCode:
  mov eax, 0B000ACB0h                   ; TpCcxApLauchStartupCode
  out 80h, eax
  mov si, 0FFF4h
o32  lgdt    cs:[si]
  mov        eax, cr0
  or         eax, 3                     ; Set PE CR0[0] and MP CR0[1]
  mov        cr0, eax
  mov        eax, cr4
  or         eax, 600h                  ; Set OSFXSR CR4[9] and OSXMMEXCP CR4[10] to support XMM.
  mov        cr4, eax
  mov        ax,  SYS_DATA_SEL
  mov        ds,  ax
  mov        es,  ax
  mov        fs,  ax
  mov        gs,  ax
  mov        ss,  ax
  jmp        LINEAR_CODE_SEL:dword 0
gPatchProtectedModeJump:
BITS 32
  ;
  ; Long Mode setup
  ;
  mov         eax, cr4
  bts         eax, 5                     ; Set PAE CR4[5]
  mov         cr4, eax
  mov         ecx, 0C0000080h            ; Read EFER MSR
  rdmsr
  bts         eax, 8                     ; Set LME (bit #8)
  wrmsr

  mov          esi, 0                    ; mov esi cs_base + ffe8h
gPatchPageTable:
  mov         ecx, [esi]

  mov         cr3, ecx                   ; Load CR3 with value from BSP
  mov         eax, cr0
  bts         eax, 31                    ; Set PG bit CR0[31]
  mov         cr0, eax

  jmp         LINEAR_CODE64_SEL:dword 0
gPatchLongModeJump:
BITS 64
  xor         rdi, rdi
  mov         edi, strict dword 0
gPatchApLaunchGlobalData:

  xor         rax, rax
  mov         eax, strict dword 0
gPatchApEntryInCOffset:
  jmp         rax
gApStartupCodeEnd:
