;
; @file  Porting.h
; @brief
;  openSIL declarations. This file is used by all of the xSIM, xPRF  and
;  xUSL Nasm modules, but NOT intended for Host usage.
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;
;

%ifndef Porting_h
  %define Porting_h

;***************************************************************************
;*  Porting declarations
;***************************************************************************
;*  These account for differnces between the tool chains (GCC vs MSVC).
;*
;*  The ASM_TAG Modifier is to ensure that all Assembler functions (*.nasm) and
;*  intrinsics use the correct C calling convention.
;*
;*  CDECL - All protocol member functions and intrinsics are required
;*   to modify their member functions with CDECL. Each tool has their own
;*   methodolgy to make the declaration.
;*
  %ifidni __OUTPUT_FORMAT__, win32
    ;
    ; The Win32 environment is the special one, requiring a leading '_' on symbols
    ;  AND the MS compiler requires _EFIAPI usage, __cdecl
    ;
    %define CDECL __cdecl
    %define ASM_TAG(name) _ %+ name
  %elifidni __OUPUT_FORMAT__, win64
    ; MS 64bit still needs the cdecl, but not the symbol prefix
    %define CDECL __cdecl
    %define ASM_TAG(name) name
  %else
    ; elf formats
    %define CDECL __attribute__((cdecl))
    %define ASM_TAG(name) name
  %endif

%endif
