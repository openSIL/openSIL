# Contributing to the openSIL Genoa POC

This repository is the public AMD openSIL Genoa proof of concept. It is published for evaluation. It is not production firmware, and it is not eligible for AMD bug-bounty programs.

## How to contribute today

1. Work only in this public tree. Do not copy confidential platform data, NDA material, private repository URLs, or unpublished silicon enablement.
2. Prefer small, reviewable changes: documentation, Meson/build fixes, and clearly bounded code fixes on the Genoa POC.
3. Open a GitHub issue or pull request against [openSIL/openSIL](https://github.com/openSIL/openSIL). The usual target branch for this POC is `genoa_poc`.
4. Read `README.md` for status and setup, `AGENTS.md` for architecture boundaries, build commands, and assistant guardrails, and the files in `.github/instructions/` that match the files you change.
5. Human review remains required. AI assistants may draft analysis; they must not approve or merge.

## Contribution timeline

This table is a **public contribution** schedule for the evaluation POC. It is not a production silicon roadmap. Product phases stay in `README.md`.

| Window | What contributors should expect |
| --- | --- |
| Now | Root `AGENTS.md` records architecture, Meson/build commands, and guardrails. File-matched C, Meson, and IP-block instruction files live in `.github/instructions/`, and PR-review, build-triage, and guidance playbooks live in `.github/prompts/`. This file is the contribution path. |
| Next | The same foundation on the other POC branches, starting with `turin_poc`, then `phoenix_poc`. |
| Ongoing | Issues and pull requests against the evaluation POC: docs, build, and bounded code fixes. Recurring lessons are proposed as guidance updates and land only through human review. |
| Product (not a community milestone) | Phase IV POR with UEFI host firmware, trending 2026, as already stated in `README.md`. |

The reusable OPF contribution is this shared-context foundation in the public repo.

## What not to contribute

- Production-firmware claims, customer platform ports, or unpublished SoC enablement
- Secrets, credentials, private clone URLs, or NDA material
- Generated Meson build directories or local toolchain files
- Force-pushes, history rewrites, or assistant self-approval of a pull request
