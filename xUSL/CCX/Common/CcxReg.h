/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxReg.h
 * @brief openSIL Ccx register definitions
 *
 */

#pragma once

/**********************************************************************************************************************
 * variable declaration
 *
 */

/// AMD Secure Encryption EAX Register
typedef union {
  struct {
    uint32_t SME:1;                           ///< Secure Memory Encryption supported.
    uint32_t SEV:1;                           ///< Secure Encrypted Virtualization supported.
    uint32_t VmPgFlush:1;                     ///< Page Flush MSR availability
    uint32_t SevEs:1;                         ///< SEV Encrypted State supported.
    uint32_t SNP:1;                           ///< SEV Secure Nested Paging supported.
    uint32_t VMPL:1;                          ///< VM Permission Levels supported.
    uint32_t RMPQUERY:1;                      ///< The RMPQUERY instruction is supported.
    uint32_t VmplSSS:1;                       ///< VMPL Supervisor Shadow Stack supported.
    uint32_t SecureTsc:1;                     ///< Support for Secure TSC.
    uint32_t TscAuxVirtualization:1;          ///< Hardware virtualizes TSC_AUX.
    uint32_t CoherencyEnforced:1;             ///< Hardware enforces cache coherency.
    uint32_t Req64BitHypervisor:1;            ///< Require 64-Bit Hypervisor.
    uint32_t RestrictInjection:1;             ///<
    uint32_t AlternateInjection:1;            ///<
    uint32_t DebugStateSwap:1;                ///<
    uint32_t PreventHostIBS:1;                ///< Prevent host IBS for a SEV-ES guest.
    uint32_t VTE:1;                           ///< The Virtual Transparent Encryption feature
    uint32_t VmgexitParameter:1;              ///< VmgexitParameter is supported in SEV_FEATURES.
    uint32_t VirtualTomMsr:1;                 ///< Virtual TOM MSR is supported.
    uint32_t IbsVirtualization:1;             ///< IBS state virtualization is supported for SEV-ES guests via opt-in
                                              ///< control in SEV_FEATURES.
    uint32_t :4;                              ///< Reserved
    uint32_t VmsaRegProt:1;                   ///< VMSA Register Protection is supported
    uint32_t :7;                              ///< Reserved
  } Field;
  uint32_t Value;
} SECURE_ENCRYPTION_EAX;
