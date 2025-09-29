; SPDX-License-Identifier: MIT
;
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file ApAsm64.nasm
; @brief 64-bit AP startup code.
; @details
;

SECTION .text

BspMsrLocationOffset                    EQU 0
ApSyncCountOffset                       EQU 4
AllowToLaunchNextThreadLocationOffset   EQU 8
ApStackBasePtrOffset                    EQU 0Ch
ApGdtDescriptorOffset                   EQU 10h

;!!! If below size is changed, please update the same definition in Ccx.h and x32/ApAsm.nasm.
AP_STACK_SIZE                           EQU 400h

LINEAR_CODE64_SEL                       EQU 38h

extern RegSettingBeforeLaunchingNextThread
extern ApEntryPointInC

global ApAsmCode

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
ApAsmCode:
  ;TpCcxApLaunchApAsmCode
  mov   eax, 0B000ACB1h
  out   80h, eax

  ; NOTE: EDI points to ApLaunchGlobalData

  ; Reset rsp
  ; Use only 1 AP stack, later increment of AllowToLaunchNextThreadLocationOffset
  ; needs to be done after finishing stack usage of current AP thread
  mov   rax, AP_STACK_SIZE

  ; Make space for the first qword
  sub   rax, 8

  xor   rsi, rsi
  mov   esi, [rdi + ApStackBasePtrOffset]
  add   rax, rsi
  mov   rsp, rax
  and   rsp, 0fffffff0h     ; Make sure 8 bype aligned.

  ; Enable Fixed MTRR modification
  mov   ecx, 0C0010010h
  rdmsr
  or    eax, 00080000h
  wrmsr

  ; Setup MSRs to BSP values
  mov   esi, [rdi + BspMsrLocationOffset]
MsrStart:
  mov   ecx, [rsi]
  cmp   ecx, 0FFFFFFFFh
  jz    MsrDone
  add   rsi, 4
  mov   eax, [rsi]
  add   rsi, 4
  mov   edx, [rsi]
  wrmsr
  add   rsi, 4
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
  mov   rax, cr0
  btr   eax, 30
  btr   eax, 29
  mov   cr0, rax

  ; Call into C code before next thread is launched
  mov   rcx, rdi
  sub   rsp, 20h
  call  RegSettingBeforeLaunchingNextThread
  add   rsp, 20h

  ; Call into C code

  mov   rcx, rdi
  sub   rsp, 20h
  call  ApEntryPointInC
  add   rsp, 20h

  ; Set up resident GDT
  xor   rsi, rsi
  mov   esi, ApGdtDescriptorOffset
  add   rsi, rdi
  lgdt  [rsi]
  ; Use stack base as a long jump pointer buffer
  mov   esi, [rdi + ApStackBasePtrOffset]
  ; Update selector
  mov   WORD [rsi + 4], LINEAR_CODE64_SEL
  mov   rbx, NewGdtAddress
  mov   [rsi], ebx
  jmp   far [rsi]
NewGdtAddress:
  ; Increment call count to allow to launch next thread, after stack usage is done
  mov   esi, [rdi + AllowToLaunchNextThreadLocationOffset]
  lock  inc DWORD [rsi]

  ;TpCcxApLaunchIncCount
  mov   eax, 0B000ACB7h
  out   80h, eax
  ; Increment call count to indicate core is done running
  lock  inc DWORD [rdi + ApSyncCountOffset]

  ;TpCcxApLaunchEnterHltLoop
  mov   eax, 0B000ACB8h
  out   80h, eax
  ; Hlt
Hlt_loop:
  cli
  hlt
  jmp   Hlt_loop
