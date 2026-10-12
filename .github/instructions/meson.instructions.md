---
applyTo: "**/meson.build,meson_options.txt,util/meson/**"
description: "Meson build conventions for openSIL"
---

# Meson build files

## How the build is put together

- The top-level `meson.build` loads the platform Kconfig output (`configs/Onyx_SilCfg` by default) with the `keyval` module, generates `config.h` from `configs/SilCfg.Template`, sets the compiler flags, and then descends into `xUSL`, `xSIM`, and `xPRF`.
- Each directory's `meson.build` appends to shared lists and descends with `subdir()`. Sources go into `xusl`, `xsim`, or `xprf` with `files([...])`, and include paths go into `incdir` with `include_directories()`.
- The targets are the libraries `AMD_xusl`, `AMD_xsim`, and `AMD_xprf`, combined into `AMDopensil32` or `AMDopensil64`, plus `pseudo_host` when `-Dunit_test=true`.

## Changing build files

- When you add or remove a `.c` file, update the nearest `meson.build` in the same change. Keep one file per line in the `files([...])` list.
- Code inclusion follows Kconfig. Read `HAVE_*` and `SOC_*` symbols with `conf.get_unquoted()`, as `xUSL/XMP/meson.build` does:

  ```meson
  Meson_HAVE_XMP_VER_B = (conf.get_unquoted('CONFIG_HAVE_XMP_VER_B', 'n') == 'y') ? 1 : 0
  if (Meson_HAVE_XMP_VER_B == 1) or COVERITY_BUILD
      subdir('Ver-B')
  endif
  ```

  Keep `or COVERITY_BUILD` so the Coverity build compiles every source.
- Assembly files go in `xusl_asm32` or `xusl_asm64` and are assembled by NASM through `asm_gen`. Coverity builds skip NASM.
- Compiler flags are global (`add_global_arguments`) and set separately for GCC/clang and for MSVC. Keep the two branches consistent, and never relax `-Werror` to get a change through.

## Options and cross files

- `meson_options.txt` defines `PlatKcfg`, `PlatKcfgDir`, `unit_test`, and `coverity`. Platform builds never enable `unit_test` or `coverity`. Prefer a Kconfig symbol over a new Meson option.
- The cross files in `util/meson/` select the target: `x86-amd64-linux-gnu` and `x86-i386-linux-gnu`, their `-clang` variants, and `x86-amd64-msvc` and `x86-i386-msvc`. `BUILD_IS_32BIT` comes from the cross file's `is32bit` property.

## Validation

- Configure fresh 32-bit and 64-bit build directories and build both. Add `-Dunit_test=true` to also link `pseudo_host`. The build section of `AGENTS.md` has the commands.
