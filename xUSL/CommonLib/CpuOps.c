/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CpuOps.c
 * @brief Defines cpu specific common operations
 *
 */

#include <CpuLib.h>

/**
 * xUslGetThreadsPerCore
 *
 * @brief    Get the threads per core from CPUID Fn0x8000001E_EBX
 *
 * @details  Uses CPUID function 8000001E ("Processor Topology Information").
 *           Bitfield EBX[15:8] is "ThreadsPerComputeUnit" per the AMD64
 *           Architecture Manual Vol 3.
 *
 * @param    None
 *
 * @retval   Threads per Core
 */
uint8_t
xUslGetThreadsPerCore (void)
{
  CPUID_DATA Regs = {0};

  xUslCpuId(0x8000001E, 0, &Regs);

  return (uint8_t) (((Regs.EbxReg >> 8) & 0xFF) + 1);
}

/**
 * xUslGetRawIdOnExecutingCore
 *
 * @brief    Get Raw CPU ID for the executing core
 *
 * @details  Uses CPUID function 80000001
 *           ("Extended Processor and Processor Feature Identifiers").
 *
 * @param    None
 *
 * @retval   The RAW CPU ID returned in EAX
 */
uint32_t
xUslGetRawIdOnExecutingCore (void)
{
  CPUID_DATA Regs = {0};

  xUslCpuId(0x80000001, 0, &Regs);

  return Regs.EaxReg;
}

/**
 * xUslIsSmtDisabled
 *
 * @brief Check is SMT is disabled
 *
 * @details CPUID_8000_001D_EAX[25:14] NumSharingCache
 *
 * @retval True if disabled
 *         False if enabled
 */
bool
xUslIsSmtDisabled (void)
{
  CPUID_DATA    Regs = {0};

  xUslCpuId(0x8000001D, 0, &Regs);

  return (((Regs.EaxReg >> 14) & 0xFFF) == 0);
}

/**
 * xUslGetProcessorId
 *
 * @brief    Get Processor ID
 *
 * @details  Uses CPUID function 80000001
 *           ("Extended Processor and Processor Feature Identifiers").
 *
 * @param    None
 *
 * @retval   The Processor ID
 */
uint16_t
xUslGetProcessorId (void)
{
  uint16_t    ProcessorId;
  CPUID_DATA  Regs = {0};

  // Read CPUID for family information
  xUslCpuId(0x80000001, 0, &Regs);

  // High byte contains extended model and extended family
  ProcessorId = (uint16_t)((Regs.EaxReg & (CPUID_EXT_MODEL_MASK | CPUID_EXT_FAMILY_MASK)) >> 8);
  // Low byte contains model and family
  ProcessorId |= (uint16_t)(Regs.EaxReg & (CPUID_STEPPING_MASK | CPUID_BASE_MODEL_MASK));
  return ProcessorId;
}

/**
 * xUslGetInitialApicId
 *
 * @brief    Get package type from CPUID Fn0x00000001_EBX
 *
 * @details  Uses CPUID function 00000001
 *           ("Processor and Processor Feature Identifiers").
 *           Bitfield EBX[31:24] is "LocalApicId" per the AMD64
 *           Architecture Programmers Manual Vol. 3.
 *
 * @param    None
 *
 * @retval   APIC Id in EAX
 */
uint32_t
xUslGetInitialApicId (void)
{
  CPUID_DATA  Regs = {0};

  // Read CPUID for family information
  xUslCpuId(0x00000001, 0, &Regs);

  return ((Regs.EbxReg >> 24) & 0xFF);
}

/**
 * xUslGetPhysAddrSize
 *
 * @brief Get maximum physical address size in bits.
 *
 * @details  CPUID_8000_0008_EAX[7:0] Maximum physical byte address size in bits
 *
 * @param    None
 *
 * @retval   Maximum physical address size in bits
 */
