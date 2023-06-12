/**
 * @file
 * @brief OpenSIL RAS IP  specific initialization function declaration.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include <RAS/Common/Ras.h>

/*******************************************************************************
 * Declare macros here
 *
 */
/**
 * @brief The maximum number of PCIe root port in any PCIe x16 link - PCIe
 *        Core 0
 *
 * @details Maximum of 9 PCIe ports in any x16 link (G0, G1, G2, G3, P0, P1, P2,
 *          P3); a x16 link cannot be connected to more than 9 PCIe end points.
 *          PCIe Core 0 is located at P0 (NBIO 0/ IOHC 0/ PCIE0), G1 (NBIO 0/
 *          IOHC 1/ PCIE2), P2 (NBIO 1/ IOHC 0/ PCIE0), G3 (NBIO 1/ IOHC 1/
 *          PCIE2)
 */
#define MAX_PCIE_CORE0_PORT_SUPPORT     (9) //Max PCI-E port support for Core 0.

/**
 * @brief The maximum number of PCIe root port in any PCIe x16 link - PCIe
 *        Core 1
 *
 * @details Maximum of 9 PCIe ports in any x16 link (G0, G1, G2, G3, P0, P1, P2,
 *          P3); a x16 link cannot be connected to more than 9 PCIe end points.
 *          PCIe Core 1 is located at G0 (NBIO 0/ IOHC 0/ PCIE1), P1 (NBIO 0/
 *          IOHC 1/ PCIE3), G2 (NBIO 1/ IOHC 0/ PCIE1), P3 (NBIO 1/ IOHC 1/
 *          PCIE3)
 */
#define MAX_PCIE_CORE1_PORT_SUPPORT     (9) //Max PCI-E port support for Core 1.

 /**
 * Genoa - Number of IOD.
 *
 * Genoa microprocessor contains multiple core/cache complex dies (CCD) plus an I/O die (IOD).
 */
#define DIE_PER_SOCKET          (1)
 /**
 * Genoa - The total number of UMC on Genoa IOD
 *
 * Twelve Unified Memory Controllers (UMC), each supporting one
 *         DRAM channel
 */
#define UMC_PER_DIE             (12)
/**
 * Genoa - The number of UMC channel on an UMC
 *
 * Twelve Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define CHANNEL_PER_UMC         (1)
/**
 * @brief The total number of UMC channels on Genoa IOD
 *
 * @details Twelve Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define MAX_UMC_CHANNEL_PER_DIE (UMC_PER_DIE * CHANNEL_PER_UMC)


#define SIL_RESERVED_804                  0x50d88UL

// These are designed to map 1:1 to the hardware encodings, but
// any special case overlaps are moved to >0x20 and handled
// specifically in decodeIntLvNumChan()
 /**
 * @brief Interleave mode - 1 channel (no interleave)
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent stations across which
 *          addresses are interleaved.
 *
 */
#define INTERLEAVE_MODE_NONE              0x00
 /**
 * @brief Interleave mode - 2 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent stations across which
 *          addresses are interleaved.
 *
 */

#define SIL_RESERVED_796        0x07
#define SIL_RESERVED_797        0x08
#define SIL_RESERVED_800        0x1C
#define SIL_RESERVED_798        0x1D
#define SIL_RESERVED_799        0x1E
/**
 * @brief Definitions related to DF used in DfAddressTranslation
 *
 */

 /**
 * @brief The Data Fabric revision 4.
 *
 * @details Read F0x40 (FabricBlockInstanceCount). Bits 27:24 specify the MajorRevision,
 *          but this was added in DFX. Prior to DFX, this field is always zero.
 */
