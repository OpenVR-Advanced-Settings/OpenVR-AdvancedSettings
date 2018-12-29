@echo off
REM Echo off to avoid spam.

REM Non zero is failure, 0 is success.
SET exit_code_success=0
SET exit_code_failure_build_apps=1
SET exit_code_failure_build_locations=2

REM Used for echos.
SET current_activity=FORMAT

REM If scripts are chain called the original dirs should not be overwritten.
IF NOT DEFINED original_dir (
    SET original_dir=%CD%
)
IF NOT DEFINED project_dir (
    REM Arg 0 (called %0) is the full path to the file.
    REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
    REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
    REM Project dir is up two dirs from the file.
    SET project_dir=%~dp0\..\..\
)

ECHO %current_activity%: Testing if clang-format can be located:

IF NOT DEFINED LLVM_LOC (
    ECHO %current_activity%: LLVM_LOC not defined. Using default value.
    SET LLVM_LOC="C:\Program Files\LLVM\bin\"
)
ECHO %current_activity%: LLVM_LOC set to '%LLVM_LOC%'.

REM EXIST won't find items in the PATH. WHERE will.
SET LLVM_VALID=0
IF EXIST %LLVM_LOC% SET LLVM_VALID=1
IF %LLVM_VALID% EQU 0 (
    WHERE /Q %LLVM_LOC%\clang-format.exe
    IF NOT ERRORLEVEL 1 SET LLVM_VALID=1
)

IF %LLVM_VALID% EQU 1 (
    ECHO %current_activity%: LLVM_LOC exists.
) ELSE (
    ECHO %current_activity%: LLVM_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: Clang-format located.

%LLVM_LOC%\clang-format.exe --version

REM First iteration.
FOR /R %project_dir%\src\ %%F IN (*.cpp *.h) DO (%LLVM_LOC%\clang-format.exe --style=file -i %%F & ECHO Iteration 1. %%F)

REM Clang-format is sometimes indecisive about which exact rules to apply,
REM leading to differences between first and second iteration. 
REM Do it twice, just to be sure.

FOR /R %project_dir%\src\ %%F IN (*.cpp *.h) DO (%LLVM_LOC%\clang-format.exe --style=file -i %%F & ECHO Iteration 2. %%F)

ECHO %current_activity%: %current_activity% DONE.