uint8_t
xUslGetPhysAddrSize (void)
{
  CPUID_DATA  Regs = {0};

  xUslCpuId(AMD_CPUID_ASIZE_PCCOUNT, 0, &Regs);

  return (uint8_t) (Regs.EaxReg & 0xFF);
}

/**
 * xUslGetPhysAddrReduction
 *
 * @brief Get physical address bit reduction
 *
 * @details  CPUID_8000_001F_EBX[11:6] MemEncryptPhysAddWidth
 *
 * @retval   Physical address bit reduction in bits
 */
uint8_t
xUslGetPhysAddrReduction (void)
{
  CPUID_DATA  Regs = {0};

  xUslCpuId(0x8000001F, 0, &Regs);

  return (uint8_t)((Regs.EbxReg >> 6) & 0x3F);
}

/**
 * xUslGetSecureEncryption
 *
 * @brief Get Secure Encryption Eax Value
 *
 * @details  Uses CPUID function CPUID_Fn8000001F_EAX ("AMD Secure Encryption EAX]").
 *
 * @retval Return AMD Secure Encryption Register Value
 */
uint32_t
xUslGetSecureEncryption (void)
{
  CPUID_DATA  Regs = {0};

  xUslCpuId(0x8000001F, 0, &Regs);

  return Regs.EaxReg;
}

/**
 * xUslGetCpuidCoreMaskWidth
 *
 * @brief Get the CoreMaskWidth value from CPUID
 *
 * @retval Return the field CoreMaskWidth
 */
uint8_t
xUslGetCpuidCoreMaskWidth (void)
{
  EXT_TOP_ENUM_EAX0 EaxReg;
  CPUID_DATA  Regs = {0};

  xUslCpuId(SIL_RESERVED_0006, 0, &Regs);
  EaxReg.Value = Regs.EaxReg;

  return (uint8_t)EaxReg.Bits.CoreMaskWidth;
}

/**
 * xUslGetCpuidExtCpuTopology
 *
 * @brief Get the extended CPU topology information for the given SubIndex value from CPUID
 *
 */
void
xUslGetCpuidExtCpuTopology (
  CPUID_DATA  *Regs,
  uint8_t     SubIndex
  )
{
  xUslCpuId(SIL_RESERVED_0005, SubIndex, Regs);
}

/**
 * xUslMsrOr
 * @brief Reads a 64-bit MSR, performs a bitwise OR, and writes the result
 * back to the 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise OR
 * between the read result and the value specified by OrData, and writes the
 * result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  OrData  The value to OR with the read value from the MSR.
 *
 * @return None
 *
 **/
void xUslMsrOr (uint32_t Index, uint64_t OrData)
{
  xUslWrMsr(Index, xUslRdMsr(Index) | OrData);
}

/**
 * xUslMsrAnd
 * @brief Reads a 64-bit MSR, performs a bitwise AND, and writes the result back to the
 * 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise AND between the
 * read result and writes the result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  AndData The value to AND with the read value from the MSR.
 *
 * @return None
 *
 **/
void xUslMsrAnd (uint32_t Index, uint64_t AndData)
{
  xUslWrMsr(Index, xUslRdMsr(Index) & AndData);
}

/**
 * xUslMsrAndThenOr
 * @brief Reads a 64-bit MSR, performs a bitwise AND followed by a bitwise
 * OR, and writes the result back to the 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise AND
 * between read result and the value specified by AndData, performs a bitwise OR
 * between the result of the AND operation and the value specified by OrData,
 * and writes the result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  AndData The value to AND with the read value from the MSR.
 * @param  OrData  The value to OR with the result of the AND operation.
 *
 * @return None.
 *
 **/
void
xUslMsrAndThenOr (
  uint32_t Index,
  uint64_t AndData,
  uint64_t OrData
  )
{
  xUslWrMsr(Index, (xUslRdMsr(Index) & AndData) | OrData);
}