#define DF_TYPE_DFX                             4
 /**
 * @brief Genoa - D18F4x1B4 [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 8
 *
 * @details Genoa - DF::SystemFabricIdMask1[11:8] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the SocketId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF_SOCKET_ID_SHIFT_BITPOS_LO_DFX        8
 /**
 * @brief Genoa - D18F4x1B4 [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 11
 *
 * @details Genoa - DF::SystemFabricIdMask1[11:8] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the SocketId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF_SOCKET_ID_SHIFT_BITPOS_HI_DFX       11
 /**
 * @brief Genoa - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 0
 *
 * @details Genoa - DF::SystemFabricIdMask2[15:0] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF_DIE_ID_MASK_BITPOS_LO_DFX            0
 /**
 * @brief Genoa - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 15
 *
 * @details Genoa - DF::SystemFabricIdMask2[15:0] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF_DIE_ID_MASK_BITPOS_HI_DFX           15

 /**
 * @brief Genoa - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 16
 *
 * @details Genoa - DF::SystemFabricIdMask2[31:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding
 *          bit in the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF_SOCKET_ID_MASK_BITPOS_LO_DFX        16
 /**
 * @brief Genoa - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 31
 *
 * @details Genoa - DF::SystemFabricIdMask2[31:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding
 *          bit in the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF_SOCKET_ID_MASK_BITPOS_HI_DFX        31

// Note: no DF_DIE_ID_SHIFT_BITPOS_LO_DFX and DF_DIE_ID_SHIFT_BITPOS_HI_DFX.
// Use the DF_NODE_ID_SHIFT_BITPOS_ to determine the LSB of NodeID.
 /**
 * @brief Genoa - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 0
 *
 * @details Genoa - DF::SystemFabricIdMask1[3:0] NodeIdShft.
 *          NodeId shift amount for the system. Tells how many bits to shift the NodeId by to align it
 *          to bit 0. In effect, this field identifies the LSB of the NodeId.
 */
#define DF_NODE_ID_SHIFT_BITPOS_LO_DFX          0
 /**
 * @brief Genoa - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 3
 *
 * @details Genoa - DF::SystemFabricIdMask1[3:0] NodeIdShft.
 *          NodeId shift amount for the system. Tells how many bits to shift the NodeId by to align it
 *          to bit 0. In effect, this field identifies the LSB of the NodeId.
 */
#define DF_NODE_ID_SHIFT_BITPOS_HI_DFX          3
 /**
 * @brief Genoa - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 16
 *
 * @details Genoa - DF::SystemFabricIdMask0[31:16] NodeIdMask.
 *          NodeId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a node. NodeId bits must be contiguous.
 */
#define DF_NODE_ID_MASK_BITPOS_LO_DFX          16
 /**
 * @brief Genoa - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 31
 *
 * @details Genoa - DF::SystemFabricIdMask0[31:16] NodeIdMask.
 *          NodeId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a node. NodeId bits must be contiguous.
 *          Not all bits are present, but they are reserved in DFX and used in DFX
 */
#define DF_NODE_ID_MASK_BITPOS_HI_DFX          31
 /**
 * @brief Genoa - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 0
 *
 * @details Genoa - DF::SystemFabricIdMask0[15:0] CompIdMask.
 *          ComponentId Mask for the system. If a bit in this mask is set, then the
 *          corresponding bit in the FabricId is used to identify a component. ComponentId bits must be contiguous.
 */
#define DF_COMPONENT_ID_MASK_BITPOS_LO_DFX      0
 /**
 * @brief Genoa - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 15
 *
 * @details Genoa - DF::SystemFabricIdMask0[15:0] CompIdMask.
 *          ComponentId Mask for the system. If a bit in this mask is set, then the
 *          corresponding bit in the FabricId is used to identify a component. ComponentId bits must be contiguous.
 *          Not all bits are present, but they are reserved in DFX and used in DFX
 */
#define DF_COMPONENT_ID_MASK_BITPOS_HI_DFX     15
 /**
 * @brief Genoa - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 0
 *
 * @details Genoa - DF::FabricBlockInstanceCount[7:0] BlkInstCount.
 *          Provides the total number of instances of all the blocks in the Data Fabric.
 */
#define DF_BLOCK_INSTANCE_COUNT_BITPOS_LO       0
 /**
 * @brief Genoa - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 7
 *
 * @details Genoa - DF::FabricBlockInstanceCount[7:0] BlkInstCount.
 *          Provides the total number of instances of all the blocks in the Data Fabric.
 */
#define DF_BLOCK_INSTANCE_COUNT_BITPOS_HI       7
 /**
 * @brief Genoa - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 24
 *
 * @details Genoa - DF::FabricBlockInstanceCount[27:24] MajorRevision.
 *          Specifies the major revision of the Data Fabric hardware, MajorRevision.MinorRevision.
 */
