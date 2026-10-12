---
description: "Find the root cause of an openSIL build or CI failure from its log"
---

# Triage a build failure

Find the root cause of a failed openSIL build, local or CI. This is read-only unless the user asks for a fix.

## Inputs

- The failing log: `meson setup` or `meson compile` output, or a CI job log.
- The commit or branch, and the configuration: cross file, compiler, 32-bit or 64-bit, and options (`unit_test`, `coverity`, `PlatKcfg`).

## Steps

1. Find the first real error, not the last line of the log. Builds use `-Werror`, so the first warning promoted to an error is usually the cause.
2. Classify the failure:

   | Category | Typical signs | Usual cause |
   | --- | --- | --- |
   | Configure | `meson setup` fails; cross file or `nasm` not found; the platform config does not load | Wrong cross-file path, missing tool, or a bad `PlatKcfg` or `PlatKcfgDir` |
   | Compile | Error in a `.c` or `.h` file, or a warning promoted by `-Werror` | Missing prototype (`-Wmissing-prototypes`), missing `(void)` (`-Wstrict-prototypes`), or a type-width difference between 32-bit and 64-bit |
   | Toolchain-specific | Fails with MSVC but not GCC or clang, or the reverse | A compiler extension, a variable-length array, or a format specifier |
   | Link | Undefined reference or unresolved external | A source file missing from `meson.build`, Kconfig gating that excludes a file still referenced, or an IP table naming a function that is not built |
   | Assembly | NASM error | A `.asm` file listed for the wrong width (`xusl_asm32` or `xusl_asm64`) |
   | Configuration | `CONFIG_*` symbol undefined | A Kconfig symbol added without regenerating `configs/Onyx_SilCfg` and `configs/Onyx_SilCfg.h` |
   | Environment | Runner, network, or tool installation errors | Not a code problem; fix the environment and rerun |

3. Decide whether the change under test caused it. Compare with the target branch (`git diff origin/genoa_poc...HEAD`) and, if you can, build the target branch with the same configuration.
4. Reproduce locally in a fresh build directory with the same cross file and options.

## Output

```markdown
## Build failure: <short title>

- Configuration: <cross file, compiler, width, options>
- First error: `<file:line: message>`
- Category: <from the table>
- Root cause: <what is wrong and why>
- Caused by this change: <yes, no, or unclear, with the evidence>
- Fix: <the smallest change that resolves it>
- Validation: <commands that confirm the fix>
```
