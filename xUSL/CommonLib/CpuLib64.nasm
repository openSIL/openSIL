; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file         CpuLib64.nasm
; @brief        Low level CPU operations
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

global xUslCpuSleep
global xUslDisableCache
global xUslEnableCache

    SECTION .text
    bits 64

;------------------------------------------------------------------------------
; NASM_ABI void
; void xUslCpuSleep(void)
;
; @brief Places the CPU in a sleep state until an interrupt is received. If
; interrupts are disabled prior to calling this function, then the CPU will be
; placed in a sleep state indefinitely.
;
; @expected users: For SIL internal use
;------------------------------------------------------------------------------
xUslCpuSleep:
    hlt
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslDisableCache (void);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;------------------------------------------------------------------------------
xUslDisableCache:
    mov     rax, cr0
    bts     rax, 30
    btr     rax, 29
    mov     cr0, rax
    wbinvd
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslEnableCache (void);
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;------------------------------------------------------------------------------
xUslEnableCache:
    wbinvd
    mov     rax, cr0
    btr     rax, 29
    btr     rax, 30
    mov     cr0, rax
    ret
