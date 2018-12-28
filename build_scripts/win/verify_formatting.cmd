@echo off
REM Arg 0 (called %0) is the full path to the file.
REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
REM Project dir is up two dirs from the file.
SET project_dir=%~dp0\..\..\

python %project_dir%/build_scripts/run-clang-format.py %project_dir%/src -r --color never --clang-format-executable %CLANG_FORMAT_LOC%
IF NOT ERRORLEVEL 1 THEN ECHO FORMATTED CORRECTLY.