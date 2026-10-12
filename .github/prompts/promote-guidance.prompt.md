---
description: "Turn a recurring review or build finding into a proposed guidance update"
---

# Promote a finding into guidance

Use this when the same problem shows up more than once in reviews or builds, to turn it into the smallest durable guidance change. You propose the change; maintainers decide through a normal pull request.

## Inputs

- The finding and its evidence: links to the pull requests, review comments, or build logs where it occurred, and the fix that resolved it.

## Steps

1. Confirm that the problem is recurring and general. A one-off mistake, or an issue specific to one change, does not need guidance.
2. Check whether existing guidance already covers it: `AGENTS.md`, `.github/instructions/`, `.github/prompts/`, `README.md`, and `CONTRIBUTING.md`. If guidance exists but was missed, consider clearer wording before adding a new rule.
3. Choose the narrowest home:

   | If the lesson applies to | Put it in |
   | --- | --- |
   | One file type or directory | The matching `.github/instructions/*.instructions.md` |
   | A repeatable task, such as review or triage | The matching `.github/prompts/*.prompt.md` |
   | The whole repository | `AGENTS.md` |
   | Human contributors | `CONTRIBUTING.md` |

4. Write the rule in one or two sentences that say what to do and why, grounded in this tree: name the real function, file, or option. Avoid rules that cannot be checked.
5. Prepare the change as a small pull request whose description links the evidence. Do not merge it yourself.

## Output

- The proposed text and the file it belongs in.
- The evidence links.
- A one-paragraph rationale.