#define DF_MAJOR_REVISION_BITPOS_LO            24
 /**
 * @brief Genoa - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 27
 *
 * @details Genoa - DF::FabricBlockInstanceCount[27:24] MajorRevision.
 *          Specifies the major revision of the Data Fabric hardware, MajorRevision.MinorRevision.
 */
#define DF_MAJOR_REVISION_BITPOS_HI            27
 /**
 * @brief Genoa - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) Bit 0
 *
 * @details Genoa - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          Instance type register.
 */
#define DF_INSTANCE_TYPE_BITPOS_LO              0
 /**
 * @brief Genoa - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) Bit 3
 *
 * @details Genoa - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          Instance type register.
 */
#define DF_INSTANCE_TYPE_BITPOS_HI              3
 /**
 * @brief Genoa - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM)
 *        Bit 8
 *
 * @details Genoa - DF::FabricBlockInstanceInformation3_CSNCSPIEALLM[19:8] BlockFabricID.
 *          This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are
 *          no pins allocated on the package to identify the socket and die. Allocation of bits in this field for
 *          socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
 */
#define DF_BLOCK_FABRICID_BITPOS_LO             8
 /**
 * @brief Genoa - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM)
 *        Bit 19
 *
 * @details Genoa - DF::FabricBlockInstanceInformation3_CSNCSPIEALLM[19:8] BlockFabricID.
 *          This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are
 *          no pins allocated on the package to identify the socket and die. Allocation of bits in this field for
 *          socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
 */
#define DF_BLOCK_FABRICID_BITPOS_HI            19

 /**
 * @brief   Genoa    - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 0
 *
 * @details Genoa - DF::DramOffset[0] HiAddrOffsetEn.
 *          The offset specified by HiAddrOffset is added when forming the normalized address.
 *          Control addition of HiAddrOffset when forming normalized address. This field must be set to one when
 *          HiAddrOffset is non-zero.
 */
#define DF_HI_ADDR_OFFSET_EN_BITPOS             0
 /**
 * @brief Genoa - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 1
 *
 * @details Genoa - DF::DramOffset[24:1] HiAddrOffset.
 *          Offset address[51:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of
 *          the associated range.
 */
#define DF_HI_ADDR_OFFSET_BITPOS_LO_DFX         1
 /**
 * @brief Genoa - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 24
 *
 * @details Genoa - DF::DramOffset[24:1] HiAddrOffset.
 *          Offset address[51:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of
 *          the associated range.
 */
#define DF_HI_ADDR_OFFSET_BITPOS_HI_DFX        24

 /**
 * @brief Genoa    - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 0
 *
 * @details Genoa    - DF::DramAddressCtl[0] AddrRngVal.
 *          Address range 0 valid (enabled/disabled)
 */
#define DF_ADDR_RANGE_VALID_BITPOS              0
 /**
 * @brief Genoa - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress) Bit 0
 *
 * @details Genoa - DF::DramBaseAddress[27:0] DramBaseAddr.
 *          Dram Base address[55:28]
 */
#define DF_DRAM_BASE_ADDR_BITPOS_LO_DFX         0
 /**
 * @brief Genoa - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress) Bit 27
 *
 * @details Genoa - DF::DramBaseAddress[27:0] DramBaseAddr.
 *          Dram Base address[55:28]
 */
#define DF_DRAM_BASE_ADDR_BITPOS_HI_DFX        27
 /**
 * @brief Genoa - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress) Bit 0
 *
 * @details Genoa - DF::DramLimitAddress[27:0] DramLimitAddr.
 *          DRAM limit address[55:28]
 */
#define DF_DRAM_LIMIT_ADDR_BITPOS_LO_DFX        0
 /**
 * @brief Genoa - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress) Bit 27
 *
 * @details Genoa - DF::DramLimitAddress[27:0] DramLimitAddr.
 *          DRAM limit address[55:28]
 */
