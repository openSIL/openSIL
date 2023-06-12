;
; @file         CpuOps32.nasm
; @brief        Various CPU functions executed in 32-bit mode
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

%include "Porting.h"

global ASM_TAG(xUslGetRawIdOnExecutingCore)
global ASM_TAG(xUslGetThreadsPerCore)
global ASM_TAG(xUslGetPackageType)
global ASM_TAG(xUslIsSmtDisabled)
global ASM_TAG(xUslGetProcessorId)
global ASM_TAG(xUslGetInitialApicId)
global ASM_TAG(xUslGetPhysAddrSize)
global ASM_TAG(xUslGetPhysAddrReduction)
global ASM_TAG(xUslRdMsr)
global ASM_TAG(xUslWrMsr)
global ASM_TAG(xUslWbinvd)
global ASM_TAG(xUslGetSecureEncryption)

    SECTION .text
    bits 32

;------------------------------------------------------------------------------
; xUslGetRawIdOnExecutingCore
;
; @brief    Get Raw CPU ID for the executing core
;
; @details  Uses CPUID function 80000001
;           ("Extended Processor and Processor Feature Identifiers").
;           CommonLib/CpuLib.h: uint32_t xUslGetRawIdOnExecutingCore (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslGetRawIdOnExecutingCore):
    push    ebx
    mov     eax, 0x80000001
    cpuid
    pop     ebx
    ret

;------------------------------------------------------------------------------
; xUslIsSmtDisabled
;
; @brief    Check if SMT is disabled
;
; @details  Uses CPUID function 0x8000001D
;           ("Cache Topology Information").
;           CommonLib/CpuLib.h: uint32_t xUslIsSmtDisabled (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslIsSmtDisabled):
    push    ebp
    mov     ebp, esp
    push    ebx
    xor     eax, eax
    mov     eax, 0x8000001D
    cpuid
    shr     eax, 14
    and     eax, 0xFFF
    pop     ebx
    leave
    ret

;------------------------------------------------------------------------------
; xUslGetProcessorId
;
; @brief    Get Processor ID
;
; @details  Uses CPUID function 80000001
;           ("Extended Processor and Processor Feature Identifiers").
;           CommonLib/CpuLib.h: uint32_t xUslGetProcessorId (void)
;
; @param    None
;
; @retval   EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslGetProcessorId):
    push    ebx
    mov     eax, 0x80000001
    cpuid
    mov     ebx, eax
    and     ebx, 0x0FFF0000
    shr     ebx, 8

    and     eax, 0x000000FF
    or      eax, ebx
    pop     ebx
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
ASM_TAG(xUslGetThreadsPerCore):
    push    ebp
    mov     ebp, esp
    push    ebx
    mov     eax, 0x8000001E
    cpuid
    mov     eax, ebx
    shr     eax, 8
    and     eax, 0xFF
    add     eax, 1
    pop     ebx
    leave
    ret

;------------------------------------------------------------------------------
; xUslGetPackageType
;
; @brief    Get package type from CPUID Fn0x80000001_EBX
;
; @details  Uses CPUID function 80000001
;           ("Extended Processor and Processor Feature Identifiers").
;           Bitfield EBX[31:28] is "PkgType" per the
;           AMD64 Architecture Programmers Manual Vol 3.
;           CommonLib/CpuLib.h: uint32_t xUslGetPackageType (void)
;
; @param    None
;
; @retval   Package type in EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslGetPackageType):
    push    ebp
    mov     ebp, esp
    push    ebx
    mov     eax, 0x80000001
    cpuid
    mov     ecx, ebx
    shr     ecx, 28
    and     ecx, 0xF
    mov     eax, 1
    shl     eax, cl
    pop     ebx
    leave
    ret


;------------------------------------------------------------------------------
; xUslGetInitialApicId
;
; @brief    Get package type from CPUID Fn0x00000001_EBX
;
; @details  Uses CPUID function 00000001
;           ("Processor and Processor Feature Identifiers").
;           Bitfield EBX[31:24] is "LocalApicId" per the AMD64
;           Architecture Programmers Manual Vol. 3.
;           CommonLib/CpuLib.h: uint32_t xUslGetInitialApicId (void)
;
; @param    None
;
; @retval   APIC Id in EAX
;------------------------------------------------------------------------------
ASM_TAG(xUslGetInitialApicId):
    push    ebp
    mov     ebp, esp
    push    ebx
    mov     eax, 0x00000001
    cpuid
    mov     eax, ebx
    shr     eax, 24
    and     eax, 0xFF
    pop     ebx
    leave
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
ASM_TAG(xUslGetPhysAddrSize):
    push    ebx
    mov     eax, 0x80000008
    cpuid
    pop     ebx
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
ASM_TAG(xUslGetPhysAddrReduction):
    push    ebx
    mov     eax, 0x8000001F
    cpuid
    mov     eax, ebx
    shr     eax, 6
    and     eax, 0x3F
    pop     ebx
    ret

ASM_TAG(xUslWbinvd):
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
ASM_TAG(xUslGetSecureEncryption):
    push    ebx
    mov     eax, 0x8000001F
    cpuid
    pop     ebx
    ret
