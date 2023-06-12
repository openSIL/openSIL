#!/usr/bin/env python
# SPDX-License-Identifier: MIT
# Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.

# This is a 'simple' python program to turn a python list into a file containing
# one list element per line.
# This is used in the Meson build to export the file lists and include dirs
# lists to text files for use by a Host environment to create their own make
# system lists.
# Parameters:
#   argv[1]  - output filename. Meson will construct the name so as to place the
#               file in the current build directory for the project
#   argv[2..N] - The python list elements exported from Meson
#

import os
import shutil
import sys

if __name__=='__main__':
  if len(sys.argv) >1 :
    outfilename = sys.argv[1]
    try:
        with open(outfilename, 'w') as outfile:
            try:
                outfile.write("# Output file from ListFiles.py\n")
            except (IOError, OSError):
                print("Error writing to file")
            else:
                outfile.write("# number Args: {}\n".format( len(sys.argv) ) )
                for parm in range(2, len(sys.argv)):
                    myfile = sys.argv[parm]
                    # Note: sysarg is full path of file
                    # Trim file path to make it relative to openSIL root
                    StrStart = myfile.lower().find("opensil")
                    # find returns start of string; add length of string to trim off root
                    if StrStart != -1:
                        myfile = myfile[StrStart+8: ]
                    outfile.write("    {}\n".format(myfile) )

    except (FileNotFoundError, PermissionError, OSError):
        print("Error opening file")

  else: # Not enough Args
    exit(1)
