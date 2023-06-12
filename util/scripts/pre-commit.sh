#!/usr/bin/env sh
# Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

retval=0

for filename in $(git diff --cached --name-only); do
  echo "Checking ${filename}"
  if ! python util/scripts/check_for_issues.py --disable-checks line_length -- "${filename}"; then
    retval=1
  fi
done

exit $retval
