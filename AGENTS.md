# openSIL contributor guidance

This file provides repository-level context for AI coding assistants and human contributors. Tool-specific configuration may add capabilities, but it must not replace or contradict this guidance.

## Project scope

- This branch contains the AMD openSIL Genoa proof of concept.
- It is published for evaluation and is not production firmware.
- openSIL is host-firmware independent and is licensed under the MIT License.
- Keep changes limited to the public code and documentation in this repository. Do not introduce confidential platform information.

## Architecture

openSIL is organized as three statically linked libraries:

- `xSIM`: silicon initialization modules and initialization sequencing.
- `xPRF`: platform reference functions exposed to host firmware.
- `xUSL`: common utilities, services, and silicon IP implementations.

Other important locations:

- `Include`: public interfaces shared across the libraries.
- `configs`: Kconfig output for the supported Onyx platform.
- `util`: Kconfig, Meson, cross-compilation, and test utilities.
- `util/unitTests/linktest.c`: the link test that becomes `pseudo_host` when unit tests are enabled.

Preserve the xSIM, xPRF, and xUSL boundaries. Keep host-firmware dependencies outside the openSIL libraries and use existing interfaces between components.

## Sources of truth

Before changing code, consult the files that define the current public POC:

- `README.md` for project status, supported hardware, and setup.
- `Kconfig*` and `configs/` for feature and platform configuration.
- `meson.build`, `meson_options.txt`, and `util/meson/` for builds.
- The nearest existing source and header files for local conventions.

Do not import assumptions from another openSIL branch or a private repository.

## Build and validation

The build needs Meson with Ninja, NASM, Python 3, and a supported C toolchain: GCC, LLVM/clang, or MSVC. The Kconfig tooling is bundled under `util/kconfig`. The default Meson configuration uses `configs/Onyx_SilCfg`.

Linux 64-bit build:

```sh
meson setup build_l64 --cross-file util/meson/x86-amd64-linux-gnu
meson compile -C build_l64
```

Linux 32-bit build:

```sh
meson setup build_l32 --cross-file util/meson/x86-i386-linux-gnu
meson compile -C build_l32
```

The `-clang` cross files select LLVM/clang instead of GCC. On Windows, use `util/meson/x86-amd64-msvc` or `util/meson/x86-i386-msvc` from a Visual Studio developer prompt; `util/SetSilEnv.cmd` sets up the environment.

To also build the link test, add `-Dunit_test=true`:

```sh
meson setup build_l64 --cross-file util/meson/x86-amd64-linux-gnu -Dunit_test=true
meson compile -C build_l64
```

A successful build of `pseudo_host` is the check: it links the full library into a host program. Do not run it on a development machine, because it calls `InitializeSiTp1()`, which accesses silicon registers.

If a build directory already exists, use Meson's reconfigure command or a new build directory rather than editing generated files.

Run the checks relevant to the change. Changes to public interfaces, pointer widths, or build files should be checked in both 32-bit and 64-bit builds. The `coverity` Meson option and the checker lists in `amd-checkers.txt` and `cert-c-all.config` support Coverity analysis with CERT C rules; new code should not add findings.

## Contributing

Human contribution process and the public contribution timeline are in `CONTRIBUTING.md`. This file is the in-repo context for assistants and reviewers. Do not treat it as a production-firmware support contract.

## Scoped guidance

Detailed conventions live in file-matched instruction files. Assistants that support the `applyTo` front matter load them automatically; other assistants must read the matching file before editing.

| Files | Guidance |
| --- | --- |
| `*.c`, `*.h` | `.github/instructions/c.instructions.md` |
| `meson.build`, `meson_options.txt`, `util/meson/` | `.github/instructions/meson.instructions.md` |
| `xSIM/`, `xPRF/`, `xUSL/`, `Kconfig*`, `configs/` | `.github/instructions/ip-blocks.instructions.md` |

## Reusable workflows

Playbooks for repeatable tasks live in `.github/prompts/`:

- `review-pr.prompt.md`: review a pull request from its current diff.
- `triage-build-failure.prompt.md`: analyze CI or local build logs.
- `promote-guidance.prompt.md`: turn a recurring finding into proposed repository guidance.

These playbooks are read-only unless the user explicitly requests edits. They produce evidence for human review; they do not approve or merge changes.

## Change discipline

1. Read the complete files being changed and the relevant scoped guidance.
2. Use repository source, current diffs, and actual logs as evidence.
3. Make the smallest coherent change that satisfies the request.
4. Update the nearest `meson.build` when adding or removing source files.
5. Preserve public interfaces unless the task explicitly changes them.
6. Validate the affected build configurations and report what was run.
7. Keep unrelated cleanup out of the change.

## Guardrails

- Never expose credentials, private repository URLs, NDA material, or confidential platform information.
- Do not add generated build output or local environment files.
- Do not bypass validation, code review, branch protection, or required human approval.
- Do not force-push, rewrite shared history, or delete branches unless a human explicitly requests and owns the operation.
- Do not let an AI assistant approve or merge its own changes.
- Treat generated code like any other contribution: inspect the diff, run the relevant checks, and obtain human review.

## Improving this guidance

When a review or failed build reveals a recurring problem, capture the evidence and use `promote-guidance.prompt.md` to identify the narrowest durable home for the lesson. An assistant may propose the update, but repository maintainers review and approve it through the normal pull-request process.
