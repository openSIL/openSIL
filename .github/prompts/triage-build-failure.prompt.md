---
description: "Find the root cause of an openSIL build, host test, or CI failure from its log"
---

# Triage a build failure

Find the root cause of a failed openSIL build or host test run, local or CI. This is read-only unless the user asks for a fix.

## Inputs

- The failing log: `make`, `meson setup`, or `meson compile` output, `tests/HostTest/run.py` output, or a CI job log.
- The commit or branch, and the configuration: cross file, compiler, 32-bit or 64-bit, and options (`KCONFIG_CONFIG` or `PlatKcfg`, `unit_test`, `coverity`).

## Steps

1. Find the first real error, not the last line of the log. The build uses `werror`, so the first warning promoted to an error is usually the cause.
2. Classify the failure:

   | Category | Typical signs | Usual cause |
   | --- | --- | --- |
   | Configure | `meson setup` fails; cross file or `nasm` not found; the platform config does not load | Wrong cross-file path, Meson older than 1.1.0, NASM older than 2.15.05, or a bad `PlatKcfg` or `PlatKcfgDir` |
   | Missing link test | `util/unitTests/linktest.c` not found | `-Dunit_test=true`; this branch does not contain that file, so leave the option off |
   | Compile | Error in a `.c` or `.h` file, or a warning promoted by `werror` | Missing prototype (`-Wmissing-prototypes`), missing `(void)` (`-Wstrict-prototypes`), a C17 issue, or a type-width difference between 32-bit and 64-bit |
   | Toolchain-specific | Fails with MSVC but not GCC, or the reverse | A compiler extension, a variable-length array, or a format specifier |
   | Link | Undefined reference or unresolved external | A source file missing from `meson.build`, Kconfig gating that excludes a file still referenced, or an IP table naming a function that is not built |
   | Configuration | `CONFIG_*` symbol undefined | A Kconfig symbol added without regenerating `configs/Purico_SilCfg` and `configs/Purico_SilCfg.h` |
   | Host test | A suite assertion fails, or ASan or UBSan reports an error | The change broke behavior the suite covers, or the suite's fixtures need updating with the change |
   | File check | `check_for_issues.py` reports line endings, tabs, trailing whitespace, or non-ASCII characters | The file was saved with LF or contains non-ASCII text; `util/scripts/fix_issues.sh` fixes most cases |
   | Environment | Runner, network, or tool installation errors | Not a code problem; fix the environment and rerun |

3. Decide whether the change under test caused it. Compare with the target branch (`git diff origin/turin_poc...HEAD`) and, if you can, build or test the target branch with the same configuration.
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
