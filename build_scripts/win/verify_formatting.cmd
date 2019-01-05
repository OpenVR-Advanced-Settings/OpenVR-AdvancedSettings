@echo off
REM Arg 0 (called %0) is the full path to the file.
REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
REM Project dir is up two dirs from the file.
SET project_dir=%~dp0\..\..\

ECHO %current_activity%: Testing if clang-format can be located:

IF NOT DEFINED CLANG_FORMAT_LOC (
    ECHO %current_activity%: CLANG_FORMAT_LOC not defined. Using default value.
    SET CLANG_FORMAT_LOC="C:\Program Files\LLVM\bin\clang-format.exe"
)
ECHO %current_activity%: CLANG_FORMAT_LOC set to '%CLANG_FORMAT_LOC%'.

REM EXIST won't find items in the PATH. WHERE will.
SET CLANG_FORMAT_VALID=0
IF EXIST %CLANG_FORMAT_LOC% SET CLANG_FORMAT_VALID=1
IF %CLANG_FORMAT_VALID% EQU 0 (
    WHERE /Q %CLANG_FORMAT_LOC%
    IF NOT ERRORLEVEL 1 SET CLANG_FORMAT_VALID=1
)

IF %CLANG_FORMAT_VALID% EQU 1 (
    ECHO %current_activity%: CLANG_FORMAT_LOC exists.
) ELSE (
    ECHO %current_activity%: CLANG_FORMAT_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: Clang-format located.

%CLANG_FORMAT_LOC% --version

%PYTHON_LOC%python %project_dir%/build_scripts/run-clang-format.py %project_dir%/src -r --color never --clang-format-executable %CLANG_FORMAT_LOC%
IF NOT ERRORLEVEL 1 (
    ECHO FORMATTED CORRECTLY.
) ELSE (
    ECHO NOT FORMATTED CORRECTLY.
)