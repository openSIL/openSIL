#!/usr/bin/env -S python3 #

"""
 SPDX-License-Identifier: MIT
 Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.

 Dependencies:
  None

 Description:
  This script reads a text file and checks for the following:
  - All lines end with a carriage return and linefeed, unless the file is a shell script.
  - There are no tab characters in the file unless it is a Makefile.
  - The file ends with a single newline character.
  - There are no spaces or tabs at the end of the line.
  - There are no non_ascii characters in the file.
  - Lines are shorter than 120 characters long.
  - A maximum of 5 errors are reported for *each type* of error.
  - The script will exit with a status of 1 if any errors are found.
"""

import argparse
import sys
import os

# This class holds the flags for enabling/disabling checks
class CheckFlags:
    """ checks being run """
    def __init__(self):
        self.check_crlf = True
        self.check_tabs = True
        self.check_newline = True
        self.check_trailing_whitespace = True
        self.check_non_ascii = True
        self.check_line_length = True

MAX_LINE_LENGTH = 120
MAX_ERRORS = 5

# Print an error message
def print_error(filename, line_no, error_message):
    """ Print the error """
    if not args.quiet:
        print(f"{filename}:{line_no} - {error_message}")

# Print when the error count has exceeded its max value.
def max_errors_reached(filename, error_type):
    """ Print when we've stopped printing errors """
    if not args.quiet:
        print(f"{filename}: More than {MAX_ERRORS} {error_type} errors. Halting reporting.")

# Create a parser to handle command-line arguments
parser = argparse.ArgumentParser(description='Validate a text file.')
parser.add_argument('filename', help='The name of the file to validate.')
parser.add_argument('-d', '--disable-checks', nargs='+', choices=['crlf', 'tabs', 'newline', 'trailing_whitespace', \
                                                                  'non_ascii', 'line_length'],
                    help='Disable specific checks. Valid options are crlf, tabs, newline, trailing_whitespace, \
                        non_ascii, line_length')
parser.add_argument('-q', '--quiet', action='store_true', help='Suppress output from the script.')

# Parse the arguments
args = parser.parse_args()

# Create a CheckFlags object to hold the enabled/disabled checks
check_flags = CheckFlags()

# Disable checks specified in the command-line arguments
if args.disable_checks:  # Check if the list exists and isn't empty
    for disabled_check in args.disable_checks:
        setattr(check_flags, f"check_{disabled_check}", False)

# Check if the file exists - If not, exit successfully
if not os.path.isfile(args.filename):
    sys.exit(0)

# Check if the file is a text file - If not, exit successfully
with open(args.filename, 'rb') as f:
    file_start = f.read(1024)
    if b'\x00' in file_start:
        sys.exit(0)

# Check the extension to see if this is a shell script
if args.filename[-3:] == ".sh":
    check_flags.check_crlf = False

# If the file is a makefile, disable tab checks
if args.filename[8:] == "Makefile":
    check_flags.check_tabs = False

# Open the file and read the lines
with open(args.filename, 'r' , encoding="UTF-8") as f:
    lines = f.readlines()

# Init error counters
error_count = {
    'crlf': 0,
    'tabs': 0,
    'trailing_whitespace': 0,
    'non_ascii': 0,
    'line_length': 0,
    'total': 0,
}

# Look for errors in file
for line_number, line in enumerate(lines, start=1):

    # Make a copy with no line endings
    bareline = line.rstrip('\r\n')

    # Check for tab characters
    if check_flags.check_tabs and '\t' in line:
        print_error(args.filename, line_number, "Line contains a tab character.")
        error_count['tabs'] += 1
        error_count['total'] += 1
        if error_count['tabs'] >= MAX_ERRORS:
            check_flags.check_tabs = False
            max_errors_reached(args.filename, "tab char")

    # Check for ending newline character
    if check_flags.check_newline and not line.endswith('\n'):
        print_error(args.filename, line_number, "Line does not end with a newline character.")
        error_count['total'] += 1

    # Check for trailing whitespace
    if check_flags.check_trailing_whitespace and  (bareline.endswith(' ') or bareline.endswith('\t')):
        print_error(args.filename, line_number, "Line ends with trailing whitespace.")
        error_count['trailing_whitespace'] += 1
        error_count['total'] += 1
        if error_count['trailing_whitespace'] >= MAX_ERRORS:
            check_flags.check_trailing_whitespace = False
            max_errors_reached(args.filename, "Trailing whitespace")

    # Check for non_ascii characters
    if check_flags.check_non_ascii and not all(ord(c) < 128 for c in line):
        print_error(args.filename, line_number, "Line contains non_ascii characters.")
        error_count['total'] += 1
        error_count['non_ascii'] += 1
        if error_count['non_ascii'] >= MAX_ERRORS:
            check_flags.check_non_ascii = False
            max_errors_reached(args.filename, "non_ascii character")

    # Check for line length
    if check_flags.check_line_length and len(bareline) > MAX_LINE_LENGTH:
        print_error(args.filename, line_number, "Line is longer than " + str(MAX_LINE_LENGTH) + " characters.")
        error_count['line_length'] += 1
        error_count['total'] += 1
        if error_count['line_length'] >= MAX_ERRORS:
            check_flags.check_line_length = False
            max_errors_reached(args.filename, "Line length")

# Open the file and read the lines as binary so we don't lose newline characters
with open(args.filename, 'rb') as file:
    for line_number, line in enumerate(file):

        # Check for a shebang line to see if this is a shell script
        if line_number == 0 and line.startswith(b"#!") and line.endswith(b"sh\n"):
            check_flags.check_crlf = False

        # Check for carriage return and linefeed
        if check_flags.check_crlf and not line.endswith(b'\r\n'):
            print_error(args.filename, line_number +1 , "Line does not end with CRLF")
            error_count['crlf'] += 1
            error_count['total'] += 1
            if error_count['crlf'] >= MAX_ERRORS:
                check_flags.check_crlf = False
                max_errors_reached(args.filename, "Line ending")

if error_count['total'] > 0:
    sys.exit(1)
