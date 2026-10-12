---
description: "Review an openSIL pull request against its diff and the repository guidance"
---

# Review a pull request

Review an openSIL pull request using the repository's own guidance as the standard. This is read-only: do not edit files, push, approve, or merge. The result is evidence for a human reviewer.

## Inputs

- A pull request number, or a local branch to compare with its target branch (`genoa_poc` for this POC).

## Steps

1. Read the title, the description, and the full diff. With the GitHub CLI, use `gh pr view <number> --repo openSIL/openSIL` and `gh pr diff <number> --repo openSIL/openSIL`. For a local branch, use `git diff origin/genoa_poc...HEAD`.
2. Read `AGENTS.md` and every file in `.github/instructions/` whose `applyTo` pattern matches a changed file.
3. Read enough of each changed file around the diff to judge the change in context.
4. Check the change against these questions:
   - **Correctness:** Does the code do what the description says? Is every returned `SIL_STATUS` handled? Is every register structure read or initialized before it is written back? Do loops cover every instance, die, and socket?
   - **Build:** Is each new `.c` file in the nearest `meson.build`? Does Kconfig gating keep `or COVERITY_BUILD`? Would anything fail `-Werror`, `-Wmissing-prototypes`, or MSVC? Does anything change behavior between 32-bit and 64-bit builds, such as pointer casts, `sizeof`, or format specifiers?
   - **Sequencing:** Do changes to `IpBlkList*Tp*.c` keep dependency order and the `SilId_ListEnd` terminator?
   - **Configuration:** Do Kconfig changes come with regenerated `configs/Onyx_SilCfg` and `configs/Onyx_SilCfg.h`?
   - **Scope:** Is the change free of confidential information, production-firmware claims, and unrelated cleanup?
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
<Files read, guidance applied, and anything not verified.>
```