#define DF_DRAM_LIMIT_ADDR_BITPOS_HI_DFX       27
 /**
 * @brief Genoa    - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 1
 *
 * @details Genoa    - DF::DramAddressCtl[1] LgcyMmioHoleEn.
 *          0=Memory hoisting is not enabled. 1=Enable memory hoisting for this address range.
 */
#define DF_LEGACY_MMIO_HOLE_EN_BITPOS           1
 /**
 * @brief Genoa - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 24
 *
 * @details Genoa - DF::DramHoleControl[31:24] DramHoleBase.
 *          Dram Hole Base[31:24]. Specifies the base address of the IO hole below the 4GB address level
 *          for legacy 32-bit devices. MMIO hole cannot spam multiple DRAM ranges; it must lie within a
 *          single DRAM address range. DRAM range which spans the IO hole needs to be hoisted (added) to
 *          compensate for the hole.
 */
#define DF_DRAM_HOLE_BASE_ADDR_BITPOS_LO       24
 /**
 * @brief Genoa - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 31
 *
 * @details Genoa - DF::DramHoleControl[31:24] DramHoleBase.
 *          Dram Hole Base[31:24]. Specifies the base address of the IO hole below the 4GB address level
 *          for legacy 32-bit devices. MMIO hole cannot spam multiple DRAM ranges; it must lie within a
 *          single DRAM address range. DRAM range which spans the IO hole needs to be hoisted (added) to
 *          compensate for the hole.
 */
