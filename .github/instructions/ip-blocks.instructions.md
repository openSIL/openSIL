---
applyTo: "xSIM/**,xPRF/**,xUSL/**,Kconfig*,configs/**"
description: "openSIL IP blocks, timepoint sequencing, register access, and Kconfig"
---

# IP blocks, timepoints, and configuration

## Where things live

- `xUSL/<IP>/` holds the silicon IP implementations (CCX, DF, FCH, MEM, Mpio, NBIO, RAS, RcMgr, SMU, Sdci, Cxl), and `xUSL/CommonLib` holds shared services.
- `xUSL/XMP` is the example device. It shows the whole pattern for wiring an IP: its own Kconfig, Meson gating on `HAVE_XMP_VER_A` or `HAVE_XMP_VER_B`, and an entry in the IP table.
- `xSIM/` sequences initialization. For the Genoa SoC (F19M10), `xSIM/SoC/F19M10/IpBlkListF19M10Tp1.c`, `Tp2.c`, and `Tp3.c` list the IP blocks for each timepoint.
- `xPRF/` holds the platform reference functions the host calls (CCX, Mem, NBIO, RAS).
- `Include/` holds the host-facing API: `Sil-api.h`, `xSIM-api.h`, and `xPRF-api.h`.

## Host flow

The host drives openSIL through three timepoints (`Include/xSIM-api.h`, `util/unitTests/linktest.c`):

1. `xSimQueryMemoryRequirements()` returns the buffer size, and the host allocates the buffer.
2. `xSimAssignMemoryTp1()` hands the buffer to xSIM, and `SilDebugSetup()` registers the host's debug output.
3. The host may call `SilFindStructure()` to locate an IP block's data, inspect the defaults, and change settings.
4. `InitializeSiTp1()` runs the TP1 IP list. TP2 and TP3 follow the same pattern with `xSimAssignMemoryTp2()` and `InitializeSiTp2()`, then `xSimAssignMemoryTp3()` and `InitializeSiTp3()`.

## IP records

Each entry in a SoC table is an `IP_RECORD` (`xSIM/IpHandler.h`):

| Field | Meaning | Example |
| --- | --- | --- |
| `IpID` | `SIL_DATA_BLOCK_ID` of the IP | `SilId_NbioClass` |
| `BlkRequestSize` | Bytes reserved for the IP's input and output blocks | `sizeof(NBIOCLASS_DATA_BLOCK)` |
| `SetInput` | Establishes the IP's input block | `NbioClassSetInputBlk` |
| `Initialize` | Initializes the silicon for this timepoint | `InitializeNbioTp1` |
| `ApiInit` | Initializes the IP's internal API and its IP-to-IP API pointer | `InitializeApiNbioIod` |

- Any function pointer may be `NULL`, and `BlkRequestSize` may be 0, when the IP has nothing to do at that timepoint.
- Entries run in table order, and some depend on others; the table notes that `SilId_MultiFchClass` must run after all other FCH IP blocks. Do not reorder entries without stating the dependency.
- Each list ends with `{SilId_ListEnd, 0, NULL, NULL, NULL}`.
- Follow the existing names: `<Ip>SetInputBlk` or `<Ip>ClassSetInputBlk`, `Initialize<Ip>Tp1` through `Initialize<Ip>Tp3`, and `InitializeApi<Ip>` or `Set<Ip>Api`.

## Register access

- Use the xUSL accessors rather than raw pointers: `xUSLSmnRead`, `xUSLSmnWrite`, and `xUSLSmnReadModifyWrite` (`xUSL/CommonLib/SmnAccess.h`) for SMN registers, and `xUslRdMsr` and `xUslWrMsr` (`xUSL/CommonLib/CpuLib.h`) for MSRs.
- Register structures are unions with `.Value` and `.Field.<NAME>`. For a read-modify-write, read the register into `.Value`, change the fields, and write `.Value` back. Never write back a structure that was not read or fully initialized first.
- Program every instance. NBIO code walks the instances with `GetGnbHandle()` and `GnbGetNextHandle()` and addresses each one with `NBIO_SPACE()`. Do not assume instance 0 is the only one.

## Kconfig and platform configuration

- Kconfig prefixes every symbol with `CONFIG_`. `HAVE_*` symbols control code inclusion and are read by the Meson files; `PLAT_*` symbols describe the platform.
- Each IP keeps its options in its own `Kconfig` (for example `xUSL/NBIO/IOD/Kconfig`), sourced from `Kconfig.IP`. Give every option `help` text, and keep `range` and `default` values accurate.
- `configs/Onyx_SilCfg` and `configs/Onyx_SilCfg.h` are generated. `util/Kcfg.sh menu Onyx` writes the first, and `util/Kcfg.sh update Onyx` writes the second (`util/Kcfg.cmd` on Windows). Regenerate them instead of editing them by hand, and commit both when a default changes.

## Scope

- This branch supports F19M10 (Genoa) on the Onyx reference board. Other POCs live on their own branches (`turin_poc`, `phoenix_poc`).
- Do not add register definitions, fuse or strap details, or errata information beyond what is already public in this tree.
