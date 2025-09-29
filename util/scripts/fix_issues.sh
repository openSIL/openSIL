#!/usr/bin/env bash

# SPDX-License-Identifier: MIT
# Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved
# Dependencies: unix2dos (apt install dos2unix) or
#               https://dos2unix.sourceforge.io/

# Description: Prepare files for upload to git by making sure files
#              have appropriate line endings, a final newline, and
#              no trailing whitespace.
#              Additionally, identifies source files with tabs.

if ! command -v unix2dos; then
  echo "Error: The tool 'unix2dos' is not found. Please make sure it's"
  echo "       installed and in your path."
  exit 1
fi

echo "Setting windows line endings, removing trailing whitespace, and"
echo "  adding final newlines where needed."

find . -type f | grep -v .git | while IFS= read -r file; do
  if file --mime-encoding "${file}" | grep -qv "binary"; then
    printf "."
    tail -c1 < "${file}" | read -r _ || printf "\n" >> "${file}"
    sed --in-place 's/[[:space:]]*$//' "${file}"
    if echo "${file}" | grep -qv "\.sh$"; then
      unix2dos "${file}" 2>/dev/null
    fi
  fi
done

printf "\nDone.\n\n"

echo "Looking for source files with tabs (represented by right arrow)"
find . -type f -name "*.c" -o -name "*.h" | grep -v .git | while IFS= read -r file; do
  if file --mime-encoding "${file}" | grep -qv "binary"; then
    grep -PHn "\t" "${file}" | sed 's/\t/--->/g'
  fi
done

printf "\nDone.\n"