#define DF_DRAM_HOLE_BASE_ADDR_BITPOS_HI       31
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 0
 *
 * @details Genoa - DF::DramAddressIntlv[2:0] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF_INTLV_ADDR_SEL_BITPOS_LO_DFX         0
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 2
 *
 * @details Genoa - DF::DramAddressIntlv[2:0] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF_INTLV_ADDR_SEL_BITPOS_HI_DFX         2
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 4
 *
 * @details Genoa - DF::DramAddressIntlv[8:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent stations across which
 *          addresses are interleaved. Hash interleaves must program IntLvAddrSel to select address bit [8].
 */
#define DF_INTLV_NUM_CHAN_BITPOS_LO_DFX         4
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8
 *
 * @details Genoa - DF::DramAddressIntlv[8:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent stations across which
 *          addresses are interleaved. Hash interleaves must program IntLvAddrSel to select address bit [8].
 */
#define DF_INTLV_NUM_CHAN_BITPOS_HI_DFX         8
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 12
 *
 * @details Genoa - DF::DramAddressIntlv[13:12] IntLvNumDies.
 *          Encoded value which specifies the number of dies across which addresses are interleaved.
 */
#define DF_INTLV_NUM_DIES_BITPOS_LO_DFX        12
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 13
 *
 * @details Genoa - DF::DramAddressIntlv[13:12] IntLvNumDies.
 *          Encoded value which specifies the number of dies across which addresses are interleaved.
 */
#define DF_INTLV_NUM_DIES_BITPOS_HI_DFX        13
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 18
 *
 * @details Genoa - DF::DramAddressIntlv[18] IntLvNumSockets.
 *          0=No socket interleave. 1=2 sockets. Specifies the number of sockets across which addresses
 *          are interleaved.
 */
#define DF_INTLV_NUM_SOCKETS_BITPOS_DFX        18
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 16
 *
 * @details Genoa - DF::DramAddressCtl[27:16] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramAddressIntlv[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF_DSTFABRICID_BITPOS_LO_DFX           16
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 27
 *
 * @details Genoa - DF::DramAddressCtl[27:16] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramAddressIntlv[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF_DSTFABRICID_BITPOS_HI_DFX           27
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 8
 *
 * @details Genoa - DF::DramAddressCtl[8] HashIntlvCtl64K.
 *          Enables inclusion of address bits on the 64KB boundary (bits 16, 17, 18) in a hashed
 *          interleave computation.
 */
#define DF_HASH_INTLV_CTL_64K_BITPOS_DFX        8
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 24
 *
 * @details Genoa - DF::DramAddressIntlv[24]
 */
#define DF_LOG2_ADDR_SPACE_BITPOS_LO_DFX       24
 /**
 * @brief Genoa - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 29
 *
 * @details Genoa - DF::DramAddressIntlv[29]
 */
#define DF_LOG2_ADDR_SPACE_BITPOS_HI_DFX       29
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 4
 *
 * @details Genoa - DF::DramAddressCtl[4] RemapEn.
 *          Remap Enable
 */
#define DF_REMAP_EN_BITPOS_DFX                  4
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::RemapSel) Bit 5
 *
 * @details Genoa - DF::DramAddressCtl[7:5] RemapSel.
 *          Remap Select
 */
#define DF_REMAP_SEL_BITPOS_LO_DFX              5
 /**
 * @brief Genoa - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::RemapSel) Bit 7
 *
 * @details Genoa - DF::DramAddressCtl[7:5] RemapSel.
 *          Remap Select
 */
#define DF_REMAP_SEL_BITPOS_HI_DFX              7
 /**
 * @brief Dram Base address LSB
 *
 * @details Genoa    - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)[27:0] DramBaseAddr.
 *                     => Dram Base address[55:28] => MSB: 55, LSB: 28
 */
#define DF_LSB_ADDR_BIT_IN_DRAM_MAPS           28
 /**
 * @brief HiAddrOffset LSB
 *
 * @details Genoa    - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset)[24:1] HiAddrOffset.
 *                     => Offset address[51:28] for CS DRAM Address range 1 => MSB: 51, LSB: 28
 */
#define DF_LSB_ADDR_BIT_IN_DRAM_OFFSET         28
 /**
 * @brief Number of address map registers in a single map
 *
 * @details In DFX, we include the remap register as part of the map
 *          BASE+LIMIT+CTL+INTLV+REMAPHI+REMAPLO
 */
#define ADDR_MAP_ARRAYSIZE                       6
 /**
 * @brief DF::DramBaseAddress register in single map[0]
 *
 * @details Genoa  - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)
 */
#define ADDR_MAP_ARRAY_BASE_OFFSET               0
 /**
 * @brief DF::DramLimitAddress register in single map[1]
 *
 * @details Genoa  - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress)
 */
#define ADDR_MAP_ARRAY_LIMIT_OFFSET              1
 /**
 * @brief DF::DramAddressCtl register in single map[2]
 *
 * @details Genoa   - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl)
 */
#define ADDR_MAP_ARRAY_CTL_OFFSET                2
 /**
 * @brief DF::DramAddressIntlv register in single map[3]
 *
 * @details Genoa   - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv)
 */
#define ADDR_MAP_ARRAY_INTLV_OFFSET              3
 /**
 * @brief DF::CsTargetRemap0B register in single map[4]
 *
 * @details Genoa - D18F7x184 [CS Target Remap 0 Register B] (DF::CsTargetRemap0B)
 */
#define ADDR_MAP_ARRAY_REMAPHI_OFFSET            4 // DFX only
 /**
 * @brief DF::CsTargetRemap0A register in single map[5]
 *
 * @details Genoa - D18F7x180 [CS Target Remap 0 Register A] (DF::CsTargetRemap0A)
 */
#define ADDR_MAP_ARRAY_REMAPLO_OFFSET            5
 /**
 * @brief The number of address maps in the hardware address space of the pre DFX processor
 *
 * @details The number of address maps in the hardware address space of the pre DFX processor: 16
 *          Pre DFX processor: Before the Genoa generation.
 */
#define DF_NUM_DRAM_MAPS_AVAILABLE_PRE_DFX     16
 /**
 * @brief The number of address maps in the hardware address space of the DFX processor
 *
 * @details The number of address maps in the hardware address space of the DFX processor: 32
 *          DFX processor: e.g. Genoa processor.
 */
#define DF_NUM_DRAM_MAPS_AVAILABLE_DFX         32
 /**
 * @brief Genoa - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) CCM instance
 *        type
 *
 * @details Genoa - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          0h = CCM
 *          1h = GCM
 */
#define DF_CCM_INSTANCE_TYPE_VALUE              0
#define DF_GCM_INSTANCE_TYPE_VALUE              1

/**
 * @brief Genoa - PCI function number and register offset of DF::SystemFabricIdMask0 register
 *
 * @details Genoa - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0)
 */
#define DF_SYSFABIDMASK0_REGADDR_DFX         ((4<<12)|(0x1B0))
/**
 * @brief Genoa - PCI function number and register offset of DF::SystemFabricIdMask1 register
 *
 * @details Genoa - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1)
 */
#define DF_SYSFABIDMASK1_REGADDR_DFX         ((4<<12)|(0x1B4))
/**
 * @brief Genoa - PCI function number and register offset of DF::SystemFabricIdMask2 register
 *
 * @details Genoa - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2)
 */
#define DF_SYSFABIDMASK2_REGADDR_DFX         ((4<<12)|(0x1B8))
/**
 * @brief Genoa - PCI function number and register offset of DF::FabricBlockInstanceCount register
 *
 * @details Genoa - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount)
 */
#define DF_FABBLKINSTCNT_REGADDR             ((0<<12)|(0x040))
/**
 * @brief Genoa - PCI function number and register offset of DF::FabricBlockInstanceInformation0 register
 *
 * @details Genoa - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0)
 */
#define DF_FABBLKINFO0_REGADDR               ((0<<12)|(0x044))
/**
 * @brief Genoa - PCI function number and register offset of DF::FabricBlockInstanceInformation3_CSNCSPIEALLM register
 *
 * @details Genoa - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM)
 */
#define DF_FABBLKINFO3_REGADDR               ((0<<12)|(0x050))
/**
 * @brief Genoa - PCI function number and register offset of DF::DramBaseAddress register
 *
 * @details Genoa - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)
 */
#define DF_DRAMBASE0_REGADDR_DFX             ((7<<12)|(0xE00))
/**
 * @brief Genoa -  PCI function number and register offset of DF::DramOffset0
 *
 * @details Genoa - DramOffset0 doesnt technically exist (always zero), DramOffset1 starts at 140, so we pretend
 *          DramOffset0 is at 13C
 */
#define DF_DRAMOFFSET0_REGADDR_DFX           ((7<<12)|(0x13C))
/**
 * @brief Genoa -  PCI function number and register offset of DF::DramHoleControl register
 *
 * @details Genoa - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl)
 */
#define DF_DRAMHOLECTL_REGADDR_DFX           ((7<<12)|(0x104))
/**
 * @brief Genoa - PCI function number and register offset of DF::DfGlobalCtrl register
 *
 * @details Genoa - D18F3x044 [DF Global Control] (DF::DfGlobalCtrl)
 */
#define DF_DFGLOBALCTRL_REGADDR              ((3<<12)|(0x044))
/**
 * @brief Genoa -  PCI function number and register offset of DF::CsTargetRemap0A register
 *
 * @details Genoa - D18F7x180 [CS Target Remap 0 Register A] (DF::CsTargetRemap0A)
 */
#define DF_CSTARGETREMAP0A_REGADDR_DFX       ((7<<12)|(0x180))

 /**
 * @brief Genoa - DF Configuration Register Broadcast access instance ID.
 */
#define FABRIC_REG_ACC_BC       (0xFF)
 /**
 * @brief Genoa - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) CS instance type
 *
 * @details Genoa - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          4h = CS
 */
#define DF_CS_INSTANCE_TYPE_VALUE               4

/*******************************************************************************
 * variable declaration
 *
 */

/*******************************************************************************
 * Declare Function prototypes
 *
 */
SIL_STATUS
InitializeApiRasGenoa (void);

SIL_STATUS
InitializeRasGenoa (void);

/*******************************************************************************
 * DfAddressTranslationLib API functions
 */
void
RetrieveRegsGenoa (
  uint32_t      PkgNo,
  uint32_t      mpuno,
  uint32_t      umcno,
  uint32_t      umcchno,
  uint32_t      BusNumberBase,
  SIL_ADDR_DATA *AddrData
  );

/*******************************************************************************
 * SocLib API functions
 */

void
ProgramCoreMcaConfigUmcGenoa (
  bool  EnableFruText
  );

uint8_t
GetUmcPerDieGenoa (void);

uint32_t
GetUmcHarvestFuseGenoa (
  size_t    BusNumberBase
  );

SIL_STATUS
UpdateFruTextToUmcGenoa (
  uint32_t              IohcBus,
  SIL_FRUTEXT_ENTRY *AmdFruTextSEntry
 );
