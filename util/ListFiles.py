#!/usr/bin/env python

# Cross-platform mkdir command.

import os
import shutil
import sys

if __name__=='__main__':
  if len(sys.argv) >1 :
    outfilename = "flist-"+sys.argv[1]+".txt"
    outfile = open( outfilename, "a")
    outfile.write("Output file from ListFiles.py\n")
    outfile.write( "number Args: {}\n".format( len(sys.argv) ) )
    for parm in range(2, len(sys.argv)) :
        myfile = sys.argv[parm]
        outfile.write( "parm:  {}\n".format(myfile))
        StrStart = myfile.lower().find("opensil")
        if StrStart != -1:
            myfile = myfile[StrStart: ]
        outfile.write("File: {}\n".format(myfile) )

    outfile.close()
  else :
    exit(1)
