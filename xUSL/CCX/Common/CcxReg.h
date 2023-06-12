/**
 * @file CcxReg.h
 * @brief openSIL Ccx register definitions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/**********************************************************************************************************************
 * variable declaration
 *
 */

/* Register Name PMREG_INITPKG7 */
#define SMN_PMREG_INITPKG7_ADDRESS      0x20002FECUL
#define SMN_PMREG_INITPKG7_CDD_OFFSET   0x800000UL
#define SMN_PMREG_INITPKG7_CORE_OFFSET  0x20000UL
#define SMN_PMREG_INITPKG7_CCX_OFFSET   0x400000UL

/// AMD Secure Encryption EAX Register
typedef union {
  struct {
    uint32_t SME:1;                           ///< Secure Memory Encryption supported.
    uint32_t SEV:1;                           ///< Secure Encrypted Virtualization supported.
    uint32_t PageFlushMsr:1;                  ///< Page Flush MSR available
    uint32_t SevEs:1;                         ///< SEV Encrypted State supported.
    uint32_t SevSnp:1;                        ///< SEV Secure Nested Paging supported.
    uint32_t VMPL:1;                          ///< VM Permission Levels supported.
    uint32_t RMPQUERY:1;                      ///< RMPQUERY Instruction supported
    uint32_t VmplSSS:1;                       ///< VMPL Supervisor Shadow Stack supported.
    uint32_t SecureTsc:1;                     ///< Secure TSC supported.
    uint32_t TscAuxVirtualization:1;          ///< TSC AUX Virtualization supported.
    uint32_t HwEnfCacheCoh:1;                 ///< Hardware cache coherency across encryption domains enforced.
    uint32_t Host64Bit:1;                     ///< SEV guest execution only allowed from a 64-bit host.
    uint32_t RestrictInjection:1;             ///< Restricted Injection supported.
    uint32_t AlternateInjection:1;            ///< Alternate Injection supported.
    uint32_t DebugSwap:1;                     ///< Full debug state swap supported for SEV-ES guests.
    uint32_t PreventHostIbs:1;                ///< Disallowing IBS use by the host supported.
    uint32_t VTE:1;                           ///< Virtual Transparent Encryption supported.
    uint32_t VmgexitParameter:1;              ///< VMGEXIT Parameter supported.
    uint32_t VirtualTomMsr:1;                 ///< Virtual TOM MSR supported.
    uint32_t IbsVirtGuestCtl:1;               ///< IBS Virtualization supported for SEV-ES guests.
    uint32_t :4;                              ///< Reserved
    uint32_t VmsaRegProt:1;                   ///< VMSA Register Protection supported.
    uint32_t SmtProtection:1;                 ///< SMT Protection supported.
    uint32_t :2;                              ///< Reserved
    uint32_t SvsmCommPageMSR:1;               ///< SVSM Communication Page MSR (C001_F000h) is supported.
    uint32_t NestedVirtSnpMsr:1;              ///< VIRT_RMPUPDATE MSR (C001_F001h) and VIRT_PSMASH MSR
    uint32_t :2;                              ///< Reserved
  } Field;
  uint32_t Value;
} SECURE_ENCRYPTION_EAX;

/**********************************************************************************************************************
 * Module data structure
 *
 */
