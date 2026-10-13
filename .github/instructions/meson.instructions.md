---
applyTo: "**/meson.build,meson_options.txt,util/meson/**,Makefile"
description: "Meson and Makefile build conventions for openSIL"
---

# Meson build files

## How the build is put together

- The top-level `meson.build` loads the platform Kconfig output (`configs/Purico_SilCfg` by default) with the `keyval` module, generates `config.h` from `configs/SilCfg.Template`, sets C17 with `werror=true`, and then descends into `xUSL`, `xSIM`, and `xPRF`.
- Each directory's `meson.build` appends to shared lists and descends with `subdir()`. Sources go into `xusl`, `xsim`, or `xprf` with `files([...])`, and include paths go into `incdir` with `include_directories()`.
- The targets are the libraries `AMD_xusl`, `AMD_xsim`, and `AMD_xprf`, combined into `AMDopensil32` or `AMDopensil64`, plus the `pseudo_host` link test when `-Dunit_test=true`. The build also writes the list of compiled files to `ListfileC_<platform>.txt` in the build directory.
- The `Makefile` is a front end for the same build on Linux and Windows: `make set32` or `make set64` creates the build directory, and `make build32` or `make build64` builds it. `KCONFIG_CONFIG` selects the platform file (passed as `-DPlatKcfg`), and `CFG_OPT` and `BLD_OPT` pass extra Meson options. `make help` lists every target.

## Changing build files

- When you add or remove a `.c` file, update the nearest `meson.build` in the same change. Keep one file per line in the `files([...])` list.
- Code inclusion follows Kconfig, read with `conf.get_unquoted()`, as `xSIM/SoC/F1AM00/meson.build` does:

  ```meson
  Meson_SOC_F1AM00 = (conf.get_unquoted('CONFIG_SOC_F1AM00', 'n') == 'y') ? 1 : 0
  if ((Meson_SOC_F1AM00 == 1) or COVERITY_BUILD)
      xsim += files([ 'IpBlkListF1AM00Tp1.c' ])
  endif
  ```

  Keep `or COVERITY_BUILD` so the Coverity build compiles every source.
- Assembly files are assembled by NASM (2.15.05 or later) through `asm_gen`; Coverity builds skip NASM.
- Compiler flags are set separately for GCC/clang and for MSVC. Keep the two branches consistent, and never relax `werror` to get a change through.
- Keep `-Wl,-z,noexecstack` in the GCC/clang link arguments. The NASM objects have no `.note.GNU-stack` section, and without the flag the linker's executable-stack warning fails the `pseudo_host` link under `werror`.

## Options and cross files

- `meson_options.txt` defines `PlatKcfg`, `PlatKcfgDir`, `unit_test`, and `coverity`. Platform builds never enable `unit_test` or `coverity`. Prefer a Kconfig symbol over a new Meson option.
- `unit_test` builds `pseudo_host` from `util/unitTests/linktest.c`. Build it to check that the library links into a host program, but do not run it: it calls into silicon initialization. The native tests live in `tests/HostTest`.
- The cross files in `util/meson/` select the target: `x86-amd64-linux-gnu` and `x86-i386-linux-gnu`, their `-clang` variants, and `x86-amd64-msvc` and `x86-i386-msvc`.

## Validation

- Build both 32-bit and 64-bit configurations from fresh build directories, adding `-Dunit_test=true` to also link `pseudo_host`. The build section of `AGENTS.md` has the commands, and `tests/HostTest/README.md` explains how to run the host tests against a build's compilation database.
