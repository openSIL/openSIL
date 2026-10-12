---
applyTo: "**/*.c,**/*.h"
description: "C conventions for openSIL source and header files"
---

# C source and headers

These conventions describe how the existing openSIL code is written. When a file you are editing differs, match the file, and do not reformat code you are not otherwise changing.

## Toolchains and warnings

- Code must build with GCC, LLVM/clang, and MSVC, for both 32-bit and 64-bit targets. Avoid compiler-specific extensions and variable-length arrays.
- GCC and clang builds use `-Wall -Werror` with `-Wstrict-prototypes` and `-Wmissing-prototypes`, so any new warning fails the build. Declare every non-`static` function in a header, and write `(void)` for an empty parameter list.
- `config.h` is force-included into every C file (`-include config.h` or `/FI config.h`), so Kconfig `CONFIG_*` values are always available. Do not include it explicitly.

## Files

- Start a new file the way existing files start: a copyright line, `// SPDX-License-Identifier: MIT`, and a Doxygen `@file` block that says what the file is for.
- Headers use `#pragma once`, not `#ifndef` include guards.

## Types and status codes

- Use the fixed-width types from `<stdint.h>` (`uint8_t`, `uint32_t`, `uint64_t`) and `size_t`. EDK2-style types such as `UINT32` are not used in this tree.
- Functions that can fail return `SIL_STATUS` from `Include/Sil-api.h`: `SilPass` on success, otherwise the most specific failure code (`SilInvalidParameter`, `SilNotFound`, `SilOutOfResources`, `SilAborted`, and so on). Values from `0xF0` up ask the host for a reset; return them only when the hardware sequence requires one.
- openSIL routines never hang. Bound every polling loop and return a status instead of waiting forever.
- Check every returned `SIL_STATUS` and propagate failures.

## Style

- Put the return type on its own line, and leave a space between the function name and its parameter list:

  ```c
  SIL_STATUS
  NbioSetTopOfMemory (void)
  {
  ```

- Functions and local variables use PascalCase (`GetGnbHandle`, `GnbHandle`). Types and macros use ALL_CAPS (`SIL_STATUS`, `IP_RECORD`, `BIT_64`). xUSL services keep their existing prefixes (`xUSLSmnRead`, `xUslRdMsr`).
- Indent with two spaces. Use spaces, not tabs, in new code.
- Document functions with Doxygen blocks (`@param`, `@return`, `@retval`) and structure members with `///<`.

## Memory and buffers

- openSIL does not allocate memory. The host sizes a buffer with `xSimQueryMemoryRequirements()` and hands it over with `xSimAssignMemoryTp1()` through `xSimAssignMemoryTp3()`, and IP blocks get their data blocks from it. Do not call `malloc` or `free` in library code.
- Bound every `memcpy` and `memset` by the size of the destination, and validate lengths that come from input blocks or hardware.
- Initialize every local before it is used or written back. A recent fix in `NbioSetTopOfMemory()` corrected a register structure that was written to hardware without first being read; the clang static analyzer flagged it. The tree also carries Coverity checker lists (`amd-checkers.txt`, and `cert-c-all.config` for CERT C), and new code should not add findings.

## Tracing

- Use the trace macro of the module you are in (`NBIO_TRACEPOINT`, `CCX_TRACEPOINT`, `FCH_TRACEPOINT`, and so on) or the library macros (`XSIM_TRACEPOINT`, `XUSL_TRACEPOINT`, `XPRF_TRACEPOINT`), with a level from `xUSL/Include/SilCommon.h`: `SIL_TRACE_ERROR`, `SIL_TRACE_WARNING`, `SIL_TRACE_INFO`, `SIL_TRACE_ENTRY`, `SIL_TRACE_EXIT`, or `SIL_TRACE_RAW`.
- Trace output goes to the host through the `HOST_DEBUG_SERVICE` registered with `SilDebugSetup()`. Never call `printf` from library code.

## Build-time selection

- Select code with Kconfig symbols from `config.h`, as in `#if CONFIG_HAVE_XMP_VER_B`, rather than with new ad-hoc `#define` switches.
