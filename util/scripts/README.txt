SPDX-License-Identifier: MIT
Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.

check_for_issues.py
-------------------
  Language: Python
  Description: This script reads a text file and checks for the following:
  - All lines end with a carriage return and linefeed, unless the file is a
    shell script.
  - There are no tab characters in the file.
  - The file ends with a single newline character.
  - There are no spaces or tabs at the end of the line.
  - There are no non_ascii characters in the file.
  - Lines are fewer than 120 characters long.
  Dependencies:
    None

fix_issues.sh
-------------
  Language: Bash
  Description: Prepare files for upload to git by making sure files have
    appropriate line endings, a final newline, no trailing whitespace and no
    tabs.
  Dependencies: unix2dos (apt install dos2unix) or
               https://dos2unix.sourceforge.io/

pre-commit.sh
-------------
  Language: Shell
  Description: Runs checks before the git commit command is allowed to
    complete. If the script exits with a non-zero return code, the commit step
    is halted. This behavior can be overridden by the -n or --no-verify command
    line option.
    Copy the file to .git/hooks/pre-commit to enable it.
  Dependencies: check_for_issues.py
