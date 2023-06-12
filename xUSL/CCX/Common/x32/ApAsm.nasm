; SPDX-License-Identifier: MIT
;
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file ApAsm32.nasm
; @brief 32-bit AP startup code.
; @details
;

%include "Porting.h"

SECTION .text

BspMsrLocationOffset                    EQU 0
ApSyncCountOffset                       EQU 4
AllowToLaunchNextThreadLocationOffset   EQU 8
ApStackBasePtrOffset                    EQU 0Ch
ApGdtDescriptorOffset                   EQU 10h

;!!! If below size is changed, please update the same definition in Ccx.h and x64/ApAsm.nasm.
AP_STACK_SIZE                           EQU 400h

LINEAR_CODE_SEL                         EQU 10h

extern ASM_TAG(RegSettingBeforeLaunchingNextThread)
extern ASM_TAG(ApEntryPointInC)

global ASM_TAG(ApAsmCode)

;------------------------------------------------------------------------------
; ApAsmCode
;
; @brief    AP startup code
;
; @details  ASM code executed by APs, called at end of ApStartupCode,
;           which syncs MSRs to BSP values, sets up GDT, calls
;           ApEntryPointInC, then allows the next AP to launch
;
;------------------------------------------------------------------------------
ASM_TAG(ApAsmCode):
  ;TpCcxApLauchApAsmCode
  mov   eax, 0B000ACB1h
  out   80h, eax

  ; NOTE: EDI points to ApLaunchGlobalData

  ; Reset ESP
  ; Use only 1 AP stack, later increment of AllowToLaunchNextThreadLocationOffset
  ; needs to be done after finishing stack usage of current AP thread
  mov   eax, AP_STACK_SIZE

  ; Make space for the first qword
  sub   eax, 8

  mov   esi, [edi + ApStackBasePtrOffset]
  add   eax, esi
  mov   esp, eax

  ; Enable Fixed MTRR modification
  mov   ecx, 0C0010010h
  rdmsr
  or    eax, 00080000h
  wrmsr

  ; Setup MSRs to BSP values
  mov   esi, [edi + BspMsrLocationOffset]
MsrStart:
  mov   ecx, [esi]
  cmp   ecx, 0FFFFFFFFh
  jz    MsrDone
  add   esi, 4
  mov   eax, [esi]
  add   esi, 4
  mov   edx, [esi]
  wrmsr
  add   esi, 4
  jmp   MsrStart

MsrDone:
  ; Disable Fixed MTRR modification and enable MTRRs
  mov   ecx, 0C0010010h
  rdmsr
  and   eax, 0FFF7FFFFh
  or    eax, 00140000h
  bt    eax, 21
  jnc   Tom2Disabled
  bts   eax, 22
Tom2Disabled:
  wrmsr

  ; Enable caching
  mov   eax, cr0
  btr   eax, 30
  btr   eax, 29
  mov   cr0, eax

  ; Call into C code before next thread is launched
  push  edi
  call  ASM_TAG(RegSettingBeforeLaunchingNextThread)
  pop   edi

  ; Call into C code
  push  edi
  call  ASM_TAG(ApEntryPointInC)
  pop   edi

  ; Set up resident GDT
  mov   esi, ApGdtDescriptorOffset
  add   esi, edi
  lgdt  [esi]
  ; Use stack base as a long jump pointer buffer
  mov   esi, [edi + ApStackBasePtrOffset]
  ; Update selector
  mov   WORD [esi + 4], LINEAR_CODE_SEL
  mov   ebx, NewGdtAddress
  mov   [esi], ebx
  jmp   far [esi]
NewGdtAddress:
  ; Increment call count to allow to launch next thread, after stack usage is done
  mov   esi, [edi + AllowToLaunchNextThreadLocationOffset]
  lock  inc DWORD [esi]

  ;TpCcxApLauchIncCount
  mov   eax, 0B000ACB7h
  out   80h, eax
  ; Increment call count to indicate core is done running
 lock   inc DWORD [edi + ApSyncCountOffset]

  ;TpCcxApLauchEnterHltLoop
  mov   eax, 0B000ACB8h
  out   80h, eax
  ; Hlt
Hlt_loop:
  cli
  hlt
  jmp Hlt_loop
