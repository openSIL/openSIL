---
description: "Review an openSIL pull request against its diff and the repository guidance"
---

# Review a pull request

Review an openSIL pull request using the repository's own guidance as the standard. This is read-only: do not edit files, push, approve, or merge. The result is evidence for a human reviewer.

## Inputs

- A pull request number, or a local branch to compare with its target branch (`turin_poc` for this POC).

## Steps

1. Read the title, the description, and the full diff. With the GitHub CLI, use `gh pr view <number> --repo openSIL/openSIL` and `gh pr diff <number> --repo openSIL/openSIL`. For a local branch, use `git diff origin/turin_poc...HEAD`.
2. Read `AGENTS.md` and every file in `.github/instructions/` whose `applyTo` pattern matches a changed file.
3. Read enough of each changed file around the diff to judge the change in context.
4. Check the change against these questions:
   - **Correctness:** Does the code do what the description says? Is every returned `SIL_STATUS` handled? Is every register structure read or initialized before it is written back? Do loops cover every root bridge, die, and socket, using each instance's own segment and bus?
   - **Build:** Is each new `.c` file in the nearest `meson.build`? Does Kconfig gating keep `or COVERITY_BUILD`? Would anything fail C17 with `werror`, `-Wmissing-prototypes`, or MSVC? Does anything change behavior between 32-bit and 64-bit builds, such as pointer casts, `sizeof`, or format specifiers?
   - **Formatting:** Are changed C files formatted with `util/uncrustify/uncrustify.cfg`? Would `util/scripts/check_for_issues.py` pass (CRLF, ASCII, no tabs, no trailing whitespace, single final newline)?
   - **Sequencing:** Do changes to `IpBlkList*Tp*.c` keep dependency order and the `{SilId_ListEnd}` terminator?
   - **Configuration:** Do Kconfig changes come with regenerated `configs/Purico_SilCfg` and `configs/Purico_SilCfg.h`?
   - **Tests:** If the change touches code a `tests/HostTest` suite covers, is the suite updated, and does the description say how it was run?
   - **Scope:** Is the change free of confidential information, production-firmware claims, claims beyond the validation limits in `README.md`, and unrelated cleanup?
5. Report only findings you can point to in the diff or the tree. Give each one a file and line. Phrase anything you could not verify as a question.

## Output

```markdown
## Review: <pull request title>

### Summary
<What the change does, and the overall assessment in two or three sentences.>

### Must fix
- `path:line`: <problem>. <Why it matters.> <Suggested fix.>

### Should fix
- `path:line`: <problem and suggestion>

### Questions
- <Anything that needs the author's answer.>

### What was checked
<Files read, guidance applied, tests considered, and anything not verified.>
```
