<!-- SPDX-License-Identifier: MIT -->
<!-- Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. -->

# Native provider tests

These tests compile and link the actual openSIL C sources with their provider
headers. They run on an x86-64 Linux host without accessing firmware hardware.
Python 3, a native GCC-compatible C compiler, a GNU-compatible linker, and the
ASan/UBSan runtime libraries are required. No Python packages are needed.

Use the compilation database and generated platform header from an existing
Turin openSIL Meson build. For a coreboot build whose output directory is
`/path/to/coreboot-build`, run from the openSIL checkout:

```sh
python3 tests/HostTest/run.py \
  --compile-commands /path/to/coreboot-build/opensil/compile_commands.json \
  --config /path/to/coreboot-build/opensil_config.h \
  --output-dir /tmp/opensil-host-tests
```

`--config` also accepts the build's `config.h` wrapper when the platform header
it includes is in an include directory recorded by the compilation database.
The runner keeps includes belonging to this checkout, the Meson build directory
(including its parent for generated coreboot headers), and the supplied
configuration directory. It deliberately omits host-firmware
libc headers and cross-compiler flags so the tests use native libc and sanitizers.
The compilation database must refer to this checkout.

The runner enables ASan and UBSan by default. Leak detection defaults to off
because the fixtures own no heap allocations and LSan cannot run under ptrace.
`ASAN_OPTIONS` and `UBSAN_OPTIONS` can override their defaults. Use
`--no-sanitizers` only when the host lacks the sanitizer runtimes. `--suite`
selects one suite. Without `--output-dir`, a temporary directory is removed
after the run; with it, binaries, compile commands, and logs are retained.

The suites cover:

- `xsim-dispatch`: all three public timepoint dispatchers, exact IP call order
  and count, empty and registration-only entries, one-shot and repeated
  deferred requests, cold-over-warm priority, and errors/immediate resets
  overriding earlier deferred requests without visiting later IPs. The SoC
  tables and IP callbacks are fixtures; no hardware reset is performed.
- `public-headers`: each SMU/fabric/CXL/CPU public header by itself and all together,
  with only `Include/` on the include path and no generated platform header.
- `provider-contracts`: real SIL allocation/lookup and FCH default assignment;
  FCH service lookup failures and dispatch; configured PM1/GPE addresses,
  access widths, SCI/sleep-type preservation, and GPE write-one-to-clear;
  NPS0/NPS1/NPS2/NPS4 and CCX/CXL domain counts; DF default and host-supplied
  locality policy; and unsupported RAS translations leaving outputs untouched.
- `multi-fch`: the production secondary FCH dispatcher and Kunlun transfer table;
  one/two sockets, absent and implemented secondary SATA callbacks, continued
  AB/SD/USB initialization, callback arguments and errors, multiple secondary
  dies, and missing table/data. Platform callbacks and lookup services are mocked.
- `cpu-topology`: the enabled-thread query with independent per-socket APOB maps;
  one/two sockets including 384/768 threads, different socket populations,
  physical harvesting, thread enable flags, missing maps/services, malformed
  coordinates, duplicate APIC IDs, and output-capacity/error-count contracts.
  DF/APOB/CCX calls are mocked; this suite does not test AP startup or the hardware
  APIC encoding calculated by CCX.
- `fabric-domain`: real NUMA domain construction and logical-CCD translation;
  NPS0/NPS1/NPS2/NPS4, the Titanite CCD2 affinity with stale global strides 0/8,
  sparse physical CCD maps, CCX-as-NUMA masks, different per-socket CCX strides,
  and socket1 physical CCD15 at bit31. APOB and hardware discovery callbacks
  are mocked. The synthetic different-stride case checks per-socket semantics;
  it does not establish support for another processor SKU.
- `memory-dmi`: the public memory query through real MEM/APOB code, with raw
  SMBIOS and SPD fixtures for both sockets and both slots; channel translation,
  empty connectors, missing services/data, malformed lengths/coordinates,
  duplicate records, SPD decoding, and cleared output on failure.
- `nbio-ioapic`: production IOAPIC BAR/ID programming and SMN access, including
  both physical RB register banks, segment-zero and nonzero-segment targets,
  complete PCI index/data transaction sequences, and other-segment/bus isolation.
- `nbio-non-pci-bar`: production generic/PSP non-PCI BAR helpers and SMN access;
  allocation targets, enable/lock combinations, preassigned BARs, lookup and
  allocation failures, and preservation of BARs on other segments and buses.
- `cxl-device-info`: the production DVSEC capability decoder, HDM register reads,
  endpoint information, absent capabilities, and invalid inputs. PCI reads and
  capability lookup are provided by fixtures; switch traversal is not covered.

The FCH suite records I/O and mocks MMIO read-modify-write functions and cache
flushes. Three services with inline MMIO reads are intercepted using linker
`--wrap`: SPD bus release and SMI timer start/stop. Their xPRF lookup and dispatch
are tested, while their hardware implementation is outside this suite. The
assembly I/O mocks retain the provider's `NASM_ABI` calling convention. Unused
production functions are removed with linker section garbage collection.

The NBIO suites mock the PCI read/write boundary and maintain separate SMN
register state for each segment and bus. The non-PCI BAR suite also supplies a
resource allocator. Their register traces validate provider dispatch and BAR
programming; they do not discover topology, execute hardware accesses, or
establish that a board boots with a particular PCI segment allocation.

The `provider-contracts` DF fixtures supply already-built domain records. The
`fabric-domain` suite builds those records from mocked discovery and NPS data;
neither validates hardware topology discovery or full silicon initialization.
The arena fixture inserts an
unused info block when necessary to align the native DF structure. The current
allocator guarantees DWORD alignment, so these service tests do not establish
that the complete firmware allocation sequence satisfies stronger alignment.

Passing these tests does not replace firmware builds or hardware boot tests.
