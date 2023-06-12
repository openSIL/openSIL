;
; @file         CpuOps64.nasm
; @brief        Various CPU functions executed in 64-bit mode
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUslGetRawIdOnExecutingCore
global xUslGetThreadsPerCore
global xUslGetPackageType
global xUslIsSmtDisabled
global xUslGetProcessorId
global xUslGetInitialApicId
global xUslGetPhysAddrSize
global xUslGetPhysAddrReduction
global xUslRdMsr
global xUslWrMsr
global xUslGetPhysAddrSize
global xUslGetPhysAddrReduction
global xUslWbinvd
global xUslGetSecureEncryption
global xUslReadCr3

    SECTION .text
    bits 64

;------------------------------------------------------------------------------
; xUslGetRawIdOnExecutingCore
;
; @brief    Get Raw CPU ID for the executing core
;
; @details  Uses CPUID function 80000001 ("Extended Processor and Processor
;           Feature Identifiers").
;           CommonLib/CpuLib.h: uint32_t xUslGetRawIdOnExecutingCore (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
xUslGetRawIdOnExecutingCore:
    push    rbx
    mov     eax, 0x80000001
    cpuid
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslGetThreadsPerCore
;
; @brief    Get the threads per core from CPUID Fn0x8000001E_EBX
;
; @details  Uses CPUID function 8000001E ("Processor Topology Information").
;           Bitfield EBX[15:8] is "ThreadsPerComputeUnit" per the AMD64
;           Architecture Manual Vol 3.
;           CommonLib/CpuLib.h: uint8_t xUslGetThreadsPerCore (void)
;
; @param    None
;
; @retval   Threads per Core in EAX
;------------------------------------------------------------------------------
xUslGetThreadsPerCore:
    push    rbx
    mov     eax, 0x8000001E
    cpuid
    xor     rax, rax
    mov     eax, ebx
    shr     eax, 8
    and     eax, 0xFF
    add     eax, 1
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslGetPackageType
;
; @brief    Get package type from CPUID Fn0x80000001_EBX
;
; @details  Uses CPUID function 80000001 ("Extended Processor and Processor
;           Feature Identifiers"). Bitfield EBX[31:28] is "PkgType" per the
;           AMD64 Architecture Programmer's Manual Vol 3.
;           CommonLib/CpuLib.h: uint32_t xUslGetPackageType (void)
;
; @param    None
;
; @retval   Package type in EAX
;------------------------------------------------------------------------------
xUslGetPackageType:
    push    rbx
    mov     eax, 0x80000001
    cpuid
    xor     rax, rax
    mov     ecx, ebx
    shr     ecx, 28
    and     ecx, 0xF
    mov     eax, 1
    shl     eax, cl
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslIsSmtDisabled
;
; @brief    Check if SMT is disabled
;
; @details  Uses CPUID function 0x8000001D ("Extended Processor and Processor
;           Feature Identifiers").
;           CommonLib/CpuLib.h: uint32_t xUslIsSmtDisabled (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
xUslIsSmtDisabled:
    push    rbx
    mov     eax, 0x8000001D
    cpuid
    shr     eax, 14
    and     eax, 0xFFF
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslGetProcessorId
;
; @brief    Get Processor ID
;
; @details  Uses CPUID function 80000001 ("Extended Processor and Processor
;           Feature Identifiers").
;           CommonLib/CpuLib.h: uint32_t xUslGetProcessorId (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
xUslGetProcessorId:
    push    rbx
    mov     eax, 0x80000001
    cpuid
    mov     ebx, eax
    and     ebx, 0x0FFF0000
    shr     ebx, 8

    and     eax, 0x000000FF
    or      eax, ebx
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslGetInitialApicId
;
; @brief    Get package type from CPUID Fn0x00000001_EBX
;
; @details  Uses CPUID function 00000001 ("Processor and Processor Feature
;           Identifiers").  Bitfield EBX[31:24] is "LocalApicId" per the AMD64
;           Architecture Programmer's Manual Vol. 3.
;           CommonLib/CpuLib.h: uint32_t xUslGetInitialApicId (void)
;
; @param    None
;
; @retval   APIC Id in EAX
;------------------------------------------------------------------------------
xUslGetInitialApicId:
    push    rbx
    mov     eax, 0x00000001
    cpuid
    xor     rax, rax
    mov     eax, ebx
    shr     eax, 24
    and     eax, 0xFF
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

;------------------------------------------------------------------------------
; xUslGetPhysAddrSize
;
; @brief    Get maximum physical address size in bits.
;
; @details  CPUID_8000_0008_EAX[7:0] Maximum physical byte address size in bits
;
; @param    None
;
; @retval   Maximum physical address size in bits in AL
;------------------------------------------------------------------------------
xUslGetPhysAddrSize:
    push    rbx
    mov     eax, 0x80000008
    cpuid
    pop     rbx
    ret

;------------------------------------------------------------------------------
; xUslGetPhysAddrReduction
;
; @brief    Get physical address bit reduction
;
; @details  CPUID_8000_001F_EBX[11:6] MemEncryptPhysAddWidth
;
; @param    None
;
; @retval   Physical address bit reduction in bits in AL
;------------------------------------------------------------------------------
xUslGetPhysAddrReduction:
    push    rbx
    mov     eax, 0x8000001F
    cpuid
    mov     eax, ebx
    shr     eax, 6
    and     eax, 0x3F
    pop     rbx
    ret

xUslWbinvd:
    wbinvd
    ret

;------------------------------------------------------------------------------
; xUslGetSecureEncryption
;
; @brief    Get Secure Encryption Eax Value
;
; @details  Uses CPUID function CPUID_Fn8000001F_EAX ("AMD Secure Encryption EAX]").
;           CommonLib/CpuLib.h: uint32_t xUslGetSecureEncryption (void)
;
; @param    None
;
; @retval   Return AMD Secure Encryption Register Value
;------------------------------------------------------------------------------
xUslGetSecureEncryption:
    push    rbx
    mov     eax, 0x8000001F
    cpuid
    pop     rbx
    ret

xUslReadCr3:
    mov     rax, cr3
    ret
