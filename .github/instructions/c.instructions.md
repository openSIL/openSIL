---
applyTo: "**/*.c,**/*.h"
description: "C conventions for openSIL source and header files"
---

# C source and headers

These conventions describe how the existing openSIL code is written. When a file you are editing differs, match the file, and do not reformat code you are not otherwise changing.

## Toolchains and warnings

- Code must build with GCC and MSVC, for both 32-bit and 64-bit targets; `-clang` cross files are also provided. Avoid compiler-specific extensions and variable-length arrays.
- The build uses C17 with `warning_level=3` and `werror=true`, and GCC and clang add `-Wstrict-prototypes` and `-Wmissing-prototypes`, so any new warning fails the build. Declare every non-`static` function in a header, and write `(void)` for an empty parameter list.
- `config.h` is force-included into every C file (`-include config.h` or `/FI config.h`), so Kconfig `CONFIG_*` values are always available. Do not include it explicitly.

## Files and formatting

- Start a new file the way existing files start: `/* SPDX-License-Identifier: MIT */`, a `/* Copyright (C) <years> Advanced Micro Devices, Inc. All rights reserved. */` line, and a Doxygen block with `@file` and `@brief`.
- Headers use `#pragma once`, not `#ifndef` include guards.
- Files use CRLF line endings, two-space indentation, ASCII only, no tabs, no trailing whitespace, and a single final newline (`.editorconfig`, `util/scripts/check_for_issues.py`). C lines stay within 120 columns.
- Format changed C files with uncrustify 0.79.0 and `util/uncrustify/uncrustify.cfg`:

  ```sh
  uncrustify -c util/uncrustify/uncrustify.cfg -l c --replace --no-backup <file>
  ```

  `make uncrustify` reformats every tracked C file; keep unrelated reformatting out of a change.

## Types and status codes

- Use the fixed-width types from `<stdint.h>` (`uint8_t`, `uint32_t`, `uint64_t`) and `size_t`. EDK2-style types such as `UINT32` are not used in this tree.
- Functions that can fail return `SIL_STATUS` from `Include/Sil-api.h`: `SilPass` on success, otherwise the most specific failure code (`SilInvalidParameter`, `SilNotFound`, `SilOutOfResources`, `SilUnsupported`, and so on). Values from `0xF0` up ask the host for a reset; return them only when the hardware sequence requires one.
- openSIL routines never hang. Bound every polling loop and return a status instead of waiting forever.
- Check every returned `SIL_STATUS` and propagate failures. A function that does not support a request returns `SilUnsupported` and leaves its outputs unchanged.

## Style

- Put the return type on its own line, a space between the function name and the opening parenthesis, one parameter per line, and the closing parenthesis on its own line:

  ```c
  void
  NbioIoApicMmioAddressBrh (
    GNB_HANDLE    *GnbHandle,
    uint64_t      IoapicMmioBase
    )
  {
  ```

- Functions and local variables use PascalCase (`GetGnbHandle`, `GnbHandle`). Types and macros use ALL_CAPS (`SIL_STATUS`, `IP_RECORD`, `NBIO_SPACE`). xUSL services keep their existing prefixes (`xUSLSmnRead`, `xUslRdMsr`).
- Document functions with Doxygen blocks (`@brief`, `@param[in]`, `@param[out]`, `@return`) and structure members with `///<`.

## Memory and buffers

- openSIL does not allocate memory. The host sizes a buffer with `xSimQueryMemoryRequirements()` and hands it over with `xSimAssignMemoryTp1()` through `xSimAssignMemoryTp3()`, and IP blocks get their data blocks from it. Do not call `malloc` or `free` in library code.
- Bound every `memcpy` and `memset` by the size of the destination, and validate lengths and counts that come from input blocks, APOB data, or hardware.
- Initialize every local before it is used or written back.

## Public headers

- Fabric, locality, CXL, and SMU consumers include the public `xPrf*.h` headers in `Include/`. Keep each of them self-contained: it must compile on its own with only `Include/` on the include path and no generated platform header. The host tests check this.

## Tracing

- Use the trace macro of the module you are in (`NBIO_TRACEPOINT`, `CCX_TRACEPOINT`, `DF_TRACEPOINT`, `FCH_TRACEPOINT`, and so on) or the library macros (`XSIM_TRACEPOINT`, `XUSL_TRACEPOINT`, `XPRF_TRACEPOINT`), with a level from `Include/Sil-api.h`: `SIL_TRACE_ERROR`, `SIL_TRACE_WARNING`, `SIL_TRACE_INFO`, `SIL_TRACE_VERBOSE`, `SIL_TRACE_ENTRY`, `SIL_TRACE_EXIT`, or `SIL_TRACE_RAW`.
- Trace output goes to the host through the `HOST_DEBUG_SERVICE` registered with `SilDebugSetup()`. Never call `printf` from library code.

## Build-time selection

- Select code with Kconfig symbols from `config.h`, as in `#if CONFIG_ESM_EN_ALL_ROOT_PORTS`, rather than with new ad-hoc `#define` switches.
