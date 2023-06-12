@echo off

REM Batch file to convert .docx (MS Word) to .md (Markdown for posting on github)
REM Leveraging pandoc: https://pandoc.org/

echo "Pandoc is required and available from: https://pandoc.org/"
echo "Input File: %1.docx"
IF %1=="" GOTO ERROR
IF NOT EXIST %1.docx GOTO ERROR

pandoc -t gfm -f docx -s --extract-media=%1 %1.docx -o %1.md
GOTO DONE

:ERROR
echo "GenMarkdown ERROR: Parameter invalid or missing"
echo "Usage: GenMarkdown <basefile>"
echo "   where <basefile> is name of file (without .docx extension)"
echo "   to be converted to markdown (.md) for usage on github"
echo.

:DONE
echo " Both .md file and folder containing media will be generated"
echo.
