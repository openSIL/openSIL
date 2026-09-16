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

- `public-headers`: each SMU/fabric/CXL public header by itself and all together,
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
- `cxl-device-info`: the production DVSEC capability decoder, HDM register reads,
  endpoint information, absent capabilities, and invalid inputs. PCI reads and
  capability lookup are provided by fixtures; switch traversal is not covered.

The FCH suite records I/O and mocks MMIO read-modify-write functions and cache
flushes. Three services with inline MMIO reads are intercepted using linker
`--wrap`: SPD bus release and SMI timer start/stop. Their xPRF lookup and dispatch
are tested, while their hardware implementation is outside this suite. The
assembly I/O mocks retain the provider's `NASM_ABI` calling convention. Unused
production functions are removed with linker section garbage collection.

The DF fixtures supply already-built topology records; they do not validate
topology discovery or full silicon initialization. The arena fixture inserts an
unused info block when necessary to align the native DF structure. The current
allocator guarantees DWORD alignment, so these service tests do not establish
that the complete firmware allocation sequence satisfies stronger alignment.

Passing these tests does not replace firmware builds or hardware boot tests.
