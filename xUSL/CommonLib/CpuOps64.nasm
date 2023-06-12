; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file         CpuOps64.nasm
; @brief        Various CPU functions executed in 64-bit mode
;
;
;
; Copyright (c) 2006 - 2008, Intel Corporation.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
; 1. Redistributions of source code must retain the above copyright notice, this
; list of conditions and the following disclaimer.
;
; 2. Redistributions in binary form must reproduce the above copyright notice,
; this list of conditions and the following disclaimer in the documentation
; and/or other materials provided with the distribution.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
; CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUslCpuId
global xUslRdMsr
global xUslWrMsr
global xUslWbinvd
global xUslWriteCr0
global xUslWriteCr3
global xUslReadCr0
global xUslReadCr3
global xUslCountTrailingZeroes

    SECTION .text
    bits 64

;------------------------------------------------------------------------------
; xUslCpuId
;
; @brief  Read CPU Id data for the function specified in EaxIn
;
; @param EaxIn    The CPU Id command to execute
; @param ExtId    Option sub index of the CPU Id command
; @param CpuRegs  A pointer to a data block containing 4 DWORDS used to return
;                 the 4 registers values (EAX, EBX, ECX, EDX)
;
;   void xUslCpuId (uint32_t EaxIn,
;                  uint32_t ExtId,
;                  CPUID_DATA *CpuRegs)
;
;   EaxIn   --> ecx
;   ExtId   --> rdx
;   *CpuReg --> r8
;
;   Preserved: rbx
;------------------------------------------------------------------------------
xUslCpuId:
    push    rbx
    mov     eax, ecx
    mov     rcx, rdx
    cpuid
    mov     [r8], eax       ; EAX
    mov     [r8 + 4], ebx   ; EBX
    mov     [r8 + 8], ecx   ; ECX
    mov     [r8 + 12], edx  ; EDX
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslRdMsr
;
; @brief    Read MSR
;
; @details  Read the MSR specified by ECX and return the 64 bit value.  The
;           value returned in RAX is copied from EDX:EAX.
;           CommonLib/CpuLib.h: uint64_t xUslRdMsr(uint32_t MsrAddress)
;
; @param    MsrAddress MSR Address passed in ECX
;
; @retval   MSR Register Value in RAX which is copied from EDX:EAX
;------------------------------------------------------------------------------
xUslRdMsr:
    rdmsr
    and     rax, 0ffffffffh
    shl     rdx, 32
    or      rax, rdx
    ret

;------------------------------------------------------------------------------
; xUslWrMsr
;
; @brief    Write MSR
;
; @details  Function converts RDX (which is MsrValue to be written at MsrAddress)
;           into EDX:EAX and writes EDX:EAX in 64-bit MSR specified by ECX.
;           CommonLib/CpuLib.h: void xUslWrMsr(uint32_t MsrAddress, uint64_t MsrValue)
;
; @param    MsrAddress  MSR Address passed as ECX
; @param    MsrValue    MSR Register Value passed as RDX to be written at MsrAddress
;
; @retval   None
;------------------------------------------------------------------------------
xUslWrMsr:
    mov     rax, rdx
    and     rax, 0ffffffffh
    shr     rdx, 32
    wrmsr
    ret

xUslWbinvd:
    wbinvd
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslWriteCr0 (uintptr_t  Cr0);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; @brief    Write CR0
;
; @details  Function writes CR0 register by input value Cr0
;
; @param    Cr0  CR0 Data
;
; @retval   None
;------------------------------------------------------------------------------
xUslWriteCr0:
    mov     rax, rcx ; Cr0 Data
    mov     cr0, rax
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslWriteCr3 (uintptr_t  Cr3);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; @brief    Write CR3
;
; @details  Function writes CR3 register by input value Cr3
;
; @param    Cr3  CR3 Data
;
; @retval   None
;------------------------------------------------------------------------------
xUslWriteCr3:
    mov     rax, rcx ; Cr3 Data
    mov     cr3, rax
    ret

;------------------------------------------------------------------------------
; NASM_ABI uintptr_t
; xUslReadCr0 (void);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; @brief    Read Cr0
;
; @details  Function Read CR0 register
;
; @retval   uintptr_t  Return CR0 register value
;------------------------------------------------------------------------------
xUslReadCr0:
    mov     rax, cr0
    ret

;------------------------------------------------------------------------------
; NASM_ABI uintptr_t
; xUslReadCr3 (void);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; @brief    Read Cr3
;
; @details  Function Read CR3 register
;
; @retval   uintptr_t  Return CR3 register value
;------------------------------------------------------------------------------
xUslReadCr3:
    mov     rax, cr3
    ret

;------------------------------------------------------------------------------
; xUslCountTrailingZeroes
;
; @brief    Count number of trailing zeroes in number
;
; @details  Utilizes the bsf (bit-scan forward) instruction to
;           return the index of the first nonzero bit in passed number
;
; @param    Number on which to perform operation passed in ECX
;
; @retval   Index of first nonzero bit in passed number returned in EAX
;------------------------------------------------------------------------------
xUslCountTrailingZeroes:
    bsf     eax, ecx
    ret
