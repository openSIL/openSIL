; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file         CpuOps32.nasm
; @brief        Various CPU functions executed in 32-bit mode
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

%include "Porting.h"

global ASM_TAG(xUslCpuId)
global ASM_TAG(xUslRdMsr)
global ASM_TAG(xUslWrMsr)
global ASM_TAG(xUslWbinvd)
global ASM_TAG(xUslWriteCr0)
global ASM_TAG(xUslWriteCr3)
global ASM_TAG(xUslReadCr0)
global ASM_TAG(xUslReadCr3)
global ASM_TAG(xUslCountTrailingZeroes)

    SECTION .text
    bits 32
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
;   EaxIn   --> [ebp + 12]
;   ExtId   --> [ebp + 16]
;   *CpuReg --> [ebp + 20]
;
;   Preserved: ebx
;------------------------------------------------------------------------------
ASM_TAG(xUslCpuId):
    push    ebx
    push    ebp
    mov     ebp, esp
    mov     eax, [ebp + 12]
    mov     ecx, [ebp + 16]
    cpuid
    push    eax
    mov     eax, [ebp + 20]
    pop     DWORD [eax]     ; EAX
    mov     [eax + 4], ebx  ; EBX
    mov     [eax + 8], ecx  ; ECX
    mov     [eax + 12], edx ; EDX
    mov     esp, ebp
    pop     ebp
    pop     ebx
    ret

;------------------------------------------------------------------------------
; xUslRdMsr
;
; @brief    Read MSR
;
; @details  Read the MsrAddress specified by ECX and return the 64 bit value
;           in EDX:EAX.
;           CommonLib/CpuLib.h: uint64_t xUslRdMsr(uint32_t MsrAddress)
;
; @param    MsrAddress MSR Address passed in [ESP + 4]
;
; @retval   MSR Register Value in EDX:EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslRdMsr):
    mov     ecx, [esp + 4]
    rdmsr
    ret

;------------------------------------------------------------------------------
; xUslWrMsr
;
; @brief    Write MSR
;
; @details  Function writes 64 bit MsrValue specified by EDX:EAX to MsrAddress
;           specified by ECX.
;           CommonLib/CpuLib.h: void xUslWrMsr(uint32_t MsrAddress, uint64_t MsrValue)
;
; @param    MsrAddress  MSR Address passed as [esp + 4]
; @param    MsrValue    MSR Register Value passed as [esp + 8] to be written at MsrAddress
;
; @retval   None
;------------------------------------------------------------------------------
ASM_TAG(xUslWrMsr):
    mov     ecx, [esp + 4]
    mov     eax, [esp + 8]
    mov     edx, [esp + 12]
    wrmsr
    ret

ASM_TAG(xUslWbinvd):
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
ASM_TAG(xUslWriteCr0):
    mov     eax, [esp +4] ; Cr0 Data
    mov     cr0, eax
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
ASM_TAG(xUslWriteCr3):
    mov     eax, [esp +4] ; Cr3 Data
    mov     cr3, eax
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
ASM_TAG(xUslReadCr0):
    mov     eax, cr0
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
ASM_TAG(xUslReadCr3):
    mov     eax, cr3
    ret

;------------------------------------------------------------------------------
; xUslCountTrailingZeroes
;
; @brief    Count number of trailing zeroes in number
;
; @details  Utilizes the bsf (bit-scan forward) instruction to
;           return the index of the first nonzero bit in passed number
;
; @param    Number on which to perform operation passed in [ESP + 4]
;
; @retval   Index of first nonzero bit in passed number returned in EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslCountTrailingZeroes):
    mov     eax, [esp + 4]
    bsf     eax, eax
    ret
